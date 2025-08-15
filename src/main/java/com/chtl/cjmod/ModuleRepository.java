package com.chtl.cjmod;

import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.stream.*;

/**
 * 模块仓库管理器
 * 管理模块的版本、依赖和存储
 */
public class ModuleRepository {
    
    private final Path repositoryRoot;
    private final Map<String, List<ModuleInfo>> moduleIndex;
    private final Map<String, Path> moduleCache;
    private final ExecutorService executorService;
    
    // 默认仓库路径
    private static final String DEFAULT_REPO = ".chtl/repository";
    private static final String INDEX_FILE = "modules.index";
    
    /**
     * 模块信息
     */
    public static class ModuleInfo {
        private final String name;
        private final ModuleVersion version;
        private final Path path;
        private final List<Dependency> dependencies;
        private final Map<String, String> metadata;
        private final long timestamp;
        
        public ModuleInfo(String name, ModuleVersion version, Path path) {
            this.name = name;
            this.version = version;
            this.path = path;
            this.dependencies = new ArrayList<>();
            this.metadata = new HashMap<>();
            this.timestamp = System.currentTimeMillis();
        }
        
        // Getters
        public String getName() { return name; }
        public ModuleVersion getVersion() { return version; }
        public Path getPath() { return path; }
        public List<Dependency> getDependencies() { return dependencies; }
        public Map<String, String> getMetadata() { return metadata; }
        public long getTimestamp() { return timestamp; }
        
        public String getFullName() {
            return name + "@" + version;
        }
    }
    
    /**
     * 依赖信息
     */
    public static class Dependency {
        private final String moduleName;
        private final ModuleVersion.VersionRange versionRange;
        private final boolean optional;
        
        public Dependency(String moduleName, String versionRange, boolean optional) {
            this.moduleName = moduleName;
            this.versionRange = new ModuleVersion.VersionRange(versionRange);
            this.optional = optional;
        }
        
        public Dependency(String moduleName, String versionRange) {
            this(moduleName, versionRange, false);
        }
        
        // Getters
        public String getModuleName() { return moduleName; }
        public ModuleVersion.VersionRange getVersionRange() { return versionRange; }
        public boolean isOptional() { return optional; }
    }
    
    /**
     * 依赖解析结果
     */
    public static class DependencyResolution {
        private final Map<String, ModuleInfo> resolvedModules;
        private final List<String> errors;
        private final List<String> warnings;
        
        public DependencyResolution() {
            this.resolvedModules = new LinkedHashMap<>();
            this.errors = new ArrayList<>();
            this.warnings = new ArrayList<>();
        }
        
        public void addModule(ModuleInfo module) {
            resolvedModules.put(module.getName(), module);
        }
        
        public void addError(String error) {
            errors.add(error);
        }
        
        public void addWarning(String warning) {
            warnings.add(warning);
        }
        
        public boolean hasErrors() {
            return !errors.isEmpty();
        }
        
        // Getters
        public Map<String, ModuleInfo> getResolvedModules() { return resolvedModules; }
        public List<String> getErrors() { return errors; }
        public List<String> getWarnings() { return warnings; }
    }
    
    public ModuleRepository() {
        this(Paths.get(System.getProperty("user.home"), DEFAULT_REPO));
    }
    
    public ModuleRepository(Path repositoryRoot) {
        this.repositoryRoot = repositoryRoot;
        this.moduleIndex = new ConcurrentHashMap<>();
        this.moduleCache = new ConcurrentHashMap<>();
        this.executorService = Executors.newCachedThreadPool(r -> {
            Thread t = new Thread(r, "ModuleRepo-Worker");
            t.setDaemon(true);
            return t;
        });
        
        initialize();
    }
    
    /**
     * 初始化仓库
     */
    private void initialize() {
        try {
            Files.createDirectories(repositoryRoot);
            loadIndex();
        } catch (IOException e) {
            throw new RuntimeException("Failed to initialize module repository", e);
        }
    }
    
    /**
     * 加载模块索引
     */
    private void loadIndex() {
        Path indexPath = repositoryRoot.resolve(INDEX_FILE);
        if (!Files.exists(indexPath)) {
            return;
        }
        
        try (BufferedReader reader = Files.newBufferedReader(indexPath)) {
            String line;
            while ((line = reader.readLine()) != null) {
                parseIndexLine(line);
            }
        } catch (IOException e) {
            System.err.println("Failed to load module index: " + e.getMessage());
        }
    }
    
    /**
     * 保存模块索引
     */
    private void saveIndex() {
        Path indexPath = repositoryRoot.resolve(INDEX_FILE);
        
        try (BufferedWriter writer = Files.newBufferedWriter(indexPath)) {
            for (List<ModuleInfo> versions : moduleIndex.values()) {
                for (ModuleInfo info : versions) {
                    writer.write(formatIndexLine(info));
                    writer.newLine();
                }
            }
        } catch (IOException e) {
            System.err.println("Failed to save module index: " + e.getMessage());
        }
    }
    
