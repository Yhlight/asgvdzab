package com.chtl.exception;

public class CompilerError extends Exception {
    private final int line;
    private final int column;
    private final String filename;

    public CompilerError(String message, String filename, int line, int column) {
        super(message);
        this.filename = filename;
        this.line = line;
        this.column = column;
    }

    public int getLine() { return line; }
    public int getColumn() { return column; }
    public String getFilename() { return filename; }
}
