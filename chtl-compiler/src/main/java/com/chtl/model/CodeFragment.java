package com.chtl.model;

public class CodeFragment {
    private final CodeFragmentType type;
    private final int startIndexInclusive;
    private final int endIndexExclusive;
    private final String sourceSlice;

    public CodeFragment(CodeFragmentType type, int startIndexInclusive, int endIndexExclusive, String sourceSlice) {
        this.type = type;
        this.startIndexInclusive = startIndexInclusive;
        this.endIndexExclusive = endIndexExclusive;
        this.sourceSlice = sourceSlice;
    }

    public CodeFragmentType getType() {
        return type;
    }

    public int getStartIndexInclusive() {
        return startIndexInclusive;
    }

    public int getEndIndexExclusive() {
        return endIndexExclusive;
    }

    public String getSourceSlice() {
        return sourceSlice;
    }
}