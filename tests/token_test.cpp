#include <iostream>
#include <cassert>
#include <string>

#include "lexer/chtl_tokens.hpp"
#include "lexer/chtl_js_tokens.hpp"

using namespace chtl;

void testCHTLTokenTypes() {
    std::cout << "测试CHTL Token类型..." << std::endl;
    
    // 测试关键字映射
    CHTLKeywordMap keywordMap;
    
    // 基础关键字
    assert(keywordMap.isKeyword("text"));
    assert(keywordMap.isKeyword("style"));
    assert(keywordMap.isKeyword("script"));
    assert(keywordMap.getKeywordType("text") == CHTLTokenType::KEYWORD_TEXT);
    assert(keywordMap.getKeywordType("style") == CHTLTokenType::KEYWORD_STYLE);
    assert(keywordMap.getKeywordType("script") == CHTLTokenType::KEYWORD_SCRIPT);
    
    // 特殊结构关键字
    assert(keywordMap.isKeyword("[Template]"));
    assert(keywordMap.isKeyword("[Custom]"));
    assert(keywordMap.isKeyword("[Origin]"));
    assert(keywordMap.isKeyword("[Import]"));
    assert(keywordMap.isKeyword("[Namespace]"));
    assert(keywordMap.isKeyword("[Configuration]"));
    
    // 继承和操作关键字
    assert(keywordMap.isKeyword("inherit"));
    assert(keywordMap.isKeyword("delete"));
    assert(keywordMap.isKeyword("insert"));
    assert(keywordMap.isKeyword("after"));
    assert(keywordMap.isKeyword("before"));
    assert(keywordMap.isKeyword("replace"));
    assert(keywordMap.isKeyword("from"));
    assert(keywordMap.isKeyword("as"));
    assert(keywordMap.isKeyword("except"));
    
    // 复合关键字
    assert(keywordMap.isKeyword("at top"));
    assert(keywordMap.isKeyword("at bottom"));
    
    // 非关键字
    assert(!keywordMap.isKeyword("div"));
    assert(!keywordMap.isKeyword("hello"));
    assert(!keywordMap.isKeyword("world"));
    
    std::cout << "✓ CHTL关键字测试通过" << std::endl;
    
    // 测试前缀映射
    CHTLPrefixMap prefixMap;
    
    assert(prefixMap.isPrefix("@Style"));
    assert(prefixMap.isPrefix("@Element"));
    assert(prefixMap.isPrefix("@Var"));
    assert(prefixMap.isPrefix("@Html"));
    assert(prefixMap.isPrefix("@JavaScript"));
    assert(prefixMap.isPrefix("@Chtl"));
    assert(prefixMap.isPrefix("@CJmod"));
    
    // 支持文档中的可选前缀
    assert(prefixMap.isPrefix("@style"));
    assert(prefixMap.isPrefix("@CSS"));
    assert(prefixMap.isPrefix("@Css"));
    assert(prefixMap.isPrefix("@css"));
    
    assert(prefixMap.getPrefixType("@Style") == CHTLTokenType::PREFIX_STYLE);
    assert(prefixMap.getPrefixType("@Element") == CHTLTokenType::PREFIX_ELEMENT);
    
    // 非前缀
    assert(!prefixMap.isPrefix("@Unknown"));
    assert(!prefixMap.isPrefix("Style"));
    
    std::cout << "✓ CHTL前缀测试通过" << std::endl;
}

