package com.chtl.test;

import com.chtl.compiler.CHTLCompiler;
import com.chtl.context.CompilationContext;
import com.chtl.model.CodeFragment;
import com.chtl.scanner.*;
import java.io.*;
import java.nio.file.*;
import java.util.*;

/**
 * CHTL编译器集成测试套件
 * 测试完整的编译流程和各个组件的集成
 */
public class IntegrationTestSuite {
    
    private static final String TEST_OUTPUT_DIR = "test-output";
    private static int testsPassed = 0;
    private static int testsFailed = 0;
    private static List<String> failedTests = new ArrayList<>();
    
    public static void main(String[] args) {
        System.out.println("=== CHTL Compiler Integration Test Suite ===\n");
        
        // 创建测试输出目录
        createTestOutputDir();
        
        // 运行测试组
        runBasicTests();
        runCHTLJSTests();
        runModuleTests();
        runEdgeCaseTests();
        runPerformanceTests();
        
        // 打印测试结果
        printTestResults();
    }
    
    /**
     * 基础功能测试
     */
    private static void runBasicTests() {
        System.out.println("Running Basic Tests...\n");
        
        // 测试1: 基本HTML生成
        testCase("Basic HTML Generation", () -> {
            String input = """
                html {
                    head {
                        title {
                            text { "Test Page" }
                        }
                    }
                    body {
                        div {
                            id: main;
                            class: container;
                            text { "Hello CHTL!" }
                        }
                    }
                }
                """;
            
            String output = compile(input);
            assert output.contains("<html>");
            assert output.contains("<title>Test Page</title>");
            assert output.contains("<div id=\"main\" class=\"container\">");
            assert output.contains("Hello CHTL!");
        });
        
        // 测试2: 局部样式块
        testCase("Local Style Block", () -> {
            String input = """
                div {
                    style {
                        width: 100px;
                        height: 200px;
                        
                        .box {
                            background-color: red;
                        }
                        
                        &:hover {
                            opacity: 0.8;
                        }
                    }
                }
                """;
            
            String output = compile(input);
            assert output.contains("width: 100px");
            assert output.contains("height: 200px");
            assert output.contains(".box");
            assert output.contains("background-color: red");
            assert output.contains(":hover");
        });
        
        // 测试3: 模板使用
        testCase("Template Usage", () -> {
            String input = """
                [Template] @Style DefaultText {
                    color: black;
                    font-size: 16px;
                }
                
                div {
                    style {
                        @Style DefaultText;
                        background-color: white;
                    }
                }
                """;
            
            String output = compile(input);
            assert output.contains("color: black");
            assert output.contains("font-size: 16px");
            assert output.contains("background-color: white");
        });
    }
    
    /**
     * CHTL JS功能测试
     */
    private static void runCHTLJSTests() {
        System.out.println("\nRunning CHTL JS Tests...\n");
        
        // 测试1: 增强选择器
        testCase("Enhanced Selector", () -> {
            String input = """
                div {
                    class: box;
                    script {
                        {{box}}.addEventListener('click', function() {
                            console.log('Clicked!');
                        });
                    }
                }
                """;
            
            String output = compile(input);
            assert output.contains("querySelector");
            assert output.contains("addEventListener");
            assert output.contains("console.log('Clicked!')");
        });
        
        // 测试2: listen方法
        testCase("Listen Method", () -> {
            String input = """
                script {
                    {{button}}->listen({
                        click: function() {
                            alert('Button clicked');
                        },
                        mouseenter: function() {
                            this.style.opacity = '0.8';
                        }
                    });
                }
                """;
            
            String output = compile(input);
            assert output.contains("addEventListener");
            assert output.contains("click");
            assert output.contains("mouseenter");
            assert output.contains("alert('Button clicked')");
        });
        
        // 测试3: 动画函数
        testCase("Animate Function", () -> {
            String input = """
                script {
                    animate({
                        duration: 1000,
                        easing: ease-in-out,
                        begin: {
                            opacity: 0
                        },
                        end: {
                            opacity: 1
                        }
                    });
                }
                """;
            
            String output = compile(input);
            assert output.contains("duration");
            assert output.contains("1000");
            assert output.contains("ease-in-out");
            assert output.contains("opacity");
        });
    }
    
    /**
     * 模块系统测试
     */
    private static void runModuleTests() {
        System.out.println("\nRunning Module Tests...\n");
        
        // 测试1: 命名空间
        testCase("Namespace", () -> {
            String input = """
                [Namespace] mymodule {
                    [Custom] @Element Box {
                        div {
                            class: module-box;
                        }
                    }
                }
                
                body {
                    @Element Box from mymodule;
                }
                """;
            
            String output = compile(input);
            assert output.contains("module-box");
        });
        
        // 测试2: 约束系统
        testCase("Constraints", () -> {
            String input = """
                div {
                    except span, @Html;
                    
                    p { text { "Allowed" } }
                    // span和原始HTML应该被禁止
                }
                """;
            
            CompilationContext context = new CompilationContext();
            context.setConstraintsEnabled(true);
            String output = compile(input, context);
            assert output.contains("Allowed");
        });
    }
    
