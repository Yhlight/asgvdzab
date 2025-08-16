import java.io.*;
import java.nio.file.*;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.regex.*;

public class FixAdvancedCompilationIssues {
    private static int filesFixed = 0;
    private static List<String> errors = new ArrayList<>();
    
    public static void main(String[] args) throws IOException {
        System.out.println("Fixing advanced compilation issues...");
        
        // Fix CHTLJSContext
        fixCHTLJSContext();
        
        // Fix all files with "cannot find symbol" errors
        fixCannotFindSymbolErrors();
        
        // Fix circular dependencies
        fixCircularDependencies();
        
        // Fix abstract method implementations
        fixAbstractMethodImplementations();
        
        // Add missing annotations
        addMissingAnnotations();
        
        System.out.println("\nFixed " + filesFixed + " files");
        if (!errors.isEmpty()) {
            System.out.println("\nErrors encountered:");
            errors.forEach(System.out::println);
        }
    }
    
    private static void fixCHTLJSContext() {
        try {
            Path path = Paths.get("src/main/java/com/chtl/context/CHTLJSContext.java");
            if (!Files.exists(path)) return;
            
            String content = Files.readString(path);
            
            // Fix the commented out DelegationInfo class
            content = content.replace("// public static class DelegationInfo { // TODO: Fix DelegationInfo access",
                                    "public static class DelegationInfo {");
            content = content.replace("// public DelegationInfo(String parentSelector, List<String> targetSelectors, // TODO: Fix DelegationInfo access",
                                    "public DelegationInfo(String parentSelector, List<String> targetSelectors,");
            
            // Make sure the class is properly closed
            // Count braces to ensure proper structure
            int openBraces = 0;
            int closeBraces = 0;
            for (char c : content.toCharArray()) {
                if (c == '{') openBraces++;
                if (c == '}') closeBraces++;
            }
            
            if (openBraces > closeBraces) {
                content += "\n}".repeat(openBraces - closeBraces);
            }
            
            Files.writeString(path, content);
            filesFixed++;
            System.out.println("Fixed: CHTLJSContext.java");
        } catch (IOException e) {
            errors.add("Error fixing CHTLJSContext: " + e.getMessage());
        }
    }
    
    private static void fixCannotFindSymbolErrors() throws IOException {
        Map<String, String> commonMissingImports = new HashMap<>();
        commonMissingImports.put("List", "import java.util.List;");
        commonMissingImports.put("ArrayList", "import java.util.ArrayList;");
        commonMissingImports.put("Map", "import java.util.Map;");
        commonMissingImports.put("HashMap", "import java.util.HashMap;");
        commonMissingImports.put("Set", "import java.util.Set;");
        commonMissingImports.put("HashSet", "import java.util.HashSet;");
        commonMissingImports.put("Optional", "import java.util.Optional;");
        commonMissingImports.put("Stream", "import java.util.stream.Stream;");
        commonMissingImports.put("Collectors", "import java.util.stream.Collectors;");
        commonMissingImports.put("Path", "import java.nio.file.Path;");
        commonMissingImports.put("Paths", "import java.nio.file.Paths;");
        commonMissingImports.put("Files", "import java.nio.file.Files;");
        commonMissingImports.put("IOException", "import java.io.IOException;");
        commonMissingImports.put("Logger", "import org.slf4j.Logger;");
        commonMissingImports.put("LoggerFactory", "import org.slf4j.LoggerFactory;");
        
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    boolean modified = false;
                    Set<String> importsToAdd = new HashSet<>();
                    
                    // Check for missing symbols
                    for (Map.Entry<String, String> entry : commonMissingImports.entrySet()) {
                        String symbol = entry.getKey();
                        String importStmt = entry.getValue();
                        
                        // Check if symbol is used but not imported
                        Pattern symbolPattern = Pattern.compile("\\b" + symbol + "\\b");
                        if (symbolPattern.matcher(content).find() && 
                            !content.contains(importStmt) &&
                            !content.contains("import java.util.*;")) {
                            importsToAdd.add(importStmt);
                            modified = true;
                        }
                    }
                    
                    if (modified) {
                        content = addImportsAfterPackage(content, importsToAdd);
                        Files.writeString(path, content);
                        filesFixed++;
                        System.out.println("Added missing imports to: " + path.getFileName());
                    }
                } catch (IOException e) {
                    errors.add("Error processing " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static void fixCircularDependencies() throws IOException {
        // Fix known circular dependencies
        Map<String, List<String>> circularDeps = new HashMap<>();
        circularDeps.put("com/chtl/compiler/CompilerResult.java", 
            Arrays.asList("com.chtl.model.CompilationResult"));
        
        for (Map.Entry<String, List<String>> entry : circularDeps.entrySet()) {
            Path path = Paths.get("src/main/java/" + entry.getKey());
            if (!Files.exists(path)) continue;
            
            String content = Files.readString(path);
            boolean modified = false;
            
            for (String badImport : entry.getValue()) {
                if (content.contains("import " + badImport + ";")) {
                    content = content.replace("import " + badImport + ";", "");
                    modified = true;
                }
            }
            
            if (modified) {
                Files.writeString(path, content);
                filesFixed++;
                System.out.println("Fixed circular dependencies in: " + path.getFileName());
            }
        }
    }
    
    private static void fixAbstractMethodImplementations() throws IOException {
        // Find interfaces and their implementations
        Map<String, Set<String>> interfaceMethods = new HashMap<>();
        
        // First pass: collect interface methods
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    if (content.contains("interface ") && !content.contains("@interface")) {
                        String interfaceName = extractClassName(path);
                        Set<String> methods = extractInterfaceMethods(content);
                        if (!methods.isEmpty()) {
                            interfaceMethods.put(interfaceName, methods);
                        }
                    }
                } catch (IOException e) {
                    // Ignore
                }
            });
        