    /**
     * 安装模块
     */
    public CompletableFuture<ModuleInfo> installModule(Path modulePath, boolean force) {
        return CompletableFuture.supplyAsync(() -> {
            try {
                // 读取模块信息
                DirectoryModuleLoader.ModuleDescriptor descriptor = 
                    readModuleDescriptor(modulePath);
                
                String name = descriptor.name;
                ModuleVersion version = ModuleVersion.parse(descriptor.version);
                
                // 检查是否已存在
                if (!force && hasModule(name, version)) {
                    throw new IllegalStateException(
                        "Module already exists: " + name + "@" + version
                    );
                }
                
                // 创建目标路径
                Path targetPath = repositoryRoot
                    .resolve(name)
                    .resolve(version.toString());
                
                // 复制模块文件
                copyModule(modulePath, targetPath);
                
                // 创建模块信息
                ModuleInfo info = new ModuleInfo(name, version, targetPath);
                
                // 解析依赖
                if (descriptor.dependencies != null) {
                    for (String dep : descriptor.dependencies) {
                        if (!dep.trim().isEmpty()) {
                            parseDependency(dep, info.dependencies);
                        }
                    }
                }
                
                // 添加元数据
                info.metadata.putAll(descriptor.metadata);
                
                // 更新索引
                addToIndex(info);
                saveIndex();
                
                return info;
                
            } catch (Exception e) {
                throw new RuntimeException("Failed to install module", e);
            }
        }, executorService);
    }
    
    /**
     * 查找模块
     */
    public ModuleInfo findModule(String name, ModuleVersion.VersionRange range) {
        List<ModuleInfo> versions = moduleIndex.get(name);
        if (versions == null || versions.isEmpty()) {
            return null;
        }
        
        // 按版本降序排序
        return versions.stream()
            .filter(info -> range.contains(info.getVersion()))
            .sorted((a, b) -> b.getVersion().compareTo(a.getVersion()))
            .findFirst()
            .orElse(null);
    }
    
    /**
     * 获取最新版本
     */
    public ModuleInfo getLatestVersion(String name) {
        List<ModuleInfo> versions = moduleIndex.get(name);
        if (versions == null || versions.isEmpty()) {
            return null;
        }
        
        return versions.stream()
            .max(Comparator.comparing(ModuleInfo::getVersion))
            .orElse(null);
    }
    
    /**
     * 解析依赖
     */
    public DependencyResolution resolveDependencies(String moduleName, String versionRange) {
        DependencyResolution resolution = new DependencyResolution();
        Set<String> visited = new HashSet<>();
        
        ModuleInfo root = findModule(moduleName, new ModuleVersion.VersionRange(versionRange));
        if (root == null) {
            resolution.addError("Module not found: " + moduleName + "@" + versionRange);
            return resolution;
        }
        
        resolveDependenciesRecursive(root, resolution, visited);
        
        return resolution;
    }
    
    /**
     * 递归解析依赖
     */
    private void resolveDependenciesRecursive(ModuleInfo module, 
                                            DependencyResolution resolution,
                                            Set<String> visited) {
        String key = module.getFullName();
        if (visited.contains(key)) {
            return;
        }
        visited.add(key);
        
        resolution.addModule(module);
        
        for (Dependency dep : module.getDependencies()) {
            ModuleInfo depModule = findModule(dep.getModuleName(), dep.getVersionRange());
            
            if (depModule == null) {
                if (dep.isOptional()) {
                    resolution.addWarning("Optional dependency not found: " + 
                        dep.getModuleName() + "@" + dep.getVersionRange());
                } else {
                    resolution.addError("Required dependency not found: " + 
                        dep.getModuleName() + "@" + dep.getVersionRange());
                }
                continue;
            }
            
            // 检查版本冲突
            ModuleInfo existing = resolution.getResolvedModules().get(dep.getModuleName());
            if (existing != null && !existing.getVersion().equals(depModule.getVersion())) {
                resolution.addWarning("Version conflict for " + dep.getModuleName() + 
                    ": " + existing.getVersion() + " vs " + depModule.getVersion());
                
                // 使用较高版本
                if (depModule.getVersion().compareTo(existing.getVersion()) > 0) {
                    resolution.addModule(depModule);
                }
            } else {
                resolveDependenciesRecursive(depModule, resolution, visited);
            }
        }
    }
    
    /**
     * 清理缓存
     */
    public void clearCache() {
        moduleCache.clear();
    }
    
    /**
     * 删除模块
     */
    public CompletableFuture<Boolean> removeModule(String name, ModuleVersion version) {
        return CompletableFuture.supplyAsync(() -> {
            try {
                ModuleInfo info = findModule(name, 
                    new ModuleVersion.VersionRange(version.toString()));
                
                if (info == null) {
                    return false;
                }
                
                // 删除文件
                deleteDirectory(info.getPath());
                
                // 更新索引
                removeFromIndex(info);
                saveIndex();
                
                // 清理缓存
                moduleCache.remove(info.getFullName());
                
                return true;
                
            } catch (Exception e) {
                throw new RuntimeException("Failed to remove module", e);
            }
        }, executorService);
    }
    
