package com.chtl.compiler;import com.chtl.model.SourceLocation;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.SourceLocation;
import com.chtl.ast.chtljs.CHTLJSASTNode;
import com.chtl.ast.node.ImportStatementNode;
import com.chtl.ast.node.NamespaceNode;
import com.chtl.compiler.CHTLCompiler.CompilationOutput;
import com.chtl.compiler.CHTLCompiler.OptimizedOutput;
import com.chtl.compiler.chtl.CHTLParser;
import com.chtl.compiler.chtl.lexer.CHTLStateMachineLexer;
import com.chtl.compiler.chtl.token.CHTLToken;
import com.chtl.compiler.chtljs.lexer.CHTLJSStateMachineLexer;
import com.chtl.compiler.chtljs.token.CHTLJSToken;
import com.chtl.compiler.debug.DebugInfo;
import com.chtl.compiler.debug.ErrorHandler;
import com.chtl.compiler.dispatcher.CompilerDispatcher;
import com.chtl.compiler.optimization.OutputOptimizer;
import com.chtl.context.CHTLJSContext;
import com.chtl.context.CompilationError;
import com.chtl.generator.CHTLGenerator;
import com.chtl.generator.CHTLJSGenerator;
import com.chtl.parser.CHTLJSParser;


import com.chtl.scanner.CHTLUnifiedScanner;
import com.chtl.scanner.Fragment;
import com.chtl.scanner.ScannerStateMachine;
import com.chtl.context.CompilationContext;
import com.chtl.compiler.debug.*;
import com.chtl.compiler.optimization.*;
import com.chtl.lexer.*;
import com.chtl.parser.*;
import com.chtl.generator.*;
import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.compiler.importer.ImportManager;
import com.chtl.compiler.namespace.NamespaceManager;
import com.chtl.compiler.constraint.ConstraintManager;
import com.chtl.compiler.integration.NamespaceImportIntegration;

import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;

/**
 * CHTL编译器主类
 * 集成所有模块，实现完整的编译流程
 * 严格遵循CHTL语法文档
 */
public class CHTLCompiler {
    private final CompilerDispatcher dispatcher;
    private final CHTLUnifiedScanner scanner;
    private final ErrorHandler errorHandler;
    private final DebugInfo debugInfo;
    private final ExecutorService executorService;
    
    // 编译器选项
    private CompilerOptions options;
    
    public CHTLCompiler() {
        this(new CompilerOptions());
    }
    
    public CHTLCompiler(CompilerOptions options) {
        this.options = options;
        this.dispatcher = new CompilerDispatcher();
        this.scanner = new CHTLUnifiedScanner();
        this.errorHandler = null; // 在compile时初始化
        this.debugInfo = null; // 在compile时初始化
        this.executorService = Executors.newFixedThreadPool(options.getParallelism());
    }
    
    /**
     * 编译CHTL文件
     */
    public CompilationResult compile(String sourcePath) throws IOException {
        CompilationContext context = new CompilationContext();
        context.setDebugMode(options.isDebugMode());
        context.setVerboseMode(options.isVerboseMode());
        
        return compile(sourcePath, context);
    }
    
