#pragma once

#include "ASTNode.hpp"

namespace chtl {

// CHTL JS节点需要在chtl命名空间下声明
namespace js {
namespace ast {
class SelectorExprNode;
class ArrowAccessNode;
class ListenExprNode;
class DelegateExprNode;
class AnimateExprNode;
class ObjectLiteralNode;
class PropertyNode;
class FunctionExprNode;
class IdentifierRefNode;
class MethodCallNode;
}
}

namespace ast {

// 前向声明所有节点类型
class ProgramNode;
class ElementNode;
class TextNode;
class AttributeNode;
class CommentNode;
class StyleBlockNode;
class StyleRuleNode;
class StyleSelectorNode;
class StylePropertyNode;
class ScriptBlockNode;
class TemplateDefNode;
class CustomDefNode;
class TemplateUseNode;
class CustomUseNode;
class DeleteOpNode;
class InsertOpNode;
class InheritOpNode;
class OriginBlockNode;
class ImportStmtNode;
class NamespaceDefNode;
class ConfigBlockNode;
class ExceptConstraintNode;
class StringLiteralNode;
class NumberLiteralNode;
class UnquotedLiteralNode;
class IdentifierNode;

/**
 * AST访问者接口
 * 使用访问者模式遍历AST
 */
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    // CHTL节点访问方法
    virtual void visitProgramNode(ProgramNode& node) = 0;
    virtual void visitElementNode(ElementNode& node) = 0;
    virtual void visitTextNode(TextNode& node) = 0;
    virtual void visitAttributeNode(AttributeNode& node) = 0;
    virtual void visitCommentNode(CommentNode& node) = 0;
    virtual void visitStyleBlockNode(StyleBlockNode& node) = 0;
    virtual void visitStyleRuleNode(StyleRuleNode& node) = 0;
    virtual void visitStyleSelectorNode(StyleSelectorNode& node) = 0;
    virtual void visitStylePropertyNode(StylePropertyNode& node) = 0;
    virtual void visitScriptBlockNode(ScriptBlockNode& node) = 0;
    virtual void visitTemplateDefNode(TemplateDefNode& node) = 0;
    virtual void visitCustomDefNode(CustomDefNode& node) = 0;
    virtual void visitTemplateUseNode(TemplateUseNode& node) = 0;
    virtual void visitCustomUseNode(CustomUseNode& node) = 0;
    virtual void visitDeleteOpNode(DeleteOpNode& node) = 0;
    virtual void visitInsertOpNode(InsertOpNode& node) = 0;
    virtual void visitInheritOpNode(InheritOpNode& node) = 0;
    virtual void visitOriginBlockNode(OriginBlockNode& node) = 0;
    virtual void visitImportStmtNode(ImportStmtNode& node) = 0;
    virtual void visitNamespaceDefNode(NamespaceDefNode& node) = 0;
    virtual void visitConfigBlockNode(ConfigBlockNode& node) = 0;
    virtual void visitExceptConstraintNode(ExceptConstraintNode& node) = 0;
    virtual void visitStringLiteralNode(StringLiteralNode& node) = 0;
    virtual void visitNumberLiteralNode(NumberLiteralNode& node) = 0;
    virtual void visitUnquotedLiteralNode(UnquotedLiteralNode& node) = 0;
    virtual void visitIdentifierNode(IdentifierNode& node) = 0;
    
    // CHTL JS节点访问方法
    virtual void visitSelectorExprNode(chtl::js::ast::SelectorExprNode& node) = 0;
    virtual void visitArrowAccessNode(chtl::js::ast::ArrowAccessNode& node) = 0;
    virtual void visitListenExprNode(chtl::js::ast::ListenExprNode& node) = 0;
    virtual void visitDelegateExprNode(chtl::js::ast::DelegateExprNode& node) = 0;
    virtual void visitAnimateExprNode(chtl::js::ast::AnimateExprNode& node) = 0;
    virtual void visitObjectLiteralNode(chtl::js::ast::ObjectLiteralNode& node) = 0;
    virtual void visitPropertyNode(chtl::js::ast::PropertyNode& node) = 0;
    virtual void visitFunctionExprNode(chtl::js::ast::FunctionExprNode& node) = 0;
    virtual void visitIdentifierRefNode(chtl::js::ast::IdentifierRefNode& node) = 0;
    virtual void visitMethodCallNode(chtl::js::ast::MethodCallNode& node) = 0;
};

/**
 * 默认访问者实现
 * 提供空的默认实现，子类可以选择性地覆盖需要的方法
 */
class DefaultASTVisitor : public ASTVisitor {
public:
    // CHTL节点默认实现
    void visitProgramNode(ProgramNode& node) override;
    void visitElementNode(ElementNode& node) override;
    void visitTextNode(TextNode& node) override;
    void visitAttributeNode(AttributeNode& node) override;
    void visitCommentNode(CommentNode& node) override;
    void visitStyleBlockNode(StyleBlockNode& node) override;
    void visitStyleRuleNode(StyleRuleNode& node) override;
    void visitStyleSelectorNode(StyleSelectorNode& node) override;
    void visitStylePropertyNode(StylePropertyNode& node) override;
    void visitScriptBlockNode(ScriptBlockNode& node) override;
    void visitTemplateDefNode(TemplateDefNode& node) override;
    void visitCustomDefNode(CustomDefNode& node) override;
    void visitTemplateUseNode(TemplateUseNode& node) override;
    void visitCustomUseNode(CustomUseNode& node) override;
    void visitDeleteOpNode(DeleteOpNode& node) override;
    void visitInsertOpNode(InsertOpNode& node) override;
    void visitInheritOpNode(InheritOpNode& node) override;
    void visitOriginBlockNode(OriginBlockNode& node) override;
    void visitImportStmtNode(ImportStmtNode& node) override;
    void visitNamespaceDefNode(NamespaceDefNode& node) override;
    void visitConfigBlockNode(ConfigBlockNode& node) override;
    void visitExceptConstraintNode(ExceptConstraintNode& node) override;
    void visitStringLiteralNode(StringLiteralNode& node) override;
    void visitNumberLiteralNode(NumberLiteralNode& node) override;
    void visitUnquotedLiteralNode(UnquotedLiteralNode& node) override;
    void visitIdentifierNode(IdentifierNode& node) override;
    
    // CHTL JS节点默认实现
    void visitSelectorExprNode(chtl::js::ast::SelectorExprNode& node) override;
    void visitArrowAccessNode(chtl::js::ast::ArrowAccessNode& node) override;
    void visitListenExprNode(chtl::js::ast::ListenExprNode& node) override;
    void visitDelegateExprNode(chtl::js::ast::DelegateExprNode& node) override;
    void visitAnimateExprNode(chtl::js::ast::AnimateExprNode& node) override;
    void visitObjectLiteralNode(chtl::js::ast::ObjectLiteralNode& node) override;
    void visitPropertyNode(chtl::js::ast::PropertyNode& node) override;
    void visitFunctionExprNode(chtl::js::ast::FunctionExprNode& node) override;
    void visitIdentifierRefNode(chtl::js::ast::IdentifierRefNode& node) override;
    void visitMethodCallNode(chtl::js::ast::MethodCallNode& node) override;
    
protected:
    // 辅助方法：访问所有子节点
    void visitChildren(ASTNode& node);
};

} // namespace ast
} // namespace chtl