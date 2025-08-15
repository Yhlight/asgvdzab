package com.chtl.test;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.chtl.compiler.CHTLCompiler;
import com.chtl.model.CompilationResult;
import com.chtl.compiler.CompilerOptions;
import com.chtl.test.PerformanceTest.PerformanceResult;


import com.chtl.compiler.*;
import com.chtl.context.CompilationContext;

import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;

/**
 * CHTL编译器性能测试
 * 测试各种场景下的编译性能
 */
public class PerformanceTest {
    private final CHTLCompiler compiler;
    private final Path testDir;
    private final List<PerformanceResult> results = new ArrayList<>();
    
    public PerformanceTest() throws IOException {
        this.compiler = new CHTLCompiler(CompilerOptions.production());
        this.testDir = Files.createTempDirectory("chtl_perf_test_");
    }
    
    /**
     * 运行所有性能测试
     */
    public void runAllTests() throws IOException {
        System.out.println("=== CHTL 性能测试 ===\n");
        
        // 预热JVM
        warmup();
        
        // 运行各种测试
        testSmallFile();
        testMediumFile();
        testLargeFile();
        testDeepNesting();
        testManyTemplates();
        testComplexStyles();
        testCHTLJS();
        testParallelCompilation();
        
        // 打印结果
        printResults();
        
        // 清理
        cleanup();
    }
    
    /**
     * JVM预热
     */
    private void warmup() throws IOException {
        System.out.println("预热JVM...");
        
        String warmupCode = """
            div {
                style { color: red; }
                text { Warmup }
            }
            """;
        
        for (int i = 0; i < 10; i++) {
            Path file = testDir.resolve("warmup" + i + ".chtl");
            Files.writeString(file, warmupCode);
            compiler.compile(file.toString());
        }
        
        System.out.println("预热完成\n");
    }
    
    /**
     * 测试小文件
     */
    private void testSmallFile() throws IOException {
        String code = """
            body {
                h1 { text { Hello World } }
                p { text { Simple paragraph } }
            }
            """;
        
        runTest("小文件 (< 10行)", code, 100);
    }
    
    /**
     * 测试中等文件
     */
    private void testMediumFile() throws IOException {
        StringBuilder code = new StringBuilder();
        
        // 生成50个元素
        for (int i = 0; i < 50; i++) {
            code.append(String.format("""
                div {
                    id: "div%d";
                    class: "item";
                    style {
                        padding: 10px;
                        margin: 5px;
                        background: #f0f0f0;
                    }
                    h3 { text { Item %d } }
                    p { text { Description for item %d } }
                }
                """, i, i, i));
        }
        
        runTest("中等文件 (50个元素)", code.toString(), 50);
    }
    
    /**
     * 测试大文件
     */
    private void testLargeFile() throws IOException {
        StringBuilder code = new StringBuilder();
        
        // 生成1000个元素
        for (int i = 0; i < 1000; i++) {
            code.append(String.format("""
                div {
                    id: "item-%d";
                    class: "list-item";
                    span { text { Item %d } }
                    button { text { Action } }
                }
                """, i, i));
        }
        
        runTest("大文件 (1000个元素)", code.toString(), 10);
    }
    
    /**
     * 测试深度嵌套
     */
    private void testDeepNesting() throws IOException {
        StringBuilder code = new StringBuilder();
        
        // 生成50层嵌套
        for (int i = 0; i < 50; i++) {
            code.append("div { ");
        }
        code.append("text { Deep nested } ");
        for (int i = 0; i < 50; i++) {
            code.append("} ");
        }
        
        runTest("深度嵌套 (50层)", code.toString(), 20);
    }
    
    /**
     * 测试多模板
     */
    private void testManyTemplates() throws IOException {
        StringBuilder code = new StringBuilder();
        
        // 生成100个模板
        for (int i = 0; i < 100; i++) {
            code.append(String.format("""
                [Template] @Element Component%d {
                    div {
                        class: "component-%d";
                        text { Component %d }
                    }
                }
                """, i, i, i));
        }
        
        // 使用所有模板
        code.append("body {\n");
        for (int i = 0; i < 100; i++) {
            code.append(String.format("    @Element Component%d;\n", i));
        }
        code.append("}\n");
        
        runTest("多模板 (100个)", code.toString(), 10);
    }
    
