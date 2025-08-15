package com.chtl.compiler.debug;import com.chtl.exception.CompilationException;
import com.chtl.model.SourceLocation;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.chtl.context.CompilationContext;
import com.chtl.context.CompilationError;
import com.chtl.context.CompilationWarning;


import com.chtl.context.*;
import com.chtl.ast.SourceLocation;
import com.chtl.scanner.ScannerStateMachine;

import java.util.*;
import java.io.*;
import java.nio.file.*;

/**
 * 错误处理器
 * 提供详细的错误位置、上下文信息和错误恢复机制
 * 严格遵循CHTL语法文档
 */
public class ErrorHandler {
    private final CompilationContext context;
    private final ScannerStateMachine stateMachine;
    private final SourceCodeManager sourceManager;
    
    // 错误恢复策略
    private final ErrorRecoveryStrategy recoveryStrategy;
    
    // 错误收集
    private final List<DetailedError> detailedErrors = new ArrayList<>();
    private final List<DetailedWarning> detailedWarnings = new ArrayList<>();
    
    // 错误限制
    private int maxErrors = 100;
    private boolean failFast = false;
    
    public ErrorHandler(CompilationContext context) {
        this.context = context;
        this.stateMachine = new ScannerStateMachine(context);
        this.sourceManager = new SourceCodeManager();
        this.recoveryStrategy = new ErrorRecoveryStrategy();
    }
    
    /**
     * 报告错误
     */
    public void reportError(CompilationError error) {
        DetailedError detailed = enhanceError(error);
        detailedErrors.add(detailed);
        
        // 输出错误信息
        if (context.isDebugMode()) {
            printDetailedError(detailed);
        }
        
        // 检查是否需要立即失败
        if (failFast || detailedErrors.size() >= maxErrors) {
            throw new CompilationException("编译失败：错误数量超过限制或启用了快速失败模式");
        }
        
        // 尝试错误恢复
        tryRecover(detailed);
    }
    
    /**
     * 报告警告
     */
    public void reportWarning(CompilationWarning warning) {
        DetailedWarning detailed = enhanceWarning(warning);
        detailedWarnings.add(detailed);
        
        if (context.isDebugMode()) {
            printDetailedWarning(detailed);
        }
    }
    
    /**
     * 增强错误信息
     */
    private DetailedError enhanceError(CompilationError error) {
        DetailedError detailed = new DetailedError(error);
        
        if (error.getLocation() != null) {
            // 获取源代码片段
            SourceCodeSnippet snippet = sourceManager.getSnippet(
                error.getLocation().getSourcePath(),
                error.getLocation().getLine(),
                error.getLocation().getColumn()
            );
            detailed.setCodeSnippet(snippet);
            
            // 获取上下文信息
            detailed.setContextInfo(gatherContextInfo(error.getLocation()));
            
            // 生成建议
            detailed.setSuggestions(generateSuggestions(error));
        }
        
        return detailed;
    }
    
    /**
     * 增强警告信息
     */
    private DetailedWarning enhanceWarning(CompilationWarning warning) {
        DetailedWarning detailed = new DetailedWarning(warning);
        
        if (warning.getLocation() != null) {
            SourceCodeSnippet snippet = sourceManager.getSnippet(
                warning.getLocation().getSourcePath(),
                warning.getLocation().getLine(),
                warning.getLocation().getColumn()
            );
            detailed.setCodeSnippet(snippet);
        }
        
        return detailed;
    }
    
    /**
     * 收集上下文信息
     */
    private ContextInfo gatherContextInfo(SourceLocation location) {
        ContextInfo info = new ContextInfo();
        
        // 当前编译状态
        info.setCompilationState(context.getCurrentState());
        
        // 当前作用域
        info.setCurrentScope("GLOBAL"); // 简化实现
        
        // 活跃的约束
        if (context.getConstraintManager() != null) {
            // 简化实现
            info.setActiveConstraints(new ArrayList<>());
        }
        
        // 导入的模块
        if (context.getImportManager() != null) {
            // 简化实现
            info.setImportedModules(new ArrayList<>());
        }
        
        return info;
    }
    
    /**
     * 生成修复建议
     */
    private List<String> generateSuggestions(CompilationError error) {
        List<String> suggestions = new ArrayList<>();
        
        switch (error.getType()) {
            case SYNTAX_ERROR:
                suggestions.addAll(generateSyntaxSuggestions(error));
                break;
                
            case UNDEFINED_REFERENCE:
                suggestions.addAll(generateReferenceSuggestions(error));
                break;
                
            case DUPLICATE_DEFINITION:
                suggestions.add("检查是否有重复的定义");
                suggestions.add("考虑使用不同的名称");
                break;
                
            case TYPE_MISMATCH:
                suggestions.add("检查类型是否匹配");
                suggestions.add("考虑使用类型转换");
                break;
                
            case CONSTRAINT_VIOLATION:
                suggestions.add("检查约束条件");
                suggestions.add("查看 except 规则是否正确");
                break;
                
            case IMPORT_ERROR:
                suggestions.add("检查文件路径是否正确");
                suggestions.add("确认文件是否存在");
                suggestions.add("验证导入语法");
                break;
                
            case CIRCULAR_DEPENDENCY:
                suggestions.add("检查导入链");
                suggestions.add("移除循环依赖");
                break;
        }
        
        return suggestions;
    }
    
