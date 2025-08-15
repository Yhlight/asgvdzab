package com.chtl.cjmod;
import java.io.File;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

import com.chtl.cjmod.ModuleSandbox.SandboxClassLoader;
import com.chtl.cjmod.ModuleSandbox.SandboxConfig;
import com.chtl.cjmod.ModuleSandbox.SandboxContext;
import com.chtl.cjmod.ModuleSandbox.SandboxSecurityManager;

import java.security.AccessController;
import java.security.PrivilegedAction;

import java.io.*;
import java.lang.reflect.*;
import java.net.*;
import java.security.*;
import java.util.*;
import java.util.concurrent.*;

/**
 * 模块沙箱隔离机制
 * 为CJMOD模块提供安全的执行环境
 */
public class ModuleSandbox {
    
    /**
     * 沙箱配置
     */
    public static class SandboxConfig {
        private boolean allowFileAccess = false;
        private boolean allowNetworkAccess = false;
        private boolean allowSystemProperties = false;
        private boolean allowReflection = false;
        private boolean allowThreadCreation = false;
        private Set<String> allowedPackages = new HashSet<>();
        private Set<String> allowedClasses = new HashSet<>();
        private Set<String> allowedFilePaths = new HashSet<>();
        private Set<String> allowedHosts = new HashSet<>();
        private long maxMemory = 128 * 1024 * 1024; // 128MB
        private long maxCpuTime = 10000; // 10秒
        private int maxThreads = 10;
        
        // Builder pattern
        public SandboxConfig allowFileAccess(boolean allow) {
            this.allowFileAccess = allow;
            return this;
        }
        
        public SandboxConfig allowNetworkAccess(boolean allow) {
            this.allowNetworkAccess = allow;
            return this;
        }
        
        public SandboxConfig allowSystemProperties(boolean allow) {
            this.allowSystemProperties = allow;
            return this;
        }
        
        public SandboxConfig allowReflection(boolean allow) {
            this.allowReflection = allow;
            return this;
        }
        
        public SandboxConfig allowThreadCreation(boolean allow) {
            this.allowThreadCreation = allow;
            return this;
        }
        
        public SandboxConfig allowPackage(String packageName) {
            this.allowedPackages.add(packageName);
            return this;
        }
        
        public SandboxConfig allowClass(String className) {
            this.allowedClasses.add(className);
            return this;
        }
        
        public SandboxConfig allowFilePath(String path) {
            this.allowedFilePaths.add(path);
            return this;
        }
        
        public SandboxConfig allowHost(String host) {
            this.allowedHosts.add(host);
            return this;
        }
        
        public SandboxConfig setMaxMemory(long bytes) {
            this.maxMemory = bytes;
            return this;
        }
        
        public SandboxConfig setMaxCpuTime(long millis) {
            this.maxCpuTime = millis;
            return this;
        }
        
        public SandboxConfig setMaxThreads(int count) {
            this.maxThreads = count;
            return this;
        }
    }
    
    /**
     * 沙箱上下文
     */
    public static class SandboxContext {
        private final String moduleName;
        private final SandboxConfig config;
        private final ThreadGroup threadGroup;
        private final AtomicInteger threadCount;
        private final long startTime;
        private volatile boolean terminated;
        
        public SandboxContext(String moduleName, SandboxConfig config) {
            this.moduleName = moduleName;
            this.config = config;
            this.threadGroup = new ThreadGroup("Sandbox-" + moduleName);
            this.threadCount = new AtomicInteger(0);
            this.startTime = System.currentTimeMillis();
            this.terminated = false;
        }
        
        public boolean canCreateThread() {
            return config.allowThreadCreation && threadCount.get() < config.maxThreads;
        }
        
        public void incrementThreadCount() {
            threadCount.incrementAndGet();
        }
        
        public void decrementThreadCount() {
            threadCount.decrementAndGet();
        }
        
        public boolean isTimeExceeded() {
            return System.currentTimeMillis() - startTime > config.maxCpuTime;
        }
        
        public void terminate() {
            terminated = true;
            threadGroup.interrupt();
        }
        
        public boolean isTerminated() {
            return terminated;
        }
    }
    
    /**
     * 安全管理器
     */
    private static class SandboxSecurityManager extends SecurityManager {
        private final Map<Thread, SandboxContext> contextMap;
        
        public SandboxSecurityManager() {
            this.contextMap = new ConcurrentHashMap<>();
        }
        
        public void registerContext(Thread thread, SandboxContext context) {
            contextMap.put(thread, context);
        }
        
        public void unregisterContext(Thread thread) {
            contextMap.remove(thread);
        }
        
        private SandboxContext getContext() {
            Thread current = Thread.currentThread();
            SandboxContext context = contextMap.get(current);
            
            // 检查线程组
            if (context == null) {
                ThreadGroup group = current.getThreadGroup();
                while (group != null) {
                    for (Map.Entry<Thread, SandboxContext> entry : contextMap.entrySet()) {
                        if (entry.getValue().threadGroup == group) {
                            return entry.getValue();
                        }
                    }
                    group = group.getParent();
                }
            }
            
            return context;
        }
        
