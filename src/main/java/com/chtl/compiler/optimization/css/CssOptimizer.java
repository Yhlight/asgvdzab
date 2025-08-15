package com.chtl.compiler.optimization.css;

import com.chtl.context.CompilationContext;
import com.chtl.compiler.optimization.OptimizationOptions.CssOptions;
import com.chtl.compiler.optimization.OutputOptimizer.CssFile;

import java.util.*;
import java.util.regex.*;

/**
 * CSS优化器
 * 负责合并和优化CSS输出
 * 严格遵循CHTL语法文档
 */
public class CssOptimizer {
    private final CompilationContext context;
    private final CssOptions options;
    
    // CSS规则缓存（用于合并相同选择器）
    private final Map<String, List<String>> selectorRules = new LinkedHashMap<>();
    
    public CssOptimizer(CompilationContext context, CssOptions options) {
        this.context = context;
        this.options = options;
    }
    
    /**
     * 优化CSS文件列表
     */
    public String optimize(List<CssFile> cssFiles) {
        // 1. 合并所有CSS文件
        String merged = mergeCssFiles(cssFiles);
        
        // 2. 解析CSS规则
        if (options.isMergeRules()) {
            merged = mergeRules(merged);
        }
        
        // 3. 优化选择器
        if (options.isOptimizeSelectors()) {
            merged = optimizeSelectors(merged);
        }
        
        // 4. 移除注释
        if (options.isRemoveComments()) {
            merged = removeComments(merged);
        }
        
        // 5. 优化颜色值
        if (options.isShortenColors()) {
            merged = shortenColors(merged);
        }
        
        // 6. 优化数值
        if (options.isShortenValues()) {
            merged = shortenValues(merged);
        }
        
        // 7. 移除空白
        if (options.isRemoveWhitespace()) {
            merged = removeWhitespace(merged);
        }
        
        return merged;
    }
    
    /**
     * 合并CSS文件
     */
    private String mergeCssFiles(List<CssFile> cssFiles) {
        StringBuilder merged = new StringBuilder();
        
        // 先处理全局样式
        for (CssFile file : cssFiles) {
            if (file.isGlobal()) {
                if (merged.length() > 0) {
                    merged.append("\n\n");
                }
                merged.append("/* Global styles from: ").append(file.getSourcePath()).append(" */\n");
                merged.append(file.getContent());
            }
        }
        
        // 再处理局部样式
        for (CssFile file : cssFiles) {
            if (!file.isGlobal()) {
                if (merged.length() > 0) {
                    merged.append("\n\n");
                }
                merged.append("/* Local styles from: ").append(file.getSourcePath()).append(" */\n");
                merged.append(file.getContent());
            }
        }
        
        return merged.toString();
    }
    
    /**
     * 合并相同选择器的规则
     */
    private String mergeRules(String css) {
        // 解析CSS规则
        Pattern rulePattern = Pattern.compile("([^{}]+)\\{([^}]+)\\}", Pattern.MULTILINE);
        Matcher matcher = rulePattern.matcher(css);
        
        // 收集规则
        while (matcher.find()) {
            String selector = matcher.group(1).trim();
            String properties = matcher.group(2).trim();
            
            selectorRules.computeIfAbsent(selector, k -> new ArrayList<>())
                        .add(properties);
        }
        
        // 重建CSS
        StringBuilder optimized = new StringBuilder();
        for (Map.Entry<String, List<String>> entry : selectorRules.entrySet()) {
            String selector = entry.getKey();
            List<String> propsList = entry.getValue();
            
            optimized.append(selector).append("{");
            
            // 合并属性，后面的覆盖前面的
            Map<String, String> mergedProps = new LinkedHashMap<>();
            for (String props : propsList) {
                String[] declarations = props.split(";");
                for (String decl : declarations) {
                    String trimmed = decl.trim();
                    if (!trimmed.isEmpty()) {
                        int colonIndex = trimmed.indexOf(':');
                        if (colonIndex > 0) {
                            String property = trimmed.substring(0, colonIndex).trim();
                            String value = trimmed.substring(colonIndex + 1).trim();
                            mergedProps.put(property, value);
                        }
                    }
                }
            }
            
            // 输出合并后的属性
            boolean first = true;
            for (Map.Entry<String, String> prop : mergedProps.entrySet()) {
                if (!first) optimized.append(";");
                optimized.append(prop.getKey()).append(":").append(prop.getValue());
                first = false;
            }
            
            optimized.append("}\n");
        }
        
        return optimized.toString();
    }
    
    /**
     * 优化选择器
     */
    private String optimizeSelectors(String css) {
        // 简化选择器（移除不必要的空格）
        css = css.replaceAll("\\s*>\\s*", ">");
        css = css.replaceAll("\\s*\\+\\s*", "+");
        css = css.replaceAll("\\s*~\\s*", "~");
        css = css.replaceAll("\\s*,\\s*", ",");
        
        return css;
    }
    
    /**
     * 移除CSS注释
     */
    private String removeComments(String css) {
        return css.replaceAll("/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/", "");
    }
    
    /**
     * 缩短颜色值
     */
    private String shortenColors(String css) {
        // #RRGGBB -> #RGB (如果可能)
        Pattern hexPattern = Pattern.compile("#([0-9a-fA-F])\\1([0-9a-fA-F])\\2([0-9a-fA-F])\\3");
        css = hexPattern.matcher(css).replaceAll("#$1$2$3");
        
        // rgb(255,255,255) -> #fff
        Pattern rgbPattern = Pattern.compile("rgb\\s*\\(\\s*(\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*\\)");
        Matcher rgbMatcher = rgbPattern.matcher(css);
        
        StringBuffer result = new StringBuffer();
        while (rgbMatcher.find()) {
            int r = Integer.parseInt(rgbMatcher.group(1));
            int g = Integer.parseInt(rgbMatcher.group(2));
            int b = Integer.parseInt(rgbMatcher.group(3));
            String hex = String.format("#%02x%02x%02x", r, g, b);
            
            // 尝试缩短
            if (hex.matches("#([0-9a-f])\\1([0-9a-f])\\2([0-9a-f])\\3")) {
                hex = "#" + hex.charAt(1) + hex.charAt(3) + hex.charAt(5);
            }
            
            rgbMatcher.appendReplacement(result, hex);
        }
        rgbMatcher.appendTail(result);
        
        return result.toString();
    }
    
    /**
     * 缩短数值
     */
    private String shortenValues(String css) {
        // 移除0值的单位 (0px -> 0)
        css = css.replaceAll("\\b0(?:px|em|%|in|cm|mm|pc|pt|ex)", "0");
        
        // 移除前导0 (0.5 -> .5)
        css = css.replaceAll("\\b0+\\.(\\d+)", ".$1");
        
        // 移除不必要的小数点后的0
        css = css.replaceAll("(\\d)\\.0+(?!\\d)", "$1");
        
        return css;
    }
    
    /**
     * 移除多余空白
     */
    private String removeWhitespace(String css) {
        // 移除多余空格
        css = css.replaceAll("\\s+", " ");
        
        // 移除规则前后的空格
        css = css.replaceAll("\\s*:\\s*", ":");
        css = css.replaceAll("\\s*;\\s*", ";");
        css = css.replaceAll("\\s*\\{\\s*", "{");
        css = css.replaceAll("\\s*\\}\\s*", "}");
        
        // 移除最后的分号
        css = css.replaceAll(";}", "}");
        
        return css.trim();
    }
}