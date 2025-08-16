#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <chrono>
#include "core/generator_comment_system.hpp"

using namespace chtl;

// 测试用例结构
struct CommentTestCase {
    std::string name;
    std::string sourceCode;
    std::vector<std::pair<std::string, CommentLanguageContext>> expectedComments;
};

// 颜色输出辅助函数
void printSuccess(const std::string& message) {
    std::cout << "\033[32m✓ " << message << "\033[0m" << std::endl;
}

void printError(const std::string& message) {
    std::cout << "\033[31m✗ " << message << "\033[0m" << std::endl;
}

void printInfo(const std::string& message) {
    std::cout << "\033[34mℹ " << message << "\033[0m" << std::endl;
}

// 测试HTML上下文注释生成
void testHtmlContextComments() {
    printInfo("测试HTML上下文注释生成...");
    
    std::string testCode = R"(
div {
    -- 这是一个主容器
    text { "Hello World" }
    
    p {
        -- 段落元素
        text { "这是段落内容" }
    }
}
)";

    GeneratorCommentSystem commentSystem;
    std::string result = commentSystem.processGeneratorComments(testCode);
    
    // 检查是否包含HTML注释
    if (result.find("<!-- 这是一个主容器 -->") != std::string::npos &&
        result.find("<!-- 段落元素 -->") != std::string::npos) {
        printSuccess("HTML上下文注释生成正确");
    } else {
        printError("HTML上下文注释生成失败");
        std::cout << "生成结果:\n" << result << std::endl;
    }
}

// 测试CSS上下文注释生成  
void testCssContextComments() {
    printInfo("测试CSS上下文注释生成...");
    
    std::string testCode = R"(
div {
    style {
        -- 主要样式设置
        color: red;
        background-color: blue;
        
        -- 边距设置
        margin: 10px;
        padding: 5px;
    }
}

[Origin] @Style globalStyles {
    -- 全局样式注释
    body {
        font-family: Arial;
    }
}
)";

    GeneratorCommentSystem commentSystem;
    std::string result = commentSystem.processGeneratorComments(testCode);
    
    // 检查是否包含CSS注释
    if (result.find("/* 主要样式设置 */") != std::string::npos &&
        result.find("/* 边距设置 */") != std::string::npos &&
        result.find("/* 全局样式注释 */") != std::string::npos) {
        printSuccess("CSS上下文注释生成正确");
    } else {
        printError("CSS上下文注释生成失败");
        std::cout << "生成结果:\n" << result << std::endl;
    }
}

// 测试JavaScript上下文注释生成
void testJavaScriptContextComments() {
    printInfo("测试JavaScript上下文注释生成...");
    
    std::string testCode = R"(
div {
    script {
        -- 初始化事件监听器
        this.addEventListener('click', function() {
            -- 点击处理逻辑
            console.log('Button clicked');
        });
        
        -- CHTL JS增强选择器
        {{button}}->listen('click', handler);
    }
}

[Origin] @JavaScript utils {
    -- 工具函数库
    function utility() {
        -- 实用函数实现
        return true;
    }
}
)";

    GeneratorCommentSystem commentSystem;
    std::string result = commentSystem.processGeneratorComments(testCode);
    
    // 检查是否包含JavaScript注释
    if (result.find("// 初始化事件监听器") != std::string::npos &&
        result.find("// 点击处理逻辑") != std::string::npos &&
        result.find("// CHTL JS增强选择器") != std::string::npos &&
        result.find("// 工具函数库") != std::string::npos &&
        result.find("// 实用函数实现") != std::string::npos) {
        printSuccess("JavaScript上下文注释生成正确");
    } else {
        printError("JavaScript上下文注释生成失败");
        std::cout << "生成结果:\n" << result << std::endl;
    }
}

// 测试混合上下文注释生成
void testMixedContextComments() {
    printInfo("测试混合上下文注释生成...");
    
    std::string testCode = R"(
-- 顶级HTML注释
div {
    -- HTML元素注释
    style {
        -- CSS样式注释
        color: red;
    }
    
    script {
        -- JavaScript脚本注释
        console.log('Hello');
    }
    
    -- 另一个HTML元素注释
    p {
        text { "Mixed content" }
    }
}
)";

    GeneratorCommentSystem commentSystem;
    std::string result = commentSystem.processGeneratorComments(testCode);
    
    // 检查混合注释类型
    bool hasHtmlComment = result.find("<!-- 顶级HTML注释 -->") != std::string::npos &&
                          result.find("<!-- HTML元素注释 -->") != std::string::npos &&
                          result.find("<!-- 另一个HTML元素注释 -->") != std::string::npos;
    
    bool hasCssComment = result.find("/* CSS样式注释 */") != std::string::npos;
    
    bool hasJsComment = result.find("// JavaScript脚本注释") != std::string::npos;
    
    if (hasHtmlComment && hasCssComment && hasJsComment) {
        printSuccess("混合上下文注释生成正确");
    } else {
        printError("混合上下文注释生成失败");
        std::cout << "HTML注释: " << (hasHtmlComment ? "✓" : "✗") << std::endl;
        std::cout << "CSS注释: " << (hasCssComment ? "✓" : "✗") << std::endl;
        std::cout << "JS注释: " << (hasJsComment ? "✓" : "✗") << std::endl;
        std::cout << "生成结果:\n" << result << std::endl;
    }
}

