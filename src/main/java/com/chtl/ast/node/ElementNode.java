package com.chtl.ast.node;
import com.chtl.ast.NodeType;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.AbstractCHTLASTNode;

import com.chtl.model.Element;

public class ElementNode extends AbstractCHTLASTNode {
    private Element element;

    public ElementNode(Element element) {
        super(NodeType.ELEMENT);
        this.element = element;
    }

    public Element getElement() {
        return element;
    }
}
