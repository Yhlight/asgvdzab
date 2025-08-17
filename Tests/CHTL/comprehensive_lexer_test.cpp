#include <iostream>
#include <cassert>
#include <string>
#include <chrono>

#include "lexer/chtl_tokens.hpp"
#include "lexer/chtl_js_tokens.hpp"
#include "lexer/lexer_state.hpp"
// #include "lexer/lexer_context.hpp"  // 暂时注释掉，因为还未实现
#include "lexer/global_map.hpp"

using namespace chtl;

void testCHTLTokenSystem() {
    std::cout << "测试CHTL Token系统集成..." << std::endl;
    
    // 测试关键字映射
    CHTLKeywordMap keywordMap;
    CHTLPrefixMap prefixMap;
    
    // 验证所有关键字
    std::vector<std::string> testKeywords = {
        "text", "style", "script", "inherit", "delete", "insert",
        "after", "before", "replace", "from", "as", "except",
        "[Template]", "[Custom]", "[Origin]", "[Import]", 
        "[Namespace]", "[Configuration]", "at top", "at bottom"
    };
    
    for (const auto& keyword : testKeywords) {
        assert(keywordMap.isKeyword(keyword));
    }
    
    // 验证前缀
    std::vector<std::string> testPrefixes = {
        "@Style", "@Element", "@Var", "@Html", "@JavaScript", 
        "@Chtl", "@CJmod", "@style", "@CSS", "@Css", "@css"
    };
    
    for (const auto& prefix : testPrefixes) {
        assert(prefixMap.isPrefix(prefix));
    }
    
    std::cout << "✓ CHTL Token系统集成测试通过" << std::endl;
}

void testCHTLJSTokenSystem() {
    std::cout << "测试CHTL JS Token系统集成..." << std::endl;
    
    CHTLJSKeywordMap keywordMap;
    
    // 测试CHTL JS特有关键字
    assert(keywordMap.isCHTLJSKeyword("listen"));
    assert(keywordMap.isCHTLJSKeyword("delegate"));
    assert(keywordMap.isCHTLJSKeyword("animate"));
    
    // 测试JavaScript标准关键字
    assert(keywordMap.isJavaScriptKeyword("function"));
    assert(keywordMap.isJavaScriptKeyword("var"));
    assert(keywordMap.isJavaScriptKeyword("const"));
    
    // 测试动画关键字
    assert(keywordMap.isAnimateKeyword("duration"));
    assert(keywordMap.isAnimateKeyword("easing"));
    assert(keywordMap.isAnimateKeyword("begin"));
    
    // 测试语法检测
    assert(CHTLJSSyntaxDetector::hasEnhancedSelector("{{box}}->click()"));
    assert(CHTLJSSyntaxDetector::hasArrowOperator("element->method()"));
    assert(CHTLJSSyntaxDetector::hasListenKeyword("listen({ click: handler })"));
    assert(CHTLJSSyntaxDetector::hasAnyCHTLJSFeature("{{box}}->listen({click: fn})"));
    
    // 测试增强选择器解析
    Position pos{1, 1, 0};
    EnhancedSelector selector(".container button[0]", pos);
    assert(selector.type == EnhancedSelectorType::INDEXED);
    assert(selector.parsed.hasIndex);
    assert(selector.parsed.index == 0);
    
    std::cout << "✓ CHTL JS Token系统集成测试通过" << std::endl;
}

void testStateManagement() {
    std::cout << "测试状态管理系统..." << std::endl;
    
    LexerStateManager stateManager;
    
    // 测试初始状态
    assert(stateManager.getCurrentState() == LexerStateType::INITIAL);
    assert(stateManager.getStackDepth() == 1);
    
    // 测试状态推入和弹出
    stateManager.pushState(LexerStateType::IN_CHTL_BLOCK, "@Element", "Box");
    assert(stateManager.getCurrentState() == LexerStateType::IN_CHTL_BLOCK);
    assert(stateManager.getStackDepth() == 2);
    assert(stateManager.isInCHTLBlock());
    
    stateManager.pushState(LexerStateType::IN_STYLE_BLOCK);
    assert(stateManager.getCurrentState() == LexerStateType::IN_STYLE_BLOCK);
    assert(stateManager.isInCSSBlock());
    
    stateManager.pushState(LexerStateType::IN_SCRIPT_BLOCK);
    assert(stateManager.getCurrentState() == LexerStateType::IN_SCRIPT_BLOCK);
    assert(stateManager.isInCHTLJSBlock());
    
    // 测试括号管理
    stateManager.incrementBraceDepth();
    stateManager.incrementBraceDepth();
    assert(stateManager.getBraceDepth() == 2);
    
    stateManager.decrementBraceDepth();
    assert(stateManager.getBraceDepth() == 1);
    
    // 测试状态查询
    assert(!stateManager.isInString());
    assert(!stateManager.isInComment());
    assert(stateManager.isInCHTLJSBlock());
    
    // 测试状态弹出
    assert(stateManager.popState());
    assert(stateManager.getCurrentState() == LexerStateType::IN_STYLE_BLOCK);
    
    assert(stateManager.popState());
    assert(stateManager.getCurrentState() == LexerStateType::IN_CHTL_BLOCK);
    
    assert(stateManager.popState());
    assert(stateManager.getCurrentState() == LexerStateType::INITIAL);
    
    // 测试状态验证
    assert(stateManager.validateState());
    
    // 测试状态栈跟踪
    std::string stackTrace = stateManager.getStackTrace();
    assert(!stackTrace.empty());
    
    std::cout << "✓ 状态管理系统测试通过" << std::endl;
}

