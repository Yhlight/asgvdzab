#include "ast/DocumentNode.h"
#include <sstream>

namespace chtl {

DocumentNode::DocumentNode()
    : ASTNode(ASTNodeType::DOCUMENT) {
    generateNodeId();
    node_id = "document_" + node_id;
}

void DocumentNode::addChild(std::shared_ptr<ASTNode> child) {
    if (child) {
        children.push_back(child);
    }
}

std::vector<std::shared_ptr<ASTNode>> DocumentNode::getChildren() const {
    return children;
}

std::unique_ptr<ASTNode> DocumentNode::clone() const {
    auto cloned_node = std::make_unique<DocumentNode>();
    cloned_node->line = line;
    cloned_node->column = column;
    cloned_node->start_position = start_position;
    cloned_node->end_position = end_position;
    cloned_node->source_file = source_file;
    
    // 克隆子节点
    for (const auto& child : children) {
        if (child) {
            cloned_node->addChild(child->clone());
        }
    }
    
    return cloned_node;
}

std::string DocumentNode::getNodeTypeName() const {
    return "Document";
}

std::string DocumentNode::serialize() const {
    std::ostringstream oss;
    oss << "DocumentNode {";
    oss << " children: " << children.size();
    oss << ", line: " << line;
    oss << ", column: " << column;
    oss << " }";
    return oss.str();
}

std::vector<std::string> DocumentNode::validate() const {
    std::vector<std::string> errors;
    if (type != ASTNodeType::DOCUMENT) {
        errors.push_back("节点类型不是文档");
    }
    return errors;
}

} // namespace chtl