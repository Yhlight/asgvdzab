package com.chtl.chtljs.ast;

public abstract class AbstractCHTLJSASTNode implements CHTLJSASTNode {
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visit(this);
    }
}
