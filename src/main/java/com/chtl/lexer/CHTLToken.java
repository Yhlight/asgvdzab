package com.chtl.lexer;

public class CHTLToken {
    private String type;
    private String value;
    public CHTLToken(String type, String value) {
        this.type = type;
        this.value = value;
    }
    public String getType() { return type; }
    public String getValue() { return value; }
}