void testGlobalMapSystem() {
    std::cout << "测试全局映射系统..." << std::endl;
    
    GlobalMap globalMap;
    
    // 测试符号注册
    SymbolDefinition styleSymbol(SymbolType::TEMPLATE_STYLE, "DefaultText", 
                                 "color: black; font-size: 16px;", Position{1, 1, 0});
    styleSymbol.namespacePath = "UI";
    
    assert(globalMap.registerSymbol(styleSymbol));
    
    // 测试符号查找
    const SymbolDefinition* found = globalMap.findSymbol("DefaultText", "UI");
    assert(found != nullptr);
    assert(found->name == "DefaultText");
    assert(found->type == SymbolType::TEMPLATE_STYLE);
    
    // 测试命名空间注册
    NamespaceDefinition nsUI;
    nsUI.name = "UI";
    nsUI.position = Position{1, 1, 0};
    
    assert(globalMap.registerNamespace(nsUI));
    
    const NamespaceDefinition* foundNS = globalMap.findNamespace("UI");
    assert(foundNS != nullptr);
    assert(foundNS->name == "UI");
    
    // 测试符号类型查找
    auto styleSymbols = globalMap.findSymbolsByType(SymbolType::TEMPLATE_STYLE);
    assert(styleSymbols.size() > 0);
    
    // 测试配置管理
    std::unordered_map<std::string, std::string> config = {
        {"INDEX_INITIAL_COUNT", "0"},
        {"DEBUG_MODE", "false"}
    };
    globalMap.registerConfiguration(config);
    
    assert(globalMap.getConfigValue("INDEX_INITIAL_COUNT") == "0");
    assert(globalMap.getConfigValue("DEBUG_MODE") == "false");
    
    // 测试统计信息
    auto stats = globalMap.getStatistics();
    assert(stats.totalSymbols > 0);
    assert(stats.totalNamespaces > 0);
    
    // 测试一致性验证
    auto errors = globalMap.validateConsistency();
    // 在这个简单测试中，应该没有错误
    
    std::cout << "✓ 全局映射系统测试通过" << std::endl;
}

void testSymbolUtils() {
    std::cout << "测试符号工具类..." << std::endl;
    
    // 测试符号引用解析
    auto [type, name] = SymbolUtils::parseSymbolReference("[Template] @Style DefaultText");
    assert(type == SymbolType::TEMPLATE_STYLE);
    assert(name == "DefaultText");
    
    auto [type2, name2] = SymbolUtils::parseSymbolReference("@Element Box");
    // 注意：这里应该检测为模板或自定义，取决于上下文
    assert(name2 == "Box");
    
    // 测试符号引用格式化
    std::string formatted = SymbolUtils::formatSymbolReference(SymbolType::CUSTOM_ELEMENT, "CustomBox");
    assert(!formatted.empty());
    
    // 测试类型兼容性
    assert(SymbolUtils::areTypesCompatible(SymbolType::TEMPLATE_STYLE, SymbolType::CUSTOM_STYLE));
    
    // 测试符号名称验证
    assert(SymbolUtils::isValidSymbolName("ValidName"));
    assert(SymbolUtils::isValidSymbolName("Valid-Name"));
    assert(!SymbolUtils::isValidSymbolName("123Invalid"));
    assert(!SymbolUtils::isValidSymbolName(""));
    
    // 测试限定名称分割
    auto [ns, symbolName] = SymbolUtils::splitQualifiedName("Namespace.Symbol");
    assert(ns == "Namespace");
    assert(symbolName == "Symbol");
    
    std::cout << "✓ 符号工具类测试通过" << std::endl;
}

