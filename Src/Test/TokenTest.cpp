#include "Scanner/Token.h"
#include <iostream>
#include <cassert>

using namespace Chtl;

void TestTokenCreation() {
    std::cout << "测试Token创建..." << std::endl;
    
    // 测试基本Token创建
    SourceLocation loc(1, 1, 0, "test.chtl");
    Token token1(TokenType::TEXT, "text", loc);
    
    assert(token1.GetType() == TokenType::TEXT);
    assert(token1.GetText() == "text");
    assert(token1.IsChtlKeyword());
    assert(!token1.IsHtmlElement());
    
    // 测试HTML元素Token
    Token token2(TokenType::HTML_ELEMENT, "div", loc);
    assert(token2.IsHtmlElement());
    
    std::cout << "✓ Token创建测试通过" << std::endl;
}

void TestChtlKeywords() {
    std::cout << "测试CHTL关键字识别..." << std::endl;
    
    // 测试CHTL核心关键字
    assert(ChtlKeywords::IsChtlCoreKeyword("text"));
    assert(ChtlKeywords::IsChtlCoreKeyword("style"));
    assert(ChtlKeywords::IsChtlCoreKeyword("script"));
    assert(!ChtlKeywords::IsChtlCoreKeyword("div"));
    
    // 测试HTML元素
    assert(ChtlKeywords::IsHtmlElement("div"));
    assert(ChtlKeywords::IsHtmlElement("span"));
    assert(ChtlKeywords::IsHtmlElement("html"));
    assert(!ChtlKeywords::IsHtmlElement("text"));
    
    // 测试CHTL JS关键字
    assert(ChtlKeywords::IsChtlJsKeyword("vir"));
    assert(ChtlKeywords::IsChtlJsKeyword("listen"));
    assert(ChtlKeywords::IsChtlJsKeyword("delegate"));
    assert(ChtlKeywords::IsChtlJsKeyword("animate"));
    
    // 测试模板关键字
    assert(ChtlKeywords::IsTemplateKeyword("[Template]"));
    assert(ChtlKeywords::IsTemplateKeyword("[Custom]"));
    assert(ChtlKeywords::IsTemplateKeyword("@Style"));
    assert(ChtlKeywords::IsTemplateKeyword("@Element"));
    
    std::cout << "✓ CHTL关键字识别测试通过" << std::endl;
}

void TestTokenUtils() {
    std::cout << "测试Token工具函数..." << std::endl;
    
    // 测试标识符验证
    assert(TokenUtils::IsValidIdentifier("validName"));
    assert(TokenUtils::IsValidIdentifier("valid_name"));
    assert(TokenUtils::IsValidIdentifier("valid-name"));
    assert(!TokenUtils::IsValidIdentifier("123invalid"));
    assert(!TokenUtils::IsValidIdentifier(""));
    
    // 测试增强选择器
    assert(TokenUtils::IsEnhancedSelector("{{.class}}"));
    assert(TokenUtils::IsEnhancedSelector("{{#id}}"));
    assert(TokenUtils::IsEnhancedSelector("{{button}}"));
    assert(!TokenUtils::IsEnhancedSelector("{invalid}"));
    assert(!TokenUtils::IsEnhancedSelector("{{incomplete"));
    
    // 测试选择器内容提取
    assert(TokenUtils::ExtractSelectorContent("{{.box}}") == ".box");
    assert(TokenUtils::ExtractSelectorContent("{{#main}}") == "#main");
    assert(TokenUtils::ExtractSelectorContent("{{button[0]}}") == "button[0]");
    
    // 测试CE对等式
    assert(TokenUtils::IsColonEqualOperator(TokenType::COLON));
    assert(TokenUtils::IsColonEqualOperator(TokenType::ASSIGN));
    assert(!TokenUtils::IsColonEqualOperator(TokenType::DOT));
    
    std::cout << "✓ Token工具函数测试通过" << std::endl;
}

void TestTokenStream() {
    std::cout << "测试TokenStream..." << std::endl;
    
    SourceLocation loc(1, 1, 0, "test.chtl");
    std::vector<Token> tokens = {
        Token(TokenType::HTML_ELEMENT, "div", loc),
        Token(TokenType::LBRACE, "{", loc),
        Token(TokenType::TEXT, "text", loc),
        Token(TokenType::LBRACE, "{", loc),
        Token(TokenType::UNQUOTED_STRING, "Hello", loc),
        Token(TokenType::RBRACE, "}", loc),
        Token(TokenType::RBRACE, "}", loc)
    };
    
    TokenStream stream(std::move(tokens));
    
    // 测试基本导航
    assert(stream.Current().GetType() == TokenType::HTML_ELEMENT);
    assert(stream.Current().GetText() == "div");
    
    stream.Advance();
    assert(stream.Current().GetType() == TokenType::LBRACE);
    
    // 测试匹配
    assert(stream.Match(TokenType::LBRACE));
    assert(stream.Current().GetType() == TokenType::TEXT);
    
    // 测试Peek
    assert(stream.Peek().GetType() == TokenType::LBRACE);
    assert(stream.Current().GetType() == TokenType::TEXT); // Peek不应该改变当前位置
    
    std::cout << "✓ TokenStream测试通过" << std::endl;
}

void TestTokenTypeConversion() {
    std::cout << "测试Token类型转换..." << std::endl;
    
    // 测试关键字类型获取
    assert(ChtlKeywords::GetKeywordType("text") == TokenType::TEXT);
    assert(ChtlKeywords::GetKeywordType("style") == TokenType::STYLE);
    assert(ChtlKeywords::GetKeywordType("div") == TokenType::HTML_ELEMENT);
    assert(ChtlKeywords::GetKeywordType("vir") == TokenType::VIR);
    assert(ChtlKeywords::GetKeywordType("listen") == TokenType::LISTEN);
    assert(ChtlKeywords::GetKeywordType("[Template]") == TokenType::TEMPLATE_BRACKET);
    assert(ChtlKeywords::GetKeywordType("@Style") == TokenType::AT_STYLE);
    assert(ChtlKeywords::GetKeywordType("unknownKeyword") == TokenType::IDENTIFIER);
    
    // 测试类型到字符串转换
    assert(TokenUtils::TokenTypeToString(TokenType::TEXT) == "TEXT");
    assert(TokenUtils::TokenTypeToString(TokenType::HTML_ELEMENT) == "HTML_ELEMENT");
    assert(TokenUtils::TokenTypeToString(TokenType::ENHANCED_SELECTOR) == "ENHANCED_SELECTOR");
    
    std::cout << "✓ Token类型转换测试通过" << std::endl;
}

int main() {
    std::cout << "=== CHTL Token组件测试 ===" << std::endl;
    
    try {
        TestTokenCreation();
        TestChtlKeywords();
        TestTokenUtils();
        TestTokenStream();
        TestTokenTypeConversion();
        
        std::cout << std::endl << "🎉 所有测试通过！Token组件工作正常。" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    }
}