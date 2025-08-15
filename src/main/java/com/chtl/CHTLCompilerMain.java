package com.chtl;

import com.chtl.compiler.dispatcher.CompilerDispatcher;
import com.chtl.model.CodeFragment;
import com.chtl.model.CompilationResult;
import com.chtl.output.HTMLMerger;
import com.chtl.scanner.CHTLUnifiedScanner;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

/**
 * CHTL编译器主类
 * 整合所有组件，提供完整的编译流程
 */
public class CHTLCompilerMain {
    private static final Logger logger = LoggerFactory.getLogger(CHTLCompilerMain.class);
    
    private final CHTLUnifiedScanner scanner;
    private final CompilerDispatcher dispatcher;
    private final HTMLMerger merger;
    
    public CHTLCompilerMain() {
        this.scanner = new CHTLUnifiedScanner();
        this.dispatcher = new CompilerDispatcher();
        this.merger = new HTMLMerger();
        
        logger.info("CHTL编译器初始化完成");
    }
    
    /**
     * 编译CHTL文件
     * @param inputPath 输入文件路径
     * @param outputPath 输出文件路径
     */
    public void compileFile(String inputPath, String outputPath) throws IOException {
        logger.info("开始编译文件: {}", inputPath);
        
        // 1. 读取源文件
        String sourceCode = readFile(inputPath);
        
        // 2. 编译
        String result = compile(sourceCode);
        
        // 3. 写入结果
        writeFile(outputPath, result);
        
        logger.info("编译完成，输出文件: {}", outputPath);
    }
    
    /**
     * 编译CHTL源代码
     * @param sourceCode CHTL源代码
     * @return 编译后的HTML
     */
    public String compile(String sourceCode) {
        logger.debug("开始编译CHTL源代码");
        
        try {
            // 1. 扫描和切割代码
            List<CodeFragment> fragments = scanner.scan(sourceCode);
            logger.info("扫描完成，生成 {} 个代码片段", fragments.size());
            
            // 2. 调度编译
            List<CompilationResult> results = dispatcher.dispatch(fragments);
            logger.info("编译完成，生成 {} 个编译结果", results.size());
            
            // 3. 检查错误
            checkErrors(results);
            
            // 4. 合并结果
            String html = merger.merge(results);
            logger.info("合并完成，生成最终HTML");
            
            return html;
            
        } catch (Exception e) {
            logger.error("编译失败", e);
            throw new CompilationException("编译失败: " + e.getMessage(), e);
        }
    }
    
    /**
     * 检查编译错误
     */
    private void checkErrors(List<CompilationResult> results) {
        int errorCount = 0;
        int warningCount = 0;
        
        for (CompilationResult result : results) {
            if (result.hasErrors()) {
                for (String error : result.getErrors()) {
                    logger.error("[{}] 错误: {}", result.getSourceType(), error);
                    errorCount++;
                }
            }
            
            for (String warning : result.getWarnings()) {
                logger.warn("[{}] 警告: {}", result.getSourceType(), warning);
                warningCount++;
            }
        }
        
        if (errorCount > 0) {
            throw new CompilationException(
                String.format("编译失败，共 %d 个错误，%d 个警告", errorCount, warningCount)
            );
        }
        
        if (warningCount > 0) {
            logger.info("编译成功，但有 {} 个警告", warningCount);
        }
    }
    
    /**
     * 读取文件
     */
    private String readFile(String path) throws IOException {
        return Files.readString(Paths.get(path));
    }
    
    /**
     * 写入文件
     */
    private void writeFile(String path, String content) throws IOException {
        Path outputPath = Paths.get(path);
        Files.createDirectories(outputPath.getParent());
        Files.writeString(outputPath, content);
    }
    
    /**
     * 关闭编译器
     */
    public void shutdown() {
        dispatcher.shutdown();
        logger.info("CHTL编译器已关闭");
    }
    
    /**
     * 主方法 - 命令行入口
     */
    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("用法: java -jar chtl-compiler.jar <输入文件> <输出文件>");
            System.out.println("示例: java -jar chtl-compiler.jar index.chtl index.html");
            System.exit(1);
        }
        
        String inputFile = args[0];
        String outputFile = args[1];
        
        CHTLCompilerMain compiler = new CHTLCompilerMain();
        
        try {
            compiler.compileFile(inputFile, outputFile);
            System.out.println("编译成功！");
        } catch (Exception e) {
            System.err.println("编译失败: " + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        } finally {
            compiler.shutdown();
        }
    }
    
    /**
     * 编译异常
     */
    public static class CompilationException extends RuntimeException {
        public CompilationException(String message) {
            super(message);
        }
        
        public CompilationException(String message, Throwable cause) {
            super(message, cause);
        }
    }
}