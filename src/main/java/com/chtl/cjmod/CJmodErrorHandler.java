package com.chtl.cjmod;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.function.Consumer;
import com.chtl.compiler.cjmod.CJmodManager;
import com.chtl.compiler.debug.ErrorStatistics;
import com.chtl.core.error.ErrorListener;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;
import java.io.*;

/**
 * CJMOD错误处理机制
 * 提供优雅降级、崩溃恢复和详细的错误追踪
 */
public class CJmodErrorHandler {
    
    /**
     * 错误级别
     */
    public enum ErrorLevel {
        DEBUG(0),     // 调试信息
        INFO(1),      // 信息
        WARNING(2),   // 警告
        ERROR(3),     // 错误
        CRITICAL(4),  // 严重错误
        FATAL(5);     // 致命错误
        
        private final int severity;
        
        ErrorLevel(int severity) {
            this.severity = severity;
        }
        
        public int getSeverity() {
            return severity;
        }
    }
    
    /**
     * 错误类别
     */
    public enum ErrorCategory {
        INITIALIZATION,      // 初始化错误
        LOADING,            // 加载错误
        EXECUTION,          // 执行错误
        SECURITY,           // 安全错误
        RESOURCE,           // 资源错误
        DEPENDENCY,         // 依赖错误
        CONFIGURATION,      // 配置错误
        COMMUNICATION,      // 通信错误
        INTERNAL            // 内部错误
    }
    
    /**
     * 错误信息
     */
    public static class ErrorInfo {
        private final String moduleId;
        private final ErrorLevel level;
        private final ErrorCategory category;
        private final String message;
        private final Throwable cause;
        private final LocalDateTime timestamp;
        private final Map<String, Object> context;
        private final StackTraceElement[] stackTrace;
        
        public ErrorInfo(String moduleId, ErrorLevel level, ErrorCategory category,
                        String message, Throwable cause) {
            this.moduleId = moduleId;
            this.level = level;
            this.category = category;
            this.message = message;
            this.cause = cause;
            this.timestamp = LocalDateTime.now();
            this.context = new HashMap<Object, Object>();
            this.stackTrace = Thread.currentThread().getStackTrace();
        }
        
        public void addContext(String key, Object value) {
            context.put(key, value);
        }
        
        // Getters
        public String getModuleId() { return moduleId; }
        public ErrorLevel getLevel() { return level; }
        public ErrorCategory getCategory() { return category; }
        public String getMessage() { return message; }
        public Throwable getCause() { return cause; }
        public LocalDateTime getTimestamp() { return timestamp; }
        public Map<String, Object> getContext() { return context; }
        public StackTraceElement[] getStackTrace() { return stackTrace; }
        
        @Override
        public String toString() {
            return String.format("[%s][%s][%s] %s: %s",
                timestamp, level, category, moduleId, message);
        }
    }
    
    /**
     * 错误恢复策略
     */
    public interface RecoveryStrategy {
        /**
         * 尝试恢复
         * @return 是否成功恢复
         */
        boolean recover(ErrorInfo error);
        
        /**
         * 获取策略描述
         */
        String getDescription();
    }
    
    /**
     * 错误监听器
     */
    public interface ErrorListener {
        void onError(ErrorInfo error);
        void onRecovery(ErrorInfo error, RecoveryStrategy strategy);
        void onFatalError(ErrorInfo error);
    }
    
    // 单例实例
    private static CJmodErrorHandler instance;
    
    // 错误日志
    private final LinkedBlockingQueue<ErrorInfo> errorLog;
    private final Map<ErrorCategory, List<RecoveryStrategy>> recoveryStrategies;
    private final List<ErrorListener> listeners;
    private final ExecutorService executorService;
    
    // 错误统计
    private final Map<String, ErrorStatistics> moduleStatistics;
    private final AtomicInteger totalErrors;
    private final AtomicInteger recoveredErrors;
    
    // 配置
    private ErrorLevel minLogLevel = ErrorLevel.INFO;
    private int maxErrorLogSize = 10000;
    private boolean enableStackTrace = true;
    private Path errorLogPath;
    
