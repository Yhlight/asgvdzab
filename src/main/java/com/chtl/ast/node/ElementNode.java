package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


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
    private Integer index; // 用于索引访问，如div[1]
    
    public ElementNode(String tagName) {
        super(NodeType.ELEMENT);
        this.tagName = tagName;
        this.attributes = new ArrayList<>();
        this.isSelfClosing = false;
        this.index = null;
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
    
    public Integer getIndex() {
        return index;
    }
    
    public void setIndex(Integer index) {
        this.index = index;
    }
    
    public boolean hasIndex() {
        return index != null;
    }
    
    /**
     * 设置属性值
     * 如果属性已存在，更新其值；否则创建新属性
     */
    public void setAttribute(String name, String value) {
        AttributeNode existing = getAttribute(name);
        if (existing != null) {
            existing.setValue(new UnquotedLiteralNode(value));
        } else {
            addAttribute(new AttributeNode(name, new UnquotedLiteralNode(value)));
        }
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitElement(this);
    }
}