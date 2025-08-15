package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class VarGroupDefinitionNode extends AbstractCHTLASTNode {
    private String name;

    public VarGroupDefinitionNode(String name) {
        super(NodeType.VAR_GROUP_DEFINITION);
        this.name = name;
    }

    public String getName() { return name; }

}