package com.chtl.core.cache;
import java.io.IOException;
import java.io.Serializable;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

import com.chtl.compiler.chtl.token.CHTLToken;
import com.chtl.core.cache.CompilationCache.ASTCacheData;
import com.chtl.core.cache.CompilationCache.CacheEntry;
import com.chtl.core.cache.CompilationCache.CacheStatistics;
import com.chtl.core.cache.CompilationCache.DependenciesCacheData;
import com.chtl.core.cache.CompilationCache.OutputCacheData;
import com.chtl.core.cache.CompilationCache.TokensCacheData;


import com.chtl.ast.CHTLASTNode;
import com.chtl.lexer.CHTLToken;

import java.io.*;
import java.nio.file.*;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

/**
 * 编译缓存系统
 * 提供多级缓存机制以提升编译性能
 */
public class CompilationCache {
    private static CompilationCache instance;
    
    // 内存缓存
    private final Map<String, CacheEntry> memoryCache = new ConcurrentHashMap<>();
    
    // 磁盘缓存
    private final Path diskCacheDir;
    private final boolean diskCacheEnabled;
    
    // 缓存统计
    private final CacheStatistics statistics = new CacheStatistics();
    
    // 缓存配置
    private long maxMemoryCacheSize = 100 * 1024 * 1024; // 100MB
    private long maxDiskCacheSize = 1024 * 1024 * 1024; // 1GB
    private long cacheExpirationTime = 24 * 60 * 60 * 1000; // 24小时
    
    // 锁管理
    private final Map<String, ReadWriteLock> fileLocks = new ConcurrentHashMap<>();
    
    private CompilationCache() {
        this.diskCacheDir = Paths.get(System.getProperty("user.home"), ".chtl", "cache");
        this.diskCacheEnabled = initializeDiskCache();
    }
    
    public static synchronized CompilationCache getInstance() {
        if (instance == null) {
            instance = new CompilationCache();
        }
        return instance;
    }
    
    /**
     * 获取词法分析结果缓存
     */
    public Optional<List<CHTLToken>> getTokens(String filePath, String content) {
        String key = generateKey(filePath, content, "tokens");
        return getFromCache(key, TokensCacheData.class)
            .map(data -> data.tokens);
    }
    
    /**
     * 缓存词法分析结果
     */
    public void putTokens(String filePath, String content, List<CHTLToken> tokens) {
        String key = generateKey(filePath, content, "tokens");
        TokensCacheData data = new TokensCacheData(tokens);
        putToCache(key, data);
    }
    
    /**
     * 获取AST缓存
     */
    public Optional<CHTLASTNode> getAST(String filePath, String content) {
        String key = generateKey(filePath, content, "ast");
        return getFromCache(key, ASTCacheData.class)
            .map(data -> data.ast);
    }
    
    /**
     * 缓存AST
     */
    public void putAST(String filePath, String content, CHTLASTNode ast) {
        String key = generateKey(filePath, content, "ast");
        ASTCacheData data = new ASTCacheData(ast);
        putToCache(key, data);
    }
    
    /**
     * 获取编译结果缓存
     */
    public Optional<String> getCompiledOutput(String filePath, String content) {
        String key = generateKey(filePath, content, "output");
        return getFromCache(key, OutputCacheData.class)
            .map(data -> data.output);
    }
    
    /**
     * 缓存编译结果
     */
    public void putCompiledOutput(String filePath, String content, String output) {
        String key = generateKey(filePath, content, "output");
        OutputCacheData data = new OutputCacheData(output);
        putToCache(key, data);
    }
    
    /**
     * 获取文件依赖信息
     */
    public Optional<Set<String>> getDependencies(String filePath) {
        String key = generateKey(filePath, "", "deps");
        return getFromCache(key, DependenciesCacheData.class)
            .map(data -> data.dependencies);
    }
    
    /**
     * 缓存文件依赖信息
     */
    public void putDependencies(String filePath, Set<String> dependencies) {
        String key = generateKey(filePath, "", "deps");
        DependenciesCacheData data = new DependenciesCacheData(dependencies);
        putToCache(key, data);
    }
    
    /**
     * 检查文件是否需要重新编译
     */
    public boolean needsRecompilation(String filePath) {
        try {
            Path path = Paths.get(filePath);
            long lastModified = Files.getLastModifiedTime(path).toMillis();
            
            // 检查文件本身的缓存
            String content = Files.readString(path);
            String key = generateKey(filePath, content, "output");
            Optional<CacheEntry> entry = getEntry(key);
            
            if (entry.isEmpty() || entry.get().timestamp < lastModified) {
                return true;
            }
            
            // 检查依赖文件
            Optional<Set<String>> deps = getDependencies(filePath);
            if (deps.isPresent()) {
                for (String dep : deps.get()) {
                    if (needsRecompilation(dep)) {
                        return true;
                    }
                }
            }
            
            return false;
        } catch (IOException e) {
            return true; // 出错时安全起见需要重新编译
        }
    }
    
