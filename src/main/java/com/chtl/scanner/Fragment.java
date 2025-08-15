package com.chtl.scanner;

public class Fragment {
    private String content;
    private String type;
    public Fragment(String content, String type) {
        this.content = content;
        this.type = type;
    }
    public String getContent() { return content; }
    public String getType() { return type; }
}
