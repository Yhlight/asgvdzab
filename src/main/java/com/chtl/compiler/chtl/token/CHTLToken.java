package com.chtl.compiler.chtl.token;

import com.chtl.compiler.chtl.CHTLParser.Token;


/**
 * CHTL Token类
 * 严格根据CHTL语法文档定义
 */
public class CHTLToken {
    private final CHTLTokenType type;
    private final String value;
    private final int line;
    private final int column;
    private final int position;
    
    public CHTLToken(CHTLTokenType type, String value, int line, int column, int position) {
        this.type = type;
        this.value = value;
        this.line = line;
        this.column = column;
        this.position = position;
    }
    
    public CHTLTokenType getType() {
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
        return String.format("Token[%s, '%s', %d:%d]", type, value, line, column);
    }
}