    /**
     * 测试复杂样式
     */
    private void testComplexStyles() throws IOException {
        StringBuilder code = new StringBuilder();
        
        code.append("""
            [Template] @Style BaseStyle {
                display: flex;
                justify-content: center;
                align-items: center;
                padding: 20px;
                margin: 10px;
                border: 1px solid #ddd;
                border-radius: 8px;
                background: linear-gradient(to right, #f0f0f0, #e0e0e0);
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
                transition: all 0.3s ease;
            }
            """);
        
        // 生成50个带复杂样式的元素
        for (int i = 0; i < 50; i++) {
            code.append(String.format("""
                div {
                    class: "styled-item-%d";
                    style {
                        @Style BaseStyle;
                        
                        &:hover {
                            transform: translateY(-2px);
                            box-shadow: 0 4px 8px rgba(0,0,0,0.15);
                        }
                        
                        .inner {
                            padding: 10px;
                            background: white;
                            border-radius: 4px;
                        }
                        
                        @media (max-width: 768px) {
                            flex-direction: column;
                            padding: 10px;
                        }
                    }
                    
                    div {
                        class: "inner";
                        text { Content %d }
                    }
                }
                """, i, i));
        }
        
        runTest("复杂样式", code.toString(), 20);
    }
    
    /**
     * 测试CHTL JS
     */
    private void testCHTLJS() throws IOException {
        StringBuilder code = new StringBuilder();
        
        // 生成带CHTL JS的元素
        for (int i = 0; i < 30; i++) {
            code.append(String.format("""
                div {
                    id: "interactive-%d";
                    button {
                        id: "btn-%d";
                        text { Click me %d }
                    }
                    p {
                        id: "output-%d";
                    }
                    
                    script {
                        let btn = {{#btn-%d}};
                        let output = {{#output-%d}};
                        let count = 0;
                        
                        btn->listen("click", () => {
                            count++;
                            output.textContent = `Clicked ${count} times`;
                        });
                        
                        btn->animate({
                            duration: 300,
                            from: { opacity: 0.5 },
                            to: { opacity: 1 }
                        });
                    }
                }
                """, i, i, i, i, i, i));
        }
        
        runTest("CHTL JS (30个交互组件)", code.toString(), 20);
    }
    
