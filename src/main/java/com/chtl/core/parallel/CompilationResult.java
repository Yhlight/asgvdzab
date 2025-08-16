package com.chtl.core.parallel;import com.chtl.compiler.CompilerResult;


public class CompilationResult {
    private final boolean success;
    private final String output;

    public CompilationResult(boolean success, String output) {
        this.success = success;
        this.output = output;
    }

    public boolean isSuccess() { return success; }
    public String getOutput() { return output; }
}