    /**
     * 边界情况测试
     */
    private static void runEdgeCaseTests() {
        System.out.println("\nRunning Edge Case Tests...\n");
        
        // 测试1: 空输入
        testCase("Empty Input", () -> {
            String output = compile("");
            assert output != null;
            assert output.isEmpty() || output.equals("<!DOCTYPE html>\n<html>\n</html>");
        });
        
        // 测试2: 深度嵌套
        testCase("Deep Nesting", () -> {
            StringBuilder input = new StringBuilder();
            int depth = 10;
            for (int i = 0; i < depth; i++) {
                input.append("div { ");
            }
            input.append("text { \"Deep\" }");
            for (int i = 0; i < depth; i++) {
                input.append(" }");
            }
            
            String output = compile(input.toString());
            assert output.contains("Deep");
            assert countOccurrences(output, "<div>") == depth;
        });
        
        // 测试3: 特殊字符处理
        testCase("Special Characters", () -> {
            String input = """
                div {
                    text { "Special: <>&\"'" }
                    data-attr: "value with \"quotes\"";
                }
                """;
            
            String output = compile(input);
            assert output.contains("&lt;");
            assert output.contains("&gt;");
            assert output.contains("&amp;");
            assert output.contains("&quot;");
        });
    }
    
    /**
     * 性能测试
     */
    private static void runPerformanceTests() {
        System.out.println("\nRunning Performance Tests...\n");
        
        // 测试1: 大文件编译
        testCase("Large File Compilation", () -> {
            StringBuilder input = new StringBuilder();
            // 生成1000个元素
            for (int i = 0; i < 1000; i++) {
                input.append(String.format("""
                    div {
                        id: element%d;
                        class: item;
                        style {
                            width: 100px;
                            height: 100px;
                        }
                        text { "Item %d" }
                    }
                    """, i, i));
            }
            
            long startTime = System.currentTimeMillis();
            String output = compile(input.toString());
            long duration = System.currentTimeMillis() - startTime;
            
            System.out.println("    Compiled 1000 elements in " + duration + "ms");
            assert output.contains("element999");
            assert duration < 5000; // 应该在5秒内完成
        });
        
        // 测试2: 片段扫描性能
        testCase("Fragment Scanning Performance", () -> {
            String complexCode = """
                script {
                    {{box}}.addEventListener('click', function() {
                        console.log('Start');
                        {{inner}}->animate({
                            duration: 1000,
                            begin: { opacity: 0 },
                            end: { opacity: 1 }
                        });
                        printMylove({
                            url: 'image.png',
                            mode: 'ASCII'
                        });
                    });
                    
                    const str = printMylove({ url: 'test.jpg', mode: 'Pixel' });
                    {{button[0]}}.style.color = 'red';
                }
                """;
            
            long startTime = System.currentTimeMillis();
            
            LanguageContextManager contextManager = new LanguageContextManager();
            PrecisionScanner scanner = new PrecisionScanner(contextManager);
            List<CodeFragment> fragments = scanner.scan(complexCode);
            
            long duration = System.currentTimeMillis() - startTime;
            
            System.out.println("    Scanned complex code in " + duration + "ms");
            System.out.println("    Generated " + fragments.size() + " fragments");
            assert duration < 100; // 应该在100ms内完成
        });
    }
    
    /**
     * 测试用例包装器
     */
    private static void testCase(String name, Runnable test) {
        System.out.print("  " + name + "... ");
        try {
            test.run();
            System.out.println("PASSED");
            testsPassed++;
        } catch (Exception | AssertionError e) {
            System.out.println("FAILED");
            System.out.println("    Error: " + e.getMessage());
            testsFailed++;
            failedTests.add(name);
        }
    }
    
    /**
     * 编译CHTL代码
     */
    private static String compile(String input) {
        return compile(input, new CompilationContext());
    }
    
    private static String compile(String input, CompilationContext context) {
        try {
            CHTLCompiler compiler = new CHTLCompiler(context);
            CHTLCompiler.CompilationResult result = compiler.compile(input, "test.chtl");
            
            if (result.hasErrors()) {
                throw new RuntimeException("Compilation errors: " + result.getErrors());
            }
            
            return result.getHtml();
        } catch (Exception e) {
            throw new RuntimeException("Compilation failed: " + e.getMessage(), e);
        }
    }
    
    /**
     * 统计字符串出现次数
     */
    private static int countOccurrences(String str, String substring) {
        int count = 0;
        int index = 0;
        while ((index = str.indexOf(substring, index)) != -1) {
            count++;
            index += substring.length();
        }
        return count;
    }
    
    /**
     * 创建测试输出目录
     */
    private static void createTestOutputDir() {
        try {
            Files.createDirectories(Paths.get(TEST_OUTPUT_DIR));
        } catch (IOException e) {
            System.err.println("Failed to create test output directory: " + e.getMessage());
        }
    }
    
    /**
     * 打印测试结果
     */
    private static void printTestResults() {
        System.out.println("\n=== Test Results ===");
        System.out.println("Total tests: " + (testsPassed + testsFailed));
        System.out.println("Passed: " + testsPassed);
        System.out.println("Failed: " + testsFailed);
        
        if (!failedTests.isEmpty()) {
            System.out.println("\nFailed tests:");
            for (String test : failedTests) {
                System.out.println("  - " + test);
            }
        }
        
        if (testsFailed == 0) {
            System.out.println("\nAll tests passed! ✅");
        } else {
            System.out.println("\nSome tests failed! ❌");
            System.exit(1);
        }
    }
}