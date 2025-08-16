package com.chtl.cjmod;

import com.chtl.compiler.cjmod.ModuleInfo;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.stream.Collectors;
import com.chtl.compiler.cjmod.CJmodInterface;
import com.chtl.compiler.cjmod.CJmodLoader;
import com.chtl.compiler.cjmod.CJmodManager;
import java.io.*;

/**
 * CJMOD管理器
 * 负责加载、管理和执行CJMOD模块
 */
public class CJmodManager {
    
    private static CJmodManager instance;
    
    // 已加载的模块
    private final Map<String, LoadedModule> loadedModules;
    
    // 模块加载器
    private final List<CJmodLoader> loaders;
    
    // 模块搜索路径
    private final List<Path> modulePaths;
    
    // 模块依赖图
    private final Map<String, Set<String>> dependencyGraph;
    
    // 执行上下文工厂
    private ExecutionContextFactory contextFactory;
    
    // 模块监听器
    private final List<ModuleEventListener> moduleListeners;
    
    // 线程池
    private final ExecutorService executorService;
    
    /**
     * 已加载的模块信息
     */
    public static class LoadedModule {
        private final String name;
        private final String version;
        private final CJmodInterface module;
        private final ClassLoader classLoader;
        private final Path sourcePath;
        private final long loadTime;
        private volatile ModuleState state;
        private final Map<String, Object> metadata;
        
        public LoadedModule(String name, String version, CJmodInterface module, 
                          ClassLoader classLoader, Path sourcePath) {
            this.name = name;
            this.version = version;
            this.module = module;
            this.classLoader = classLoader;
            this.sourcePath = sourcePath;
            this.loadTime = System.currentTimeMillis();
            this.state = ModuleState.LOADED;
            this.metadata = new ConcurrentHashMap<Object, Object>();
        }
        
        // Getters
        public String getName() { return name; }
        public String getVersion() { return version; }
        public CJmodInterface getModule() { return module; }
        public ClassLoader getClassLoader() { return classLoader; }
        public Path getSourcePath() { return sourcePath; }
        public long getLoadTime() { return loadTime; }
        public ModuleState getState() { return state; }
        public Map<String, Object> getMetadata() { return metadata; }
        
        public void setState(ModuleState state) {
            this.state = state;
        }
    }
    
    /**
     * 模块状态
     */
    public enum ModuleState {
        LOADING,     // 正在加载
        LOADED,      // 已加载
        INITIALIZED, // 已初始化
        ACTIVE,      // 活动中
        SUSPENDED,   // 已暂停
        ERROR,       // 错误状态
        UNLOADED     // 已卸载
    }
    
    /**
     * 模块事件监听器
     */
    public interface ModuleEventListener {
        void onModuleLoaded(String moduleName, String version);
        void onModuleInitialized(String moduleName);
        void onModuleActivated(String moduleName);
        void onModuleSuspended(String moduleName);
        void onModuleUnloaded(String moduleName);
        void onModuleError(String moduleName, Throwable error);
    }
    
    /**
     * 模块加载器接口
     */
    public interface CJmodLoader {
        boolean canLoad(Path path);
        LoadedModule load(Path path) throws Exception;
        String getLoaderType();
    }
    
    /**
     * 执行上下文工厂
     */
    public interface ExecutionContextFactory {
        CJmodInterface.ExecutionContext createContext(String moduleName, String functionName);
    }
    
    private CJmodManager() {
        this.loadedModules = new ConcurrentHashMap<Object, Object>();
        this.loaders = new CopyOnWriteArrayList<Object>();
        this.modulePaths = new CopyOnWriteArrayList<Object>();
        this.dependencyGraph = new ConcurrentHashMap<Object, Object>();
        this.moduleListeners = new CopyOnWriteArrayList<Object>();
        this.executorService = Executors.newCachedThreadPool(r -> {
            Thread t = new Thread(r, "CJmod-Executor");
            t.setDaemon(true);
            return t;
        });
        
        // 注册默认加载器
        registerDefaultLoaders();
        
        // 添加默认模块路径
        addModulePath(Paths.get("modules"));
        addModulePath(Paths.get(System.getProperty("user.home"), ".chtl", "modules"));
    }
    
    public static synchronized CJmodManager getInstance() {
        if (instance == null) {
            instance = new CJmodManager();
        }
        return instance;
    }
    
