#include "CHTLJSASTNodes.hpp"
#include "chtl/ast/ASTVisitor.hpp"

namespace chtl {
namespace js {
namespace ast {

// 实现所有CHTL JS节点的accept方法
void SelectorExprNode::accept(chtl::ast::ASTVisitor& visitor) {
    visitor.visitSelectorExprNode(*this);
}

void ArrowAccessNode::accept(chtl::ast::ASTVisitor& visitor) {
    visitor.visitArrowAccessNode(*this);
}

void ListenExprNode::accept(chtl::ast::ASTVisitor& visitor) {
    visitor.visitListenExprNode(*this);
}

void DelegateExprNode::accept(chtl::ast::ASTVisitor& visitor) {
    visitor.visitDelegateExprNode(*this);
}

void AnimateExprNode::accept(chtl::ast::ASTVisitor& visitor) {
    visitor.visitAnimateExprNode(*this);
}

void ObjectLiteralNode::accept(chtl::ast::ASTVisitor& visitor) {
    visitor.visitObjectLiteralNode(*this);
}

void PropertyNode::accept(chtl::ast::ASTVisitor& visitor) {
    visitor.visitPropertyNode(*this);
}

void FunctionExprNode::accept(chtl::ast::ASTVisitor& visitor) {
    visitor.visitFunctionExprNode(*this);
}

void IdentifierRefNode::accept(chtl::ast::ASTVisitor& visitor) {
    visitor.visitIdentifierRefNode(*this);
}

void MethodCallNode::accept(chtl::ast::ASTVisitor& visitor) {
    visitor.visitMethodCallNode(*this);
}

} // namespace ast
} // namespace js
} // namespace chtl