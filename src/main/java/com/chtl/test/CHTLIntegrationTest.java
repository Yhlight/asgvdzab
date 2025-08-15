package com.chtl.test;

import com.chtl.compiler.CHTLCompiler;
import com.chtl.context.CompilationContext;
import com.chtl.compiler.debug.*;
import com.chtl.compiler.optimization.OptimizationOptions;

import java.io.*;
import java.nio.file.*;
import java.util.*;

/**
 * CHTL集成测试框架
 * 严格根据CHTL语法文档进行端到端测试
 */
public class CHTLIntegrationTest {
    private final Path testDir;
    private final CHTLCompiler compiler;
    private final List<TestCase> testCases = new ArrayList<>();
    private final TestReport report = new TestReport();
    
    public CHTLIntegrationTest(Path testDir) throws IOException {
        this.testDir = testDir;
        Files.createDirectories(testDir);
        this.compiler = new CHTLCompiler();
        initializeTestCases();
    }
    
    /**
     * 初始化测试用例 - 覆盖CHTL语法文档中的所有特性
     */
    private void initializeTestCases() {
        // 1. 基础语法测试
        addBasicSyntaxTests();
        
        // 2. 模板测试
        addTemplateTests();
        
        // 3. 自定义测试
        addCustomTests();
        
        // 4. 样式系统测试
        addStyleTests();
        
        // 5. CHTL JS测试
        addCHTLJSTests();
        
        // 6. 高级特性测试
        addAdvancedFeatureTests();
        
        // 7. 错误处理测试
        addErrorHandlingTests();
        
        // 8. 性能测试
        addPerformanceTests();
    }
    
    /**
     * 基础语法测试
     */
    private void addBasicSyntaxTests() {
        // 注释测试
        testCases.add(new TestCase("注释语法", """
            // 单行注释
            /* 多行注释
               继续注释 */
            body {
                text { Hello CHTL }
            }
            """, "<body>Hello CHTL</body>"));
        
        // 文本节点测试
        testCases.add(new TestCase("文本节点", """
            div {
                text { Hello World }
                text { "带引号的文本" }
                text { '单引号文本' }
            }
            """, "<div>Hello World带引号的文本单引号文本</div>"));
        
        // 属性测试
        testCases.add(new TestCase("属性语法", """
            div {
                id: "myDiv";
                class: "container";
                data-value = "123"; // CE对等式
                text { Content }
            }
            """, "<div id=\"myDiv\" class=\"container\" data-value=\"123\">Content</div>"));
        
        // 元素嵌套测试
        testCases.add(new TestCase("元素嵌套", """
            div {
                h1 {
                    text { Title }
                }
                p {
                    text { Paragraph }
                }
            }
            """, "<div><h1>Title</h1><p>Paragraph</p></div>"));
    }
    
    /**
     * 模板测试
     */
    private void addTemplateTests() {
        // 样式组模板
        testCases.add(new TestCase("样式组模板", """
            [Template] @Style RedText {
                color: red;
                font-weight: bold;
            }
            
            div {
                style {
                    @Style RedText;
                }
                text { Red Bold Text }
            }
            """, "<div style=\"color: red; font-weight: bold;\">Red Bold Text</div>"));
        
        // 元素模板
        testCases.add(new TestCase("元素模板", """
            [Template] @Element Card {
                div {
                    class: "card";
                    h3 {
                        text { Card Title }
                    }
                    p {
                        text { Card Content }
                    }
                }
            }
            
            body {
                @Element Card;
            }
            """, "<body><div class=\"card\"><h3>Card Title</h3><p>Card Content</p></div></body>"));
        
        // 变量组模板
        testCases.add(new TestCase("变量组模板", """
            [Template] @Var Colors {
                primary: "#007bff";
                secondary: "#6c757d";
            }
            
            div {
                style {
                    color: @Var Colors.primary;
                    background: @Var Colors.secondary;
                }
            }
            """, "<div style=\"color: #007bff; background: #6c757d;\"></div>"));
        
        // 模板继承
        testCases.add(new TestCase("模板继承", """
            [Template] @Style BaseText {
                font-size: 16px;
                line-height: 1.5;
            }
            
            [Template] @Style RedText {
                @Style BaseText;
                color: red;
            }
            
            p {
                style {
                    @Style RedText;
                }
                text { Inherited Styles }
            }
            """, "<p style=\"font-size: 16px; line-height: 1.5; color: red;\">Inherited Styles</p>"));
    }
    
