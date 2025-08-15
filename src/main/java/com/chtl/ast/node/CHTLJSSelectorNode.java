package com.chtl.ast.node;import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.CHTLJSVisitor;

public class CHTLJSSelectorNode extends AbstractCHTLASTNode {
    private String selector;
    
    public CHTLJSSelectorNode(String selector) {
        this.selector = selector;
    }
    
    public String getSelector() {
        return selector;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSSelector(this);
        }
    }
}