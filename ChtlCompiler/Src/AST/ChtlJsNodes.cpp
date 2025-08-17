#include "AST/ChtlJsNodes.h"
#include "AST/ASTVisitor.h"

namespace Chtl {

// SelectorExpressionNode 实现
SelectorExpressionNode::SelectorExpressionNode(const Token& token)
    : ASTNode(ASTNodeType::SELECTOR_EXPRESSION, token),
      selectorType_(SelectorType::TAG) {
}

void SelectorExpressionNode::accept(ASTVisitor* visitor) {
    visitor->visitSelectorExpression(this);
}

// ArrowAccessNode 实现
ArrowAccessNode::ArrowAccessNode(const Token& token)
    : ASTNode(ASTNodeType::ARROW_ACCESS, token) {
}

void ArrowAccessNode::accept(ASTVisitor* visitor) {
    visitor->visitArrowAccess(this);
}

// ListenCallNode 实现
ListenCallNode::ListenCallNode(const Token& token)
    : ASTNode(ASTNodeType::LISTEN_CALL, token) {
}

void ListenCallNode::accept(ASTVisitor* visitor) {
    visitor->visitListenCall(this);
}

// DelegateCallNode 实现
DelegateCallNode::DelegateCallNode(const Token& token)
    : ASTNode(ASTNodeType::DELEGATE_CALL, token) {
}

void DelegateCallNode::accept(ASTVisitor* visitor) {
    visitor->visitDelegateCall(this);
}

// AnimateCallNode 实现
AnimateCallNode::AnimateCallNode(const Token& token)
    : ASTNode(ASTNodeType::ANIMATE_CALL, token) {
}

void AnimateCallNode::accept(ASTVisitor* visitor) {
    visitor->visitAnimateCall(this);
}

// ChtlJsExpressionNode 实现
ChtlJsExpressionNode::ChtlJsExpressionNode(const Token& token)
    : ASTNode(ASTNodeType::EXPRESSION, token) {
}

void ChtlJsExpressionNode::accept(ASTVisitor* visitor) {
    visitor->visitChtlJsExpression(this);
}

// VirtualObjectNode 实现
VirtualObjectNode::VirtualObjectNode(const Token& token)
    : ASTNode(ASTNodeType::VIRTUAL_OBJECT, token) {
}

void VirtualObjectNode::accept(ASTVisitor* visitor) {
    visitor->visitVirtualObject(this);
}

// VirtualCallNode 实现
VirtualCallNode::VirtualCallNode(const Token& token)
    : ASTNode(ASTNodeType::VIRTUAL_CALL, token) {
}

void VirtualCallNode::accept(ASTVisitor* visitor) {
    visitor->visitVirtualCall(this);
}

} // namespace Chtl