package com.chtl.compiler.optimization;

import com.chtl.context.CompilationContext;
import com.chtl.scanner.ScannerStateMachine;
import com.chtl.compiler.optimization.html.HtmlOptimizer;
import com.chtl.compiler.optimization.css.CssOptimizer;
import com.chtl.compiler.optimization.js.JavaScriptOptimizer;
import com.chtl.compiler.optimization.sourcemap.SourceMapGenerator;
import com.chtl.compiler.optimization.sourcemap.SourceMap;

import java.util.*;

/**
 * 编译输出优化器
 * 负责协调各种优化策略
 * 严格遵循CHTL语法文档
 */
public class OutputOptimizer {
    private final CompilationContext context;
    private final ScannerStateMachine stateMachine;
    
    // 各种优化器
    private final HtmlOptimizer htmlOptimizer;
    private final CssOptimizer cssOptimizer;
    private final JavaScriptOptimizer jsOptimizer;
    private final SourceMapGenerator sourceMapGenerator;
    
    // 优化选项
    private final OptimizationOptions options;
    
    public OutputOptimizer(CompilationContext context, OptimizationOptions options) {
        this.context = context;
        this.stateMachine = new ScannerStateMachine(context);
        this.options = options != null ? options : OptimizationOptions.getDefault();
        
        // 初始化各种优化器
        this.htmlOptimizer = new HtmlOptimizer(context, this.options.getHtmlOptions());
        this.cssOptimizer = new CssOptimizer(context, this.options.getCssOptions());
        this.jsOptimizer = new JavaScriptOptimizer(context, this.options.getJsOptions());
        this.sourceMapGenerator = new SourceMapGenerator(context);
    }
    
    /**
     * 优化编译输出
     */
    public OptimizedOutput optimize(CompilationOutput input) {
        // 转换为内部格式
        com.chtl.compiler.CHTLCompiler.CompilationOutput internalInput = 
            new com.chtl.compiler.CHTLCompiler.CompilationOutput();
        internalInput.setHtml(input.getHtml());
        internalInput.setCssFiles(input.getCssFiles());
        internalInput.setJsFiles(input.getJsFiles());
        
        return optimize(internalInput);
    }
    
    /**
     * 优化编译输出（内部格式）
     */
    public com.chtl.compiler.CHTLCompiler.OptimizedOutput optimize(
            com.chtl.compiler.CHTLCompiler.CompilationOutput input) {
        try (var stateGuard = context.enterState(CompilationContext.State.OPTIMIZING)) {
            OptimizedOutput output = new OptimizedOutput();
            
            // 1. 优化HTML
            if (options.isOptimizeHtml()) {
                output.setHtml(htmlOptimizer.optimize(input.getHtml()));
            } else {
                output.setHtml(input.getHtml());
            }
            
            // 2. 优化CSS（包括合并）
            if (options.isOptimizeCss()) {
                output.setCss(cssOptimizer.optimize(input.getCssFiles()));
            } else {
                output.setCss(mergeCss(input.getCssFiles()));
            }
            
            // 3. 优化JavaScript
            if (options.isOptimizeJs()) {
                output.setJavaScript(jsOptimizer.optimize(input.getJsFiles()));
            } else {
                output.setJavaScript(mergeJs(input.getJsFiles()));
            }
            
            // 4. 生成源码映射
            if (options.isGenerateSourceMap()) {
                generateSourceMaps(input, output);
            }
            
            // 5. 收集优化统计信息
            collectStatistics(input, output);
            
            return output;
        }
    }
    
    /**
     * 简单合并CSS（不优化）
     */
    private String mergeCss(List<CssFile> cssFiles) {
        StringBuilder merged = new StringBuilder();
        
        for (CssFile file : cssFiles) {
            if (merged.length() > 0) {
                merged.append("\n\n");
            }
            
            // 添加文件注释
            merged.append("/* Source: ").append(file.getSourcePath()).append(" */\n");
            merged.append(file.getContent());
        }
        
        return merged.toString();
    }
    
    /**
     * 简单合并JavaScript（不优化）
     */
    private String mergeJs(List<JsFile> jsFiles) {
        StringBuilder merged = new StringBuilder();
        
        // CHTL JS运行时始终在最前面
        for (JsFile file : jsFiles) {
            if (file.isRuntime()) {
                merged.append(file.getContent());
                merged.append("\n\n");
                break;
            }
        }
        
        // 其他JS文件
        for (JsFile file : jsFiles) {
            if (!file.isRuntime()) {
                // 使用IIFE包装避免污染
                merged.append("/* Source: ").append(file.getSourcePath()).append(" */\n");
                merged.append("(function() {\n");
                merged.append("'use strict';\n");
                merged.append(file.getContent());
                merged.append("\n})();\n\n");
            }
        }
        
        return merged.toString();
    }
    
    /**
     * 生成源码映射
     */
    private void generateSourceMaps(CompilationOutput input, OptimizedOutput output) {
        // HTML源码映射
        if (options.getSourceMapOptions().isMapHtml()) {
            SourceMap htmlMap = sourceMapGenerator.generateHtmlMap(
                input.getHtml(),
                output.getHtml(),
                input.getSourceFiles()
            );
            output.setHtmlSourceMap(htmlMap);
        }
        
        // CSS源码映射
        if (options.getSourceMapOptions().isMapCss()) {
            SourceMap cssMap = sourceMapGenerator.generateCssMap(
                input.getCssFiles(),
                output.getCss()
            );
            output.setCssSourceMap(cssMap);
        }
        
        // JavaScript源码映射
        if (options.getSourceMapOptions().isMapJs()) {
            SourceMap jsMap = sourceMapGenerator.generateJsMap(
                input.getJsFiles(),
                output.getJavaScript()
            );
            output.setJsSourceMap(jsMap);
        }
    }
    
