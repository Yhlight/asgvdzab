#include "../../include/common/AST.h"
#include "../../include/common/Utils.h"
#include <sstream>

namespace CHTL {

void ASTNode::addChild(ASTNodePtr child) {
    if (child) {
        child->parent = shared_from_this();
        children.push_back(child);
    }
}

void ASTNode::addAttribute(const std::string& key, const std::string& value) {
    attributes[key] = value;
}

ASTNodePtr ASTNode::findChild(ASTNodeType type, const std::string& name) const {
    for (const auto& child : children) {
        if (child->type == type && (name.empty() || child->name == name)) {
            return child;
        }
    }
    return nullptr;
}

std::vector<ASTNodePtr> ASTNode::findChildren(ASTNodeType type) const {
    std::vector<ASTNodePtr> result;
    for (const auto& child : children) {
        if (child->type == type) {
            result.push_back(child);
        }
    }
    return result;
}

std::string ASTNode::getAttribute(const std::string& key) const {
    auto it = attributes.find(key);
    return (it != attributes.end()) ? it->second : "";
}

bool ASTNode::hasAttribute(const std::string& key) const {
    return attributes.find(key) != attributes.end();
}

void ASTNode::setPosition(size_t sl, size_t el, size_t sc, size_t ec) {
    start_line = sl;
    end_line = el;
    start_column = sc;
    end_column = ec;
}

std::string ASTNode::toString(int indent) const {
    std::stringstream ss;
    std::string indentStr(indent * 2, ' ');
    
    ss << indentStr << getTypeString();
    if (!name.empty()) {
        ss << " (" << name << ")";
    }
    if (!value.empty()) {
        ss << " = \"" << value << "\"";
    }
    
    // 输出属性
    if (!attributes.empty()) {
        ss << " [";
        bool first = true;
        for (const auto& attr : attributes) {
            if (!first) ss << ", ";
            ss << attr.first << "=" << attr.second;
            first = false;
        }
        ss << "]";
    }
    
    ss << "\n";
    
    // 递归输出子节点
    for (const auto& child : children) {
        ss << child->toString(indent + 1);
    }
    
    return ss.str();
}

std::string ASTNode::getTypeString() const {
    switch (type) {
        case ASTNodeType::ROOT: return "ROOT";
        case ASTNodeType::ELEMENT: return "ELEMENT";
        case ASTNodeType::TEXT: return "TEXT";
        case ASTNodeType::ATTRIBUTE: return "ATTRIBUTE";
        
        case ASTNodeType::STYLE_BLOCK: return "STYLE_BLOCK";
        case ASTNodeType::CSS_RULE: return "CSS_RULE";
        case ASTNodeType::CSS_PROPERTY: return "CSS_PROPERTY";
        case ASTNodeType::CSS_SELECTOR: return "CSS_SELECTOR";
        
        case ASTNodeType::SCRIPT_BLOCK: return "SCRIPT_BLOCK";
        case ASTNodeType::JS_STATEMENT: return "JS_STATEMENT";
        case ASTNodeType::JS_EXPRESSION: return "JS_EXPRESSION";
        
        case ASTNodeType::TEMPLATE_STYLE: return "TEMPLATE_STYLE";
        case ASTNodeType::TEMPLATE_ELEMENT: return "TEMPLATE_ELEMENT";
        case ASTNodeType::TEMPLATE_VAR: return "TEMPLATE_VAR";
        
        case ASTNodeType::CUSTOM_STYLE: return "CUSTOM_STYLE";
        case ASTNodeType::CUSTOM_ELEMENT: return "CUSTOM_ELEMENT";
        case ASTNodeType::CUSTOM_VAR: return "CUSTOM_VAR";
        
        case ASTNodeType::ORIGIN_HTML: return "ORIGIN_HTML";
        case ASTNodeType::ORIGIN_CSS: return "ORIGIN_CSS";
        case ASTNodeType::ORIGIN_JS: return "ORIGIN_JS";
        case ASTNodeType::ORIGIN_CUSTOM: return "ORIGIN_CUSTOM";
        
        case ASTNodeType::IMPORT: return "IMPORT";
        case ASTNodeType::NAMESPACE: return "NAMESPACE";
        case ASTNodeType::CONFIGURATION: return "CONFIGURATION";
        
        case ASTNodeType::INHERIT: return "INHERIT";
        case ASTNodeType::DELETE: return "DELETE";
        case ASTNodeType::INSERT: return "INSERT";
        
        case ASTNodeType::VIR_OBJECT: return "VIR_OBJECT";
        case ASTNodeType::ENHANCED_SELECTOR: return "ENHANCED_SELECTOR";
        case ASTNodeType::LISTEN_BLOCK: return "LISTEN_BLOCK";
        case ASTNodeType::DELEGATE_BLOCK: return "DELEGATE_BLOCK";
        case ASTNodeType::ANIMATE_BLOCK: return "ANIMATE_BLOCK";
        
        default: return "UNKNOWN";
    }
}

} // namespace CHTL