#include "ast/CommentNode.h"
#include <sstream>

namespace chtl {

CommentNode::CommentNode(const std::string& comment_content, ASTNodeType type)
    : ASTNode(type), content(comment_content), comment_type(type) {
    generateNodeId();
    node_id = "comment_" + node_id;
}

std::string CommentNode::getContent() const {
    return content;
}

void CommentNode::setContent(const std::string& comment_content) {
    content = comment_content;
}

ASTNodeType CommentNode::getCommentType() const {
    return comment_type;
}

void CommentNode::setCommentType(ASTNodeType type) {
    comment_type = type;
    this->type = type;
}

std::unique_ptr<ASTNode> CommentNode::clone() const {
    auto cloned_node = std::make_unique<CommentNode>(content, comment_type);
    cloned_node->line = line;
    cloned_node->column = column;
    cloned_node->start_position = start_position;
    cloned_node->end_position = end_position;
    cloned_node->source_file = source_file;
    return cloned_node;
}

std::string CommentNode::getNodeTypeName() const {
    switch (comment_type) {
        case ASTNodeType::COMMENT_SINGLE_LINE:
            return "CommentSingleLine";
        case ASTNodeType::COMMENT_MULTI_LINE:
            return "CommentMultiLine";
        case ASTNodeType::COMMENT_GENERATOR:
            return "CommentGenerator";
        default:
            return "Comment";
    }
}

std::string CommentNode::serialize() const {
    std::ostringstream oss;
    oss << "CommentNode {";
    oss << " type: " << getNodeTypeName();
    oss << ", content: \"" << content << "\"";
    oss << ", line: " << line;
    oss << ", column: " << column;
    oss << " }";
    return oss.str();
}

std::vector<std::string> CommentNode::validate() const {
    std::vector<std::string> errors;
    if (content.empty()) {
        errors.push_back("注释内容不能为空");
    }
    if (type != comment_type) {
        errors.push_back("注释类型不匹配");
    }
    return errors;
}

} // namespace chtl