void testCHTLTokenUtils() {
    std::cout << "测试CHTL Token工具..." << std::endl;
    
    // 字符类型检查
    assert(CHTLTokenUtils::isIdentifierStart('a'));
    assert(CHTLTokenUtils::isIdentifierStart('_'));
    assert(!CHTLTokenUtils::isIdentifierStart('1'));
    assert(!CHTLTokenUtils::isIdentifierStart('-'));
    
    assert(CHTLTokenUtils::isIdentifierChar('a'));
    assert(CHTLTokenUtils::isIdentifierChar('1'));
    assert(CHTLTokenUtils::isIdentifierChar('_'));
    assert(CHTLTokenUtils::isIdentifierChar('-'));
    assert(!CHTLTokenUtils::isIdentifierChar(' '));
    
    assert(CHTLTokenUtils::isWhitespace(' '));
    assert(CHTLTokenUtils::isWhitespace('\t'));
    assert(!CHTLTokenUtils::isWhitespace('a'));
    
    assert(CHTLTokenUtils::isStringDelimiter('"'));
    assert(CHTLTokenUtils::isStringDelimiter('\''));
    assert(!CHTLTokenUtils::isStringDelimiter('a'));
    
    // 字符串处理
    std::string escaped = CHTLTokenUtils::escapeString("hello\nworld\t\"test\"");
    assert(escaped == "hello\\nworld\\t\\\"test\\\"");
    
    std::string unescaped = CHTLTokenUtils::unescapeString("hello\\nworld\\t");
    assert(unescaped == "hello\nworld\t");
    
    // Token类型检查
    assert(CHTLTokenUtils::isLiteralType(CHTLTokenType::IDENTIFIER));
    assert(CHTLTokenUtils::isLiteralType(CHTLTokenType::STRING_DOUBLE));
    assert(!CHTLTokenUtils::isLiteralType(CHTLTokenType::LBRACE));
    
    assert(CHTLTokenUtils::isKeywordType(CHTLTokenType::KEYWORD_TEXT));
    assert(!CHTLTokenUtils::isKeywordType(CHTLTokenType::IDENTIFIER));
    
    assert(CHTLTokenUtils::isSeparatorType(CHTLTokenType::LBRACE));
    assert(CHTLTokenUtils::isSeparatorType(CHTLTokenType::SEMICOLON));
    assert(!CHTLTokenUtils::isSeparatorType(CHTLTokenType::IDENTIFIER));
    
    std::cout << "✓ CHTL Token工具测试通过" << std::endl;
}

void testCHTLJSTokenTypes() {
    std::cout << "测试CHTL JS Token类型..." << std::endl;
    
    // 测试关键字映射
    CHTLJSKeywordMap keywordMap;
    
    // CHTL JS特有关键字
    assert(keywordMap.isKeyword("listen"));
    assert(keywordMap.isKeyword("delegate"));
    assert(keywordMap.isKeyword("animate"));
    assert(keywordMap.getKeywordType("listen") == CHTLJSTokenType::KEYWORD_LISTEN);
    assert(keywordMap.getKeywordType("delegate") == CHTLJSTokenType::KEYWORD_DELEGATE);
    assert(keywordMap.getKeywordType("animate") == CHTLJSTokenType::KEYWORD_ANIMATE);
    
    // JavaScript标准关键字
    assert(keywordMap.isKeyword("function"));
    assert(keywordMap.isKeyword("var"));
    assert(keywordMap.isKeyword("let"));
    assert(keywordMap.isKeyword("const"));
    assert(keywordMap.isKeyword("if"));
    assert(keywordMap.isKeyword("return"));
    
    // 动画相关关键字
    assert(keywordMap.isKeyword("duration"));
    assert(keywordMap.isKeyword("easing"));
    assert(keywordMap.isKeyword("begin"));
    assert(keywordMap.isKeyword("end"));
    assert(keywordMap.isKeyword("when"));
    assert(keywordMap.isKeyword("at"));
    
    // 事件委托关键字
    assert(keywordMap.isKeyword("target"));
    
    // 检查特定类型关键字
    assert(keywordMap.isCHTLJSKeyword("listen"));
    assert(keywordMap.isCHTLJSKeyword("delegate"));
    assert(keywordMap.isCHTLJSKeyword("animate"));
    assert(!keywordMap.isCHTLJSKeyword("function"));
    
    assert(keywordMap.isJavaScriptKeyword("function"));
    assert(keywordMap.isJavaScriptKeyword("var"));
    assert(!keywordMap.isJavaScriptKeyword("listen"));
    
    assert(keywordMap.isAnimateKeyword("duration"));
    assert(keywordMap.isAnimateKeyword("easing"));
    assert(!keywordMap.isAnimateKeyword("listen"));
    
    assert(keywordMap.isDelegateKeyword("target"));
    assert(!keywordMap.isDelegateKeyword("duration"));
    
    std::cout << "✓ CHTL JS关键字测试通过" << std::endl;
}

