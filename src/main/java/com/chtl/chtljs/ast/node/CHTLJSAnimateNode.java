package com.chtl.chtljs.ast.node;

import com.chtl.ast.chtljs.CHTLJSASTNode;
import com.chtl.ast.NodeType;
import com.chtl.ast.chtljs.AbstractCHTLJSASTNode;
import com.chtl.ast.node.CHTLJSAnimateNode;


import com.chtl.chtljs.ast.*;

public class CHTLJSAnimateNode extends AbstractCHTLJSASTNode {
    public CHTLJSAnimateNode() {
        super(CHTLJSASTNode.NodeType.ANIMATE);
    }
}