    /**
     * 生成语法建议
     */
    private List<String> generateSyntaxSuggestions(CompilationError error) {
        List<String> suggestions = new ArrayList<>();
        String message = error.getMessage();
        
        if (message.contains("期望")) {
            // 提取期望的token
            String expected = extractExpected(message);
            if (expected != null) {
                suggestions.add("尝试添加 " + expected);
            }
        }
        
        if (message.contains("意外的")) {
            suggestions.add("检查语法是否正确");
            suggestions.add("可能存在多余的字符");
        }
        
        // 常见语法错误
        if (message.contains(";")) {
            suggestions.add("检查是否缺少分号");
        }
        if (message.contains("{") || message.contains("}")) {
            suggestions.add("检查花括号是否匹配");
        }
        if (message.contains("[") || message.contains("]")) {
            suggestions.add("检查方括号是否匹配");
        }
        
        return suggestions;
    }
    
    /**
     * 生成引用建议
     */
    private List<String> generateReferenceSuggestions(CompilationError error) {
        List<String> suggestions = new ArrayList<>();
        
        // 提取未定义的标识符
        String identifier = extractIdentifier(error.getMessage());
        if (identifier != null) {
            // 查找相似的已定义标识符
            List<String> similar = findSimilarIdentifiers(identifier);
            for (String sim : similar) {
                suggestions.add("是否想要使用 '" + sim + "'?");
            }
        }
        
        suggestions.add("检查拼写是否正确");
        suggestions.add("确认是否已经导入或定义");
        
        return suggestions;
    }
    
    /**
     * 尝试错误恢复
     */
    private void tryRecover(DetailedError error) {
        CompilationError.ErrorType type = error.getOriginalError().getType();
        
        switch (type) {
            case SYNTAX_ERROR:
                recoveryStrategy.recoverFromSyntaxError(error);
                break;
                
            case UNDEFINED_REFERENCE:
                recoveryStrategy.recoverFromUndefinedReference(error);
                break;
                
            default:
                // 其他错误类型的恢复
                break;
        }
    }
    
    /**
     * 打印详细错误
     */
    private void printDetailedError(DetailedError error) {
        System.err.println("\n" + ColorUtil.red("错误") + ": " + error.getOriginalError().getMessage());
        
        SourceLocation loc = error.getOriginalError().getLocation();
        if (loc != null) {
            System.err.println("  位置: " + loc.getSourcePath() + ":" + loc.getLine() + ":" + loc.getColumn());
        }
        
        // 打印代码片段
        if (error.getCodeSnippet() != null) {
            error.getCodeSnippet().print(System.err);
        }
        
        // 打印上下文信息
        if (error.getContextInfo() != null && context.isVerboseMode()) {
            System.err.println("\n  上下文信息:");
            error.getContextInfo().print(System.err);
        }
        
        // 打印建议
        if (!error.getSuggestions().isEmpty()) {
            System.err.println("\n  建议:");
            for (String suggestion : error.getSuggestions()) {
                System.err.println("    • " + suggestion);
            }
        }
    }
    
    /**
     * 打印详细警告
     */
    private void printDetailedWarning(DetailedWarning warning) {
        System.err.println("\n" + ColorUtil.yellow("警告") + ": " + warning.getOriginalWarning().getMessage());
        
        SourceLocation loc = warning.getOriginalWarning().getLocation();
        if (loc != null) {
            System.err.println("  位置: " + loc.getSourcePath() + ":" + loc.getLine() + ":" + loc.getColumn());
        }
        
        if (warning.getCodeSnippet() != null) {
            warning.getCodeSnippet().print(System.err);
        }
    }
    
    /**
     * 生成错误报告
     */
    public ErrorReport generateReport() {
        ErrorReport report = new ErrorReport();
        report.setErrors(detailedErrors);
        report.setWarnings(detailedWarnings);
        report.setStatistics(generateStatistics());
        return report;
    }
    
    /**
     * 生成统计信息
     */
    private ErrorStatistics generateStatistics() {
        ErrorStatistics stats = new ErrorStatistics();
        
        // 按类型统计错误
        Map<CompilationError.ErrorType, Integer> errorCounts = new HashMap<>();
        for (DetailedError error : detailedErrors) {
            CompilationError.ErrorType type = error.getOriginalError().getType();
            errorCounts.put(type, errorCounts.getOrDefault(type, 0) + 1);
        }
        stats.setErrorCountsByType(errorCounts);
        
        // 按文件统计错误
        Map<String, Integer> errorsByFile = new HashMap<>();
        for (DetailedError error : detailedErrors) {
            SourceLocation loc = error.getOriginalError().getLocation();
            if (loc != null) {
                String file = loc.getSourcePath();
                errorsByFile.put(file, errorsByFile.getOrDefault(file, 0) + 1);
            }
        }
        stats.setErrorCountsByFile(errorsByFile);
        
        stats.setTotalErrors(detailedErrors.size());
        stats.setTotalWarnings(detailedWarnings.size());
        
        return stats;
    }
    
    // 辅助方法
    private String extractExpected(String message) {
        // 从错误消息中提取期望的内容
        int index = message.indexOf("期望");
        if (index >= 0) {
            int start = index + 2;
            int end = message.indexOf("'", start + 1);
            if (end > start) {
                return message.substring(start + 1, end);
            }
        }
        return null;
    }
    
    private String extractIdentifier(String message) {
        // 从错误消息中提取标识符
        // 简化实现
        return null;
    }
    
    private List<String> findSimilarIdentifiers(String identifier) {
        // 查找相似的标识符
        // 简化实现
        return new ArrayList<>();
    }
    
    // Getters and setters
    public void setMaxErrors(int maxErrors) {
        this.maxErrors = maxErrors;
    }
    
    public void setFailFast(boolean failFast) {
        this.failFast = failFast;
    }
    
    public List<DetailedError> getDetailedErrors() {
        return detailedErrors;
    }
    
    public List<DetailedWarning> getDetailedWarnings() {
        return detailedWarnings;
    }
}