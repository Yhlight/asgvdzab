package com.chtl.context;

import com.chtl.model.SourceLocation;

import com.chtl.model.SourceLocation;

/**
 * 编译警告
 */
public class CompilationWarning {
    private final String message;
    private final SourceLocation location;
    private final WarningType type;
    
    public enum WarningType {
        UNUSED_DEFINITION,      // 未使用的定义
        DEPRECATED_FEATURE,     // 废弃的特性
        PERFORMANCE_ISSUE,      // 性能问题
        STYLE_ISSUE,            // 样式问题
        POTENTIAL_ERROR,        // 潜在错误
        NAMING_CONVENTION,      // 命名约定
        COMPATIBILITY_ISSUE,    // 兼容性问题
        SECURITY_ISSUE,         // 安全问题
        REDEFINITION,           // 重定义
        MISSING_VALUE,          // 缺失值
        IMPORT_IGNORED          // 导入被忽略
    }
    
    public CompilationWarning(String message, SourceLocation location, WarningType type) {
        this.message = message;
        this.location = location;
        this.type = type;
    }
    
    public CompilationWarning(String message, WarningType type) {
        this(message, null, type);
    }
    
    public String getMessage() {
        return message;
    }
    
    public SourceLocation getLocation() {
        return location;
    }
    
    public WarningType getType() {
        return type;
    }
    
    @Override
    public String toString() {
        if (location != null) {
            return String.format("[WARNING:%s] %s at %s", type, message, location);
        } else {
            return String.format("[WARNING:%s] %s", type, message);
        }
    }
}
