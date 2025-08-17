#include "Lexer/Lexer.h"
#include <iostream>
#include <cassert>

using namespace Chtl;

void testBasicTokens() {
    std::cout << "=== Testing Basic Tokens ===\n";
    
    std::string code = R"(
div {
    id: main;
    class: container;
    
    text {
        "Hello CHTL!"
    }
}
)";
    
    Lexer lexer;
    lexer.setSource(code, "test.chtl");
    
    auto tokens = lexer.tokenize();
    
    std::cout << "Total tokens: " << tokens.size() << "\n";
    
    for (const auto& token : tokens) {
        std::cout << token.toString() << "\n";
    }
    
    // 验证关键token
    assert(tokens[0].type == TokenType::IDENTIFIER);
    assert(tokens[0].value == "div");
    assert(tokens[1].type == TokenType::LBRACE);
    
    std::cout << "Basic token test passed!\n\n";
}

void testChtlKeywords() {
    std::cout << "=== Testing CHTL Keywords ===\n";
    
    std::string code = R"(
[Template] @Style DefaultText {
    color: black;
}

[Custom] @Element Box {
    div {
        style {
            .box {
                width: 100px;
            }
        }
    }
}
)";
    
    Lexer lexer;
    lexer.setSource(code);
    
    auto tokens = lexer.tokenize();
    
    bool foundTemplate = false;
    bool foundAtStyle = false;
    bool foundCustom = false;
    bool foundAtElement = false;
    
    for (const auto& token : tokens) {
        if (token.type == TokenType::TEMPLATE_MARK) foundTemplate = true;
        if (token.type == TokenType::AT_STYLE) foundAtStyle = true;
        if (token.type == TokenType::CUSTOM_MARK) foundCustom = true;
        if (token.type == TokenType::AT_ELEMENT) foundAtElement = true;
    }
    
    assert(foundTemplate);
    assert(foundAtStyle);
    assert(foundCustom);
    assert(foundAtElement);
    
    std::cout << "CHTL keywords test passed!\n\n";
}

void testChtlJsTokens() {
    std::cout << "=== Testing CHTL JS Tokens ===\n";
    
    std::string code = R"(
script {
    {{button}}->addEventListener('click', () => {
        console.log('Clicked!');
    });
}
)";
    
    Lexer lexer;
    lexer.setSource(code);
    
    auto tokens = lexer.tokenize();
    
    bool foundDoubleLBrace = false;
    bool foundDoubleRBrace = false;
    bool foundArrow = false;
    
    for (const auto& token : tokens) {
        if (token.type == TokenType::DOUBLE_LBRACE) foundDoubleLBrace = true;
        if (token.type == TokenType::DOUBLE_RBRACE) foundDoubleRBrace = true;
        if (token.type == TokenType::ARROW) foundArrow = true;
        
        std::cout << token.toString() << "\n";
    }
    
    assert(foundDoubleLBrace);
    assert(foundDoubleRBrace);
    assert(foundArrow);
    
    std::cout << "CHTL JS tokens test passed!\n\n";
}

void testStringLiterals() {
    std::cout << "=== Testing String Literals ===\n";
    
    std::string code = R"(
text {
    "双引号字符串"
    '单引号字符串'
    无引号字面量
}
)";
    
    Lexer lexer;
    lexer.setSource(code);
    
    auto tokens = lexer.tokenize();
    
    bool foundDoubleQuote = false;
    bool foundSingleQuote = false;
    bool foundUnquoted = false;
    
    for (const auto& token : tokens) {
        if (token.type == TokenType::STRING_DOUBLE_QUOTE) {
            foundDoubleQuote = true;
            std::cout << "Double quote string: " << token.value << "\n";
        }
        if (token.type == TokenType::STRING_SINGLE_QUOTE) {
            foundSingleQuote = true;
            std::cout << "Single quote string: " << token.value << "\n";
        }
        if (token.type == TokenType::UNQUOTED_LITERAL) {
            foundUnquoted = true;
            std::cout << "Unquoted literal: " << token.value << "\n";
        }
    }
    
    assert(foundDoubleQuote);
    assert(foundSingleQuote);
    assert(foundUnquoted);
    
    std::cout << "String literals test passed!\n\n";
}

void testComplexOperations() {
    std::cout << "=== Testing Complex Operations ===\n";
    
    std::string code = R"(
inherit @Style ThemeColor;
delete line-height, border;
insert after div[0] {
    span { }
}
except @Html;
)";
    
    Lexer lexer;
    lexer.setSource(code);
    
    auto tokens = lexer.tokenize();
    
    bool foundInherit = false;
    bool foundDelete = false;
    bool foundInsert = false;
    bool foundAfter = false;
    bool foundExcept = false;
    
    for (const auto& token : tokens) {
        if (token.type == TokenType::INHERIT) foundInherit = true;
        if (token.type == TokenType::DELETE) foundDelete = true;
        if (token.type == TokenType::INSERT) foundInsert = true;
        if (token.type == TokenType::AFTER) foundAfter = true;
        if (token.type == TokenType::EXCEPT) foundExcept = true;
    }
    
    assert(foundInherit);
    assert(foundDelete);
    assert(foundInsert);
    assert(foundAfter);
    assert(foundExcept);
    
    std::cout << "Complex operations test passed!\n\n";
}

int main() {
    std::cout << "Running Lexer Tests...\n\n";
    
    try {
        testBasicTokens();
        testChtlKeywords();
        testChtlJsTokens();
        testStringLiterals();
        testComplexOperations();
        
        std::cout << "All tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}