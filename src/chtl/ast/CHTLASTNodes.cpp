#include "CHTLASTNodes.hpp"
#include "ASTVisitor.hpp"

namespace chtl {
namespace ast {

// 实现所有节点的accept方法
void ProgramNode::accept(ASTVisitor& visitor) {
    visitor.visitProgramNode(*this);
}

void ElementNode::accept(ASTVisitor& visitor) {
    visitor.visitElementNode(*this);
}

void TextNode::accept(ASTVisitor& visitor) {
    visitor.visitTextNode(*this);
}

void AttributeNode::accept(ASTVisitor& visitor) {
    visitor.visitAttributeNode(*this);
}

void CommentNode::accept(ASTVisitor& visitor) {
    visitor.visitCommentNode(*this);
}

void StyleBlockNode::accept(ASTVisitor& visitor) {
    visitor.visitStyleBlockNode(*this);
}

void StyleRuleNode::accept(ASTVisitor& visitor) {
    visitor.visitStyleRuleNode(*this);
}

void StyleSelectorNode::accept(ASTVisitor& visitor) {
    visitor.visitStyleSelectorNode(*this);
}

void StylePropertyNode::accept(ASTVisitor& visitor) {
    visitor.visitStylePropertyNode(*this);
}

void ScriptBlockNode::accept(ASTVisitor& visitor) {
    visitor.visitScriptBlockNode(*this);
}

void TemplateDefNode::accept(ASTVisitor& visitor) {
    visitor.visitTemplateDefNode(*this);
}

void CustomDefNode::accept(ASTVisitor& visitor) {
    visitor.visitCustomDefNode(*this);
}

void TemplateUseNode::accept(ASTVisitor& visitor) {
    visitor.visitTemplateUseNode(*this);
}

void CustomUseNode::accept(ASTVisitor& visitor) {
    visitor.visitCustomUseNode(*this);
}

void DeleteOpNode::accept(ASTVisitor& visitor) {
    visitor.visitDeleteOpNode(*this);
}

void InsertOpNode::accept(ASTVisitor& visitor) {
    visitor.visitInsertOpNode(*this);
}

void InheritOpNode::accept(ASTVisitor& visitor) {
    visitor.visitInheritOpNode(*this);
}

void OriginBlockNode::accept(ASTVisitor& visitor) {
    visitor.visitOriginBlockNode(*this);
}

void ImportStmtNode::accept(ASTVisitor& visitor) {
    visitor.visitImportStmtNode(*this);
}

void NamespaceDefNode::accept(ASTVisitor& visitor) {
    visitor.visitNamespaceDefNode(*this);
}

void ConfigBlockNode::accept(ASTVisitor& visitor) {
    visitor.visitConfigBlockNode(*this);
}

void ExceptConstraintNode::accept(ASTVisitor& visitor) {
    visitor.visitExceptConstraintNode(*this);
}

void StringLiteralNode::accept(ASTVisitor& visitor) {
    visitor.visitStringLiteralNode(*this);
}

void NumberLiteralNode::accept(ASTVisitor& visitor) {
    visitor.visitNumberLiteralNode(*this);
}

void UnquotedLiteralNode::accept(ASTVisitor& visitor) {
    visitor.visitUnquotedLiteralNode(*this);
}

void IdentifierNode::accept(ASTVisitor& visitor) {
    visitor.visitIdentifierNode(*this);
}

} // namespace ast
} // namespace chtl