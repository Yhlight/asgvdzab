package com.chtl.context;

import com.chtl.ast.CHTLASTNode;
import java.util.*;

/**
 * 编译上下文，管理编译过程中的状态和错误
 */
public class CompilationContext {
    
    /**
     * 编译状态枚举
     */
    public enum State {
        INITIALIZING,
        SCANNING,
        PARSING,
        VALIDATING,
        CONSTRAINT_CHECKING,
        GENERATING,
        OPTIMIZING,
        COMPLETED,
        FAILED
    }
    
    private State currentState = State.INITIALIZING;
    private final List<CompilationError> errors = new ArrayList<>();
    private final List<CompilationWarning> warnings = new ArrayList<>();
    private final Map<String, Object> attributes = new HashMap<>();
    private final Stack<State> stateStack = new Stack<>();
    
    private String sourceFile;
    private CHTLASTNode rootNode;
    
    public CompilationContext() {
        this.currentState = State.INITIALIZING;
    }
    
    public CompilationContext(String sourceFile) {
        this.sourceFile = sourceFile;
        this.currentState = State.INITIALIZING;
    }
    
    /**
     * 进入新的编译状态
     */
    public StateGuard enterState(State newState) {
        stateStack.push(currentState);
        currentState = newState;
        return new StateGuard(this);
    }
    
    /**
     * 状态守卫，用于自动恢复状态
     */
    public static class StateGuard implements AutoCloseable {
        private final CompilationContext context;
        
        private StateGuard(CompilationContext context) {
            this.context = context;
        }
        
        @Override
        public void close() {
            if (!context.stateStack.isEmpty()) {
                context.currentState = context.stateStack.pop();
            }
        }
    }
    
    /**
     * 添加编译错误
     */
    public void addError(CompilationError error) {
        errors.add(error);
    }
    
    /**
     * 添加编译警告
     */
    public void addWarning(CompilationWarning warning) {
        warnings.add(warning);
    }
    
    /**
     * 检查是否有错误
     */
    public boolean hasErrors() {
        return !errors.isEmpty();
    }
    
    /**
     * 获取所有错误
     */
    public List<CompilationError> getErrors() {
        return new ArrayList<>(errors);
    }
    
    /**
     * 获取所有警告
     */
    public List<CompilationWarning> getWarnings() {
        return new ArrayList<>(warnings);
    }
    
    /**
     * 设置属性
     */
    public void setAttribute(String key, Object value) {
        attributes.put(key, value);
    }
    
    /**
     * 获取属性
     */
    @SuppressWarnings("unchecked")
    public <T> T getAttribute(String key) {
        return (T) attributes.get(key);
    }
    
    /**
     * 获取属性，带默认值
     */
    @SuppressWarnings("unchecked")
    public <T> T getAttribute(String key, T defaultValue) {
        return attributes.containsKey(key) ? (T) attributes.get(key) : defaultValue;
    }
    
    // Getters and setters
    public State getCurrentState() {
        return currentState;
    }
    
    public void setCurrentState(State currentState) {
        this.currentState = currentState;
    }
    
    public String getSourceFile() {
        return sourceFile;
    }
    
    public void setSourceFile(String sourceFile) {
        this.sourceFile = sourceFile;
    }
    
    public CHTLASTNode getRootNode() {
        return rootNode;
    }
    
    public void setRootNode(CHTLASTNode rootNode) {
        this.rootNode = rootNode;
    }
    
    /**
     * 清除所有错误和警告
     */
    public void clear() {
        errors.clear();
        warnings.clear();
        attributes.clear();
        stateStack.clear();
        currentState = State.INITIALIZING;
        rootNode = null;
    }
    
    /**
     * 创建错误报告
     */
    public String generateErrorReport() {
        StringBuilder report = new StringBuilder();
        report.append("Compilation Report for: ").append(sourceFile).append("\n");
        report.append("=====================================\n");
        
        if (hasErrors()) {
            report.append("\nErrors (").append(errors.size()).append("):\n");
            for (int i = 0; i < errors.size(); i++) {
                CompilationError error = errors.get(i);
                report.append(String.format("%d. %s\n", i + 1, error.getMessage()));
                if (error.getLocation() != null) {
                    report.append("   Location: ").append(error.getLocation()).append("\n");
                }
            }
        }
        
        if (!warnings.isEmpty()) {
            report.append("\nWarnings (").append(warnings.size()).append("):\n");
            for (int i = 0; i < warnings.size(); i++) {
                CompilationWarning warning = warnings.get(i);
                report.append(String.format("%d. %s\n", i + 1, warning.getMessage()));
                if (warning.getLocation() != null) {
                    report.append("   Location: ").append(warning.getLocation()).append("\n");
                }
            }
        }
        
        if (!hasErrors() && warnings.isEmpty()) {
            report.append("\nCompilation completed successfully with no errors or warnings.\n");
        }
        
        return report.toString();
    }
}