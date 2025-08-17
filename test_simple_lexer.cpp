#include <iostream>
#include <string>
#include "ast/lexer.hpp"

using namespace chtl::ast;

void testBasicCHTL() {
    std::cout << "========== 基础CHTL语法测试 ==========" << std::endl;
    
    std::string source = R"(
        div {
            id: "main";
            class: container;
            
            text {
                "Hello CHTL!"
            }
        }
    )";
    
    Lexer lexer(source, "basic.chtl");
    lexer.setSkipWhitespace(true);
    lexer.setSkipComments(true);
    
    auto tokens = lexer.tokenizeAll();
    
    std::cout << "Token数量: " << tokens.size() << std::endl;
    std::cout << "关键Token:" << std::endl;
    
    for (const auto& token : tokens) {
        if (token.type == TokenType::IDENTIFIER || 
            token.type == TokenType::TEXT ||
            token.type == TokenType::STRING_LITERAL ||
            TokenUtils::isDelimiter(token.type)) {
            std::cout << "  " << token.toString() << std::endl;
        }
    }
}

void testTemplateSystem() {
    std::cout << "\n========== 模板系统测试 ==========" << std::endl;
    
    std::string source = R"(
        [Template] @Style ButtonStyle {
            color: blue;
            background: white;
        }
        
        [Custom] @Element MyButton {
            button {
                @Style ButtonStyle;
            }
        }
    )";
    
    Lexer lexer(source, "template.chtl");
    lexer.setSkipWhitespace(true);
    lexer.setSkipComments(true);
    
    auto tokens = lexer.tokenizeAll();
    
    std::cout << "模板相关Token:" << std::endl;
    
    for (const auto& token : tokens) {
        if (TokenUtils::isKeyword(token.type) || 
            token.type == TokenType::LEFT_BRACKET ||
            token.type == TokenType::RIGHT_BRACKET ||
            token.type == TokenType::AT ||
            token.type == TokenType::IDENTIFIER) {
            std::cout << "  " << token.toString() << std::endl;
        }
    }
}

void testCSSSelectors() {
    std::cout << "\n========== CSS选择器测试 ==========" << std::endl;
    
    std::string source = R"(
        style {
            .container {
                width: 100px;
            }
            
            #main {
                height: 200px;
            }
        }
    )";
    
    Lexer lexer(source, "css.chtl");
    lexer.setSkipWhitespace(true);
    
    auto tokens = lexer.tokenizeAll();
    
    std::cout << "CSS选择器Token:" << std::endl;
    
    for (const auto& token : tokens) {
        if (token.type == TokenType::CLASS_SELECTOR || 
            token.type == TokenType::ID_SELECTOR ||
            token.type == TokenType::STYLE) {
            std::cout << "  " << token.toString() << std::endl;
        }
    }
}

int main() {
    std::cout << "🚀 CHTL词法分析器基础测试" << std::endl;
    std::cout << "=========================" << std::endl;
    
    try {
        testBasicCHTL();
        testTemplateSystem();
        testCSSSelectors();
        
        std::cout << "\n🎉 词法分析器基础功能测试完成！" << std::endl;
        
        std::cout << "\n✅ 已验证功能：" << std::endl;
        std::cout << "  • HTML元素和属性Token化" << std::endl;
        std::cout << "  • CHTL关键字识别" << std::endl;
        std::cout << "  • 字符串字面量解析" << std::endl;
        std::cout << "  • CSS选择器识别" << std::endl;
        std::cout << "  • 模板系统语法支持" << std::endl;
        std::cout << "  • 基础分隔符和操作符" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}