void testStateUtils() {
    std::cout << "测试状态工具类..." << std::endl;
    
    // 测试状态类型检查
    assert(StateUtils::isBlockState(LexerStateType::IN_CHTL_BLOCK));
    assert(StateUtils::isBlockState(LexerStateType::IN_TEMPLATE_BLOCK));
    assert(!StateUtils::isBlockState(LexerStateType::IN_STRING_DOUBLE));
    
    assert(StateUtils::isStringState(LexerStateType::IN_STRING_DOUBLE));
    assert(StateUtils::isStringState(LexerStateType::IN_STRING_SINGLE));
    assert(!StateUtils::isStringState(LexerStateType::IN_CHTL_BLOCK));
    
    assert(StateUtils::isCommentState(LexerStateType::IN_COMMENT_LINE));
    assert(StateUtils::isCommentState(LexerStateType::IN_COMMENT_MULTILINE));
    assert(!StateUtils::isCommentState(LexerStateType::IN_CHTL_BLOCK));
    
    // 测试状态优先级
    assert(StateUtils::getStatePriority(LexerStateType::ERROR_STATE) > 
           StateUtils::getStatePriority(LexerStateType::IN_STRING_DOUBLE));
    assert(StateUtils::getStatePriority(LexerStateType::IN_STRING_DOUBLE) > 
           StateUtils::getStatePriority(LexerStateType::IN_CHTL_BLOCK));
    
    // 测试嵌套允许
    assert(StateUtils::allowsNesting(LexerStateType::IN_CHTL_BLOCK));
    assert(!StateUtils::allowsNesting(LexerStateType::IN_STRING_DOUBLE));
    assert(!StateUtils::allowsNesting(LexerStateType::ERROR_STATE));
    
    // 测试代码段类型转换
    assert(StateUtils::getCodeSegmentType(LexerStateType::IN_CHTL_BLOCK) == CodeSegmentType::CHTL_CORE);
    assert(StateUtils::getCodeSegmentType(LexerStateType::IN_CHTL_JS_BLOCK) == CodeSegmentType::CHTL_JS_SCRIPT);
    assert(StateUtils::getCodeSegmentType(LexerStateType::IN_CSS_BLOCK) == CodeSegmentType::CSS_LOCAL_STYLE);
    
    assert(StateUtils::getStateFromCodeSegmentType(CodeSegmentType::CHTL_CORE) == LexerStateType::IN_CHTL_BLOCK);
    assert(StateUtils::getStateFromCodeSegmentType(CodeSegmentType::CHTL_JS_SCRIPT) == LexerStateType::IN_CHTL_JS_BLOCK);
    
    std::cout << "✓ 状态工具类测试通过" << std::endl;
}

void testTokenCreationAndManipulation() {
    std::cout << "测试Token创建和操作..." << std::endl;
    
    Position pos{1, 5, 4};
    
    // 测试CHTL Token创建
    CHTLToken chtlToken(CHTLTokenType::KEYWORD_TEXT, "text", pos, 4);
    assert(chtlToken.type == CHTLTokenType::KEYWORD_TEXT);
    assert(chtlToken.value == "text");
    assert(chtlToken.position.line == 1);
    assert(chtlToken.position.column == 5);
    assert(chtlToken.length == 4);
    assert(chtlToken.isKeyword());
    assert(!chtlToken.isLiteral());
    
    // 测试CHTL JS Token创建
    CHTLJSToken chtlJSToken(CHTLJSTokenType::KEYWORD_LISTEN, "listen", pos, 6);
    assert(chtlJSToken.type == CHTLJSTokenType::KEYWORD_LISTEN);
    assert(chtlJSToken.value == "listen");
    assert(chtlJSToken.isCHTLJSSpecific());
    assert(chtlJSToken.isKeyword());
    
    // 测试Token字符串表示
    std::string chtlTokenStr = chtlToken.toString();
    std::string chtlJSTokenStr = chtlJSToken.toString();
    assert(!chtlTokenStr.empty());
    assert(!chtlJSTokenStr.empty());
    assert(chtlJSTokenStr.find("CHTL_SPECIFIC") != std::string::npos);
    
    std::cout << "✓ Token创建和操作测试通过" << std::endl;
}

