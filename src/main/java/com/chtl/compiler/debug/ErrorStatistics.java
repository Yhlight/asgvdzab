package com.chtl.compiler.debug;
import java.util.HashMap;
import java.util.Map;


import com.chtl.context.CompilationError;
import java.util.*;

/**
 * 错误统计信息
 */
public class ErrorStatistics {
    private int totalErrors;
    private int totalWarnings;
    private Map<CompilationError.ErrorType, Integer> errorCountsByType = new HashMap<>();
    private Map<String, Integer> errorCountsByFile = new HashMap<>();
    
    // Getters and setters
    public int getTotalErrors() {
        return totalErrors;
    }
    
    public void setTotalErrors(int totalErrors) {
        this.totalErrors = totalErrors;
    }
    
    public int getTotalWarnings() {
        return totalWarnings;
    }
    
    public void setTotalWarnings(int totalWarnings) {
        this.totalWarnings = totalWarnings;
    }
    
    public Map<CompilationError.ErrorType, Integer> getErrorCountsByType() {
        return errorCountsByType;
    }
    
    public void setErrorCountsByType(Map<CompilationError.ErrorType, Integer> errorCountsByType) {
        this.errorCountsByType = errorCountsByType;
    }
    
    public Map<String, Integer> getErrorCountsByFile() {
        return errorCountsByFile;
    }
    
    public void setErrorCountsByFile(Map<String, Integer> errorCountsByFile) {
        this.errorCountsByFile = errorCountsByFile;
    }
}