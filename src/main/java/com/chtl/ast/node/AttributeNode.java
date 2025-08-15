package com.chtl.ast.node;
import com.chtl.ast.NodeType;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.AbstractCHTLASTNode;

import com.chtl.model.Attribute;

public class AttributeNode extends AbstractCHTLASTNode {
    private Attribute attribute;

    public AttributeNode(Attribute attribute) {
        super(NodeType.ATTRIBUTE);
        this.attribute = attribute;
    }

    public Attribute getAttribute() {
        return attribute;
    }
}
