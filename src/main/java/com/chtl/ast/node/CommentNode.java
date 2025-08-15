package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class CommentNode extends AbstractCHTLASTNode {
    private String content;

    public CommentNode(String content) {
        super(NodeType.COMMENT);
        this.content = content;
    }

    public String getContent() { return content; }

}