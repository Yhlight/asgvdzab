package com.chtl.ast.node;

import com.chtl.ast.*;
import java.util.ArrayList;
import java.util.List;

/**
 * 自定义定义节点
 * 对应CHTL中的[Custom] @Type Name { }
 */
public class CustomDefinitionNode extends AbstractCHTLASTNode {
    private TemplateDefinitionNode.TemplateType customType;
    private String name;
    
    // 用于样式组的无值属性，如 color, font-size;
    private List<String> emptyProperties;
    
    public CustomDefinitionNode(TemplateDefinitionNode.TemplateType customType, String name) {
        super(NodeType.CUSTOM_DEFINITION);
        this.customType = customType;
        this.name = name;
        this.emptyProperties = new ArrayList<>();
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
    
    public List<String> getEmptyProperties() {
        return emptyProperties;
    }
    
    public void addEmptyProperty(String property) {
        if (property != null && !emptyProperties.contains(property)) {
            emptyProperties.add(property);
        }
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitCustomDefinition(this);
    }
}