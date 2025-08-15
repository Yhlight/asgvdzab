package com.chtl.ast.node;import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.CHTLJSVisitor;

public class CHTLJSIdentifierNode extends AbstractCHTLASTNode {
    private String name;

    public CHTLJSIdentifierNode(String name) {
        this.name = name;
    }

    public String getName() { return name; }

    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSIdentifier(this);
        }
    }
}