    /**
     * 获取所有模块
     */
    public List<ModuleInfo> getAllModules() {
        return moduleIndex.values().stream()
            .flatMap(List::stream)
            .sorted(Comparator.comparing(ModuleInfo::getName)
                .thenComparing(ModuleInfo::getVersion))
            .collect(Collectors.toList());
    }
    
    // 辅助方法
    
    private boolean hasModule(String name, ModuleVersion version) {
        List<ModuleInfo> versions = moduleIndex.get(name);
        if (versions == null) return false;
        
        return versions.stream()
            .anyMatch(info -> info.getVersion().equals(version));
    }
    
    private void addToIndex(ModuleInfo info) {
        moduleIndex.computeIfAbsent(info.getName(), k -> new CopyOnWriteArrayList<>())
            .add(info);
    }
    
    private void removeFromIndex(ModuleInfo info) {
        List<ModuleInfo> versions = moduleIndex.get(info.getName());
        if (versions != null) {
            versions.removeIf(i -> i.getVersion().equals(info.getVersion()));
            if (versions.isEmpty()) {
                moduleIndex.remove(info.getName());
            }
        }
    }
    
    private void copyModule(Path source, Path target) throws IOException {
        Files.createDirectories(target);
        
        Files.walk(source)
            .forEach(sourcePath -> {
                try {
                    Path targetPath = target.resolve(source.relativize(sourcePath));
                    if (Files.isDirectory(sourcePath)) {
                        Files.createDirectories(targetPath);
                    } else {
                        Files.copy(sourcePath, targetPath, StandardCopyOption.REPLACE_EXISTING);
                    }
                } catch (IOException e) {
                    throw new RuntimeException("Failed to copy file: " + sourcePath, e);
                }
            });
    }
    
    private void deleteDirectory(Path path) throws IOException {
        if (!Files.exists(path)) return;
        
        Files.walk(path)
            .sorted(Comparator.reverseOrder())
            .forEach(p -> {
                try {
                    Files.delete(p);
                } catch (IOException e) {
                    System.err.println("Failed to delete: " + p);
                }
            });
    }
    
    private DirectoryModuleLoader.ModuleDescriptor readModuleDescriptor(Path modulePath) 
            throws Exception {
        Path descriptorPath;
        
        if (Files.isDirectory(modulePath)) {
            descriptorPath = modulePath.resolve("module.json");
        } else if (modulePath.toString().endsWith(".jar")) {
            // TODO: 从JAR读取
            throw new UnsupportedOperationException("JAR modules not yet supported");
        } else {
            throw new IllegalArgumentException("Invalid module path: " + modulePath);
        }
        
        String content = Files.readString(descriptorPath);
        return parseModuleDescriptor(content);
    }
    
    private DirectoryModuleLoader.ModuleDescriptor parseModuleDescriptor(String json) {
        // 复用DirectoryModuleLoader的解析逻辑
        DirectoryModuleLoader loader = new DirectoryModuleLoader();
        try {
            java.lang.reflect.Method method = DirectoryModuleLoader.class
                .getDeclaredMethod("parseModuleDescriptor", String.class);
            method.setAccessible(true);
            return (DirectoryModuleLoader.ModuleDescriptor) method.invoke(loader, json);
        } catch (Exception e) {
            throw new RuntimeException("Failed to parse module descriptor", e);
        }
    }
    
    private void parseDependency(String dep, List<Dependency> dependencies) {
        // 格式: moduleName@versionRange 或 moduleName@versionRange?optional
        String[] parts = dep.split("\\?");
        boolean optional = parts.length > 1 && "optional".equals(parts[1]);
        
        String[] nameParts = parts[0].split("@");
        if (nameParts.length != 2) {
            throw new IllegalArgumentException("Invalid dependency format: " + dep);
        }
        
        dependencies.add(new Dependency(nameParts[0], nameParts[1], optional));
    }
    
    private void parseIndexLine(String line) {
        // 格式: name|version|path|timestamp
        String[] parts = line.split("\\|");
        if (parts.length < 4) return;
        
        try {
            String name = parts[0];
            ModuleVersion version = ModuleVersion.parse(parts[1]);
            Path path = Paths.get(parts[2]);
            
            ModuleInfo info = new ModuleInfo(name, version, path);
            addToIndex(info);
            
        } catch (Exception e) {
            System.err.println("Invalid index line: " + line);
        }
    }
    
    private String formatIndexLine(ModuleInfo info) {
        return String.format("%s|%s|%s|%d",
            info.getName(),
            info.getVersion(),
            info.getPath(),
            info.getTimestamp()
        );
    }
    
    /**
     * 关闭仓库
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