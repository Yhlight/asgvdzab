#include <iostream>
#include <string>
#include "lexer/CHTLToken.h"
#include "lexer/GlobalMap.h"
#include "lexer/LexerState.h"
#include "lexer/LexerContext.h"
#include "lexer/CHTLLexer.h"

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
    
    std::cout << "符号总数: " << globalMap.getSymbolCount() << std::endl;
    std::cout << std::endl;
}

void testLexerState() {
    std::cout << "=== 测试LexerState系统 ===" << std::endl;
    
    LexerStateManager stateManager;
    
    std::cout << "初始状态: " << static_cast<int>(stateManager.getCurrentState()) << std::endl;
    
    // 状态转换
    stateManager.setState(LexerState::IN_TEMPLATE_DECLARATION);
    std::cout << "转换后状态: " << static_cast<int>(stateManager.getCurrentState()) << std::endl;
    
    // 压栈
    stateManager.pushState(LexerState::IN_ATTRIBUTE, "style");
    std::cout << "压栈后栈深度: " << stateManager.getStackDepth() << std::endl;
    
    // 弹栈
    stateManager.popState();
    std::cout << "弹栈后栈深度: " << stateManager.getStackDepth() << std::endl;
    
    std::cout << std::endl;
}

void testLexerContext() {
    std::cout << "=== 测试LexerContext系统 ===" << std::endl;
    
    LexerContext context;
    
    // 推入上下文
    context.pushContext(ContextType::TEMPLATE, "DefaultText", 1, 1);
    std::cout << "当前上下文类型: " << static_cast<int>(context.getCurrentContextType()) << std::endl;
    std::cout << "当前上下文名称: " << context.getCurrentContextName() << std::endl;
    
    // 嵌套上下文
    context.pushContext(ContextType::STYLE_BLOCK, "", 2, 1);
    std::cout << "嵌套后上下文深度: " << context.getContextDepth() << std::endl;
    
    // 弹出上下文
    context.popContext();
    std::cout << "弹出后上下文深度: " << context.getContextDepth() << std::endl;
    
    std::cout << std::endl;
}

void testCHTLLexer() {
    std::cout << "=== 测试CHTLLexer系统 ===" << std::endl;
    
    LexerConfig config;
    config.enable_debug = true;
    config.strict_mode = true;
    
    CHTLLexer lexer(config);
    
    // 测试简单的CHTL代码
    std::string testCode = R"(
// 测试注释
html {
    head {
        title {
            text {
                "测试页面"
            }
        }
    }
    
    body {
        div {
            style {
                .box {
                    width: 100px;
                    height: 100px;
                    background-color: red;
                }
            }
            
            text {
                这是一个测试div
            }
        }
    }
}
)";
    
    std::cout << "开始词法分析..." << std::endl;
    auto tokens = lexer.tokenize(testCode);
    
    std::cout << "生成的Token数量: " << tokens.size() << std::endl;
    
    // 显示前10个Token
    std::cout << "前10个Token:" << std::endl;
    for (size_t i = 0; i < std::min(size_t(10), tokens.size()); i++) {
        std::cout << "  " << i << ": " << tokens[i].toString() << std::endl;
    }
    
    if (tokens.size() > 10) {
        std::cout << "  ... 还有 " << (tokens.size() - 10) << " 个Token" << std::endl;
    }
    
    // 显示统计信息
    auto stats = lexer.getStats();
    std::cout << "统计信息:" << std::endl;
    std::cout << "  总Token数: " << stats.total_tokens << std::endl;
    std::cout << "  总行数: " << stats.total_lines << std::endl;
    std::cout << "  总字符数: " << stats.total_characters << std::endl;
    std::cout << "  注释数: " << stats.comment_count << std::endl;
    
    // 检查错误
    if (lexer.hasErrors()) {
        std::cout << "词法分析错误:" << std::endl;
        for (const auto& error : lexer.getErrors()) {
            std::cout << "  " << error << std::endl;
        }
    } else {
        std::cout << "词法分析成功，无错误" << std::endl;
    }
    
    std::cout << std::endl;
}

void testCHTLJSLexer() {
    std::cout << "=== 测试CHTLJSLexer系统 ===" << std::endl;
    
    LexerConfig config;
    config.enable_debug = true;
    config.strict_mode = true;
    
    CHTLJSLexer jsLexer(config);
    
    // 测试CHTL JS代码
    std::string testJS = R"(
script {
    // 测试增强选择器
    {{box}}->click();
    
    // 测试事件监听器
    {{button}}->listen({
        click: function() {
            {{.modal}}->show();
        }
    });
    
    // 测试事件委托
    {{.parent}}->delegate({
        target: {{.child}},
        click: function() {
            console.log('Child clicked');
        }
    });
    
    // 测试动画
    animate({
        duration: 1000,
        easing: ease-in-out,
        begin: {
            opacity: 0
        },
        end: {
            opacity: 1
        }
    });
}
)";
    
    std::cout << "开始CHTL JS词法分析..." << std::endl;
    auto tokens = jsLexer.tokenize(testJS);
    
    std::cout << "生成的Token数量: " << tokens.size() << std::endl;
    
    // 显示前10个Token
    std::cout << "前10个Token:" << std::endl;
    for (size_t i = 0; i < std::min(size_t(10), tokens.size()); i++) {
        std::cout << "  " << i << ": " << tokens[i].toString() << std::endl;
    }
    
    if (tokens.size() > 10) {
        std::cout << "  ... 还有 " << (tokens.size() - 10) << " 个Token" << std::endl;
    }
    
    // 显示统计信息
    auto stats = jsLexer.getStats();
    std::cout << "统计信息:" << std::endl;
    std::cout << "  总Token数: " << stats.total_tokens << std::endl;
    std::cout << "  总行数: " << stats.total_lines << std::endl;
    std::cout << "  增强选择器数: " << stats.enhanced_selector_count << std::endl;
    std::cout << "  链式访问数: " << stats.chain_access_count << std::endl;
    std::cout << "  监听器数: " << stats.listen_count << std::endl;
    std::cout << "  事件委托数: " << stats.delegate_count << std::endl;
    std::cout << "  动画数: " << stats.animate_count << std::endl;
    
    // 检查错误
    if (jsLexer.hasErrors()) {
        std::cout << "CHTL JS词法分析错误:" << std::endl;
        for (const auto& error : jsLexer.getErrors()) {
            std::cout << "  " << error << std::endl;
        }
    } else {
        std::cout << "CHTL JS词法分析成功，无错误" << std::endl;
    }
    
    std::cout << std::endl;
}

int main() {
    std::cout << "CHTL词法分析器系统测试" << std::endl;
    std::cout << "========================" << std::endl << std::endl;
    
    try {
        testTokens();
        testGlobalMap();
        testLexerState();
        testLexerContext();
        testCHTLLexer();
        testCHTLJSLexer();
        
        std::cout << "所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}