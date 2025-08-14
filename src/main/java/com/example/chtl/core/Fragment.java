package com.example.chtl.core;

public record Fragment(
        FragmentType type,
        String content,
        int startOffset,
        int endOffset,
        String origin
) {
}