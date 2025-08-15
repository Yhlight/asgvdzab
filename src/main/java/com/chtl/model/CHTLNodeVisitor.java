package com.chtl.model;

/**
 * CHTL节点访问者接口
 */
public interface CHTLNodeVisitor {
    void visit(ElementNode node);
    void visit(TextNode node);
    void visit(AttributeNode node);
    void visit(StyleNode node);
    void visit(ScriptNode node);
    void visit(TemplateNode node);
    void visit(CustomNode node);
}