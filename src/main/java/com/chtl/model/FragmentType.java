package com.chtl.model;

/**
 * 代码片段类型枚举
 */
public enum FragmentType {
    /**
     * CHTL主体代码
     */
    CHTL("CHTL主体代码"),
    
    /**
     * CHTL局部样式（style块）
     */
    CHTL_LOCAL_STYLE("CHTL局部样式"),
    
    /**
     * CHTL JS代码（包含CHTL JS特有语法）
     */
    CHTL_JS("CHTL JS代码"),
    
    /**
     * 纯CSS代码（全局样式）
     */
    CSS("CSS代码"),
    
    /**
     * 纯JavaScript代码
     */
    JAVASCRIPT("JavaScript代码");
    
    private final String description;
    
    FragmentType(String description) {
        this.description = description;
    }
    
    public String getDescription() {
        return description;
    }
}