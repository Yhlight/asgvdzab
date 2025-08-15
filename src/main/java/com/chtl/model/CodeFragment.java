package com.chtl.model;

/**
 * 代码片段模型
 */
public class CodeFragment {
    private final FragmentType type;
    private final String content;
    private final int sourcePosition;
    
    public CodeFragment(FragmentType type, String content, int sourcePosition) {
        this.type = type;
        this.content = content;
        this.sourcePosition = sourcePosition;
    }
    
    public FragmentType getType() {
        return type;
    }
    
    public String getContent() {
        return content;
    }
    
    public int getSourcePosition() {
        return sourcePosition;
    }
    
    @Override
    public String toString() {
        return String.format("CodeFragment{type=%s, position=%d, contentLength=%d}", 
            type, sourcePosition, content.length());
    }
}