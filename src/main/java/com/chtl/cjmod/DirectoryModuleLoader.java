package com.chtl.cjmod;

import com.chtl.compiler.cjmod.CJmodLoader;
import com.chtl.compiler.cjmod.CJmodManager;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;
import java.util.stream.Stream;
import com.chtl.cjmod.CJmodManager.LoadedModule;
import com.chtl.cjmod.JavaJarLoader.ModuleDescriptor;
import com.chtl.compiler.cjmod.CJmodInterface;
import java.io.*;

/**
 * 目录模块加载器
 * 从目录结构加载CJMOD模块，支持开发时的热重载
 */
public class DirectoryModuleLoader implements CJmodManager.CJmodLoader {
    
    private static final String MODULE_DESCRIPTOR = "module.json";
    private static final String SOURCE_DIR = "src";
    private static final String LIB_DIR = "lib";
    private static final String RESOURCES_DIR = "resources";
    private static final String BUILD_DIR = "build";
    
    @Override
    public boolean canLoad(Path path) {
        return Files.isDirectory(path) && 
               Files.exists(path.resolve(MODULE_DESCRIPTOR));
    }
    
    @Override
    public CJmodManager.LoadedModule load(Path path) throws Exception {
        // 读取模块描述符
        ModuleDescriptor descriptor = readModuleDescriptor(path.resolve(MODULE_DESCRIPTOR));
        
        // 构建类路径
        List<URL> classpath = buildClasspath(path);
        
        // 创建类加载器
        URLClassLoader classLoader = new URLClassLoader(
            classpath.toArray(new URL[0]),
            getClass().getClassLoader()
        );
        
        // 编译源代码（如果需要）
        if (shouldCompile(path, descriptor)) {
            compileModule(path, descriptor, classpath);
        }
        
        // 加载主类
        Class<?> moduleClass = classLoader.loadClass(descriptor.mainClass);
        
        // 验证接口
        if (!CJmodInterface.class.isAssignableFrom(moduleClass)) {
            throw new IllegalArgumentException(
                "Module class does not implement CJmodInterface: " + moduleClass.getName()
            );
        }
        
        // 创建实例
        CJmodInterface module = (CJmodInterface) moduleClass.getDeclaredConstructor().newInstance();
        
        // 创建LoadedModule
        CJmodManager.LoadedModule loadedModule = new CJmodManager.LoadedModule(
            descriptor.name,
            descriptor.version,
            module,
            classLoader,
            path
        );
        
        // 添加模块元数据
        loadedModule.getMetadata().put("descriptor", descriptor);
        loadedModule.getMetadata().put("developmentMode", true);
        loadedModule.getMetadata().put("lastCompiled", System.currentTimeMillis());
        
        return loadedModule;
    }
    
    @Override
    public String getLoaderType() {
        return "DirectoryModuleLoader";
    }
    
    /**
     * 构建类路径
     */
    private List<URL> buildClasspath(Path moduleDir) throws Exception {
        List<URL> classpath = new ArrayList<Object>();
        
        // 添加build目录
        Path buildDir = moduleDir.resolve(BUILD_DIR);
        if (Files.exists(buildDir)) {
            classpath.add(buildDir.toUri().toURL());
        }
        
        // 添加源代码目录（用于资源文件）
        Path srcDir = moduleDir.resolve(SOURCE_DIR);
        if (Files.exists(srcDir)) {
            classpath.add(srcDir.toUri().toURL());
        }
        
        // 添加资源目录
        Path resourcesDir = moduleDir.resolve(RESOURCES_DIR);
        if (Files.exists(resourcesDir)) {
            classpath.add(resourcesDir.toUri().toURL());
        }
        
        // 添加lib目录中的所有JAR文件
        Path libDir = moduleDir.resolve(LIB_DIR);
        if (Files.exists(libDir) && Files.isDirectory(libDir)) {
            try (Stream<Path> jars = Files.list(libDir)) {
                jars.filter(p -> p.toString().endsWith(".jar"))
                    .forEach(jar -> {
                        try {
                            classpath.add(jar.toUri().toURL());
                        } catch (MalformedURLException e) {
                            System.err.println("Failed to add JAR to classpath: " + jar);
                        }
                    });
            }
        }
        
        return classpath;
    }
    
    /**
     * 检查是否需要编译
     */
    private boolean shouldCompile(Path moduleDir, ModuleDescriptor descriptor) throws IOException {
        Path srcDir = moduleDir.resolve(SOURCE_DIR);
        Path buildDir = moduleDir.resolve(BUILD_DIR);
        
        if (!Files.exists(srcDir)) {
            return false;
        }
        
        if (!Files.exists(buildDir)) {
            return true;
        }
        
        // 检查源文件是否比编译文件新
        long lastSourceModified = getLastModified(srcDir);
        long lastBuildModified = getLastModified(buildDir);
        
        return lastSourceModified > lastBuildModified;
    }
    
    /**
     * 获取目录中最新修改时间
     */
    private long getLastModified(Path dir) throws IOException {
        try (Stream<Path> paths = Files.walk(dir)) {
            return paths.filter(Files::isRegularFile)
                       .mapToLong(p -> {
                           try {
                               return Files.getLastModifiedTime(p).toMillis();
                           } catch (IOException e) {
                               return 0;
                           }
                       })
                       .max()
                       .orElse(0);
        }
    }
    
