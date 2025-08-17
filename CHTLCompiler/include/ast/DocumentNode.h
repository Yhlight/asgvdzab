#pragma once

#include "ASTNode.h"

namespace chtl {

// 文档节点类
class DocumentNode : public ASTNode {
public:
    std::vector<std::shared_ptr<ASTNode>> children;    // 子节点列表
    
    DocumentNode();
    ~DocumentNode() = default;
    
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