    /**
     * 自定义测试
     */
    private void addCustomTests() {
        // 自定义样式组
        testCases.add(new TestCase("自定义样式组", """
            [Custom] @Style FlexCenter {
                display: flex;
                justify-content: ;
                align-items: ;
            }
            
            div {
                style {
                    @Style FlexCenter {
                        justify-content: center;
                        align-items: center;
                    }
                }
            }
            """, "<div style=\"display: flex; justify-content: center; align-items: center;\"></div>"));
        
        // 自定义元素特例化
        testCases.add(new TestCase("自定义元素特例化", """
            [Template] @Element Box {
                div {
                    class: "box";
                    text { Default Box }
                }
            }
            
            [Custom] @Element Box {
                delete text;
                text { Custom Box }
            }
            
            body {
                @Element Box;
            }
            """, "<body><div class=\"box\">Custom Box</div></body>"));
        
        // 索引访问
        testCases.add(new TestCase("索引访问", """
            [Template] @Element List {
                ul {
                    li { text { Item 1 } }
                    li { text { Item 2 } }
                    li { text { Item 3 } }
                }
            }
            
            [Custom] @Element List {
                ul > li[2] {
                    style {
                        color: red;
                    }
                }
            }
            
            @Element List;
            """, "<ul><li>Item 1</li><li style=\"color: red;\">Item 2</li><li>Item 3</li></ul>"));
    }
    
    /**
     * 样式系统测试
     */
    private void addStyleTests() {
        // 局部样式块
        testCases.add(new TestCase("局部样式块", """
            div {
                style {
                    color: blue;
                    font-size: 20px;
                }
                text { Styled Text }
            }
            """, "<div style=\"color: blue; font-size: 20px;\">Styled Text</div>"));
        
        // 自动化类名
        testCases.add(new TestCase("自动化类名", """
            div {
                style {
                    .container {
                        padding: 20px;
                        margin: 10px;
                    }
                }
                text { Auto Class }
            }
            """, contains("class=") && contains("padding: 20px") && contains("margin: 10px")));
        
        // 上下文推导
        testCases.add(new TestCase("上下文推导", """
            div {
                class: "my-box";
                style {
                    & {
                        border: 1px solid black;
                    }
                    &:hover {
                        background: #f0f0f0;
                    }
                }
            }
            """, contains(".my-box") && contains("border: 1px solid black") && contains(".my-box:hover")));
    }
    
    /**
     * CHTL JS测试
     */
    private void addCHTLJSTests() {
        // 局部script
        testCases.add(new TestCase("局部script", """
            div {
                id: "myDiv";
                script {
                    {{#myDiv}}->listen("click", () => {
                        console.log("Clicked!");
                    });
                }
                text { Click Me }
            }
            """, contains("getElementById") && contains("addEventListener") && contains("click")));
        
        // 增强选择器
        testCases.add(new TestCase("增强选择器", """
            script {
                let box = {{.box}};
                box->style->color = "red";
            }
            """, contains("querySelector(\".box\")") && contains(".style.color")));
        
        // 事件委托
        testCases.add(new TestCase("事件委托", """
            ul {
                id: "list";
                script {
                    {{#list}}->delegate("click", "li", (e) => {
                        console.log("Item clicked");
                    });
                }
            }
            """, contains("addEventListener") && contains("matches")));
        
        // 动画
        testCases.add(new TestCase("动画", """
            script {
                {{#box}}->animate({
                    duration: 1000,
                    from: { opacity: 0 },
                    to: { opacity: 1 }
                });
            }
            """, contains("requestAnimationFrame") && contains("opacity")));
    }
    
