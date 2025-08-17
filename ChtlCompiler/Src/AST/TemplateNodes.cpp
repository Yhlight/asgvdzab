#include "AST/TemplateNodes.h"
#include "AST/ASTVisitor.h"

namespace Chtl {

// TemplateDefinitionNode
TemplateDefinitionNode::TemplateDefinitionNode(const Token& token, TemplateType type)
    : ASTNode(ASTNodeType::TEMPLATE_DEFINITION, token),
      templateType_(type) {
}

void TemplateDefinitionNode::accept(ASTVisitor* visitor) {
    visitor->visitTemplateDefinition(this);
}

// StyleTemplateNode
StyleTemplateNode::StyleTemplateNode(const Token& token)
    : TemplateDefinitionNode(token, TemplateType::STYLE) {
    type_ = ASTNodeType::STYLE_TEMPLATE;
}

std::vector<ASTNodePtr> StyleTemplateNode::getProperties() const {
    std::vector<ASTNodePtr> props;
    for (const auto& child : children_) {
        if (child && child->getType() == ASTNodeType::STYLE_PROPERTY) {
            props.push_back(child);
        }
    }
    return props;
}

std::vector<ASTNodePtr> StyleTemplateNode::getInherits() const {
    std::vector<ASTNodePtr> inherits;
    for (const auto& child : children_) {
        if (child && (child->getType() == ASTNodeType::INHERIT_STATEMENT ||
                     child->getType() == ASTNodeType::TEMPLATE_USE)) {
            inherits.push_back(child);
        }
    }
    return inherits;
}

void StyleTemplateNode::accept(ASTVisitor* visitor) {
    visitor->visitStyleTemplate(this);
}

// ElementTemplateNode
ElementTemplateNode::ElementTemplateNode(const Token& token)
    : TemplateDefinitionNode(token, TemplateType::ELEMENT) {
    type_ = ASTNodeType::ELEMENT_TEMPLATE;
}

std::vector<ASTNodePtr> ElementTemplateNode::getElements() const {
    std::vector<ASTNodePtr> elements;
    for (const auto& child : children_) {
        if (child && child->getType() == ASTNodeType::ELEMENT) {
            elements.push_back(child);
        }
    }
    return elements;
}

void ElementTemplateNode::accept(ASTVisitor* visitor) {
    visitor->visitElementTemplate(this);
}

// VarTemplateNode
VarTemplateNode::VarTemplateNode(const Token& token)
    : TemplateDefinitionNode(token, TemplateType::VAR) {
    type_ = ASTNodeType::VAR_TEMPLATE;
}

std::vector<ASTNodePtr> VarTemplateNode::getVariables() const {
    std::vector<ASTNodePtr> vars;
    for (const auto& child : children_) {
        if (child && child->getType() == ASTNodeType::ATTRIBUTE) {
            vars.push_back(child);
        }
    }
    return vars;
}

void VarTemplateNode::accept(ASTVisitor* visitor) {
    visitor->visitVarTemplate(this);
}

// CustomDefinitionNode
CustomDefinitionNode::CustomDefinitionNode(const Token& token, CustomType type)
    : ASTNode(ASTNodeType::CUSTOM_DEFINITION, token),
      customType_(type) {
}

void CustomDefinitionNode::accept(ASTVisitor* visitor) {
    visitor->visitCustomDefinition(this);
}

// CustomStyleNode
CustomStyleNode::CustomStyleNode(const Token& token)
    : CustomDefinitionNode(token, CustomType::STYLE) {
    type_ = ASTNodeType::CUSTOM_STYLE;
}

std::vector<ASTNodePtr> CustomStyleNode::getProperties() const {
    std::vector<ASTNodePtr> props;
    for (const auto& child : children_) {
        if (child && child->getType() == ASTNodeType::STYLE_PROPERTY) {
            props.push_back(child);
        }
    }
    return props;
}

std::vector<ASTNodePtr> CustomStyleNode::getInherits() const {
    std::vector<ASTNodePtr> inherits;
    for (const auto& child : children_) {
        if (child && (child->getType() == ASTNodeType::INHERIT_STATEMENT ||
                     child->getType() == ASTNodeType::TEMPLATE_USE)) {
            inherits.push_back(child);
        }
    }
    return inherits;
}

void CustomStyleNode::accept(ASTVisitor* visitor) {
    visitor->visitCustomStyle(this);
}

// CustomElementNode
CustomElementNode::CustomElementNode(const Token& token)
    : CustomDefinitionNode(token, CustomType::ELEMENT) {
    type_ = ASTNodeType::CUSTOM_ELEMENT;
}

std::vector<ASTNodePtr> CustomElementNode::getElements() const {
    std::vector<ASTNodePtr> elements;
    for (const auto& child : children_) {
        if (child && (child->getType() == ASTNodeType::ELEMENT ||
                     child->getType() == ASTNodeType::DELETE_STATEMENT ||
                     child->getType() == ASTNodeType::INSERT_STATEMENT)) {
            elements.push_back(child);
        }
    }
    return elements;
}

void CustomElementNode::accept(ASTVisitor* visitor) {
    visitor->visitCustomElement(this);
}

// CustomVarNode
CustomVarNode::CustomVarNode(const Token& token)
    : CustomDefinitionNode(token, CustomType::VAR) {
    type_ = ASTNodeType::CUSTOM_VAR;
}

std::vector<ASTNodePtr> CustomVarNode::getVariables() const {
    std::vector<ASTNodePtr> vars;
    for (const auto& child : children_) {
        if (child && child->getType() == ASTNodeType::ATTRIBUTE) {
            vars.push_back(child);
        }
    }
    return vars;
}

void CustomVarNode::accept(ASTVisitor* visitor) {
    visitor->visitCustomVar(this);
}

// TemplateUseNode
TemplateUseNode::TemplateUseNode(const Token& token, UseType type)
    : ASTNode(ASTNodeType::TEMPLATE_USE, token),
      useType_(type) {
}

void TemplateUseNode::addParameter(const std::string& name, ASTNodePtr value) {
    parameters_[name] = value;
}

void TemplateUseNode::accept(ASTVisitor* visitor) {
    visitor->visitTemplateUse(this);
}

// VarUseNode
VarUseNode::VarUseNode(const Token& token)
    : ASTNode(ASTNodeType::VAR_USE, token) {
}

void VarUseNode::accept(ASTVisitor* visitor) {
    visitor->visitVarUse(this);
}

// InheritStatementNode
InheritStatementNode::InheritStatementNode(const Token& token)
    : ASTNode(ASTNodeType::INHERIT_STATEMENT, token) {
}

void InheritStatementNode::accept(ASTVisitor* visitor) {
    visitor->visitInheritStatement(this);
}

// DeleteStatementNode
DeleteStatementNode::DeleteStatementNode(const Token& token)
    : ASTNode(ASTNodeType::DELETE_STATEMENT, token) {
}

void DeleteStatementNode::accept(ASTVisitor* visitor) {
    visitor->visitDeleteStatement(this);
}

// InsertStatementNode
InsertStatementNode::InsertStatementNode(const Token& token)
    : ASTNode(ASTNodeType::INSERT_STATEMENT, token) {
}

void InsertStatementNode::accept(ASTVisitor* visitor) {
    visitor->visitInsertStatement(this);
}

} // namespace Chtl