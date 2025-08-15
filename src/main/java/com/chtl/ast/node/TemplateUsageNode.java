package com.chtl.ast.node;import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.*;
import java.util.HashMap;
import java.util.Map;

/**
 * 模板使用节点
 * 对应CHTL中的@Type Name;或@Type Name { }
 */
public class TemplateUsageNode extends AbstractCHTLASTNode {
    private TemplateDefinitionNode.TemplateType templateType;
    private String name;
    private boolean hasSpecialization; // 是否有特例化内容
    private boolean useFullName; // 是否使用全缀名，如[Template] @Element Box
    
    // 用于变量组的参数覆盖，如ThemeColor(tableColor = rgb(145, 155, 200))
    private Map<String, CHTLASTNode> parameters;
    
    public TemplateUsageNode(TemplateDefinitionNode.TemplateType templateType, String name) {
        super(NodeType.TEMPLATE_USAGE);
        this.templateType = templateType;
        this.name = name;
        this.hasSpecialization = false;
        this.useFullName = false;
        this.parameters = new HashMap<>();
    }
    
    public TemplateDefinitionNode.TemplateType getTemplateType() {
        return templateType;
    }
    
    public void setTemplateType(TemplateDefinitionNode.TemplateType templateType) {
        this.templateType = templateType;
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public boolean hasSpecialization() {
        return hasSpecialization;
    }
    
    public void setHasSpecialization(boolean hasSpecialization) {
        this.hasSpecialization = hasSpecialization;
    }
    
    public boolean isUseFullName() {
        return useFullName;
    }
    
    public void setUseFullName(boolean useFullName) {
        this.useFullName = useFullName;
    }
    
    public Map<String, CHTLASTNode> getParameters() {
        return parameters;
    }
    
    public void addParameter(String key, CHTLASTNode value) {
        parameters.put(key, value);
        if (value != null) {
            value.setParent(this);
        }
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitTemplateUsage(this);
    }
}