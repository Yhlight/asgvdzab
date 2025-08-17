#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "parser/BasicSyntaxParser.h"
#include "lexer/CHTLLexer.h"

using namespace chtl;

void testBasicSyntaxParserCreation() {
    std::cout << "=== 测试基础语法解析器创建 ===" << std::endl;
    
    // 测试默认配置
    BasicSyntaxParserConfig default_config;
    auto parser1 = std::make_unique<BasicSyntaxParser>(default_config);
    
    std::cout << "默认配置基础语法解析器创建成功" << std::endl;
    std::cout << "CE等价性: " << (default_config.enable_ce_equivalence ? "启用" : "禁用") << std::endl;
    std::cout << "无修饰字面量: " << (default_config.enable_unquoted_literals ? "启用" : "禁用") << std::endl;
    std::cout << "生成器注释: " << (default_config.enable_generator_comments ? "启用" : "禁用") << std::endl;
    std::cout << "严格模式: " << (default_config.strict_mode ? "启用" : "禁用") << std::endl;
    std::cout << "最大文本长度: " << default_config.max_text_length << std::endl;
    std::cout << "最大注释长度: " << default_config.max_comment_length << std::endl;
    
    // 测试自定义配置
    BasicSyntaxParserConfig custom_config;
    custom_config.enable_ce_equivalence = false;
    custom_config.enable_unquoted_literals = false;
    custom_config.strict_mode = true;
    custom_config.max_text_length = 5000;
    custom_config.max_comment_length = 500;
    
    auto parser2 = std::make_unique<BasicSyntaxParser>(custom_config);
    
    std::cout << "自定义配置基础语法解析器创建成功" << std::endl;
    std::cout << "CE等价性: " << (custom_config.enable_ce_equivalence ? "启用" : "禁用") << std::endl;
    std::cout << "无修饰字面量: " << (custom_config.enable_unquoted_literals ? "启用" : "禁用") << std::endl;
    std::cout << "严格模式: " << (custom_config.strict_mode ? "启用" : "禁用") << std::endl;
    std::cout << "最大文本长度: " << custom_config.max_text_length << std::endl;
    std::cout << "最大注释长度: " << custom_config.max_comment_length << std::endl;
    
    std::cout << std::endl;
}

void testCommentParsing() {
    std::cout << "=== 测试注释解析 ===" << std::endl;
    
    BasicSyntaxParserConfig config;
    config.enable_debug = true;
    auto parser = std::make_unique<BasicSyntaxParser>(config);
    
    // 创建测试token
    std::vector<Token> tokens;
    
    // 单行注释
    tokens.push_back(Token{TokenType::COMMENT, "// 这是一个单行注释", 1, 1});
    
    // 多行注释
    tokens.push_back(Token{TokenType::COMMENT, "/* 这是一个多行注释 */", 2, 1});
    
    // 生成器注释
    tokens.push_back(Token{TokenType::COMMENT, "-- 这是一个生成器注释", 3, 1});
    
    size_t position = 0;
    
    // 测试单行注释解析
    auto comment1 = parser->parseComment(tokens, position);
    if (comment1) {
        std::cout << "单行注释解析成功: " << comment1->getNodeTypeName() << std::endl;
    } else {
        std::cout << "单行注释解析失败" << std::endl;
    }
    
    // 测试多行注释解析
    auto comment2 = parser->parseComment(tokens, position);
    if (comment2) {
        std::cout << "多行注释解析成功: " << comment2->getNodeTypeName() << std::endl;
    } else {
        std::cout << "多行注释解析失败" << std::endl;
    }
    
    // 测试生成器注释解析
    auto comment3 = parser->parseComment(tokens, position);
    if (comment3) {
        std::cout << "生成器注释解析成功: " << comment3->getNodeTypeName() << std::endl;
    } else {
        std::cout << "生成器注释解析失败" << std::endl;
    }
    
    std::cout << std::endl;
}

