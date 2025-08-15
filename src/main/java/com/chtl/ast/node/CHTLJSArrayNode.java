package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.CHTLJSVisitor;
import java.util.List;
import java.util.ArrayList;

public class CHTLJSArrayNode extends AbstractCHTLASTNode {
    private List<CHTLASTNode> elements;

    public CHTLJSArrayNode() {
        super(NodeType.CHTL_JS_EXPRESSION);
        this.elements = new ArrayList<>();
    }

    public List<CHTLASTNode> getElements() { return elements; }

    public void addElement(CHTLASTNode element) {
        elements.add(element);
    }

    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSArray(this);
        }
    }
}
