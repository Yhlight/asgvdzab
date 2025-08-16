#include "ast/TextNode.h"
#include <sstream>

namespace chtl {

TextNode::TextNode(const std::string& text_content)
    : ASTNode(ASTNodeType::TEXT_NODE), content(text_content) {
    generateNodeId();
    node_id = "text_" + node_id;
}

std::string TextNode::getContent() const {
    return content;
}

void TextNode::setContent(const std::string& text_content) {
    content = text_content;
}

std::unique_ptr<ASTNode> TextNode::clone() const {
    auto cloned_node = std::make_unique<TextNode>(content);
    cloned_node->line = line;
    cloned_node->column = column;
    cloned_node->start_position = start_position;
    cloned_node->end_position = end_position;
    cloned_node->source_file = source_file;
    return cloned_node;
}

std::string TextNode::getNodeTypeName() const {
    return "TextNode";
}

std::string TextNode::serialize() const {
    std::ostringstream oss;
    oss << "TextNode {";
    oss << " content: \"" << content << "\"";
    oss << ", line: " << line;
    oss << ", column: " << column;
    oss << " }";
    return oss.str();
}

std::vector<std::string> TextNode::validate() const {
    std::vector<std::string> errors;
    if (content.empty()) {
        errors.push_back("文本内容不能为空");
    }
    if (type != ASTNodeType::TEXT_NODE) {
        errors.push_back("节点类型不是文本节点");
    }
    return errors;
}

} // namespace chtl