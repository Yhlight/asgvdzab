package com.chtl.doc;

import com.chtl.cjmod.Parameter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.io.*;

/**
 * CHTL API 文档生成器
 * 根据代码注释和结构自动生成API文档
 */
public class CHTLApiDocGenerator {
    private final Path outputDir;
    private final List<ApiClass> apiClasses = new ArrayList<Object>();
    
    public CHTLApiDocGenerator(Path outputDir) throws IOException {
        this.outputDir = outputDir;
        Files.createDirectories(outputDir);
    }
    
    /**
     * 扫描包并生成文档
     */
    public void generateDocs(String... packageNames) throws IOException {
        System.out.println("=== CHTL API 文档生成 ===\n");
        
        // 扫描所有类
        for (String packageName : packageNames) {
            scanPackage(packageName);
        }
        
        // 生成文档
        generateIndexPage();
        for (ApiClass apiClass : apiClasses) {
            generateClassPage(apiClass);
        }
        
        // 生成CSS
        generateCSS();
        
        System.out.println("\nAPI文档已生成到: " + outputDir);
    }
    
    /**
     * 扫描包中的类
     */
    private void scanPackage(String packageName) {
        String packagePath = packageName.replace('.', '/');
        try {
            ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
            Enumeration<URL> resources = classLoader.getResources(packagePath);
            
            while (resources.hasMoreElements()) {
                URL resource = resources.nextElement();
                Path path = Paths.get(resource.toURI());
                if (Files.isDirectory(path)) {
                    scanDirectory(path, packageName);
                }
            }
        } catch (Exception e) {
            System.err.println("扫描包失败: " + packageName);
        }
    }
    
    /**
     * 扫描目录
     */
    private void scanDirectory(Path dir, String packageName) throws IOException {
        Files.walk(dir)
            .filter(path -> path.toString().endsWith(".java"))
            .forEach(path -> {
                String className = extractClassName(path, packageName);
                try {
                    Class<?> clazz = Class.forName(className);
                    if (isPublicApi(clazz)) {
                        apiClasses.add(analyzeClass(clazz));
                    }
                } catch (Exception e) {
                    // 忽略无法加载的类
                }
            });
    }
    
    /**
     * 判断是否是公共API
     */
    private boolean isPublicApi(Class<?> clazz) {
        return Modifier.isPublic(clazz.getModifiers()) &&
               !clazz.isAnonymousClass() &&
               !clazz.isSynthetic();
    }
    
    /**
     * 分析类
     */
    private ApiClass analyzeClass(Class<?> clazz) {
        ApiClass apiClass = new ApiClass();
        apiClass.name = clazz.getSimpleName();
        apiClass.fullName = clazz.getName();
        apiClass.packageName = clazz.getPackage().getName();
        apiClass.description = getClassDescription(clazz);
        
        // 分析构造函数
        for (Constructor<?> constructor : clazz.getConstructors()) {
            if (Modifier.isPublic(constructor.getModifiers())) {
                apiClass.constructors.add(analyzeConstructor(constructor));
            }
        }
        
        // 分析方法
        for (Method method : clazz.getMethods()) {
            if (Modifier.isPublic(method.getModifiers()) &&
                !method.getDeclaringClass().equals(Object.class)) {
                apiClass.methods.add(analyzeMethod(method));
            }
        }
        
        // 分析字段
        for (Field field : clazz.getFields()) {
            if (Modifier.isPublic(field.getModifiers())) {
                apiClass.fields.add(analyzeField(field));
            }
        }
        
        return apiClass;
    }
    
    /**
     * 生成索引页
     */
    private void generateIndexPage() throws IOException {
        Path indexPath = outputDir.resolve("index.html");
        try (PrintWriter writer = new PrintWriter(new FileWriter(indexPath.toFile()))) {
            writer.println("<!DOCTYPE html>");
            writer.println("<html>");
            writer.println("<head>");
            writer.println("<meta charset=\"UTF-8\">");
            writer.println("<title>CHTL API 文档</title>");
            writer.println("<link rel=\"stylesheet\" href=\"style.css\">");
            writer.println("</head>");
            writer.println("<body>");
            
            writer.println("<div class='container'>");
            writer.println("<h1>CHTL API 文档</h1>");
            writer.println("<p>CHTL (Custom HyperText Language) 编译器 API 参考文档</p>");
            
            // 按包分组
            Map<String, List<ApiClass>> byPackage = new TreeMap<Object, Object>();
            for (ApiClass apiClass : apiClasses) {
                byPackage.computeIfAbsent(apiClass.packageName, k -> new ArrayList<Object>())
                    .add(apiClass);
            }
            
            writer.println("<h2>包列表</h2>");
            for (Map.Entry<String, List<ApiClass>> entry : byPackage.entrySet()) {
                writer.println("<div class='package'>");
                writer.println("<h3>" + entry.getKey() + "</h3>");
                writer.println("<ul>");
                
                List<ApiClass> classes = entry.getValue();
                classes.sort(Comparator.comparing(c -> c.name));
                
                for (ApiClass apiClass : classes) {
                    writer.println("<li><a href='" + apiClass.fullName + ".html'>" + 
                        apiClass.name + "</a> - " + apiClass.description + "</li>");
                }
                
                writer.println("</ul>");
                writer.println("</div>");
            }
            
            writer.println("</div>");
            writer.println("</body>");
            writer.println("</html>");
        }
    }
    
