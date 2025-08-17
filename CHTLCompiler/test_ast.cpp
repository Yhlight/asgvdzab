#include <iostream>
#include <string>
#include <memory>
#include "ast/ASTNode.h"
#include "ast/CHTLASTNodes.h"
#include "ast/CHTLJSASTNode.h"

using namespace chtl;

void testASTNode() {
    std::cout << "=== 测试ASTNode基类 ===" << std::endl;
    
    // 创建根节点
    auto root = std::make_unique<ASTNode>(ASTNodeType::ROOT, 1, 1, 0, 100);
    root->generateNodeId();
    
    std::cout << "根节点: " << root->toDebugString() << std::endl;
    std::cout << "节点路径: " << root->getNodePath() << std::endl;
    std::cout << "节点深度: " << root->getDepth() << std::endl;
    
    // 创建HTML元素节点
    auto html = std::make_unique<HTMLElementNode>("html", false, 2, 1, 10, 90);
    html->generateNodeId();
    
    // 创建head元素
    auto head = std::make_unique<HTMLElementNode>("head", false, 3, 5, 20, 40);
    head->generateNodeId();
    
    // 创建title元素
    auto title = std::make_unique<HTMLElementNode>("title", false, 4, 9, 25, 35);
    title->generateNodeId();
    
    // 创建文本节点
    auto titleText = std::make_unique<TextNode>("测试页面", true, 5, 13, 30, 32);
    titleText->generateNodeId();
    
    // 构建节点树
    title->addChild(std::move(titleText));
    head->addChild(std::move(title));
    html->addChild(std::move(head));
    root->addChild(std::move(html));
    
    std::cout << "节点总数: " << root->getTotalNodeCount() << std::endl;
    std::cout << "HTML元素数量: " << root->getNodeCountByType(ASTNodeType::HTML_ELEMENT) << std::endl;
    std::cout << "文本节点数量: " << root->getNodeCountByType(ASTNodeType::TEXT_NODE) << std::endl;
    
    // 查找节点
    auto htmlNodes = root->findNodesByType(ASTNodeType::HTML_ELEMENT);
    std::cout << "找到 " << htmlNodes.size() << " 个HTML元素节点" << std::endl;
    
    auto firstHtml = root->findFirstNodeByType(ASTNodeType::HTML_ELEMENT);
    if (firstHtml) {
        std::cout << "第一个HTML元素: " << firstHtml->toDebugString() << std::endl;
    }
    
    std::cout << std::endl;
}

void testCHTLASTNodes() {
    std::cout << "=== 测试CHTL AST节点 ===" << std::endl;
    
    // 测试注释节点
    auto comment = std::make_unique<CommentNode>("//", "这是一个测试注释", false, 1, 1, 0, 20);
    comment->generateNodeId();
    std::cout << "注释节点: " << comment->toDebugString() << std::endl;
    std::cout << "注释类型: " << comment->comment_type << std::endl;
    std::cout << "注释内容: " << comment->content << std::endl;
    
    // 测试文本节点
    auto text = std::make_unique<TextNode>("Hello World", true, 2, 1, 25, 35);
    text->generateNodeId();
    std::cout << "文本节点: " << text->toDebugString() << std::endl;
    std::cout << "文本内容: " << text->text_content << std::endl;
    std::cout << "是否引号包围: " << (text->is_quoted ? "是" : "否") << std::endl;
    
    // 测试字面量节点
    auto literal = std::make_unique<LiteralNode>("red", "\"", 3, 1, 40, 45);
    literal->generateNodeId();
    std::cout << "字面量节点: " << literal->toDebugString() << std::endl;
    std::cout << "字面量值: " << literal->value << std::endl;
    std::cout << "引号类型: " << literal->quote_type << std::endl;
    
    // 测试HTML元素节点
    auto div = std::make_unique<HTMLElementNode>("div", false, 4, 1, 50, 80);
    div->generateNodeId();
    
    // 添加属性
    auto classAttr = std::make_unique<AttributeNode>("class", std::make_unique<LiteralNode>("container", "\""), ":", 5, 5, 55, 65);
    classAttr->generateNodeId();
    div->addAttribute(std::move(classAttr));
    
    // 添加子元素
    auto span = std::make_unique<HTMLElementNode>("span", false, 6, 9, 70, 75);
    span->generateNodeId();
    div->addChild(std::move(span));
    
    std::cout << "HTML元素节点: " << div->toDebugString() << std::endl;
    std::cout << "标签名: " << div->tag_name << std::endl;
    std::cout << "属性数量: " << div->attributes.size() << std::endl;
    std::cout << "子元素数量: " << div->children.size() << std::endl;
    
    // 测试样式块节点
    auto style = std::make_unique<StyleBlockNode>(7, 1, 85, 95);
    style->generateNodeId();
    
    // 添加CSS规则
    auto cssRule = std::make_unique<CSSRuleNode>(
        std::make_unique<CSSSelectorNode>(".", "box", false, 8, 9, 87, 90),
        8, 9, 87, 93
    );
    cssRule->generateNodeId();
    
    // 添加CSS属性
    auto cssProp = std::make_unique<CSSPropertyNode>(
        "width", 
        std::make_unique<LiteralNode>("100px", ""),
        8, 13, 87, 93
    );
    cssProp->generateNodeId();
    cssRule->addProperty(std::move(cssProp));
    
    style->addCSSRule(std::move(cssRule));
    
    std::cout << "样式块节点: " << style->toDebugString() << std::endl;
    std::cout << "CSS规则数量: " << style->css_rules.size() << std::endl;
    
    std::cout << std::endl;
}

