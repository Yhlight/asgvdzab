package com.chtl.ast.node;

import com.chtl.ast.*;

/**
 * 根节点
 * CHTL文档的最顶层节点
 */
public class RootNode extends AbstractCHTLASTNode {
    
    public RootNode() {
        super(NodeType.ROOT);
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitRoot(this);
    }
}