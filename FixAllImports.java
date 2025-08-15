import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;
import java.util.stream.*;

public class FixAllImports {
    // 映射：类名 -> 完整包路径
    private static final Map<String, String> CLASS_TO_PACKAGE = new HashMap<>();
    private static final Map<String, List<String>> FILE_REQUIRED_IMPORTS = new HashMap<>();
    
    public static void main(String[] args) throws IOException {
        System.out.println("=== Comprehensive Import Fix Tool ===\n");
        
        // Step 1: 扫描所有Java文件，建立类名到包的映射
        System.out.println("Step 1: Scanning all Java files to build class index...");
        buildClassIndex();
        
        // Step 2: 分析每个文件需要的导入
        System.out.println("\nStep 2: Analyzing import requirements for each file...");
        analyzeImportRequirements();
        
        // Step 3: 修复所有导入问题
        System.out.println("\nStep 3: Fixing all import issues...");
        fixAllImports();
        
        System.out.println("\n=== Import Fix Complete! ===");
    }
    
    static void buildClassIndex() throws IOException {
        Files.walk(Paths.get("src/main/java"))
            .filter(path -> path.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    
                    // 提取包名
                    Pattern packagePattern = Pattern.compile("package\\s+([\\w.]+);");
                    Matcher packageMatcher = packagePattern.matcher(content);
                    String packageName = "";
                    if (packageMatcher.find()) {
                        packageName = packageMatcher.group(1);
                    }
                    
                    // 提取类名
                    String fileName = path.getFileName().toString();
                    String className = fileName.substring(0, fileName.length() - 5);
                    
                    // 存储映射
                    CLASS_TO_PACKAGE.put(className, packageName + "." + className);
                    
                    // 同时提取内部类
                    Pattern classPattern = Pattern.compile("(?:public\\s+)?(?:static\\s+)?(?:final\\s+)?(?:abstract\\s+)?class\\s+(\\w+)");
                    Matcher classMatcher = classPattern.matcher(content);
                    while (classMatcher.find()) {
                        String innerClassName = classMatcher.group(1);
                        if (!innerClassName.equals(className)) {
                            CLASS_TO_PACKAGE.put(innerClassName, packageName + "." + className + "." + innerClassName);
                        }
                    }
                    
                    // 提取接口
                    Pattern interfacePattern = Pattern.compile("(?:public\\s+)?interface\\s+(\\w+)");
                    Matcher interfaceMatcher = interfacePattern.matcher(content);
                    while (interfaceMatcher.find()) {
                        String interfaceName = interfaceMatcher.group(1);
                        CLASS_TO_PACKAGE.put(interfaceName, packageName + "." + interfaceName);
                    }
                    
                    // 提取枚举
                    Pattern enumPattern = Pattern.compile("(?:public\\s+)?enum\\s+(\\w+)");
                    Matcher enumMatcher = enumPattern.matcher(content);
                    while (enumMatcher.find()) {
                        String enumName = enumMatcher.group(1);
                        CLASS_TO_PACKAGE.put(enumName, packageName + "." + enumName);
                    }
                    
                } catch (IOException e) {
                    System.err.println("Error reading " + path + ": " + e.getMessage());
                }
            });
        
        System.out.println("  Found " + CLASS_TO_PACKAGE.size() + " classes/interfaces/enums");
    }
    
    static void analyzeImportRequirements() throws IOException {
        // 标准Java类
        Map<String, String> standardClasses = new HashMap<>();
        standardClasses.put("List", "java.util.List");
        standardClasses.put("ArrayList", "java.util.ArrayList");
        standardClasses.put("Map", "java.util.Map");
        standardClasses.put("HashMap", "java.util.HashMap");
        standardClasses.put("LinkedHashMap", "java.util.LinkedHashMap");
        standardClasses.put("Set", "java.util.Set");
        standardClasses.put("HashSet", "java.util.HashSet");
        standardClasses.put("Collection", "java.util.Collection");
        standardClasses.put("Collections", "java.util.Collections");
        standardClasses.put("Iterator", "java.util.Iterator");
        standardClasses.put("Optional", "java.util.Optional");
        standardClasses.put("Objects", "java.util.Objects");
        standardClasses.put("Arrays", "java.util.Arrays");
        standardClasses.put("Stream", "java.util.stream.Stream");
        standardClasses.put("Collectors", "java.util.stream.Collectors");
        standardClasses.put("Consumer", "java.util.function.Consumer");
        standardClasses.put("Function", "java.util.function.Function");
        standardClasses.put("Supplier", "java.util.function.Supplier");
        standardClasses.put("Predicate", "java.util.function.Predicate");
        standardClasses.put("BiFunction", "java.util.function.BiFunction");
        
        // 并发相关
        standardClasses.put("ExecutorService", "java.util.concurrent.ExecutorService");
        standardClasses.put("Executors", "java.util.concurrent.Executors");
        standardClasses.put("Future", "java.util.concurrent.Future");
        standardClasses.put("CompletableFuture", "java.util.concurrent.CompletableFuture");
        standardClasses.put("ConcurrentHashMap", "java.util.concurrent.ConcurrentHashMap");
        standardClasses.put("CopyOnWriteArrayList", "java.util.concurrent.CopyOnWriteArrayList");
        standardClasses.put("BlockingQueue", "java.util.concurrent.BlockingQueue");
        standardClasses.put("LinkedBlockingQueue", "java.util.concurrent.LinkedBlockingQueue");
        standardClasses.put("CountDownLatch", "java.util.concurrent.CountDownLatch");
        standardClasses.put("Semaphore", "java.util.concurrent.Semaphore");
        standardClasses.put("AtomicInteger", "java.util.concurrent.atomic.AtomicInteger");
        standardClasses.put("AtomicLong", "java.util.concurrent.atomic.AtomicLong");
        standardClasses.put("AtomicBoolean", "java.util.concurrent.atomic.AtomicBoolean");
        standardClasses.put("AtomicReference", "java.util.concurrent.atomic.AtomicReference");
        standardClasses.put("ForkJoinPool", "java.util.concurrent.ForkJoinPool");
        standardClasses.put("RecursiveTask", "java.util.concurrent.RecursiveTask");
        standardClasses.put("RecursiveAction", "java.util.concurrent.RecursiveAction");
        standardClasses.put("ThreadPoolExecutor", "java.util.concurrent.ThreadPoolExecutor");
        standardClasses.put("ScheduledExecutorService", "java.util.concurrent.ScheduledExecutorService");
        standardClasses.put("TimeUnit", "java.util.concurrent.TimeUnit");
        
        // IO相关
        standardClasses.put("File", "java.io.File");
        standardClasses.put("IOException", "java.io.IOException");
        standardClasses.put("InputStream", "java.io.InputStream");
        standardClasses.put("OutputStream", "java.io.OutputStream");
        standardClasses.put("Reader", "java.io.Reader");
        standardClasses.put("Writer", "java.io.Writer");
        standardClasses.put("BufferedReader", "java.io.BufferedReader");
        standardClasses.put("BufferedWriter", "java.io.BufferedWriter");
        standardClasses.put("FileReader", "java.io.FileReader");
        standardClasses.put("FileWriter", "java.io.FileWriter");
        standardClasses.put("PrintWriter", "java.io.PrintWriter");
        standardClasses.put("Serializable", "java.io.Serializable");
        
        // NIO相关
        standardClasses.put("Path", "java.nio.file.Path");
        standardClasses.put("Paths", "java.nio.file.Paths");
        standardClasses.put("Files", "java.nio.file.Files");
        standardClasses.put("FileChannel", "java.nio.channels.FileChannel");
        standardClasses.put("ByteBuffer", "java.nio.ByteBuffer");
        standardClasses.put("MappedByteBuffer", "java.nio.MappedByteBuffer");
        standardClasses.put("StandardOpenOption", "java.nio.file.StandardOpenOption");
        standardClasses.put("WatchService", "java.nio.file.WatchService");
        standardClasses.put("WatchKey", "java.nio.file.WatchKey");
        standardClasses.put("WatchEvent", "java.nio.file.WatchEvent");
        
        // 安全相关
        standardClasses.put("AccessController", "java.security.AccessController");
        standardClasses.put("PrivilegedAction", "java.security.PrivilegedAction");
        standardClasses.put("MessageDigest", "java.security.MessageDigest");
        
        // 其他
        standardClasses.put("Pattern", "java.util.regex.Pattern");
        standardClasses.put("Matcher", "java.util.regex.Matcher");
        standardClasses.put("Logger", "org.slf4j.Logger");
        standardClasses.put("LoggerFactory", "org.slf4j.LoggerFactory");
        
        // ANTLR相关
        standardClasses.put("CharStream", "org.antlr.v4.runtime.CharStream");
        standardClasses.put("CharStreams", "org.antlr.v4.runtime.CharStreams");
        standardClasses.put("CommonTokenStream", "org.antlr.v4.runtime.CommonTokenStream");
        standardClasses.put("TokenStream", "org.antlr.v4.runtime.TokenStream");
        standardClasses.put("ParseTree", "org.antlr.v4.runtime.tree.ParseTree");
        standardClasses.put("ParseTreeWalker", "org.antlr.v4.runtime.tree.ParseTreeWalker");
        standardClasses.put("ParseTreeVisitor", "org.antlr.v4.runtime.tree.ParseTreeVisitor");
        standardClasses.put("AbstractParseTreeVisitor", "org.antlr.v4.runtime.tree.AbstractParseTreeVisitor");
        standardClasses.put("ParserRuleContext", "org.antlr.v4.runtime.ParserRuleContext");
        standardClasses.put("Lexer", "org.antlr.v4.runtime.Lexer");
        standardClasses.put("Parser", "org.antlr.v4.runtime.Parser");
        standardClasses.put("ATN", "org.antlr.v4.runtime.atn.ATN");
        
        // 合并所有类映射
        Map<String, String> allClasses = new HashMap<>();
        allClasses.putAll(standardClasses);
        allClasses.putAll(CLASS_TO_PACKAGE);
        
        // 分析每个文件
        Files.walk(Paths.get("src/main/java"))
            .filter(path -> path.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    analyzeFileImports(path, allClasses);
                } catch (IOException e) {
                    System.err.println("Error analyzing " + path + ": " + e.getMessage());
                }
            });
    }
    
    static void analyzeFileImports(Path path, Map<String, String> allClasses) throws IOException {
        String content = Files.readString(path);
        String packageName = "";
        
        // 获取当前包名
        Pattern packagePattern = Pattern.compile("package\\s+([\\w.]+);");
        Matcher packageMatcher = packagePattern.matcher(content);
        if (packageMatcher.find()) {
            packageName = packageMatcher.group(1);
        }
        
        // 获取已有的导入
        Set<String> existingImports = new HashSet<>();
        Pattern importPattern = Pattern.compile("import\\s+(?:static\\s+)?([\\w.]+);");
        Matcher importMatcher = importPattern.matcher(content);
        while (importMatcher.find()) {
            existingImports.add(importMatcher.group(1));
        }
        
        // 需要的导入
        List<String> requiredImports = new ArrayList<>();
        
        // 查找使用的类
        // 匹配模式：类名后跟空格、<、>、{、}、(、)、;、,、.等
        Pattern classUsagePattern = Pattern.compile("\\b([A-Z][\\w]*(?:\\.[A-Z][\\w]*)*)\\b(?=[\\s<>{}();,.\\[])");
        Matcher usageMatcher = classUsagePattern.matcher(content);
        
        Set<String> usedClasses = new HashSet<>();
        while (usageMatcher.find()) {
            String className = usageMatcher.group(1);
            
            // 跳过基本类型和当前包的类
            if (className.equals("String") || className.equals("Object") || 
                className.equals("Class") || className.equals("Exception") ||
                className.equals("RuntimeException") || className.equals("Error") ||
                className.equals("Throwable") || className.equals("System")) {
                continue;
            }
            
            usedClasses.add(className);
        }
        
        // 确定需要导入的类
        for (String className : usedClasses) {
            if (allClasses.containsKey(className)) {
                String fullClassName = allClasses.get(className);
                
                // 跳过同包的类
                if (fullClassName.startsWith(packageName + ".") && 
                    !fullClassName.substring(packageName.length() + 1).contains(".")) {
                    continue;
                }
                
                // 跳过java.lang包的类
                if (fullClassName.startsWith("java.lang.") && 
                    !fullClassName.substring(10).contains(".")) {
                    continue;
                }
                
                // 跳过已导入的
                if (existingImports.contains(fullClassName)) {
                    continue;
                }
                
                // 检查是否已经导入了通配符
                String wildcardImport = fullClassName.substring(0, fullClassName.lastIndexOf('.')) + ".*";
                if (existingImports.contains(wildcardImport)) {
                    continue;
                }
                
                requiredImports.add(fullClassName);
            }
        }
        
        if (!requiredImports.isEmpty()) {
            FILE_REQUIRED_IMPORTS.put(path.toString(), requiredImports);
        }
    }
    
    static void fixAllImports() throws IOException {
        int fixedCount = 0;
        
        for (Map.Entry<String, List<String>> entry : FILE_REQUIRED_IMPORTS.entrySet()) {
            Path path = Paths.get(entry.getKey());
            List<String> imports = entry.getValue();
            
            if (!imports.isEmpty()) {
                System.out.println("\nFixing imports in: " + path.getFileName());
                for (String imp : imports) {
                    System.out.println("  + import " + imp + ";");
                }
                
                addImportsToFile(path, imports);
                fixedCount++;
            }
        }
        
        System.out.println("\nFixed imports in " + fixedCount + " files");
    }
    
    static void addImportsToFile(Path path, List<String> imports) throws IOException {
        String content = Files.readString(path);
        
        // 找到插入位置（package声明之后）
        Pattern packagePattern = Pattern.compile("package\\s+[\\w.]+;");
        Matcher matcher = packagePattern.matcher(content);
        
        if (matcher.find()) {
            int insertPos = matcher.end();
            
            // 检查是否已有import语句
            Pattern firstImportPattern = Pattern.compile("\n\\s*import\\s+");
            Matcher importMatcher = firstImportPattern.matcher(content);
            if (importMatcher.find(insertPos)) {
                // 在第一个import之前插入
                insertPos = importMatcher.start();
            }
            
            // 构建导入语句
            StringBuilder importStatements = new StringBuilder();
            
            // 对导入进行排序和分组
            List<String> javaImports = imports.stream()
                .filter(imp -> imp.startsWith("java."))
                .sorted()
                .collect(Collectors.toList());
            
            List<String> javaxImports = imports.stream()
                .filter(imp -> imp.startsWith("javax."))
                .sorted()
                .collect(Collectors.toList());
            
            List<String> orgImports = imports.stream()
                .filter(imp -> imp.startsWith("org."))
                .sorted()
                .collect(Collectors.toList());
            
            List<String> projectImports = imports.stream()
                .filter(imp -> imp.startsWith("com.chtl."))
                .sorted()
                .collect(Collectors.toList());
            
            List<String> otherImports = imports.stream()
                .filter(imp -> !imp.startsWith("java.") && !imp.startsWith("javax.") && 
                              !imp.startsWith("org.") && !imp.startsWith("com.chtl."))
                .sorted()
                .collect(Collectors.toList());
            
            // 添加导入
            if (!javaImports.isEmpty()) {
                importStatements.append("\n");
                for (String imp : javaImports) {
                    importStatements.append("import ").append(imp).append(";\n");
                }
            }
            
            if (!javaxImports.isEmpty()) {
                if (importStatements.length() > 0) importStatements.append("\n");
                for (String imp : javaxImports) {
                    importStatements.append("import ").append(imp).append(";\n");
                }
            }
            
            if (!orgImports.isEmpty()) {
                if (importStatements.length() > 0) importStatements.append("\n");
                for (String imp : orgImports) {
                    importStatements.append("import ").append(imp).append(";\n");
                }
            }
            
            if (!projectImports.isEmpty()) {
                if (importStatements.length() > 0) importStatements.append("\n");
                for (String imp : projectImports) {
                    importStatements.append("import ").append(imp).append(";\n");
                }
            }
            
            if (!otherImports.isEmpty()) {
                if (importStatements.length() > 0) importStatements.append("\n");
                for (String imp : otherImports) {
                    importStatements.append("import ").append(imp).append(";\n");
                }
            }
            
            // 插入导入语句
            StringBuilder newContent = new StringBuilder(content);
            newContent.insert(insertPos, importStatements.toString());
            
            Files.write(path, newContent.toString().getBytes());
        }
    }
}