void testTextNodeParsing() {
    std::cout << "=== 测试文本节点解析 ===" << std::endl;
    
    BasicSyntaxParserConfig config;
    config.enable_debug = true;
    auto parser = std::make_unique<BasicSyntaxParser>(config);
    
    // 创建测试token
    std::vector<Token> tokens;
    
    // text { "这是一段文本" }
    tokens.push_back(Token{TokenType::IDENTIFIER, "text", 1, 1});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 1, 6});
    tokens.push_back(Token{TokenType::STRING, "\"这是一段文本\"", 1, 8});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 1, 20});
    
    size_t position = 0;
    
    // 测试文本节点解析
    auto text_node = parser->parseTextNode(tokens, position);
    if (text_node) {
        std::cout << "文本节点解析成功: " << text_node->getNodeTypeName() << std::endl;
    } else {
        std::cout << "文本节点解析失败" << std::endl;
    }
    
    std::cout << std::endl;
}

void testLiteralParsing() {
    std::cout << "=== 测试字面量解析 ===" << std::endl;
    
    BasicSyntaxParserConfig config;
    config.enable_debug = true;
    auto parser = std::make_unique<BasicSyntaxParser>(config);
    
    // 创建测试token
    std::vector<Token> tokens;
    
    // 双引号字符串
    tokens.push_back(Token{TokenType::STRING, "\"双引号字符串\"", 1, 1});
    
    // 单引号字符串
    tokens.push_back(Token{TokenType::STRING, "'单引号字符串'", 2, 1});
    
    // 无修饰字面量
    tokens.push_back(Token{TokenType::IDENTIFIER, "无修饰字面量", 3, 1});
    
    size_t position = 0;
    
    // 测试双引号字符串解析
    auto literal1 = parser->parseLiteral(tokens, position);
    if (literal1) {
        std::cout << "双引号字符串解析成功: " << literal1->getNodeTypeName() << std::endl;
    } else {
        std::cout << "双引号字符串解析失败" << std::endl;
    }
    
    // 测试单引号字符串解析
    auto literal2 = parser->parseLiteral(tokens, position);
    if (literal2) {
        std::cout << "单引号字符串解析成功: " << literal2->getNodeTypeName() << std::endl;
    } else {
        std::cout << "单引号字符串解析失败" << std::endl;
    }
    
    // 测试无修饰字面量解析
    auto literal3 = parser->parseLiteral(tokens, position);
    if (literal3) {
        std::cout << "无修饰字面量解析成功: " << literal3->getNodeTypeName() << std::endl;
    } else {
        std::cout << "无修饰字面量解析失败" << std::endl;
    }
    
    std::cout << std::endl;
}

void testHTMLElementParsing() {
    std::cout << "=== 测试HTML元素解析 ===" << std::endl;
    
    BasicSyntaxParserConfig config;
    config.enable_debug = true;
    auto parser = std::make_unique<BasicSyntaxParser>(config);
    
    // 创建测试token
    std::vector<Token> tokens;
    
    // div { id: "box"; class: "welcome"; text { "HelloWorld" } }
    tokens.push_back(Token{TokenType::IDENTIFIER, "div", 1, 1});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 1, 5});
    tokens.push_back(Token{TokenType::IDENTIFIER, "id", 2, 5});
    tokens.push_back(Token{TokenType::COLON, ":", 2, 8});
    tokens.push_back(Token{TokenType::STRING, "\"box\"", 2, 10});
    tokens.push_back(Token{TokenType::SEMICOLON, ";", 2, 15});
    tokens.push_back(Token{TokenType::IDENTIFIER, "class", 3, 5});
    tokens.push_back(Token{TokenType::COLON, ":", 3, 11});
    tokens.push_back(Token{TokenType::STRING, "\"welcome\"", 3, 13});
    tokens.push_back(Token{TokenType::SEMICOLON, ";", 3, 22});
    tokens.push_back(Token{TokenType::IDENTIFIER, "text", 4, 5});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 4, 10});
    tokens.push_back(Token{TokenType::STRING, "\"HelloWorld\"", 4, 12});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 4, 24});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 5, 1});
    
    size_t position = 0;
    
    // 测试HTML元素解析
    auto element = parser->parseHTMLElement(tokens, position);
    if (element) {
        std::cout << "HTML元素解析成功: " << element->getNodeTypeName() << std::endl;
        std::cout << "子节点数量: " << element->getChildren().size() << std::endl;
    } else {
        std::cout << "HTML元素解析失败" << std::endl;
    }
    
    std::cout << std::endl;
}

