package com.chtl.model;

/**
 * Text node in the model
 */
public class TextNode extends CHTLNode {
    private String content;

    public TextNode(String content) {
        this.content = content;
    }

    public String getContent() {
        return content;
    }

    public void accept(CHTLNodeVisitor visitor) {
        visitor.visitText(this);
    }
}
