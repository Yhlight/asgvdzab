package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;
import com.chtl.compiler.constraint.ConstraintManager.ConstraintTarget;


import com.chtl.ast.*;
import java.util.ArrayList;
import java.util.List;

/**
 * 约束节点
 * 对应CHTL中的except语句
 */
public class ConstraintNode extends AbstractCHTLASTNode {
    
    public enum ConstraintTarget {
        HTML,           // @Html
        STYLE,          // @Style
        ELEMENT,        // @Element
        VAR,            // @Var
        TEMPLATE,       // [Template]
        CUSTOM,         // [Custom]
        TEMPLATE_STYLE, // [Template] @Style
        TEMPLATE_ELEMENT, // [Template] @Element
        TEMPLATE_VAR,   // [Template] @Var
        CUSTOM_STYLE,   // [Custom] @Style
        CUSTOM_ELEMENT, // [Custom] @Element
        CUSTOM_VAR,     // [Custom] @Var
        SPECIFIC_NAME   // 具体的名称，如Box
    }
    
    private ConstraintTarget target;
    private String specificName; // 当target为SPECIFIC_NAME时使用
    private boolean isGlobal; // 是否是全局约束
    
    public ConstraintNode(ConstraintTarget target) {
        super(NodeType.CONSTRAINT);
        this.target = target;
        this.isGlobal = false;
    }
    
    public ConstraintNode(String specificName) {
        super(NodeType.CONSTRAINT);
        this.target = ConstraintTarget.SPECIFIC_NAME;
        this.specificName = specificName;
        this.isGlobal = false;
    }
    
    public ConstraintTarget getTarget() {
        return target;
    }
    
    public void setTarget(ConstraintTarget target) {
        this.target = target;
    }
    
    public String getSpecificName() {
        return specificName;
    }
    
    public void setSpecificName(String specificName) {
        this.specificName = specificName;
    }
    
    public boolean isGlobal() {
        return isGlobal;
    }
    
    public void setGlobal(boolean global) {
        isGlobal = global;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitConstraint(this);
    }
}