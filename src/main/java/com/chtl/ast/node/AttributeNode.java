package com.chtl.ast.node;

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
