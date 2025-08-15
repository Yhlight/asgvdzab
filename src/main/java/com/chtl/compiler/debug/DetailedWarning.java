package com.chtl.compiler.debug;

import com.chtl.context.CompilationWarning;

/**
 * 详细警告信息
 */
public class DetailedWarning {
    private final CompilationWarning originalWarning;
    private SourceCodeSnippet codeSnippet;
    
    public DetailedWarning(CompilationWarning originalWarning) {
        this.originalWarning = originalWarning;
    }
    
    // Getters and setters
    public CompilationWarning getOriginalWarning() {
        return originalWarning;
    }
    
    public SourceCodeSnippet getCodeSnippet() {
        return codeSnippet;
    }
    
    public void setCodeSnippet(SourceCodeSnippet codeSnippet) {
        this.codeSnippet = codeSnippet;
    }
}