void testCHTLJSSyntaxDetector() {
    std::cout << "测试CHTL JS语法检测..." << std::endl;
    
    // 增强选择器检测
    assert(CHTLJSSyntaxDetector::hasEnhancedSelector("{{box}}->click()"));
    assert(CHTLJSSyntaxDetector::hasEnhancedSelector("const el = {{#container}};"));
    assert(!CHTLJSSyntaxDetector::hasEnhancedSelector("normal.javascript.code()"));
    
    // 箭头操作符检测
    assert(CHTLJSSyntaxDetector::hasArrowOperator("element->method()"));
    assert(CHTLJSSyntaxDetector::hasArrowOperator("{{box}}->classList.add('active')"));
    assert(!CHTLJSSyntaxDetector::hasArrowOperator("element.method()"));
    
    // 监听器检测
    assert(CHTLJSSyntaxDetector::hasListenKeyword("listen({ click: handler })"));
    assert(CHTLJSSyntaxDetector::hasListenKeyword("{{box}}->listen({click: fn})"));
    assert(!CHTLJSSyntaxDetector::hasListenKeyword("addEventListener('click', handler)"));
    
    // 委托检测
    assert(CHTLJSSyntaxDetector::hasDelegateKeyword("delegate({ target: {{box}} })"));
    assert(!CHTLJSSyntaxDetector::hasDelegateKeyword("normal delegation"));
    
    // 动画检测
    assert(CHTLJSSyntaxDetector::hasAnimateKeyword("animate({ duration: 500 })"));
    assert(!CHTLJSSyntaxDetector::hasAnimateKeyword("normal animation"));
    
    // 综合特征检测
    assert(CHTLJSSyntaxDetector::hasAnyCHTLJSFeature("{{box}}->click()"));
    assert(CHTLJSSyntaxDetector::hasAnyCHTLJSFeature("listen({click: handler})"));
    assert(CHTLJSSyntaxDetector::hasAnyCHTLJSFeature("animate({duration: 500})"));
    assert(!CHTLJSSyntaxDetector::hasAnyCHTLJSFeature("normal.javascript.code()"));
    
    std::cout << "✓ CHTL JS语法检测测试通过" << std::endl;
}

