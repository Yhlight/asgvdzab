package com.chtl.core.error;

import com.chtl.context.CompilationError;
import com.chtl.context.CompilationWarning;
import com.chtl.context.CompilationContext;
import com.chtl.ast.SourceLocation;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Consumer;

/**
 * 全局错误处理器
 * 提供统一的错误处理策略和错误恢复机制
 */
public class GlobalErrorHandler {
    private static GlobalErrorHandler instance;
    
    // 错误收集器
    private final Map<String, List<CompilationError>> errorsByFile = new ConcurrentHashMap<>();
    private final Map<String, List<CompilationWarning>> warningsByFile = new ConcurrentHashMap<>();
    
    // 错误统计
    private final AtomicInteger totalErrors = new AtomicInteger(0);
    private final AtomicInteger totalWarnings = new AtomicInteger(0);
    private final Map<CompilationError.ErrorType, AtomicInteger> errorCountByType = new ConcurrentHashMap<>();
    
    // 错误处理策略
    private ErrorHandlingStrategy strategy = ErrorHandlingStrategy.COLLECT_ALL;
    private int maxErrors = 100;
    private boolean failFast = false;
    
    // 错误监听器
    private final List<ErrorListener> errorListeners = new ArrayList<>();
    
    // 错误恢复策略
    private final Map<CompilationError.ErrorType, ErrorRecoveryAction> recoveryActions = new HashMap<>();
    
    private GlobalErrorHandler() {
        initializeRecoveryActions();
    }
    
    public static synchronized GlobalErrorHandler getInstance() {
        if (instance == null) {
            instance = new GlobalErrorHandler();
        }
        return instance;
    }
    
    /**
     * 报告错误
     */
    public void reportError(String file, CompilationError error) {
        // 增加计数
        totalErrors.incrementAndGet();
        errorCountByType.computeIfAbsent(error.getType(), k -> new AtomicInteger(0)).incrementAndGet();
        
        // 收集错误
        errorsByFile.computeIfAbsent(file, k -> Collections.synchronizedList(new ArrayList<>())).add(error);
        
        // 通知监听器
        notifyErrorListeners(error);
        
        // 检查是否需要立即失败
        if (failFast || totalErrors.get() >= maxErrors) {
            throw new CompilationFailedException("编译失败：" + error.getMessage(), error);
        }
        
        // 尝试错误恢复
        tryRecover(error);
    }
    
    /**
     * 报告警告
     */
    public void reportWarning(String file, CompilationWarning warning) {
        totalWarnings.incrementAndGet();
        warningsByFile.computeIfAbsent(file, k -> Collections.synchronizedList(new ArrayList<>())).add(warning);
        notifyWarningListeners(warning);
    }
    
    /**
     * 批量报告错误（用于并行处理）
     */
    public void reportErrors(String file, List<CompilationError> errors) {
        for (CompilationError error : errors) {
            reportError(file, error);
        }
    }
    
    /**
     * 创建错误构建器
     */
    public ErrorBuilder error(CompilationError.ErrorType type) {
        return new ErrorBuilder(type);
    }
    
    /**
     * 创建警告构建器
     */
    public WarningBuilder warning(CompilationWarning.WarningType type) {
        return new WarningBuilder(type);
    }
    
    /**
     * 尝试错误恢复
     */
    private void tryRecover(CompilationError error) {
        ErrorRecoveryAction action = recoveryActions.get(error.getType());
        if (action != null) {
            action.recover(error);
        }
    }
    
