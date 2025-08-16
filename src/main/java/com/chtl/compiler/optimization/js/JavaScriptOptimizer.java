package com.chtl.compiler.optimization.js;

import com.chtl.compiler.optimization.JsFile;
import com.chtl.compiler.optimization.JsOptions;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import com.chtl.context.CompilationContext;
import java.util.*;

/**
 * JavaScript优化器
 * 负责合并和优化JavaScript输出
 * 特别注意保护CHTL JS运行时代码
 * 严格遵循CHTL语法文档
 */
public class JavaScriptOptimizer {
    private final CompilationContext context;
    private final JsOptions options;
    
    // CHTL JS运行时函数名（不能被混淆）
    private static final Set<String> CHTL_RUNTIME_FUNCTIONS = Set.of(
        "_chtl_select",
        "listen",
        "delegate",
        "animate"
    );
    
    public JavaScriptOptimizer(CompilationContext context, JsOptions options) {
        this.context = context;
        this.options = options;
    }
    
    /**
     * 优化JavaScript文件列表
     */
    public String optimize(List<JsFile> jsFiles) {
        StringBuilder optimized = new StringBuilder();
        
        // 1. 先处理CHTL JS运行时（始终在最前，不混淆）
        for (JsFile file : jsFiles) {
            if (file.isRuntime()) {
                String runtimeCode = file.getContent();
                
                // 运行时代码仅做基本优化
                if (options.isRemoveComments()) {
                    runtimeCode = removeComments(runtimeCode);
                }
                if (options.isRemoveWhitespace()) {
                    runtimeCode = removeWhitespace(runtimeCode);
                }
                
                optimized.append(runtimeCode);
                optimized.append("\n\n");
            }
        }
        
        // 2. 处理其他JavaScript文件
        for (JsFile file : jsFiles) {
            if (!file.isRuntime()) {
                String code = file.getContent();
                
                // 应用所有优化
                code = optimizeJavaScript(code);
                
                // 使用IIFE包装
                optimized.append("/* Source: ").append(file.getSourcePath()).append(" */\n");
                optimized.append("(function(){\n'use strict';\n");
                optimized.append(code);
                optimized.append("\n})();\n\n");
            }
        }
        
        return optimized.toString();
    }
    
    /**
     * 优化JavaScript代码
     */
    private String optimizeJavaScript(String js) {
        String optimized = js;
        
        // 1. 移除注释
        if (options.isRemoveComments()) {
            optimized = removeComments(optimized);
        }
        
        // 2. 移除死代码
        if (options.isRemoveDeadCode()) {
            optimized = removeDeadCode(optimized);
        }
        
        // 3. 优化表达式
        if (options.isOptimizeExpressions()) {
            optimized = optimizeExpressions(optimized);
        }
        
        // 4. 内联常量
        if (options.isInlineConstants()) {
            optimized = inlineConstants(optimized);
        }
        
        // 5. 混淆变量（谨慎使用）
        if (options.isMangleVariables()) {
            optimized = mangleVariables(optimized);
        }
        
        // 6. 移除空白
        if (options.isRemoveWhitespace()) {
            optimized = removeWhitespace(optimized);
        }
        
        return optimized;
    }
    
    /**
     * 移除JavaScript注释
     */
    private String removeComments(String js) {
        // 移除单行注释（保留包含sourceURL的注释）
        js = js.replaceAll("//(?!.*[@#]\\s*source(?:URL|MappingURL))[^\n]*", "");
        
        // 移除多行注释
        js = js.replaceAll("/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/", "");
        
        return js;
    }
    
    /**
     * 移除死代码
     */
    private String removeDeadCode(String js) {
        // 移除未使用的函数（简单实现）
        // 注意：这是一个简化的实现，实际的死代码删除需要更复杂的AST分析
        
        // 移除 if(false) 块
        js = js.replaceAll("if\\s*\\(\\s*false\\s*\\)\\s*\\{[^}]*\\}", "");
        
        // 移除 if(true) 的条件，保留内容
        js = js.replaceAll("if\\s*\\(\\s*true\\s*\\)\\s*\\{([^}]*)\\}", "$1");
        
        return js;
    }
    
