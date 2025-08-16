#pragma once

#include "ASTNode.h"

namespace chtl {

// HTML元素节点类
class HTMLElementNode : public ASTNode {
public:
    std::string tag_name;       // 标签名
    std::vector<std::shared_ptr<ASTNode>> attributes;  // 属性列表
    std::vector<std::shared_ptr<ASTNode>> children;    // 子节点列表
    
    HTMLElementNode(const std::string& element_name);
    ~HTMLElementNode() = default;
    
    // 获取标签名
    std::string getTagName() const;
    
    // 设置标签名
    void setTagName(const std::string& name);
    
    // 添加属性
    void addAttribute(std::shared_ptr<ASTNode> attribute);
    
    // 获取属性列表
    std::vector<std::shared_ptr<ASTNode>> getAttributes() const;
    
    // 添加子节点
    void addChild(std::shared_ptr<ASTNode> child);
    
    // 获取子节点列表
    std::vector<std::shared_ptr<ASTNode>> getChildren() const;
    
    // 克隆节点
    std::unique_ptr<ASTNode> clone() const override;
    
    // 获取节点类型名称
    std::string getNodeTypeName() const;
    
    // 序列化
    std::string serialize() const;
    
    // 验证节点
    std::vector<std::string> validate() const;
};

} // namespace chtl