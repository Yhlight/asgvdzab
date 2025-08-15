package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class RootNode extends AbstractCHTLASTNode {

    public RootNode() {
        super(NodeType.ROOT);
    }


}