void testAttributeParsing() {
    std::cout << "=== 测试属性解析 ===" << std::endl;
    
    BasicSyntaxParserConfig config;
    config.enable_debug = true;
    auto parser = std::make_unique<BasicSyntaxParser>(config);
    
    // 创建测试token
    std::vector<Token> tokens;
    
    // id: "box";
    tokens.push_back(Token{TokenType::IDENTIFIER, "id", 1, 1});
    tokens.push_back(Token{TokenType::COLON, ":", 1, 4});
    tokens.push_back(Token{TokenType::STRING, "\"box\"", 1, 6});
    tokens.push_back(Token{TokenType::SEMICOLON, ";", 1, 11});
    
    size_t position = 0;
    
    // 测试属性解析
    auto attribute = parser->parseAttribute(tokens, position);
    if (attribute) {
        std::cout << "属性解析成功: " << attribute->getNodeTypeName() << std::endl;
    } else {
        std::cout << "属性解析失败" << std::endl;
    }
    
    std::cout << std::endl;
}

void testCEEquivalence() {
    std::cout << "=== 测试CE等价性 ===" << std::endl;
    
    BasicSyntaxParserConfig config;
    config.enable_debug = true;
    config.enable_ce_equivalence = true;
    auto parser = std::make_unique<BasicSyntaxParser>(config);
    
    // 创建测试token
    std::vector<Token> tokens;
    
    // 使用 = 代替 : 的属性
    tokens.push_back(Token{TokenType::IDENTIFIER, "id", 1, 1});
    tokens.push_back(Token{TokenType::EQUALS, "=", 1, 4});
    tokens.push_back(Token{TokenType::STRING, "\"box\"", 1, 6});
    tokens.push_back(Token{TokenType::SEMICOLON, ";", 1, 11});
    
    size_t position = 0;
    
    // 测试CE等价性
    auto attribute = parser->parseAttribute(tokens, position);
    if (attribute) {
        std::cout << "CE等价性测试成功: 使用 = 代替 : 解析成功" << std::endl;
    } else {
        std::cout << "CE等价性测试失败: 使用 = 代替 : 解析失败" << std::endl;
    }
    
    std::cout << std::endl;
}

void testBasicSyntaxValidation() {
    std::cout << "=== 测试基础语法验证 ===" << std::endl;
    
    BasicSyntaxParserConfig config;
    config.enable_debug = true;
    auto parser = std::make_unique<BasicSyntaxParser>(config);
    
    // 创建测试token
    std::vector<Token> tokens;
    
    // 有效的CHTL代码
    tokens.push_back(Token{TokenType::IDENTIFIER, "html", 1, 1});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 1, 6});
    tokens.push_back(Token{TokenType::IDENTIFIER, "head", 2, 5});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 2, 10});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 3, 5});
    tokens.push_back(Token{TokenType::IDENTIFIER, "body", 4, 5});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 4, 10});
    tokens.push_back(Token{TokenType::IDENTIFIER, "div", 5, 9});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 5, 13});
    tokens.push_back(Token{TokenType::IDENTIFIER, "text", 6, 13});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 6, 18});
    tokens.push_back(Token{TokenType::STRING, "\"Hello World\"", 6, 20});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 7, 13});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 8, 9});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 9, 5});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 10, 1});
    
    // 测试语法验证
    bool is_valid = parser->validateBasicSyntax(tokens);
    if (is_valid) {
        std::cout << "基础语法验证成功: 代码结构有效" << std::endl;
    } else {
        std::cout << "基础语法验证失败: 代码结构无效" << std::endl;
        
        std::vector<std::string> validation_errors = parser->getValidationErrors();
        std::cout << "验证错误:" << std::endl;
        for (const auto& error : validation_errors) {
            std::cout << "  " << error << std::endl;
        }
    }
    
    std::cout << std::endl;
}