    /**
     * 注册默认加载器
     */
    private void registerDefaultLoaders() {
        // Java JAR加载器
        registerLoader(new JavaJarLoader());
        
        // 目录加载器
        registerLoader(new DirectoryModuleLoader());
    }
    
    /**
     * 注册模块加载器
     */
    public void registerLoader(CJmodLoader loader) {
        loaders.add(loader);
    }
    
    /**
     * 添加模块搜索路径
     */
    public void addModulePath(Path path) {
        if (Files.isDirectory(path)) {
            modulePaths.add(path);
        }
    }
    
    /**
     * 设置执行上下文工厂
     */
    public void setContextFactory(ExecutionContextFactory factory) {
        this.contextFactory = factory;
    }
    
    /**
     * 加载模块
     */
    public CompletableFuture<LoadedModule> loadModule(String moduleName) {
        return CompletableFuture.supplyAsync(() -> {
            try {
                // 检查是否已加载
                LoadedModule existing = loadedModules.get(moduleName);
                if (existing != null && existing.getState() != ModuleState.ERROR) {
                    return existing;
                }
                
                // 在搜索路径中查找模块
                for (Path searchPath : modulePaths) {
                    Path modulePath = searchPath.resolve(moduleName);
                    if (Files.exists(modulePath)) {
                        return loadModuleFromPath(modulePath);
                    }
                    
                    // 尝试带扩展名
                    for (String ext : Arrays.asList(".jar", ".so", ".dll", ".dylib")) {
                        Path moduleFile = searchPath.resolve(moduleName + ext);
                        if (Files.exists(moduleFile)) {
                            return loadModuleFromPath(moduleFile);
                        }
                    }
                }
                
                throw new ModuleNotFoundException("Module not found: " + moduleName);
                
            } catch (Exception e) {
                notifyError(moduleName, e);
                throw new RuntimeException("Failed to load module: " + moduleName, e);
            }
        }, executorService);
    }
    
    /**
     * 从路径加载模块
     */
    private LoadedModule loadModuleFromPath(Path path) throws Exception {
        // 查找合适的加载器
        for (CJmodLoader loader : loaders) {
            if (loader.canLoad(path)) {
                LoadedModule module = loader.load(path);
                
                // 注册模块
                loadedModules.put(module.getName(), module);
                
                // 通知监听器
                notifyLoaded(module.getName(), module.getVersion());
                
                // 初始化模块
                initializeModule(module);
                
                return module;
            }
        }
        
        throw new UnsupportedModuleException("No loader available for: " + path);
    }
    
    /**
     * 初始化模块
     */
    private void initializeModule(LoadedModule module) {
        try {
            module.setState(ModuleState.LOADING);
            
            // 检查依赖
            CJmodInterface.ModuleInfo info = module.getModule().getModuleInfo();
            List<String> dependencies = info.getDependencies();
            
            if (dependencies != null && !dependencies.isEmpty()) {
                // 加载依赖
                List<CompletableFuture<LoadedModule>> depFutures = dependencies.stream()
                    .map(this::loadModule)
                    .collect(Collectors.toList());
                    
                // 等待所有依赖加载完成
                CompletableFuture.allOf(depFutures.toArray(new CompletableFuture[0])).join();
                
                // 更新依赖图
                dependencyGraph.put(module.getName(), new HashSet<Object>(dependencies));
            }
            
            // 初始化模块
            Map<String, Object> config = new HashMap<Object, Object>();
            config.put("moduleDir", module.getSourcePath().toString());
            config.put("globalStateManager", CJmodGlobalStateManager.getInstance());
            
            module.getModule().initialize(config);
            module.setState(ModuleState.INITIALIZED);
            
            // 通知监听器
            notifyInitialized(module.getName());
            
            // 激活模块
            activateModule(module);
            
        } catch (Exception e) {
            module.setState(ModuleState.ERROR);
            throw new RuntimeException("Failed to initialize module: " + module.getName(), e);
        }
    }
    
    /**
     * 激活模块
     */
    private void activateModule(LoadedModule module) {
        module.setState(ModuleState.ACTIVE);
        notifyActivated(module.getName());
    }
    
