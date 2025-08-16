import java.io.*;
import java.nio.file.*;
import java.util.*;

public class FixFileNaming {
    public static void main(String[] args) throws IOException {
        System.out.println("Fixing file naming and import issues...");
        
        // Rename CompilationResult.java in compiler package to CompilerResult.java
        Path oldPath = Paths.get("src/main/java/com/chtl/compiler/CompilationResult.java");
        Path newPath = Paths.get("src/main/java/com/chtl/compiler/CompilerResult.java");
        
        if (Files.exists(oldPath)) {
            Files.move(oldPath, newPath, StandardCopyOption.REPLACE_EXISTING);
            System.out.println("Renamed CompilationResult.java to CompilerResult.java");
        }
        
        // Update imports in files that use CompilerResult
        updateCompilerResultImports();
        
        // Fix missing SourceLocation imports
        fixSourceLocationImports();
        
        System.out.println("File naming fixes complete!");
    }
    
    private static void updateCompilerResultImports() throws IOException {
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    boolean modified = false;
                    
                    // Update references to com.chtl.compiler.CompilationResult
                    if (content.contains("com.chtl.compiler.CompilationResult")) {
                        content = content.replace("com.chtl.compiler.CompilationResult", 
                                                "com.chtl.compiler.CompilerResult");
                        modified = true;
                    }
                    
                    if (modified) {
                        Files.writeString(path, content);
                        System.out.println("Updated imports in: " + path.getFileName());
                    }
                } catch (IOException e) {
                    System.err.println("Error updating " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static void fixSourceLocationImports() throws IOException {
        // Files that need SourceLocation import
        String[] filesNeedingSourceLocation = {
            "src/main/java/com/chtl/context/CompilationError.java",
            "src/main/java/com/chtl/context/CompilationWarning.java",
            "src/main/java/com/chtl/CHTLCompilerMain.java"
        };
        
        for (String filePath : filesNeedingSourceLocation) {
            Path path = Paths.get(filePath);
            if (!Files.exists(path)) continue;
            
            List<String> lines = Files.readAllLines(path);
            List<String> fixedLines = new ArrayList<>();
            boolean hasSourceLocationImport = false;
            boolean addedImport = false;
            
            for (String line : lines) {
                if (line.contains("import com.chtl.model.SourceLocation;")) {
                    hasSourceLocationImport = true;
                }
                
                // Add import after package declaration
                if (!hasSourceLocationImport && !addedImport && 
                    line.trim().startsWith("package ") && !line.trim().isEmpty()) {
                    fixedLines.add(line);
                    fixedLines.add("");
                    fixedLines.add("import com.chtl.model.SourceLocation;");
                    addedImport = true;
                    continue;
                }
                
                fixedLines.add(line);
            }
            
            if (addedImport) {
                Files.write(path, fixedLines);
                System.out.println("Added SourceLocation import to: " + path.getFileName());
            }
        }
    }
}