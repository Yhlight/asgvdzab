package com.chtl.chtljs.ast;

import com.chtl.ast.chtljs.CHTLJSASTNode;
import com.chtl.ast.chtljs.CHTLJSASTVisitor;


public interface CHTLJSASTVisitor {
    void visit(CHTLJSASTNode node);
}
