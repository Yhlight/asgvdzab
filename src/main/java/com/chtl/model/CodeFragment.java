package com.chtl.model;

/**
 * 代码片段模型
 */
public class CodeFragment {
    private final FragmentType type;
    private final String content;
    private final int sourcePosition;
    private final int startPos;
    private final int endPos;
    
    public CodeFragment(FragmentType type, String content, int sourcePosition) {
        this.type = type;
        this.content = content;
        this.sourcePosition = sourcePosition;
        this.startPos = sourcePosition;
        this.endPos = sourcePosition + content.length();
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
    
    public int getStartPos() {
        return startPos;
    }
    
    public int getEndPos() {
        return endPos;
    }
    
    @Override
    public String toString() {
        return String.format("CodeFragment{type=%s, position=%d, contentLength=%d}", 
            type, sourcePosition, content.length());
    }
}