    /**
     * 编译模块
     */
    private void compileModule(Path moduleDir, ModuleDescriptor descriptor, 
                              List<URL> classpath) throws Exception {
        Path srcDir = moduleDir.resolve(SOURCE_DIR);
        Path buildDir = moduleDir.resolve(BUILD_DIR);
        
        // 创建build目录
        Files.createDirectories(buildDir);
        
        // 收集所有Java文件
        List<Path> javaFiles = new ArrayList<Object>();
        try (Stream<Path> paths = Files.walk(srcDir)) {
            paths.filter(p -> p.toString().endsWith(".java"))
                 .forEach(javaFiles::add);
        }
        
        if (javaFiles.isEmpty()) {
            return;
        }
        
        // 构建编译命令
        List<String> command = new ArrayList<Object>();
        command.add("javac");
        command.add("-d");
        command.add(buildDir.toString());
        command.add("-cp");
        command.add(buildClasspathString(classpath));
        
        // 添加源文件
        for (Path javaFile : javaFiles) {
            command.add(javaFile.toString());
        }
        
        // 执行编译
        ProcessBuilder pb = new ProcessBuilder(command);
        pb.directory(moduleDir.toFile());
        pb.redirectErrorStream(true);
        
        Process process = pb.start();
        
        // 读取输出
        try (BufferedReader reader = new BufferedReader(
                new InputStreamReader(process.getInputStream()))) {
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println("[Compile] " + line);
            }
        }
        
        int exitCode = process.waitFor();
        if (exitCode != 0) {
            throw new RuntimeException("Compilation failed with exit code: " + exitCode);
        }
        
        // 复制资源文件
        copyResources(srcDir, buildDir);
    }
    
    /**
     * 构建类路径字符串
     */
    private String buildClasspathString(List<URL> classpath) {
        return classpath.stream()
            .map(url -> {
                try {
                    return Paths.get(url.toURI()).toString();
                } catch (URISyntaxException e) {
                    return url.getPath();
                }
            })
            .collect(Collectors.joining(File.pathSeparator));
    }
    
    /**
     * 复制资源文件
     */
    private void copyResources(Path srcDir, Path buildDir) throws IOException {
        try (Stream<Path> paths = Files.walk(srcDir)) {
            paths.filter(Files::isRegularFile)
                 .filter(p -> !p.toString().endsWith(".java"))
                 .forEach(src -> {
                     Path relative = srcDir.relativize(src);
                     Path dest = buildDir.resolve(relative);
                     try {
                         Files.createDirectories(dest.getParent());
                         Files.copy(src, dest, StandardCopyOption.REPLACE_EXISTING);
                     } catch (IOException e) {
                         System.err.println("Failed to copy resource: " + src);
                     }
                 });
        }
    }
    
    /**
     * 读取模块描述符
     */
    private ModuleDescriptor readModuleDescriptor(Path descriptorPath) throws Exception {
        String content = Files.readString(descriptorPath);
        return parseModuleDescriptor(content);
    }
    
    /**
     * 解析模块描述符
     */
    private ModuleDescriptor parseModuleDescriptor(String json) throws Exception {
        // 简单的JSON解析实现
        ModuleDescriptor descriptor = new ModuleDescriptor();
        
        // 提取字段
        descriptor.name = extractJsonField(json, "name");
        descriptor.version = extractJsonField(json, "version");
        descriptor.mainClass = extractJsonField(json, "mainClass");
        descriptor.description = extractJsonField(json, "description");
        descriptor.author = extractJsonField(json, "author");
        descriptor.license = extractJsonField(json, "license");
        
        // 解析依赖
        String depsStr = extractJsonField(json, "dependencies");
        if (depsStr != null && !depsStr.isEmpty()) {
            descriptor.dependencies = Arrays.asList(depsStr.split(","));
        } else {
            descriptor.dependencies = new ArrayList<Object>();
        }
        
        // 解析元数据
        descriptor.metadata = new HashMap<Object, Object>();
        String metaStr = extractJsonField(json, "metadata");
        if (metaStr != null) {
            // 简单解析metadata对象
            descriptor.metadata.put("raw", metaStr);
        }
        
        // 验证必需字段
        if (descriptor.name == null || descriptor.name.isEmpty()) {
            throw new IllegalArgumentException("Module name is required");
        }
        if (descriptor.version == null || descriptor.version.isEmpty()) {
            descriptor.version = "1.0.0";
        }
        if (descriptor.mainClass == null || descriptor.mainClass.isEmpty()) {
            throw new IllegalArgumentException("Main class is required");
        }
        
        return descriptor;
    }
    
    /**
     * 从JSON字符串中提取字段值
     */
    private String extractJsonField(String json, String field) {
        String pattern = "\"" + field + "\"\\s*:\\s*\"([^\"]+)\"";
        java.util.regex.Pattern p = java.util.regex.Pattern.compile(pattern);
        java.util.regex.Matcher m = p.matcher(json);
        
        if (m.find()) {
            return m.group(1);
        }
        
        return null;
    }
    
    /**
     * 模块描述符
     */
    public static class ModuleDescriptor {
        public String name;
        public String version;
        public String mainClass;
        public String description;
        public String author;
        public String license;
        public List<String> dependencies;
        public Map<String, String> metadata;
        
        @Override
        public String toString() {
            return String.format("Module[name=%s, version=%s, mainClass=%s]", 
                                name, version, mainClass);
        }
    }

    @Override
    public void loadModuleFromPath() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void IllegalStateException() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void ModuleNotFoundException() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void DefaultExecutionContext() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void onModuleLoaded() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void onModuleInitialized() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void onModuleSuspended() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void Thread() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void onModuleUnloaded() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void onModuleActivated() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void UnsupportedModuleException() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void onModuleError() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }
}
