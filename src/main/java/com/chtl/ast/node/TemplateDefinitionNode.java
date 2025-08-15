package com.chtl.ast.node;

import com.chtl.ast.*;

/**
 * 模板定义节点
 * 对应CHTL中的[Template] @Type Name { }
 */
public class TemplateDefinitionNode extends AbstractCHTLASTNode {
    
    public enum TemplateType {
        STYLE,    // @Style
        ELEMENT,  // @Element
        VAR       // @Var
    }
    
    private TemplateType templateType;
    private String name;
    
    public TemplateDefinitionNode(TemplateType templateType, String name) {
        super(NodeType.TEMPLATE_DEFINITION);
        this.templateType = templateType;
        this.name = name;
    }
    
    public TemplateType getTemplateType() {
        return templateType;
    }
    
    public void setTemplateType(TemplateType templateType) {
        this.templateType = templateType;
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitTemplateDefinition(this);
    }
}