package com.chtl.ast.node;

import java.io.Serializable;

/**
 * 特例化操作节点
 * 用于[Custom]定义中的特殊操作
 */
public class SpecializationNode extends AbstractASTNode implements Serializable {
    private static final long serialVersionUID = 1L;
    
    /**
     * 特例化操作类型
     */
    public enum SpecializationType {
        ADD_STYLE,        // 增加样式
        REMOVE_STYLE,     // 删除样式  
        ADD_ELEMENT,      // 增加元素
        REMOVE_ELEMENT,   // 删除元素
        MODIFY_PROPERTY,  // 修改属性
        OVERRIDE_INHERIT  // 覆盖继承
    }
    
    private SpecializationType type;
    private String target;      // 操作目标
    private ASTNode value;      // 操作值
    private int position = -1;  // 位置（用于删除/插入操作）
    
    public SpecializationNode(SpecializationType type) {
        super(NodeType.SPECIALIZATION);
        this.type = type;
    }
    
    // Getters and setters
    public SpecializationType getType() {
        return type;
    }
    
    public void setType(SpecializationType type) {
        this.type = type;
    }
    
    public String getTarget() {
        return target;
    }
    
    public void setTarget(String target) {
        this.target = target;
    }
    
    public ASTNode getValue() {
        return value;
    }
    
    public void setValue(ASTNode value) {
        this.value = value;
    }
    
    public int getPosition() {
        return position;
    }
    
    public void setPosition(int position) {
        this.position = position;
    }
    
    @Override
    public void accept(ASTVisitor visitor) {
        visitor.visit(this);
    }
    
    @Override
    public String toString() {
        return String.format("Specialization[%s]: %s", type, target);
    }
}