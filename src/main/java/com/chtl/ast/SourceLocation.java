package com.chtl.ast;

/**
 * 源代码位置信息
 */
public class SourceLocation {
    private final int line;
    private final int column;
    private final int startPosition;
    private final int endPosition;
    
    public SourceLocation(int line, int column, int startPosition, int endPosition) {
        this.line = line;
        this.column = column;
        this.startPosition = startPosition;
        this.endPosition = endPosition;
    }
    
    public int getLine() {
        return line;
    }
    
    public int getColumn() {
        return column;
    }
    
    public int getStartPosition() {
        return startPosition;
    }
    
    public int getEndPosition() {
        return endPosition;
    }
    
    @Override
    public String toString() {
        return String.format("%d:%d", line, column);
    }
}