package com.chtl.cjmod;

import java.util.*;
import java.util.concurrent.*;
import java.io.*;
import java.nio.file.*;

/**
 * CJMOD全局状态管理器
 * 提供线程安全的全局状态管理
 * 支持持久化和跨模块共享
 */
public class CJmodGlobalStateManager {
    
    private static CJmodGlobalStateManager instance;
    
    // 全局状态存储
    private final ConcurrentHashMap<String, StateEntry> globalStates;
    
    // 模块私有状态
    private final ConcurrentHashMap<String, ConcurrentHashMap<String, StateEntry>> moduleStates;
    
    // 状态监听器
    private final ConcurrentHashMap<String, List<StateChangeListener>> listeners;
    
    // 持久化配置
    private Path persistencePath;
    private boolean autoPersist = false;
    private final ScheduledExecutorService persistenceExecutor;
    
    /**
     * 状态条目
     */
    public static class StateEntry {
        private final Object value;
        private final long timestamp;
        private final String owner;
        private final Map<String, Object> metadata;
        private volatile int accessCount;
        
        public StateEntry(Object value, String owner) {
            this.value = value;
            this.timestamp = System.currentTimeMillis();
            this.owner = owner;
            this.metadata = new ConcurrentHashMap<>();
            this.accessCount = 0;
        }
        
        public Object getValue() {
            accessCount++;
            return value;
        }
        
        public long getTimestamp() {
            return timestamp;
        }
        
        public String getOwner() {
            return owner;
        }
        
        public Map<String, Object> getMetadata() {
            return metadata;
        }
        
        public int getAccessCount() {
            return accessCount;
        }
    }
    
    /**
     * 状态变化监听器
     */
    public interface StateChangeListener {
        void onStateChanged(String key, Object oldValue, Object newValue, String modifier);
        void onStateRemoved(String key, Object oldValue, String remover);
    }
    
    /**
     * 状态查询条件
     */
    public static class StateQuery {
        private String keyPattern;
        private String owner;
        private Long minTimestamp;
        private Long maxTimestamp;
        private Integer minAccessCount;
        private Map<String, Object> metadataFilter;
        
        // Builder pattern
        public StateQuery withKeyPattern(String pattern) {
            this.keyPattern = pattern;
            return this;
        }
        
        public StateQuery withOwner(String owner) {
            this.owner = owner;
            return this;
        }
        
        public StateQuery withTimestampRange(Long min, Long max) {
            this.minTimestamp = min;
            this.maxTimestamp = max;
            return this;
        }
        
        public StateQuery withMinAccessCount(int count) {
            this.minAccessCount = count;
            return this;
        }
        
        public StateQuery withMetadata(String key, Object value) {
            if (metadataFilter == null) {
                metadataFilter = new HashMap<>();
            }
            metadataFilter.put(key, value);
            return this;
        }
        
        private boolean matches(String key, StateEntry entry) {
            if (keyPattern != null && !key.matches(keyPattern)) {
                return false;
            }
            if (owner != null && !owner.equals(entry.getOwner())) {
                return false;
            }
            if (minTimestamp != null && entry.getTimestamp() < minTimestamp) {
                return false;
            }
            if (maxTimestamp != null && entry.getTimestamp() > maxTimestamp) {
                return false;
            }
            if (minAccessCount != null && entry.getAccessCount() < minAccessCount) {
                return false;
            }
            if (metadataFilter != null) {
                for (Map.Entry<String, Object> filter : metadataFilter.entrySet()) {
                    Object value = entry.getMetadata().get(filter.getKey());
                    if (!Objects.equals(value, filter.getValue())) {
                        return false;
                    }
                }
            }
            return true;
        }
    }
    
    private CJmodGlobalStateManager() {
        this.globalStates = new ConcurrentHashMap<>();
        this.moduleStates = new ConcurrentHashMap<>();
        this.listeners = new ConcurrentHashMap<>();
        this.persistenceExecutor = Executors.newSingleThreadScheduledExecutor(r -> {
            Thread t = new Thread(r, "CJmod-State-Persistence");
            t.setDaemon(true);
            return t;
        });
    }
    
