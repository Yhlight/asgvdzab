package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.*;

/**
 * 数字字面量节点
 * 对应CHTL中的数字，如 100、50.5等
 */
public class NumberLiteralNode extends AbstractCHTLASTNode {
    private String value; // 保存为字符串以保留原始格式
    private double numericValue;
    
    public NumberLiteralNode(String value) {
        super(NodeType.NUMBER_LITERAL);
        this.value = value;
        try {
            this.numericValue = Double.parseDouble(value);
        } catch (NumberFormatException e) {
            this.numericValue = 0.0;
        }
    }
    
    public String getValue() {
        return value;
    }
    
    public void setValue(String value) {
        this.value = value;
        try {
            this.numericValue = Double.parseDouble(value);
        } catch (NumberFormatException e) {
            this.numericValue = 0.0;
        }
    }
    
    public double getNumericValue() {
        return numericValue;
    }
    
    public boolean isInteger() {
        return numericValue == Math.floor(numericValue);
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitNumberLiteral(this);
    }
}