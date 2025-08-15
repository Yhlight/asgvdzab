package com.chtl.ast.node;

import com.chtl.ast.*;
import java.util.ArrayList;
import java.util.List;

/**
 * 元素节点
 * 对应CHTL中的HTML元素，如div、span等
 */
public class ElementNode extends AbstractCHTLASTNode {
    private String tagName;
    private List<AttributeNode> attributes;
    private boolean isSelfClosing;
    
    public ElementNode(String tagName) {
        super(NodeType.ELEMENT);
        this.tagName = tagName;
        this.attributes = new ArrayList<>();
        this.isSelfClosing = false;
    }
    
    public String getTagName() {
        return tagName;
    }
    
    public void setTagName(String tagName) {
        this.tagName = tagName;
    }
    
    public List<AttributeNode> getAttributes() {
        return attributes;
    }
    
    public void addAttribute(AttributeNode attribute) {
        if (attribute != null) {
            attributes.add(attribute);
            attribute.setParent(this);
        }
    }
    
    public AttributeNode getAttribute(String name) {
        return attributes.stream()
            .filter(attr -> attr.getName().equals(name))
            .findFirst()
            .orElse(null);
    }
    
    public boolean isSelfClosing() {
        return isSelfClosing;
    }
    
    public void setSelfClosing(boolean selfClosing) {
        isSelfClosing = selfClosing;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitElement(this);
    }
}