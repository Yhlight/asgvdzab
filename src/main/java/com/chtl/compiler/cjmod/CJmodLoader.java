package com.chtl.compiler.cjmod;

import java.io.*;
import java.net.*;
import java.nio.file.*;
import java.util.*;
import java.util.jar.*;

/**
 * CJmod加载器
 * 负责从.cjmod文件中加载模块
 */
public class CJmodLoader {
    
    private final ClassLoader parentClassLoader;
    private final Map<String, CJmodInterface> loadedModules;
    
    public CJmodLoader() {
        this(CJmodLoader.class.getClassLoader());
    }
    
    public CJmodLoader(ClassLoader parentClassLoader) {
        this.parentClassLoader = parentClassLoader;
        this.loadedModules = new HashMap<>();
    }
    
    /**
     * 从.cjmod文件加载模块
     */
    public CJmodInterface loadModule(Path cjmodFile) throws IOException, ClassNotFoundException {
        if (!Files.exists(cjmodFile)) {
            throw new FileNotFoundException("CJmod file not found: " + cjmodFile);
        }
        
        // 检查是否已加载
        String moduleKey = cjmodFile.toAbsolutePath().toString();
        if (loadedModules.containsKey(moduleKey)) {
            return loadedModules.get(moduleKey);
        }
        
        // 读取manifest信息
        CJmodManifest manifest = readManifest(cjmodFile);
        
        // 创建模块类加载器
        URLClassLoader moduleClassLoader = new URLClassLoader(
            new URL[] { cjmodFile.toUri().toURL() },
            parentClassLoader
        );
        
        try {
            // 加载主类
            String mainClass = manifest.getMainClass();
            if (mainClass == null || mainClass.isEmpty()) {
                throw new IOException("Main-Class not specified in manifest");
            }
            
            Class<?> moduleClass = moduleClassLoader.loadClass(mainClass);
            
            // 验证是否实现了CJmodInterface
            if (!CJmodInterface.class.isAssignableFrom(moduleClass)) {
                throw new ClassCastException(
                    "Module class " + mainClass + " does not implement CJmodInterface"
                );
            }
            
            // 创建实例
            CJmodInterface module = (CJmodInterface) moduleClass.getDeclaredConstructor().newInstance();
            
            // 验证模块信息
            validateModule(module, manifest);
            
            // 缓存模块
            loadedModules.put(moduleKey, module);
            
            return module;
            
        } catch (ReflectiveOperationException e) {
            throw new IOException("Failed to instantiate module", e);
        }
    }
    
    /**
     * 从目录加载所有CJmod文件
     */
    public List<CJmodInterface> loadModulesFromDirectory(Path directory) throws IOException {
        List<CJmodInterface> modules = new ArrayList<>();
        
        if (!Files.isDirectory(directory)) {
            return modules;
        }
        
        Files.walk(directory, 1)
            .filter(path -> path.toString().endsWith(".cjmod"))
            .forEach(path -> {
                try {
                    modules.add(loadModule(path));
                } catch (Exception e) {
                    System.err.println("Failed to load module: " + path + " - " + e.getMessage());
                }
            });
        
        return modules;
    }
    
    /**
     * 读取CJmod的manifest信息
     */
    private CJmodManifest readManifest(Path cjmodFile) throws IOException {
        try (JarFile jarFile = new JarFile(cjmodFile.toFile())) {
            Manifest manifest = jarFile.getManifest();
            if (manifest == null) {
                throw new IOException("No manifest found in CJmod file");
            }
            
            Attributes attrs = manifest.getMainAttributes();
            
            CJmodManifest cjmodManifest = new CJmodManifest();
            cjmodManifest.setMainClass(attrs.getValue("Main-Class"));
            cjmodManifest.setModuleName(attrs.getValue("Module-Name"));
            cjmodManifest.setModuleVersion(attrs.getValue("Module-Version"));
            cjmodManifest.setDescription(attrs.getValue("Module-Description"));
            cjmodManifest.setAuthor(attrs.getValue("Module-Author"));
            cjmodManifest.setDependencies(attrs.getValue("Module-Dependencies"));
            
            return cjmodManifest;
        }
    }
    
    /**
     * 验证模块信息
     */
    private void validateModule(CJmodInterface module, CJmodManifest manifest) {
        // 验证模块名称
        if (!module.getModuleName().equals(manifest.getModuleName())) {
            throw new IllegalStateException(
                "Module name mismatch: " + module.getModuleName() + 
                " != " + manifest.getModuleName()
            );
        }
        
        // 验证版本
        if (!module.getModuleVersion().equals(manifest.getModuleVersion())) {
            throw new IllegalStateException(
                "Module version mismatch: " + module.getModuleVersion() + 
                " != " + manifest.getModuleVersion()
            );
        }
    }
    
    /**
     * 获取所有已加载的模块
     */
    public Collection<CJmodInterface> getLoadedModules() {
        return Collections.unmodifiableCollection(loadedModules.values());
    }
    
    /**
     * 卸载模块
     */
    public void unloadModule(String modulePath) {
        CJmodInterface module = loadedModules.remove(modulePath);
        if (module != null) {
            module.cleanup();
        }
    }
    
    /**
     * 卸载所有模块
     */
    public void unloadAll() {
        for (CJmodInterface module : loadedModules.values()) {
            module.cleanup();
        }
        loadedModules.clear();
    }
    
    /**
     * CJmod Manifest信息
     */
    public static class CJmodManifest {
        private String mainClass;
        private String moduleName;
        private String moduleVersion;
        private String description;
        private String author;
        private String dependencies;
        
        // Getters and Setters
        public String getMainClass() { return mainClass; }
        public void setMainClass(String mainClass) { this.mainClass = mainClass; }
        
        public String getModuleName() { return moduleName; }
        public void setModuleName(String moduleName) { this.moduleName = moduleName; }
        
        public String getModuleVersion() { return moduleVersion; }
        public void setModuleVersion(String moduleVersion) { this.moduleVersion = moduleVersion; }
        
        public String getDescription() { return description; }
        public void setDescription(String description) { this.description = description; }
        
        public String getAuthor() { return author; }
        public void setAuthor(String author) { this.author = author; }
        
        public String getDependencies() { return dependencies; }
        public void setDependencies(String dependencies) { this.dependencies = dependencies; }
    }
}