#pragma once

#include "ASTNode.h"

namespace chtl {

// 属性节点类
class AttributeNode : public ASTNode {
public:
    std::string name;           // 属性名
    std::string value;          // 属性值
    
    AttributeNode(const std::string& attr_name, const std::string& attr_value);
    ~AttributeNode() = default;
    
    // 获取属性名
    std::string getName() const;
    
    // 设置属性名
    void setName(const std::string& attr_name);
    
    // 获取属性值
    std::string getValue() const;
    
    // 设置属性值
    void setValue(const std::string& attr_value);
    
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