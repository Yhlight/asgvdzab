#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "parser/BasicSyntaxParser.h"
#include "lexer/CHTLLexer.h"

using namespace chtl;

void testBasicSyntaxParserCreation() {
    std::cout << "测试基础语法解析器创建..." << std::endl;
    
    BasicSyntaxParserConfig config;
    config.enable_ce_equivalence = true;
    config.enable_unquoted_literals = true;
    config.enable_generator_comments = true;
    config.strict_mode = false;
    
    BasicSyntaxParser parser(config);
    
    std::cout << "✓ 基础语法解析器创建成功" << std::endl;
    std::cout << "配置信息: " << parser.getDebugInfo() << std::endl;
}

void testCommentParsing() {
    std::cout << "\n测试注释解析..." << std::endl;
    
    BasicSyntaxParser parser;
    
    // 创建测试token
    std::vector<Token> tokens = {
        {TokenType::SINGLE_LINE_COMMENT, "//", 1, 1, 0},
        {TokenType::IDENTIFIER, "这是单行注释", 1, 3, 2},
        {TokenType::NEWLINE, "\n", 1, 15, 15},
        {TokenType::MULTI_LINE_COMMENT, "/*", 2, 1, 16},
        {TokenType::IDENTIFIER, "这是多行注释", 2, 3, 18},
        {TokenType::MULTI_LINE_COMMENT, "*/", 2, 15, 30},
        {TokenType::GENERATOR_COMMENT, "--", 3, 1, 32},
        {TokenType::IDENTIFIER, "这是生成器注释", 3, 3, 34}
    };
    
    size_t position = 0;
    
    // 测试单行注释
    auto single_comment = parser.parseComment(tokens, position);
    if (single_comment) {
        std::cout << "✓ 单行注释解析成功: " << single_comment->serialize() << std::endl;
    } else {
        std::cout << "✗ 单行注释解析失败" << std::endl;
    }
    
    // 测试多行注释
    auto multi_comment = parser.parseComment(tokens, position);
    if (multi_comment) {
        std::cout << "✓ 多行注释解析成功: " << multi_comment->serialize() << std::endl;
    } else {
        std::cout << "✗ 多行注释解析失败" << std::endl;
    }
    
    // 测试生成器注释
    auto generator_comment = parser.parseComment(tokens, position);
    if (generator_comment) {
        std::cout << "✓ 生成器注释解析成功: " << generator_comment->serialize() << std::endl;
    } else {
        std::cout << "✗ 生成器注释解析失败" << std::endl;
    }
}

void testTextNodeParsing() {
    std::cout << "\n测试文本节点解析..." << std::endl;
    
    BasicSyntaxParser parser;
    
    // 创建测试token
    std::vector<Token> tokens = {
        {TokenType::TEXT_LITERAL, "text", 1, 1, 0},
        {TokenType::WHITESPACE, " ", 1, 5, 4},
        {TokenType::LEFT_BRACE, "{", 1, 6, 5},
        {TokenType::IDENTIFIER, "这是文本内容", 1, 7, 6},
        {TokenType::RIGHT_BRACE, "}", 1, 20, 19}
    };
    
    size_t position = 0;
    
    auto text_node = parser.parseTextNode(tokens, position);
    if (text_node) {
        std::cout << "✓ 文本节点解析成功: " << text_node->serialize() << std::endl;
    } else {
        std::cout << "✗ 文本节点解析失败" << std::endl;
    }
}

