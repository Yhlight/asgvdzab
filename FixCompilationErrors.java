import java.io.*;
import java.nio.file.*;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.regex.*;

public class FixCompilationErrors {
    private static int filesFixed = 0;
    private static List<String> errors = new ArrayList<>();

    public static void main(String[] args) throws IOException {
        System.out.println("Fixing compilation errors...");
        
        // Fix NodeType.java - remove duplicate enum declaration
        fixNodeType();
        
        // Fix JavaScriptCompiler.java - fix imports
        fixJavaScriptCompiler();
        
        // Fix duplicate imports in all files
        fixDuplicateImports();
        
        // Fix CHTLParser issues
        fixCHTLParser();
        
        // Fix other common issues
        fixCommonIssues();
        
        System.out.println("\nFixed " + filesFixed + " files");
        if (!errors.isEmpty()) {
            System.out.println("\nErrors encountered:");
            errors.forEach(System.out::println);
        }
    }
    
    private static void fixNodeType() {
        try {
            Path path = Paths.get("src/main/java/com/chtl/ast/NodeType.java");
            if (!Files.exists(path)) return;
            
            String content = Files.readString(path);
            
            // Remove the class declaration and keep only the enum
            content = content.replaceAll("import com\\.chtl\\.ast\\.AbstractCHTLASTNode;\\s*", "");
            content = content.replaceAll("import com\\.chtl\\.ast\\.NodeType;\\s*", "");
            content = content.replaceAll("public class NodeType extends AbstractCHTLASTNode \\{\\s*public enum NodeType \\{", "public enum NodeType {");
            
            // Remove extra closing brace at the end
            content = content.replaceAll("\\}\\s*\\}\\s*$", "}");
            
            Files.writeString(path, content);
            filesFixed++;
            System.out.println("Fixed: " + path);
        } catch (IOException e) {
            errors.add("Error fixing NodeType.java: " + e.getMessage());
        }
    }
    
    private static void fixJavaScriptCompiler() {
        try {
            Path path = Paths.get("src/main/java/com/chtl/compiler/javascript/JavaScriptCompiler.java");
            if (!Files.exists(path)) return;
            
            List<String> lines = Files.readAllLines(path);
            List<String> fixedLines = new ArrayList<>();
            Set<String> imports = new HashSet<>();
            boolean inImports = false;
            
            for (String line : lines) {
                // Fix the first line if it has import statement merged with package
                if (line.startsWith("package com.chtl.compiler.javascript;import")) {
                    fixedLines.add("package com.chtl.compiler.javascript;");
                    fixedLines.add("");
                    imports.add("import org.antlr.v4.runtime.CharStreams;");
                    inImports = true;
                    continue;
                }
                
                // Handle import section
                if (line.trim().startsWith("import ")) {
                    inImports = true;
                    String importLine = line.trim();
                    
                    // Skip problematic imports
                    if (importLine.contains("JavaScriptCompiler.ECMAScriptParser") ||
                        importLine.contains("JavaScriptCompiler.JavaScriptOptimizingVisitor")) {
                        continue;
                    }
                    
                    // Avoid duplicate imports
                    if (!imports.contains(importLine)) {
                        imports.add(importLine);
                    }
                    continue;
                }
                
                // End of imports
                if (inImports && !line.trim().isEmpty() && !line.trim().startsWith("import")) {
                    inImports = false;
                    
                    // Add all collected imports
                    for (String imp : imports) {
                        fixedLines.add(imp);
                    }
                    fixedLines.add("");
                }
                
                if (!inImports) {
                    fixedLines.add(line);
                }
            }
            
            Files.write(path, fixedLines);
            filesFixed++;
            System.out.println("Fixed: " + path);
        } catch (IOException e) {
            errors.add("Error fixing JavaScriptCompiler.java: " + e.getMessage());
        }
    }
    
