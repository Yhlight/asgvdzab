#include "ast/LiteralNode.h"
#include <sstream>

namespace chtl {

LiteralNode::LiteralNode(const std::string& literal_content, ASTNodeType type)
    : ASTNode(type), content(literal_content), literal_type(type) {
    generateNodeId();
    node_id = "literal_" + node_id;
}

std::string LiteralNode::getContent() const {
    return content;
}

void LiteralNode::setContent(const std::string& literal_content) {
    content = literal_content;
}

ASTNodeType LiteralNode::getLiteralType() const {
    return literal_type;
}

void LiteralNode::setLiteralType(ASTNodeType type) {
    literal_type = type;
    this->type = type;
}

std::unique_ptr<ASTNode> LiteralNode::clone() const {
    auto cloned_node = std::make_unique<LiteralNode>(content, literal_type);
    cloned_node->line = line;
    cloned_node->column = column;
    cloned_node->start_position = start_position;
    cloned_node->end_position = end_position;
    cloned_node->source_file = source_file;
    return cloned_node;
}

std::string LiteralNode::getNodeTypeName() const {
    switch (literal_type) {
        case ASTNodeType::LITERAL_UNQUOTED:
            return "LiteralUnquoted";
        case ASTNodeType::LITERAL_DOUBLE_QUOTE:
            return "LiteralDoubleQuote";
        case ASTNodeType::LITERAL_SINGLE_QUOTE:
            return "LiteralSingleQuote";
        default:
            return "Literal";
    }
}

std::string LiteralNode::serialize() const {
    std::ostringstream oss;
    oss << "LiteralNode {";
    oss << " type: " << getNodeTypeName();
    oss << ", content: \"" << content << "\"";
    oss << ", line: " << line;
    oss << ", column: " << column;
    oss << " }";
    return oss.str();
}

std::vector<std::string> LiteralNode::validate() const {
    std::vector<std::string> errors;
    if (content.empty()) {
        errors.push_back("字面量内容不能为空");
    }
    if (type != literal_type) {
        errors.push_back("字面量类型不匹配");
    }
    return errors;
}

} // namespace chtl