package com.chtl.model;

import java.util.List;
import java.util.ArrayList;

public class Element {
    private String tagName;
    private List<Attribute> attributes;
    private List<Element> children;

    public Element(String tagName) {
        this.tagName = tagName;
        this.attributes = new ArrayList<Object>();
        this.children = new ArrayList<Object>();
    }

    public String getTagName() { return tagName; }
    public List<Attribute> getAttributes() { return attributes; }
    public List<Element> getChildren() { return children; }

    public void addAttribute(Attribute attr) {
        attributes.add(attr);
    }

    public void addChild(Element child) {
        children.add(child);
    }
}