    /**
     * 初始化错误恢复策略
     */
    private void initializeRecoveryActions() {
        // 语法错误恢复
        recoveryActions.put(CompilationError.ErrorType.SYNTAX_ERROR, error -> {
            // 尝试跳过当前语句
            if (error.getLocation() != null) {
                // 记录恢复点
                RecoveryPoint.save(error.getLocation());
            }
        });
        
        // 未定义引用恢复
        recoveryActions.put(CompilationError.ErrorType.UNDEFINED_REFERENCE, error -> {
            // 创建占位符定义
            String reference = extractReference(error.getMessage());
            if (reference != null) {
                PlaceholderRegistry.register(reference);
            }
        });
        
        // 类型错误恢复
        recoveryActions.put(CompilationError.ErrorType.TYPE_ERROR, error -> {
            // 尝试类型转换
            TypeRecovery.attempt(error);
        });
    }
    
    /**
     * 清除所有错误
     */
    public void clear() {
        errorsByFile.clear();
        warningsByFile.clear();
        totalErrors.set(0);
        totalWarnings.set(0);
        errorCountByType.clear();
    }
    
    /**
     * 清除特定文件的错误
     */
    public void clearFile(String file) {
        List<CompilationError> fileErrors = errorsByFile.remove(file);
        if (fileErrors != null) {
            totalErrors.addAndGet(-fileErrors.size());
            for (CompilationError error : fileErrors) {
                errorCountByType.get(error.getType()).decrementAndGet();
            }
        }
        
        List<CompilationWarning> fileWarnings = warningsByFile.remove(file);
        if (fileWarnings != null) {
            totalWarnings.addAndGet(-fileWarnings.size());
        }
    }
    
    /**
     * 获取错误摘要
     */
    public ErrorSummary getSummary() {
        return new ErrorSummary(
            totalErrors.get(),
            totalWarnings.get(),
            new HashMap<>(errorsByFile),
            new HashMap<>(warningsByFile),
            getErrorTypeStats()
        );
    }
    
    /**
     * 获取错误类型统计
     */
    private Map<CompilationError.ErrorType, Integer> getErrorTypeStats() {
        Map<CompilationError.ErrorType, Integer> stats = new HashMap<>();
        errorCountByType.forEach((type, count) -> stats.put(type, count.get()));
        return stats;
    }
    
    /**
     * 添加错误监听器
     */
    public void addErrorListener(ErrorListener listener) {
        errorListeners.add(listener);
    }
    
    /**
     * 移除错误监听器
     */
    public void removeErrorListener(ErrorListener listener) {
        errorListeners.remove(listener);
    }
    
    /**
     * 通知错误监听器
     */
    private void notifyErrorListeners(CompilationError error) {
        for (ErrorListener listener : errorListeners) {
            listener.onError(error);
        }
    }
    
    /**
     * 通知警告监听器
     */
    private void notifyWarningListeners(CompilationWarning warning) {
        for (ErrorListener listener : errorListeners) {
            listener.onWarning(warning);
        }
    }
    
    /**
     * 设置错误处理策略
     */
    public void setStrategy(ErrorHandlingStrategy strategy) {
        this.strategy = strategy;
    }
    
    /**
     * 设置最大错误数
     */
    public void setMaxErrors(int maxErrors) {
        this.maxErrors = maxErrors;
    }
    
    /**
     * 设置快速失败
     */
    public void setFailFast(boolean failFast) {
        this.failFast = failFast;
    }
    
    /**
     * 检查是否有错误
     */
    public boolean hasErrors() {
        return totalErrors.get() > 0;
    }
    
    /**
     * 检查是否有警告
     */
    public boolean hasWarnings() {
        return totalWarnings.get() > 0;
    }
    
    /**
     * 从错误消息中提取引用
     */
    private String extractReference(String message) {
        // 简单的引用提取逻辑
        if (message.contains("未定义的引用: ")) {
            int start = message.indexOf("未定义的引用: ") + "未定义的引用: ".length();
            int end = message.indexOf(" ", start);
            if (end == -1) end = message.length();
            return message.substring(start, end);
        }
        return null;
    }
    
    /**
     * 错误处理策略枚举
     */
    public enum ErrorHandlingStrategy {
        COLLECT_ALL,      // 收集所有错误
        FAIL_FAST,        // 遇到错误立即失败
        RECOVER_CONTINUE  // 尝试恢复并继续
    }
    