        // Second pass: fix implementations
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    boolean modified = false;
                    
                    // Check if this implements any interfaces
                    for (Map.Entry<String, Set<String>> entry : interfaceMethods.entrySet()) {
                        String interfaceName = entry.getKey();
                        if (content.contains("implements " + interfaceName) ||
                            content.contains("implements .*" + interfaceName)) {
                            
                            // Check for missing methods
                            for (String method : entry.getValue()) {
                                if (!content.contains(method)) {
                                    // Add stub implementation
                                    String stub = generateMethodStub(method);
                                    int lastBrace = content.lastIndexOf('}');
                                    if (lastBrace > 0) {
                                        content = content.substring(0, lastBrace) + 
                                                "\n" + stub + "\n" + 
                                                content.substring(lastBrace);
                                        modified = true;
                                    }
                                }
                            }
                        }
                    }
                    
                    if (modified) {
                        Files.writeString(path, content);
                        filesFixed++;
                        System.out.println("Added missing method implementations to: " + path.getFileName());
                    }
                } catch (IOException e) {
                    errors.add("Error processing " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static void addMissingAnnotations() throws IOException {
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    boolean modified = false;
                    
                    // Add @Override annotations where missing
                    Pattern overridePattern = Pattern.compile("\\n\\s+(public|protected|private)?\\s*(\\w+\\s+)?\\w+\\s*\\([^)]*\\)\\s*\\{");
                    Matcher matcher = overridePattern.matcher(content);
                    
                    StringBuffer sb = new StringBuffer();
                    while (matcher.find()) {
                        String method = matcher.group();
                        if (isOverrideMethod(method) && !isPrecededByOverride(content, matcher.start())) {
                            matcher.appendReplacement(sb, "\n    @Override" + method);
                            modified = true;
                        } else {
                            matcher.appendReplacement(sb, method);
                        }
                    }
                    matcher.appendTail(sb);
                    
                    if (modified) {
                        Files.writeString(path, sb.toString());
                        filesFixed++;
                        System.out.println("Added @Override annotations to: " + path.getFileName());
                    }
                } catch (IOException e) {
                    errors.add("Error processing " + path + ": " + e.getMessage());
                }
            });
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
    
    private static String extractClassName(Path path) {
        String fileName = path.getFileName().toString();
        return fileName.substring(0, fileName.length() - 5);
    }
    
    private static Set<String> extractInterfaceMethods(String content) {
        Set<String> methods = new HashSet<>();
        Pattern methodPattern = Pattern.compile("\\s+(\\w+)\\s+(\\w+)\\s*\\([^)]*\\)\\s*;");
        Matcher matcher = methodPattern.matcher(content);
        while (matcher.find()) {
            methods.add(matcher.group(2)); // method name
        }
        return methods;
    }
    
    private static String generateMethodStub(String methodName) {
        return "    @Override\n" +
               "    public void " + methodName + "() {\n" +
               "        // TODO: Implement this method\n" +
               "        throw new UnsupportedOperationException(\"Not implemented yet\");\n" +
               "    }";
    }
    
    private static boolean isOverrideMethod(String method) {
        return method.contains("toString") || method.contains("equals") || 
               method.contains("hashCode") || method.contains("clone");
    }
    
    private static boolean isPrecededByOverride(String content, int position) {
        int overridePos = content.lastIndexOf("@Override", position);
        if (overridePos < 0) return false;
        
        String between = content.substring(overridePos + 9, position).trim();
        return between.isEmpty() || between.matches("\\s*");
    }
}