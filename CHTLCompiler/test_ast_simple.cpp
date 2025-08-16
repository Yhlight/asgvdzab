#include <iostream>
#include <string>
#include <memory>
#include "ast/ASTNode.h"

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
    auto html = std::make_unique<ASTNode>(ASTNodeType::HTML_ELEMENT, 2, 1, 10, 90);
    html->generateNodeId();
    
    // 创建head元素
    auto head = std::make_unique<ASTNode>(ASTNodeType::HTML_ELEMENT, 3, 5, 20, 40);
    head->generateNodeId();
    
    // 创建title元素
    auto title = std::make_unique<ASTNode>(ASTNodeType::HTML_ELEMENT, 4, 9, 25, 35);
    title->generateNodeId();
    
    // 创建文本节点
    auto titleText = std::make_unique<ASTNode>(ASTNodeType::TEXT_NODE, 5, 13, 30, 32);
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
    
    // 测试元数据
    root->setMetadata("source", "test.chtl");
    root->setMetadata("version", "1.0.0");
    
    std::cout << "源文件: " << root->getMetadata("source") << std::endl;
    std::cout << "版本: " << root->getMetadata("version") << std::endl;
    std::cout << "是否有源文件元数据: " << (root->hasMetadata("source") ? "是" : "否") << std::endl;
    
    // 测试节点验证
    std::cout << "根节点是否有效: " << (root->isValid() ? "是" : "否") << std::endl;
    
    auto validationErrors = root->validate();
    if (validationErrors.empty()) {
        std::cout << "验证错误: 无" << std::endl;
    } else {
        std::cout << "验证错误:" << std::endl;
        for (const auto& error : validationErrors) {
            std::cout << "  " << error << std::endl;
        }
    }
    
    // 测试节点遍历
    std::cout << "\n节点遍历测试:" << std::endl;
    root->traverse([](ASTNode* node) {
        std::cout << "  访问节点: " << node->toDebugString() << std::endl;
    });
    
    std::cout << "\nConst节点遍历测试:" << std::endl;
    root->traverseConst([](const ASTNode* node) {
        std::cout << "  访问节点: " << node->toDebugString() << std::endl;
    });
    
    // 测试节点克隆
    std::cout << "\n节点克隆测试:" << std::endl;
    auto clonedRoot = root->clone();
    std::cout << "克隆的根节点: " << clonedRoot->toDebugString() << std::endl;
    std::cout << "克隆的节点总数: " << clonedRoot->getTotalNodeCount() << std::endl;
    
    // 测试节点比较
    std::cout << "原节点与克隆节点是否相等: " << (root->equals(clonedRoot.get()) ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

void testNodeTypes() {
    std::cout << "=== 测试节点类型 ===" << std::endl;
    
    // 测试各种节点类型
    std::vector<ASTNodeType> nodeTypes = {
        ASTNodeType::ROOT,
        ASTNodeType::COMMENT,
        ASTNodeType::TEXT_NODE,
        ASTNodeType::LITERAL,
        ASTNodeType::HTML_ELEMENT,
        ASTNodeType::ATTRIBUTE,
        ASTNodeType::STYLE_BLOCK,
        ASTNodeType::TEMPLATE_DECLARATION,
        ASTNodeType::CUSTOM_DECLARATION,
        ASTNodeType::ORIGIN_DECLARATION,
        ASTNodeType::IMPORT_DECLARATION,
        ASTNodeType::NAMESPACE_DECLARATION,
        ASTNodeType::SCRIPT_BLOCK
    };
    
    for (auto type : nodeTypes) {
        auto node = std::make_unique<ASTNode>(type, 1, 1, 0, 10);
        node->generateNodeId();
        std::cout << "节点类型 " << static_cast<int>(type) << ": " << node->getNodeTypeName() << std::endl;
    }
    
    std::cout << std::endl;
}

void testNodeOperations() {
    std::cout << "=== 测试节点操作 ===" << std::endl;
    
    // 创建父节点
    auto parent = std::make_unique<ASTNode>(ASTNodeType::HTML_ELEMENT, 1, 1, 0, 50);
    parent->generateNodeId();
    
    // 创建子节点
    auto child1 = std::make_unique<ASTNode>(ASTNodeType::TEXT_NODE, 2, 5, 10, 20);
    child1->generateNodeId();
    
    auto child2 = std::make_unique<ASTNode>(ASTNodeType::ATTRIBUTE, 3, 5, 25, 35);
    child2->generateNodeId();
    
    // 添加子节点
    parent->addChild(std::move(child1));
    parent->addChild(std::move(child2));
    
    std::cout << "父节点子节点数量: " << parent->getChildCount() << std::endl;
    
    // 获取子节点
    auto firstChild = parent->getChild(0);
    if (firstChild) {
        std::cout << "第一个子节点: " << firstChild->toDebugString() << std::endl;
    }
    
    auto secondChild = parent->getChild(1);
    if (secondChild) {
        std::cout << "第二个子节点: " << secondChild->toDebugString() << std::endl;
    }
    
    // 测试无效索引
    auto invalidChild = parent->getChild(10);
    if (!invalidChild) {
        std::cout << "无效索引返回nullptr: 正确" << std::endl;
    }
    
    // 测试节点路径
    std::cout << "父节点路径: " << parent->getNodePath() << std::endl;
    if (firstChild) {
        std::cout << "第一个子节点路径: " << firstChild->getNodePath() << std::endl;
        std::cout << "第一个子节点祖先路径: ";
        auto ancestors = firstChild->getAncestorPath();
        for (size_t i = 0; i < ancestors.size(); i++) {
            if (i > 0) std::cout << " -> ";
            std::cout << ancestors[i];
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
}

int main() {
    std::cout << "CHTL AST节点系统简化测试" << std::endl;
    std::cout << "=========================" << std::endl << std::endl;
    
    try {
        testASTNode();
        testNodeTypes();
        testNodeOperations();
        
        std::cout << "所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}