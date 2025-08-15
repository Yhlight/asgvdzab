package com.chtl.compiler.debug;

import com.chtl.context.CompilationError;

/**
 * 错误恢复策略
 * 尝试从错误中恢复，继续编译
 */
public class ErrorRecoveryStrategy {
    
    /**
     * 从语法错误恢复
     */
    public void recoverFromSyntaxError(DetailedError error) {
        String message = error.getOriginalError().getMessage();
        
        if (message.contains("期望 ';'")) {
            // 自动插入分号
            // 在实际实现中，这里会修改token流
            logRecovery("自动插入分号");
        } else if (message.contains("期望 '}'")) {
            // 自动插入右花括号
            logRecovery("自动插入右花括号");
        } else if (message.contains("期望 ']'")) {
            // 自动插入右方括号
            logRecovery("自动插入右方括号");
        } else {
            // 跳过当前语句
            logRecovery("跳过错误语句");
        }
    }
    
    /**
     * 从未定义引用恢复
     */
    public void recoverFromUndefinedReference(DetailedError error) {
        // 可以尝试：
        // 1. 创建临时定义
        // 2. 忽略该引用
        // 3. 使用默认值
        
        logRecovery("忽略未定义的引用");
    }
    
    /**
     * 从类型不匹配恢复
     */
    public void recoverFromTypeMismatch(DetailedError error) {
        // 可以尝试：
        // 1. 自动类型转换
        // 2. 使用默认值
        
        logRecovery("尝试自动类型转换");
    }
    
    /**
     * 从约束违反恢复
     */
    public void recoverFromConstraintViolation(DetailedError error) {
        // 约束违反通常不能恢复
        // 但可以标记并继续编译
        
        logRecovery("标记约束违反，继续编译");
    }
    
    /**
     * 记录恢复操作
     */
    private void logRecovery(String action) {
        // 在实际实现中，这里会记录恢复操作
        // 以便在编译报告中显示
    }
}