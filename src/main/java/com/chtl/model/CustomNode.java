package com.chtl.model;

/**
 * 自定义节点
 */
public class CustomNode extends CHTLNode {
    private CustomType type;
    private String customName;
    
    public enum CustomType {
        STYLE("@Style"),
        ELEMENT("@Element"),
        VAR("@Var");
        
        private final String keyword;
        
        CustomType(String keyword) {
            this.keyword = keyword;
        }
        
        public String getKeyword() {
            return keyword;
        }
    }
    
    public CustomNode(CustomType type, String customName) {
        super("custom");
        this.type = type;
        this.customName = customName;
    }
    
    public CustomType getType() {
        return type;
    }
    
    public String getCustomName() {
        return customName;
    }
    
    @Override
    public void accept(CHTLNodeVisitor visitor) {
        visitor.visitCustomNode(this);
    }
}