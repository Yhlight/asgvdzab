package com.chtl.compiler.dispatcher;import com.chtl.compiler.CHTLCompiler;
import com.chtl.compiler.CompilationResult;


import com.chtl.compiler.chtl.CHTLCompiler;
import com.chtl.compiler.chtljs.CHTLJSCompiler;
import com.chtl.compiler.css.CSSCompiler;
import com.chtl.compiler.javascript.JavaScriptCompiler;
import com.chtl.model.CodeFragment;
import com.chtl.model.CompilationResult;
import com.chtl.model.FragmentType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * 编译器调度器
 * 负责将不同类型的代码片段分发给相应的编译器处理
 */
public class CompilerDispatcher {
    private static final Logger logger = LoggerFactory.getLogger(CompilerDispatcher.class);
    
    private final CHTLCompiler chtlCompiler;
    private final CHTLJSCompiler chtlJsCompiler;
    private final CSSCompiler cssCompiler;
    private final JavaScriptCompiler jsCompiler;
    
    private final ExecutorService executorService;
    
    public CompilerDispatcher() {
        this.chtlCompiler = new CHTLCompiler();
        this.chtlJsCompiler = new CHTLJSCompiler();
        this.cssCompiler = new CSSCompiler();
        this.jsCompiler = new JavaScriptCompiler();
        
        // 创建线程池用于并行编译
        this.executorService = Executors.newFixedThreadPool(
            Runtime.getRuntime().availableProcessors()
        );
        
        logger.info("编译器调度器初始化完成");
    }
    
    /**
     * 调度编译任务
     * @param fragments 代码片段列表
     * @return 编译结果列表
     */
    public List<CompilationResult> dispatch(List<CodeFragment> fragments) {
        logger.info("开始调度编译任务，共 {} 个片段", fragments.size());
        
        List<CompletableFuture<CompilationResult>> futures = new ArrayList<>();
        
        for (CodeFragment fragment : fragments) {
            CompletableFuture<CompilationResult> future = CompletableFuture
                .supplyAsync(() -> compileFragment(fragment), executorService);
            futures.add(future);
        }
        
        // 等待所有编译任务完成
        List<CompilationResult> results = new ArrayList<>();
        for (CompletableFuture<CompilationResult> future : futures) {
            try {
                results.add(future.get());
            } catch (Exception e) {
                logger.error("编译任务执行失败", e);
                results.add(new CompilationResult(
                    FragmentType.CHTL, 
                    "", 
                    "编译失败: " + e.getMessage()
                ));
            }
        }
        
        logger.info("编译调度完成，生成 {} 个结果", results.size());
        return results;
    }
    
    /**
     * 编译单个代码片段
     */
    private CompilationResult compileFragment(CodeFragment fragment) {
        logger.debug("编译 {} 类型的代码片段", fragment.getType());
        
        try {
            switch (fragment.getType()) {
                case CHTL:
                    return chtlCompiler.compile(fragment);
                    
                case CHTL_LOCAL_STYLE:
                    // 局部样式由CHTL编译器处理
                    return chtlCompiler.compileLocalStyle(fragment);
                    
                case CHTL_JS:
                    return chtlJsCompiler.compile(fragment);
                    
                case CSS:
                    return cssCompiler.compile(fragment);
                    
                case JAVASCRIPT:
                    return jsCompiler.compile(fragment);
                    
                default:
                    throw new IllegalArgumentException("未知的片段类型: " + fragment.getType());
            }
        } catch (Exception e) {
            logger.error("编译 {} 类型的片段时发生错误", fragment.getType(), e);
            return new CompilationResult(
                fragment.getType(), 
                "", 
                "编译错误: " + e.getMessage()
            );
        }
    }
    
    /**
     * 关闭调度器
     */
    public void shutdown() {
        logger.info("关闭编译器调度器");
        executorService.shutdown();
    }
}