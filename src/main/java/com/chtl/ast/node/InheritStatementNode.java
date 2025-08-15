package com.chtl.ast.node;

import com.chtl.ast.*;

/**
 * 继承语句节点
 * 对应CHTL中的inherit @Type Name;语句
 */
public class InheritStatementNode extends AbstractCHTLASTNode {
    private TemplateDefinitionNode.TemplateType inheritType;
    private String targetName;
    
    public InheritStatementNode(TemplateDefinitionNode.TemplateType inheritType, String targetName) {
        super(NodeType.INHERIT_STATEMENT);
        this.inheritType = inheritType;
        this.targetName = targetName;
    }
    
    public TemplateDefinitionNode.TemplateType getInheritType() {
        return inheritType;
    }
    
    public void setInheritType(TemplateDefinitionNode.TemplateType inheritType) {
        this.inheritType = inheritType;
    }
    
    public String getTargetName() {
        return targetName;
    }
    
    public void setTargetName(String targetName) {
        this.targetName = targetName;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitInheritStatement(this);
    }
}