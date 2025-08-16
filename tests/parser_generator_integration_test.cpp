#include <gtest/gtest.h>
#include "parsers/chtl_parser.hpp"
#include "generators/chtl_generator.hpp"
#include "ast/chtl_ast.hpp"
#include <iostream>

namespace chtl {

class CHTLParserGeneratorIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        parserConfig.enableStrictMode = true;
        parserConfig.enableWarnings = true;
        parserConfig.enableCommentsInAST = false;
        
        generatorConfig.enablePrettyPrint = true;
        generatorConfig.enableMinification = false;
        generatorConfig.enableComments = false;
        generatorConfig.cssPrefix = "chtl-";
    }
    
    CHTLParserConfig parserConfig;
    CHTLGeneratorConfig generatorConfig;
    
    // 辅助方法：完整的CHTL处理流程
    CHTLGenerateResult processClTL(const std::string& chtlSource) {
        CHTLParser parser(parserConfig);
        CHTLGenerator generator(generatorConfig);
        
        auto parseResult = parser.parse(chtlSource, "test.chtl");
        if (!parseResult.success) {
            std::cerr << "Parse errors:\n";
            for (const auto& error : parseResult.errors) {
                std::cerr << "  " << error.message << " at " 
                         << error.position.line << ":" << error.position.column << "\n";
            }
        }
        
        EXPECT_TRUE(parseResult.success) << "解析失败";
        
        if (parseResult.success) {
            return generator.generate(parseResult.ast, "test.chtl");
        }
        
        return CHTLGenerateResult{};
    }
};

// 测试基本的HTML元素生成
TEST_F(CHTLParserGeneratorIntegrationTest, testBasicElement) {
    std::string chtlSource = R"(
div
{
    id: container;
    class: main;
    
    text
    {
        Hello World
    }
}
)";
    
    auto result = processClTL(chtlSource);
    
    ASSERT_TRUE(result.success) << "生成失败";
    
    std::cout << "=== Generated HTML ===\n" << result.html << "\n";
    
    // 验证生成的HTML
    EXPECT_TRUE(result.html.find("<div") != std::string::npos);
    EXPECT_TRUE(result.html.find("id=\"container\"") != std::string::npos);
    EXPECT_TRUE(result.html.find("class=\"main\"") != std::string::npos);
    EXPECT_TRUE(result.html.find("Hello World") != std::string::npos);
    EXPECT_TRUE(result.html.find("</div>") != std::string::npos);
}

// 测试CE对等式 (: 和 = 等价)
TEST_F(CHTLParserGeneratorIntegrationTest, testCEEquivalence) {
    std::string chtlSource = R"(
div
{
    id: test1;
    class = test2;
    data-value: "with colon";
    data-name = "with equals";
}
)";
    
    auto result = processClTL(chtlSource);
    
    ASSERT_TRUE(result.success) << "CE对等式解析失败";
    
    std::cout << "=== CE Equivalence HTML ===\n" << result.html << "\n";
    
    // 验证两种赋值方式都正确处理
    EXPECT_TRUE(result.html.find("id=\"test1\"") != std::string::npos);
    EXPECT_TRUE(result.html.find("class=\"test2\"") != std::string::npos);
    EXPECT_TRUE(result.html.find("data-value=\"with colon\"") != std::string::npos);
    EXPECT_TRUE(result.html.find("data-name=\"with equals\"") != std::string::npos);
}

// 测试文本节点的字面量类型
TEST_F(CHTLParserGeneratorIntegrationTest, testTextLiterals) {
    std::string chtlSource = R"(
div
{
    text { "Double quoted text" }
    text { 'Single quoted text' }
    text { Unquoted text }
}
)";
    
    auto result = processClTL(chtlSource);
    
    ASSERT_TRUE(result.success) << "文本字面量解析失败";
    
    std::cout << "=== Text Literals HTML ===\n" << result.html << "\n";
    
    // 验证不同类型的字面量都被正确处理
    EXPECT_TRUE(result.html.find("Double quoted text") != std::string::npos);
    EXPECT_TRUE(result.html.find("Single quoted text") != std::string::npos);
    EXPECT_TRUE(result.html.find("Unquoted text") != std::string::npos);
}

