package com.chtl.ast;

import com.chtl.ast.node.*;

/**
 * CHTL JS AST访问者接口
 */
public interface CHTLJSVisitor extends CHTLASTVisitor {
    // CHTL JS节点访问方法
    void visitCHTLJSSelector(CHTLJSSelectorNode node);
    void visitCHTLJSMethodCall(CHTLJSMethodCallNode node);
    void visitCHTLJSAnimate(CHTLJSAnimateNode node);
    void visitCHTLJSObject(CHTLJSObjectNode node);
    void visitCHTLJSArray(CHTLJSArrayNode node);
    void visitCHTLJSFunction(CHTLJSFunctionNode node);

    void visitCHTLJSIdentifier(CHTLJSIdentifierNode node);
}