    /**
     * 执行模块函数
     */
    public CompletableFuture<CJmodInterface.FunctionResult> executeFunction(
            String moduleName, String functionName, Map<String, Object> parameters) {
        
        return CompletableFuture.supplyAsync(() -> {
            LoadedModule module = loadedModules.get(moduleName);
            if (module == null) {
                throw new IllegalArgumentException("Module not loaded: " + moduleName);
            }
            
            if (module.getState() != ModuleState.ACTIVE) {
                throw new IllegalStateException("Module not active: " + moduleName);
            }
            
            // 创建执行上下文
            CJmodInterface.ExecutionContext context = contextFactory != null ?
                contextFactory.createContext(moduleName, functionName) :
                createDefaultContext(moduleName, functionName);
            
            // 执行函数
            return module.getModule().executeFunction(functionName, parameters, context);
            
        }, executorService);
    }
    
    /**
     * 创建默认执行上下文
     */
    private CJmodInterface.ExecutionContext createDefaultContext(String moduleName, String functionName) {
        return new DefaultExecutionContext(moduleName, functionName);
    }
    
    /**
     * 获取已加载的模块
     */
    public LoadedModule getModule(String moduleName) {
        return loadedModules.get(moduleName);
    }
    
    /**
     * 获取所有已加载的模块
     */
    public Collection<LoadedModule> getAllModules() {
        return Collections.unmodifiableCollection(loadedModules.values());
    }
    
    /**
     * 卸载模块
     */
    public CompletableFuture<Void> unloadModule(String moduleName) {
        return CompletableFuture.runAsync(() -> {
            LoadedModule module = loadedModules.remove(moduleName);
            if (module != null) {
                try {
                    // 暂停模块
                    module.setState(ModuleState.SUSPENDED);
                    notifySuspended(moduleName);
                    
                    // 清理资源
                    module.getModule().cleanup();
                    
                    // 更新依赖图
                    dependencyGraph.remove(moduleName);
                    
                    // 卸载依赖此模块的其他模块
                    for (Map.Entry<String, Set<String>> entry : dependencyGraph.entrySet()) {
                        if (entry.getValue().contains(moduleName)) {
                            unloadModule(entry.getKey());
                        }
                    }
                    
                    module.setState(ModuleState.UNLOADED);
                    notifyUnloaded(moduleName);
                    
                } catch (Exception e) {
                    notifyError(moduleName, e);
                    throw new RuntimeException("Failed to unload module: " + moduleName, e);
                }
            }
        }, executorService);
    }
    
    /**
     * 重新加载模块
     */
    public CompletableFuture<LoadedModule> reloadModule(String moduleName) {
        return unloadModule(moduleName).thenCompose(v -> loadModule(moduleName));
    }
    
    /**
     * 添加模块事件监听器
     */
    public void addModuleEventListener(ModuleEventListener listener) {
        moduleListeners.add(listener);
    }
    
    /**
     * 移除模块事件监听器
     */
    public void removeModuleEventListener(ModuleEventListener listener) {
        moduleListeners.remove(listener);
    }
    
    // 通知方法
    private void notifyLoaded(String moduleName, String version) {
        for (ModuleEventListener listener : moduleListeners) {
            try {
                listener.onModuleLoaded(moduleName, version);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    private void notifyInitialized(String moduleName) {
        for (ModuleEventListener listener : moduleListeners) {
            try {
                listener.onModuleInitialized(moduleName);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    private void notifyActivated(String moduleName) {
        for (ModuleEventListener listener : moduleListeners) {
            try {
                listener.onModuleActivated(moduleName);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    private void notifySuspended(String moduleName) {
        for (ModuleEventListener listener : moduleListeners) {
            try {
                listener.onModuleSuspended(moduleName);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    private void notifyUnloaded(String moduleName) {
        for (ModuleEventListener listener : moduleListeners) {
            try {
                listener.onModuleUnloaded(moduleName);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    private void notifyError(String moduleName, Throwable error) {
        for (ModuleEventListener listener : moduleListeners) {
            try {
                listener.onModuleError(moduleName, error);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    /**
     * 关闭管理器
     */
    public void shutdown() {
        // 卸载所有模块
        List<String> moduleNames = new ArrayList<Object>(loadedModules.keySet());
        for (String moduleName : moduleNames) {
            try {
                unloadModule(moduleName).get(10, TimeUnit.SECONDS);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        
        // 关闭线程池
        executorService.shutdown();
        try {
            executorService.awaitTermination(30, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            executorService.shutdownNow();
        }
    }
    
    // 异常类
    public static class ModuleNotFoundException extends Exception {
        public ModuleNotFoundException(String message) {
            super(message);
        }
    }
    
    public static class UnsupportedModuleException extends Exception {
        public UnsupportedModuleException(String message) {
            super(message);
        }
    }
}