// 测试局部样式块
TEST_F(CHTLParserGeneratorIntegrationTest, testLocalStyles) {
    std::string chtlSource = R"(
div
{
    style
    {
        width: 100px;
        height: 200px;
        background-color: red;
    }
    
    text { Styled div }
}
)";
    
    auto result = processClTL(chtlSource);
    
    ASSERT_TRUE(result.success) << "局部样式解析失败";
    
    std::cout << "=== Local Styles HTML ===\n" << result.html << "\n";
    std::cout << "=== Local Styles CSS ===\n" << result.css << "\n";
    
    // 验证元素自动添加了类名
    EXPECT_TRUE(result.html.find("class=\"chtl-cls-") != std::string::npos);
    
    // 验证CSS被提取
    EXPECT_FALSE(result.css.empty());
    EXPECT_TRUE(result.css.find("width: 100px") != std::string::npos);
    EXPECT_TRUE(result.css.find("height: 200px") != std::string::npos);
    EXPECT_TRUE(result.css.find("background-color: red") != std::string::npos);
}

// 测试模板系统
TEST_F(CHTLParserGeneratorIntegrationTest, testTemplateSystem) {
    std::string chtlSource = R"(
[Template] @Style DefaultButton
{
    padding: 10px;
    border: 1px solid black;
    background-color: white;
}

[Template] @Element ButtonElement
{
    button
    {
        type: button;
        
        text { Click me }
    }
}

[Template] @Var ThemeColors
{
    primaryColor: "#007bff";
    secondaryColor: "#6c757d";
}

div
{
    @Element ButtonElement;
}
)";
    
    auto result = processClTL(chtlSource);
    
    ASSERT_TRUE(result.success) << "模板系统解析失败";
    
    std::cout << "=== Template System HTML ===\n" << result.html << "\n";
    
    // 注意：完整的模板展开功能需要进一步实现
    // 这里主要验证模板声明能够正确解析
}

// 测试嵌套元素
TEST_F(CHTLParserGeneratorIntegrationTest, testNestedElements) {
    std::string chtlSource = R"(
html
{
    head
    {
        title
        {
            text { Test Page }
        }
    }
    
    body
    {
        div
        {
            class: container;
            
            h1
            {
                text { Welcome }
            }
            
            p
            {
                text { This is a test paragraph. }
            }
            
            ul
            {
                li { text { Item 1 } }
                li { text { Item 2 } }
                li { text { Item 3 } }
            }
        }
    }
}
)";
    
    auto result = processClTL(chtlSource);
    
    ASSERT_TRUE(result.success) << "嵌套元素解析失败";
    
    std::cout << "=== Nested Elements HTML ===\n" << result.html << "\n";
    
    // 验证HTML结构
    EXPECT_TRUE(result.html.find("<html>") != std::string::npos);
    EXPECT_TRUE(result.html.find("<head>") != std::string::npos);
    EXPECT_TRUE(result.html.find("<title>Test Page</title>") != std::string::npos);
    EXPECT_TRUE(result.html.find("<body>") != std::string::npos);
    EXPECT_TRUE(result.html.find("class=\"container\"") != std::string::npos);
    EXPECT_TRUE(result.html.find("<h1>Welcome</h1>") != std::string::npos);
    EXPECT_TRUE(result.html.find("<p>This is a test paragraph.</p>") != std::string::npos);
    EXPECT_TRUE(result.html.find("<ul>") != std::string::npos);
    EXPECT_TRUE(result.html.find("<li>Item 1</li>") != std::string::npos);
    EXPECT_TRUE(result.html.find("</html>") != std::string::npos);
}

