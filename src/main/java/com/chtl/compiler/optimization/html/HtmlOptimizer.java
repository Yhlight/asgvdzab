package com.chtl.compiler.optimization.html;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


import com.chtl.context.CompilationContext;
import com.chtl.compiler.optimization.OptimizationOptions.HtmlOptions;

import java.util.regex.*;

/**
 * HTML优化器
 * 负责压缩和优化HTML输出
 * 严格遵循CHTL语法文档
 */
public class HtmlOptimizer {
    private final CompilationContext context;
    private final HtmlOptions options;
    
    // 预编译的正则表达式
    private static final Pattern COMMENT_PATTERN = Pattern.compile("<!--[^>]*?-->");
    private static final Pattern WHITESPACE_PATTERN = Pattern.compile("\\s+");
    private static final Pattern INTER_TAG_WHITESPACE = Pattern.compile(">\\s+<");
    private static final Pattern EMPTY_ATTR_PATTERN = Pattern.compile("\\s+(\\w+)=(['\"])\\2");
    private static final Pattern BOOLEAN_ATTR_PATTERN = Pattern.compile("\\s+(checked|disabled|selected|readonly|multiple|defer|async)=(['\"])\\1\\2", Pattern.CASE_INSENSITIVE);
    
    public HtmlOptimizer(CompilationContext context, HtmlOptions options) {
        this.context = context;
        this.options = options;
    }
    
    /**
     * 优化HTML
     */
    public String optimize(String html) {
        if (html == null || html.isEmpty()) {
            return html;
        }
        
        String optimized = html;
        
        // 1. 移除注释
        if (options.isRemoveComments()) {
            optimized = removeComments(optimized);
        }
        
        // 2. 处理空白
        if (options.isRemoveWhitespace()) {
            optimized = removeWhitespace(optimized);
        }
        
        // 3. 折叠空白
        if (options.isCollapseWhitespace()) {
            optimized = collapseWhitespace(optimized);
        }
        
        // 4. 移除空属性
        if (options.isRemoveEmptyAttributes()) {
            optimized = removeEmptyAttributes(optimized);
        }
        
        // 5. 移除冗余属性
        if (options.isRemoveRedundantAttributes()) {
            optimized = removeRedundantAttributes(optimized);
        }
        
        // 6. 压缩内联样式
        if (options.isMinifyInlineStyles()) {
            optimized = minifyInlineStyles(optimized);
        }
        
        // 7. 压缩内联脚本
        if (options.isMinifyInlineScripts()) {
            optimized = minifyInlineScripts(optimized);
        }
        
        return optimized;
    }
    
    /**
     * 移除HTML注释
     */
    private String removeComments(String html) {
        // 保留条件注释（IE条件注释）
        return html.replaceAll("<!--(?!\\[if)[^>]*?-->", "");
    }
    
    /**
     * 移除多余空白
     */
    private String removeWhitespace(String html) {
        // 保护pre和textarea标签内的内容
        StringBuilder result = new StringBuilder();
        int lastEnd = 0;
        
        Pattern preTextareaPattern = Pattern.compile("<(pre|textarea|script|style)\\b[^>]*>.*?</\\1>", Pattern.CASE_INSENSITIVE | Pattern.DOTALL);
        Matcher matcher = preTextareaPattern.matcher(html);
        
        while (matcher.find()) {
            // 处理标签外的内容
            String beforeTag = html.substring(lastEnd, matcher.start());
            result.append(WHITESPACE_PATTERN.matcher(beforeTag).replaceAll(" "));
            
            // 保留标签内的内容
            result.append(matcher.group());
            
            lastEnd = matcher.end();
        }
        
        // 处理最后一部分
        if (lastEnd < html.length()) {
            String remaining = html.substring(lastEnd);
            result.append(WHITESPACE_PATTERN.matcher(remaining).replaceAll(" "));
        }
        
        return result.toString();
    }
    
    /**
     * 折叠空白
     */
    private String collapseWhitespace(String html) {
        // 移除标签之间的空白
        return INTER_TAG_WHITESPACE.matcher(html).replaceAll("><");
    }
    
    /**
     * 移除空属性
     */
    private String removeEmptyAttributes(String html) {
        return EMPTY_ATTR_PATTERN.matcher(html).replaceAll("");
    }
    
    /**
     * 移除冗余属性
     */
    private String removeRedundantAttributes(String html) {
        // 移除布尔属性的值（如 checked="checked" -> checked）
        return BOOLEAN_ATTR_PATTERN.matcher(html).replaceAll(" $1");
    }
    
    /**
     * 压缩内联样式
     */
    private String minifyInlineStyles(String html) {
        Pattern stylePattern = Pattern.compile("style=(['\"])(.*?)\\1", Pattern.CASE_INSENSITIVE);
        Matcher matcher = stylePattern.matcher(html);
        
        StringBuffer result = new StringBuffer();
        while (matcher.find()) {
            String style = matcher.group(2);
            String minified = minifyCssString(style);
            matcher.appendReplacement(result, "style=\"" + minified + "\"");
        }
        matcher.appendTail(result);
        
        return result.toString();
    }
    
    /**
     * 压缩内联脚本
     */
    private String minifyInlineScripts(String html) {
        Pattern scriptPattern = Pattern.compile("<script\\b[^>]*>([^<]+)</script>", Pattern.CASE_INSENSITIVE);
        Matcher matcher = scriptPattern.matcher(html);
        
        StringBuffer result = new StringBuffer();
        while (matcher.find()) {
            String script = matcher.group(1);
            // 简单的脚本压缩：移除注释和多余空白
            String minified = minifyJsString(script);
            matcher.appendReplacement(result, matcher.group(0).replace(script, minified));
        }
        matcher.appendTail(result);
        
        return result.toString();
    }
    
    /**
     * 简单的CSS压缩
     */
    private String minifyCssString(String css) {
        // 移除注释
        css = css.replaceAll("/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/", "");
        // 移除多余空白
        css = css.replaceAll("\\s+", " ");
        // 移除分号前后的空格
        css = css.replaceAll("\\s*:\\s*", ":");
        css = css.replaceAll("\\s*;\\s*", ";");
        // 移除最后的分号
        css = css.replaceAll(";$", "");
        
        return css.trim();
    }
    
    /**
     * 简单的JavaScript压缩
     */
    private String minifyJsString(String js) {
        // 移除单行注释
        js = js.replaceAll("//[^\n]*", "");
        // 移除多行注释
        js = js.replaceAll("/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/", "");
        // 移除多余空白
        js = js.replaceAll("\\s+", " ");
        // 移除语句前后的空格
        js = js.replaceAll("\\s*([{}();,])\\s*", "$1");
        
        return js.trim();
    }
}