    /**
     * 测试并行编译
     */
    private void testParallelCompilation() throws IOException {
        System.out.println("测试并行编译性能...");
        
        // 创建多个文件
        List<Path> files = new ArrayList<>();
        for (int i = 0; i < 20; i++) {
            String code = String.format("""
                div {
                    id: "file-%d";
                    h1 { text { File %d } }
                    p { text { Content for file %d } }
                }
                """, i, i, i);
            
            Path file = testDir.resolve("parallel" + i + ".chtl");
            Files.writeString(file, code);
            files.add(file);
        }
        
        // 串行编译
        long serialStart = System.nanoTime();
        for (Path file : files) {
            compiler.compile(file.toString());
        }
        long serialTime = System.nanoTime() - serialStart;
        
        // 并行编译
        ExecutorService executor = Executors.newFixedThreadPool(4);
        long parallelStart = System.nanoTime();
        
        List<CompletableFuture<CompilationResult>> futures = files.stream()
            .map(file -> CompletableFuture.supplyAsync(() -> {
                try {
                    return compiler.compile(file.toString());
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }, executor))
            .toList();
        
        CompletableFuture.allOf(futures.toArray(new CompletableFuture[0])).join();
        long parallelTime = System.nanoTime() - parallelStart;
        
        executor.shutdown();
        
        // 记录结果
        PerformanceResult result = new PerformanceResult("并行编译 (20个文件)");
        result.serialTime = serialTime / 1_000_000; // 转换为毫秒
        result.parallelTime = parallelTime / 1_000_000;
        result.speedup = (double) serialTime / parallelTime;
        results.add(result);
        
        System.out.println(String.format("  串行: %dms, 并行: %dms, 加速比: %.2fx\n",
            result.serialTime, result.parallelTime, result.speedup));
    }
    
    /**
     * 运行单个测试
     */
    private void runTest(String name, String code, int iterations) throws IOException {
        System.out.println("测试: " + name);
        
        Path testFile = testDir.resolve("test.chtl");
        Files.writeString(testFile, code);
        
        // 预热
        for (int i = 0; i < 5; i++) {
            compiler.compile(testFile.toString());
        }
        
        // 正式测试
        List<Long> times = new ArrayList<>();
        for (int i = 0; i < iterations; i++) {
            // 清除缓存效果
            CompilationContext context = new CompilationContext();
            
            long start = System.nanoTime();
            CompilationResult result = compiler.compile(testFile.toString(), context);
            long end = System.nanoTime();
            
            if (result.isSuccess()) {
                times.add(end - start);
            }
        }
        
        // 计算统计信息
        PerformanceResult result = new PerformanceResult(name);
        result.iterations = iterations;
        result.avgTime = times.stream().mapToLong(Long::longValue).average().orElse(0) / 1_000_000;
        result.minTime = times.stream().mapToLong(Long::longValue).min().orElse(0) / 1_000_000;
        result.maxTime = times.stream().mapToLong(Long::longValue).max().orElse(0) / 1_000_000;
        
        // 计算标准差
        double variance = times.stream()
            .mapToDouble(t -> Math.pow(t / 1_000_000.0 - result.avgTime, 2))
            .average().orElse(0);
        result.stdDev = Math.sqrt(variance);
        
        result.fileSize = code.length();
        result.throughput = (result.fileSize / 1024.0) / (result.avgTime / 1000.0); // KB/s
        
        results.add(result);
        
        System.out.println(String.format("  平均: %.2fms, 最小: %.2fms, 最大: %.2fms, 标准差: %.2fms",
            result.avgTime, result.minTime, result.maxTime, result.stdDev));
        System.out.println(String.format("  文件大小: %d bytes, 吞吐量: %.2f KB/s\n",
            result.fileSize, result.throughput));
    }
    
    /**
     * 打印结果摘要
     */
    private void printResults() {
        System.out.println("=== 性能测试摘要 ===\n");
        
        System.out.println("测试结果:");
        System.out.println("┌─────────────────────────┬──────────┬──────────┬──────────┬──────────┬────────────┐");
        System.out.println("│ 测试名称                │ 平均时间 │ 最小时间 │ 最大时间 │ 标准差   │ 吞吐量     │");
        System.out.println("├─────────────────────────┼──────────┼──────────┼──────────┼──────────┼────────────┤");
        
        for (PerformanceResult result : results) {
            if (result.parallelTime == 0) {
                System.out.println(String.format("│ %-23s │ %7.1fms │ %7.1fms │ %7.1fms │ %7.1fms │ %8.1f KB/s │",
                    truncate(result.name, 23),
                    result.avgTime,
                    result.minTime,
                    result.maxTime,
                    result.stdDev,
                    result.throughput));
            }
        }
        
        System.out.println("└─────────────────────────┴──────────┴──────────┴──────────┴──────────┴────────────┘");
        
        // 性能建议
        System.out.println("\n性能优化建议:");
        analyzePerformance();
    }
    
    /**
     * 分析性能并给出建议
     */
    private void analyzePerformance() {
        PerformanceResult largeFileResult = results.stream()
            .filter(r -> r.name.contains("大文件"))
            .findFirst().orElse(null);
        
        if (largeFileResult != null && largeFileResult.avgTime > 1000) {
            System.out.println("- 大文件编译较慢，建议启用增量编译或拆分文件");
        }
        
        PerformanceResult deepNestingResult = results.stream()
            .filter(r -> r.name.contains("深度嵌套"))
            .findFirst().orElse(null);
        
        if (deepNestingResult != null && deepNestingResult.avgTime > 500) {
            System.out.println("- 深度嵌套影响性能，建议优化代码结构");
        }
        
        PerformanceResult parallelResult = results.stream()
            .filter(r -> r.name.contains("并行编译"))
            .findFirst().orElse(null);
        
        if (parallelResult != null && parallelResult.speedup < 2) {
            System.out.println("- 并行编译加速比较低，考虑调整并行度");
        }
        
        System.out.println("- 使用生产模式可以获得更好的运行时性能");
        System.out.println("- 合理使用模板可以减少编译时间");
    }
    
    /**
     * 清理测试文件
     */
    private void cleanup() throws IOException {
        Files.walk(testDir)
            .sorted(Comparator.reverseOrder())
            .forEach(path -> {
                try {
                    Files.delete(path);
                } catch (IOException e) {
                    // 忽略
                }
            });
        
        compiler.shutdown();
    }
    
    /**
     * 截断字符串
     */
    private String truncate(String str, int maxLength) {
        if (str.length() <= maxLength) return str;
        return str.substring(0, maxLength - 3) + "...";
    }
    
    /**
     * 性能测试结果
     */
    private static class PerformanceResult {
        String name;
        int iterations;
        double avgTime;
        double minTime;
        double maxTime;
        double stdDev;
        int fileSize;
        double throughput;
        
        // 并行测试专用
        long serialTime;
        long parallelTime;
        double speedup;
        
        PerformanceResult(String name) {
            this.name = name;
        }
    }
    
    /**
     * 主方法
     */
    public static void main(String[] args) {
        try {
            PerformanceTest test = new PerformanceTest();
            test.runAllTests();
        } catch (Exception e) {
            System.err.println("性能测试失败: " + e.getMessage());
            e.printStackTrace();
        }
    }
}