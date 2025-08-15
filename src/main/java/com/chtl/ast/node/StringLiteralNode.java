package com.chtl.ast.node;

import com.chtl.ast.*;

/**
 * 字符串字面量节点
 * 对应CHTL中的"..."或'...'
 */
public class StringLiteralNode extends AbstractCHTLASTNode {
    private String value;
    private char quoteChar; // '"' 或 '\''
    
    public StringLiteralNode(String value, char quoteChar) {
        super(NodeType.STRING_LITERAL);
        this.value = value;
        this.quoteChar = quoteChar;
    }
    
    public String getValue() {
        return value;
    }
    
    public void setValue(String value) {
        this.value = value;
    }
    
    public char getQuoteChar() {
        return quoteChar;
    }
    
    public void setQuoteChar(char quoteChar) {
        this.quoteChar = quoteChar;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitStringLiteral(this);
    }
}