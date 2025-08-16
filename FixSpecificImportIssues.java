import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.Pattern;

public class FixSpecificImportIssues {
    public static void main(String[] args) throws IOException {
        System.out.println("Fixing specific import issues...");
        
        // Fix CHTLCompilerMain.java
        fixCHTLCompilerMain();
        
        // Fix other specific files with known issues
        fixKnownImportIssues();
        
        // Fix class visibility issues
        fixClassVisibilityIssues();
        
        System.out.println("Done!");
    }
    
    private static void fixCHTLCompilerMain() throws IOException {
        Path path = Paths.get("src/main/java/com/chtl/CHTLCompilerMain.java");
        if (!Files.exists(path)) return;
        
        String content = Files.readString(path);
        
        // Check if CmodPackager and CmodUnpacker exist in the expected location
        Path packagerPath = Paths.get("src/main/java/com/chtl/compiler/cmod/CmodPackager.java");
        Path unpackerPath = Paths.get("src/main/java/com/chtl/compiler/cmod/CmodUnpacker.java");
        
        if (Files.exists(packagerPath) && Files.exists(unpackerPath)) {
            // The imports are correct, the classes must not be public
            System.out.println("CmodPackager and CmodUnpacker exist, checking visibility...");
            
            // Make sure they are public
            makeClassPublic(packagerPath, "CmodPackager");
            makeClassPublic(unpackerPath, "CmodUnpacker");
        } else {
            System.out.println("CmodPackager or CmodUnpacker not found in expected location");
            
            // Try to find them
            Optional<Path> foundPackager = findClass("CmodPackager");
            Optional<Path> foundUnpacker = findClass("CmodUnpacker");
            
            if (foundPackager.isPresent() && foundUnpacker.isPresent()) {
                // Update imports
                String packagerPackage = extractPackageFromFile(foundPackager.get());
                String unpackerPackage = extractPackageFromFile(foundUnpacker.get());
                
                content = content.replace("import com.chtl.compiler.cmod.CmodPackager;", 
                                        "import " + packagerPackage + ".CmodPackager;");
                content = content.replace("import com.chtl.compiler.cmod.CmodUnpacker;", 
                                        "import " + unpackerPackage + ".CmodUnpacker;");
                
                Files.writeString(path, content);
                System.out.println("Updated imports in CHTLCompilerMain.java");
            }
        }
    }
    
    private static void fixKnownImportIssues() throws IOException {
        Map<String, Map<String, String>> fileImportFixes = new HashMap<>();
        
        // Add specific import fixes for known files
        Map<String, String> cliImports = new HashMap<>();
        cliImports.put("CHTLCompiler", "com.chtl.compiler.CHTLCompiler");
        cliImports.put("CompilationResult", "com.chtl.model.CompilationResult");
        cliImports.put("CHTLException", "com.chtl.exception.CHTLException");
        fileImportFixes.put("src/main/java/com/chtl/cli/CHTLCLI.java", cliImports);
        
        // Apply fixes
        for (Map.Entry<String, Map<String, String>> entry : fileImportFixes.entrySet()) {
            Path path = Paths.get(entry.getKey());
            if (!Files.exists(path)) continue;
            
            String content = Files.readString(path);
            boolean modified = false;
            
            for (Map.Entry<String, String> importFix : entry.getValue().entrySet()) {
                String className = importFix.getKey();
                String fullImport = importFix.getValue();
                
                if (content.contains(className) && !content.contains("import " + fullImport)) {
                    // Add import after package
                    int packageEnd = content.indexOf(';');
                    if (packageEnd > 0) {
                        content = content.substring(0, packageEnd + 1) + 
                                "\nimport " + fullImport + ";" + 
                                content.substring(packageEnd + 1);
                        modified = true;
                    }
                }
            }
            
            if (modified) {
                Files.writeString(path, content);
                System.out.println("Fixed imports in: " + path.getFileName());
            }
        }
    }
    
    private static void fixClassVisibilityIssues() throws IOException {
        // List of classes that should be public
        List<String> classesToMakePublic = Arrays.asList(
            "CmodPackager",
            "CmodUnpacker",
            "SimpleZip",
            "CmodFormat",
            "CHTLCompiler",
            "CompilerResult"
        );
        
        for (String className : classesToMakePublic) {
            Optional<Path> classPath = findClass(className);
            if (classPath.isPresent()) {
                makeClassPublic(classPath.get(), className);
            }
        }
    }
    
    private static void makeClassPublic(Path path, String className) throws IOException {
        String content = Files.readString(path);
        
        // Check if class is not public
        Pattern classPattern = Pattern.compile("(?<!public\\s)class\\s+" + className + "\\b");
        if (classPattern.matcher(content).find()) {
            content = content.replaceAll("class\\s+" + className + "\\b", "public class " + className);
            Files.writeString(path, content);
            System.out.println("Made " + className + " public in: " + path.getFileName());
        }
        
        // Also check for interface
        Pattern interfacePattern = Pattern.compile("(?<!public\\s)interface\\s+" + className + "\\b");
        if (interfacePattern.matcher(content).find()) {
            content = content.replaceAll("interface\\s+" + className + "\\b", "public interface " + className);
            Files.writeString(path, content);
            System.out.println("Made " + className + " interface public in: " + path.getFileName());
        }
    }
    
    private static Optional<Path> findClass(String className) throws IOException {
        return Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.getFileName().toString().equals(className + ".java"))
            .findFirst();
    }
    
    private static String extractPackageFromFile(Path path) throws IOException {
        List<String> lines = Files.readAllLines(path);
        for (String line : lines) {
            if (line.trim().startsWith("package ")) {
                return line.trim().substring(8, line.trim().length() - 1);
            }
        }
        return "com.chtl";
    }
}