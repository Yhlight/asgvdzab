package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class InheritStatementNode extends AbstractCHTLASTNode {
    private String parent;

    public InheritStatementNode(String parent) {
        super(NodeType.INHERIT_STATEMENT);
        this.parent = parent;
    }

    public String getParent() { return parent; }

}