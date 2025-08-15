package com.chtl.scanner;

public class Fragment {
    private String content;
    private String type;
    private int startPosition;

    public Fragment(String content, String type, int startPosition) {
        this.content = content;
        this.type = type;
        this.startPosition = startPosition;
    }

    public String getContent() { return content; }
    public String getType() { return type; }
    public int getStartPosition() { return startPosition; }
}
