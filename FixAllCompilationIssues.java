import java.io.*;
import java.nio.file.*;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.regex.*;

public class FixAllCompilationIssues {
    private static int filesFixed = 0;
    private static List<String> errors = new ArrayList<>();

    public static void main(String[] args) throws IOException {
        System.out.println("Fixing all compilation issues...");
        
        // Fix ECMAScriptParser imports
        fixECMAScriptParser();
        
        // Fix duplicate class definitions
        fixDuplicateClasses();
        
        // Fix private access issues
        fixPrivateAccessIssues();
        
        // Fix JavaScriptCompiler
        fixJavaScriptCompilerComprehensively();
        
        // Remove all problematic self-imports
        removeSelfImports();
        
        System.out.println("\nFixed " + filesFixed + " files");
        if (!errors.isEmpty()) {
            System.out.println("\nErrors encountered:");
            errors.forEach(System.out::println);
        }
    }
    
    private static void fixECMAScriptParser() {
        try {
            Path path = Paths.get("src/main/java/com/chtl/javascript/ECMAScriptParser.java");
            if (!Files.exists(path)) return;
            
            List<String> lines = Files.readAllLines(path);
            List<String> fixedLines = new ArrayList<>();
            
            for (String line : lines) {
                // Skip problematic imports
                if (line.contains("import com.chtl.compiler.javascript.JavaScriptCompiler.ECMAScriptParser") ||
                    line.contains("import com.chtl.javascript.ECMAScriptParser.")) {
                    continue;
                }
                fixedLines.add(line);
            }
            
            Files.write(path, fixedLines);
            filesFixed++;
            System.out.println("Fixed: ECMAScriptParser.java");
        } catch (IOException e) {
            errors.add("Error fixing ECMAScriptParser: " + e.getMessage());
        }
    }
    
    private static void fixDuplicateClasses() throws IOException {
        // Fix CompilationResult duplicate
        Path modelPath = Paths.get("src/main/java/com/chtl/model/CompilationResult.java");
        Path compilerPath = Paths.get("src/main/java/com/chtl/compiler/CompilationResult.java");
        
        if (Files.exists(modelPath) && Files.exists(compilerPath)) {
            // Keep model version, update compiler version to use different name
            String content = Files.readString(compilerPath);
            content = content.replace("class CompilationResult", "class CompilerResult");
            content = content.replace("public CompilationResult", "public CompilerResult");
            Files.writeString(compilerPath, content);
            filesFixed++;
            System.out.println("Renamed duplicate CompilationResult in compiler package");
        }
        
        // Fix SourceLocation duplicate
        Path astPath = Paths.get("src/main/java/com/chtl/ast/SourceLocation.java");
        Path modelSourcePath = Paths.get("src/main/java/com/chtl/model/SourceLocation.java");
        
        if (Files.exists(astPath) && Files.exists(modelSourcePath)) {
            // Keep model version, delete AST version
            Files.deleteIfExists(astPath);
            filesFixed++;
            System.out.println("Removed duplicate SourceLocation from ast package");
        }
    }
    
    private static void fixPrivateAccessIssues() {
        try {
            Path path = Paths.get("src/main/java/com/chtl/compiler/chtljs/CHTLJSSystem.java");
            if (!Files.exists(path)) {
                return;
            }
            
            List<String> lines = Files.readAllLines(path);
            List<String> fixedLines = new ArrayList<>();
            
            for (String line : lines) {
                // Remove problematic imports
                if (line.contains("import com.chtl.compiler.chtljs.CHTLJSSystem.DelegationInfo") ||
                    line.contains("import com.chtl.compiler.chtljs.CHTLJSSystem.DelegationTarget")) {
                    continue;
                }
                fixedLines.add(line);
            }
            
            Files.write(path, fixedLines);
            filesFixed++;
            System.out.println("Fixed: CHTLJSSystem.java");
        } catch (IOException e) {
            errors.add("Error fixing CHTLJSSystem: " + e.getMessage());
        }
    }
    
    private static void fixJavaScriptCompilerComprehensively() {
        try {
            Path path = Paths.get("src/main/java/com/chtl/compiler/javascript/JavaScriptCompiler.java");
            if (!Files.exists(path)) return;
            
            List<String> lines = Files.readAllLines(path);
            List<String> fixedLines = new ArrayList<>();
            Set<String> imports = new LinkedHashSet<>();
            boolean packageDone = false;
            boolean importsDone = false;
            
            for (String line : lines) {
                if (line.trim().startsWith("package ")) {
                    fixedLines.add(line);
                    fixedLines.add("");
                    packageDone = true;
                    continue;
                }
                
                if (packageDone && !importsDone && line.trim().startsWith("import ")) {
                    String importLine = line.trim();
                    
                    // Skip duplicate ECMAScriptParser import
                    if (importLine.equals("import com.chtl.javascript.ECMAScriptParser;") && 
                        imports.contains("import com.chtl.javascript.ECMAScriptParser;")) {
                        continue;
                    }
                    
                    // Choose the correct CompilationResult import
                    if (importLine.contains("CompilationResult")) {
                        if (!imports.stream().anyMatch(i -> i.contains("CompilationResult"))) {
                            imports.add("import com.chtl.model.CompilationResult;");
                        }
                        continue;
                    }
                    
                    imports.add(importLine);
                    continue;
                }
                
                if (packageDone && !importsDone && !line.trim().isEmpty() && !line.trim().startsWith("import")) {
                    // End of imports, write them out
                    for (String imp : imports) {
                        fixedLines.add(imp);
                    }
                    fixedLines.add("");
                    importsDone = true;
                }
                
                if (importsDone || (!packageDone && !line.trim().startsWith("import"))) {
                    fixedLines.add(line);
                }
            }
            
            Files.write(path, fixedLines);
            filesFixed++;
            System.out.println("Fixed: JavaScriptCompiler.java");
        } catch (IOException e) {
            errors.add("Error fixing JavaScriptCompiler: " + e.getMessage());
        }
    }
    
    private static void removeSelfImports() throws IOException {
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String fileName = path.getFileName().toString();
                    String className = fileName.substring(0, fileName.length() - 5);
                    
                    List<String> lines = Files.readAllLines(path);
                    List<String> fixedLines = new ArrayList<>();
                    boolean modified = false;
                    String packageName = "";
                    
                    for (String line : lines) {
                        if (line.trim().startsWith("package ")) {
                            packageName = line.trim().substring(8, line.trim().length() - 1);
                        }
                        
                        // Skip self-imports
                        if (line.trim().startsWith("import ") && packageName.length() > 0) {
                            String importedClass = line.trim().substring(7, line.trim().length() - 1);
                            if (importedClass.equals(packageName + "." + className) ||
                                importedClass.startsWith(packageName + "." + className + ".")) {
                                modified = true;
                                continue;
                            }
                        }
                        
                        fixedLines.add(line);
                    }
                    
                    if (modified) {
                        Files.write(path, fixedLines);
                        filesFixed++;
                        System.out.println("Removed self-imports from: " + path.getFileName());
                    }
                } catch (IOException e) {
                    errors.add("Error processing " + path + ": " + e.getMessage());
                }
            });
    }
}