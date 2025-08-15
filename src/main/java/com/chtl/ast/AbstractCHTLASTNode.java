package com.chtl.ast;

import java.util.ArrayList;
import java.util.List;
import java.io.Serializable;

/**
 * CHTL AST节点抽象基类
 */
public abstract class AbstractCHTLASTNode implements CHTLASTNode, Serializable {
    private static final long serialVersionUID = 1L;
    protected NodeType nodeType;
    protected CHTLASTNode parent;
    protected List<CHTLASTNode> children;
    protected SourceLocation location;
    
    public AbstractCHTLASTNode(NodeType nodeType) {
        this.nodeType = nodeType;
        this.children = new ArrayList<>();
    }
    
    @Override
    public NodeType getNodeType() {
        return nodeType;
    }
    
    @Override
    public List<CHTLASTNode> getChildren() {
        return children;
    }
    
    @Override
    public void addChild(CHTLASTNode child) {
        if (child != null) {
            children.add(child);
            child.setParent(this);
        }
    }
    
    @Override
    public CHTLASTNode getParent() {
        return parent;
    }
    
    @Override
    public void setParent(CHTLASTNode parent) {
        this.parent = parent;
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