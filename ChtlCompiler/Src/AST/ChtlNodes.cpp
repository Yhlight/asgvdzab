#include "AST/ChtlNodes.h"
#include "AST/ASTVisitor.h"

namespace Chtl {

// ProgramNode
ProgramNode::ProgramNode() 
    : ASTNode(ASTNodeType::PROGRAM, Token()) {
}

void ProgramNode::accept(ASTVisitor* visitor) {
    visitor->visitProgram(this);
}

// ElementNode
ElementNode::ElementNode(const Token& tagName) 
    : ASTNode(ASTNodeType::ELEMENT, tagName),
      tagName_(tagName.value) {
}

std::vector<ASTNodePtr> ElementNode::getAttributes() const {
    std::vector<ASTNodePtr> attrs;
    for (const auto& child : children_) {
        if (child && child->getType() == ASTNodeType::ATTRIBUTE) {
            attrs.push_back(child);
        }
    }
    return attrs;
}

std::vector<ASTNodePtr> ElementNode::getStyleBlocks() const {
    std::vector<ASTNodePtr> styles;
    for (const auto& child : children_) {
        if (child && child->getType() == ASTNodeType::STYLE_BLOCK) {
            styles.push_back(child);
        }
    }
    return styles;
}

std::vector<ASTNodePtr> ElementNode::getScriptBlocks() const {
    std::vector<ASTNodePtr> scripts;
    for (const auto& child : children_) {
        if (child && child->getType() == ASTNodeType::SCRIPT_BLOCK) {
            scripts.push_back(child);
        }
    }
    return scripts;
}

void ElementNode::accept(ASTVisitor* visitor) {
    visitor->visitElement(this);
}

// TextNode
TextNode::TextNode(const Token& token)
    : ASTNode(ASTNodeType::TEXT, token) {
}

void TextNode::accept(ASTVisitor* visitor) {
    visitor->visitText(this);
}

// AttributeNode
AttributeNode::AttributeNode(const Token& name)
    : ASTNode(ASTNodeType::ATTRIBUTE, name),
      name_(name.value) {
}

void AttributeNode::accept(ASTVisitor* visitor) {
    visitor->visitAttribute(this);
}

// StyleBlockNode
StyleBlockNode::StyleBlockNode(const Token& token)
    : ASTNode(ASTNodeType::STYLE_BLOCK, token) {
}

std::vector<ASTNodePtr> StyleBlockNode::getInlineStyles() const {
    std::vector<ASTNodePtr> inlines;
    for (const auto& child : children_) {
        if (child && child->getType() == ASTNodeType::STYLE_PROPERTY) {
            inlines.push_back(child);
        }
    }
    return inlines;
}

std::vector<ASTNodePtr> StyleBlockNode::getCSSRules() const {
    std::vector<ASTNodePtr> rules;
    for (const auto& child : children_) {
        if (child && child->getType() == ASTNodeType::STYLE_RULE) {
            rules.push_back(child);
        }
    }
    return rules;
}

void StyleBlockNode::accept(ASTVisitor* visitor) {
    visitor->visitStyleBlock(this);
}

// StyleRuleNode
StyleRuleNode::StyleRuleNode(const Token& token)
    : ASTNode(ASTNodeType::STYLE_RULE, token) {
}

void StyleRuleNode::accept(ASTVisitor* visitor) {
    visitor->visitStyleRule(this);
}

// StyleSelectorNode
StyleSelectorNode::StyleSelectorNode(const Token& token, SelectorType type)
    : ASTNode(ASTNodeType::STYLE_SELECTOR, token),
      selectorType_(type) {
}

void StyleSelectorNode::accept(ASTVisitor* visitor) {
    visitor->visitStyleSelector(this);
}

// StylePropertyNode
StylePropertyNode::StylePropertyNode(const Token& token)
    : ASTNode(ASTNodeType::STYLE_PROPERTY, token) {
}

void StylePropertyNode::accept(ASTVisitor* visitor) {
    visitor->visitStyleProperty(this);
}

// ScriptBlockNode
ScriptBlockNode::ScriptBlockNode(const Token& token)
    : ASTNode(ASTNodeType::SCRIPT_BLOCK, token) {
}

void ScriptBlockNode::accept(ASTVisitor* visitor) {
    visitor->visitScriptBlock(this);
}

// CommentNode
CommentNode::CommentNode(const Token& token, CommentType type)
    : ASTNode(ASTNodeType::COMMENT, token),
      commentType_(type) {
}

void CommentNode::accept(ASTVisitor* visitor) {
    visitor->visitComment(this);
}

// LiteralNode
LiteralNode::LiteralNode(ASTNodeType type, const Token& token)
    : ASTNode(type, token),
      value_(token.value) {
}

// StringLiteralNode
StringLiteralNode::StringLiteralNode(const Token& token, QuoteType quoteType)
    : LiteralNode(ASTNodeType::STRING_LITERAL, token),
      quoteType_(quoteType) {
}

void StringLiteralNode::accept(ASTVisitor* visitor) {
    visitor->visitStringLiteral(this);
}

// NumberLiteralNode
NumberLiteralNode::NumberLiteralNode(const Token& token)
    : LiteralNode(ASTNodeType::NUMBER_LITERAL, token) {
}

double NumberLiteralNode::getNumericValue() const {
    try {
        return std::stod(value_);
    } catch (...) {
        return 0.0;
    }
}

void NumberLiteralNode::accept(ASTVisitor* visitor) {
    visitor->visitNumberLiteral(this);
}

// IdentifierNode
IdentifierNode::IdentifierNode(const Token& token)
    : ASTNode(ASTNodeType::IDENTIFIER, token),
      name_(token.value) {
}

void IdentifierNode::accept(ASTVisitor* visitor) {
    visitor->visitIdentifier(this);
}

} // namespace Chtl