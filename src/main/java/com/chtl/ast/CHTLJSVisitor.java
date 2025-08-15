package com.chtl.ast;import com.chtl.ast.node.CHTLJSAnimateNode;
import com.chtl.ast.node.CHTLJSArrayNode;
import com.chtl.ast.node.CHTLJSFunctionNode;
import com.chtl.ast.node.CHTLJSIdentifierNode;
import com.chtl.ast.node.CHTLJSMethodCallNode;
import com.chtl.ast.node.CHTLJSObjectNode;
import com.chtl.ast.node.CHTLJSSelectorNode;


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