        @Override
        public void checkPermission(Permission perm) {
            SandboxContext context = getContext();
            if (context == null) {
                // 不在沙箱中，允许
                return;
            }
            
            // 检查是否已终止
            if (context.isTerminated()) {
                throw new SecurityException("Sandbox has been terminated");
            }
            
            // 检查时间限制
            if (context.isTimeExceeded()) {
                context.terminate();
                throw new SecurityException("CPU time limit exceeded");
            }
            
            // 检查具体权限
            checkPermissionInternal(perm, context);
        }
        
        private void checkPermissionInternal(Permission perm, SandboxContext context) {
            SandboxConfig config = context.config;
            
            // 文件权限
            if (perm instanceof FilePermission) {
                if (!config.allowFileAccess) {
                    throw new SecurityException("File access not allowed");
                }
                
                FilePermission fp = (FilePermission) perm;
                String path = fp.getName();
                
                // 检查允许的路径
                boolean allowed = config.allowedFilePaths.stream()
                    .anyMatch(allowedPath -> path.startsWith(allowedPath));
                
                if (!allowed) {
                    throw new SecurityException("Access to path not allowed: " + path);
                }
            }
            
            // 网络权限
            else if (perm instanceof SocketPermission) {
                if (!config.allowNetworkAccess) {
                    throw new SecurityException("Network access not allowed");
                }
                
                SocketPermission sp = (SocketPermission) perm;
                String host = sp.getName().split(":")[0];
                
                // 检查允许的主机
                boolean allowed = config.allowedHosts.contains(host) ||
                                config.allowedHosts.contains("*");
                
                if (!allowed) {
                    throw new SecurityException("Access to host not allowed: " + host);
                }
            }
            
            // 反射权限
            else if (perm instanceof ReflectPermission) {
                if (!config.allowReflection) {
                    throw new SecurityException("Reflection not allowed");
                }
            }
            
            // 系统属性权限
            else if (perm instanceof PropertyPermission) {
                if (!config.allowSystemProperties) {
                    PropertyPermission pp = (PropertyPermission) perm;
                    if (!pp.getName().startsWith("cjmod.")) {
                        throw new SecurityException("System property access not allowed");
                    }
                }
            }
            
            // 运行时权限
            else if (perm instanceof RuntimePermission) {
                String name = perm.getName();
                
                // 线程创建
                if (name.equals("modifyThreadGroup") || name.startsWith("modifyThread")) {
                    if (!context.canCreateThread()) {
                        throw new SecurityException("Thread creation limit exceeded");
                    }
                }
                
                // 类加载器
                else if (name.equals("createClassLoader") || name.equals("getClassLoader")) {
                    // 总是拒绝创建新的类加载器
                    throw new SecurityException("Creating class loaders not allowed");
                }
                
                // 退出JVM
                else if (name.startsWith("exitVM")) {
                    throw new SecurityException("System exit not allowed");
                }
            }
        }
        
        @Override
        public void checkAccess(Thread t) {
            SandboxContext context = getContext();
            if (context != null && t.getThreadGroup() != context.threadGroup) {
                throw new SecurityException("Access to external threads not allowed");
            }
        }
        
        @Override
        public void checkAccess(ThreadGroup g) {
            SandboxContext context = getContext();
            if (context != null && g != context.threadGroup) {
                throw new SecurityException("Access to external thread groups not allowed");
            }
        }
    }
    
    /**
     * 沙箱类加载器
     */
    private static class SandboxClassLoader extends URLClassLoader {
        private final SandboxConfig config;
        private final Set<String> loadedClasses;
        
        public SandboxClassLoader(URL[] urls, ClassLoader parent, SandboxConfig config) {
            super(urls, parent);
            this.config = config;
            this.loadedClasses = new HashSet<>();
        }
        
        @Override
        protected Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
            // 检查是否允许加载此类
            if (!isClassAllowed(name)) {
                throw new ClassNotFoundException("Class not allowed: " + name);
            }
            
            // 记录已加载的类
            loadedClasses.add(name);
            
            return super.loadClass(name, resolve);
        }
        
        private boolean isClassAllowed(String className) {
            // 总是允许基本的Java类
            if (className.startsWith("java.lang.") ||
                className.startsWith("java.util.") ||
                className.startsWith("java.io.") ||
                className.startsWith("com.chtl.cjmod.")) {
                return true;
            }
            
            // 检查允许的类
            if (config.allowedClasses.contains(className)) {
                return true;
            }
            
            // 检查允许的包
            return config.allowedPackages.stream()
                .anyMatch(pkg -> className.startsWith(pkg + "."));
        }
        