void testEnhancedSelector() {
    std::cout << "测试增强选择器..." << std::endl;
    
    Position pos{1, 1, 0};
    
    // 类选择器
    EnhancedSelector classSelector(".box", pos);
    assert(classSelector.type == EnhancedSelectorType::CLASS_NAME);
    assert(classSelector.parsed.className == "box");
    assert(classSelector.toCSSSelector() == ".box");
    assert(classSelector.toDOMQuery() == "document.querySelector('.box')");
    
    // ID选择器
    EnhancedSelector idSelector("#container", pos);
    assert(idSelector.type == EnhancedSelectorType::ID_NAME);
    assert(idSelector.parsed.idName == "container");
    assert(idSelector.toCSSSelector() == "#container");
    assert(idSelector.toDOMQuery() == "document.getElementById('container')");
    
    // 标签选择器/自动检测
    EnhancedSelector tagSelector("button", pos);
    assert(tagSelector.type == EnhancedSelectorType::AUTO_DETECT);
    assert(tagSelector.parsed.tagName == "button");
    assert(tagSelector.toCSSSelector() == "button");
    assert(tagSelector.toDOMQuery() == "document.querySelector('button')");
    
    // 后代选择器
    EnhancedSelector descendantSelector(".container button", pos);
    assert(descendantSelector.type == EnhancedSelectorType::DESCENDANT);
    assert(descendantSelector.parsed.className == "container");
    assert(descendantSelector.parsed.descendantPath.size() == 1);
    assert(descendantSelector.parsed.descendantPath[0] == "button");
    assert(descendantSelector.toCSSSelector() == ".container button");
    
    // 索引选择器
    EnhancedSelector indexedSelector("button[0]", pos);
    assert(indexedSelector.type == EnhancedSelectorType::INDEXED);
    assert(indexedSelector.parsed.hasIndex == true);
    assert(indexedSelector.parsed.index == 0);
    assert(indexedSelector.parsed.tagName == "button");
    assert(indexedSelector.toDOMQuery().find("querySelectorAll") != std::string::npos);
    assert(indexedSelector.toDOMQuery().find("[0]") != std::string::npos);
    
    std::cout << "✓ 增强选择器测试通过" << std::endl;
}

void testCHTLJSTokenUtils() {
    std::cout << "测试CHTL JS Token工具..." << std::endl;
    
    // 字符类型检查
    assert(CHTLJSTokenUtils::isIdentifierStart('a'));
    assert(CHTLJSTokenUtils::isIdentifierStart('_'));
    assert(CHTLJSTokenUtils::isIdentifierStart('$'));
    assert(!CHTLJSTokenUtils::isIdentifierStart('1'));
    
    assert(CHTLJSTokenUtils::isIdentifierChar('a'));
    assert(CHTLJSTokenUtils::isIdentifierChar('1'));
    assert(CHTLJSTokenUtils::isIdentifierChar('_'));
    assert(CHTLJSTokenUtils::isIdentifierChar('$'));
    assert(!CHTLJSTokenUtils::isIdentifierChar('-')); // JS标识符不支持连字符
    
    assert(CHTLJSTokenUtils::isDigit('0'));
    assert(CHTLJSTokenUtils::isDigit('9'));
    assert(!CHTLJSTokenUtils::isDigit('a'));
    
    // 选择器字符验证
    assert(CHTLJSTokenUtils::isValidSelectorChar('a'));
    assert(CHTLJSTokenUtils::isValidSelectorChar('.'));
    assert(CHTLJSTokenUtils::isValidSelectorChar('#'));
    assert(CHTLJSTokenUtils::isValidSelectorChar('['));
    assert(CHTLJSTokenUtils::isValidSelectorChar(' '));
    
    // 数字验证
    assert(CHTLJSTokenUtils::isValidNumber("123"));
    assert(CHTLJSTokenUtils::isValidNumber("123.456"));
    assert(CHTLJSTokenUtils::isValidNumber("-123"));
    assert(CHTLJSTokenUtils::isValidNumber("+123.456"));
    assert(!CHTLJSTokenUtils::isValidNumber("abc"));
    assert(!CHTLJSTokenUtils::isValidNumber("12.34.56"));
    
    // 数字解析
    assert(CHTLJSTokenUtils::parseNumber("123") == 123.0);
    assert(CHTLJSTokenUtils::parseNumber("123.456") == 123.456);
    assert(CHTLJSTokenUtils::parseNumber("-123") == -123.0);
    
    // 多字符序列检查
    assert(CHTLJSTokenUtils::startsMultipleCharSequence('/'));
    assert(CHTLJSTokenUtils::startsMultipleCharSequence('-'));
    assert(CHTLJSTokenUtils::startsMultipleCharSequence('{'));
    assert(!CHTLJSTokenUtils::startsMultipleCharSequence('a'));
    
    assert(CHTLJSTokenUtils::getMultipleCharSequence("//comment", 0) == "//");
    assert(CHTLJSTokenUtils::getMultipleCharSequence("/*comment*/", 0) == "/*");
    assert(CHTLJSTokenUtils::getMultipleCharSequence("->method", 0) == "->");
    assert(CHTLJSTokenUtils::getMultipleCharSequence("{{selector}}", 0) == "{{");
    
    // Token类型检查
    assert(CHTLJSTokenUtils::isLiteralType(CHTLJSTokenType::IDENTIFIER));
    assert(CHTLJSTokenUtils::isLiteralType(CHTLJSTokenType::NUMBER));
    assert(!CHTLJSTokenUtils::isLiteralType(CHTLJSTokenType::LBRACE));
    
    assert(CHTLJSTokenUtils::isKeywordType(CHTLJSTokenType::KEYWORD_LISTEN));
    assert(CHTLJSTokenUtils::isKeywordType(CHTLJSTokenType::KEYWORD_FUNCTION));
    assert(!CHTLJSTokenUtils::isKeywordType(CHTLJSTokenType::IDENTIFIER));
    
    assert(CHTLJSTokenUtils::isCHTLJSSpecificType(CHTLJSTokenType::ARROW_OP));
    assert(CHTLJSTokenUtils::isCHTLJSSpecificType(CHTLJSTokenType::KEYWORD_LISTEN));
    assert(CHTLJSTokenUtils::isCHTLJSSpecificType(CHTLJSTokenType::ENHANCED_SELECTOR_START));
    assert(!CHTLJSTokenUtils::isCHTLJSSpecificType(CHTLJSTokenType::KEYWORD_FUNCTION));
    assert(!CHTLJSTokenUtils::isCHTLJSSpecificType(CHTLJSTokenType::IDENTIFIER));
    
    std::cout << "✓ CHTL JS Token工具测试通过" << std::endl;
}