void testLiteralParsing() {
    std::cout << "\n测试字面量解析..." << std::endl;
    
    BasicSyntaxParser parser;
    
    // 创建测试token
    std::vector<Token> tokens = {
        {TokenType::STRING_LITERAL, "\"", 1, 1, 0},
        {TokenType::IDENTIFIER, "双引号字符串", 1, 2, 1},
        {TokenType::STRING_LITERAL, "\"", 1, 13, 12},
        {TokenType::STRING_LITERAL, "'", 2, 1, 14},
        {TokenType::IDENTIFIER, "单引号字符串", 2, 2, 15},
        {TokenType::STRING_LITERAL, "'", 2, 13, 26},
        {TokenType::IDENTIFIER, "无修饰字面量", 3, 1, 28}
    };
    
    size_t position = 0;
    
    // 测试双引号字符串
    auto double_quote_literal = parser.parseLiteral(tokens, position);
    if (double_quote_literal) {
        std::cout << "✓ 双引号字符串解析成功: " << double_quote_literal->serialize() << std::endl;
    } else {
        std::cout << "✗ 双引号字符串解析失败" << std::endl;
    }
    
    // 测试单引号字符串
    auto single_quote_literal = parser.parseLiteral(tokens, position);
    if (single_quote_literal) {
        std::cout << "✓ 单引号字符串解析成功: " << single_quote_literal->serialize() << std::endl;
    } else {
        std::cout << "✗ 单引号字符串解析失败" << std::endl;
    }
    
    // 测试无修饰字面量
    auto unquoted_literal = parser.parseLiteral(tokens, position);
    if (unquoted_literal) {
        std::cout << "✓ 无修饰字面量解析成功: " << unquoted_literal->serialize() << std::endl;
    } else {
        std::cout << "✗ 无修饰字面量解析失败" << std::endl;
    }
}

void testHTMLElementParsing() {
    std::cout << "\n测试HTML元素解析..." << std::endl;
    
    BasicSyntaxParser parser;
    
    // 创建测试token
    std::vector<Token> tokens = {
        {TokenType::HTML_ELEMENT, "div", 1, 1, 0},
        {TokenType::WHITESPACE, " ", 1, 4, 3},
        {TokenType::LEFT_BRACE, "{", 1, 5, 4},
        {TokenType::IDENTIFIER, "id", 1, 6, 5},
        {TokenType::COLON, ":", 1, 8, 7},
        {TokenType::STRING_LITERAL, "\"", 1, 9, 8},
        {TokenType::IDENTIFIER, "box", 1, 10, 9},
        {TokenType::STRING_LITERAL, "\"", 1, 11, 10},
        {TokenType::SEMICOLON, ";", 1, 12, 11},
        {TokenType::WHITESPACE, " ", 1, 13, 12},
        {TokenType::IDENTIFIER, "class", 1, 14, 13},
        {TokenType::COLON, ":", 1, 19, 18},
        {TokenType::IDENTIFIER, "welcome", 1, 20, 19},
        {TokenType::SEMICOLON, ";", 1, 27, 26},
        {TokenType::RIGHT_BRACE, "}", 1, 28, 27}
    };
    
    size_t position = 0;
    
    auto html_element = parser.parseHTMLElement(tokens, position);
    if (html_element) {
        std::cout << "✓ HTML元素解析成功: " << html_element->serialize() << std::endl;
    } else {
        std::cout << "✗ HTML元素解析失败" << std::endl;
    }
}

void testAttributeParsing() {
    std::cout << "\n测试属性解析..." << std::endl;
    
    BasicSyntaxParser parser;
    
    // 创建测试token
    std::vector<Token> tokens = {
        {TokenType::IDENTIFIER, "id", 1, 1, 0},
        {TokenType::COLON, ":", 1, 3, 2},
        {TokenType::STRING_LITERAL, "\"", 1, 4, 3},
        {TokenType::IDENTIFIER, "box", 1, 5, 4},
        {TokenType::STRING_LITERAL, "\"", 1, 6, 5},
        {TokenType::SEMICOLON, ";", 1, 7, 6}
    };
    
    size_t position = 0;
    
    auto attribute = parser.parseAttribute(tokens, position);
    if (attribute) {
        std::cout << "✓ 属性解析成功: " << attribute->serialize() << std::endl;
    } else {
        std::cout << "✗ 属性解析失败" << std::endl;
    }
}

