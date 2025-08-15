package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.*;

/**
 * 样式属性节点
 * 对应CSS属性，如 color: red;
 */
public class StylePropertyNode extends AbstractCHTLASTNode {
    private String property;
    private CHTLASTNode value; // 可以是字符串字面量、无引号字面量、数字或变量组使用
    private boolean important;
    
    public StylePropertyNode(String property) {
        super(NodeType.STYLE_PROPERTY);
        this.property = property;
        this.important = false;
    }
    
    public StylePropertyNode(String property, CHTLASTNode value) {
        super(NodeType.STYLE_PROPERTY);
        this.property = property;
        this.value = value;
        this.important = false;
        if (value != null) {
            value.setParent(this);
        }
    }
    
    public String getProperty() {
        return property;
    }
    
    public void setProperty(String property) {
        this.property = property;
    }
    
    public CHTLASTNode getValue() {
        return value;
    }
    
    public void setValue(CHTLASTNode value) {
        this.value = value;
        if (value != null) {
            value.setParent(this);
        }
    }
    
    public boolean isImportant() {
        return important;
    }
    
    public void setImportant(boolean important) {
        this.important = important;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitStyleProperty(this);
    }
}