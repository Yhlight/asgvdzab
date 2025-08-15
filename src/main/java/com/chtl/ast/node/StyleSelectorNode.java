package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class StyleSelectorNode extends AbstractCHTLASTNode {
    private String selector;

    public StyleSelectorNode(String selector) {
        super(NodeType.STYLE_SELECTOR);
        this.selector = selector;
    }

    public String getSelector() { return selector; }

}