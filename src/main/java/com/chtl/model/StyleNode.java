package com.chtl.model;

import com.chtl.model.StyleNode.StyleProperty;
import com.chtl.model.StyleNode.StyleRule;


import java.util.ArrayList;
import java.util.List;

/**
 * 样式节点
 */
public class StyleNode extends CHTLNode {
    private boolean isLocal;
    private List<StyleRule> rules;
    
    public StyleNode(boolean isLocal) {
        super("style");
        this.isLocal = isLocal;
        this.rules = new ArrayList<>();
    }
    
    public boolean isLocal() {
        return isLocal;
    }
    
    public List<StyleRule> getRules() {
        return rules;
    }
    
    public void addRule(StyleRule rule) {
        rules.add(rule);
    }
    
    @Override
    public void accept(CHTLNodeVisitor visitor) {
        visitor.visitStyleNode(this);
    }
    
    /**
     * 样式规则内部类
     */
    public static class StyleRule {
        private String selector;
        private List<StyleProperty> properties;
        
        public StyleRule(String selector) {
            this.selector = selector;
            this.properties = new ArrayList<>();
        }
        
        public String getSelector() {
            return selector;
        }
        
        public List<StyleProperty> getProperties() {
            return properties;
        }
        
        public void addProperty(StyleProperty property) {
            properties.add(property);
        }
    }
    
    /**
     * 样式属性内部类
     */
    public static class StyleProperty {
        private String name;
        private String value;
        
        public StyleProperty(String name, String value) {
            this.name = name;
            this.value = value;
        }
        
        public String getName() {
            return name;
        }
        
        public String getValue() {
            return value;
        }
    }
}