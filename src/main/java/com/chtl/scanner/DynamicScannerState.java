package com.chtl.scanner;

public class DynamicScannerState {
    private String currentContext;
    private int depth;

    public DynamicScannerState(String context) {
        this.currentContext = context;
        this.depth = 0;
    }

    public String getContext() { return currentContext; }
    public void setContext(String context) { this.currentContext = context; }
    public int getDepth() { return depth; }
    public void incrementDepth() { depth++; }
    public void decrementDepth() { depth--; }
}
