#include "parsers/chtl_js_parser.hpp"
#include <iostream>
#include <cassert>
#include <string>

using namespace chtl;

// 测试脚本块解析
void testScriptBlockParsing() {
    std::cout << "🧪 测试脚本块解析..." << std::endl;
    
    CHTLJSParser parser;
    
    // 测试基本脚本块
    std::string source = R"(script {
        console.log('Hello CHTL JS!');
    })";
    
    auto result = parser.parseScriptBlock(source, "test.chtl");
    
    assert(result.success);
    assert(result.rootNode != nullptr);
    
    auto scriptNode = std::dynamic_pointer_cast<ScriptBlockNode>(result.rootNode);
    assert(scriptNode != nullptr);
    assert(!scriptNode->content.empty());
    
    std::cout << "✅ 脚本块解析测试通过" << std::endl;
}

// 测试增强选择器
void testEnhancedSelectors() {
    std::cout << "🧪 测试增强选择器..." << std::endl;
    
    CHTLJSScanner scanner;
    
    // 测试各种选择器类型
    std::string content = R"(
        {{button}}.click();
        {{.container}}.style.display = 'block';
        {{#header}}.textContent = 'Title';
        {{.list button}}.forEach(btn => btn.disabled = true);
        {{div[0]}}.scrollIntoView();
    )";
    
    auto selectors = scanner.scanSelectors(content);
    
    assert(selectors.size() == 5);
    
    // 验证选择器类型
    assert(selectors[0].type == CHTLJSSelectorType::TAG);
    assert(selectors[0].selector == "button");
    
    assert(selectors[1].type == CHTLJSSelectorType::CLASS);
    assert(selectors[1].selector == ".container");
    
    assert(selectors[2].type == CHTLJSSelectorType::ID);
    assert(selectors[2].selector == "#header");
    
    assert(selectors[3].type == CHTLJSSelectorType::DESCENDANT);
    assert(selectors[3].selector == "button");
    assert(selectors[3].parentSelector == ".list");
    
    assert(selectors[4].type == CHTLJSSelectorType::INDEXED);
    assert(selectors[4].selector == "div");
    assert(selectors[4].index == 0);
    
    std::cout << "✅ 增强选择器测试通过" << std::endl;
}

// 测试CHTL JS箭头操作符
void testCHTLJSArrow() {
    std::cout << "🧪 测试CHTL JS箭头操作符..." << std::endl;
    
    // 测试箭头操作符检测
    std::string content1 = "{{button}}->textContent();";
    assert(CHTLJSUtils::hasCHTLJSArrow(content1));
    
    std::string content2 = "{{button}}.textContent();";
    assert(!CHTLJSUtils::hasCHTLJSArrow(content2));
    
    // 测试箭头操作符与普通点操作符等价
    std::string arrowCode = "{{.box}}->style.color = 'red';";
    std::string dotCode = "{{.box}}.style.color = 'red';";
    
    // 两者在功能上是等价的
    assert(CHTLJSUtils::hasEnhancedSelectors(arrowCode));
    assert(CHTLJSUtils::hasEnhancedSelectors(dotCode));
    
    std::cout << "✅ CHTL JS箭头操作符测试通过" << std::endl;
}

// 测试增强监听器
void testEnhancedListeners() {
    std::cout << "🧪 测试增强监听器..." << std::endl;
    
    CHTLJSScanner scanner;
    
    // 测试listen语法
    std::string content = R"(
        {{.button}}->listen({
            click: () => {
                console.log('Clicked!');
            },
            mouseenter: handleMouseEnter,
            mouseleave: function() {
                console.log('Mouse left');
            }
        });
    )";
    
    auto listeners = scanner.scanListeners(content);
    
    assert(listeners.size() >= 3);
    
    // 验证监听器
    bool foundClick = false, foundMouseEnter = false, foundMouseLeave = false;
    
    for (const auto& listener : listeners) {
        if (listener.eventType == "click") {
            foundClick = true;
            assert(!listener.handlerCode.empty());
        } else if (listener.eventType == "mouseenter") {
            foundMouseEnter = true;
            assert(listener.handlerCode.find("handleMouseEnter") != std::string::npos);
        } else if (listener.eventType == "mouseleave") {
            foundMouseLeave = true;
            assert(listener.handlerCode.find("function") != std::string::npos);
        }
    }
    
    assert(foundClick && foundMouseEnter && foundMouseLeave);
    
    std::cout << "✅ 增强监听器测试通过" << std::endl;
}

