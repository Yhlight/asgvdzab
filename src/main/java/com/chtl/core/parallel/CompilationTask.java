package com.chtl.core.parallel;import com.chtl.compiler.CompilerResult;


import java.util.concurrent.RecursiveTask;

public class CompilationTask extends RecursiveTask<CompilationResult> {
    private final String source;

    public CompilationTask(String source) {
        this.source = source;
    }

    @Override
    protected CompilationResult compute() {
        // Stub implementation
        return new CompilationResult(true, "");
    }
}
