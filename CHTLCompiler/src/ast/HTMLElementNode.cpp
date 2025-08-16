#include "ast/HTMLElementNode.h"
#include <sstream>

namespace chtl {

HTMLElementNode::HTMLElementNode(const std::string& element_name)
    : ASTNode(ASTNodeType::HTML_ELEMENT), tag_name(element_name) {
    generateNodeId();
    node_id = "element_" + node_id;
}

std::string HTMLElementNode::getTagName() const {
    return tag_name;
}

void HTMLElementNode::setTagName(const std::string& name) {
    tag_name = name;
}

void HTMLElementNode::addAttribute(std::shared_ptr<ASTNode> attribute) {
    if (attribute) {
        attributes.push_back(attribute);
    }
}

std::vector<std::shared_ptr<ASTNode>> HTMLElementNode::getAttributes() const {
    return attributes;
}

void HTMLElementNode::addChild(std::shared_ptr<ASTNode> child) {
    if (child) {
        children.push_back(child);
    }
}

std::vector<std::shared_ptr<ASTNode>> HTMLElementNode::getChildren() const {
    return children;
}

std::unique_ptr<ASTNode> HTMLElementNode::clone() const {
    auto cloned_node = std::make_unique<HTMLElementNode>(tag_name);
    cloned_node->line = line;
    cloned_node->column = column;
    cloned_node->start_position = start_position;
    cloned_node->end_position = end_position;
    cloned_node->source_file = source_file;
    
    // 克隆属性
    for (const auto& attr : attributes) {
        if (attr) {
            cloned_node->addAttribute(attr->clone());
        }
    }
    
    // 克隆子节点
    for (const auto& child : children) {
        if (child) {
            cloned_node->addChild(child->clone());
        }
    }
    
    return cloned_node;
}

std::string HTMLElementNode::getNodeTypeName() const {
    return "HTMLElement";
}

std::string HTMLElementNode::serialize() const {
    std::ostringstream oss;
    oss << "HTMLElementNode {";
    oss << " tag: " << tag_name;
    oss << ", attributes: " << attributes.size();
    oss << ", children: " << children.size();
    oss << ", line: " << line;
    oss << ", column: " << column;
    oss << " }";
    return oss.str();
}

std::vector<std::string> HTMLElementNode::validate() const {
    std::vector<std::string> errors;
    if (tag_name.empty()) {
        errors.push_back("HTML元素标签名不能为空");
    }
    if (type != ASTNodeType::HTML_ELEMENT) {
        errors.push_back("节点类型不是HTML元素");
    }
    return errors;
}

} // namespace chtl