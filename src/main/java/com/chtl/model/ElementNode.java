package com.chtl.model;


import com.chtl.model.CHTLNode;
import java.util.List;
import java.util.ArrayList;

/**
 * Element node in the model
 */
public class ElementNode extends CHTLNode {
    private String tagName;
    private List<AttributeNode> attributes;
    private List<CHTLNode> children;

    public ElementNode(String tagName) {
        this.tagName = tagName;
        this.attributes = new ArrayList<>();
        this.children = new ArrayList<>();
    }

    public String getTagName() { return tagName; }
    public List<AttributeNode> getAttributes() { return attributes; }
    public List<CHTLNode> getChildren() { return children; }

    public void addAttribute(AttributeNode attr) {
        attributes.add(attr);
    }

    public void addChild(CHTLNode child) {
        children.add(child);
    }

    public void accept(CHTLNodeVisitor visitor) {
        visitor.visitElement(this);
    }
}
