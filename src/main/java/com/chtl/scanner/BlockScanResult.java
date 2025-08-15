package com.chtl.scanner;

import java.util.*;

public class BlockScanResult {
    private final List<CodeFragment> fragments;
    private final Map<String, Object> metadata;

    public BlockScanResult(List<CodeFragment> fragments) {
        this.fragments = fragments;
        this.metadata = new HashMap<>();
    }

    public List<CodeFragment> getFragments() {
        return fragments;
    }

    public Map<String, Object> getMetadata() {
        return metadata;
    }
}