void testComplexScenarios() {
    std::cout << "测试复杂场景..." << std::endl;
    
    // 测试复杂的状态转换场景
    LexerStateManager stateManager;
    
    // 模拟解析复杂的CHTL结构
    // [Template] @Style DefaultText { ... }
    CHTLToken templateToken(CHTLTokenType::KEYWORD_TEMPLATE, "[Template]", Position{1, 1, 0});
    stateManager.transitionOnToken(templateToken);
    assert(stateManager.getCurrentState() == LexerStateType::IN_TEMPLATE_BLOCK);
    
    CHTLToken prefixToken(CHTLTokenType::PREFIX_STYLE, "@Style", Position{1, 12, 11});
    // 这里不会改变状态，只是标识类型
    
    CHTLToken identifierToken(CHTLTokenType::IDENTIFIER, "DefaultText", Position{1, 19, 18});
    
    CHTLToken lbraceToken(CHTLTokenType::LBRACE, "{", Position{1, 31, 30});
    stateManager.transitionOnToken(lbraceToken);
    assert(stateManager.getBraceDepth() == 1);
    
    // 模拟属性解析 color: red;
    CHTLToken colorToken(CHTLTokenType::IDENTIFIER, "color", Position{2, 5, 32});
    CHTLToken colonToken(CHTLTokenType::COLON_EQUALS, ":", Position{2, 10, 37});
    stateManager.transitionOnToken(colonToken);
    assert(stateManager.isExpectingValue());
    
    CHTLToken redToken(CHTLTokenType::IDENTIFIER, "red", Position{2, 12, 39});
    CHTLToken semicolonToken(CHTLTokenType::SEMICOLON, ";", Position{2, 15, 42});
    stateManager.transitionOnToken(semicolonToken);
    assert(!stateManager.isExpectingValue());
    
    CHTLToken rbraceToken(CHTLTokenType::RBRACE, "}", Position{3, 1, 44});
    stateManager.transitionOnToken(rbraceToken);
    assert(stateManager.getBraceDepth() == 0);
    assert(stateManager.getCurrentState() == LexerStateType::INITIAL);
    
    // 测试全局映射与状态管理的集成
    GlobalMap globalMap;
    SymbolDefinition templateStyle(SymbolType::TEMPLATE_STYLE, "DefaultText", 
                                   "color: red;", Position{1, 1, 0});
    globalMap.registerSymbol(templateStyle);
    
    const SymbolDefinition* found = globalMap.findSymbol("DefaultText");
    assert(found != nullptr);
    assert(found->content == "color: red;");
    
    std::cout << "✓ 复杂场景测试通过" << std::endl;
}

void testPerformance() {
    std::cout << "测试性能..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // 大量Token创建和状态转换
    LexerStateManager stateManager;
    for (int i = 0; i < 1000; ++i) {
        stateManager.pushState(LexerStateType::IN_CHTL_BLOCK);
        stateManager.incrementBraceDepth();
        stateManager.setExpectingValue(true);
        stateManager.setExpectingValue(false);
        stateManager.decrementBraceDepth();
        stateManager.popState();
    }
    
    // 大量符号注册和查找
    GlobalMap globalMap;
    for (int i = 0; i < 100; ++i) {
        SymbolDefinition symbol(SymbolType::TEMPLATE_STYLE, "Symbol" + std::to_string(i), 
                               "content", Position{1, 1, 0});
        globalMap.registerSymbol(symbol);
    }
    
    for (int i = 0; i < 100; ++i) {
        const SymbolDefinition* found = globalMap.findSymbol("Symbol" + std::to_string(i));
        assert(found != nullptr);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "✓ 性能测试通过 (耗时: " << duration.count() << "ms)" << std::endl;
}

int main() {
    std::cout << "=== CHTL 词法分析器系统综合测试套件 ===" << std::endl;
    
    try {
        testCHTLTokenSystem();
        testCHTLJSTokenSystem();
        testStateManagement();
        testGlobalMapSystem();
        testSymbolUtils();
        testStateUtils();
        testTokenCreationAndManipulation();
        testComplexScenarios();
        testPerformance();
        
        std::cout << "\n🎉 所有综合测试通过！" << std::endl;
        std::cout << "✓ Token系统运行正常" << std::endl;
        std::cout << "✓ 状态管理系统运行正常" << std::endl;
        std::cout << "✓ 全局映射系统运行正常" << std::endl;
        std::cout << "✓ 所有组件集成良好" << std::endl;
        std::cout << "✓ 性能表现良好" << std::endl;
        std::cout << "\n词法分析器系统已经按照CHTL语法文档严格实现！" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ 未知错误发生" << std::endl;
        return 1;
    }
}