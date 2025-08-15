package com.chtl.ast.node;

import com.chtl.ast.*;

/**
 * 属性节点
 * 对应CHTL中的属性，如 id: box; 或 class= container;
 */
public class AttributeNode extends AbstractCHTLASTNode {
    private String name;
    private CHTLASTNode value; // 可以是字符串字面量、无引号字面量或数字
    private boolean usesColon; // true表示使用:，false表示使用=
    
    public AttributeNode(String name) {
        super(NodeType.ATTRIBUTE);
        this.name = name;
        this.usesColon = true; // 默认使用冒号
    }
    
    public AttributeNode(String name, CHTLASTNode value) {
        super(NodeType.ATTRIBUTE);
        this.name = name;
        this.value = value;
        this.usesColon = true;
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
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
    
    public boolean usesColon() {
        return usesColon;
    }
    
    public void setUsesColon(boolean usesColon) {
        this.usesColon = usesColon;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitAttribute(this);
    }
}