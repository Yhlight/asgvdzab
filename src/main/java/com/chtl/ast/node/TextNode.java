package com.chtl.ast.node;import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;
import com.chtl.model.TextNode;


import com.chtl.ast.*;

/**
 * 文本节点
 * 对应CHTL中的text { }块
 */
public class TextNode extends AbstractCHTLASTNode {
    private String content;
    
    public TextNode() {
        super(NodeType.TEXT);
        this.content = "";
    }
    
    public TextNode(String content) {
        super(NodeType.TEXT);
        this.content = content;
    }
    
    public String getContent() {
        return content;
    }
    
    public void setContent(String content) {
        this.content = content;
    }
    
    public void appendContent(String text) {
        this.content += text;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitText(this);
    }
}