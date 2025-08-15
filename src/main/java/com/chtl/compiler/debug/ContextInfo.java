package com.chtl.compiler.debug;
import java.util.ArrayList;
import java.util.List;


import com.chtl.context.CompilationContext;
import java.io.PrintStream;
import java.util.*;

/**
 * 编译上下文信息
 */
public class ContextInfo {
    private CompilationContext.State compilationState;
    private String currentScope;
    private List<String> activeConstraints = new ArrayList<>();
    private List<String> importedModules = new ArrayList<>();
    
    public void print(PrintStream out) {
        if (compilationState != null) {
            out.println("    状态: " + compilationState);
        }
        
        if (currentScope != null && !currentScope.isEmpty()) {
            out.println("    作用域: " + currentScope);
        }
        
        if (!activeConstraints.isEmpty()) {
            out.println("    活跃约束:");
            for (String constraint : activeConstraints) {
                out.println("      - " + constraint);
            }
        }
        
        if (!importedModules.isEmpty()) {
            out.println("    已导入模块:");
            for (String module : importedModules) {
                out.println("      - " + module);
            }
        }
    }
    
    // Getters and setters
    public CompilationContext.State getCompilationState() {
        return compilationState;
    }
    
    public void setCompilationState(CompilationContext.State compilationState) {
        this.compilationState = compilationState;
    }
    
    public String getCurrentScope() {
        return currentScope;
    }
    
    public void setCurrentScope(String currentScope) {
        this.currentScope = currentScope;
    }
    
    public List<String> getActiveConstraints() {
        return activeConstraints;
    }
    
    public void setActiveConstraints(List<String> activeConstraints) {
        this.activeConstraints = activeConstraints;
    }
    
    public List<String> getImportedModules() {
        return importedModules;
    }
    
    public void setImportedModules(List<String> importedModules) {
        this.importedModules = importedModules;
    }
}