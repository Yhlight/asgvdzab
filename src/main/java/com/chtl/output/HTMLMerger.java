package com.chtl.output;
import com.chtl.output.HTMLMerger.HTMLBuilder;


import com.chtl.model.CompilationResult;
import com.chtl.model.FragmentType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.List;
import java.util.ArrayList;
import java.util.stream.Collectors;

/**
 * HTML合并器
 * 负责将各个编译器的结果合并成最终的HTML文档
 */
public class HTMLMerger {
    private static final Logger logger = LoggerFactory.getLogger(HTMLMerger.class);
    
    public HTMLMerger() {
        logger.info("HTML合并器初始化完成");
    }
    
    /**
     * 合并编译结果
     * @param results 编译结果列表
     * @return 完整的HTML文档
     */
    public String merge(List<CompilationResult> results) {
        logger.debug("开始合并编译结果，共 {} 个片段", results.size());
        
        // 分类结果
        List<CompilationResult> htmlResults = new ArrayList<>();
        List<CompilationResult> cssResults = new ArrayList<>();
        List<CompilationResult> jsResults = new ArrayList<>();
        StringBuilder additionalCSS = new StringBuilder();
        StringBuilder additionalJS = new StringBuilder();
        
        for (CompilationResult result : results) {
            switch (result.getSourceType()) {
                case CHTL:
                case CHTL_LOCAL_STYLE:
                    htmlResults.add(result);
                    // 收集生成的CSS和JS
                    if (result.getGeneratedCSS() != null && !result.getGeneratedCSS().isEmpty()) {
                        additionalCSS.append(result.getGeneratedCSS()).append("\n");
                    }
                    if (result.getGeneratedJS() != null && !result.getGeneratedJS().isEmpty()) {
                        additionalJS.append(result.getGeneratedJS()).append("\n");
                    }
                    break;
                case CSS:
                    cssResults.add(result);
                    break;
                case CHTL_JS:
                case JAVASCRIPT:
                    jsResults.add(result);
                    break;
            }
        }
        
        // 构建HTML文档
        HTMLBuilder builder = new HTMLBuilder();
        
        // 1. 合并HTML内容
        String mainHTML = mergeHTMLContent(htmlResults);
        
        // 2. 合并CSS
        String allCSS = mergeCSSContent(cssResults, additionalCSS.toString());
        
        // 3. 合并JavaScript
        String allJS = mergeJavaScriptContent(jsResults, additionalJS.toString());
        
        // 4. 构建最终HTML
        String finalHTML = builder.build(mainHTML, allCSS, allJS);
        
        // 5. 后处理优化
        finalHTML = postProcess(finalHTML);
        
        logger.debug("HTML合并完成");
        return finalHTML;
    }
    
    /**
     * 合并HTML内容
     */
    private String mergeHTMLContent(List<CompilationResult> results) {
        return results.stream()
            .filter(r -> !r.hasErrors())
            .map(CompilationResult::getCompiledContent)
            .collect(Collectors.joining("\n"));
    }
    
    /**
     * 合并CSS内容
     */
    private String mergeCSSContent(List<CompilationResult> cssResults, String additionalCSS) {
        StringBuilder css = new StringBuilder();
        
        // 添加CSS重置样式（可选）
        css.append(getCSSReset()).append("\n");
        
        // 合并所有CSS结果
        for (CompilationResult result : cssResults) {
            if (!result.hasErrors()) {
                css.append(result.getCompiledContent()).append("\n");
            }
        }
        
        // 添加额外的CSS（来自CHTL编译器）
        css.append(additionalCSS);
        
        return optimizeCSS(css.toString());
    }
    
    /**
     * 合并JavaScript内容
     */
    private String mergeJavaScriptContent(List<CompilationResult> jsResults, String additionalJS) {
        StringBuilder js = new StringBuilder();
        
        // 添加CHTL运行时（如果需要）
        boolean needsCHTLRuntime = jsResults.stream()
            .anyMatch(r -> r.getSourceType() == FragmentType.CHTL_JS);
        
        if (needsCHTLRuntime) {
            js.append("// CHTL Runtime included\n");
        }
        
        // 合并所有JS结果
        for (CompilationResult result : jsResults) {
            if (!result.hasErrors()) {
                js.append(result.getCompiledContent()).append("\n");
            }
        }
        
        // 添加额外的JS（来自CHTL编译器）
        js.append(additionalJS);
        
        return optimizeJS(js.toString());
    }
    
