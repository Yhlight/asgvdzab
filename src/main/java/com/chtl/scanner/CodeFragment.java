package com.chtl.scanner;

import com.chtl.model.FragmentType;

public class CodeFragment {
    private final String content;
    private final FragmentType type;
    private final int startPosition;
    private final int endPosition;
    private final int line;
    private final int column;

    public CodeFragment(String content, FragmentType type, int startPosition, int endPosition, int line, int column) {
        this.content = content;
        this.type = type;
        this.startPosition = startPosition;
        this.endPosition = endPosition;
        this.line = line;
        this.column = column;
    }

    public String getContent() { return content; }
    public FragmentType getType() { return type; }
    public int getStartPosition() { return startPosition; }
    public int getEndPosition() { return endPosition; }
    public int getLine() { return line; }
    public int getColumn() { return column; }
    public int getLength() { return endPosition - startPosition; }
}
