package com.chtl.compiler.chtljs.token;

/**
 * CHTL JS Token类
 */
public class CHTLJSToken {
    private final CHTLJSTokenType type;
    private final String value;
    private final int line;
    private final int column;
    private final int position;
    
    public CHTLJSToken(CHTLJSTokenType type, String value, int line, int column, int position) {
        this.type = type;
        this.value = value;
        this.line = line;
        this.column = column;
        this.position = position;
    }
    
    public CHTLJSTokenType getType() {
        return type;
    }
    
    public String getValue() {
        return value;
    }
    
    public int getLine() {
        return line;
    }
    
    public int getColumn() {
        return column;
    }
    
    public int getPosition() {
        return position;
    }
    
    @Override
    public String toString() {
        return String.format("CHTLJSToken[%s, '%s', %d:%d]", type, value, line, column);
    }
}