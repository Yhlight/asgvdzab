package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class NumberLiteralNode extends AbstractCHTLASTNode {
    private double value;

    public NumberLiteralNode(double value) {
        super(NodeType.NUMBER_LITERAL);
        this.value = value;
    }

    public double getValue() { return value; }

}