    /**
     * 收集优化统计信息
     */
    private void collectStatistics(CompilationOutput input, OptimizedOutput output) {
        OptimizationStatistics stats = new OptimizationStatistics();
        
        // HTML统计
        int htmlOriginalSize = input.getHtml().length();
        int htmlOptimizedSize = output.getHtml().length();
        stats.setHtmlCompressionRatio(calculateCompressionRatio(htmlOriginalSize, htmlOptimizedSize));
        stats.setHtmlSizeSaved(htmlOriginalSize - htmlOptimizedSize);
        
        // CSS统计
        int cssOriginalSize = calculateTotalSize(input.getCssFiles());
        int cssOptimizedSize = output.getCss().length();
        stats.setCssCompressionRatio(calculateCompressionRatio(cssOriginalSize, cssOptimizedSize));
        stats.setCssSizeSaved(cssOriginalSize - cssOptimizedSize);
        stats.setCssFilesMerged(input.getCssFiles().size());
        
        // JavaScript统计
        int jsOriginalSize = calculateTotalSize(input.getJsFiles());
        int jsOptimizedSize = output.getJavaScript().length();
        stats.setJsCompressionRatio(calculateCompressionRatio(jsOriginalSize, jsOptimizedSize));
        stats.setJsSizeSaved(jsOriginalSize - jsOptimizedSize);
        stats.setJsFilesMerged(input.getJsFiles().size());
        
        // 总体统计
        stats.setTotalOriginalSize(htmlOriginalSize + cssOriginalSize + jsOriginalSize);
        stats.setTotalOptimizedSize(htmlOptimizedSize + cssOptimizedSize + jsOptimizedSize);
        stats.setTotalCompressionRatio(
            calculateCompressionRatio(stats.getTotalOriginalSize(), stats.getTotalOptimizedSize())
        );
        
        output.setStatistics(stats);
    }
    
    private int calculateTotalSize(List<? extends FileContent> files) {
        return files.stream().mapToInt(f -> f.getContent().length()).sum();
    }
    
    private double calculateCompressionRatio(int original, int optimized) {
        if (original == 0) return 0;
        return (1.0 - (double) optimized / original) * 100;
    }
    
    /**
     * 编译输出
     */
    public static class CompilationOutput {
        private String html;
        private List<CssFile> cssFiles = new ArrayList<>();
        private List<JsFile> jsFiles = new ArrayList<>();
        private List<SourceFile> sourceFiles = new ArrayList<>();
        
        // Getters and setters
        public String getHtml() { return html; }
        public void setHtml(String html) { this.html = html; }
        public List<CssFile> getCssFiles() { return cssFiles; }
        public List<JsFile> getJsFiles() { return jsFiles; }
        public List<SourceFile> getSourceFiles() { return sourceFiles; }
    }
    
    /**
     * 优化后的输出
     */
    public static class OptimizedOutput {
        private String html;
        private String css;
        private String javaScript;
        private SourceMap htmlSourceMap;
        private SourceMap cssSourceMap;
        private SourceMap jsSourceMap;
        private OptimizationStatistics statistics;
        
        // Getters and setters
        public String getHtml() { return html; }
        public void setHtml(String html) { this.html = html; }
        public String getCss() { return css; }
        public void setCss(String css) { this.css = css; }
        public String getJavaScript() { return javaScript; }
        public void setJavaScript(String javaScript) { this.javaScript = javaScript; }
        public SourceMap getHtmlSourceMap() { return htmlSourceMap; }
        public void setHtmlSourceMap(SourceMap map) { this.htmlSourceMap = map; }
        public SourceMap getCssSourceMap() { return cssSourceMap; }
        public void setCssSourceMap(SourceMap map) { this.cssSourceMap = map; }
        public SourceMap getJsSourceMap() { return jsSourceMap; }
        public void setJsSourceMap(SourceMap map) { this.jsSourceMap = map; }
        public OptimizationStatistics getStatistics() { return statistics; }
        public void setStatistics(OptimizationStatistics stats) { this.statistics = stats; }
    }
    
    /**
     * 文件内容基类
     */
    public static abstract class FileContent {
        protected String sourcePath;
        protected String content;
        
        public String getSourcePath() { return sourcePath; }
        public String getContent() { return content; }
        public void setContent(String content) { this.content = content; }
    }
    
    /**
     * CSS文件
     */
    public static class CssFile extends FileContent {
        private boolean isGlobal;
        
        public CssFile(String sourcePath, String content, boolean isGlobal) {
            this.sourcePath = sourcePath;
            this.content = content;
            this.isGlobal = isGlobal;
        }
        
        public boolean isGlobal() { return isGlobal; }
    }
    
    /**
     * JavaScript文件
     */
    public static class JsFile extends FileContent {
        private boolean isRuntime;
        
        public JsFile(String sourcePath, String content, boolean isRuntime) {
            this.sourcePath = sourcePath;
            this.content = content;
            this.isRuntime = isRuntime;
        }
        
        public boolean isRuntime() { return isRuntime; }
    }
    
    /**
     * 源文件信息
     */
    public static class SourceFile {
        private String path;
        private String content;
        private int lineCount;
        
        public SourceFile(String path, String content) {
            this.path = path;
            this.content = content;
            this.lineCount = content.split("\n").length;
        }
        
        public String getPath() { return path; }
        public String getContent() { return content; }
        public int getLineCount() { return lineCount; }
    }
}