package com.chtl.model;









import com.chtl.model.SourceLocation;
public class SourceLocation {
    private final String filename;
    private final int line;
    private final int column;
    private final int offset;

    public SourceLocation(String filename, int line, int column, int offset) {
        this.filename = filename;
        this.line = line;
        this.column = column;
        this.offset = offset;
    }

    public String getFilename() { return filename; }
    public int getLine() { return line; }
    public int getColumn() { return column; }
    public int getOffset() { return offset; }

    @Override
    public String toString() {
        return filename + ":" + line + ":" + column;
    }
}