        public Set<String> getLoadedClasses() {
            return new HashSet<>(loadedClasses);
        }
    }
    
    // 单例安全管理器
    private static SandboxSecurityManager securityManager;
    private static final Object securityManagerLock = new Object();
    
    /**
     * 初始化沙箱系统
     */
    public static void initialize() {
        synchronized (securityManagerLock) {
            if (securityManager == null) {
                securityManager = new SandboxSecurityManager();
                // System.setSecurityManager(...); // Deprecated in Java 17+
            }
        }
    }
    
    /**
     * 在沙箱中执行代码
     */
    public static <T> T execute(String moduleName, SandboxConfig config, 
                                Callable<T> task, ClassLoader moduleClassLoader) throws Exception {
        // 确保安全管理器已初始化
        initialize();
        
        // 创建沙箱上下文
        SandboxContext context = new SandboxContext(moduleName, config);
        
        // 创建沙箱类加载器
        SandboxClassLoader sandboxLoader;
        if (moduleClassLoader instanceof URLClassLoader) {
            URL[] urls = ((URLClassLoader) moduleClassLoader).getURLs();
            sandboxLoader = new SandboxClassLoader(urls, moduleClassLoader.getParent(), config);
        } else {
            sandboxLoader = new SandboxClassLoader(new URL[0], moduleClassLoader, config);
        }
        
        // 创建执行线程
        FutureTask<T> future = new FutureTask<>(() -> {
            // 设置线程上下文类加载器
            Thread.currentThread().setContextClassLoader(sandboxLoader);
            
            // 注册安全上下文
            securityManager.registerContext(Thread.currentThread(), context);
            
            try {
                return task.call();
            } finally {
                // 清理上下文
                securityManager.unregisterContext(Thread.currentThread());
            }
        });
        
        // 在沙箱线程组中执行
        Thread executorThread = new Thread(context.threadGroup, future, 
                                         "Sandbox-Executor-" + moduleName);
        
        // 设置线程优先级较低
        executorThread.setPriority(Thread.MIN_PRIORITY);
        
        // 启动执行
        executorThread.start();
        
        try {
            // 等待执行完成或超时
            return future.get(config.maxCpuTime, TimeUnit.MILLISECONDS);
            
        } catch (TimeoutException e) {
            // 超时，终止执行
            context.terminate();
            executorThread.interrupt();
            throw new SecurityException("Execution timeout", e);
            
        } catch (ExecutionException e) {
            // 执行异常
            Throwable cause = e.getCause();
            if (cause instanceof Exception) {
                throw (Exception) cause;
            } else {
                throw new RuntimeException("Execution failed", cause);
            }
            
        } finally {
            // 确保线程终止
            if (executorThread.isAlive()) {
                executorThread.interrupt();
                try {
                    executorThread.join(1000);
                } catch (InterruptedException ignored) {
                }
                
                if (executorThread.isAlive()) {
                    // 强制停止（不推荐，但作为最后手段）
                    executorThread.interrupt(); // stop() is deprecated
                }
            }
        }
    }
    
    /**
     * 创建默认的沙箱配置
     */
    public static SandboxConfig createDefaultConfig() {
        return new SandboxConfig()
            .allowFileAccess(false)
            .allowNetworkAccess(false)
            .allowSystemProperties(false)
            .allowReflection(false)
            .allowThreadCreation(false)
            .setMaxMemory(128 * 1024 * 1024) // 128MB
            .setMaxCpuTime(10000) // 10秒
            .setMaxThreads(5)
            // 允许基本的Java包
            .allowPackage("java.lang")
            .allowPackage("java.util")
            .allowPackage("java.io")
            .allowPackage("java.math")
            .allowPackage("java.text")
            .allowPackage("java.time")
            // 允许CJMOD API
            .allowPackage("com.chtl.cjmod");
    }
    
    /**
     * 创建受限的沙箱配置
     */
    public static SandboxConfig createRestrictedConfig() {
        return new SandboxConfig()
            .allowFileAccess(false)
            .allowNetworkAccess(false)
            .allowSystemProperties(false)
            .allowReflection(false)
            .allowThreadCreation(false)
            .setMaxMemory(64 * 1024 * 1024) // 64MB
            .setMaxCpuTime(5000) // 5秒
            .setMaxThreads(1)
            // 只允许最基本的类
            .allowPackage("java.lang")
            .allowPackage("java.util");
    }
    
    /**
     * 创建开发模式的沙箱配置
     */
    public static SandboxConfig createDevelopmentConfig() {
        return new SandboxConfig()
            .allowFileAccess(true)
            .allowNetworkAccess(true)
            .allowSystemProperties(true)
            .allowReflection(true)
            .allowThreadCreation(true)
            .setMaxMemory(512 * 1024 * 1024) // 512MB
            .setMaxCpuTime(60000) // 60秒
            .setMaxThreads(20)
            .allowPackage("java")
            .allowPackage("javax")
            .allowPackage("com.chtl")
            .allowFilePath(System.getProperty("user.dir"))
            .allowHost("localhost")
            .allowHost("127.0.0.1");
    }
}