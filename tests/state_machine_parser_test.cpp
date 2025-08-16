#include "constraints/raw_embedding_parser.hpp"
#include <iostream>
#include <cassert>

using namespace chtl;

void testBasicParsing() {
    std::cout << "=== 基本解析测试 ===" << std::endl;
    
    RawEmbeddingParser parser;
    
    // 测试基本原始嵌入
    std::string basicTest = R"(
        [Origin] @Html {
            <div>Basic HTML</div>
        }
    )";
    
    auto blocks = parser.parseRawEmbeddings(basicTest);
    std::cout << "基本原始嵌入: " << (blocks.size() == 1 ? "✓" : "✗") << " 找到 " << blocks.size() << " 个块" << std::endl;
    
    if (!blocks.empty()) {
        std::cout << "  类型: " << blocks[0].type << std::endl;
        std::cout << "  内容: " << blocks[0].content << std::endl;
    }
    
    // 测试带名原始嵌入
    std::string namedTest = R"([Origin] @Vue component { <template><div>Vue</div></template> })";
    blocks = parser.parseRawEmbeddings(namedTest);
    std::cout << "带名原始嵌入: " << (blocks.size() == 1 ? "✓" : "✗") << " 找到 " << blocks.size() << " 个块" << std::endl;
    
    if (!blocks.empty()) {
        std::cout << "  类型: " << blocks[0].type << std::endl;
        std::cout << "  名称: " << blocks[0].name << std::endl;
        std::cout << "  内容: " << blocks[0].content << std::endl;
    }
    
    // 测试原始嵌入引用
    std::string refTest = "[Origin] @Html header;";
    blocks = parser.parseRawEmbeddings(refTest);
    std::cout << "原始嵌入引用: " << (blocks.size() == 1 ? "✓" : "✗") << " 找到 " << blocks.size() << " 个块" << std::endl;
    
    if (!blocks.empty()) {
        std::cout << "  类型: " << blocks[0].type << std::endl;
        std::cout << "  名称: " << blocks[0].name << std::endl;
        std::cout << "  是引用: " << (blocks[0].isReference ? "✓" : "✗") << std::endl;
    }
}

void testNestedBraces() {
    std::cout << "\n=== 嵌套大括号测试 ===" << std::endl;
    
    RawEmbeddingParser parser;
    
    // 测试嵌套大括号
    std::string nestedTest = R"(
        [Origin] @JavaScript {
            function test() {
                const obj = {
                    nested: {
                        data: "value"
                    }
                };
                if (condition) {
                    console.log("nested block");
                }
            }
        }
    )";
    
    auto blocks = parser.parseRawEmbeddings(nestedTest);
    std::cout << "嵌套大括号: " << (blocks.size() == 1 ? "✓" : "✗") << " 找到 " << blocks.size() << " 个块" << std::endl;
    
    if (!blocks.empty()) {
        std::cout << "  类型: " << blocks[0].type << std::endl;
        std::cout << "  内容包含嵌套: " << (blocks[0].content.find("nested") != std::string::npos ? "✓" : "✗") << std::endl;
    }
}

void testStringLiterals() {
    std::cout << "\n=== 字符串字面量测试 ===" << std::endl;
    
    RawEmbeddingParser parser;
    
    // 测试字符串中的大括号
    std::string stringTest = R"(
        [Origin] @JavaScript {
            const code = "function() { return '{nested}'; }";
            const template = `<div>{variable}</div>`;
            const regex = /\{.*\}/g;
        }
    )";
    
    auto blocks = parser.parseRawEmbeddings(stringTest);
    std::cout << "字符串字面量: " << (blocks.size() == 1 ? "✓" : "✗") << " 找到 " << blocks.size() << " 个块" << std::endl;
    
    if (!blocks.empty()) {
        std::cout << "  类型: " << blocks[0].type << std::endl;
        std::cout << "  内容包含字符串: " << (blocks[0].content.find("function()") != std::string::npos ? "✓" : "✗") << std::endl;
    }
}

void testFalsePositives() {
    std::cout << "\n=== 误匹配防护测试 ===" << std::endl;
    
    RawEmbeddingParser parser;
    
    // 测试不应该匹配的情况
    std::string falsePositiveTest = R"(
        // 这不是原始嵌入
        const array = [Origin, @Html, other];
        
        // 这也不是
        if (origin && @Html) {
            console.log("not a raw embedding");
        }
        
        // 字符串中的伪造内容
        const str = "[Origin] @Html { fake }";
        
        // 真正的原始嵌入
        [Origin] @Html {
            <div>Real content</div>
        }
        
        // 注释中的伪造内容
        /* [Origin] @Style { fake } */
    )";
    
    auto blocks = parser.parseRawEmbeddings(falsePositiveTest);
    std::cout << "误匹配防护: " << (blocks.size() == 1 ? "✓" : "✗") << " 找到 " << blocks.size() << " 个块（应该只有1个）" << std::endl;
    
    if (blocks.size() == 1) {
        std::cout << "  正确识别的类型: " << blocks[0].type << std::endl;
        std::cout << "  内容: " << blocks[0].content << std::endl;
    }
}

