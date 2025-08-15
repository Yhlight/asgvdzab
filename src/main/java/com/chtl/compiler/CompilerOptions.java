package com.chtl.compiler;

import com.chtl.compiler.optimization.OptimizationOptions;

/**
 * CHTL编译器选项
 */
public class CompilerOptions {
    // 调试选项
    private boolean debugMode = false;
    private boolean verboseMode = false;
    private boolean showTokens = false;
    private boolean showAST = false;
    private boolean showStates = false;
    
    // 优化选项
    private boolean optimizationEnabled = true;
    private OptimizationOptions optimizationOptions = OptimizationOptions.getDefault();
    
    // 并行选项
    private int parallelism = Runtime.getRuntime().availableProcessors();
    private int compilationTimeout = 60; // 秒
    
    // 输出选项
    private String outputPath = null;
    private boolean generateSourceMap = true;
    private String sourceMapPath = null;
    
    // 错误处理选项
    private int maxErrors = 100;
    private boolean failFast = false;
    private boolean showWarnings = true;
    
    // Cmod选项
    private String cmodPath = "cmod_modules";
    private String officialModulePath = "chtl_modules";
    
    // 构造函数
    public CompilerOptions() {
    }
    
    // 快速配置方法
    public static CompilerOptions development() {
        CompilerOptions options = new CompilerOptions();
        options.setDebugMode(true);
        options.setOptimizationOptions(OptimizationOptions.getDevelopment());
        options.setGenerateSourceMap(true);
        return options;
    }
    
    public static CompilerOptions production() {
        CompilerOptions options = new CompilerOptions();
        options.setDebugMode(false);
        options.setOptimizationOptions(OptimizationOptions.getProduction());
        options.setGenerateSourceMap(false);
        return options;
    }
    
    // Getters and setters
    public boolean isDebugMode() {
        return debugMode;
    }
    
    public void setDebugMode(boolean debugMode) {
        this.debugMode = debugMode;
    }
    
    public boolean isVerboseMode() {
        return verboseMode;
    }
    
    public void setVerboseMode(boolean verboseMode) {
        this.verboseMode = verboseMode;
    }
    
    public boolean isShowTokens() {
        return showTokens;
    }
    
    public void setShowTokens(boolean showTokens) {
        this.showTokens = showTokens;
    }
    
    public boolean isShowAST() {
        return showAST;
    }
    
    public void setShowAST(boolean showAST) {
        this.showAST = showAST;
    }
    
    public boolean isShowStates() {
        return showStates;
    }
    
    public void setShowStates(boolean showStates) {
        this.showStates = showStates;
    }
    
    public boolean isOptimizationEnabled() {
        return optimizationEnabled;
    }
    
    public void setOptimizationEnabled(boolean optimizationEnabled) {
        this.optimizationEnabled = optimizationEnabled;
    }
    
    public OptimizationOptions getOptimizationOptions() {
        return optimizationOptions;
    }
    
    public void setOptimizationOptions(OptimizationOptions optimizationOptions) {
        this.optimizationOptions = optimizationOptions;
    }
    
    public int getParallelism() {
        return parallelism;
    }
    
    public void setParallelism(int parallelism) {
        this.parallelism = parallelism;
    }
    
    public int getCompilationTimeout() {
        return compilationTimeout;
    }
    
    public void setCompilationTimeout(int compilationTimeout) {
        this.compilationTimeout = compilationTimeout;
    }
    
    public String getOutputPath() {
        return outputPath;
    }
    
    public void setOutputPath(String outputPath) {
        this.outputPath = outputPath;
    }
    
    public boolean isGenerateSourceMap() {
        return generateSourceMap;
    }
    
    public void setGenerateSourceMap(boolean generateSourceMap) {
        this.generateSourceMap = generateSourceMap;
    }
    
    public String getSourceMapPath() {
        return sourceMapPath;
    }
    
    public void setSourceMapPath(String sourceMapPath) {
        this.sourceMapPath = sourceMapPath;
    }
    
    public int getMaxErrors() {
        return maxErrors;
    }
    
    public void setMaxErrors(int maxErrors) {
        this.maxErrors = maxErrors;
    }
    
    public boolean isFailFast() {
        return failFast;
    }
    
    public void setFailFast(boolean failFast) {
        this.failFast = failFast;
    }
    
    public boolean isShowWarnings() {
        return showWarnings;
    }
    
    public void setShowWarnings(boolean showWarnings) {
        this.showWarnings = showWarnings;
    }
    
    public String getCmodPath() {
        return cmodPath;
    }
    
    public void setCmodPath(String cmodPath) {
        this.cmodPath = cmodPath;
    }
    
    public String getOfficialModulePath() {
        return officialModulePath;
    }
    
    public void setOfficialModulePath(String officialModulePath) {
        this.officialModulePath = officialModulePath;
    }
}