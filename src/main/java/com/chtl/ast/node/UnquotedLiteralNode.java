package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.*;

/**
 * 无引号字面量节点
 * 对应CHTL中的无引号字符串，如 color: red;
 */
public class UnquotedLiteralNode extends AbstractCHTLASTNode {
    private String value;
    
    public UnquotedLiteralNode(String value) {
        super(NodeType.UNQUOTED_LITERAL);
        this.value = value;
    }
    
    public String getValue() {
        return value;
    }
    
    public void setValue(String value) {
        this.value = value;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitUnquotedLiteral(this);
    }
}