#pragma once

#include "ASTNode.h"

namespace chtl {

// 文本节点类
class TextNode : public ASTNode {
public:
    std::string content;        // 文本内容
    
    TextNode(const std::string& text_content);
    ~TextNode() = default;
    
    // 获取文本内容
    std::string getContent() const;
    
    // 设置文本内容
    void setContent(const std::string& content);
    
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