package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class NamespaceNode extends AbstractCHTLASTNode {
    private String name;

    public NamespaceNode(String name) {
        super(NodeType.NAMESPACE);
        this.name = name;
    }

    public String getName() { return name; }

}