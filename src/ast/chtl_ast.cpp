#include "ast/chtl_ast.hpp"
#include <sstream>
#include <algorithm>

namespace chtl {

// CHTLASTNode 基础实现
void CHTLASTNode::addChild(CHTLASTNodePtr child) {
    if (child) {
        child->parent = this;
        children.push_back(child);
    }
}

std::vector<CHTLASTNodePtr> CHTLASTNode::getChildrenByType(CHTLASTNodeType targetType) const {
    std::vector<CHTLASTNodePtr> result;
    for (const auto& child : children) {
        if (child && child->type == targetType) {
            result.push_back(child);
        }
    }
    return result;
}

CHTLASTNodePtr CHTLASTNode::findChild(CHTLASTNodeType targetType) const {
    for (const auto& child : children) {
        if (child && child->type == targetType) {
            return child;
        }
    }
    return nullptr;
}

bool CHTLASTNode::isTemplateNode() const {
    return type == CHTLASTNodeType::TEMPLATE_DECLARATION ||
           type == CHTLASTNodeType::TEMPLATE_STYLE ||
           type == CHTLASTNodeType::TEMPLATE_ELEMENT ||
           type == CHTLASTNodeType::TEMPLATE_VAR ||
           type == CHTLASTNodeType::TEMPLATE_USAGE;
}

bool CHTLASTNode::isCustomNode() const {
    return type == CHTLASTNodeType::CUSTOM_DECLARATION ||
           type == CHTLASTNodeType::CUSTOM_STYLE ||
           type == CHTLASTNodeType::CUSTOM_ELEMENT ||
           type == CHTLASTNodeType::CUSTOM_VAR ||
           type == CHTLASTNodeType::CUSTOM_USAGE ||
           type == CHTLASTNodeType::CUSTOM_SPECIALIZATION;
}

bool CHTLASTNode::isStyleNode() const {
    return type == CHTLASTNodeType::STYLE_BLOCK ||
           type == CHTLASTNodeType::INLINE_STYLE ||
           type == CHTLASTNodeType::CLASS_SELECTOR ||
           type == CHTLASTNodeType::ID_SELECTOR ||
           type == CHTLASTNodeType::CONTEXT_REFERENCE;
}

bool CHTLASTNode::isElementNode() const {
    return type == CHTLASTNodeType::ELEMENT_NODE;
}

bool CHTLASTNode::isCommentNode() const {
    return type == CHTLASTNodeType::COMMENT_LINE ||
           type == CHTLASTNodeType::COMMENT_MULTILINE ||
           type == CHTLASTNodeType::COMMENT_GENERATOR;
}

std::string CHTLASTNode::toString() const {
    return "CHTLASTNode(type: " + std::to_string(static_cast<int>(type)) + ")";
}

CHTLASTNodePtr CHTLASTNode::clone() const {
    return std::make_shared<CHTLASTNode>(type, position);
}

// TextNode 实现
std::string TextNode::toString() const {
    std::string quote = "";
    switch (literalType) {
        case LiteralType::DOUBLE_QUOTED: quote = "\""; break;
        case LiteralType::SINGLE_QUOTED: quote = "'"; break;
        case LiteralType::UNQUOTED: quote = ""; break;
    }
    return "TextNode(content: " + quote + content + quote + ")";
}

CHTLASTNodePtr TextNode::clone() const {
    return std::make_shared<TextNode>(content, literalType, position);
}

// ElementNode 实现
void ElementNode::addAttribute(CHTLASTNodePtr attr) {
    if (attr && attr->type == CHTLASTNodeType::ATTRIBUTE_NODE) {
        attributes.push_back(attr);
        attr->parent = this;
    }
}

void ElementNode::setStyleBlock(CHTLASTNodePtr style) {
    if (style && style->type == CHTLASTNodeType::STYLE_BLOCK) {
        styleBlock = style;
        style->parent = this;
        children.push_back(style);  // 同时添加到children中以便查找
    }
}

void ElementNode::setScriptBlock(CHTLASTNodePtr script) {
    if (script && script->type == CHTLASTNodeType::SCRIPT_BLOCK) {
        scriptBlock = script;
        script->parent = this;
        children.push_back(script);  // 同时添加到children中以便查找
    }
}

std::string ElementNode::toString() const {
    std::ostringstream oss;
    oss << "ElementNode(tag: " << tagName;
    if (!attributes.empty()) {
        oss << ", attributes: " << attributes.size();
    }
    if (styleBlock) {
        oss << ", hasStyle: true";
    }
    if (scriptBlock) {
        oss << ", hasScript: true";
    }
    oss << ")";
    return oss.str();
}

CHTLASTNodePtr ElementNode::clone() const {
    auto cloned = std::make_shared<ElementNode>(tagName, position);
    for (const auto& attr : attributes) {
        cloned->addAttribute(attr->clone());
    }
    if (styleBlock) {
        cloned->setStyleBlock(styleBlock->clone());
    }
    if (scriptBlock) {
        cloned->setScriptBlock(scriptBlock->clone());
    }
    return cloned;
}

// AttributeNode 实现
std::string AttributeNode::toString() const {
    std::string quote = "";
    switch (valueType) {
        case LiteralType::DOUBLE_QUOTED: quote = "\""; break;
        case LiteralType::SINGLE_QUOTED: quote = "'"; break;
        case LiteralType::UNQUOTED: quote = ""; break;
    }
    return "AttributeNode(" + name + ": " + quote + value + quote + ")";
}

CHTLASTNodePtr AttributeNode::clone() const {
    return std::make_shared<AttributeNode>(name, value, valueType, position);
}

// StyleBlockNode 实现
std::string StyleBlockNode::toString() const {
    return "StyleBlockNode(children: " + std::to_string(children.size()) + ")";
}

CHTLASTNodePtr StyleBlockNode::clone() const {
    auto cloned = std::make_shared<StyleBlockNode>(position);
    for (const auto& child : children) {
        cloned->addChild(child->clone());
    }
    return cloned;
}

// ClassSelectorNode 实现
std::string ClassSelectorNode::toString() const {
    return "ClassSelectorNode(class: ." + className + ", properties: " + std::to_string(properties.size()) + ")";
}

CHTLASTNodePtr ClassSelectorNode::clone() const {
    auto cloned = std::make_shared<ClassSelectorNode>(className, position);
    for (const auto& prop : properties) {
        cloned->properties.push_back(prop->clone());
    }
    return cloned;
}

// IDSelectorNode 实现
std::string IDSelectorNode::toString() const {
    return "IDSelectorNode(id: #" + idName + ", properties: " + std::to_string(properties.size()) + ")";
}

CHTLASTNodePtr IDSelectorNode::clone() const {
    auto cloned = std::make_shared<IDSelectorNode>(idName, position);
    for (const auto& prop : properties) {
        cloned->properties.push_back(prop->clone());
    }
    return cloned;
}

// InlineStyleNode 实现
std::string InlineStyleNode::toString() const {
    return "InlineStyleNode(" + property + ": " + value + ")";
}

CHTLASTNodePtr InlineStyleNode::clone() const {
    return std::make_shared<InlineStyleNode>(property, value, position);
}

// ContextReferenceNode 实现
std::string ContextReferenceNode::toString() const {
    return "ContextReferenceNode(&" + selector + ", properties: " + std::to_string(properties.size()) + ")";
}

CHTLASTNodePtr ContextReferenceNode::clone() const {
    auto cloned = std::make_shared<ContextReferenceNode>(selector, position);
    for (const auto& prop : properties) {
        cloned->properties.push_back(prop->clone());
    }
    return cloned;
}

// TemplateDeclarationNode 实现
std::string TemplateDeclarationNode::toString() const {
    return "TemplateDeclarationNode([Template] " + templateType + " " + name + ")";
}

CHTLASTNodePtr TemplateDeclarationNode::clone() const {
    auto cloned = std::make_shared<TemplateDeclarationNode>(templateType, name, position);
    for (const auto& child : children) {
        cloned->addChild(child->clone());
    }
    return cloned;
}

// TemplateUsageNode 实现
std::string TemplateUsageNode::toString() const {
    return "TemplateUsageNode(" + templateType + " " + name + ";)";
}

CHTLASTNodePtr TemplateUsageNode::clone() const {
    return std::make_shared<TemplateUsageNode>(templateType, name, position);
}

// CustomDeclarationNode 实现
std::string CustomDeclarationNode::toString() const {
    return "CustomDeclarationNode([Custom] " + customType + " " + name + ")";
}

CHTLASTNodePtr CustomDeclarationNode::clone() const {
    auto cloned = std::make_shared<CustomDeclarationNode>(customType, name, position);
    for (const auto& child : children) {
        cloned->addChild(child->clone());
    }
    return cloned;
}

// CustomSpecializationNode 实现
std::string CustomSpecializationNode::toString() const {
    return "CustomSpecializationNode(" + customName + " specialization)";
}

CHTLASTNodePtr CustomSpecializationNode::clone() const {
    auto cloned = std::make_shared<CustomSpecializationNode>(customName, position);
    for (const auto& child : children) {
        cloned->addChild(child->clone());
    }
    return cloned;
}

// DeleteOperationNode 实现
std::string DeleteOperationNode::toString() const {
    std::string result = "DeleteOperationNode(delete " + selector;
    if (index >= 0) {
        result += "[" + std::to_string(index) + "]";
    }
    result += ";)";
    return result;
}

CHTLASTNodePtr DeleteOperationNode::clone() const {
    auto cloned = std::make_shared<DeleteOperationNode>(selector, position);
    cloned->index = index;
    return cloned;
}

// InsertOperationNode 实现
std::string InsertOperationNode::toString() const {
    std::string posStr;
    switch (position) {
        case InsertPosition::AFTER: posStr = "after"; break;
        case InsertPosition::BEFORE: posStr = "before"; break;
        case InsertPosition::REPLACE: posStr = "replace"; break;
        case InsertPosition::AT_TOP: posStr = "at top"; break;
        case InsertPosition::AT_BOTTOM: posStr = "at bottom"; break;
    }
    
    std::string result = "InsertOperationNode(insert " + posStr + " " + selector;
    if (index >= 0) {
        result += "[" + std::to_string(index) + "]";
    }
    result += ")";
    return result;
}

CHTLASTNodePtr InsertOperationNode::clone() const {
    auto cloned = std::make_shared<InsertOperationNode>(this->position, selector, this->CHTLASTNode::position);
    cloned->index = index;
    for (const auto& child : children) {
        cloned->addChild(child->clone());
    }
    return cloned;
}

// IndexAccessNode 实现
std::string IndexAccessNode::toString() const {
    return "IndexAccessNode(" + element + "[" + std::to_string(index) + "])";
}

CHTLASTNodePtr IndexAccessNode::clone() const {
    return std::make_shared<IndexAccessNode>(element, index, position);
}

// InheritStatementNode 实现
std::string InheritStatementNode::toString() const {
    return "InheritStatementNode(inherit " + inheritType + " " + name + ";)";
}

CHTLASTNodePtr InheritStatementNode::clone() const {
    return std::make_shared<InheritStatementNode>(inheritType, name, position);
}

// VarGroupNode 实现
std::string VarGroupNode::toString() const {
    return "VarGroupNode(@Var " + name + ", properties: " + std::to_string(properties.size()) + ")";
}

CHTLASTNodePtr VarGroupNode::clone() const {
    auto cloned = std::make_shared<VarGroupNode>(name, position);
    for (const auto& prop : properties) {
        cloned->properties.push_back(prop->clone());
    }
    return cloned;
}

// VarUsageNode 实现
std::string VarUsageNode::toString() const {
    std::string result = "VarUsageNode(" + varName + "(" + property;
    if (hasSpecialization) {
        result += " = " + value;
    }
    result += "))";
    return result;
}

CHTLASTNodePtr VarUsageNode::clone() const {
    auto cloned = std::make_shared<VarUsageNode>(varName, property, position);
    if (hasSpecialization) {
        cloned->setSpecialization(value);
    }
    return cloned;
}

// ImportDeclarationNode 实现
std::string ImportDeclarationNode::toString() const {
    std::string result = "ImportDeclarationNode([Import] " + importType;
    if (!targetName.empty()) {
        result += " " + targetName;
    }
    result += " from " + path;
    if (!alias.empty()) {
        result += " as " + alias;
    }
    result += ")";
    return result;
}

CHTLASTNodePtr ImportDeclarationNode::clone() const {
    auto cloned = std::make_shared<ImportDeclarationNode>(importType, targetName, path, position);
    cloned->setAlias(alias);
    return cloned;
}

// NamespaceDeclarationNode 实现
std::string NamespaceDeclarationNode::toString() const {
    return "NamespaceDeclarationNode([Namespace] " + name + ")";
}

CHTLASTNodePtr NamespaceDeclarationNode::clone() const {
    auto cloned = std::make_shared<NamespaceDeclarationNode>(name, position);
    for (const auto& child : children) {
        cloned->addChild(child->clone());
    }
    return cloned;
}

// ConfigurationDeclarationNode 实现
std::string ConfigurationDeclarationNode::toString() const {
    return "ConfigurationDeclarationNode([Configuration], properties: " + std::to_string(properties.size()) + ")";
}

CHTLASTNodePtr ConfigurationDeclarationNode::clone() const {
    auto cloned = std::make_shared<ConfigurationDeclarationNode>(position);
    for (const auto& prop : properties) {
        cloned->properties.push_back(prop->clone());
    }
    return cloned;
}

// OriginDeclarationNode 实现
std::string OriginDeclarationNode::toString() const {
    return "OriginDeclarationNode([Origin] " + originType + ")";
}

CHTLASTNodePtr OriginDeclarationNode::clone() const {
    auto cloned = std::make_shared<OriginDeclarationNode>(originType, position);
    cloned->content = content;
    return cloned;
}

// ExceptStatementNode 实现
std::string ExceptStatementNode::toString() const {
    return "ExceptStatementNode(except " + constraint + ";)";
}

CHTLASTNodePtr ExceptStatementNode::clone() const {
    return std::make_shared<ExceptStatementNode>(constraint, position);
}

// ScriptBlockNode 实现
std::string ScriptBlockNode::toString() const {
    return "ScriptBlockNode(content length: " + std::to_string(content.length()) + ")";
}

CHTLASTNodePtr ScriptBlockNode::clone() const {
    auto cloned = std::make_shared<ScriptBlockNode>(position);
    cloned->content = content;
    for (const auto& child : children) {
        cloned->addChild(child->clone());
    }
    return cloned;
}

// StringLiteralNode 实现
std::string StringLiteralNode::toString() const {
    std::string quote = "";
    switch (literalType) {
        case LiteralType::DOUBLE_QUOTED: quote = "\""; break;
        case LiteralType::SINGLE_QUOTED: quote = "'"; break;
        case LiteralType::UNQUOTED: quote = ""; break;
    }
    return "StringLiteralNode(" + quote + value + quote + ")";
}

CHTLASTNodePtr StringLiteralNode::clone() const {
    return std::make_shared<StringLiteralNode>(value, literalType, position);
}

// IdentifierNode 实现
std::string IdentifierNode::toString() const {
    return "IdentifierNode(" + name + ")";
}

CHTLASTNodePtr IdentifierNode::clone() const {
    return std::make_shared<IdentifierNode>(name, position);
}

// ProgramNode 实现
std::string ProgramNode::toString() const {
    return "ProgramNode(children: " + std::to_string(children.size()) + ")";
}

CHTLASTNodePtr ProgramNode::clone() const {
    auto cloned = std::make_shared<ProgramNode>(position);
    for (const auto& child : children) {
        cloned->addChild(child->clone());
    }
    return cloned;
}

// CommentNode 实现
std::string CommentNode::toString() const {
    std::string typeStr;
    switch (commentType) {
        case CHTLASTNodeType::COMMENT_LINE: typeStr = "//"; break;
        case CHTLASTNodeType::COMMENT_MULTILINE: typeStr = "/* */"; break;
        case CHTLASTNodeType::COMMENT_GENERATOR: typeStr = "--"; break;
        default: typeStr = "unknown"; break;
    }
    return "CommentNode(" + typeStr + " " + content + ")";
}

CHTLASTNodePtr CommentNode::clone() const {
    return std::make_shared<CommentNode>(content, commentType, position);
}

} // namespace chtl