void testDocumentParsing() {
    std::cout << "=== 测试文档解析 ===" << std::endl;
    
    BasicSyntaxParserConfig config;
    config.enable_debug = true;
    auto parser = std::make_unique<BasicSyntaxParser>(config);
    
    // 创建测试token
    std::vector<Token> tokens;
    
    // 完整的CHTL文档
    tokens.push_back(Token{TokenType::COMMENT, "// CHTL文档示例", 1, 1});
    tokens.push_back(Token{TokenType::IDENTIFIER, "html", 2, 1});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 2, 6});
    tokens.push_back(Token{TokenType::IDENTIFIER, "head", 3, 5});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 3, 10});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 4, 5});
    tokens.push_back(Token{TokenType::IDENTIFIER, "body", 5, 5});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 5, 10});
    tokens.push_back(Token{TokenType::IDENTIFIER, "div", 6, 9});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 6, 13});
    tokens.push_back(Token{TokenType::IDENTIFIER, "id", 7, 13});
    tokens.push_back(Token{TokenType::COLON, ":", 7, 16});
    tokens.push_back(Token{TokenType::STRING, "\"main\"", 7, 18});
    tokens.push_back(Token{TokenType::SEMICOLON, ";", 7, 24});
    tokens.push_back(Token{TokenType::IDENTIFIER, "text", 8, 13});
    tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 8, 18});
    tokens.push_back(Token{TokenType::STRING, "\"欢迎使用CHTL\"", 8, 20});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 9, 13});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 10, 9});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 11, 5});
    tokens.push_back(Token{TokenType::BRACE_CLOSE, "}", 12, 1});
    
    // 测试文档解析
    auto document = parser->parseDocument(tokens);
    if (document) {
        std::cout << "文档解析成功: " << document->getNodeTypeName() << std::endl;
        std::cout << "根节点子节点数量: " << document->getChildren().size() << std::endl;
    } else {
        std::cout << "文档解析失败" << std::endl;
    }
    
    std::cout << std::endl;
}

void testErrorHandling() {
    std::cout << "=== 测试错误处理 ===" << std::endl;
    
    BasicSyntaxParserConfig config;
    config.enable_debug = true;
    auto parser = std::make_unique<BasicSyntaxParser>(config);
    
    // 测试无效的token序列
    std::vector<Token> invalid_tokens;
    invalid_tokens.push_back(Token{TokenType::IDENTIFIER, "div", 1, 1});
    invalid_tokens.push_back(Token{TokenType::BRACE_OPEN, "{", 1, 5});
    // 缺少结束括号
    
    size_t position = 0;
    
    // 测试HTML元素解析（应该失败）
    auto element = parser->parseHTMLElement(invalid_tokens, position);
    if (element) {
        std::cout << "HTML元素解析成功（意外）" << std::endl;
    } else {
        std::cout << "HTML元素解析失败（预期）" << std::endl;
    }
    
    // 检查错误信息
    if (parser->hasErrors()) {
        std::vector<std::string> errors = parser->getErrors();
        std::cout << "错误信息:" << std::endl;
        for (const auto& error : errors) {
            std::cout << "  " << error << std::endl;
        }
    }
    
    std::cout << std::endl;
}

void testDebugInfo() {
    std::cout << "=== 测试调试信息 ===" << std::endl;
    
    BasicSyntaxParserConfig config;
    config.enable_debug = true;
    auto parser = std::make_unique<BasicSyntaxParser>(config);
    
    // 获取调试信息
    std::string debug_info = parser->getDebugInfo();
    std::cout << "调试信息:" << std::endl;
    std::cout << debug_info << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "CHTL基础语法解析器测试" << std::endl;
    std::cout << "=====================" << std::endl << std::endl;
    
    try {
        testBasicSyntaxParserCreation();
        testCommentParsing();
        testTextNodeParsing();
        testLiteralParsing();
        testHTMLElementParsing();
        testAttributeParsing();
        testCEEquivalence();
        testBasicSyntaxValidation();
        testDocumentParsing();
        testErrorHandling();
        testDebugInfo();
        
        std::cout << "所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}