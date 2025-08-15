package com.chtl.service;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.ForkJoinPool;
import java.util.function.Consumer;

import com.chtl.service.IncrementalCompiler.DependencyGraph;
import com.chtl.service.IncrementalCompiler.FileWatcher;


import com.chtl.compiler.CHTLCompiler;
import com.chtl.compiler.CompilationResult;
import com.chtl.core.cache.CompilationCache;
import com.chtl.context.CompilationContext;

import java.io.IOException;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;

/**
 * 增量编译器
 * 只编译修改的文件及其依赖
 */
public class IncrementalCompiler {
    private final CHTLCompiler compiler;
    private final CompilationCache cache;
    private final DependencyGraph dependencyGraph;
    private final FileWatcher fileWatcher;
    
    // 文件修改时间戳
    private final Map<String, Long> fileTimestamps = new ConcurrentHashMap<>();
    
    // 并行执行器
    private final ExecutorService executorService;
    
    public IncrementalCompiler(CHTLCompiler compiler, CompilationCache cache) {
        this.compiler = compiler;
        this.cache = cache;
        this.dependencyGraph = new DependencyGraph();
        this.fileWatcher = new FileWatcher();
        this.executorService = ForkJoinPool.commonPool();
    }
    
    /**
     * 执行增量编译
     */
    public CompilerService.IncrementalResult compile(CompilerService.IncrementalCompileRequest request) {
        CompilerService.IncrementalResult result = new CompilerService.IncrementalResult();
        result.recompiledFiles = new ArrayList<>();
        result.outputs = new HashMap<>();
        result.success = true;
        
        try {
            // 1. 识别需要重新编译的文件
            Set<String> filesToCompile = identifyFilesToCompile(request.changedFiles);
            
            // 2. 并行编译这些文件
            Map<String, CompletableFuture<CompilationResult>> futures = new HashMap<>();
            
            for (String file : filesToCompile) {
                futures.put(file, CompletableFuture.supplyAsync(() -> {
                    try {
                        // 更新文件内容（如果提供）
                        String content = request.fileContents.get(file);
                        if (content != null) {
                            // 使用提供的内容
                            return compileContent(file, content);
                        } else {
                            // 从文件系统读取
                            return compiler.compile(file);
                        }
                    } catch (Exception e) {
                        CompilationResult errorResult = new CompilationResult();
                        errorResult.addError(e.getMessage());
                        return errorResult;
                    }
                }, executorService));
            }
            
            // 3. 收集结果
            for (Map.Entry<String, CompletableFuture<CompilationResult>> entry : futures.entrySet()) {
                String file = entry.getKey();
                try {
                    CompilationResult compilationResult = entry.getValue().get();
                    if (compilationResult.isSuccess()) {
                        result.recompiledFiles.add(file);
                        result.outputs.put(file, compilationResult.getOutput());
                        
                        // 更新缓存
                        String content = request.fileContents.getOrDefault(file, 
                            Files.readString(Paths.get(file)));
                        cache.putCompiledOutput(file, content, compilationResult.getOutput());
                        
                        // 更新依赖图
                        updateDependencies(file, compilationResult.getDependencies());
                    } else {
                        result.success = false;
                    }
                } catch (Exception e) {
                    result.success = false;
                }
            }
            
            // 4. 更新时间戳
            updateTimestamps(filesToCompile);
            
        } catch (Exception e) {
            result.success = false;
        }
        
        return result;
    }
    
    /**
     * 识别需要重新编译的文件
     */
    private Set<String> identifyFilesToCompile(List<String> changedFiles) {
        Set<String> filesToCompile = new HashSet<>(changedFiles);
        
        // 添加所有依赖于修改文件的文件
        for (String changedFile : changedFiles) {
            filesToCompile.addAll(dependencyGraph.getDependents(changedFile));
        }
        
        // 检查是否有新的依赖关系
        filesToCompile.addAll(checkForNewDependencies(changedFiles));
        
        return filesToCompile;
    }
    
    /**
     * 检查新的依赖关系
     */
    private Set<String> checkForNewDependencies(List<String> files) {
        Set<String> newDependencies = new HashSet<>();
        
        for (String file : files) {
            try {
                // 快速扫描import语句
                List<String> imports = scanImports(file);
                for (String importedFile : imports) {
                    if (!dependencyGraph.hasDependency(file, importedFile)) {
                        newDependencies.add(importedFile);
                    }
                }
            } catch (IOException e) {
                // 忽略扫描错误
            }
        }
        
        return newDependencies;
    }
    
    /**
     * 扫描import语句
     */
    private List<String> scanImports(String file) throws IOException {
        List<String> imports = new ArrayList<>();
        List<String> lines = Files.readAllLines(Paths.get(file));
        
        for (String line : lines) {
            line = line.trim();
            if (line.startsWith("[Import]")) {
                // 提取import路径
                int fromIndex = line.indexOf("from");
                if (fromIndex != -1) {
                    String path = extractPath(line.substring(fromIndex + 4));
                    if (path != null) {
                        imports.add(resolveImportPath(file, path));
                    }
                }
            }
        }
        
        return imports;
    }
    
    /**
     * 提取路径
     */
    private String extractPath(String text) {
        text = text.trim();
        if (text.startsWith("\"") && text.contains("\"", 1)) {
            return text.substring(1, text.indexOf("\"", 1));
        }
        if (text.startsWith("'") && text.contains("'", 1)) {
            return text.substring(1, text.indexOf("'", 1));
        }
        return null;
    }
    
