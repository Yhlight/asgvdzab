package com.chtl.ast.node;import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.*;
import java.util.ArrayList;
import java.util.List;

/**
 * 命名空间节点
 * 对应CHTL中的[Namespace] name { }
 */
public class NamespaceNode extends AbstractCHTLASTNode {
    private String name;
    private List<ConstraintNode> globalConstraints; // 全局约束
    
    public NamespaceNode(String name) {
        super(NodeType.NAMESPACE);
        this.name = name;
        this.globalConstraints = new ArrayList<>();
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public List<ConstraintNode> getGlobalConstraints() {
        return globalConstraints;
    }
    
    public void addGlobalConstraint(ConstraintNode constraint) {
        if (constraint != null) {
            globalConstraints.add(constraint);
            constraint.setParent(this);
        }
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitNamespace(this);
    }
}