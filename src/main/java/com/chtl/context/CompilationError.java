package com.chtl.context;

import com.chtl.ast.SourceLocation;

/**
 * 编译错误
 */
public class CompilationError {
    private final String message;
    private final SourceLocation location;
    private final ErrorType type;
    
    public enum ErrorType {
        SYNTAX_ERROR,           // 语法错误
        SEMANTIC_ERROR,         // 语义错误
        CONSTRAINT_VIOLATION,   // 约束违反
        UNDEFINED_REFERENCE,    // 未定义引用
        DUPLICATE_DEFINITION,   // 重复定义
        TYPE_MISMATCH,          // 类型不匹配
        INVALID_OPERATION,      // 无效操作
        IMPORT_ERROR,           // 导入错误
        CONFIGURATION_ERROR,    // 配置错误
        CIRCULAR_DEPENDENCY     // 循环依赖
    }
    
    public CompilationError(String message, SourceLocation location, ErrorType type) {
        this.message = message;
        this.location = location;
        this.type = type;
    }
    
    public CompilationError(String message, ErrorType type) {
        this(message, null, type);
    }
    
    public String getMessage() {
        return message;
    }
    
    public SourceLocation getLocation() {
        return location;
    }
    
    public ErrorType getType() {
        return type;
    }
    
    @Override
    public String toString() {
        if (location != null) {
            return String.format("[%s] %s at %s", type, message, location);
        } else {
            return String.format("[%s] %s", type, message);
        }
    }
}