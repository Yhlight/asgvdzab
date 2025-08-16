#include "ast/AttributeNode.h"
#include <sstream>

namespace chtl {

AttributeNode::AttributeNode(const std::string& attr_name, const std::string& attr_value)
    : ASTNode(ASTNodeType::ATTRIBUTE), name(attr_name), value(attr_value) {
    generateNodeId();
    node_id = "attr_" + node_id;
}

std::string AttributeNode::getName() const {
    return name;
}

void AttributeNode::setName(const std::string& attr_name) {
    name = attr_name;
}

std::string AttributeNode::getValue() const {
    return value;
}

void AttributeNode::setValue(const std::string& attr_value) {
    value = attr_value;
}

std::unique_ptr<ASTNode> AttributeNode::clone() const {
    auto cloned_node = std::make_unique<AttributeNode>(name, value);
    cloned_node->line = line;
    cloned_node->column = column;
    cloned_node->start_position = start_position;
    cloned_node->end_position = end_position;
    cloned_node->source_file = source_file;
    return cloned_node;
}

std::string AttributeNode::getNodeTypeName() const {
    return "Attribute";
}

std::string AttributeNode::serialize() const {
    std::ostringstream oss;
    oss << "AttributeNode {";
    oss << " name: " << name;
    oss << ", value: \"" << value << "\"";
    oss << ", line: " << line;
    oss << ", column: " << column;
    oss << " }";
    return oss.str();
}

std::vector<std::string> AttributeNode::validate() const {
    std::vector<std::string> errors;
    if (name.empty()) {
        errors.push_back("属性名不能为空");
    }
    if (type != ASTNodeType::ATTRIBUTE) {
        errors.push_back("节点类型不是属性");
    }
    return errors;
}

} // namespace chtl