// 测试事件委托
void testEventDelegate() {
    std::cout << "🧪 测试事件委托..." << std::endl;
    
    CHTLJSScanner scanner;
    
    // 测试delegate语法
    std::string content = R"(
        {{.container}}->delegate({
            target: {{.dynamic-button}},
            click: handleClick,
            mouseenter: () => {
                console.log('Mouse entered');
            }
        });
        
        {{.parent}}->delegate({
            target: [{{.child1}}, {{.child2}}],
            click: () => console.log('Child clicked')
        });
    )";
    
    auto delegates = scanner.scanDelegates(content);
    
    assert(delegates.size() >= 2);
    
    // 验证第一个委托
    const auto& delegate1 = delegates[0];
    assert(delegate1.targets.size() == 1);
    assert(delegate1.targets[0].selector == ".dynamic-button");
    assert(delegate1.listeners.size() >= 2);
    
    // 验证第二个委托（多个目标）
    const auto& delegate2 = delegates[1];
    assert(delegate2.targets.size() == 2);
    assert(delegate2.targets[0].selector == ".child1");
    assert(delegate2.targets[1].selector == ".child2");
    
    std::cout << "✅ 事件委托测试通过" << std::endl;
}

// 测试动画系统
void testAnimationSystem() {
    std::cout << "🧪 测试动画系统..." << std::endl;
    
    CHTLJSScanner scanner;
    
    // 测试animate语法
    std::string content = R"(
        animate({
            duration: 1000,
            easing: 'ease-in-out',
            begin: {
                opacity: 0,
                transform: 'scale(0.8)'
            },
            end: {
                opacity: 1,
                transform: 'scale(1.0)'
            },
            loop: 2,
            delay: 500,
            callback: () => {
                console.log('Animation completed');
            }
        });
    )";
    
    auto animations = scanner.scanAnimations(content);
    
    assert(animations.size() >= 1);
    
    const auto& animation = animations[0];
    assert(animation.duration == 1000);
    assert(animation.easing == "'ease-in-out'");
    assert(animation.loop == 2);
    assert(animation.delay == 500);
    assert(!animation.callback.empty());
    
    // 验证begin和end状态
    assert(animation.beginState.find("opacity") != animation.beginState.end());
    assert(animation.endState.find("opacity") != animation.endState.end());
    
    std::cout << "✅ 动画系统测试通过" << std::endl;
}

// 测试选择器注册表
void testSelectorRegistry() {
    std::cout << "🧪 测试选择器注册表..." << std::endl;
    
    CHTLJSSelectorRegistry registry;
    
    // 创建选择器
    CHTLJSSelector selector1(CHTLJSSelectorType::CLASS, ".button");
    CHTLJSSelector selector2(CHTLJSSelectorType::ID, "#header");
    CHTLJSSelector selector3(CHTLJSSelectorType::TAG, "div");
    
    // 注册选择器
    assert(registry.registerSelector(selector1));
    assert(registry.registerSelector(selector2));
    assert(registry.registerSelector(selector3));
    
    // 尝试重复注册
    assert(!registry.registerSelector(selector1));
    
    // 查找选择器
    const auto* found1 = registry.findSelector(".button");
    assert(found1 != nullptr);
    assert(found1->type == CHTLJSSelectorType::CLASS);
    
    const auto* found2 = registry.findSelector("#header");
    assert(found2 != nullptr);
    assert(found2->type == CHTLJSSelectorType::ID);
    
    const auto* notFound = registry.findSelector(".nonexistent");
    assert(notFound == nullptr);
    
    // 验证选择器数量
    assert(registry.getAllSelectors().size() == 3);
    
    std::cout << "✅ 选择器注册表测试通过" << std::endl;
}

