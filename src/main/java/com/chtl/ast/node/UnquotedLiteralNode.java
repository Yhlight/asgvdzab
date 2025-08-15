package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class UnquotedLiteralNode extends AbstractCHTLASTNode {
    private String value;

    public UnquotedLiteralNode(String value) {
        super(NodeType.UNQUOTED_LITERAL);
        this.value = value;
    }

    public String getValue() { return value; }

}