package com.chtl.chtljs.ast;

import com.chtl.ast.NodeType;
import com.chtl.ast.chtljs.CHTLJSASTNode;
import com.chtl.ast.chtljs.CHTLJSASTVisitor;


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
