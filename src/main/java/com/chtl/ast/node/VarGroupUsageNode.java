package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class VarGroupUsageNode extends AbstractCHTLASTNode {
    private String varGroup;
    private String varName;

    public VarGroupUsageNode(String varGroup, String varName) {
        super(NodeType.VAR_GROUP_USAGE);
        this.varGroup = varGroup;
        this.varName = varName;
    }

    public String getVarGroup() { return varGroup; }
    public String getVarName() { return varName; }

}