    /**
     * 错误统计
     */
    public static class ErrorStatistics {
        private final String moduleId;
        private final Map<ErrorLevel, AtomicInteger> levelCounts;
        private final Map<ErrorCategory, AtomicInteger> categoryCounts;
        private final AtomicInteger totalCount;
        private final AtomicInteger recoveredCount;
        private volatile LocalDateTime lastErrorTime;
        
        public ErrorStatistics(String moduleId) {
            this.moduleId = moduleId;
            this.levelCounts = new ConcurrentHashMap<Object, Object>();
            this.categoryCounts = new ConcurrentHashMap<Object, Object>();
            this.totalCount = new AtomicInteger(0);
            this.recoveredCount = new AtomicInteger(0);
            
            // 初始化计数器
            for (ErrorLevel level : ErrorLevel.values()) {
                levelCounts.put(level, new AtomicInteger(0));
            }
            for (ErrorCategory category : ErrorCategory.values()) {
                categoryCounts.put(category, new AtomicInteger(0));
            }
        }
        
        public void recordError(ErrorInfo error) {
            levelCounts.get(error.getLevel()).incrementAndGet();
            categoryCounts.get(error.getCategory()).incrementAndGet();
            totalCount.incrementAndGet();
            lastErrorTime = error.getTimestamp();
        }
        
        public void recordRecovery() {
            recoveredCount.incrementAndGet();
        }
        
        // Getters
        public String getModuleId() { return moduleId; }
        public int getTotalErrors() { return totalCount.get(); }
        public int getRecoveredErrors() { return recoveredCount.get(); }
        public LocalDateTime getLastErrorTime() { return lastErrorTime; }
        
        public Map<ErrorLevel, Integer> getLevelDistribution() {
            Map<ErrorLevel, Integer> result = new HashMap<Object, Object>();
            levelCounts.forEach((k, v) -> result.put(k, v.get()));
            return result;
        }
        
        public Map<ErrorCategory, Integer> getCategoryDistribution() {
            Map<ErrorCategory, Integer> result = new HashMap<Object, Object>();
            categoryCounts.forEach((k, v) -> result.put(k, v.get()));
            return result;
        }
    }
    
    private CJmodErrorHandler() {
        this.errorLog = new LinkedBlockingQueue<>(maxErrorLogSize);
        this.recoveryStrategies = new ConcurrentHashMap<Object, Object>();
        this.listeners = new CopyOnWriteArrayList<Object>();
        this.moduleStatistics = new ConcurrentHashMap<Object, Object>();
        this.totalErrors = new AtomicInteger(0);
        this.recoveredErrors = new AtomicInteger(0);
        
        this.executorService = Executors.newCachedThreadPool(r -> {
            Thread t = new Thread(r, "CJmod-ErrorHandler");
            t.setDaemon(true);
            return t;
        });
        
        // 注册默认恢复策略
        registerDefaultRecoveryStrategies();
    }
    
    public static synchronized CJmodErrorHandler getInstance() {
        if (instance == null) {
            instance = new CJmodErrorHandler();
        }
        return instance;
    }
    
    /**
     * 记录错误
     */
    public void logError(String moduleId, ErrorLevel level, ErrorCategory category,
                        String message, Throwable cause) {
        
        ErrorInfo error = new ErrorInfo(moduleId, level, category, message, cause);
        
        // 添加上下文信息
        error.addContext("thread", Thread.currentThread().getName());
        error.addContext("classLoader", Thread.currentThread().getContextClassLoader());
        
        // 记录到日志
        if (level.getSeverity() >= minLogLevel.getSeverity()) {
            if (!errorLog.offer(error)) {
                // 日志已满，移除最旧的
                errorLog.poll();
                errorLog.offer(error);
            }
        }
        
        // 更新统计
        totalErrors.incrementAndGet();
        moduleStatistics.computeIfAbsent(moduleId, k -> new ErrorStatistics(k))
            .recordError(error);
        
        // 异步处理
        executorService.submit(() -> handleError(error));
    }
    