    /**
     * 生成类文档页
     */
    private void generateClassPage(ApiClass apiClass) throws IOException {
        Path classPath = outputDir.resolve(apiClass.fullName + ".html");
        try (PrintWriter writer = new PrintWriter(new FileWriter(classPath.toFile()))) {
            writer.println("<!DOCTYPE html>");
            writer.println("<html>");
            writer.println("<head>");
            writer.println("<meta charset=\"UTF-8\">");
            writer.println("<title>" + apiClass.name + " - CHTL API</title>");
            writer.println("<link rel=\"stylesheet\" href=\"style.css\">");
            writer.println("</head>");
            writer.println("<body>");
            
            writer.println("<div class='container'>");
            writer.println("<div class='breadcrumb'>");
            writer.println("<a href='index.html'>CHTL API</a> / ");
            writer.println("<a href='index.html#" + apiClass.packageName + "'>" + 
                apiClass.packageName + "</a> / ");
            writer.println(apiClass.name);
            writer.println("</div>");
            
            writer.println("<h1>类 " + apiClass.name + "</h1>");
            writer.println("<div class='class-info'>");
            writer.println("<p><strong>包:</strong> " + apiClass.packageName + "</p>");
            writer.println("<p><strong>全名:</strong> " + apiClass.fullName + "</p>");
            if (!apiClass.description.isEmpty()) {
                writer.println("<p><strong>描述:</strong> " + apiClass.description + "</p>");
            }
            writer.println("</div>");
            
            // 构造函数
            if (!apiClass.constructors.isEmpty()) {
                writer.println("<h2>构造函数</h2>");
                writer.println("<div class='members'>");
                for (ApiMember constructor : apiClass.constructors) {
                    writeMember(writer, constructor);
                }
                writer.println("</div>");
            }
            
            // 方法
            if (!apiClass.methods.isEmpty()) {
                writer.println("<h2>方法</h2>");
                writer.println("<div class='members'>");
                apiClass.methods.sort(Comparator.comparing(m -> m.name));
                for (ApiMember method : apiClass.methods) {
                    writeMember(writer, method);
                }
                writer.println("</div>");
            }
            
            // 字段
            if (!apiClass.fields.isEmpty()) {
                writer.println("<h2>字段</h2>");
                writer.println("<div class='members'>");
                apiClass.fields.sort(Comparator.comparing(f -> f.name));
                for (ApiMember field : apiClass.fields) {
                    writeMember(writer, field);
                }
                writer.println("</div>");
            }
            
            writer.println("</div>");
            writer.println("</body>");
            writer.println("</html>");
        }
    }
    
    /**
     * 写入成员信息
     */
    private void writeMember(PrintWriter writer, ApiMember member) {
        writer.println("<div class='member'>");
        writer.println("<h3>" + member.name + "</h3>");
        writer.println("<div class='signature'><code>" + member.signature + "</code></div>");
        if (!member.description.isEmpty()) {
            writer.println("<p>" + member.description + "</p>");
        }
        if (!member.parameters.isEmpty()) {
            writer.println("<h4>参数:</h4>");
            writer.println("<ul>");
            for (String param : member.parameters) {
                writer.println("<li>" + param + "</li>");
            }
            writer.println("</ul>");
        }
        if (!member.returnType.isEmpty() && !member.returnType.equals("void")) {
            writer.println("<p><strong>返回:</strong> " + member.returnType + "</p>");
        }
        writer.println("</div>");
    }
    
