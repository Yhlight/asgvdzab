package com.chtl.chtljs.ast;

public interface CHTLJSASTNode {
    void accept(CHTLJSASTVisitor visitor);
    NodeType getNodeType();

    enum NodeType {
        ANIMATE,
        LISTEN,
        DELEGATE,
        SELECTOR,
        FUNCTION,
        ARRAY,
        OBJECT,
        STRING,
        NUMBER,
        IDENTIFIER,
        ANIMATION_CONTROLLER
    }
}
