package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class StylePropertyNode extends AbstractCHTLASTNode {
    private String name;
    private String value;

    public StylePropertyNode(String name, String value) {
        super(NodeType.STYLE_PROPERTY);
        this.name = name;
        this.value = value;
    }

    public String getName() { return name; }
    public String getValue() { return value; }

}