    /**
     * 高级特性测试
     */
    private void addAdvancedFeatureTests() {
        // 命名空间
        testCases.add(new TestCase("命名空间", """
            [Namespace] components {
                [Template] @Element Button {
                    button {
                        class: "btn";
                        text { Click }
                    }
                }
            }
            
            body {
                @Element Button from components;
            }
            """, "<body><button class=\"btn\">Click</button></body>"));
        
        // 约束
        testCases.add(new TestCase("约束系统", """
            [Namespace] restricted {
                except [Template];
                
                div {
                    text { Allowed }
                }
            }
            """, "<div>Allowed</div>"));
        
        // 原始嵌入
        testCases.add(new TestCase("原始嵌入", """
            [Origin] @Html
            <custom-element>Raw HTML</custom-element>
            [/Origin]
            
            div {
                text { Normal CHTL }
            }
            """, contains("<custom-element>Raw HTML</custom-element>") && contains("<div>Normal CHTL</div>")));
        
        // 导入
        testCases.add(new TestCase("导入测试", """
            [Import] @Style from "styles.css";
            [Import] @Chtl from "components.chtl" as comp;
            
            body {
                text { Import Test }
            }
            """, "<body>Import Test</body>"));
    }
    
    /**
     * 错误处理测试
     */
    private void addErrorHandlingTests() {
        // 语法错误
        testCases.add(new TestCase("语法错误检测", """
            div {
                style {
                    color: red  // 缺少分号
                    background: blue;
                }
            }
            """, null, true)); // 期望产生错误
        
        // 未定义引用
        testCases.add(new TestCase("未定义引用", """
            body {
                @Element UndefinedElement;
            }
            """, null, true));
        
        // 约束违反
        testCases.add(new TestCase("约束违反", """
            [Namespace] restricted {
                except [Template];
                
                [Template] @Element Forbidden {
                    div { }
                }
            }
            """, null, true));
    }
    
    /**
     * 性能测试
     */
    private void addPerformanceTests() {
        // 大文件测试
        StringBuilder largeFile = new StringBuilder();
        for (int i = 0; i < 1000; i++) {
            largeFile.append("div { text { Item ").append(i).append(" } }\n");
        }
        testCases.add(new TestCase("大文件性能", largeFile.toString(), null, false, true));
        
        // 深度嵌套测试
        StringBuilder deepNesting = new StringBuilder();
        for (int i = 0; i < 50; i++) {
            deepNesting.append("div { ");
        }
        deepNesting.append("text { Deep }");
        for (int i = 0; i < 50; i++) {
            deepNesting.append(" }");
        }
        testCases.add(new TestCase("深度嵌套性能", deepNesting.toString(), null, false, true));
    }
    
    /**
     * 运行所有测试
     */
    public void runAllTests() {
        System.out.println("=== CHTL 集成测试开始 ===\n");
        
        long startTime = System.currentTimeMillis();
        
        for (TestCase testCase : testCases) {
            runTest(testCase);
        }
        
        long totalTime = System.currentTimeMillis() - startTime;
        
        // 生成测试报告
        report.setTotalTime(totalTime);
        report.generateReport(testDir.resolve("test-report.html"));
        
        // 打印总结
        printSummary();
    }
    
