package com.chtl.chtljs.ast;

public interface CHTLJSASTNode {
    void accept(CHTLJSASTVisitor visitor);
}
