package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class StringLiteralNode extends AbstractCHTLASTNode {
    private String value;

    public StringLiteralNode(String value) {
        super(NodeType.STRING_LITERAL);
        this.value = value;
    }

    public String getValue() { return value; }

}