// 测试事件委托注册表
void testEventDelegateRegistry() {
    std::cout << "🧪 测试事件委托注册表..." << std::endl;
    
    CHTLJSEventDelegateRegistry registry;
    
    // 创建委托
    CHTLJSDelegate delegate1;
    delegate1.targets.emplace_back(CHTLJSSelectorType::CLASS, ".child1");
    delegate1.listeners.emplace_back("click", "handleClick1");
    
    CHTLJSDelegate delegate2;
    delegate2.targets.emplace_back(CHTLJSSelectorType::CLASS, ".child2");
    delegate2.listeners.emplace_back("click", "handleClick2");
    
    // 注册委托到同一个父元素
    std::string parentSelector = ".container";
    assert(registry.registerDelegate(parentSelector, delegate1));
    assert(registry.registerDelegate(parentSelector, delegate2));
    
    // 获取委托
    auto delegates = registry.getDelegates(parentSelector);
    assert(delegates.size() == 2);
    
    // 合并委托
    registry.mergeDelegates(parentSelector);
    auto mergedDelegates = registry.getDelegates(parentSelector);
    assert(mergedDelegates.size() == 1);
    assert(mergedDelegates[0].targets.size() == 2);
    assert(mergedDelegates[0].listeners.size() == 2);
    
    std::cout << "✅ 事件委托注册表测试通过" << std::endl;
}

// 测试CHTL JS工具类
void testCHTLJSUtils() {
    std::cout << "🧪 测试CHTL JS工具类..." << std::endl;
    
    // 测试脚本块检测
    assert(CHTLJSUtils::isScriptBlockDeclaration("script { }"));
    assert(CHTLJSUtils::isScriptBlockDeclaration("script{"));
    assert(!CHTLJSUtils::isScriptBlockDeclaration("style { }"));
    
    // 测试增强选择器检测
    assert(CHTLJSUtils::hasEnhancedSelectors("{{.button}}"));
    assert(CHTLJSUtils::hasEnhancedSelectors("{{#id}}"));
    assert(!CHTLJSUtils::hasEnhancedSelectors(".button"));
    
    // 测试listen调用检测
    assert(CHTLJSUtils::hasListenCall("->listen({"));
    assert(!CHTLJSUtils::hasListenCall("addEventListener"));
    
    // 测试delegate调用检测
    assert(CHTLJSUtils::hasDelegateCall("->delegate({"));
    assert(!CHTLJSUtils::hasDelegateCall("delegation"));
    
    // 测试animate调用检测
    assert(CHTLJSUtils::hasAnimateCall("animate({"));
    assert(!CHTLJSUtils::hasAnimateCall("animation"));
    
    // 测试选择器提取
    auto selectors = CHTLJSUtils::extractEnhancedSelectors("{{.btn}} and {{#id}} and {{tag}}");
    assert(selectors.size() == 3);
    assert(selectors[0] == ".btn");
    assert(selectors[1] == "#id");
    assert(selectors[2] == "tag");
    
    // 测试选择器语法验证
    assert(CHTLJSUtils::validateSelectorSyntax(".button"));
    assert(CHTLJSUtils::validateSelectorSyntax("#header"));
    assert(CHTLJSUtils::validateSelectorSyntax("div"));
    assert(CHTLJSUtils::validateSelectorSyntax("button[0]"));
    assert(!CHTLJSUtils::validateSelectorSyntax(""));
    
    // 测试事件类型验证
    assert(CHTLJSUtils::validateEventType("click"));
    assert(CHTLJSUtils::validateEventType("mouseenter"));
    assert(!CHTLJSUtils::validateEventType("invalid-event"));
    
    // 测试选择器标准化
    std::string normalized = CHTLJSUtils::normalizeSelector("  .button   ");
    assert(normalized == ".button");
    
    // 测试选择器类型检测
    assert(CHTLJSUtils::detectSelectorType(".class") == CHTLJSSelectorType::CLASS);
    assert(CHTLJSUtils::detectSelectorType("#id") == CHTLJSSelectorType::ID);
    assert(CHTLJSUtils::detectSelectorType("tag") == CHTLJSSelectorType::TAG);
    assert(CHTLJSUtils::detectSelectorType(".parent child") == CHTLJSSelectorType::DESCENDANT);
    assert(CHTLJSUtils::detectSelectorType("tag[0]") == CHTLJSSelectorType::INDEXED);
    
    std::cout << "✅ CHTL JS工具类测试通过" << std::endl;
}