void testTokenStructures() {
    std::cout << "测试Token结构..." << std::endl;
    
    Position pos{1, 5, 4};
    
    // CHTL Token
    CHTLToken chtlToken(CHTLTokenType::KEYWORD_TEXT, "text", pos);
    assert(chtlToken.type == CHTLTokenType::KEYWORD_TEXT);
    assert(chtlToken.value == "text");
    assert(chtlToken.position.line == 1);
    assert(chtlToken.position.column == 5);
    assert(chtlToken.length == 4);
    assert(chtlToken.isKeyword());
    assert(!chtlToken.isLiteral());
    assert(!chtlToken.isOperator());
    assert(!chtlToken.isComment());
    
    // CHTL JS Token
    CHTLJSToken chtlJSToken(CHTLJSTokenType::KEYWORD_LISTEN, "listen", pos);
    assert(chtlJSToken.type == CHTLJSTokenType::KEYWORD_LISTEN);
    assert(chtlJSToken.value == "listen");
    assert(chtlJSToken.isCHTLJSSpecific());
    assert(chtlJSToken.isKeyword());
    assert(!chtlJSToken.isLiteral());
    
    // 增强选择器Token
    CHTLJSToken selectorStart(CHTLJSTokenType::ENHANCED_SELECTOR_START, "{{", pos);
    assert(selectorStart.isEnhancedSelector());
    assert(selectorStart.isCHTLJSSpecific());
    
    std::cout << "✓ Token结构测试通过" << std::endl;
}

int main() {
    std::cout << "=== CHTL/CHTL JS Token系统测试套件 ===" << std::endl;
    
    try {
        testCHTLTokenTypes();
        testCHTLTokenUtils();
        testCHTLJSTokenTypes();
        testCHTLJSSyntaxDetector();
        testEnhancedSelector();
        testCHTLJSTokenUtils();
        testTokenStructures();
        
        std::cout << "\n🎉 所有Token系统测试通过！" << std::endl;
        std::cout << "Token定义严格符合CHTL语法文档规范。" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ 未知错误发生" << std::endl;
        return 1;
    }
}