void testMultipleBlocks() {
    std::cout << "\n=== 多个块测试 ===" << std::endl;
    
    RawEmbeddingParser parser;
    
    std::string multipleTest = R"(
        // 第一个原始嵌入
        [Origin] @Html header {
            <header>Header content</header>
        }
        
        // 一些常规代码
        const config = { enabled: true };
        
        // 第二个原始嵌入
        [Origin] @Style {
            .header { color: blue; }
        }
        
        // 第三个原始嵌入（引用）
        [Origin] @Vue component;
        
        // 更多代码
        function process() {
            return "processed";
        }
        
        // 第四个原始嵌入
        [Origin] @React dashboard {
            const Dashboard = () => <div>Dashboard</div>;
        }
    )";
    
    auto blocks = parser.parseRawEmbeddings(multipleTest);
    std::cout << "多个块识别: " << (blocks.size() == 4 ? "✓" : "✗") << " 找到 " << blocks.size() << " 个块（应该有4个）" << std::endl;
    
    for (size_t i = 0; i < blocks.size(); i++) {
        std::cout << "  块 " << (i+1) << ": " << blocks[i].type;
        if (!blocks[i].name.empty()) {
            std::cout << " (" << blocks[i].name << ")";
        }
        if (blocks[i].isReference) {
            std::cout << " [引用]";
        }
        std::cout << std::endl;
    }
}

void testRemoveBlocks() {
    std::cout << "\n=== 移除块测试 ===" << std::endl;
    
    RawEmbeddingParser parser;
    
    std::string originalText = R"(
        const before = "before";
        
        [Origin] @Html {
            <div>Should be removed</div>
        }
        
        const middle = "middle";
        
        [Origin] @Style nav {
            .nav { color: red; }
        }
        
        const after = "after";
        
        [Origin] @Vue component;
    )";
    
    std::string cleaned = parser.removeRawEmbeddingBlocks(originalText);
    
    std::cout << "移除块测试:" << std::endl;
    std::cout << "  包含 'before': " << (cleaned.find("before") != std::string::npos ? "✓" : "✗") << std::endl;
    std::cout << "  包含 'middle': " << (cleaned.find("middle") != std::string::npos ? "✓" : "✗") << std::endl;
    std::cout << "  包含 'after': " << (cleaned.find("after") != std::string::npos ? "✓" : "✗") << std::endl;
    std::cout << "  不包含 '[Origin]': " << (cleaned.find("[Origin]") == std::string::npos ? "✓" : "✗") << std::endl;
    std::cout << "  不包含 'Should be removed': " << (cleaned.find("Should be removed") == std::string::npos ? "✓" : "✗") << std::endl;
    
    std::cout << "\n清理后的内容:" << std::endl;
    std::cout << cleaned << std::endl;
}

void testValidation() {
    std::cout << "\n=== 验证测试 ===" << std::endl;
    
    RawEmbeddingParser parser;
    
    // 有效的原始嵌入
    std::vector<std::string> validCases = {
        "[Origin] @Html { <div>content</div> }",
        "  [Origin] @Vue component { template }  ",
        "[Origin] @Style;",
        "[Origin] @React dashboard;"
    };
    
    // 无效的原始嵌入
    std::vector<std::string> invalidCases = {
        "some text [Origin] @Html { content }",
        "[Origin] @Html { content } more text",
        "const x = [Origin];",
        "[Origin] @Html { content } [Origin] @Style { css }"
    };
    
    std::cout << "有效原始嵌入验证:" << std::endl;
    for (const auto& testCase : validCases) {
        bool isValid = parser.isValidRawEmbedding(testCase);
        std::cout << "  '" << testCase << "': " << (isValid ? "✓" : "✗") << std::endl;
    }
    
    std::cout << "\n无效原始嵌入验证:" << std::endl;
    for (const auto& testCase : invalidCases) {
        bool isValid = parser.isValidRawEmbedding(testCase);
        std::cout << "  '" << testCase << "': " << (!isValid ? "✓" : "✗") << std::endl;
    }
}

int main() {
    try {
        std::cout << "🚀 CHTL状态机原始嵌入解析器测试" << std::endl;
        std::cout << "验证状态机能正确处理复杂嵌套和上下文\n" << std::endl;
        
        testBasicParsing();
        testNestedBraces();
        testStringLiterals();
        testFalsePositives();
        testMultipleBlocks();
        testRemoveBlocks();
        testValidation();
        
        std::cout << "\n🎉 状态机解析器测试完成!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "测试异常: " << e.what() << std::endl;
        return 1;
    }
}