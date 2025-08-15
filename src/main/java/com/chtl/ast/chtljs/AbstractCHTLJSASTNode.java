package com.chtl.ast.chtljs;
import com.chtl.model.SourceLocation;import com.chtl.ast.NodeType;


import com.chtl.ast.SourceLocation;

/**
 * CHTL JS AST节点抽象基类
 */
public abstract class AbstractCHTLJSASTNode implements CHTLJSASTNode {
    protected NodeType nodeType;
    protected SourceLocation location;
    
    public AbstractCHTLJSASTNode(NodeType nodeType) {
        this.nodeType = nodeType;
    }
    
    @Override
    public NodeType getNodeType() {
        return nodeType;
    }
    
    @Override
    public SourceLocation getLocation() {
        return location;
    }
    
    @Override
    public void setLocation(SourceLocation location) {
        this.location = location;
    }
}