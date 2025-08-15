package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class StyleBlockNode extends AbstractCHTLASTNode {
    private boolean isGlobal;

    public StyleBlockNode(boolean isGlobal) {
        super(NodeType.STYLE_BLOCK);
        this.isGlobal = isGlobal;
    }

    public boolean getIsGlobal() { return isGlobal; }

}