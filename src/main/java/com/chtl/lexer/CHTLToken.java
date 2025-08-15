package com.chtl.lexer;import com.chtl.compiler.chtl.token.CHTLToken;


public class CHTLToken {
    private String type;
    private String value;
    private int line;
    private int column;

    public CHTLToken(String type, String value) {
        this.type = type;
        this.value = value;
    }

    public String getType() { return type; }
    public String getValue() { return value; }
    public int getLine() { return line; }
    public int getColumn() { return column; }
}
