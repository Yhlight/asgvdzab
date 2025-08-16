package com.chtl.compiler.cjmod;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;
import java.io.*;

/**
 * CJmod打包工具
 * 将Java类打包成.cjmod文件
 */
public class CJmodPackager {
    
    /**
     * 打包CJmod模块
     */
    public static void packageModule(Path sourceDir, Path outputFile, 
                                   String mainClass, Map<String, String> moduleInfo) 
            throws IOException {
        
        // 创建manifest
        Manifest manifest = createManifest(mainClass, moduleInfo);
        
        // 创建JAR文件
        try (JarOutputStream jos = new JarOutputStream(
                new BufferedOutputStream(Files.newOutputStream(outputFile)), manifest)) {
            
            // 添加所有class文件
            addClassFiles(sourceDir, sourceDir, jos);
            
            // 添加资源文件
            addResourceFiles(sourceDir, sourceDir, jos);
        }
        
        System.out.println("CJmod package created: " + outputFile);
    }
    
    /**
     * 创建manifest
     */
    private static Manifest createManifest(String mainClass, Map<String, String> moduleInfo) {
        Manifest manifest = new Manifest();
        Attributes attrs = manifest.getMainAttributes();
        
        // 必需的manifest版本
        attrs.put(Attributes.Name.MANIFEST_VERSION, "1.0");
        
        // 主类
        attrs.put(Attributes.Name.MAIN_CLASS, mainClass);
        
        // 模块信息
        attrs.putValue("Module-Name", moduleInfo.getOrDefault("name", "Unknown"));
        attrs.putValue("Module-Version", moduleInfo.getOrDefault("version", "1.0.0"));
        attrs.putValue("Module-Description", moduleInfo.getOrDefault("description", ""));
        attrs.putValue("Module-Author", moduleInfo.getOrDefault("author", "Unknown"));
        
        // 依赖（如果有）
        String dependencies = moduleInfo.get("dependencies");
        if (dependencies != null && !dependencies.isEmpty()) {
            attrs.putValue("Module-Dependencies", dependencies);
        }
        
        // 创建时间
        attrs.putValue("Created-By", System.getProperty("java.version") + " (" + 
                      System.getProperty("java.vendor") + ")");
        attrs.putValue("Created-Time", new Date().toString());
        
        return manifest;
    }
    
    /**
     * 添加class文件
     */
    private static void addClassFiles(Path baseDir, Path currentDir, JarOutputStream jos) 
            throws IOException {
        
        Files.walk(currentDir)
            .filter(Files::isRegularFile)
            .filter(path -> path.toString().endsWith(".class"))
            .forEach(path -> {
                try {
                    String entryName = baseDir.relativize(path).toString()
                                              .replace(File.separatorChar, '/');
                    
                    JarEntry entry = new JarEntry(entryName);
                    entry.setTime(Files.getLastModifiedTime(path).toMillis());
                    jos.putNextEntry(entry);
                    
                    Files.copy(path, jos);
                    jos.closeEntry();
                } catch (IOException e) {
                    throw new UncheckedIOException(e);
                }
            });
    }
    
    /**
     * 添加资源文件
     */
    private static void addResourceFiles(Path baseDir, Path currentDir, JarOutputStream jos) 
            throws IOException {
        
        // 查找resources目录
        Path resourcesDir = currentDir.resolve("resources");
        if (!Files.exists(resourcesDir)) {
            return;
        }
        
        Files.walk(resourcesDir)
            .filter(Files::isRegularFile)
            .forEach(path -> {
                try {
                    String entryName = baseDir.relativize(path).toString()
                                              .replace(File.separatorChar, '/');
                    
                    JarEntry entry = new JarEntry(entryName);
                    entry.setTime(Files.getLastModifiedTime(path).toMillis());
                    jos.putNextEntry(entry);
                    
                    Files.copy(path, jos);
                    jos.closeEntry();
                } catch (IOException e) {
                    throw new UncheckedIOException(e);
                }
            });
    }
    
    /**
     * 从已编译的类创建CJmod
     */
    public static void createFromCompiledClasses(Path classesDir, Path outputFile,
                                                String mainClass, String moduleName,
                                                String moduleVersion, String description,
                                                String author) throws IOException {
        
        Map<String, String> moduleInfo = new HashMap<>();
        moduleInfo.put("name", moduleName);
        moduleInfo.put("version", moduleVersion);
        moduleInfo.put("description", description);
        moduleInfo.put("author", author);
        
        packageModule(classesDir, outputFile, mainClass, moduleInfo);
    }
    
    /**
     * 主方法 - 命令行工具
     */
    public static void main(String[] args) {
        if (args.length < 6) {
            System.out.println("Usage: CJmodPackager <classesDir> <outputFile> <mainClass> " +
                             "<moduleName> <version> <author> [description]");
            System.exit(1);
        }
        
        try {
            Path classesDir = Paths.get(args[0]);
            Path outputFile = Paths.get(args[1]);
            String mainClass = args[2];
            String moduleName = args[3];
            String version = args[4];
            String author = args[5];
            String description = args.length > 6 ? args[6] : "";
            
            createFromCompiledClasses(classesDir, outputFile, mainClass,
                                    moduleName, version, description, author);
            
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        }
    }
}
