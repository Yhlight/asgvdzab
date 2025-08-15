package com.chtl.ast.node;import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.*;

/**
 * 变量组使用节点
 * 对应CHTL中的VarGroupName(varName)使用
 */
public class VarGroupUsageNode extends AbstractCHTLASTNode {
    private String groupName;
    private String variableName;
    private CHTLASTNode overrideValue; // 可选的覆盖值
    
    public VarGroupUsageNode(String groupName, String variableName) {
        super(NodeType.VAR_GROUP_USAGE);
        this.groupName = groupName;
        this.variableName = variableName;
    }
    
    public String getGroupName() {
        return groupName;
    }
    
    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }
    
    public String getVariableName() {
        return variableName;
    }
    
    public void setVariableName(String variableName) {
        this.variableName = variableName;
    }
    
    public CHTLASTNode getOverrideValue() {
        return overrideValue;
    }
    
    public void setOverrideValue(CHTLASTNode overrideValue) {
        this.overrideValue = overrideValue;
        if (overrideValue != null) {
            overrideValue.setParent(this);
        }
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitVarGroupUsage(this);
    }
}