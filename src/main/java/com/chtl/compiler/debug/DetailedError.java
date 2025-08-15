package com.chtl.compiler.debug;

import com.chtl.context.CompilationError;
import java.util.*;

/**
 * 详细错误信息
 */
public class DetailedError {
    private final CompilationError originalError;
    private SourceCodeSnippet codeSnippet;
    private ContextInfo contextInfo;
    private List<String> suggestions = new ArrayList<>();
    
    public DetailedError(CompilationError originalError) {
        this.originalError = originalError;
    }
    
    // Getters and setters
    public CompilationError getOriginalError() {
        return originalError;
    }
    
    public SourceCodeSnippet getCodeSnippet() {
        return codeSnippet;
    }
    
    public void setCodeSnippet(SourceCodeSnippet codeSnippet) {
        this.codeSnippet = codeSnippet;
    }
    
    public ContextInfo getContextInfo() {
        return contextInfo;
    }
    
    public void setContextInfo(ContextInfo contextInfo) {
        this.contextInfo = contextInfo;
    }
    
    public List<String> getSuggestions() {
        return suggestions;
    }
    
    public void setSuggestions(List<String> suggestions) {
        this.suggestions = suggestions;
    }
}