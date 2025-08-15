package com.chtl.chtljs.ast.node;

import com.chtl.ast.chtljs.AbstractCHTLJSASTNode;
import com.chtl.ast.node.CHTLJSFunctionNode;


import com.chtl.chtljs.ast.*;

public class CHTLJSFunctionNode extends AbstractCHTLJSASTNode {
    public CHTLJSFunctionNode() {
        super(CHTLJSASTNode.NodeType.FUNCTION);
    }
}
