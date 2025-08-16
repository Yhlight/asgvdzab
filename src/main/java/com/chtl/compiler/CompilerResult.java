package com.chtl.compiler;






import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


import com.chtl.context.CompilationError;
import com.chtl.context.CompilationWarning;

import java.util.*;

/**
 * 编译结果
 */
public class CompilerResult {
    private boolean success;
    private String output;
    private List<CompilationError> errors = new ArrayList<Object>();
    private List<CompilationWarning> warnings = new ArrayList<Object>();
    private long compilationTime;
    private Map<String, String> sourceMaps = new HashMap<Object, Object>();
    
    // 构造函数
    public CompilerResult() {
    }
    
    // 便捷方法
    public boolean hasErrors() {
        return !errors.isEmpty();
    }
    
    public boolean hasWarnings() {
        return !warnings.isEmpty();
    }
    
    public int getErrorCount() {
        return errors.size();
    }
    
    public int getWarningCount() {
        return warnings.size();
    }
    
    // Getters and setters
    public boolean isSuccess() {
        return success;
    }
    
    public void setSuccess(boolean success) {
        this.success = success;
    }
    
    public String getOutput() {
        return output;
    }
    
    public void setOutput(String output) {
        this.output = output;
    }
    
    public List<CompilationError> getErrors() {
        return errors;
    }
    
    public void setErrors(List<CompilationError> errors) {
        this.errors = errors;
    }
    
    public List<CompilationWarning> getWarnings() {
        return warnings;
    }
    
    public void setWarnings(List<CompilationWarning> warnings) {
        this.warnings = warnings;
    }
    
    public long getCompilationTime() {
        return compilationTime;
    }
    
    public void setCompilationTime(long compilationTime) {
        this.compilationTime = compilationTime;
    }
    
    public Map<String, String> getSourceMaps() {
        return sourceMaps;
    }
    
    public void setSourceMaps(Map<String, String> sourceMaps) {
        this.sourceMaps = sourceMaps;
    }
    
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("CompilationResult{\n");
        sb.append("  success=").append(success).append(",\n");
        sb.append("  errors=").append(errors.size()).append(",\n");
        sb.append("  warnings=").append(warnings.size()).append(",\n");
        sb.append("  compilationTime=").append(compilationTime).append("ms\n");
        sb.append("}");
        return sb.toString();
    }
}