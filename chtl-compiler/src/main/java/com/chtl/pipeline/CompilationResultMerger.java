package com.chtl.pipeline;

import com.chtl.model.CodeFragment;

import java.util.List;

public class CompilationResultMerger {
    public String merge(String originalSource, List<CodeFragment> fragments, List<String> compiledOutputs) {
        StringBuilder builder = new StringBuilder(originalSource.length());
        for (int i = 0; i < fragments.size(); i++) {
            builder.append(compiledOutputs.get(i));
        }
        return builder.toString();
    }
}