package com.chtl.compiler.optimization;

import com.chtl.compiler.optimization.OptimizationOptions.CssOptions;
import com.chtl.compiler.optimization.OptimizationOptions.HtmlOptions;
import com.chtl.compiler.optimization.OptimizationOptions.JsOptions;
import com.chtl.compiler.optimization.OptimizationOptions.SourceMapOptions;


/**
 * 优化选项配置
 */
public class OptimizationOptions {
    private boolean optimizeHtml = true;
    private boolean optimizeCss = true;
    private boolean optimizeJs = true;
    private boolean generateSourceMap = true;
    
    private HtmlOptions htmlOptions = new HtmlOptions();
    private CssOptions cssOptions = new CssOptions();
    private JsOptions jsOptions = new JsOptions();
    private SourceMapOptions sourceMapOptions = new SourceMapOptions();
    
    /**
     * 获取默认优化选项
     */
    public static OptimizationOptions getDefault() {
        return new OptimizationOptions();
    }
    
    /**
     * 获取生产环境优化选项
     */
    public static OptimizationOptions getProduction() {
        OptimizationOptions options = new OptimizationOptions();
        
        // HTML: 最大化压缩
        options.htmlOptions.setRemoveComments(true);
        options.htmlOptions.setRemoveWhitespace(true);
        options.htmlOptions.setCollapseWhitespace(true);
        options.htmlOptions.setMinifyInlineStyles(true);
        options.htmlOptions.setMinifyInlineScripts(true);
        
        // CSS: 最大化压缩
        options.cssOptions.setRemoveComments(true);
        options.cssOptions.setRemoveWhitespace(true);
        options.cssOptions.setMergeRules(true);
        options.cssOptions.setOptimizeSelectors(true);
        options.cssOptions.setRemoveUnused(true);
        
        // JS: 最大化压缩
        options.jsOptions.setRemoveComments(true);
        options.jsOptions.setRemoveWhitespace(true);
        options.jsOptions.setMangleVariables(true);
        options.jsOptions.setOptimizeExpressions(true);
        
        return options;
    }
    
    /**
     * 获取开发环境优化选项
     */
    public static OptimizationOptions getDevelopment() {
        OptimizationOptions options = new OptimizationOptions();
        
        // 开发环境保留可读性
        options.htmlOptions.setRemoveComments(false);
        options.htmlOptions.setRemoveWhitespace(false);
        options.htmlOptions.setCollapseWhitespace(false);
        
        options.cssOptions.setRemoveComments(false);
        options.cssOptions.setRemoveWhitespace(false);
        
        options.jsOptions.setRemoveComments(false);
        options.jsOptions.setRemoveWhitespace(false);
        options.jsOptions.setMangleVariables(false);
        
        // 但生成源码映射
        options.generateSourceMap = true;
        
        return options;
    }
    
    // Getters and setters
    public boolean isOptimizeHtml() { return optimizeHtml; }
    public void setOptimizeHtml(boolean optimizeHtml) { this.optimizeHtml = optimizeHtml; }
    
    public boolean isOptimizeCss() { return optimizeCss; }
    public void setOptimizeCss(boolean optimizeCss) { this.optimizeCss = optimizeCss; }
    
    public boolean isOptimizeJs() { return optimizeJs; }
    public void setOptimizeJs(boolean optimizeJs) { this.optimizeJs = optimizeJs; }
    
    public boolean isGenerateSourceMap() { return generateSourceMap; }
    public void setGenerateSourceMap(boolean generateSourceMap) { this.generateSourceMap = generateSourceMap; }
    
    public HtmlOptions getHtmlOptions() { return htmlOptions; }
    public CssOptions getCssOptions() { return cssOptions; }
    public JsOptions getJsOptions() { return jsOptions; }
    public SourceMapOptions getSourceMapOptions() { return sourceMapOptions; }
    
    /**
     * HTML优化选项
     */
    public static class HtmlOptions {
        private boolean removeComments = true;
        private boolean removeWhitespace = true;
        private boolean collapseWhitespace = true;
        private boolean removeEmptyAttributes = true;
        private boolean removeRedundantAttributes = true;
        private boolean minifyInlineStyles = true;
        private boolean minifyInlineScripts = true;
        
        // Getters and setters
        public boolean isRemoveComments() { return removeComments; }
        public void setRemoveComments(boolean removeComments) { this.removeComments = removeComments; }
        
        public boolean isRemoveWhitespace() { return removeWhitespace; }
        public void setRemoveWhitespace(boolean removeWhitespace) { this.removeWhitespace = removeWhitespace; }
        
        public boolean isCollapseWhitespace() { return collapseWhitespace; }
        public void setCollapseWhitespace(boolean collapseWhitespace) { this.collapseWhitespace = collapseWhitespace; }
        
