package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class TemplateDefinitionNode extends AbstractCHTLASTNode {
    private String type;
    private String name;

    public TemplateDefinitionNode(String type, String name) {
        super(NodeType.TEMPLATE_DEFINITION);
        this.type = type;
        this.name = name;
    }

    public String getType() { return type; }
    public String getName() { return name; }

}