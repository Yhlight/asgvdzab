package com.chtl.model;

import java.util.ArrayList;
import java.util.List;

/**
 * CHTL语法树节点基类
 */
public abstract class CHTLNode {
    protected String name;
    protected List<CHTLNode> children;
    protected CHTLNode parent;
    protected int lineNumber;
    protected int columnNumber;
    
    public CHTLNode(String name) {
        this.name = name;
        this.children = new ArrayList<>();
    }
    
    public void addChild(CHTLNode child) {
        children.add(child);
        child.setParent(this);
    }
    
    public String getName() {
        return name;
    }
    
    public List<CHTLNode> getChildren() {
        return children;
    }
    
    public CHTLNode getParent() {
        return parent;
    }
    
    public void setParent(CHTLNode parent) {
        this.parent = parent;
    }
    
    public int getLineNumber() {
        return lineNumber;
    }
    
    public void setLineNumber(int lineNumber) {
        this.lineNumber = lineNumber;
    }
    
    public int getColumnNumber() {
        return columnNumber;
    }
    
    public void setColumnNumber(int columnNumber) {
        this.columnNumber = columnNumber;
    }
    
    public abstract void accept(CHTLNodeVisitor visitor);
}