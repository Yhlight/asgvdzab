package com.chtl.ast.chtljs;

/**
 * CHTL JS AST访问者接口
 */
public interface CHTLJSASTVisitor {
    // 选择器
    void visitEnhancedSelector(EnhancedSelectorNode node);
    
    // 方法调用
    void visitListenCall(ListenCallNode node);
    void visitDelegateCall(DelegateCallNode node);
    void visitAnimateCall(AnimateCallNode node);
    
    // 事件处理器
    void visitEventHandler(EventHandlerNode node);
    
    // 动画配置
    void visitAnimationConfig(AnimationConfigNode node);
    void visitAnimationKeyframe(AnimationKeyframeNode node);
    void visitAnimationController(AnimationControllerNode node);
    
    // 操作和表达式
    void visitArrowOperation(ArrowOperationNode node);
    void visitPropertyAccess(PropertyAccessNode node);
    void visitExpression(ExpressionNode node);
}