#include <iostream>
#include <string>
#include "lexer/CHTLToken.h"
#include "lexer/GlobalMap.h"

using namespace chtl;

void testTokens() {
    std::cout << "=== 测试Token系统 ===" << std::endl;
    
    // 测试CHTL Token
    Token token(TokenType::HTML_ELEMENT, "div", 1, 1, 0);
    std::cout << "CHTL Token: " << token.toString() << std::endl;
    
    // 测试CHTL JS Token
    CHTLJSToken jsToken(CHTLJSTokenType::ENHANCED_SELECTOR_START, "{{", 1, 1, 0);
    std::cout << "CHTL JS Token: " << jsToken.toString() << std::endl;
    
    std::cout << std::endl;
}

void testGlobalMap() {
    std::cout << "=== 测试GlobalMap系统 ===" << std::endl;
    
    GlobalMap globalMap;
    
    // 添加符号
    SymbolInfo symbol(SymbolType::TEMPLATE_STYLE, "DefaultText", "", "test.chtl", 1, 1);
    symbol.value = "color: black;";
    
    if (globalMap.addSymbol(symbol)) {
        std::cout << "成功添加符号: " << symbol.name << std::endl;
    }
    
    // 查找符号
    SymbolInfo* found = globalMap.findSymbol("DefaultText");
    if (found) {
        std::cout << "找到符号: " << found->name << ", 值: " << found->value << std::endl;
    }
    
    // 添加命名空间
    if (globalMap.addNamespace("space")) {
        std::cout << "成功添加命名空间: space" << std::endl;
    }
    
    // 添加带命名空间的符号
    SymbolInfo nsSymbol(SymbolType::CUSTOM_ELEMENT, "Box", "space", "test.chtl", 10, 1);
    if (globalMap.addSymbol(nsSymbol)) {
        std::cout << "成功添加命名空间符号: " << nsSymbol.name << " 在 " << nsSymbol.namespace_path << std::endl;
    }
    
    // 查找命名空间符号
    SymbolInfo* nsFound = globalMap.findSymbol("Box", "space");
    if (nsFound) {
        std::cout << "找到命名空间符号: " << nsFound->name << " 在 " << nsFound->namespace_path << std::endl;
    }
    
    std::cout << "符号总数: " << globalMap.getSymbolCount() << std::endl;
    std::cout << "命名空间总数: " << globalMap.getNamespaceCount() << std::endl;
    std::cout << "模板总数: " << globalMap.getTemplateCount() << std::endl;
    std::cout << "自定义对象总数: " << globalMap.getCustomObjectCount() << std::endl;
    
    // 显示调试信息
    std::cout << std::endl << "调试信息:" << std::endl;
    std::cout << globalMap.getDebugInfo() << std::endl;
    
    std::cout << std::endl;
}

void testTokenTypes() {
    std::cout << "=== 测试Token类型 ===" << std::endl;
    
    // 测试CHTL Token类型
    std::cout << "CHTL Token类型:" << std::endl;
    std::cout << "  HTML_ELEMENT: " << static_cast<int>(TokenType::HTML_ELEMENT) << std::endl;
    std::cout << "  TEXT_LITERAL: " << static_cast<int>(TokenType::TEXT_LITERAL) << std::endl;
    std::cout << "  STYLE_KEYWORD: " << static_cast<int>(TokenType::STYLE_KEYWORD) << std::endl;
    std::cout << "  TEMPLATE_START: " << static_cast<int>(TokenType::TEMPLATE_START) << std::endl;
    std::cout << "  CUSTOM_START: " << static_cast<int>(TokenType::CUSTOM_START) << std::endl;
    std::cout << "  ORIGIN_START: " << static_cast<int>(TokenType::ORIGIN_START) << std::endl;
    std::cout << "  IMPORT_START: " << static_cast<int>(TokenType::IMPORT_START) << std::endl;
    std::cout << "  NAMESPACE_START: " << static_cast<int>(TokenType::NAMESPACE_START) << std::endl;
    
    // 测试CHTL JS Token类型
    std::cout << std::endl << "CHTL JS Token类型:" << std::endl;
    std::cout << "  ENHANCED_SELECTOR_START: " << static_cast<int>(CHTLJSTokenType::ENHANCED_SELECTOR_START) << std::endl;
    std::cout << "  CHAIN_ACCESS: " << static_cast<int>(CHTLJSTokenType::CHAIN_ACCESS) << std::endl;
    std::cout << "  LISTEN_KEYWORD: " << static_cast<int>(CHTLJSTokenType::LISTEN_KEYWORD) << std::endl;
    std::cout << "  DELEGATE_KEYWORD: " << static_cast<int>(CHTLJSTokenType::DELEGATE_KEYWORD) << std::endl;
    std::cout << "  ANIMATE_KEYWORD: " << static_cast<int>(CHTLJSTokenType::ANIMATE_KEYWORD) << std::endl;
    
    std::cout << std::endl;
}

void testSymbolTypes() {
    std::cout << "=== 测试符号类型 ===" << std::endl;
    
    std::cout << "符号类型:" << std::endl;
    std::cout << "  TEMPLATE_STYLE: " << static_cast<int>(SymbolType::TEMPLATE_STYLE) << std::endl;
    std::cout << "  TEMPLATE_ELEMENT: " << static_cast<int>(SymbolType::TEMPLATE_ELEMENT) << std::endl;
    std::cout << "  TEMPLATE_VAR: " << static_cast<int>(SymbolType::TEMPLATE_VAR) << std::endl;
    std::cout << "  CUSTOM_STYLE: " << static_cast<int>(SymbolType::CUSTOM_STYLE) << std::endl;
    std::cout << "  CUSTOM_ELEMENT: " << static_cast<int>(SymbolType::CUSTOM_ELEMENT) << std::endl;
    std::cout << "  CUSTOM_VAR: " << static_cast<int>(SymbolType::CUSTOM_VAR) << std::endl;
    std::cout << "  NAMESPACE: " << static_cast<int>(SymbolType::NAMESPACE) << std::endl;
    std::cout << "  IMPORTED_MODULE: " << static_cast<int>(SymbolType::IMPORTED_MODULE) << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "CHTL词法分析器系统简化测试" << std::endl;
    std::cout << "============================" << std::endl << std::endl;
    
    try {
        testTokens();
        testGlobalMap();
        testTokenTypes();
        testSymbolTypes();
        
        std::cout << "所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}