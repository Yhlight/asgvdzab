package com.chtl.model;

/**
 * Visitor interface for CHTLNode hierarchy.
 * Implements the visitor pattern for traversing CHTL nodes.
 */
public interface CHTLNodeVisitor {
    void visitTemplateNode(TemplateNode node);
    void visitScriptNode(ScriptNode node);
    void visitStyleNode(StyleNode node);
    void visitCustomNode(CustomNode node);
    void visitElementNode(ElementNode node);
    void visitTextNode(TextNode node);
    void visitAttributeNode(AttributeNode node);
}