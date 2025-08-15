package com.chtl.compiler.chtljs;

import com.chtl.model.CodeFragment;
import com.chtl.model.CompilationResult;
import com.chtl.model.FragmentType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * CHTL JS编译器 - 手写实现
 * 负责编译包含CHTL JS特有语法的JavaScript代码
 */
public class CHTLJSCompiler {
    private static final Logger logger = LoggerFactory.getLogger(CHTLJSCompiler.class);
    
    // 正则表达式模式
    private static final Pattern ENHANCED_SELECTOR_PATTERN = Pattern.compile(
        "\\{\\{([^}]+)\\}\\}", Pattern.MULTILINE
    );
    
    private static final Pattern ARROW_OPERATOR_PATTERN = Pattern.compile(
        "(\\w+|\\))\\s*->\\s*(\\w+)", Pattern.MULTILINE
    );
    
    private static final Pattern LISTEN_METHOD_PATTERN = Pattern.compile(
        "->\\s*listen\\s*\\(\\s*\\{([^}]*)\\}\\s*\\)", Pattern.DOTALL
    );
    
    private static final Pattern DELEGATE_METHOD_PATTERN = Pattern.compile(
        "->\\s*delegate\\s*\\(\\s*\\{([^}]*)\\}\\s*\\)", Pattern.DOTALL
    );
    
    private static final Pattern ANIMATE_FUNCTION_PATTERN = Pattern.compile(
        "animate\\s*\\(\\s*\\{([^}]*)\\}\\s*\\)", Pattern.DOTALL
    );
    
    // 事件委托注册表管理器
    private final EventDelegationManager delegationManager;
    
    public CHTLJSCompiler() {
        this.delegationManager = new EventDelegationManager();
        logger.info("CHTL JS编译器初始化完成");
    }
    
    /**
     * 编译CHTL JS代码片段
     */
    public CompilationResult compile(CodeFragment fragment) {
        logger.debug("开始编译CHTL JS代码");
        
        try {
            String jsCode = fragment.getContent();
            
            // 1. 处理增强选择器 {{}}
            jsCode = processEnhancedSelectors(jsCode);
            
            // 2. 处理箭头操作符 ->
            jsCode = processArrowOperators(jsCode);
            
            // 3. 处理listen方法
            jsCode = processListenMethod(jsCode);
            
            // 4. 处理delegate方法
            jsCode = processDelegateMethod(jsCode);
            
            // 5. 处理animate函数
            jsCode = processAnimateFunction(jsCode);
            
            // 6. 添加必要的辅助函数
            String finalCode = wrapWithHelperFunctions(jsCode);
            
            CompilationResult result = new CompilationResult(FragmentType.CHTL_JS, finalCode);
            
            logger.debug("CHTL JS编译成功");
            return result;
            
        } catch (Exception e) {
            logger.error("CHTL JS编译失败", e);
            return new CompilationResult(FragmentType.CHTL_JS, "", e.getMessage());
        }
    }
    
    /**
     * 处理增强选择器 {{}}
     */
    private String processEnhancedSelectors(String code) {
        Matcher matcher = ENHANCED_SELECTOR_PATTERN.matcher(code);
        StringBuffer result = new StringBuffer();
        
        while (matcher.find()) {
            String selector = matcher.group(1).trim();
            String replacement = convertEnhancedSelector(selector);
            matcher.appendReplacement(result, replacement);
        }
        
        matcher.appendTail(result);
        return result.toString();
    }
    