void testCHTLJSASTNodes() {
    std::cout << "=== 测试CHTL JS AST节点 ===" << std::endl;
    
    // 测试增强选择器节点
    auto selector = std::make_unique<EnhancedSelectorNode>(".box", "0", 1, 1, 0, 10);
    selector->generateNodeId();
    std::cout << "增强选择器节点: " << selector->toDebugString() << std::endl;
    std::cout << "选择器内容: " << selector->selector_content << std::endl;
    std::cout << "数组索引: " << selector->array_index << std::endl;
    std::cout << "是否有数组访问: " << (selector->has_array_access ? "是" : "否") << std::endl;
    
    // 测试链式访问节点
    auto chainAccess = std::make_unique<ChainAccessNode>("->", "click", 2, 1, 15, 25);
    chainAccess->generateNodeId();
    std::cout << "链式访问节点: " << chainAccess->toDebugString() << std::endl;
    std::cout << "访问类型: " << chainAccess->access_type << std::endl;
    std::cout << "方法名: " << chainAccess->method_name << std::endl;
    
    // 测试增强监听器节点
    auto listen = std::make_unique<ListenDeclarationNode>(".button", 3, 1, 30, 50);
    listen->generateNodeId();
    
    // 添加事件处理器
    auto clickHandler = std::make_unique<LiteralNode>("function() { console.log('clicked'); }", "", 4, 9, 35, 45);
    clickHandler->generateNodeId();
    listen->addEventHandler("click", std::move(clickHandler));
    
    std::cout << "增强监听器节点: " << listen->toDebugString() << std::endl;
    std::cout << "目标选择器: " << listen->target_selector << std::endl;
    std::cout << "事件处理器数量: " << listen->event_handlers.size() << std::endl;
    
    // 测试事件委托节点
    auto delegate = std::make_unique<DelegateDeclarationNode>(".parent", 5, 1, 55, 75);
    delegate->generateNodeId();
    
    // 添加委托目标
    auto targetSelector = std::make_unique<LiteralNode>(".child", "", 6, 9, 60, 65);
    targetSelector->generateNodeId();
    delegate->addTargetSelector(std::move(targetSelector));
    
    // 添加事件处理器
    auto delegateHandler = std::make_unique<LiteralNode>("function() { console.log('delegated'); }", "", 7, 9, 70, 75);
    delegateHandler->generateNodeId();
    delegate->addEventHandler("click", std::move(delegateHandler));
    
    std::cout << "事件委托节点: " << delegate->toDebugString() << std::endl;
    std::cout << "父元素选择器: " << delegate->parent_selector << std::endl;
    std::cout << "目标选择器数量: " << delegate->target_selectors.size() << std::endl;
    std::cout << "事件处理器数量: " << delegate->event_handlers.size() << std::endl;
    
    // 测试动画声明节点
    auto animate = std::make_unique<AnimateDeclarationNode>(8, 1, 80, 100);
    animate->generateNodeId();
    
    // 设置动画属性
    animate->setDuration(std::make_unique<LiteralNode>("1000", "", 9, 9, 85, 89));
    animate->setEasing(std::make_unique<LiteralNode>("ease-in-out", "", 10, 9, 90, 94));
    animate->setBeginState(std::make_unique<LiteralNode>("{ opacity: 0 }", "", 11, 9, 95, 99));
    animate->setEndState(std::make_unique<LiteralNode>("{ opacity: 1 }", "", 12, 9, 100, 104));
    
    std::cout << "动画声明节点: " << animate->toDebugString() << std::endl;
    std::cout << "是否有持续时间: " << (animate->duration ? "是" : "否") << std::endl;
    std::cout << "是否有缓动函数: " << (animate->easing ? "是" : "否") << std::endl;
    std::cout << "是否有开始状态: " << (animate->begin_state ? "是" : "否") << std::endl;
    std::cout << "是否有结束状态: " << (animate->end_state ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

void testNodeValidation() {
    std::cout << "=== 测试节点验证 ===" << std::endl;
    
    // 创建有效的节点
    auto validNode = std::make_unique<HTMLElementNode>("div", false, 1, 1, 0, 10);
    validNode->generateNodeId();
    
    std::cout << "有效节点验证: " << (validNode->isValid() ? "通过" : "失败") << std::endl;
    
    auto validationErrors = validNode->validate();
    if (validationErrors.empty()) {
        std::cout << "验证错误: 无" << std::endl;
    } else {
        std::cout << "验证错误:" << std::endl;
        for (const auto& error : validationErrors) {
            std::cout << "  " << error << std::endl;
        }
    }
    
    // 创建无效的节点
    auto invalidNode = std::make_unique<ASTNode>(ASTNodeType::UNKNOWN, 1, 1, 0, 10);
    invalidNode->generateNodeId();
    
    std::cout << "无效节点验证: " << (invalidNode->isValid() ? "通过" : "失败") << std::endl;
    
    auto invalidErrors = invalidNode->validate();
    if (invalidErrors.empty()) {
        std::cout << "验证错误: 无" << std::endl;
    } else {
        std::cout << "验证错误:" << std::endl;
        for (const auto& error : invalidErrors) {
            std::cout << "  " << error << std::endl;
        }
    }
    
    std::cout << std::endl;
}

void testNodeSerialization() {
    std::cout << "=== 测试节点序列化 ===" << std::endl;
    
    // 创建复杂的节点树
    auto root = std::make_unique<ASTNode>(ASTNodeType::ROOT, 1, 1, 0, 100);
    root->generateNodeId();
    
    auto html = std::make_unique<HTMLElementNode>("html", false, 2, 1, 10, 90);
    html->generateNodeId();
    
    auto head = std::make_unique<HTMLElementNode>("head", false, 3, 5, 20, 40);
    head->generateNodeId();
    
    auto title = std::make_unique<HTMLElementNode>("title", false, 4, 9, 25, 35);
    title->generateNodeId();
    
    auto titleText = std::make_unique<TextNode>("测试页面", true, 5, 13, 30, 32);
    titleText->generateNodeId();
    
    // 构建树
    title->addChild(std::move(titleText));
    head->addChild(std::move(title));
    html->addChild(std::move(head));
    root->addChild(std::move(html));
    
    // 序列化
    std::cout << "序列化结果:" << std::endl;
    std::cout << root->serialize() << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "CHTL AST节点系统测试" << std::endl;
    std::cout << "=====================" << std::endl << std::endl;
    
    try {
        testASTNode();
        testCHTLASTNodes();
        testCHTLJSASTNodes();
        testNodeValidation();
        testNodeSerialization();
        
        std::cout << "所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}