package com.chtl.model;

/**
 * 属性节点
 */
public class AttributeNode extends CHTLNode {
    private String value;
    
    public AttributeNode(String name, String value) {
        super(name);
        this.value = value;
    }
    
    public String getValue() {
        return value;
    }
    
    public void setValue(String value) {
        this.value = value;
    }
    
    @Override
    public void accept(CHTLNodeVisitor visitor) {
        visitor.visit(this);
    }
}