void testBasicSyntaxValidation() {
    std::cout << "\n测试基础语法验证..." << std::endl;
    
    BasicSyntaxParser parser;
    
    // 创建有效的测试token
    std::vector<Token> valid_tokens = {
        {TokenType::HTML_ELEMENT, "div", 1, 1, 0},
        {TokenType::WHITESPACE, " ", 1, 4, 3},
        {TokenType::LEFT_BRACE, "{", 1, 5, 4},
        {TokenType::IDENTIFIER, "id", 1, 6, 5},
        {TokenType::COLON, ":", 1, 8, 7},
        {TokenType::STRING_LITERAL, "\"", 1, 9, 8},
        {TokenType::IDENTIFIER, "box", 1, 10, 9},
        {TokenType::STRING_LITERAL, "\"", 1, 11, 10},
        {TokenType::SEMICOLON, ";", 1, 12, 11},
        {TokenType::RIGHT_BRACE, "}", 1, 13, 12}
    };
    
    bool is_valid = parser.validateBasicSyntax(valid_tokens);
    if (is_valid) {
        std::cout << "✓ 基础语法验证成功" << std::endl;
    } else {
        std::cout << "✗ 基础语法验证失败" << std::endl;
        auto errors = parser.getValidationErrors();
        for (const auto& error : errors) {
            std::cout << "  错误: " << error << std::endl;
        }
    }
}

void testDocumentParsing() {
    std::cout << "\n测试文档解析..." << std::endl;
    
    BasicSyntaxParser parser;
    
    // 创建简单的文档token
    std::vector<Token> document_tokens = {
        {TokenType::HTML_ELEMENT, "html", 1, 1, 0},
        {TokenType::WHITESPACE, " ", 1, 5, 4},
        {TokenType::LEFT_BRACE, "{", 1, 6, 5},
        {TokenType::HTML_ELEMENT, "head", 2, 5, 6},
        {TokenType::WHITESPACE, " ", 2, 9, 10},
        {TokenType::LEFT_BRACE, "{", 2, 10, 11},
        {TokenType::RIGHT_BRACE, "}", 2, 11, 12},
        {TokenType::HTML_ELEMENT, "body", 3, 5, 13},
        {TokenType::WHITESPACE, " ", 3, 9, 17},
        {TokenType::LEFT_BRACE, "{", 3, 10, 18},
        {TokenType::HTML_ELEMENT, "div", 4, 9, 19},
        {TokenType::WHITESPACE, " ", 4, 12, 22},
        {TokenType::LEFT_BRACE, "{", 4, 13, 23},
        {TokenType::TEXT_LITERAL, "text", 5, 13, 24},
        {TokenType::WHITESPACE, " ", 5, 17, 28},
        {TokenType::LEFT_BRACE, "{", 5, 18, 29},
        {TokenType::IDENTIFIER, "Hello World", 5, 19, 30},
        {TokenType::RIGHT_BRACE, "}", 5, 30, 41},
        {TokenType::RIGHT_BRACE, "}", 6, 9, 42},
        {TokenType::RIGHT_BRACE, "}", 7, 5, 43},
        {TokenType::RIGHT_BRACE, "}", 8, 1, 44}
    };
    
    auto document = parser.parseDocument(document_tokens);
    if (document) {
        std::cout << "✓ 文档解析成功: " << document->serialize() << std::endl;
    } else {
        std::cout << "✗ 文档解析失败" << std::endl;
        auto errors = parser.getErrors();
        for (const auto& error : errors) {
            std::cout << "  错误: " << error << std::endl;
        }
    }
}

int main() {
    std::cout << "CHTL基础语法解析器测试" << std::endl;
    std::cout << "=======================" << std::endl << std::endl;
    
    try {
        testBasicSyntaxParserCreation();
        testCommentParsing();
        testTextNodeParsing();
        testLiteralParsing();
        testAttributeParsing();
        testBasicSyntaxValidation();
        testDocumentParsing();
        
        std::cout << "\n所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}