    /**
     * 转换增强选择器为JavaScript代码
     */
    private String convertEnhancedSelector(String selector) {
        // 处理索引访问 如 button[0]
        if (selector.contains("[") && selector.contains("]")) {
            int bracketIndex = selector.indexOf("[");
            String baseSelector = selector.substring(0, bracketIndex);
            String index = selector.substring(bracketIndex + 1, selector.indexOf("]"));
            
            return String.format("_chtl.select('%s')[%s]", baseSelector, index);
        }
        
        // 处理类选择器
        if (selector.startsWith(".")) {
            return String.format("_chtl.selectAll('%s')", selector);
        }
        
        // 处理ID选择器
        if (selector.startsWith("#")) {
            return String.format("_chtl.selectOne('%s')", selector);
        }
        
        // 处理复合选择器（如 .box button）
        if (selector.contains(" ")) {
            return String.format("_chtl.selectAll('%s')", selector);
        }
        
        // 默认：先尝试作为标签，然后尝试类名/ID
        return String.format("_chtl.smartSelect('%s')", selector);
    }
    
    /**
     * 处理箭头操作符 ->
     */
    private String processArrowOperators(String code) {
        // 简单替换 -> 为 .
        return code.replaceAll("->", ".");
    }
    
    /**
     * 处理listen方法
     */
    private String processListenMethod(String code) {
        Matcher matcher = LISTEN_METHOD_PATTERN.matcher(code);
        StringBuffer result = new StringBuffer();
        
        while (matcher.find()) {
            String listenContent = matcher.group(1);
            String replacement = convertListenMethod(listenContent);
            matcher.appendReplacement(result, replacement);
        }
        
        matcher.appendTail(result);
        return result.toString();
    }
    
    /**
     * 转换listen方法
     */
    private String convertListenMethod(String content) {
        StringBuilder js = new StringBuilder(".addEventListener(function() {\n");
        js.append("  const events = {\n");
        
        // 解析事件映射
        String[] lines = content.split(",");
        for (String line : lines) {
            String trimmed = line.trim();
            if (trimmed.contains(":")) {
                String[] parts = trimmed.split(":", 2);
                String event = parts[0].trim();
                String handler = parts[1].trim();
                
                js.append(String.format("    '%s': %s,\n", event, handler));
            }
        }
        
        js.append("  };\n");
        js.append("  for (let event in events) {\n");
        js.append("    this.addEventListener(event, events[event]);\n");
        js.append("  }\n");
        js.append("}.call(this))");
        
        return js.toString();
    }
    
    /**
     * 处理delegate方法
     */
    private String processDelegateMethod(String code) {
        Matcher matcher = DELEGATE_METHOD_PATTERN.matcher(code);
        StringBuffer result = new StringBuffer();
        
        while (matcher.find()) {
            String delegateContent = matcher.group(1);
            String replacement = convertDelegateMethod(delegateContent);
            matcher.appendReplacement(result, replacement);
        }
        
        matcher.appendTail(result);
        return result.toString();
    }
    
    /**
     * 转换delegate方法
     */
    private String convertDelegateMethod(String content) {
        Map<String, String> config = parseDelegateConfig(content);
        
        StringBuilder js = new StringBuilder();
        js.append("._chtlDelegate({\n");
        js.append("  parent: this,\n");
        
        for (Map.Entry<String, String> entry : config.entrySet()) {
            js.append(String.format("  %s: %s,\n", entry.getKey(), entry.getValue()));
        }
        
        js.append("})");
        
        return js.toString();
    }
    
    /**
     * 解析delegate配置
     */
    private Map<String, String> parseDelegateConfig(String content) {
        Map<String, String> config = new HashMap<>();
        
        // 简化解析，实际应该更复杂
        String[] lines = content.split(",");
        for (String line : lines) {
            String trimmed = line.trim();
            if (trimmed.contains(":")) {
                String[] parts = trimmed.split(":", 2);
                config.put(parts[0].trim(), parts[1].trim());
            }
        }
        
        return config;
    }
    
    /**
     * 处理animate函数
     */
    private String processAnimateFunction(String code) {
        Matcher matcher = ANIMATE_FUNCTION_PATTERN.matcher(code);
        StringBuffer result = new StringBuffer();
        
        while (matcher.find()) {
            String animateContent = matcher.group(1);
            String replacement = convertAnimateFunction(animateContent);
            matcher.appendReplacement(result, replacement);
        }
        
        matcher.appendTail(result);
        return result.toString();
    }
    