// 测试自闭合标签
TEST_F(CHTLParserGeneratorIntegrationTest, testSelfClosingTags) {
    std::string chtlSource = R"(
div
{
    img
    {
        src: "test.jpg";
        alt: "Test Image";
    }
    
    br {}
    
    input
    {
        type: "text";
        name: "username";
    }
}
)";
    
    auto result = processClTL(chtlSource);
    
    ASSERT_TRUE(result.success) << "自闭合标签解析失败";
    
    std::cout << "=== Self-Closing Tags HTML ===\n" << result.html << "\n";
    
    // 验证自闭合标签格式
    EXPECT_TRUE(result.html.find("<img") != std::string::npos);
    EXPECT_TRUE(result.html.find("src=\"test.jpg\"") != std::string::npos);
    EXPECT_TRUE(result.html.find("alt=\"Test Image\"") != std::string::npos);
    EXPECT_TRUE(result.html.find("<br />") != std::string::npos);
    EXPECT_TRUE(result.html.find("<input") != std::string::npos);
    EXPECT_TRUE(result.html.find("type=\"text\"") != std::string::npos);
}

// 测试错误处理
TEST_F(CHTLParserGeneratorIntegrationTest, testErrorHandling) {
    std::string invalidChtlSource = R"(
div
{
    id: "unclosed quote;
    invalid-syntax here
}
)";
    
    CHTLParser parser(parserConfig);
    auto parseResult = parser.parse(invalidChtlSource, "invalid.chtl");
    
    // 验证错误被正确捕获
    EXPECT_FALSE(parseResult.success);
    EXPECT_FALSE(parseResult.errors.empty());
    
    std::cout << "=== Error Handling ===\n";
    for (const auto& error : parseResult.errors) {
        std::cout << "Error: " << error.message << " at " 
                 << error.position.line << ":" << error.position.column << "\n";
        if (!error.suggestion.empty()) {
            std::cout << "Suggestion: " << error.suggestion << "\n";
        }
    }
}

// 测试完整的CHTL文档
TEST_F(CHTLParserGeneratorIntegrationTest, testCompleteDocument) {
    std::string chtlSource = R"(
[Configuration]
{
    DEBUG_MODE = true;
}

[Template] @Style CardStyle
{
    border: 1px solid #ddd;
    border-radius: 8px;
    padding: 16px;
    margin: 8px;
    box-shadow: 0 2px 4px rgba(0,0,0,0.1);
}

html
{
    head
    {
        title { text { CHTL Test Document } }
        
        meta
        {
            charset: "UTF-8";
        }
        
        meta
        {
            name: "viewport";
            content: "width=device-width, initial-scale=1.0";
        }
    }
    
    body
    {
        div
        {
            class: "main-container";
            
            style
            {
                max-width: 1200px;
                margin: 0 auto;
                padding: 20px;
            }
            
            h1
            {
                text { "Welcome to CHTL" }
                
                style
                {
                    color: #333;
                    text-align: center;
                }
            }
            
            div
            {
                class: "card";
                
                style
                {
                    @Style CardStyle;
                }
                
                h2 { text { Feature 1 } }
                p { text { This is the first feature description. } }
            }
            
            div
            {
                class: "card";
                
                style
                {
                    @Style CardStyle;
                }
                
                h2 { text { Feature 2 } }
                p { text { This is the second feature description. } }
            }
        }
    }
}
)";
    
    auto result = processClTL(chtlSource);
    
    ASSERT_TRUE(result.success) << "完整文档解析失败";
    
    std::cout << "=== Complete Document HTML ===\n" << result.html << "\n";
    std::cout << "=== Complete Document CSS ===\n" << result.css << "\n";
    
    // 验证基本结构
    EXPECT_TRUE(result.html.find("<!DOCTYPE html>") == std::string::npos || 
                result.html.find("<html>") != std::string::npos);
    EXPECT_TRUE(result.html.find("CHTL Test Document") != std::string::npos);
    EXPECT_TRUE(result.html.find("Welcome to CHTL") != std::string::npos);
    EXPECT_TRUE(result.html.find("main-container") != std::string::npos);
    
    // 验证CSS生成
    EXPECT_FALSE(result.css.empty());
    EXPECT_TRUE(result.css.find("max-width: 1200px") != std::string::npos);
    EXPECT_TRUE(result.css.find("text-align: center") != std::string::npos);
}

} // namespace chtl