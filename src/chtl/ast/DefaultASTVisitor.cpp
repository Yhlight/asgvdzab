#include "ASTVisitor.hpp"
#include "CHTLASTNodes.hpp"
#include "chtljs/ast/CHTLJSASTNodes.hpp"

namespace chtl {
namespace ast {

// 实现DefaultASTVisitor的方法
void DefaultASTVisitor::visitChildren(ASTNode& node) {
    for (auto& child : node.getChildren()) {
        if (child) {
            child->accept(*this);
        }
    }
}

// CHTL节点默认实现 - 只访问子节点
void DefaultASTVisitor::visitProgramNode(ProgramNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitElementNode(ElementNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitTextNode(TextNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitAttributeNode(AttributeNode& node) {
    if (node.getValue()) {
        node.getValue()->accept(*this);
    }
}

void DefaultASTVisitor::visitCommentNode(CommentNode& node) {
    // 注释节点通常没有子节点
}

void DefaultASTVisitor::visitStyleBlockNode(StyleBlockNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitStyleRuleNode(StyleRuleNode& node) {
    if (node.getSelector()) {
        node.getSelector()->accept(*this);
    }
    visitChildren(node);
}

void DefaultASTVisitor::visitStyleSelectorNode(StyleSelectorNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitStylePropertyNode(StylePropertyNode& node) {
    if (node.getValue()) {
        node.getValue()->accept(*this);
    }
}

void DefaultASTVisitor::visitScriptBlockNode(ScriptBlockNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitTemplateDefNode(TemplateDefNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitCustomDefNode(CustomDefNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitTemplateUseNode(TemplateUseNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitCustomUseNode(CustomUseNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitDeleteOpNode(DeleteOpNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitInsertOpNode(InsertOpNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitInheritOpNode(InheritOpNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitOriginBlockNode(OriginBlockNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitImportStmtNode(ImportStmtNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitNamespaceDefNode(NamespaceDefNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitConfigBlockNode(ConfigBlockNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitExceptConstraintNode(ExceptConstraintNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitStringLiteralNode(StringLiteralNode& node) {
    // 字面量节点通常没有子节点
}

void DefaultASTVisitor::visitNumberLiteralNode(NumberLiteralNode& node) {
    // 字面量节点通常没有子节点
}

void DefaultASTVisitor::visitUnquotedLiteralNode(UnquotedLiteralNode& node) {
    // 字面量节点通常没有子节点
}

void DefaultASTVisitor::visitIdentifierNode(IdentifierNode& node) {
    // 标识符节点通常没有子节点
}

// CHTL JS节点默认实现
void DefaultASTVisitor::visitSelectorExprNode(js::ast::SelectorExprNode& node) {
    visitChildren(node);
}

void DefaultASTVisitor::visitArrowAccessNode(js::ast::ArrowAccessNode& node) {
    if (node.getLeft()) {
        node.getLeft()->accept(*this);
    }
    if (node.getRight()) {
        node.getRight()->accept(*this);
    }
}

void DefaultASTVisitor::visitListenExprNode(js::ast::ListenExprNode& node) {
    for (const auto& [event, handler] : node.getEventHandlers()) {
        if (handler) {
            handler->accept(*this);
        }
    }
}

void DefaultASTVisitor::visitDelegateExprNode(js::ast::DelegateExprNode& node) {
    if (node.getTarget()) {
        node.getTarget()->accept(*this);
    }
    for (const auto& target : node.getTargets()) {
        if (target) {
            target->accept(*this);
        }
    }
    for (const auto& [event, handler] : node.getEventHandlers()) {
        if (handler) {
            handler->accept(*this);
        }
    }
}

void DefaultASTVisitor::visitAnimateExprNode(js::ast::AnimateExprNode& node) {
    if (node.getBegin()) {
        node.getBegin()->accept(*this);
    }
    if (node.getEnd()) {
        node.getEnd()->accept(*this);
    }
    for (const auto& [at, properties] : node.getWhenFrames()) {
        if (properties) {
            properties->accept(*this);
        }
    }
    if (node.getCallback()) {
        node.getCallback()->accept(*this);
    }
}

void DefaultASTVisitor::visitObjectLiteralNode(js::ast::ObjectLiteralNode& node) {
    for (const auto& [key, value] : node.getProperties()) {
        if (value) {
            value->accept(*this);
        }
    }
}

void DefaultASTVisitor::visitPropertyNode(js::ast::PropertyNode& node) {
    if (node.getValue()) {
        node.getValue()->accept(*this);
    }
}

void DefaultASTVisitor::visitFunctionExprNode(js::ast::FunctionExprNode& node) {
    // 函数体暂时是字符串，不需要递归访问
}

void DefaultASTVisitor::visitIdentifierRefNode(js::ast::IdentifierRefNode& node) {
    // 标识符引用没有子节点
}

void DefaultASTVisitor::visitMethodCallNode(js::ast::MethodCallNode& node) {
    for (const auto& arg : node.getArguments()) {
        if (arg) {
            arg->accept(*this);
        }
    }
}

} // namespace ast
} // namespace chtl