package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class CustomDefinitionNode extends AbstractCHTLASTNode {
    private String name;

    public CustomDefinitionNode(String name) {
        super(NodeType.CUSTOM_DEFINITION);
        this.name = name;
    }

    public String getName() { return name; }

}