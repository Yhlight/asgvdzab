package com.chtl.ast.node;
import com.chtl.ast.CHTLASTNode;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class ConstraintNode extends AbstractCHTLASTNode {
    private String target;
    private String constraint;

    public ConstraintNode(String target, String constraint) {
        super(NodeType.CONSTRAINT);
        this.target = target;
        this.constraint = constraint;
    }

    public String getTarget() { return target; }
    public String getConstraint() { return constraint; }

}