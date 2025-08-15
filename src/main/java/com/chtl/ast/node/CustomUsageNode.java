package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class CustomUsageNode extends AbstractCHTLASTNode {
    private String customName;

    public CustomUsageNode(String customName) {
        super(NodeType.CUSTOM_USAGE);
        this.customName = customName;
    }

    public String getCustomName() { return customName; }

}