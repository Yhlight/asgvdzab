package com.chtl.compiler.chtl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 局部样式处理结果
 */
public class LocalStyleResult {
    private String inlineStyles;
    private StringBuilder globalStyles;
    private List<String> autoClasses;
    private String autoId;
    private Map<String, String> contextSelectors;
    
    public LocalStyleResult() {
        this.inlineStyles = "";
        this.globalStyles = new StringBuilder();
        this.autoClasses = new ArrayList<Object>();
        this.contextSelectors = new HashMap<Object, Object>();
    }
    
    public String getInlineStyles() {
        return inlineStyles;
    }
    
    public void setInlineStyles(String inlineStyles) {
        this.inlineStyles = inlineStyles;
    }
    
    public String getGlobalStyles() {
        return globalStyles.toString();
    }
    
    public void addGlobalStyle(String style) {
        globalStyles.append(style).append("\n");
    }
    
    public List<String> getAutoClasses() {
        return autoClasses;
    }
    
    public void addAutoClass(String className) {
        autoClasses.add(className);
    }
    
    public String getAutoId() {
        return autoId;
    }
    
    public void setAutoId(String autoId) {
        this.autoId = autoId;
    }
    
    public Map<String, String> getContextSelectors() {
        return contextSelectors;
    }
    
    public void addContextSelector(String selector, String properties) {
        contextSelectors.put(selector, properties);
    }
}