    /**
     * 优化表达式
     */
    private String optimizeExpressions(String js) {
        // true === x -> x === true
        js = js.replaceAll("true\\s*===\\s*([a-zA-Z_$][a-zA-Z0-9_$]*)", "$1 === true");
        js = js.replaceAll("false\\s*===\\s*([a-zA-Z_$][a-zA-Z0-9_$]*)", "$1 === false");
        
        // !!(x) -> Boolean(x)
        js = js.replaceAll("!!\\(([^)]+)\\)", "Boolean($1)");
        
        // undefined === x -> x === void 0
        js = js.replaceAll("undefined\\s*===\\s*([a-zA-Z_$][a-zA-Z0-9_$]*)", "$1 === void 0");
        
        return js;
    }
    
    /**
     * 内联常量
     */
    private String inlineConstants(String js) {
        // 查找 const 声明
        Pattern constPattern = Pattern.compile("const\\s+([a-zA-Z_$][a-zA-Z0-9_$]*)\\s*=\\s*([^;]+);");
        Matcher matcher = constPattern.matcher(js);
        
        Map<String, String> constants = new HashMap<Object, Object>();
        
        while (matcher.find()) {
            String name = matcher.group(1);
            String value = matcher.group(2).trim();
            
            // 只内联简单的常量（数字、字符串、布尔值）
            if (value.matches("\\d+|\"[^\"]*\"|'[^']*'|true|false")) {
                constants.put(name, value);
            }
        }
        
        // 替换常量引用
        for (Map.Entry<String, String> entry : constants.entrySet()) {
            String name = entry.getKey();
            String value = entry.getValue();
            
            // 确保不是属性访问
            js = js.replaceAll("\\b" + name + "\\b(?!\\s*:)", value);
        }
        
        return js;
    }
    
    /**
     * 混淆变量名
     */
    private String mangleVariables(String js) {
        // 简单的变量混淆实现
        // 注意：实际使用需要更复杂的作用域分析
        
        if (!options.isMangleVariables()) {
            return js;
        }
        
        // 提取所有局部变量声明
        Pattern varPattern = Pattern.compile("\\b(var|let|const)\\s+([a-zA-Z_$][a-zA-Z0-9_$]*)");
        Matcher matcher = varPattern.matcher(js);
        
        Map<String, String> varMap = new HashMap<Object, Object>();
        int counter = 0;
        
        while (matcher.find()) {
            String varName = matcher.group(2);
            
            // 跳过CHTL运行时函数
            if (!CHTL_RUNTIME_FUNCTIONS.contains(varName) && !varMap.containsKey(varName)) {
                varMap.put(varName, generateShortName(counter++));
            }
        }
        
        // 替换变量名
        String result = js;
        for (Map.Entry<String, String> entry : varMap.entrySet()) {
            String original = entry.getKey();
            String mangled = entry.getValue();
            
            // 替换变量声明和使用
            result = result.replaceAll("\\b" + original + "\\b", mangled);
        }
        
        return result;
    }
    
    /**
     * 生成短变量名
     */
    private String generateShortName(int index) {
        String chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        if (index < chars.length()) {
            return String.valueOf(chars.charAt(index));
        }
        
        // 多字符变量名
        return chars.charAt(index % chars.length()) + String.valueOf(index / chars.length());
    }
    
    /**
     * 移除多余空白
     */
    private String removeWhitespace(String js) {
        // 保护字符串内容
        List<String> strings = new ArrayList<Object>();
        Pattern stringPattern = Pattern.compile("(['\"])(?:(?=(\\\\?))\\2.)*?\\1");
        Matcher stringMatcher = stringPattern.matcher(js);
        
        StringBuffer temp = new StringBuffer();
        int stringIndex = 0;
        
        while (stringMatcher.find()) {
            strings.add(stringMatcher.group());
            stringMatcher.appendReplacement(temp, "___STRING_" + stringIndex + "___");
            stringIndex++;
        }
        stringMatcher.appendTail(temp);
        js = temp.toString();
        
        // 移除多余空白
        js = js.replaceAll("\\s+", " ");
        js = js.replaceAll("\\s*([{}()\\[\\];,:<>!~*/%+-])\\s*", "$1");
        
        // 恢复必要的空格
        js = js.replaceAll("\\b(var|let|const|function|return|if|else|for|while|do|switch|case|break|continue|throw|try|catch|finally|class|extends|new|typeof|instanceof|in|of)([^\\s])", "$1 $2");
        
        // 恢复字符串
        for (int i = 0; i < strings.size(); i++) {
            js = js.replace("___STRING_" + i + "___", strings.get(i));
        }
        
        return js.trim();
    }
}
