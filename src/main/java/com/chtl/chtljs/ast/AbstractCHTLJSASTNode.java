package com.chtl.chtljs.ast;

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
}
