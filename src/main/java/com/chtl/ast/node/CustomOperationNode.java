package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class CustomOperationNode extends AbstractCHTLASTNode {
    private String operation;

    public CustomOperationNode(String operation) {
        super(NodeType.CUSTOM_OPERATION);
        this.operation = operation;
    }

    public String getOperation() { return operation; }

}