    /**
     * 生成CSS
     */
    private void generateCSS() throws IOException {
        Path cssPath = outputDir.resolve("style.css");
        try (PrintWriter writer = new PrintWriter(new FileWriter(cssPath.toFile()))) {
            writer.println("""
                body {
                    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
                    margin: 0;
                    padding: 0;
                    background-color: #f5f5f5;
                }
                .container {
                    max-width: 1200px;
                    margin: 0 auto;
                    padding: 20px;
                    background: white;
                    min-height: 100vh;
                }
                h1 {
                    color: #333;
                    border-bottom: 2px solid #007bff;
                    padding-bottom: 10px;
                }
                h2 {
                    color: #555;
                    margin-top: 30px;
                }
                h3 {
                    color: #007bff;
                    margin: 0;
                }
                .breadcrumb {
                    color: #666;
                    margin-bottom: 20px;
                }
                .breadcrumb a {
                    color: #007bff;
                    text-decoration: none;
                }
                .package {
                    margin-bottom: 30px;
                    padding: 20px;
                    background: #f9f9f9;
                    border-radius: 8px;
                }
                .package h3 {
                    margin-top: 0;
                }
                .package ul {
                    list-style: none;
                    padding-left: 0;
                }
                .package li {
                    margin: 5px 0;
                }
                .class-info {
                    background: #f0f0f0;
                    padding: 15px;
                    border-radius: 8px;
                    margin: 20px 0;
                }
                .member {
                    margin-bottom: 30px;
                    padding: 20px;
                    border: 1px solid #e0e0e0;
                    border-radius: 8px;
                }
                .signature {
                    background: #f5f5f5;
                    padding: 10px;
                    border-radius: 4px;
                    margin: 10px 0;
                    overflow-x: auto;
                }
                code {
                    font-family: 'Consolas', 'Monaco', monospace;
                    font-size: 14px;
                }
                a {
                    color: #007bff;
                }
                """);
        }
    }
    
    // 辅助方法
    private String extractClassName(Path path, String packageName) {
        String fileName = path.getFileName().toString();
        String className = fileName.substring(0, fileName.length() - 5); // 去掉.java
        return packageName + "." + className;
    }
    
    private String getClassDescription(Class<?> clazz) {
        // 简化实现，实际应该从源码注释中提取
        return "CHTL " + clazz.getSimpleName() + " 类";
    }
    
    private ApiMember analyzeConstructor(Constructor<?> constructor) {
        ApiMember member = new ApiMember();
        member.name = constructor.getDeclaringClass().getSimpleName();
        member.signature = constructor.toGenericString();
        member.description = "构造函数";
        
        for (Parameter param : constructor.getParameters()) {
            member.parameters.add(param.getType().getSimpleName() + " " + param.getName());
        }
        
        return member;
    }
    
    private ApiMember analyzeMethod(Method method) {
        ApiMember member = new ApiMember();
        member.name = method.getName();
        member.signature = method.toGenericString();
        member.returnType = method.getReturnType().getSimpleName();
        member.description = "方法 " + method.getName();
        
        for (Parameter param : method.getParameters()) {
            member.parameters.add(param.getType().getSimpleName() + " " + param.getName());
        }
        
        return member;
    }
    
    private ApiMember analyzeField(Field field) {
        ApiMember member = new ApiMember();
        member.name = field.getName();
        member.signature = Modifier.toString(field.getModifiers()) + " " + 
            field.getType().getSimpleName() + " " + field.getName();
        member.description = "字段 " + field.getName();
        return member;
    }
    
    /**
     * API类信息
     */
    private static class ApiClass {
        String name;
        String fullName;
        String packageName;
        String description;
        List<ApiMember> constructors = new ArrayList<Object>();
        List<ApiMember> methods = new ArrayList<Object>();
        List<ApiMember> fields = new ArrayList<Object>();
    }
    
    /**
     * API成员信息
     */
    private static class ApiMember {
        String name;
        String signature;
        String description;
        String returnType = "";
        List<String> parameters = new ArrayList<Object>();
    }
    
    /**
     * 主方法
     */
    public static void main(String[] args) throws IOException {
        Path docDir = Paths.get("api-docs");
        CHTLApiDocGenerator generator = new CHTLApiDocGenerator(docDir);
        
        // 生成主要包的API文档
        generator.generateDocs(
            "com.chtl.ast",
            "com.chtl.ast.node",
            "com.chtl.compiler",
            "com.chtl.context",
            "com.chtl.lexer",
            "com.chtl.parser",
            "com.chtl.scanner",
            "com.chtl.generator",
            "com.chtl.compiler.debug",
            "com.chtl.compiler.optimization",
            "com.chtl.test"
        );
    }
}