    /**
     * 运行单个测试
     */
    private void runTest(TestCase testCase) {
        System.out.println("测试: " + testCase.getName());
        
        long startTime = System.currentTimeMillis();
        TestResult result = new TestResult(testCase.getName());
        
        try {
            // 创建测试文件
            Path testFile = testDir.resolve(testCase.getName().replaceAll("\\s+", "_") + ".chtl");
            Files.writeString(testFile, testCase.getInput());
            
            // 编译
            CompilationContext context = new CompilationContext();
            if (testCase.isPerformanceTest()) {
                context.setDebugMode(false);
            }
            
            String output = compiler.compile(testFile.toString(), context);
            
            // 验证结果
            if (testCase.isExpectError()) {
                if (context.hasErrors()) {
                    result.setStatus(TestStatus.PASSED);
                    result.setMessage("正确检测到错误");
                } else {
                    result.setStatus(TestStatus.FAILED);
                    result.setMessage("应该产生错误但没有");
                }
            } else if (testCase.getExpectedOutput() != null) {
                if (testCase.getExpectedOutput() instanceof String) {
                    String expected = (String) testCase.getExpectedOutput();
                    if (output.trim().equals(expected.trim())) {
                        result.setStatus(TestStatus.PASSED);
                    } else {
                        result.setStatus(TestStatus.FAILED);
                        result.setMessage("输出不匹配\n期望: " + expected + "\n实际: " + output);
                    }
                } else if (testCase.getExpectedOutput() instanceof TestMatcher) {
                    TestMatcher matcher = (TestMatcher) testCase.getExpectedOutput();
                    if (matcher.matches(output)) {
                        result.setStatus(TestStatus.PASSED);
                    } else {
                        result.setStatus(TestStatus.FAILED);
                        result.setMessage("输出不满足条件");
                    }
                }
            } else {
                // 只检查是否成功编译
                if (!context.hasErrors()) {
                    result.setStatus(TestStatus.PASSED);
                } else {
                    result.setStatus(TestStatus.FAILED);
                    result.setMessage("编译失败: " + context.getErrors().get(0).getMessage());
                }
            }
            
            result.setOutput(output);
            
        } catch (Exception e) {
            result.setStatus(TestStatus.ERROR);
            result.setMessage("测试异常: " + e.getMessage());
            result.setException(e);
        }
        
        long endTime = System.currentTimeMillis();
        result.setExecutionTime(endTime - startTime);
        
        report.addResult(result);
        
        // 打印结果
        System.out.println("  状态: " + result.getStatus());
        if (result.getMessage() != null) {
            System.out.println("  消息: " + result.getMessage());
        }
        if (testCase.isPerformanceTest()) {
            System.out.println("  执行时间: " + result.getExecutionTime() + "ms");
        }
        System.out.println();
    }
    
    /**
     * 打印总结
     */
    private void printSummary() {
        System.out.println("=== 测试总结 ===");
        System.out.println("总测试数: " + report.getTotalTests());
        System.out.println("通过: " + report.getPassedTests());
        System.out.println("失败: " + report.getFailedTests());
        System.out.println("错误: " + report.getErrorTests());
        System.out.println("总耗时: " + report.getTotalTime() + "ms");
        System.out.println("通过率: " + String.format("%.2f%%", report.getPassRate()));
    }
    
    /**
     * 测试用例
     */
    private static class TestCase {
        private final String name;
        private final String input;
        private final Object expectedOutput;
        private final boolean expectError;
        private final boolean isPerformanceTest;
        
        public TestCase(String name, String input, Object expectedOutput) {
            this(name, input, expectedOutput, false, false);
        }
        
        public TestCase(String name, String input, Object expectedOutput, boolean expectError) {
            this(name, input, expectedOutput, expectError, false);
        }
        
        public TestCase(String name, String input, Object expectedOutput, boolean expectError, boolean isPerformanceTest) {
            this.name = name;
            this.input = input;
            this.expectedOutput = expectedOutput;
            this.expectError = expectError;
            this.isPerformanceTest = isPerformanceTest;
        }
        
        // Getters
        public String getName() { return name; }
        public String getInput() { return input; }
        public Object getExpectedOutput() { return expectedOutput; }
        public boolean isExpectError() { return expectError; }
        public boolean isPerformanceTest() { return isPerformanceTest; }
    }
    
    /**
     * 测试匹配器
     */
    private interface TestMatcher {
        boolean matches(String output);
    }
    
    /**
     * 创建包含匹配器
     */
    private static TestMatcher contains(String... strings) {
        return output -> {
            for (String str : strings) {
                if (!output.contains(str)) {
                    return false;
                }
            }
            return true;
        };
    }
    
    /**
     * 主方法
     */
    public static void main(String[] args) throws IOException {
        Path testDir = Paths.get("test-output");
        CHTLIntegrationTest test = new CHTLIntegrationTest(testDir);
        test.runAllTests();
    }
}