#pragma once

#include "ASTNode.h"

namespace chtl {

// 字面量节点类
class LiteralNode : public ASTNode {
public:
    std::string content;        // 字面量内容
    ASTNodeType literal_type;   // 字面量类型
    
    LiteralNode(const std::string& literal_content, ASTNodeType type);
    ~LiteralNode() = default;
    
    // 获取字面量内容
    std::string getContent() const;
    
    // 设置字面量内容
    void setContent(const std::string& content);
    
    // 获取字面量类型
    ASTNodeType getLiteralType() const;
    
    // 设置字面量类型
    void setLiteralType(ASTNodeType type);
    
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