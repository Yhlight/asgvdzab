#include "AST/ASTNode.h"
#include <iostream>
#include <sstream>

namespace Chtl {

ASTNode::ASTNode(ASTNodeType type, const Token& token)
    : type_(type), token_(token) {
}

void ASTNode::addChild(std::shared_ptr<ASTNode> child) {
    if (child) {
        children_.push_back(child);
    }
}

std::shared_ptr<ASTNode> ASTNode::getChild(size_t index) const {
    if (index < children_.size()) {
        return children_[index];
    }
    return nullptr;
}

std::string ASTNode::toString() const {
    std::stringstream ss;
    ss << "ASTNode[type=";
    
    switch (type_) {
        case ASTNodeType::PROGRAM: ss << "PROGRAM"; break;
        case ASTNodeType::ELEMENT: ss << "ELEMENT"; break;
        case ASTNodeType::TEXT: ss << "TEXT"; break;
        case ASTNodeType::ATTRIBUTE: ss << "ATTRIBUTE"; break;
        case ASTNodeType::COMMENT: ss << "COMMENT"; break;
        case ASTNodeType::STYLE_BLOCK: ss << "STYLE_BLOCK"; break;
        case ASTNodeType::STYLE_RULE: ss << "STYLE_RULE"; break;
        case ASTNodeType::STYLE_PROPERTY: ss << "STYLE_PROPERTY"; break;
        case ASTNodeType::STYLE_SELECTOR: ss << "STYLE_SELECTOR"; break;
        case ASTNodeType::SCRIPT_BLOCK: ss << "SCRIPT_BLOCK"; break;
        case ASTNodeType::TEMPLATE_DEFINITION: ss << "TEMPLATE_DEFINITION"; break;
        case ASTNodeType::STYLE_TEMPLATE: ss << "STYLE_TEMPLATE"; break;
        case ASTNodeType::ELEMENT_TEMPLATE: ss << "ELEMENT_TEMPLATE"; break;
        case ASTNodeType::VAR_TEMPLATE: ss << "VAR_TEMPLATE"; break;
        case ASTNodeType::CUSTOM_DEFINITION: ss << "CUSTOM_DEFINITION"; break;
        case ASTNodeType::CUSTOM_STYLE: ss << "CUSTOM_STYLE"; break;
        case ASTNodeType::CUSTOM_ELEMENT: ss << "CUSTOM_ELEMENT"; break;
        case ASTNodeType::CUSTOM_VAR: ss << "CUSTOM_VAR"; break;
        case ASTNodeType::TEMPLATE_USE: ss << "TEMPLATE_USE"; break;
        case ASTNodeType::VAR_USE: ss << "VAR_USE"; break;
        case ASTNodeType::INHERIT_STATEMENT: ss << "INHERIT_STATEMENT"; break;
        case ASTNodeType::DELETE_STATEMENT: ss << "DELETE_STATEMENT"; break;
        case ASTNodeType::INSERT_STATEMENT: ss << "INSERT_STATEMENT"; break;
        case ASTNodeType::ORIGIN_BLOCK: ss << "ORIGIN_BLOCK"; break;
        case ASTNodeType::CONFIGURATION_BLOCK: ss << "CONFIGURATION_BLOCK"; break;
        case ASTNodeType::NAME_BLOCK: ss << "NAME_BLOCK"; break;
        case ASTNodeType::ORIGIN_TYPE_BLOCK: ss << "ORIGIN_TYPE_BLOCK"; break;
        case ASTNodeType::IMPORT_STATEMENT: ss << "IMPORT_STATEMENT"; break;
        case ASTNodeType::EXPORT_STATEMENT: ss << "EXPORT_STATEMENT"; break;
        case ASTNodeType::NAMESPACE_BLOCK: ss << "NAMESPACE_BLOCK"; break;
        case ASTNodeType::STRING_LITERAL: ss << "STRING_LITERAL"; break;
        case ASTNodeType::NUMBER_LITERAL: ss << "NUMBER_LITERAL"; break;
        case ASTNodeType::IDENTIFIER: ss << "IDENTIFIER"; break;
        case ASTNodeType::SELECTOR_EXPRESSION: ss << "SELECTOR_EXPRESSION"; break;
        case ASTNodeType::ARROW_ACCESS: ss << "ARROW_ACCESS"; break;
        case ASTNodeType::LISTEN_CALL: ss << "LISTEN_CALL"; break;
        case ASTNodeType::DELEGATE_CALL: ss << "DELEGATE_CALL"; break;
        case ASTNodeType::ANIMATE_CALL: ss << "ANIMATE_CALL"; break;
        case ASTNodeType::EXPRESSION: ss << "EXPRESSION"; break;
        case ASTNodeType::STATEMENT_LIST: ss << "STATEMENT_LIST"; break;
        default: ss << "UNKNOWN"; break;
    }
    
    ss << ", token=" << token_.value;
    ss << ", line=" << token_.line;
    ss << ", children=" << children_.size() << "]";
    
    return ss.str();
}

void ASTNode::dump(int indent) const {
    std::cout << getIndent(indent) << toString() << std::endl;
    
    // 递归打印子节点
    for (const auto& child : children_) {
        if (child) {
            child->dump(indent + 2);
        }
    }
}

std::string ASTNode::getIndent(int level) const {
    return std::string(level, ' ');
}

} // namespace Chtl