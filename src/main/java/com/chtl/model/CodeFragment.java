package com.chtl.model;

import java.util.HashMap;
import java.util.Map;

/**
 * 代码片段
 * 表示扫描器识别出的一段特定语言的代码
 */
public class CodeFragment {
    private FragmentType type;
    private String content;
    private int startPosition;
    private int length;
    private int line;
    private int column;
    private Map<String, Object> metadata;
    
    public CodeFragment(FragmentType type, String content, int startPosition, int length) {
        this.type = type;
        this.content = content;
        this.startPosition = startPosition;
        this.length = length;
        this.metadata = new HashMap<>();
    }
    
    // Getters and Setters
    public FragmentType getType() {
        return type;
    }
    
    public void setType(FragmentType type) {
        this.type = type;
    }
    
    public String getContent() {
        return content;
    }
    
    public void setContent(String content) {
        this.content = content;
    }
    
    public int getStartPosition() {
        return startPosition;
    }
    
    public void setStartPosition(int startPosition) {
        this.startPosition = startPosition;
    }
    
    public int getLength() {
        return length;
    }
    
    public void setLength(int length) {
        this.length = length;
    }
    
    public int getLine() {
        return line;
    }
    
    public void setLine(int line) {
        this.line = line;
    }
    
    public int getColumn() {
        return column;
    }
    
    public void setColumn(int column) {
        this.column = column;
    }
    
    public void setMetadata(String key, Object value) {
        metadata.put(key, value);
    }
    
    public Object getMetadata(String key) {
        return metadata.get(key);
    }
    
    public Map<String, Object> getAllMetadata() {
        return new HashMap<>(metadata);
    }
    
    @Override
    public String toString() {
        return String.format("CodeFragment[type=%s, line=%d, col=%d, len=%d]", 
            type, line, column, length);
    }
}