    /**
     * 处理错误
     */
    private void handleError(ErrorInfo error) {
        // 通知监听器
        for (ErrorListener listener : listeners) {
            try {
                listener.onError(error);
            } catch (Exception e) {
                // 防止监听器异常影响错误处理
                e.printStackTrace();
            }
        }
        
        // 写入日志文件
        if (errorLogPath != null) {
            writeErrorToFile(error);
        }
        
        // 尝试恢复
        if (error.getLevel() != ErrorLevel.FATAL) {
            attemptRecovery(error);
        } else {
            // 致命错误
            handleFatalError(error);
        }
    }
    
    /**
     * 尝试恢复
     */
    private void attemptRecovery(ErrorInfo error) {
        List<RecoveryStrategy> strategies = recoveryStrategies.get(error.getCategory());
        if (strategies == null || strategies.isEmpty()) {
            return;
        }
        
        for (RecoveryStrategy strategy : strategies) {
            try {
                if (strategy.recover(error)) {
                    // 恢复成功
                    recoveredErrors.incrementAndGet();
                    moduleStatistics.get(error.getModuleId()).recordRecovery();
                    
                    // 通知监听器
                    for (ErrorListener listener : listeners) {
                        try {
                            listener.onRecovery(error, strategy);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                    
                    return;
                }
            } catch (Exception e) {
                // 恢复策略失败
                logError(error.getModuleId(), ErrorLevel.ERROR, ErrorCategory.INTERNAL,
                    "Recovery strategy failed: " + strategy.getDescription(), e);
            }
        }
    }
    
    /**
     * 处理致命错误
     */
    private void handleFatalError(ErrorInfo error) {
        // 通知监听器
        for (ErrorListener listener : listeners) {
            try {
                listener.onFatalError(error);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        
        // 尝试卸载模块
        try {
            CJmodManager.getInstance().unloadModule(error.getModuleId()).get();
        } catch (Exception e) {
            // 卸载失败
            e.printStackTrace();
        }
    }
    
    /**
     * 写入错误到文件
     */
    private void writeErrorToFile(ErrorInfo error) {
        try {
            String filename = String.format("error_%s_%s.log",
                error.getModuleId(),
                error.getTimestamp().toString().replace(":", "-")
            );
            
            Path errorFile = errorLogPath.resolve(filename);
            
            try (PrintWriter writer = new PrintWriter(Files.newBufferedWriter(errorFile))) {
                writer.println("=== CJMOD Error Report ===");
                writer.println("Module: " + error.getModuleId());
                writer.println("Time: " + error.getTimestamp());
                writer.println("Level: " + error.getLevel());
                writer.println("Category: " + error.getCategory());
                writer.println("Message: " + error.getMessage());
                writer.println();
                
                if (error.getCause() != null) {
                    writer.println("=== Exception ===");
                    error.getCause().printStackTrace(writer);
                    writer.println();
                }
                
                if (enableStackTrace) {
                    writer.println("=== Stack Trace ===");
                    for (StackTraceElement element : error.getStackTrace()) {
                        writer.println("  at " + element);
                    }
                    writer.println();
                }
                
                writer.println("=== Context ===");
                for (Map.Entry<String, Object> entry : error.getContext().entrySet()) {
                    writer.println(entry.getKey() + ": " + entry.getValue());
                }
            }
            
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    /**
     * 注册默认恢复策略
     */
    private void registerDefaultRecoveryStrategies() {
        // 初始化错误恢复
        registerRecoveryStrategy(ErrorCategory.INITIALIZATION, new RecoveryStrategy() {
            @Override
            public boolean recover(ErrorInfo error) {
                // 尝试重新初始化模块
                try {
                    CJmodManager manager = CJmodManager.getInstance();
                    manager.reloadModule(error.getModuleId()).get();
                    return true;
                } catch (Exception e) {
                    return false;
                }
            }
            
            @Override
            public String getDescription() {
                return "Reload module";
            }
        });
        
        // 资源错误恢复
        registerRecoveryStrategy(ErrorCategory.RESOURCE, new RecoveryStrategy() {
            @Override
            public boolean recover(ErrorInfo error) {
                // 清理资源并重试
                System.gc();
                return true;
            }
            
            @Override
            public String getDescription() {
                return "Clean up resources";
            }
        });
        
        // 依赖错误恢复
        registerRecoveryStrategy(ErrorCategory.DEPENDENCY, new RecoveryStrategy() {
            @Override
            public boolean recover(ErrorInfo error) {
                // 尝试重新解析依赖
                try {
                    // TODO: 实现依赖重新解析
                    return false;
                } catch (Exception e) {
                    return false;
                }
            }
            
            @Override
            public String getDescription() {
                return "Resolve dependencies";
            }
        });
    }
    
    /**
     * 注册恢复策略
     */
    public void registerRecoveryStrategy(ErrorCategory category, RecoveryStrategy strategy) {
        recoveryStrategies.computeIfAbsent(category, k -> new CopyOnWriteArrayList<Object>())
            .add(strategy);
    }
    
    /**
     * 添加错误监听器
     */
    public void addErrorListener(ErrorListener listener) {
        listeners.add(listener);
    }
    
    /**
     * 移除错误监听器
     */
    public void removeErrorListener(ErrorListener listener) {
        listeners.remove(listener);
    }
    
    /**
     * 获取错误统计
     */
    public ErrorStatistics getStatistics(String moduleId) {
        return moduleStatistics.get(moduleId);
    }
    
    /**
     * 获取全局统计
     */
    public Map<String, Object> getGlobalStatistics() {
        Map<String, Object> stats = new HashMap<Object, Object>();
        stats.put("totalErrors", totalErrors.get());
        stats.put("recoveredErrors", recoveredErrors.get());
        stats.put("recoveryRate", 
            totalErrors.get() > 0 ? 
            (double) recoveredErrors.get() / totalErrors.get() : 0.0);
        stats.put("moduleCount", moduleStatistics.size());
        
        // 按错误数量排序的模块列表
        List<String> topErrorModules = moduleStatistics.entrySet().stream()
            .sorted((a, b) -> Integer.compare(
                b.getValue().getTotalErrors(),
                a.getValue().getTotalErrors()
            ))
            .limit(10)
            .map(Map.Entry::getKey)
            .toList();
        
        stats.put("topErrorModules", topErrorModules);
        
        return stats;
    }
    
    /**
     * 获取最近的错误
     */
    public List<ErrorInfo> getRecentErrors(int count) {
        List<ErrorInfo> recent = new ArrayList<Object>();
        errorLog.drainTo(recent, count);
        // 将错误放回队列
        errorLog.addAll(recent);
        return recent;
    }
    
    /**
     * 配置错误处理器
     */
    public void configure(Consumer<ErrorHandlerConfig> configurer) {
        ErrorHandlerConfig config = new ErrorHandlerConfig();
        configurer.accept(config);
        
        this.minLogLevel = config.minLogLevel;
        this.maxErrorLogSize = config.maxErrorLogSize;
        this.enableStackTrace = config.enableStackTrace;
        this.errorLogPath = config.errorLogPath;
        
        if (errorLogPath != null) {
            try {
                Files.createDirectories(errorLogPath);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
    
    /**
     * 错误处理器配置
     */
    public static class ErrorHandlerConfig {
        private ErrorLevel minLogLevel = ErrorLevel.INFO;
        private int maxErrorLogSize = 10000;
        private boolean enableStackTrace = true;
        private Path errorLogPath;
        
        public ErrorHandlerConfig setMinLogLevel(ErrorLevel level) {
            this.minLogLevel = level;
            return this;
        }
        
        public ErrorHandlerConfig setMaxErrorLogSize(int size) {
            this.maxErrorLogSize = size;
            return this;
        }
        
        public ErrorHandlerConfig setEnableStackTrace(boolean enable) {
            this.enableStackTrace = enable;
            return this;
        }
        
        public ErrorHandlerConfig setErrorLogPath(Path path) {
            this.errorLogPath = path;
            return this;
        }
    }
    
    /**
     * 关闭错误处理器
     */
    public void shutdown() {
        executorService.shutdown();
        try {
            executorService.awaitTermination(10, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            executorService.shutdownNow();
        }
    }
}
