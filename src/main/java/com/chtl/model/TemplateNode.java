package com.chtl.model;

/**
 * 模板节点
 */
public class TemplateNode extends CHTLNode {
    private TemplateType type;
    private String templateName;
    
    public enum TemplateType {
        STYLE("@Style"),
        ELEMENT("@Element"),
        VAR("@Var");
        
        private final String keyword;
        
        TemplateType(String keyword) {
            this.keyword = keyword;
        }
        
        public String getKeyword() {
            return keyword;
        }
    }
    
    public TemplateNode(TemplateType type, String templateName) {
        super("template");
        this.type = type;
        this.templateName = templateName;
    }
    
    public TemplateType getType() {
        return type;
    }
    
    public String getTemplateName() {
        return templateName;
    }
    
    
}