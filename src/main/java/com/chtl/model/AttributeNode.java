package com.chtl.model;


import com.chtl.model.CHTLNode;
/**
 * Attribute node in the model
 */
public class AttributeNode extends CHTLNode {
    private String name;
    private String value;

    public AttributeNode(String name, String value) {
        this.name = name;
        this.value = value;
    }

    public String getName() { return name; }
    public String getValue() { return value; }

    public void accept(CHTLNodeVisitor visitor) {
        visitor.visitAttribute(this);
    }
}
