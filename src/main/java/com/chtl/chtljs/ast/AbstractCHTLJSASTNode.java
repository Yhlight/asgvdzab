package com.chtl.chtljs.ast;

import com.chtl.ast.NodeType;
import com.chtl.ast.chtljs.AbstractCHTLJSASTNode;
import com.chtl.ast.chtljs.CHTLJSASTNode;
import com.chtl.ast.chtljs.CHTLJSASTVisitor;


public abstract class AbstractCHTLJSASTNode implements CHTLJSASTNode {
    protected CHTLJSASTNode.NodeType nodeType;

    public AbstractCHTLJSASTNode() {
        this.nodeType = CHTLJSASTNode.NodeType.IDENTIFIER;
    }

    public AbstractCHTLJSASTNode(CHTLJSASTNode.NodeType type) {
        this.nodeType = type;
    }

    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visit(this);
    }

    @Override
    public CHTLJSASTNode.NodeType getNodeType() {
        return nodeType;
    }

    @Override
    public void getLocation() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void setLocation() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }
}