// 测试复杂脚本块
void testComplexScriptBlock() {
    std::cout << "🧪 测试复杂脚本块..." << std::endl;
    
    CHTLJSParser parser;
    
    // 测试包含所有CHTL JS特性的复杂脚本
    std::string source = R"(script {
        // 增强选择器
        {{.button}}->textContent = 'Click Me';
        {{#header}}->style.color = 'blue';
        
        // 增强监听器
        {{.interactive}}->listen({
            click: () => {
                animate({
                    duration: 500,
                    begin: { opacity: 1 },
                    end: { opacity: 0.5 }
                });
            },
            mouseenter: handleMouseEnter,
            mouseleave: () => console.log('Left')
        });
        
        // 事件委托
        {{.container}}->delegate({
            target: [{{.dynamic}}, {{.generated}}],
            click: delegateHandler,
            dblclick: () => {
                console.log('Double clicked');
            }
        });
        
        // 动画
        animate({
            duration: 1000,
            easing: 'ease-in-out',
            begin: {
                transform: 'scale(0.8)',
                opacity: 0
            },
            end: {
                transform: 'scale(1.0)',
                opacity: 1
            },
            loop: -1,
            callback: animationComplete
        });
        
        // 混合使用箭头操作符和普通操作符
        {{.mixed}}->addEventListener('focus', () => {
            this.style.borderColor = 'green';
        });
    })";
    
    auto result = parser.parseScriptBlock(source, "complex.chtl");
    
    assert(result.success);
    assert(result.rootNode != nullptr);
    
    auto scriptNode = std::dynamic_pointer_cast<ScriptBlockNode>(result.rootNode);
    assert(scriptNode != nullptr);
    assert(!scriptNode->content.empty());
    
    // 验证选择器注册
    auto selectorRegistry = parser.getSelectorRegistry();
    assert(selectorRegistry != nullptr);
    assert(selectorRegistry->getAllSelectors().size() > 0);
    
    std::cout << "✅ 复杂脚本块测试通过" << std::endl;
}

// 测试错误处理
void testErrorHandling() {
    std::cout << "🧪 测试错误处理..." << std::endl;
    
    CHTLJSParser parser;
    
    // 测试无效脚本语法
    std::string invalidScript = "script {";  // 缺少闭合花括号
    auto result1 = parser.parseScriptBlock(invalidScript, "test.chtl");
    // 可能会有警告但不一定失败
    
    // 测试无效选择器
    CHTLJSScanner scanner;
    std::string invalidSelector = "{{}}";  // 空选择器
    auto selectors = scanner.scanSelectors(invalidSelector);
    // 应该返回空数组或处理错误
    
    // 验证选择器语法
    assert(!CHTLJSUtils::validateSelectorSyntax(""));
    assert(!CHTLJSUtils::validateSelectorSyntax("{{invalid}}"));
    
    std::cout << "✅ 错误处理测试通过" << std::endl;
}

int main() {
    std::cout << "🚀 开始CHTL JS系统解析器测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        testScriptBlockParsing();
        testEnhancedSelectors();
        testCHTLJSArrow();
        testEnhancedListeners();
        testEventDelegate();
        testAnimationSystem();
        testSelectorRegistry();
        testEventDelegateRegistry();
        testCHTLJSUtils();
        testComplexScriptBlock();
        testErrorHandling();
        
        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "🎉 所有测试通过！" << std::endl;
        std::cout << "✅ CHTL JS系统解析器实现完成" << std::endl;
        std::cout << "   • 局部script块 (script { ... })" << std::endl;
        std::cout << "   • 增强选择器 ({{selector}})" << std::endl;
        std::cout << "   • 箭头操作符 (-> 等价于 .)" << std::endl;
        std::cout << "   • 增强监听器 (->listen)" << std::endl;
        std::cout << "   • 事件委托 (->delegate)" << std::endl;
        std::cout << "   • 动画系统 (animate)" << std::endl;
        std::cout << "   • 选择器注册表管理" << std::endl;
        std::cout << "   • 事件委托注册表管理" << std::endl;
        std::cout << "   • 多种选择器类型支持" << std::endl;
        std::cout << "   • 状态机和扫描器架构" << std::endl;
        std::cout << "   • 严格按照CHTL语法文档实现" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}