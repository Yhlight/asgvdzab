package com.chtl.model;

import java.util.HashMap;
import java.util.Map;

/**
 * 元素节点
 */
public class ElementNode extends CHTLNode {
    private Map<String, String> attributes;
    
    public ElementNode(String tagName) {
        super(tagName);
        this.attributes = new HashMap<>();
    }
    
    public void setAttribute(String name, String value) {
        attributes.put(name, value);
    }
    
    public String getAttribute(String name) {
        return attributes.get(name);
    }
    
    public Map<String, String> getAttributes() {
        return attributes;
    }
    
    @Override
    public void accept(CHTLNodeVisitor visitor) {
        visitor.visit(this);
    }
}