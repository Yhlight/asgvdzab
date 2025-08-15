package com.chtl.model;

/**
 * 文本节点
 */
public class TextNode extends CHTLNode {
    private String content;
    
    public TextNode(String content) {
        super("text");
        this.content = content;
    }
    
    public String getContent() {
        return content;
    }
    
    public void setContent(String content) {
        this.content = content;
    }
    
    @Override
    public void accept(CHTLNodeVisitor visitor) {
        visitor.visit(this);
    }
}