    /**
     * 清除特定文件的缓存
     */
    public void invalidate(String filePath) {
        // 清除内存缓存
        memoryCache.entrySet().removeIf(entry -> 
            entry.getKey().startsWith(filePath + ":"));
        
        // 清除磁盘缓存
        if (diskCacheEnabled) {
            try {
                Files.walk(diskCacheDir)
                    .filter(Files::isRegularFile)
                    .filter(path -> path.getFileName().toString().startsWith(
                        generateFilePrefix(filePath)))
                    .forEach(path -> {
                        try {
                            Files.delete(path);
                        } catch (IOException e) {
                            // 忽略删除失败
                        }
                    });
            } catch (IOException e) {
                // 忽略错误
            }
        }
    }
    
    /**
     * 清除所有缓存
     */
    public void clear() {
        memoryCache.clear();
        
        if (diskCacheEnabled) {
            try {
                Files.walk(diskCacheDir)
                    .sorted(Comparator.reverseOrder())
                    .forEach(path -> {
                        try {
                            Files.delete(path);
                        } catch (IOException e) {
                            // 忽略删除失败
                        }
                    });
            } catch (IOException e) {
                // 忽略错误
            }
        }
        
        statistics.reset();
    }
    
    /**
     * 获取缓存统计信息
     */
    public CacheStatistics getStatistics() {
        return statistics;
    }
    
    /**
     * 从缓存获取数据
     */
    private <T extends CacheData> Optional<T> getFromCache(String key, Class<T> type) {
        statistics.incrementRequests();
        
        // 先从内存缓存获取
        CacheEntry entry = memoryCache.get(key);
        if (entry != null && !entry.isExpired()) {
            statistics.incrementMemoryHits();
            return Optional.of(type.cast(entry.data));
        }
        
        // 从磁盘缓存获取
        if (diskCacheEnabled) {
            Optional<T> diskData = loadFromDisk(key, type);
            if (diskData.isPresent()) {
                statistics.incrementDiskHits();
                // 加载到内存缓存
                memoryCache.put(key, new CacheEntry(diskData.get()));
                return diskData;
            }
        }
        
        statistics.incrementMisses();
        return Optional.empty();
    }
    
    /**
     * 获取缓存条目
     */
    private Optional<CacheEntry> getEntry(String key) {
        CacheEntry entry = memoryCache.get(key);
        if (entry != null && !entry.isExpired()) {
            return Optional.of(entry);
        }
        return Optional.empty();
    }
    
    /**
     * 放入缓存
     */
    private void putToCache(String key, CacheData data) {
        // 检查内存缓存大小
        if (getCurrentMemorySize() > maxMemoryCacheSize) {
            evictFromMemory();
        }
        
        // 放入内存缓存
        memoryCache.put(key, new CacheEntry(data));
        
        // 异步写入磁盘
        if (diskCacheEnabled) {
            saveToDiskAsync(key, data);
        }
    }
    
    /**
     * 生成缓存键
     */
    private String generateKey(String filePath, String content, String type) {
        String contentHash = content.isEmpty() ? "" : hash(content);
        return filePath + ":" + contentHash + ":" + type;
    }
    
    /**
     * 生成文件前缀
     */
    private String generateFilePrefix(String filePath) {
        return hash(filePath);
    }
    
