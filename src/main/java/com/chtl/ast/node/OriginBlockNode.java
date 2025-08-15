package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class OriginBlockNode extends AbstractCHTLASTNode {
    private String targetType;
    private String content;

    public OriginBlockNode(String targetType, String content) {
        super(NodeType.ORIGIN_BLOCK);
        this.targetType = targetType;
        this.content = content;
    }

    public String getTargetType() { return targetType; }
    public String getContent() { return content; }

}