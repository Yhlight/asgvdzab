package com.chtl.model;import com.chtl.compiler.CompilerResult;


import java.util.ArrayList;
import java.util.List;

/**
 * 编译结果模型
 */
public class CompilationResult {
    private final FragmentType sourceType;
    private final String compiledContent;
    private final List<String> errors;
    private final List<String> warnings;
    
    // 额外的元数据
    private String generatedCSS;
    private String generatedJS;
    
    public CompilationResult(FragmentType sourceType, String compiledContent) {
        this(sourceType, compiledContent, null);
    }
    
    public CompilationResult(FragmentType sourceType, String compiledContent, String error) {
        this.sourceType = sourceType;
        this.compiledContent = compiledContent;
        this.errors = new ArrayList<Object>();
        this.warnings = new ArrayList<Object>();
        
        if (error != null && !error.isEmpty()) {
            this.errors.add(error);
        }
    }
    
    public FragmentType getSourceType() {
        return sourceType;
    }
    
    public String getCompiledContent() {
        return compiledContent;
    }
    
    public List<String> getErrors() {
        return errors;
    }
    
    public List<String> getWarnings() {
        return warnings;
    }
    
    public boolean hasErrors() {
        return !errors.isEmpty();
    }
    
    public void addError(String error) {
        errors.add(error);
    }
    
    public void addWarning(String warning) {
        warnings.add(warning);
    }
    
    public String getGeneratedCSS() {
        return generatedCSS;
    }
    
    public void setGeneratedCSS(String generatedCSS) {
        this.generatedCSS = generatedCSS;
    }
    
    public String getGeneratedJS() {
        return generatedJS;
    }
    
    public void setGeneratedJS(String generatedJS) {
        this.generatedJS = generatedJS;
    }
}