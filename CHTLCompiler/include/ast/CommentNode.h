#pragma once

#include "ASTNode.h"

namespace chtl {

// 注释节点类
class CommentNode : public ASTNode {
public:
    std::string content;        // 注释内容
    ASTNodeType comment_type;   // 注释类型
    
    CommentNode(const std::string& comment_content, ASTNodeType type);
    ~CommentNode() = default;
    
    // 获取注释内容
    std::string getContent() const;
    
    // 设置注释内容
    void setContent(const std::string& content);
    
    // 获取注释类型
    ASTNodeType getCommentType() const;
    
    // 设置注释类型
    void setCommentType(ASTNodeType type);
    
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