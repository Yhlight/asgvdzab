package com.chtl.ast;

import java.io.Serializable;

/**
 * 源代码位置信息
 */
public class SourceLocation implements Serializable {
    private static final long serialVersionUID = 1L;
    private final String sourcePath;
    private final int line;
    private final int column;
    private final int startPosition;
    private final int endPosition;
    
    public SourceLocation(int line, int column, int startPosition, int endPosition) {
        this(null, line, column, startPosition, endPosition);
    }
    
    public SourceLocation(String sourcePath, int line, int column) {
        this(sourcePath, line, column, -1, -1);
    }
    
    public SourceLocation(String sourcePath, int line, int column, int startPosition, int endPosition) {
        this.sourcePath = sourcePath;
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
    
    public String getSourcePath() {
        return sourcePath;
    }
    
    @Override
    public String toString() {
        if (sourcePath != null) {
            return String.format("%s:%d:%d", sourcePath, line, column);
        }
        return String.format("%d:%d", line, column);
    }
}