    /**
     * 解析import路径
     */
    private String resolveImportPath(String currentFile, String importPath) {
        Path current = Paths.get(currentFile).getParent();
        Path resolved = current.resolve(importPath).normalize();
        return resolved.toString();
    }
    
    /**
     * 编译内容
     */
    private CompilationResult compileContent(String file, String content) {
        CompilationContext context = new CompilationContext();
        // 实际编译逻辑
        return compiler.compileContent(content, file, context);
    }
    
    /**
     * 更新依赖关系
     */
    private void updateDependencies(String file, Set<String> dependencies) {
        dependencyGraph.updateDependencies(file, dependencies);
    }
    
    /**
     * 更新时间戳
     */
    private void updateTimestamps(Set<String> files) {
        for (String file : files) {
            try {
                long timestamp = Files.getLastModifiedTime(Paths.get(file)).toMillis();
                fileTimestamps.put(file, timestamp);
            } catch (IOException e) {
                // 忽略
            }
        }
    }
    
    /**
     * 启动文件监视
     */
    public void startWatching(Path directory) throws IOException {
        fileWatcher.watch(directory, this::onFileChanged);
    }
    
    /**
     * 文件变化处理
     */
    private void onFileChanged(Path file) {
        if (file.toString().endsWith(".chtl")) {
            // 触发增量编译
            CompilerService.IncrementalCompileRequest request = new CompilerService.IncrementalCompileRequest();
            request.changedFiles = Arrays.asList(file.toString());
            request.fileContents = new HashMap<>();
            
            compile(request);
        }
    }
    
    /**
     * 依赖图
     */
    private static class DependencyGraph {
        // file -> 它依赖的文件
        private final Map<String, Set<String>> dependencies = new ConcurrentHashMap<>();
        // file -> 依赖它的文件
        private final Map<String, Set<String>> dependents = new ConcurrentHashMap<>();
        
        public void updateDependencies(String file, Set<String> deps) {
            // 清除旧的依赖
            Set<String> oldDeps = dependencies.get(file);
            if (oldDeps != null) {
                for (String dep : oldDeps) {
                    dependents.get(dep).remove(file);
                }
            }
            
            // 添加新的依赖
            dependencies.put(file, new HashSet<>(deps));
            for (String dep : deps) {
                dependents.computeIfAbsent(dep, k -> new HashSet<>()).add(file);
            }
        }
        
        public Set<String> getDependents(String file) {
            return dependents.getOrDefault(file, Collections.emptySet());
        }
        
        public boolean hasDependency(String file, String dependency) {
            Set<String> deps = dependencies.get(file);
            return deps != null && deps.contains(dependency);
        }
    }
    
    /**
     * 文件监视器
     */
    private static class FileWatcher {
        private WatchService watchService;
        private final Map<WatchKey, Path> watchKeys = new HashMap<>();
        private Thread watchThread;
        
        public void watch(Path directory, Consumer<Path> onChange) throws IOException {
            this.watchService = FileSystems.getDefault().newWatchService();
            
            // 注册目录
            registerDirectory(directory);
            
            // 启动监视线程
            watchThread = new Thread(() -> {
                try {
                    while (!Thread.currentThread().isInterrupted()) {
                        WatchKey key = watchService.take();
                        Path dir = watchKeys.get(key);
                        
                        if (dir != null) {
                            for (WatchEvent<?> event : key.pollEvents()) {
                                WatchEvent.Kind<?> kind = event.kind();
                                
                                if (kind == StandardWatchEventKinds.OVERFLOW) {
                                    continue;
                                }
                                
                                Path filename = (Path) event.context();
                                Path child = dir.resolve(filename);
                                
                                // 处理文件变化
                                if (kind == StandardWatchEventKinds.ENTRY_MODIFY) {
                                    onChange.accept(child);
                                }
                                
                                // 如果是新目录，注册它
                                if (kind == StandardWatchEventKinds.ENTRY_CREATE &&
                                    Files.isDirectory(child)) {
                                    registerDirectory(child);
                                }
                            }
                        }
                        
                        key.reset();
                    }
                } catch (InterruptedException e) {
                    // 正常退出
                } catch (Exception e) {
                    e.printStackTrace();
                }
            });
            
            watchThread.setDaemon(true);
            watchThread.start();
        }
        
        private void registerDirectory(Path directory) throws IOException {
            Files.walkFileTree(directory, new SimpleFileVisitor<Path>() {
                @Override
                public FileVisitResult preVisitDirectory(Path dir, BasicFileAttributes attrs) 
                        throws IOException {
                    WatchKey key = dir.register(watchService,
                        StandardWatchEventKinds.ENTRY_CREATE,
                        StandardWatchEventKinds.ENTRY_DELETE,
                        StandardWatchEventKinds.ENTRY_MODIFY);
                    watchKeys.put(key, dir);
                    return FileVisitResult.CONTINUE;
                }
            });
        }
        
        public void stop() {
            if (watchThread != null) {
                watchThread.interrupt();
            }
            try {
                if (watchService != null) {
                    watchService.close();
                }
            } catch (IOException e) {
                // 忽略
            }
        }
    }
}