    /**
     * 转换animate函数
     */
    private String convertAnimateFunction(String content) {
        StringBuilder js = new StringBuilder("_chtl.animate({\n");
        js.append(content);
        js.append("\n})");
        
        return js.toString();
    }
    
    /**
     * 包装代码并添加辅助函数
     */
    private String wrapWithHelperFunctions(String code) {
        StringBuilder wrapped = new StringBuilder();
        
        // 添加CHTL运行时库
        wrapped.append("// CHTL JS Runtime\n");
        wrapped.append("(function() {\n");
        wrapped.append("  'use strict';\n\n");
        
        // 添加辅助对象
        wrapped.append(generateCHTLHelperObject());
        
        wrapped.append("\n  // User code\n");
        wrapped.append("  " + code.replaceAll("\n", "\n  "));
        
        wrapped.append("\n})();\n");
        
        return wrapped.toString();
    }
    
    /**
     * 生成CHTL辅助对象
     */
    private String generateCHTLHelperObject() {
        return """
            const _chtl = {
              selectOne: function(selector) {
                return document.querySelector(selector);
              },
              
              selectAll: function(selector) {
                return document.querySelectorAll(selector);
              },
              
              select: function(selector) {
                return document.querySelectorAll(selector);
              },
              
              smartSelect: function(selector) {
                // 先尝试作为标签
                let elements = document.getElementsByTagName(selector);
                if (elements.length > 0) return elements;
                
                // 再尝试作为类名
                elements = document.getElementsByClassName(selector);
                if (elements.length > 0) return elements;
                
                // 最后尝试作为ID
                const element = document.getElementById(selector);
                if (element) return [element];
                
                return [];
              },
              
              animate: function(config) {
                const { duration = 1000, easing = 'ease-in-out', begin = {}, 
                        end = {}, when = [], loop = 1, callback } = config;
                
                let currentLoop = 0;
                const animate = () => {
                  if (loop !== -1 && currentLoop >= loop) {
                    if (callback) callback();
                    return;
                  }
                  
                  // 使用requestAnimationFrame实现动画
                  const startTime = performance.now();
                  
                  const frame = (currentTime) => {
                    const elapsed = currentTime - startTime;
                    const progress = Math.min(elapsed / duration, 1);
                    
                    // 应用动画样式
                    // 这里简化处理，实际应该更复杂
                    
                    if (progress < 1) {
                      requestAnimationFrame(frame);
                    } else {
                      currentLoop++;
                      if (loop === -1 || currentLoop < loop) {
                        animate();
                      } else if (callback) {
                        callback();
                      }
                    }
                  };
                  
                  requestAnimationFrame(frame);
                };
                
                animate();
              },
              
              _delegationRegistry: new Map(),
              
              _chtlDelegate: function(config) {
                const { parent, target, ...events } = config;
                
                // 为父元素创建委托处理器
                const key = parent;
                if (!this._delegationRegistry.has(key)) {
                  this._delegationRegistry.set(key, new Map());
                }
                
                const registry = this._delegationRegistry.get(key);
                
                // 注册事件
                for (let eventType in events) {
                  if (!registry.has(eventType)) {
                    parent.addEventListener(eventType, (e) => {
                      const handlers = registry.get(eventType);
                      if (handlers) {
                        handlers.forEach((handler, selector) => {
                          if (e.target.matches(selector)) {
                            handler.call(e.target, e);
                          }
                        });
                      }
                    });
                    registry.set(eventType, new Map());
                  }
                  
                  const targetSelectors = Array.isArray(target) ? target : [target];
                  targetSelectors.forEach(selector => {
                    registry.get(eventType).set(selector, events[eventType]);
                  });
                }
              }
            };
            
            // 扩展Element原型
            Element.prototype._chtlDelegate = function(config) {
              config.parent = this;
              _chtl._chtlDelegate(config);
              return this;
            };
            """;
    }
}