    /**
     * 错误监听器接口
     */
    public interface ErrorListener {
        void onError(CompilationError error);
        void onWarning(CompilationWarning warning);
    }
    
    /**
     * 错误恢复动作接口
     */
    private interface ErrorRecoveryAction {
        void recover(CompilationError error);
    }
    
    /**
     * 错误构建器
     */
    public class ErrorBuilder {
        private final CompilationError.ErrorType type;
        private String message;
        private SourceLocation location;
        private String file;
        
        private ErrorBuilder(CompilationError.ErrorType type) {
            this.type = type;
        }
        
        public ErrorBuilder message(String message) {
            this.message = message;
            return this;
        }
        
        public ErrorBuilder at(SourceLocation location) {
            this.location = location;
            return this;
        }
        
        public ErrorBuilder at(int line, int column) {
            this.location = new SourceLocation(line, column);
            return this;
        }
        
        public ErrorBuilder file(String file) {
            this.file = file;
            return this;
        }
        
        public void report() {
            CompilationError error = location != null 
                ? new CompilationError(message, location, type)
                : new CompilationError(message, type);
            
            if (file != null) {
                reportError(file, error);
            } else {
                reportError("unknown", error);
            }
        }
    }
    
    /**
     * 警告构建器
     */
    public class WarningBuilder {
        private final CompilationWarning.WarningType type;
        private String message;
        private SourceLocation location;
        private String file;
        
        private WarningBuilder(CompilationWarning.WarningType type) {
            this.type = type;
        }
        
        public WarningBuilder message(String message) {
            this.message = message;
            return this;
        }
        
        public WarningBuilder at(SourceLocation location) {
            this.location = location;
            return this;
        }
        
        public WarningBuilder file(String file) {
            this.file = file;
            return this;
        }
        
        public void report() {
            CompilationWarning warning = location != null
                ? new CompilationWarning(message, location, type)
                : new CompilationWarning(message, type);
            
            if (file != null) {
                reportWarning(file, warning);
            } else {
                reportWarning("unknown", warning);
            }
        }
    }
    
    /**
     * 错误摘要
     */
    public static class ErrorSummary {
        public final int totalErrors;
        public final int totalWarnings;
        public final Map<String, List<CompilationError>> errorsByFile;
        public final Map<String, List<CompilationWarning>> warningsByFile;
        public final Map<CompilationError.ErrorType, Integer> errorTypeStats;
        
        public ErrorSummary(int totalErrors, int totalWarnings,
                          Map<String, List<CompilationError>> errorsByFile,
                          Map<String, List<CompilationWarning>> warningsByFile,
                          Map<CompilationError.ErrorType, Integer> errorTypeStats) {
            this.totalErrors = totalErrors;
            this.totalWarnings = totalWarnings;
            this.errorsByFile = errorsByFile;
            this.warningsByFile = warningsByFile;
            this.errorTypeStats = errorTypeStats;
        }
    }
    
    /**
     * 编译失败异常
     */
    public static class CompilationFailedException extends RuntimeException {
        private final CompilationError error;
        
        public CompilationFailedException(String message, CompilationError error) {
            super(message);
            this.error = error;
        }
        
        public CompilationError getError() {
            return error;
        }
    }
    
    /**
     * 恢复点注册表
     */
    private static class RecoveryPoint {
        private static final List<SourceLocation> recoveryPoints = new ArrayList<>();
        
        public static void save(SourceLocation location) {
            recoveryPoints.add(location);
        }
    }
    
    /**
     * 占位符注册表
     */
    private static class PlaceholderRegistry {
        private static final Set<String> placeholders = new HashSet<>();
        
        public static void register(String name) {
            placeholders.add(name);
        }
    }
    
    /**
     * 类型恢复工具
     */
    private static class TypeRecovery {
        public static void attempt(CompilationError error) {
            // 类型恢复逻辑
        }
    }
}