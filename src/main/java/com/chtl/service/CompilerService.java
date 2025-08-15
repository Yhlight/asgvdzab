package com.chtl.service;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicLong;

import com.chtl.compiler.CHTLCompiler;
import com.chtl.model.CompilationResult;
import com.chtl.service.CompilerService.BatchCompileHandler;
import com.chtl.service.CompilerService.BatchCompileRequest;
import com.chtl.service.CompilerService.BatchCompileResponse;
import com.chtl.service.CompilerService.CacheClearHandler;
import com.chtl.service.CompilerService.CacheStatsHandler;
import com.chtl.service.CompilerService.CompileHandler;
import com.chtl.service.CompilerService.CompileRequest;
import com.chtl.service.CompilerService.CompileResponse;
import com.chtl.service.CompilerService.CompileResult;
import com.chtl.service.CompilerService.ErrorResponse;
import com.chtl.service.CompilerService.HealthCheckHandler;
import com.chtl.service.CompilerService.HealthStatus;
import com.chtl.service.CompilerService.IncrementalCompileHandler;
import com.chtl.service.CompilerService.IncrementalCompileRequest;
import com.chtl.service.CompilerService.IncrementalResult;
import com.chtl.service.CompilerService.ServiceStatistics;
import com.chtl.service.CompilerService.StatsHandler;
import com.chtl.service.CompilerService.ValidateHandler;
import com.chtl.service.CompilerService.ValidateRequest;
import com.chtl.service.CompilerService.ValidateResponse;

import java.util.function.Consumer;

import com.chtl.compiler.*;
import com.chtl.context.CompilationContext;
import com.chtl.core.error.GlobalErrorHandler;
import com.chtl.core.cache.CompilationCache;
import com.chtl.core.memory.StreamingFileProcessor;

import java.io.*;
import java.net.*;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;

import com.sun.net.httpserver.*;

/**
 * CHTL编译器服务
 * 提供HTTP接口的微服务化编译器
 */
public class CompilerService {
    private static final int DEFAULT_PORT = 8080;
    private static final String API_VERSION = "/api/v1";
    
    private final HttpServer server;
    private final CHTLCompiler compiler;
    private final ExecutorService executorService;
    private final CompilationCache cache;
    private final GlobalErrorHandler errorHandler;
    
    // 服务统计
    private final ServiceStatistics statistics = new ServiceStatistics();
    
    public CompilerService(int port) throws IOException {
        this.compiler = new CHTLCompiler();
        this.executorService = Executors.newCachedThreadPool();
        this.cache = CompilationCache.getInstance();
        this.errorHandler = GlobalErrorHandler.getInstance();
        
        // 创建HTTP服务器
        this.server = HttpServer.create(new InetSocketAddress(port), 0);
        setupRoutes();
        server.setExecutor(executorService);
    }
    
    /**
     * 设置路由
     */
    private void setupRoutes() {
        // 编译端点
        server.createContext(API_VERSION + "/compile", new CompileHandler());
        
        // 验证端点
        server.createContext(API_VERSION + "/validate", new ValidateHandler());
        
        // 增量编译端点
        server.createContext(API_VERSION + "/compile/incremental", new IncrementalCompileHandler());
        
        // 批量编译端点
        server.createContext(API_VERSION + "/compile/batch", new BatchCompileHandler());
        
        // 缓存管理端点
        server.createContext(API_VERSION + "/cache/clear", new CacheClearHandler());
        server.createContext(API_VERSION + "/cache/stats", new CacheStatsHandler());
        
        // 健康检查端点
        server.createContext(API_VERSION + "/health", new HealthCheckHandler());
        
        // 统计信息端点
        server.createContext(API_VERSION + "/stats", new StatsHandler());
    }
    
    /**
     * 启动服务
     */
    public void start() {
        server.start();
        System.out.println("CHTL编译器服务已启动，端口：" + server.getAddress().getPort());
    }
    