    /**
     * 使用指定上下文编译CHTL文件
     */
    public CompilationResult compile(String sourcePath, CompilationContext context) throws IOException {
        long startTime = System.currentTimeMillis();
        
        // 初始化错误处理和调试
        ErrorHandler errorHandler = new ErrorHandler(context);
        DebugInfo debugInfo = new DebugInfo(context, System.out);
        configureDebugInfo(debugInfo);
        
        // 开始编译
        debugInfo.compilationStart(sourcePath);
        
        try {
            // 1. 读取源文件
            String sourceCode = Files.readString(Paths.get(sourcePath));
            context.setCurrentScope(sourcePath);
            
            // 2. 扫描和分片
            debugInfo.lexingStart("扫描阶段");
            List<Fragment> fragments = scanner.scan(sourceCode, context);
            
            if (context.hasErrors()) {
                return createErrorResult(context, startTime);
            }
            
            // 3. 并行编译各片段
            debugInfo.parsingStart("编译阶段");
            CompilationOutput rawOutput = compileFragments(fragments, context, sourcePath);
            
            if (context.hasErrors()) {
                return createErrorResult(context, startTime);
            }
            
            // 4. 优化输出
            OptimizedOutput optimizedOutput = null;
            if (options.isOptimizationEnabled()) {
                debugInfo.parsingStart("优化阶段");
                OutputOptimizer optimizer = new OutputOptimizer(context, options.getOptimizationOptions());
                optimizedOutput = optimizer.optimize(rawOutput);
            }
            
            // 5. 生成最终输出
            String finalOutput = generateFinalOutput(rawOutput, optimizedOutput);
            
            // 6. 创建成功结果
            long endTime = System.currentTimeMillis();
            debugInfo.compilationEnd(true, endTime - startTime);
            
            CompilationResult result = new CompilationResult();
            result.setSuccess(true);
            result.setOutput(finalOutput);
            result.setCompilationTime(endTime - startTime);
            result.setWarnings(context.getWarnings());
            
            // 生成Source Map
            if (optimizedOutput != null && optimizedOutput.getSourceMaps() != null) {
                result.setSourceMaps(optimizedOutput.getSourceMaps());
            }
            
            return result;
            
        } catch (Exception e) {
            // 处理异常
            context.addError(new CompilationError(
                "编译失败: " + e.getMessage(),
                CompilationError.ErrorType.CONFIGURATION_ERROR
            ));
            
            long endTime = System.currentTimeMillis();
            debugInfo.compilationEnd(false, endTime - startTime);
            
            return createErrorResult(context, startTime);
        }
    }
    
    /**
     * 编译代码片段
     */
    private CompilationOutput compileFragments(List<Fragment> fragments, CompilationContext context, String sourcePath) {
        // 设置管理器
        ImportManager importManager = new ImportManager(Paths.get(sourcePath).getParent());
        NamespaceManager namespaceManager = new NamespaceManager();
        ConstraintManager constraintManager = context.getConstraintManager();
        
        // 创建集成器
        NamespaceImportIntegration integration = new NamespaceImportIntegration(
            context, namespaceManager, importManager
        );
        
        // 分类片段
        List<Fragment> chtlFragments = new ArrayList<>();
        List<Fragment> chtlJsFragments = new ArrayList<>();
        List<Fragment> cssFragments = new ArrayList<>();
        List<Fragment> jsFragments = new ArrayList<>();
        
        for (Fragment fragment : fragments) {
            switch (fragment.getType()) {
                case CHTL -> chtlFragments.add(fragment);
                case CHTL_JS -> chtlJsFragments.add(fragment);
                case CSS -> cssFragments.add(fragment);
                case JAVASCRIPT -> jsFragments.add(fragment);
            }
        }
        
        // 并行编译
        List<CompletableFuture<String>> futures = new ArrayList<>();
        
        // 编译CHTL片段
        if (!chtlFragments.isEmpty()) {
            CompletableFuture<String> chtlFuture = CompletableFuture.supplyAsync(() -> {
                return compileCHTL(chtlFragments, context, integration);
            }, executorService);
            futures.add(chtlFuture);
        }
        
        // 编译CHTL JS片段
        if (!chtlJsFragments.isEmpty()) {
            CompletableFuture<String> chtlJsFuture = CompletableFuture.supplyAsync(() -> {
                return compileCHTLJS(chtlJsFragments, context);
            }, executorService);
            futures.add(chtlJsFuture);
        }
        
        // 编译CSS片段
        if (!cssFragments.isEmpty()) {
            CompletableFuture<String> cssFuture = CompletableFuture.supplyAsync(() -> {
                return compileCSS(cssFragments, context);
            }, executorService);
            futures.add(cssFuture);
        }
        
        // 编译JavaScript片段
        if (!jsFragments.isEmpty()) {
            CompletableFuture<String> jsFuture = CompletableFuture.supplyAsync(() -> {
                return compileJS(jsFragments, context);
            }, executorService);
            futures.add(jsFuture);
        }
        
        // 等待所有编译完成
        CompletableFuture<Void> allFutures = CompletableFuture.allOf(
            futures.toArray(new CompletableFuture[0])
        );
        
        try {
            allFutures.get(options.getCompilationTimeout(), TimeUnit.SECONDS);
        } catch (Exception e) {
            context.addError(new CompilationError(
                "编译超时或失败: " + e.getMessage(),
                CompilationError.ErrorType.CONFIGURATION_ERROR
            ));
        }
        
        // 收集结果
        CompilationOutput output = new CompilationOutput();
        
        // 提取HTML、CSS、JS
        String html = "";
        List<String> cssFiles = new ArrayList<>();
        List<String> jsFiles = new ArrayList<>();
        
        for (CompletableFuture<String> future : futures) {
            try {
                String result = future.get();
                if (result.contains("<") && result.contains(">")) {
                    html = result;
                } else if (result.contains("{") && result.contains("}") && result.contains(":")) {
                    cssFiles.add(result);
                } else {
                    jsFiles.add(result);
                }
            } catch (Exception e) {
                // 已在上面处理
            }
        }
        
        output.setHtml(html);
        output.setCssFiles(cssFiles);
        output.setJsFiles(jsFiles);
        
        return output;
    }
    