    private static void fixCommonIssues() throws IOException {
        // Fix files with duplicate class definitions
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    String fileName = path.getFileName().toString();
                    String className = fileName.substring(0, fileName.length() - 5);
                    
                    // Check for duplicate CompilationResult definitions
                    if (content.contains("class CompilationResult") && 
                        content.contains("CompilationResult is already defined")) {
                        // This file might have issues, check imports
                        List<String> lines = Files.readAllLines(path);
                        List<String> fixedLines = new ArrayList<>();
                        Set<String> seenImports = new HashSet<>();
                        
                        for (String line : lines) {
                            if (line.trim().startsWith("import ")) {
                                if (!seenImports.contains(line.trim())) {
                                    seenImports.add(line.trim());
                                    fixedLines.add(line);
                                }
                            } else {
                                fixedLines.add(line);
                            }
                        }
                        
                        Files.write(path, fixedLines);
                        filesFixed++;
                        System.out.println("Fixed duplicate imports in: " + path);
                    }
                    
                    // Fix missing package declarations
                    if (!content.trim().startsWith("package ")) {
                        String packageName = getPackageFromPath(path);
                        content = "package " + packageName + ";\n\n" + content;
                        Files.writeString(path, content);
                        filesFixed++;
                        System.out.println("Added package declaration to: " + path);
                    }
                } catch (IOException e) {
                    errors.add("Error processing " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static String getPackageFromPath(Path path) {
        String pathStr = path.toString().replace('\\', '/');
        int srcIndex = pathStr.indexOf("src/main/java/");
        if (srcIndex >= 0) {
            String packagePath = pathStr.substring(srcIndex + 14, pathStr.lastIndexOf('/'));
            return packagePath.replace('/', '.');
        }
        return "com.chtl";
    }
    
    private static void fixDuplicateImports() throws IOException {
        System.out.println("Fixing duplicate imports in all files...");
        
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    List<String> lines = Files.readAllLines(path);
                    List<String> fixedLines = new ArrayList<>();
                    Set<String> imports = new LinkedHashSet<>(); // Preserve order
                    Map<String, String> classToImport = new HashMap<>();
                    boolean inImports = false;
                    boolean modified = false;
                    
                    for (String line : lines) {
                        if (line.trim().startsWith("package ")) {
                            fixedLines.add(line);
                            continue;
                        }
                        
                        if (line.trim().startsWith("import ")) {
                            inImports = true;
                            String importLine = line.trim();
                            
                            // Extract class name from import
                            String className = importLine.substring(importLine.lastIndexOf('.') + 1, importLine.length() - 1);
                            
                            // Check for duplicate class imports
                            if (classToImport.containsKey(className)) {
                                // Skip duplicate import
                                modified = true;
                                continue;
                            }
                            
                            imports.add(importLine);
                            classToImport.put(className, importLine);
                            continue;
                        }
                        
                        if (inImports && !line.trim().isEmpty() && !line.trim().startsWith("import")) {
                            // End of imports, add them
                            for (String imp : imports) {
                                fixedLines.add(imp);
                            }
                            if (!imports.isEmpty()) {
                                fixedLines.add(""); // Empty line after imports
                            }
                            inImports = false;
                        }
                        
                        if (!inImports) {
                            fixedLines.add(line);
                        }
                    }
                    
                    if (modified) {
                        Files.write(path, fixedLines);
                        filesFixed++;
                        System.out.println("Fixed duplicate imports in: " + path.getFileName());
                    }
                } catch (IOException e) {
                    errors.add("Error fixing imports in " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static void fixCHTLParser() {
        try {
            Path path = Paths.get("src/main/java/com/chtl/compiler/chtl/CHTLParser.java");
            if (!Files.exists(path)) return;
            
            String content = Files.readString(path);
            
            // Remove problematic imports
            content = content.replaceAll("import com\\.chtl\\.compiler\\.chtl\\.CHTLParser\\.Lexer;\\s*", "");
            content = content.replaceAll("import com\\.chtl\\.compiler\\.chtl\\.CHTLParser\\.Token;\\s*", "");
            
            Files.writeString(path, content);
            filesFixed++;
            System.out.println("Fixed: " + path);
            
            // Also fix ECMAScriptLexer if it has similar issues
            path = Paths.get("src/main/java/com/chtl/javascript/ECMAScriptLexer.java");
            if (Files.exists(path)) {
                content = Files.readString(path);
                content = content.replaceAll("import com\\.chtl\\.compiler\\.chtl\\.CHTLParser\\.Lexer;\\s*", "");
                Files.writeString(path, content);
                filesFixed++;
                System.out.println("Fixed: " + path);
            }
        } catch (IOException e) {
            errors.add("Error fixing CHTLParser: " + e.getMessage());
        }
    }
}