    /**
     * 停止服务
     */
    public void stop() {
        server.stop(0);
        executorService.shutdown();
        try {
            if (!executorService.awaitTermination(60, TimeUnit.SECONDS)) {
                executorService.shutdownNow();
            }
        } catch (InterruptedException e) {
            executorService.shutdownNow();
        }
    }
    
    /**
     * 编译处理器
     */
    public static class CompileHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            statistics.incrementRequests();
            long startTime = System.currentTimeMillis();
            
            try {
                if (!"POST".equals(exchange.getRequestMethod())) {
                    sendResponse(exchange, 405, "Method Not Allowed");
                    return;
                }
                
                // 解析请求
                CompileRequest request = parseCompileRequest(exchange);
                
                // 检查缓存
                Optional<String> cached = cache.getCompiledOutput(request.filePath, request.content);
                if (cached.isPresent()) {
                    statistics.incrementCacheHits();
                    sendJsonResponse(exchange, 200, new CompileResponse(true, cached.get()));
                    return;
                }
                
                // 执行编译
                CompilationContext context = new CompilationContext();
                context.setDebugMode(request.debug);
                
                CompilationResult result = compiler.compile(request.filePath, context);
                
                // 缓存结果
                if (result.isSuccess()) {
                    cache.putCompiledOutput(request.filePath, request.content, result.getOutput());
                }
                
                // 返回响应
                CompileResponse response = new CompileResponse(
                    result.isSuccess(),
                    result.getOutput(),
                    result.getErrors(),
                    result.getWarnings()
                );
                
                sendJsonResponse(exchange, result.isSuccess() ? 200 : 400, response);
                
            } catch (Exception e) {
                statistics.incrementErrors();
                sendErrorResponse(exchange, e);
            } finally {
                statistics.addResponseTime(System.currentTimeMillis() - startTime);
            }
        }
    }
    
    /**
     * 增量编译处理器
     */
    public static class IncrementalCompileHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            try {
                if (!"POST".equals(exchange.getRequestMethod())) {
                    sendResponse(exchange, 405, "Method Not Allowed");
                    return;
                }
                
                IncrementalCompileRequest request = parseIncrementalRequest(exchange);
                
                // 执行增量编译
                IncrementalCompiler incrementalCompiler = new IncrementalCompiler(compiler, cache);
                IncrementalResult result = incrementalCompiler.compile(request);
                
                sendJsonResponse(exchange, 200, result);
                
            } catch (Exception e) {
                sendErrorResponse(exchange, e);
            }
        }
    }
    
    /**
     * 批量编译处理器
     */
    public static class BatchCompileHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            try {
                if (!"POST".equals(exchange.getRequestMethod())) {
                    sendResponse(exchange, 405, "Method Not Allowed");
                    return;
                }
                
                BatchCompileRequest request = parseBatchRequest(exchange);
                
                // 并行批量编译
                List<CompletableFuture<CompileResult>> futures = new ArrayList<>();
                
                for (String filePath : request.files) {
                    futures.add(CompletableFuture.supplyAsync(() -> {
                        try {
                            CompilationResult result = compiler.compile(filePath);
                            return new CompileResult(filePath, result);
                        } catch (Exception e) {
                            return new CompileResult(filePath, e);
                        }
                    }, executorService));
                }
                
                // 等待所有编译完成
                CompletableFuture.allOf(futures.toArray(new CompletableFuture[0])).join();
                
                // 收集结果
                BatchCompileResponse response = new BatchCompileResponse();
                for (CompletableFuture<CompileResult> future : futures) {
                    response.results.add(future.get());
                }
                
                sendJsonResponse(exchange, 200, response);
                
            } catch (Exception e) {
                sendErrorResponse(exchange, e);
            }
        }
    }
    
    /**
     * 验证处理器
     */
    public static class ValidateHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            try {
                if (!"POST".equals(exchange.getRequestMethod())) {
                    sendResponse(exchange, 405, "Method Not Allowed");
                    return;
                }
                
                ValidateRequest request = parseValidateRequest(exchange);
                
                // 执行验证
                CompilationContext context = new CompilationContext();
                errorHandler.clear();
                
                // 只进行语法验证，不生成输出
                boolean isValid = validateContent(request.content, context);
                
                ValidateResponse response = new ValidateResponse(
                    isValid,
                    errorHandler.getSummary()
                );
                
                sendJsonResponse(exchange, 200, response);
                
            } catch (Exception e) {
                sendErrorResponse(exchange, e);
            }
        }
    }
    
    /**
     * 缓存清理处理器
     */
    public static class CacheClearHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            if (!"POST".equals(exchange.getRequestMethod())) {
                sendResponse(exchange, 405, "Method Not Allowed");
                return;
            }
            
            cache.clear();
            sendResponse(exchange, 200, "Cache cleared");
        }
    }
    
    /**
     * 缓存统计处理器
     */
    public static class CacheStatsHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            CompilationCache.CacheStatistics stats = cache.getStatistics();
            sendJsonResponse(exchange, 200, stats);
        }
    }
    
    /**
     * 健康检查处理器
     */
    public static class HealthCheckHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            HealthStatus status = new HealthStatus(
                "UP",
                System.currentTimeMillis(),
                Runtime.getRuntime().freeMemory(),
                Runtime.getRuntime().totalMemory()
            );
            sendJsonResponse(exchange, 200, status);
        }
    }
    
    /**
     * 统计信息处理器
     */
    public static class StatsHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            sendJsonResponse(exchange, 200, statistics);
        }
    }
    
    // 辅助方法
    
    private void sendResponse(HttpExchange exchange, int code, String response) throws IOException {
        byte[] bytes = response.getBytes("UTF-8");
        exchange.sendResponseHeaders(code, bytes.length);
        try (OutputStream os = exchange.getResponseBody()) {
            os.write(bytes);
        }
    }
    
    private void sendJsonResponse(HttpExchange exchange, int code, Object response) throws IOException {
        exchange.getResponseHeaders().set("Content-Type", "application/json");
        String json = toJson(response);
        sendResponse(exchange, code, json);
    }
    
    private void sendErrorResponse(HttpExchange exchange, Exception e) throws IOException {
        ErrorResponse error = new ErrorResponse(e.getMessage(), e.getClass().getSimpleName());
        sendJsonResponse(exchange, 500, error);
    }
    
    private CompileRequest parseCompileRequest(HttpExchange exchange) throws IOException {
        // 简化的JSON解析
        String body = new String(exchange.getRequestBody().readAllBytes());
        // 实际应使用JSON库
        CompileRequest request = new CompileRequest();
        request.filePath = extractJsonField(body, "filePath");
        request.content = extractJsonField(body, "content");
        request.debug = Boolean.parseBoolean(extractJsonField(body, "debug"));
        return request;
    }
    
    private String extractJsonField(String json, String field) {
        // 简化实现
        int start = json.indexOf("\"" + field + "\":");
        if (start == -1) return "";
        start = json.indexOf("\"", start + field.length() + 3) + 1;
        int end = json.indexOf("\"", start);
        return json.substring(start, end);
    }
    
    private String toJson(Object obj) {
        // 简化的JSON序列化
        // 实际应使用JSON库
        return obj.toString();
    }
    
    private boolean validateContent(String content, CompilationContext context) {
        // 简化的验证逻辑
        return true;
    }
    
    private IncrementalCompileRequest parseIncrementalRequest(HttpExchange exchange) throws IOException {
        // 简化实现
        return new IncrementalCompileRequest();
    }
    
    private BatchCompileRequest parseBatchRequest(HttpExchange exchange) throws IOException {
        // 简化实现
        return new BatchCompileRequest();
    }
    
    private ValidateRequest parseValidateRequest(HttpExchange exchange) throws IOException {
        // 简化实现
        return new ValidateRequest();
    }
    
    // 请求/响应类
    
    private static class CompileRequest {
        String filePath;
        String content;
        boolean debug;
    }
    
    private static class CompileResponse {
        final boolean success;
        final String output;
        final List<String> errors;
        final List<String> warnings;
        
        CompileResponse(boolean success, String output) {
            this(success, output, new ArrayList<>(), new ArrayList<>());
        }
        
        CompileResponse(boolean success, String output, List<String> errors, List<String> warnings) {
            this.success = success;
            this.output = output;
            this.errors = errors;
            this.warnings = warnings;
        }
    }
    
    public static class IncrementalCompileRequest {
        List<String> changedFiles;
        Map<String, String> fileContents;
    }
    
    public static class IncrementalResult {
        List<String> recompiledFiles;
        Map<String, String> outputs;
        boolean success;
    }
    
    private static class BatchCompileRequest {
        List<String> files;
    }
    
    private static class BatchCompileResponse {
        List<CompileResult> results = new ArrayList<>();
    }
    
    private static class CompileResult {
        final String file;
        final boolean success;
        final String output;
        final String error;
        
        CompileResult(String file, CompilationResult result) {
            this.file = file;
            this.success = result.isSuccess();
            this.output = result.getOutput();
            this.error = null;
        }
        
        CompileResult(String file, Exception e) {
            this.file = file;
            this.success = false;
            this.output = null;
            this.error = e.getMessage();
        }
    }
    
    private static class ValidateRequest {
        String content;
    }
    
    private static class ValidateResponse {
        final boolean valid;
        final GlobalErrorHandler.ErrorSummary errors;
        
        ValidateResponse(boolean valid, GlobalErrorHandler.ErrorSummary errors) {
            this.valid = valid;
            this.errors = errors;
        }
    }
    
    private static class ErrorResponse {
        final String message;
        final String type;
        
        ErrorResponse(String message, String type) {
            this.message = message;
            this.type = type;
        }
    }
    
    private static class HealthStatus {
        final String status;
        final long timestamp;
        final long freeMemory;
        final long totalMemory;
        
        HealthStatus(String status, long timestamp, long freeMemory, long totalMemory) {
            this.status = status;
            this.timestamp = timestamp;
            this.freeMemory = freeMemory;
            this.totalMemory = totalMemory;
        }
    }
    
    /**
     * 服务统计
     */
    private static class ServiceStatistics {
        private final AtomicLong totalRequests = new AtomicLong(0);
        private final AtomicLong cacheHits = new AtomicLong(0);
        private final AtomicLong errors = new AtomicLong(0);
        private final AtomicLong totalResponseTime = new AtomicLong(0);
        
        void incrementRequests() {
            totalRequests.incrementAndGet();
        }
        
        void incrementCacheHits() {
            cacheHits.incrementAndGet();
        }
        
        void incrementErrors() {
            errors.incrementAndGet();
        }
        
        void addResponseTime(long time) {
            totalResponseTime.addAndGet(time);
        }
        
        double getAverageResponseTime() {
            long requests = totalRequests.get();
            return requests > 0 ? (double) totalResponseTime.get() / requests : 0;
        }
        
        double getCacheHitRate() {
            long requests = totalRequests.get();
            return requests > 0 ? (double) cacheHits.get() / requests : 0;
        }
    }
    
    /**
     * 主方法
     */
    public static void main(String[] args) {
        int port = args.length > 0 ? Integer.parseInt(args[0]) : DEFAULT_PORT;
        
        try {
            CompilerService service = new CompilerService(port);
            service.start();
            
            // 添加关闭钩子
            Runtime.getRuntime().addShutdownHook(new Thread(service::stop));
            
        } catch (Exception e) {
            System.err.println("启动编译器服务失败: " + e.getMessage());
            e.printStackTrace();
        }
    }
}