// 测试注释转义
void testCommentEscaping() {
    printInfo("测试注释转义...");
    
    std::string testCode = R"(
div {
    -- HTML注释包含--特殊字符
    style {
        -- CSS注释包含*/特殊字符
        color: red;
    }
    
    script {
        -- JS注释包含换行
第二行内容
        console.log('test');
    }
}
)";

    GeneratorCommentSystem commentSystem;
    std::string result = commentSystem.processGeneratorComments(testCode);
    
    // 检查转义是否正确
    bool htmlEscaped = result.find("<!-- HTML注释包含&#45;&#45;特殊字符 -->") != std::string::npos;
    bool cssEscaped = result.find("/* CSS注释包含*\\/特殊字符 */") != std::string::npos;
    bool jsEscaped = result.find("// JS注释包含换行 第二行内容") != std::string::npos;
    
    if (htmlEscaped && cssEscaped && jsEscaped) {
        printSuccess("注释转义正确");
    } else {
        printError("注释转义失败");
        std::cout << "HTML转义: " << (htmlEscaped ? "✓" : "✗") << std::endl;
        std::cout << "CSS转义: " << (cssEscaped ? "✓" : "✗") << std::endl;
        std::cout << "JS转义: " << (jsEscaped ? "✓" : "✗") << std::endl;
        std::cout << "生成结果:\n" << result << std::endl;
    }
}

// 测试上下文分析器
void testContextAnalyzer() {
    printInfo("测试上下文分析器...");
    
    CommentContextAnalyzer analyzer;
    
    // 测试不同上下文的分析
    std::string styleContext = "div { style { color: red; -- 这里是CSS注释 } }";
    std::string scriptContext = "div { script { console.log('test'); -- 这里是JS注释 } }";
    std::string htmlContext = "div { text { 'hello' } -- 这里是HTML注释 }";
    
    CommentPosition stylePos = {50, 70, 1, 50, " 这里是CSS注释 "};
    CommentPosition scriptPos = {60, 80, 1, 60, " 这里是JS注释 "};
    CommentPosition htmlPos = {40, 60, 1, 40, " 这里是HTML注释 "};
    
    auto styleResult = analyzer.analyzeContext(styleContext, stylePos);
    auto scriptResult = analyzer.analyzeContext(scriptContext, scriptPos);
    auto htmlResult = analyzer.analyzeContext(htmlContext, htmlPos);
    
    bool styleCorrect = (styleResult == CommentLanguageContext::CSS_CONTEXT);
    bool scriptCorrect = (scriptResult == CommentLanguageContext::JAVASCRIPT_CONTEXT);
    bool htmlCorrect = (htmlResult == CommentLanguageContext::HTML_CONTEXT);
    
    if (styleCorrect && scriptCorrect && htmlCorrect) {
        printSuccess("上下文分析器工作正确");
    } else {
        printError("上下文分析器失败");
        std::cout << "样式上下文: " << (styleCorrect ? "✓" : "✗") << std::endl;
        std::cout << "脚本上下文: " << (scriptCorrect ? "✓" : "✗") << std::endl;
        std::cout << "HTML上下文: " << (htmlCorrect ? "✓" : "✗") << std::endl;
    }
}

// 测试状态机
void testStateMachine() {
    printInfo("测试注释生成器状态机...");
    
    CommentGeneratorStateMachine stateMachine;
    std::string testCode = "div { style { color: red; } }";
    
    stateMachine.setSourceCode(testCode);
    
    // 模拟处理字符
    for (size_t i = 0; i < testCode.length(); ++i) {
        stateMachine.processCharacter(testCode[i], i);
        
        // 在样式块开始后检查状态
        if (testCode.substr(0, i + 1).find("style {") != std::string::npos) {
            auto context = stateMachine.getCurrentLanguageContext();
            if (context == CommentLanguageContext::CSS_CONTEXT) {
                printSuccess("状态机正确识别CSS上下文");
                return;
            }
        }
    }
    
    printError("状态机未能正确识别CSS上下文");
}

// 性能测试
void testPerformance() {
    printInfo("测试性能...");
    
    // 生成大量注释的测试代码
    std::string largeTestCode;
    for (int i = 0; i < 1000; ++i) {
        largeTestCode += "div { -- 注释" + std::to_string(i) + "\n";
        largeTestCode += "  style { -- CSS注释" + std::to_string(i) + "\n";
        largeTestCode += "    color: red; }\n";
        largeTestCode += "  script { -- JS注释" + std::to_string(i) + "\n";
        largeTestCode += "    console.log('test'); }\n";
        largeTestCode += "}\n";
    }
    
    GeneratorCommentSystem commentSystem;
    
    auto start = std::chrono::high_resolution_clock::now();
    std::string result = commentSystem.processGeneratorComments(largeTestCode);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "处理3000个注释耗时: " << duration.count() << "ms" << std::endl;
    
    if (duration.count() < 1000) { // 小于1秒认为性能合格
        printSuccess("性能测试通过");
    } else {
        printError("性能测试失败，处理时间过长");
    }
}

int main() {
    std::cout << "=== CHTL生成器注释系统测试 ===" << std::endl;
    std::cout << std::endl;
    
    try {
        // 基本功能测试
        testHtmlContextComments();
        testCssContextComments(); 
        testJavaScriptContextComments();
        testMixedContextComments();
        
        // 特殊情况测试
        testCommentEscaping();
        
        // 组件测试
        testContextAnalyzer();
        testStateMachine();
        
        // 性能测试
        testPerformance();
        
        std::cout << std::endl;
        printSuccess("所有测试完成！");
        
    } catch (const std::exception& e) {
        printError("测试过程中发生异常: " + std::string(e.what()));
        return 1;
    }
    
    return 0;
}