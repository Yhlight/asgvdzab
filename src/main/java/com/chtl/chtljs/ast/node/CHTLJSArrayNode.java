package com.chtl.chtljs.ast.node;

import com.chtl.ast.chtljs.AbstractCHTLJSASTNode;
import com.chtl.ast.node.CHTLJSArrayNode;


import com.chtl.chtljs.ast.*;

public class CHTLJSArrayNode extends AbstractCHTLJSASTNode {
    public CHTLJSArrayNode() {
        super(CHTLJSASTNode.NodeType.ARRAY);
    }
}
