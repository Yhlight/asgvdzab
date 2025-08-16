import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;
import java.util.stream.*;

public class FixMoreCompilationIssues {
    private static int filesFixed = 0;
    private static Map<String, String> classLocationMap = new HashMap<>();
    
    public static void main(String[] args) throws IOException {
        System.out.println("Fixing more compilation issues...");
        
        // First, build a map of all classes and their locations
        buildClassLocationMap();
        
        // Fix import issues based on actual class locations
        fixImportIssues();
        
        // Fix generic type issues
        fixGenericTypeIssues();
        
        // Fix method signature issues
        fixMethodSignatureIssues();
        
        // Fix interface implementation issues
        fixInterfaceImplementations();
        
        // Fix enum and constant issues
        fixEnumAndConstantIssues();
        
        System.out.println("\nFixed " + filesFixed + " files");
    }
    
    private static void buildClassLocationMap() throws IOException {
        System.out.println("Building class location map...");
        
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    String packageName = extractPackage(content);
                    String className = path.getFileName().toString().replace(".java", "");
                    
                    if (packageName != null) {
                        String fullClassName = packageName + "." + className;
                        classLocationMap.put(className, fullClassName);
                        
                        // Also extract inner classes and interfaces
                        Pattern classPattern = Pattern.compile("(?:public\\s+)?(?:static\\s+)?(?:class|interface|enum)\\s+(\\w+)");
                        Matcher matcher = classPattern.matcher(content);
                        while (matcher.find()) {
                            String innerClass = matcher.group(1);
                            if (!innerClass.equals(className)) {
                                classLocationMap.put(innerClass, packageName + "." + innerClass);
                            }
                        }
                    }
                } catch (IOException e) {
                    // Ignore
                }
            });
        
        System.out.println("Found " + classLocationMap.size() + " classes");
    }
    
    private static void fixImportIssues() throws IOException {
        System.out.println("Fixing import issues...");
        
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    boolean modified = false;
                    
                    // Find all unresolved symbols
                    List<String> lines = Files.readAllLines(path);
                    Set<String> missingImports = new HashSet<>();
                    
                    for (String line : lines) {
                        // Skip comments and imports
                        if (line.trim().startsWith("//") || line.trim().startsWith("/*") || 
                            line.trim().startsWith("*") || line.trim().startsWith("import")) {
                            continue;
                        }
                        
                        // Look for class names that might need imports
                        Pattern classNamePattern = Pattern.compile("\\b([A-Z][a-zA-Z0-9_]*)\\b");
                        Matcher matcher = classNamePattern.matcher(line);
                        
                        while (matcher.find()) {
                            String className = matcher.group(1);
                            
                            // Check if this class needs an import
                            if (classLocationMap.containsKey(className) && 
                                !content.contains("import " + classLocationMap.get(className)) &&
                                !content.contains("class " + className) &&
                                !content.contains("interface " + className) &&
                                !content.contains("enum " + className)) {
                                
                                String fullClassName = classLocationMap.get(className);
                                // Don't import from same package
                                String currentPackage = extractPackage(content);
                                if (currentPackage != null && !fullClassName.startsWith(currentPackage + ".")) {
                                    missingImports.add("import " + fullClassName + ";");
                                    modified = true;
                                }
                            }
                        }
                    }
                    
                    if (modified) {
                        content = addImportsAfterPackage(content, missingImports);
                        Files.writeString(path, content);
                        filesFixed++;
                        System.out.println("Fixed imports in: " + path.getFileName());
                    }
                } catch (IOException e) {
                    System.err.println("Error processing " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static void fixGenericTypeIssues() throws IOException {
        System.out.println("Fixing generic type issues...");
        
        Map<String, String> genericFixes = new HashMap<>();
        genericFixes.put("List<>", "List<Object>");
        genericFixes.put("Set<>", "Set<Object>");
        genericFixes.put("Map<>", "Map<Object, Object>");
        genericFixes.put("Optional<>", "Optional<Object>");
        genericFixes.put("CompletableFuture<>", "CompletableFuture<Object>");
        genericFixes.put("Stream<>", "Stream<Object>");
        
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    boolean modified = false;
                    
                    for (Map.Entry<String, String> entry : genericFixes.entrySet()) {
                        if (content.contains(entry.getKey())) {
                            content = content.replace(entry.getKey(), entry.getValue());
                            modified = true;
                        }
                    }
                    
                    // Fix raw types
                    content = content.replaceAll("\\bnew\\s+ArrayList\\(\\)", "new ArrayList<>()");
                    content = content.replaceAll("\\bnew\\s+HashMap\\(\\)", "new HashMap<>()");
                    content = content.replaceAll("\\bnew\\s+HashSet\\(\\)", "new HashSet<>()");
                    
                    if (modified || !content.equals(Files.readString(path))) {
                        Files.writeString(path, content);
                        filesFixed++;
                        System.out.println("Fixed generic types in: " + path.getFileName());
                    }
                } catch (IOException e) {
                    System.err.println("Error processing " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static void fixMethodSignatureIssues() throws IOException {
        System.out.println("Fixing method signature issues...");
        
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    boolean modified = false;
                    
                    // Fix visitor methods with missing parameters
                    Pattern visitorMethodPattern = Pattern.compile(
                        "(public\\s+\\w+\\s+visit\\w+)\\(\\s*\\)\\s*\\{");
                    Matcher matcher = visitorMethodPattern.matcher(content);
                    
                    StringBuffer sb = new StringBuffer();
                    while (matcher.find()) {
                        String methodStart = matcher.group(1);
                        matcher.appendReplacement(sb, methodStart + "(ParserRuleContext ctx) {");
                        modified = true;
                    }
                    matcher.appendTail(sb);
                    
                    if (modified) {
                        content = sb.toString();
                        // Ensure ParserRuleContext is imported
                        if (!content.contains("import org.antlr.v4.runtime.ParserRuleContext;")) {
                            content = addImportAfterPackage(content, "import org.antlr.v4.runtime.ParserRuleContext;");
                        }
                        Files.writeString(path, content);
                        filesFixed++;
                        System.out.println("Fixed method signatures in: " + path.getFileName());
                    }
                } catch (IOException e) {
                    System.err.println("Error processing " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static void fixInterfaceImplementations() throws IOException {
        System.out.println("Fixing interface implementations...");
        
        // Common interface methods that need implementation
        Map<String, String> interfaceMethods = new HashMap<>();
        interfaceMethods.put("CHTLNode", 
            "\n    @Override\n" +
            "    public void accept(CHTLNodeVisitor visitor) {\n" +
            "        visitor.visit(this);\n" +
            "    }\n");
        
        interfaceMethods.put("Visitable",
            "\n    @Override\n" +
            "    public void accept(Visitor visitor) {\n" +
            "        visitor.visit(this);\n" +
            "    }\n");
        
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    boolean modified = false;
                    
                    for (Map.Entry<String, String> entry : interfaceMethods.entrySet()) {
                        String interfaceName = entry.getKey();
                        String methodImpl = entry.getValue();
                        
                        if (content.contains("implements " + interfaceName) && 
                            !content.contains("accept(") && 
                            content.contains("class ")) {
                            
                            // Add the method implementation before the last closing brace
                            int lastBrace = content.lastIndexOf("}");
                            if (lastBrace > 0) {
                                content = content.substring(0, lastBrace) + 
                                        methodImpl + "\n" + 
                                        content.substring(lastBrace);
                                modified = true;
                            }
                        }
                    }
                    
                    if (modified) {
                        Files.writeString(path, content);
                        filesFixed++;
                        System.out.println("Fixed interface implementations in: " + path.getFileName());
                    }
                } catch (IOException e) {
                    System.err.println("Error processing " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static void fixEnumAndConstantIssues() throws IOException {
        System.out.println("Fixing enum and constant issues...");
        
        // Fix files with enum issues
        Path nodeTypePath = Paths.get("src/main/java/com/chtl/ast/NodeType.java");
        if (Files.exists(nodeTypePath)) {
            String content = Files.readString(nodeTypePath);
            
            // Ensure it's properly formatted as an enum
            if (!content.contains("public enum NodeType {") && content.contains("enum NodeType {")) {
                content = content.replace("enum NodeType {", "public enum NodeType {");
                Files.writeString(nodeTypePath, content);
                filesFixed++;
                System.out.println("Fixed NodeType enum");
            }
        }
        
        // Fix constant definitions
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    boolean modified = false;
                    
                    // Fix static final constants without initialization
                    Pattern constantPattern = Pattern.compile(
                        "(public\\s+static\\s+final\\s+\\w+\\s+\\w+)\\s*;");
                    Matcher matcher = constantPattern.matcher(content);
                    
                    StringBuffer sb = new StringBuffer();
                    while (matcher.find()) {
                        String constantDecl = matcher.group(1);
                        // Add default initialization based on type
                        if (constantDecl.contains("String")) {
                            matcher.appendReplacement(sb, constantDecl + " = \"\";");
                        } else if (constantDecl.contains("int") || constantDecl.contains("Integer")) {
                            matcher.appendReplacement(sb, constantDecl + " = 0;");
                        } else if (constantDecl.contains("boolean") || constantDecl.contains("Boolean")) {
                            matcher.appendReplacement(sb, constantDecl + " = false;");
                        } else {
                            matcher.appendReplacement(sb, constantDecl + " = null;");
                        }
                        modified = true;
                    }
                    matcher.appendTail(sb);
                    
                    if (modified) {
                        Files.writeString(path, sb.toString());
                        filesFixed++;
                        System.out.println("Fixed constants in: " + path.getFileName());
                    }
                } catch (IOException e) {
                    System.err.println("Error processing " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static String extractPackage(String content) {
        Pattern packagePattern = Pattern.compile("package\\s+([\\w\\.]+)\\s*;");
        Matcher matcher = packagePattern.matcher(content);
        if (matcher.find()) {
            return matcher.group(1);
        }
        return null;
    }
    
    private static String addImportsAfterPackage(String content, Set<String> imports) {
        int packageEnd = content.indexOf(';');
        if (packageEnd > 0) {
            StringBuilder sb = new StringBuilder();
            sb.append(content.substring(0, packageEnd + 1));
            sb.append("\n");
            for (String imp : imports) {
                sb.append("\n").append(imp);
            }
            sb.append("\n");
            sb.append(content.substring(packageEnd + 1));
            return sb.toString();
        }
        return content;
    }
    
    private static String addImportAfterPackage(String content, String importStmt) {
        return addImportsAfterPackage(content, Collections.singleton(importStmt));
    }
}