    /**
     * 编译CHTL片段
     */
    private String compileCHTL(List<Fragment> fragments, CompilationContext context, 
                               NamespaceImportIntegration integration) {
        StringBuilder result = new StringBuilder();
        
        for (Fragment fragment : fragments) {
            try {
                // 词法分析
                CHTLStateMachineLexer lexer = new CHTLStateMachineLexer(context);
                List<CHTLToken> tokens = lexer.tokenize(fragment.getContent());
                
                // 语法分析
                CHTLParser parser = new CHTLParser(context);
                CHTLASTNode ast = parser.parse(tokens);
                
                // 处理命名空间和导入
                if (ast instanceof NamespaceNode) {
                    integration.processNamespace((NamespaceNode) ast);
                } else if (ast instanceof ImportStatementNode) {
                    integration.processImport((ImportStatementNode) ast);
                }
                
                // 代码生成
                CHTLGenerator generator = new CHTLGenerator(context);
                String output = generator.generate(ast);
                result.append(output);
                
            } catch (Exception e) {
                context.addError(new CompilationError(
                    "CHTL编译失败: " + e.getMessage(),
                    new SourceLocation(fragment.getSourcePath(), fragment.getStartLine(), 1),
                    CompilationError.ErrorType.SYNTAX_ERROR
                ));
            }
        }
        
        return result.toString();
    }
    
    /**
     * 编译CHTL JS片段
     */
    private String compileCHTLJS(List<Fragment> fragments, CompilationContext context) {
        StringBuilder result = new StringBuilder();
        
        // 添加CHTL JS运行时
        result.append(generateCHTLJSRuntime());
        result.append("\n");
        
        for (Fragment fragment : fragments) {
            try {
                // 词法分析
                CHTLJSStateMachineLexer lexer = new CHTLJSStateMachineLexer(new CHTLJSContext());
                List<CHTLJSToken> tokens = lexer.tokenize(fragment.getContent());
                
                // 语法分析
                CHTLJSParser parser = new CHTLJSParser(new CHTLJSContext());
                CHTLJSASTNode ast = parser.parse(tokens);
                
                // 代码生成
                CHTLJSGenerator generator = new CHTLJSGenerator();
                String output = generator.generate(ast);
                result.append(output);
                result.append("\n");
                
            } catch (Exception e) {
                context.addError(new CompilationError(
                    "CHTL JS编译失败: " + e.getMessage(),
                    new SourceLocation(fragment.getSourcePath(), fragment.getStartLine(), 1),
                    CompilationError.ErrorType.SYNTAX_ERROR
                ));
            }
        }
        
        return result.toString();
    }
    
    /**
     * 编译CSS片段
     */
    private String compileCSS(List<Fragment> fragments, CompilationContext context) {
        StringBuilder result = new StringBuilder();
        
        for (Fragment fragment : fragments) {
            // CSS直接输出（可以添加ANTLR解析器）
            result.append(fragment.getContent());
            result.append("\n");
        }
        
        return result.toString();
    }
    
    /**
     * 编译JavaScript片段
     */
    private String compileJS(List<Fragment> fragments, CompilationContext context) {
        StringBuilder result = new StringBuilder();
        
        for (Fragment fragment : fragments) {
            // JavaScript直接输出（可以添加ANTLR解析器）
            result.append(fragment.getContent());
            result.append("\n");
        }
        
        return result.toString();
    }
    
