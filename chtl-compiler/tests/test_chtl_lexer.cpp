#include <iostream>
#include <sstream>
#include "compiler/chtl/chtl_lexer.h"

using namespace chtl;
using namespace chtl::compiler;

// 将TokenType转换为字符串
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::NUMBER_LITERAL: return "NUMBER_LITERAL";
        case TokenType::UNQUOTED_LITERAL: return "UNQUOTED_LITERAL";
        
        case TokenType::KEYWORD_TEXT: return "KEYWORD_TEXT";
        case TokenType::KEYWORD_STYLE: return "KEYWORD_STYLE";
        case TokenType::KEYWORD_SCRIPT: return "KEYWORD_SCRIPT";
        case TokenType::KEYWORD_TEMPLATE: return "KEYWORD_TEMPLATE";
        case TokenType::KEYWORD_CUSTOM: return "KEYWORD_CUSTOM";
        case TokenType::KEYWORD_IMPORT: return "KEYWORD_IMPORT";
        case TokenType::KEYWORD_NAMESPACE: return "KEYWORD_NAMESPACE";
        case TokenType::KEYWORD_ORIGIN: return "KEYWORD_ORIGIN";
        case TokenType::KEYWORD_CONFIGURATION: return "KEYWORD_CONFIGURATION";
        
        case TokenType::KEYWORD_INHERIT: return "KEYWORD_INHERIT";
        case TokenType::KEYWORD_DELETE: return "KEYWORD_DELETE";
        case TokenType::KEYWORD_INSERT: return "KEYWORD_INSERT";
        case TokenType::KEYWORD_EXCEPT: return "KEYWORD_EXCEPT";
        case TokenType::KEYWORD_FROM: return "KEYWORD_FROM";
        case TokenType::KEYWORD_AS: return "KEYWORD_AS";
        
        case TokenType::KEYWORD_AFTER: return "KEYWORD_AFTER";
        case TokenType::KEYWORD_BEFORE: return "KEYWORD_BEFORE";
        case TokenType::KEYWORD_REPLACE: return "KEYWORD_REPLACE";
        case TokenType::KEYWORD_AT: return "KEYWORD_AT";
        case TokenType::KEYWORD_TOP: return "KEYWORD_TOP";
        case TokenType::KEYWORD_BOTTOM: return "KEYWORD_BOTTOM";
        
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COLON: return "COLON";
        case TokenType::EQUALS: return "EQUALS";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";
        case TokenType::AT: return "AT";
        case TokenType::AMPERSAND: return "AMPERSAND";
        case TokenType::ARROW: return "ARROW";
        case TokenType::DOUBLE_LEFT_BRACE: return "DOUBLE_LEFT_BRACE";
        case TokenType::DOUBLE_RIGHT_BRACE: return "DOUBLE_RIGHT_BRACE";
        case TokenType::HASH: return "HASH";
        case TokenType::DOUBLE_DASH: return "DOUBLE_DASH";
        
        case TokenType::SINGLE_LINE_COMMENT: return "SINGLE_LINE_COMMENT";
        case TokenType::MULTI_LINE_COMMENT: return "MULTI_LINE_COMMENT";
        case TokenType::SEMANTIC_COMMENT: return "SEMANTIC_COMMENT";
        
        case TokenType::WHITESPACE: return "WHITESPACE";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::EOF_TOKEN: return "EOF_TOKEN";
        case TokenType::UNKNOWN: return "UNKNOWN";
        
        default: return "UNKNOWN_TYPE";
    }
}

void testLexer(const std::string& testName, const std::string& source) {
    std::cout << "\n=== Test: " << testName << " ===" << std::endl;
    std::cout << "Source: \"" << source << "\"" << std::endl;
    std::cout << "Tokens:" << std::endl;
    
    CHTLLexer lexer(source, testName);
    
    while (true) {
        Token token = lexer.nextToken();
        if (token.type == TokenType::EOF_TOKEN) {
            std::cout << "  EOF" << std::endl;
            break;
        }
        
        std::cout << "  " << tokenTypeToString(token.type) 
                  << " [" << token.value << "]"
                  << " at " << token.location.line << ":" << token.location.column
                  << std::endl;
    }
}

int main() {
    std::cout << "CHTL Lexer Test" << std::endl;
    
    // 测试基本元素
    testLexer("Basic Element", R"(
div {
    text { "Hello World" }
}
)");
    
    // 测试无修饰字面量
    testLexer("Unquoted Literal", R"(
div {
    class: container;
    id = main-content;
}
)");
    
    // 测试模板语法
    testLexer("Template Syntax", R"(
[Template] @Style DefaultText {
    color: black;
    font-size: 16px;
}
)");
    
    // 测试CHTL JS语法
    testLexer("CHTL JS Syntax", R"(
script {
    {{box}}->addEventListener('click', handler);
    {{#id}}->style.color = "red";
    {{.class button}}->forEach(btn => {
        btn->classList.add('active');
    });
}
)");
    
    // 测试注释
    testLexer("Comments", R"(
// 单行注释
/* 多行
   注释 */
-- 语义注释
div { }
)");
    
    // 测试自定义语法
    testLexer("Custom Syntax", R"(
[Custom] Button from button {
    delete at(0);
    insert after(.text) {
        span { class: icon; }
    }
}
)");
    
    // 测试配置组
    testLexer("Configuration", R"(
[Configuration] @Main {
    indexInitialCount: 100;
    disableNameGroup: true;
}
)");
    
    // 测试导入和命名空间
    testLexer("Import and Namespace", R"(
[Import] "./components.chtl" as Components except { Button, Modal }
[Namespace] MyApp {
    div { }
}
)");
    
    return 0;
}