    public static synchronized CJmodGlobalStateManager getInstance() {
        if (instance == null) {
            instance = new CJmodGlobalStateManager();
        }
        return instance;
    }
    
    /**
     * 设置全局状态
     */
    public void setGlobalState(String key, Object value, String setter) {
        StateEntry oldEntry = globalStates.get(key);
        Object oldValue = oldEntry != null ? oldEntry.getValue() : null;
        
        StateEntry newEntry = new StateEntry(value, setter);
        globalStates.put(key, newEntry);
        
        // 触发监听器
        notifyListeners(key, oldValue, value, setter);
        
        // 自动持久化
        if (autoPersist) {
            schedulePersistence();
        }
    }
    
    /**
     * 获取全局状态
     */
    public Object getGlobalState(String key) {
        StateEntry entry = globalStates.get(key);
        return entry != null ? entry.getValue() : null;
    }
    
    /**
     * 检查全局状态是否存在
     */
    public boolean hasGlobalState(String key) {
        return globalStates.containsKey(key);
    }
    
    /**
     * 移除全局状态
     */
    public Object removeGlobalState(String key, String remover) {
        StateEntry entry = globalStates.remove(key);
        if (entry != null) {
            Object value = entry.getValue();
            notifyListenersRemoval(key, value, remover);
            return value;
        }
        return null;
    }
    
    /**
     * 设置模块私有状态
     */
    public void setModuleState(String module, String key, Object value) {
        ConcurrentHashMap<String, StateEntry> moduleMap = moduleStates.computeIfAbsent(
            module, k -> new ConcurrentHashMap<>()
        );
        moduleMap.put(key, new StateEntry(value, module));
    }
    
    /**
     * 获取模块私有状态
     */
    public Object getModuleState(String module, String key) {
        ConcurrentHashMap<String, StateEntry> moduleMap = moduleStates.get(module);
        if (moduleMap != null) {
            StateEntry entry = moduleMap.get(key);
            return entry != null ? entry.getValue() : null;
        }
        return null;
    }
    
    /**
     * 查询状态
     */
    public Map<String, Object> queryStates(StateQuery query) {
        Map<String, Object> results = new HashMap<>();
        
        // 查询全局状态
        for (Map.Entry<String, StateEntry> entry : globalStates.entrySet()) {
            if (query.matches(entry.getKey(), entry.getValue())) {
                results.put(entry.getKey(), entry.getValue().getValue());
            }
        }
        
        return results;
    }
    
    /**
     * 注册状态监听器
     */
    public void addStateChangeListener(String keyPattern, StateChangeListener listener) {
        listeners.computeIfAbsent(keyPattern, k -> new CopyOnWriteArrayList<>()).add(listener);
    }
    
    /**
     * 移除状态监听器
     */
    public void removeStateChangeListener(String keyPattern, StateChangeListener listener) {
        List<StateChangeListener> list = listeners.get(keyPattern);
        if (list != null) {
            list.remove(listener);
        }
    }
    
    /**
     * 批量操作
     */
    public void batchSetGlobalStates(Map<String, Object> states, String setter) {
        for (Map.Entry<String, Object> entry : states.entrySet()) {
            setGlobalState(entry.getKey(), entry.getValue(), setter);
        }
    }
    
    /**
     * 事务操作
     */
    public void transactional(Runnable operation) {
        // 简单的事务实现，可以扩展为更复杂的事务机制
        Map<String, StateEntry> backup = new HashMap<>(globalStates);
        try {
            operation.run();
        } catch (Exception e) {
            // 回滚
            globalStates.clear();
            globalStates.putAll(backup);
            throw new RuntimeException("Transaction failed", e);
        }
    }
    