        public boolean isRemoveEmptyAttributes() { return removeEmptyAttributes; }
        public void setRemoveEmptyAttributes(boolean removeEmptyAttributes) { this.removeEmptyAttributes = removeEmptyAttributes; }
        
        public boolean isRemoveRedundantAttributes() { return removeRedundantAttributes; }
        public void setRemoveRedundantAttributes(boolean removeRedundantAttributes) { this.removeRedundantAttributes = removeRedundantAttributes; }
        
        public boolean isMinifyInlineStyles() { return minifyInlineStyles; }
        public void setMinifyInlineStyles(boolean minifyInlineStyles) { this.minifyInlineStyles = minifyInlineStyles; }
        
        public boolean isMinifyInlineScripts() { return minifyInlineScripts; }
        public void setMinifyInlineScripts(boolean minifyInlineScripts) { this.minifyInlineScripts = minifyInlineScripts; }
    }
    
    /**
     * CSS优化选项
     */
    public static class CssOptions {
        private boolean removeComments = true;
        private boolean removeWhitespace = true;
        private boolean mergeRules = true;
        private boolean optimizeSelectors = true;
        private boolean removeUnused = false; // 需要谨慎使用
        private boolean shortenColors = true;
        private boolean shortenValues = true;
        
        // Getters and setters
        public boolean isRemoveComments() { return removeComments; }
        public void setRemoveComments(boolean removeComments) { this.removeComments = removeComments; }
        
        public boolean isRemoveWhitespace() { return removeWhitespace; }
        public void setRemoveWhitespace(boolean removeWhitespace) { this.removeWhitespace = removeWhitespace; }
        
        public boolean isMergeRules() { return mergeRules; }
        public void setMergeRules(boolean mergeRules) { this.mergeRules = mergeRules; }
        
        public boolean isOptimizeSelectors() { return optimizeSelectors; }
        public void setOptimizeSelectors(boolean optimizeSelectors) { this.optimizeSelectors = optimizeSelectors; }
        
        public boolean isRemoveUnused() { return removeUnused; }
        public void setRemoveUnused(boolean removeUnused) { this.removeUnused = removeUnused; }
        
        public boolean isShortenColors() { return shortenColors; }
        public void setShortenColors(boolean shortenColors) { this.shortenColors = shortenColors; }
        
        public boolean isShortenValues() { return shortenValues; }
        public void setShortenValues(boolean shortenValues) { this.shortenValues = shortenValues; }
    }
    
    /**
     * JavaScript优化选项
     */
    public static class JsOptions {
        private boolean removeComments = true;
        private boolean removeWhitespace = true;
        private boolean removeDeadCode = true;
        private boolean mangleVariables = false; // 默认关闭，避免破坏CHTL JS功能
        private boolean optimizeExpressions = true;
        private boolean inlineConstants = true;
        
        // Getters and setters
        public boolean isRemoveComments() { return removeComments; }
        public void setRemoveComments(boolean removeComments) { this.removeComments = removeComments; }
        
        public boolean isRemoveWhitespace() { return removeWhitespace; }
        public void setRemoveWhitespace(boolean removeWhitespace) { this.removeWhitespace = removeWhitespace; }
        
        public boolean isRemoveDeadCode() { return removeDeadCode; }
        public void setRemoveDeadCode(boolean removeDeadCode) { this.removeDeadCode = removeDeadCode; }
        
        public boolean isMangleVariables() { return mangleVariables; }
        public void setMangleVariables(boolean mangleVariables) { this.mangleVariables = mangleVariables; }
        
        public boolean isOptimizeExpressions() { return optimizeExpressions; }
        public void setOptimizeExpressions(boolean optimizeExpressions) { this.optimizeExpressions = optimizeExpressions; }
        
        public boolean isInlineConstants() { return inlineConstants; }
        public void setInlineConstants(boolean inlineConstants) { this.inlineConstants = inlineConstants; }
    }
    
    /**
     * 源码映射选项
     */
    public static class SourceMapOptions {
        private boolean mapHtml = true;
        private boolean mapCss = true;
        private boolean mapJs = true;
        private boolean includeSourceContent = true;
        
        // Getters and setters
        public boolean isMapHtml() { return mapHtml; }
        public void setMapHtml(boolean mapHtml) { this.mapHtml = mapHtml; }
        
        public boolean isMapCss() { return mapCss; }
        public void setMapCss(boolean mapCss) { this.mapCss = mapCss; }
        
        public boolean isMapJs() { return mapJs; }
        public void setMapJs(boolean mapJs) { this.mapJs = mapJs; }
        
        public boolean isIncludeSourceContent() { return includeSourceContent; }
        public void setIncludeSourceContent(boolean includeSourceContent) { this.includeSourceContent = includeSourceContent; }
    }
}