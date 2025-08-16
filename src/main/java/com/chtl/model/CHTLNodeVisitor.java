package com.chtl.model;

/**
 * Visitor interface for CHTL model nodes
 */
public interface CHTLNodeVisitor {
    void visitElement(ElementNode node);
    void visitAttribute(AttributeNode node);
    void visitText(TextNode node);
    void visitTemplate(TemplateNode node);
    void visitScript(ScriptNode node);
    void visitStyle(StyleNode node);
    void visitCustom(CustomNode node);
}