    /**
     * 获取CSS重置样式
     */
    private String getCSSReset() {
        return """
            /* CSS Reset */
            * {
                margin: 0;
                padding: 0;
                box-sizing: border-box;
            }
            
            html {
                font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 
                           'Helvetica Neue', Arial, sans-serif;
                font-size: 16px;
                line-height: 1.5;
                color: #333;
            }
            """;
    }
    
    /**
     * 优化CSS
     */
    private String optimizeCSS(String css) {
        // 移除重复规则
        // 合并相同选择器
        // 压缩空白
        return css.trim();
    }
    
    /**
     * 优化JavaScript
     */
    private String optimizeJS(String js) {
        // 移除重复代码
        // 合并相同函数
        return js.trim();
    }
    
    /**
     * 后处理优化
     */
    private String postProcess(String html) {
        // 格式化HTML
        html = formatHTML(html);
        
        // 添加必要的meta标签
        html = ensureMetaTags(html);
        
        return html;
    }
    
    /**
     * 格式化HTML
     */
    private String formatHTML(String html) {
        // 简单的格式化，实际可以使用更复杂的格式化库
        return html;
    }
    
    /**
     * 确保必要的meta标签存在
     */
    private String ensureMetaTags(String html) {
        if (!html.contains("<meta charset=")) {
            html = html.replace("<head>", "<head>\n  <meta charset=\"UTF-8\">");
        }
        
        if (!html.contains("<meta name=\"viewport\"")) {
            html = html.replace("<head>", 
                "<head>\n  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
        }
        
        return html;
    }
    
    /**
     * HTML构建器内部类
     */
    private static class HTMLBuilder {
        
        public String build(String bodyContent, String css, String js) {
            StringBuilder html = new StringBuilder();
            
            // 检查是否已有完整的HTML结构
            if (bodyContent.contains("<html") && bodyContent.contains("</html>")) {
                // 已有完整结构，插入CSS和JS
                return insertAssetsIntoExistingHTML(bodyContent, css, js);
            }
            
            // 构建新的HTML结构
            html.append("<!DOCTYPE html>\n");
            html.append("<html lang=\"zh-CN\">\n");
            html.append("<head>\n");
            html.append("  <meta charset=\"UTF-8\">\n");
            html.append("  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
            html.append("  <title>CHTL Generated Page</title>\n");
            
            // 添加CSS
            if (!css.isEmpty()) {
                html.append("  <style>\n");
                html.append(indentCSS(css, "    "));
                html.append("  </style>\n");
            }
            
            html.append("</head>\n");
            html.append("<body>\n");
            
            // 添加主体内容
            html.append(indentHTML(bodyContent, "  "));
            
            // 添加JavaScript
            if (!js.isEmpty()) {
                html.append("  <script>\n");
                html.append(indentJS(js, "    "));
                html.append("  </script>\n");
            }
            
            html.append("</body>\n");
            html.append("</html>\n");
            
            return html.toString();
        }
        
        private String insertAssetsIntoExistingHTML(String html, String css, String js) {
            // 在</head>前插入CSS
            if (!css.isEmpty()) {
                String styleTag = "<style>\n" + css + "\n</style>\n";
                html = html.replace("</head>", styleTag + "</head>");
            }
            
            // 在</body>前插入JS
            if (!js.isEmpty()) {
                String scriptTag = "<script>\n" + js + "\n</script>\n";
                html = html.replace("</body>", scriptTag + "</body>");
            }
            
            return html;
        }
        
        private String indentCSS(String css, String indent) {
            return css.lines()
                     .map(line -> indent + line)
                     .collect(Collectors.joining("\n")) + "\n";
        }
        
        private String indentHTML(String html, String indent) {
            return html.lines()
                     .map(line -> indent + line)
                     .collect(Collectors.joining("\n")) + "\n";
        }
        
        private String indentJS(String js, String indent) {
            return js.lines()
                     .map(line -> indent + line)
                     .collect(Collectors.joining("\n")) + "\n";
        }
    }
}