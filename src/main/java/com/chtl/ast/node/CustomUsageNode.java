package com.chtl.ast.node;

import com.chtl.ast.*;

/**
 * 自定义使用节点
 * 对应CHTL中的@Type Name { }的使用，支持特例化
 */
public class CustomUsageNode extends AbstractCHTLASTNode {
    private TemplateDefinitionNode.TemplateType customType;
    private String name;
    private boolean useFullName; // 是否使用全缀名，如[Custom] @Element Box
    
    public CustomUsageNode(TemplateDefinitionNode.TemplateType customType, String name) {
        super(NodeType.CUSTOM_USAGE);
        this.customType = customType;
        this.name = name;
        this.useFullName = false;
    }
    
    public TemplateDefinitionNode.TemplateType getCustomType() {
        return customType;
    }
    
    public void setCustomType(TemplateDefinitionNode.TemplateType customType) {
        this.customType = customType;
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public boolean isUseFullName() {
        return useFullName;
    }
    
    public void setUseFullName(boolean useFullName) {
        this.useFullName = useFullName;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitCustomUsage(this);
    }
}