    /**
     * 持久化配置
     */
    public void enablePersistence(Path path, boolean autoPersist) {
        this.persistencePath = path;
        this.autoPersist = autoPersist;
        
        if (autoPersist) {
            // 每5分钟自动持久化一次
            persistenceExecutor.scheduleAtFixedRate(
                this::persist, 5, 5, TimeUnit.MINUTES
            );
        }
        
        // 立即加载已有状态
        loadPersistedStates();
    }
    
    /**
     * 手动持久化
     */
    public void persist() {
        if (persistencePath == null) return;
        
        try {
            Map<String, Object> toSave = new HashMap<>();
            for (Map.Entry<String, StateEntry> entry : globalStates.entrySet()) {
                Map<String, Object> entryData = new HashMap<>();
                entryData.put("value", entry.getValue().getValue());
                entryData.put("owner", entry.getValue().getOwner());
                entryData.put("timestamp", entry.getValue().getTimestamp());
                entryData.put("metadata", entry.getValue().getMetadata());
                toSave.put(entry.getKey(), entryData);
            }
            
            // 序列化为JSON或其他格式
            String json = serializeToJson(toSave);
            Files.write(persistencePath, json.getBytes(), 
                       StandardOpenOption.CREATE, StandardOpenOption.TRUNCATE_EXISTING);
            
        } catch (IOException e) {
            System.err.println("Failed to persist global states: " + e.getMessage());
        }
    }
    
    /**
     * 加载持久化的状态
     */
    private void loadPersistedStates() {
        if (persistencePath == null || !Files.exists(persistencePath)) {
            return;
        }
        
        try {
            String json = new String(Files.readAllBytes(persistencePath));
            Map<String, Object> loaded = deserializeFromJson(json);
            
            for (Map.Entry<String, Object> entry : loaded.entrySet()) {
                if (entry.getValue() instanceof Map) {
                    Map<String, Object> entryData = (Map<String, Object>) entry.getValue();
                    Object value = entryData.get("value");
                    String owner = (String) entryData.get("owner");
                    if (value != null && owner != null) {
                        globalStates.put(entry.getKey(), new StateEntry(value, owner));
                    }
                }
            }
            
        } catch (IOException e) {
            System.err.println("Failed to load persisted states: " + e.getMessage());
        }
    }
    
    /**
     * 通知监听器
     */
    private void notifyListeners(String key, Object oldValue, Object newValue, String modifier) {
        for (Map.Entry<String, List<StateChangeListener>> entry : listeners.entrySet()) {
            if (key.matches(entry.getKey())) {
                for (StateChangeListener listener : entry.getValue()) {
                    try {
                        listener.onStateChanged(key, oldValue, newValue, modifier);
                    } catch (Exception e) {
                        System.err.println("Listener error: " + e.getMessage());
                    }
                }
            }
        }
    }
    
    private void notifyListenersRemoval(String key, Object oldValue, String remover) {
        for (Map.Entry<String, List<StateChangeListener>> entry : listeners.entrySet()) {
            if (key.matches(entry.getKey())) {
                for (StateChangeListener listener : entry.getValue()) {
                    try {
                        listener.onStateRemoved(key, oldValue, remover);
                    } catch (Exception e) {
                        System.err.println("Listener error: " + e.getMessage());
                    }
                }
            }
        }
    }
    
    private void schedulePersistence() {
        persistenceExecutor.schedule(this::persist, 10, TimeUnit.SECONDS);
    }
    
    private String serializeToJson(Map<String, Object> data) {
        // 简单的JSON序列化，实际使用时可以用Jackson或Gson
        return data.toString();
    }
    
    private Map<String, Object> deserializeFromJson(String json) {
        // 简单的JSON反序列化，实际使用时可以用Jackson或Gson
        return new HashMap<>();
    }
    
    /**
     * 关闭管理器
     */
    public void shutdown() {
        if (autoPersist) {
            persist();
        }
        persistenceExecutor.shutdown();
        try {
            persistenceExecutor.awaitTermination(10, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            persistenceExecutor.shutdownNow();
        }
    }
}