    /**
     * 生成最终输出
     */
    private String generateFinalOutput(CompilationOutput rawOutput, OptimizedOutput optimizedOutput) {
        if (optimizedOutput != null) {
            return optimizedOutput.getHtml();
        }
        
        // 未优化的输出
        StringBuilder output = new StringBuilder();
        
        // HTML
        output.append(rawOutput.getHtml());
        
        // 内联CSS
        if (!rawOutput.getCssFiles().isEmpty()) {
            output.append("\n<style>\n");
            for (String css : rawOutput.getCssFiles()) {
                output.append(css);
                output.append("\n");
            }
            output.append("</style>\n");
        }
        
        // 内联JavaScript
        if (!rawOutput.getJsFiles().isEmpty()) {
            output.append("\n<script>\n");
            for (String js : rawOutput.getJsFiles()) {
                output.append(js);
                output.append("\n");
            }
            output.append("</script>\n");
        }
        
        return output.toString();
    }
    
    /**
     * 生成CHTL JS运行时
     */
    private String generateCHTLJSRuntime() {
        return """
            // CHTL JS Runtime
            window.CHTL = {
                select: function(selector) {
                    if (selector.startsWith('#')) {
                        return document.getElementById(selector.substring(1));
                    } else if (selector.startsWith('.')) {
                        return document.querySelector(selector);
                    } else {
                        return document.querySelector(selector);
                    }
                },
                
                enhance: function(element) {
                    if (!element) return null;
                    
                    element.listen = function(event, handler) {
                        this.addEventListener(event, handler);
                        return this;
                    };
                    
                    element.delegate = function(event, selector, handler) {
                        this.addEventListener(event, function(e) {
                            if (e.target.matches(selector)) {
                                handler.call(e.target, e);
                            }
                        });
                        return this;
                    };
                    
                    element.animate = function(options) {
                        const duration = options.duration || 1000;
                        const from = options.from || {};
                        const to = options.to || {};
                        const startTime = performance.now();
                        
                        const animate = (currentTime) => {
                            const elapsed = currentTime - startTime;
                            const progress = Math.min(elapsed / duration, 1);
                            
                            for (let prop in from) {
                                if (to[prop] !== undefined) {
                                    const start = parseFloat(from[prop]);
                                    const end = parseFloat(to[prop]);
                                    const current = start + (end - start) * progress;
                                    this.style[prop] = current;
                                }
                            }
                            
                            if (progress < 1) {
                                requestAnimationFrame(animate);
                            }
                        };
                        
                        requestAnimationFrame(animate);
                        return this;
                    };
                    
                    return element;
                }
            };
            """;
    }
    
    /**
     * 创建错误结果
     */
    private CompilationResult createErrorResult(CompilationContext context, long startTime) {
        CompilationResult result = new CompilationResult();
        result.setSuccess(false);
        result.setErrors(context.getErrors());
        result.setWarnings(context.getWarnings());
        result.setCompilationTime(System.currentTimeMillis() - startTime);
        return result;
    }
    
    /**
     * 配置调试信息
     */
    private void configureDebugInfo(DebugInfo debugInfo) {
        if (options.isDebugMode()) {
            debugInfo.setLevel(DebugInfo.DebugLevel.DETAILED);
            debugInfo.setShowTokens(options.isShowTokens());
            debugInfo.setShowAST(options.isShowAST());
            debugInfo.setShowStates(options.isShowStates());
        }
    }
    
    /**
     * 关闭编译器
     */
    public void shutdown() {
        executorService.shutdown();
        try {
            if (!executorService.awaitTermination(60, TimeUnit.SECONDS)) {
                executorService.shutdownNow();
            }
        } catch (InterruptedException e) {
            executorService.shutdownNow();
        }
    }
    
    /**
     * 编译结果内部类
     */
    public static class CompilationOutput {
        private String html = "";
        private List<String> cssFiles = new ArrayList<>();
        private List<String> jsFiles = new ArrayList<>();
        
        // Getters and setters
        public String getHtml() { return html; }
        public void setHtml(String html) { this.html = html; }
        public List<String> getCssFiles() { return cssFiles; }
        public void setCssFiles(List<String> cssFiles) { this.cssFiles = cssFiles; }
        public List<String> getJsFiles() { return jsFiles; }
        public void setJsFiles(List<String> jsFiles) { this.jsFiles = jsFiles; }
    }
    
    /**
     * 优化输出内部类
     */
    public static class OptimizedOutput {
        private String html;
        private Map<String, String> sourceMaps;
        
        // Getters and setters
        public String getHtml() { return html; }
        public void setHtml(String html) { this.html = html; }
        public Map<String, String> getSourceMaps() { return sourceMaps; }
        public void setSourceMaps(Map<String, String> sourceMaps) { this.sourceMaps = sourceMaps; }
    }
}