    /**
     * 计算哈希值
     */
    private String hash(String input) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            byte[] hash = md.digest(input.getBytes());
            StringBuilder sb = new StringBuilder();
            for (byte b : hash) {
                sb.append(String.format("%02x", b));
            }
            return sb.toString().substring(0, 16); // 使用前16个字符
        } catch (NoSuchAlgorithmException e) {
            return String.valueOf(input.hashCode());
        }
    }
    
    /**
     * 初始化磁盘缓存
     */
    private boolean initializeDiskCache() {
        try {
            Files.createDirectories(diskCacheDir);
            return true;
        } catch (IOException e) {
            return false;
        }
    }
    
    /**
     * 从磁盘加载缓存
     */
    private <T extends CacheData> Optional<T> loadFromDisk(String key, Class<T> type) {
        Path cachePath = diskCacheDir.resolve(hash(key) + ".cache");
        
        if (!Files.exists(cachePath)) {
            return Optional.empty();
        }
        
        ReadWriteLock lock = fileLocks.computeIfAbsent(key, k -> new ReentrantReadWriteLock());
        lock.readLock().lock();
        try {
            try (ObjectInputStream ois = new ObjectInputStream(
                    new BufferedInputStream(Files.newInputStream(cachePath)))) {
                CacheEntry entry = (CacheEntry) ois.readObject();
                if (!entry.isExpired() && type.isInstance(entry.data)) {
                    return Optional.of(type.cast(entry.data));
                }
            }
        } catch (IOException | ClassNotFoundException e) {
            // 缓存损坏，删除
            try {
                Files.delete(cachePath);
            } catch (IOException ignored) {}
        } finally {
            lock.readLock().unlock();
        }
        
        return Optional.empty();
    }
    
    /**
     * 异步保存到磁盘
     */
    private void saveToDiskAsync(String key, CacheData data) {
        // 使用单独的线程池处理磁盘IO
        CacheIOExecutor.submit(() -> saveToDisk(key, data));
    }
    
    /**
     * 保存到磁盘
     */
    private void saveToDisk(String key, CacheData data) {
        Path cachePath = diskCacheDir.resolve(hash(key) + ".cache");
        
        ReadWriteLock lock = fileLocks.computeIfAbsent(key, k -> new ReentrantReadWriteLock());
        lock.writeLock().lock();
        try {
            try (ObjectOutputStream oos = new ObjectOutputStream(
                    new BufferedOutputStream(Files.newOutputStream(cachePath)))) {
                oos.writeObject(new CacheEntry(data));
            }
        } catch (IOException e) {
            // 忽略写入错误
        } finally {
            lock.writeLock().unlock();
        }
    }
    
    /**
     * 获取当前内存缓存大小
     */
    private long getCurrentMemorySize() {
        // 简化实现，实际应该计算对象大小
        return memoryCache.size() * 1024; // 假设每个条目1KB
    }
    
    /**
     * 从内存中驱逐缓存
     */
    private void evictFromMemory() {
        // LRU驱逐策略
        List<Map.Entry<String, CacheEntry>> entries = new ArrayList<>(memoryCache.entrySet());
        entries.sort(Comparator.comparingLong(e -> e.getValue().lastAccessed));
        
        // 驱逐最旧的20%
        int evictCount = entries.size() / 5;
        for (int i = 0; i < evictCount; i++) {
            memoryCache.remove(entries.get(i).getKey());
        }
    }
    
    /**
     * 缓存条目
     */
    private static class CacheEntry implements Serializable {
        private static final long serialVersionUID = 1L;
        
        final CacheData data;
        final long timestamp;
        long lastAccessed;
        
        CacheEntry(CacheData data) {
            this.data = data;
            this.timestamp = System.currentTimeMillis();
            this.lastAccessed = this.timestamp;
        }
        
        boolean isExpired() {
            return System.currentTimeMillis() - timestamp > 
                CompilationCache.getInstance().cacheExpirationTime;
        }
    }
    
    /**
     * 缓存数据基类
     */
    private interface CacheData extends Serializable {
    }
    
    /**
     * Token缓存数据
     */
    private static class TokensCacheData implements CacheData {
        private static final long serialVersionUID = 1L;
        final List<CHTLToken> tokens;
        
        TokensCacheData(List<CHTLToken> tokens) {
            this.tokens = new ArrayList<>(tokens);
        }
    }
    
    /**
     * AST缓存数据
     */
    private static class ASTCacheData implements CacheData {
        private static final long serialVersionUID = 1L;
        final CHTLASTNode ast;
        
        ASTCacheData(CHTLASTNode ast) {
            this.ast = ast;
        }
    }
    
    /**
     * 输出缓存数据
     */
    private static class OutputCacheData implements CacheData {
        private static final long serialVersionUID = 1L;
        final String output;
        
        OutputCacheData(String output) {
            this.output = output;
        }
    }
    
    /**
     * 依赖缓存数据
     */
    private static class DependenciesCacheData implements CacheData {
        private static final long serialVersionUID = 1L;
        final Set<String> dependencies;
        
        DependenciesCacheData(Set<String> dependencies) {
            this.dependencies = new HashSet<>(dependencies);
        }
    }
    
    /**
     * 缓存统计
     */
    public static class CacheStatistics {
        private long totalRequests = 0;
        private long memoryHits = 0;
        private long diskHits = 0;
        private long misses = 0;
        
        public synchronized void incrementRequests() {
            totalRequests++;
        }
        
        public synchronized void incrementMemoryHits() {
            memoryHits++;
        }
        
        public synchronized void incrementDiskHits() {
            diskHits++;
        }
        
        public synchronized void incrementMisses() {
            misses++;
        }
        
        public synchronized void reset() {
            totalRequests = 0;
            memoryHits = 0;
            diskHits = 0;
            misses = 0;
        }
        
        public double getHitRate() {
            if (totalRequests == 0) return 0;
            return (double) (memoryHits + diskHits) / totalRequests;
        }
        
        public long getTotalRequests() {
            return totalRequests;
        }
        
        public long getMemoryHits() {
            return memoryHits;
        }
        
        public long getDiskHits() {
            return diskHits;
        }
        
        public long getMisses() {
            return misses;
        }
    }
}