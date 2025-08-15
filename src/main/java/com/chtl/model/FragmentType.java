package com.chtl.model;

/**
 * 代码片段类型枚举
 */
public enum FragmentType {
    CHTL("CHTL"),
    CHTL_JS("CHTL_JS"),
    JS("JavaScript"),              // 标准JavaScript
    JAVASCRIPT("JavaScript"),      // 别名，为了兼容性
    CSS("CSS"),
    HTML("HTML"),
    CHTL_LOCAL_STYLE("CHTL_LOCAL_STYLE"),
    CHTL_LOCAL_SCRIPT("CHTL_LOCAL_SCRIPT"),
    MIXED("MIXED");
    
    private final String displayName;
    
    FragmentType(String displayName) {
        this.displayName = displayName;
    }
    
    public String getDisplayName() {
        return displayName;
    }
    
    @Override
    public String toString() {
        return displayName;
    }
}