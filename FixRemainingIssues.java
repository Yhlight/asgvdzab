import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;

public class FixRemainingIssues {
    public static void main(String[] args) throws IOException {
        System.out.println("=== Fixing Remaining Compilation Issues ===\n");
        
        // Step 1: Fix imports in all files that need them
        fixAllMissingImports();
        
        // Step 2: Create any missing base classes
        createMissingBaseClasses();
        
        // Step 3: Fix specific file issues
        fixSpecificFiles();
        
        System.out.println("\n=== Fixes completed! ===");
    }
    
    static void fixAllMissingImports() throws IOException {
        System.out.println("Adding missing imports to all files...");
        
        // Map of files to their required imports
        Map<String, List<String>> fileImports = new HashMap<>();
        
        // AbstractCHTLASTNode needs imports
        fileImports.put("src/main/java/com/chtl/ast/AbstractCHTLASTNode.java", Arrays.asList(
            "com.chtl.ast.CHTLASTNode",
            "com.chtl.ast.NodeType",
            "com.chtl.model.SourceLocation",
            "java.util.List",
            "java.util.ArrayList",
            "java.io.Serializable"
        ));
        
        // Add imports to each file
        for (Map.Entry<String, List<String>> entry : fileImports.entrySet()) {
            addImportsToFile(entry.getKey(), entry.getValue());
        }
        
        // Fix all files in ast package
        Files.walk(Paths.get("src/main/java/com/chtl/ast"))
            .filter(path -> path.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    ensureBasicImports(path);
                } catch (IOException e) {
                    System.err.println("Error fixing " + path + ": " + e.getMessage());
                }
            });
    }
    
    static void addImportsToFile(String filePath, List<String> imports) throws IOException {
        Path path = Paths.get(filePath);
        if (!Files.exists(path)) return;
        
        String content = Files.readString(path);
        
        // Find package statement
        Pattern packagePattern = Pattern.compile("package\\s+([\\w.]+);");
        Matcher matcher = packagePattern.matcher(content);
        
        if (matcher.find()) {
            int insertPos = matcher.end();
            
            // Check which imports are missing and add them
            StringBuilder newImports = new StringBuilder();
            for (String imp : imports) {
                String importStatement = "import " + imp + ";";
                if (!content.contains(importStatement)) {
                    newImports.append("\nimport ").append(imp).append(";");
                }
            }
            
            if (newImports.length() > 0) {
                content = content.substring(0, insertPos) + newImports + content.substring(insertPos);
                Files.write(path, content.getBytes());
                System.out.println("  Fixed imports in: " + filePath);
            }
        }
    }
    
    static void ensureBasicImports(Path path) throws IOException {
        String content = Files.readString(path);
        String fileName = path.getFileName().toString();
        
        // Skip if it's an interface or enum
        if (content.contains("public interface") || content.contains("public enum")) {
            return;
        }
        
        // Common imports needed
        List<String> commonImports = new ArrayList<>();
        
        // Check what the file uses and add appropriate imports
        if (content.contains("List<") && !content.contains("import java.util.List")) {
            commonImports.add("java.util.List");
        }
        if (content.contains("ArrayList<") && !content.contains("import java.util.ArrayList")) {
            commonImports.add("java.util.ArrayList");
        }
        if (content.contains("Map<") && !content.contains("import java.util.Map")) {
            commonImports.add("java.util.Map");
        }
        if (content.contains("HashMap<") && !content.contains("import java.util.HashMap")) {
            commonImports.add("java.util.HashMap");
        }
        if (content.contains("Set<") && !content.contains("import java.util.Set")) {
            commonImports.add("java.util.Set");
        }
        if (content.contains("HashSet<") && !content.contains("import java.util.HashSet")) {
            commonImports.add("java.util.HashSet");
        }
        
        // Add model imports if referenced
        if (content.contains("SourceLocation") && !content.contains("import com.chtl.model.SourceLocation")) {
            commonImports.add("com.chtl.model.SourceLocation");
        }
        if (content.contains("NodeType") && !content.contains("import com.chtl.ast.NodeType") && !fileName.equals("NodeType.java")) {
            commonImports.add("com.chtl.ast.NodeType");
        }
        if (content.contains("CHTLASTNode") && !content.contains("import com.chtl.ast.CHTLASTNode") && !fileName.equals("CHTLASTNode.java")) {
            commonImports.add("com.chtl.ast.CHTLASTNode");
        }
        if (content.contains("CHTLASTVisitor") && !content.contains("import com.chtl.ast.CHTLASTVisitor") && !fileName.equals("CHTLASTVisitor.java")) {
            commonImports.add("com.chtl.ast.CHTLASTVisitor");
        }
        if (content.contains("AbstractCHTLASTNode") && !content.contains("import com.chtl.ast.AbstractCHTLASTNode") && !fileName.equals("AbstractCHTLASTNode.java")) {
            commonImports.add("com.chtl.ast.AbstractCHTLASTNode");
        }
        
        if (!commonImports.isEmpty()) {
            addImportsToFile(path.toString(), commonImports);
        }
    }
    
    static void createMissingBaseClasses() throws IOException {
        System.out.println("Creating any missing base classes...");
        
        // Ensure CHTLASTNode interface exists with all methods
        createClass("com.chtl.ast", "CHTLASTNode", """
import com.chtl.model.SourceLocation;
import java.util.List;

/**
 * Base interface for all CHTL AST nodes
 */
public interface CHTLASTNode {
    /**
     * Get the type of this node
     */
    NodeType getNodeType();
    
    /**
     * Get child nodes
     */
    List<CHTLASTNode> getChildren();
    
    /**
     * Add a child node
     */
    void addChild(CHTLASTNode child);
    
    /**
     * Get parent node
     */
    CHTLASTNode getParent();
    
    /**
     * Set parent node
     */
    void setParent(CHTLASTNode parent);
    
    /**
     * Get source location
     */
    SourceLocation getLocation();
    
    /**
     * Set source location
     */
    void setLocation(SourceLocation location);
    
    /**
     * Accept a visitor
     */
    void accept(CHTLASTVisitor visitor);
}
""");
    }
    
    static void fixSpecificFiles() throws IOException {
        System.out.println("Fixing specific file issues...");
        
        // Fix token-related classes
        fixTokenClasses();
        
        // Fix CHTL JS related imports
        fixCHTLJSImports();
        
        // Fix parser imports
        fixParserImports();
    }
    
    static void fixTokenClasses() throws IOException {
        // Create Token class if referenced
        Path tokenPath = Paths.get("src/main/java/com/chtl/compiler/chtl/token/CHTLToken.java");
        if (!Files.exists(tokenPath)) {
            createClass("com.chtl.compiler.chtl.token", "CHTLToken", """
public class CHTLToken {
    private String type;
    private String value;
    private int line;
    private int column;
    
    public CHTLToken(String type, String value, int line, int column) {
        this.type = type;
        this.value = value;
        this.line = line;
        this.column = column;
    }
    
    public String getType() { return type; }
    public String getValue() { return value; }
    public int getLine() { return line; }
    public int getColumn() { return column; }
}
""");
        }
    }
    
    static void fixCHTLJSImports() throws IOException {
        // Fix imports in CHTL JS related files
        Files.walk(Paths.get("src/main/java/com/chtl/ast/chtljs"))
            .filter(path -> path.toString().endsWith(".java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    
                    // Add base imports if missing
                    if (!content.contains("import com.chtl.ast.CHTLASTNode") && content.contains("CHTLASTNode")) {
                        content = addImportAfterPackage(content, "com.chtl.ast.CHTLASTNode");
                    }
                    if (!content.contains("import com.chtl.ast.NodeType") && content.contains("NodeType")) {
                        content = addImportAfterPackage(content, "com.chtl.ast.NodeType");
                    }
                    
                    Files.write(path, content.getBytes());
                } catch (IOException e) {
                    System.err.println("Error fixing " + path + ": " + e.getMessage());
                }
            });
    }
    
    static void fixParserImports() throws IOException {
        // Fix parser-related imports
        Path parserPath = Paths.get("src/main/java/com/chtl/parser/CHTLParser.java");
        if (Files.exists(parserPath)) {
            List<String> parserImports = Arrays.asList(
                "com.chtl.ast.CHTLASTNode",
                "com.chtl.ast.node.*",
                "com.chtl.context.CompilationContext",
                "com.chtl.exception.CompilationException",
                "com.chtl.model.SourceLocation",
                "java.util.List",
                "java.util.ArrayList",
                "java.util.Stack"
            );
            addImportsToFile(parserPath.toString(), parserImports);
        }
        
        // Fix CHTL JS parser
        Path jsParserPath = Paths.get("src/main/java/com/chtl/parser/CHTLJSParser.java");
        if (Files.exists(jsParserPath)) {
            List<String> jsParserImports = Arrays.asList(
                "com.chtl.ast.CHTLASTNode",
                "com.chtl.ast.chtljs.*",
                "com.chtl.context.CHTLJSContext",
                "com.chtl.exception.CompilationException",
                "java.util.List",
                "java.util.ArrayList"
            );
            addImportsToFile(jsParserPath.toString(), jsParserImports);
        }
    }
    
    static String addImportAfterPackage(String content, String importClass) {
        Pattern packagePattern = Pattern.compile("(package\\s+[\\w.]+;)");
        Matcher matcher = packagePattern.matcher(content);
        if (matcher.find()) {
            String packageLine = matcher.group(1);
            return content.replace(packageLine, packageLine + "\nimport " + importClass + ";");
        }
        return content;
    }
    
    static void createClass(String packageName, String className, String classBody) throws IOException {
        String path = "src/main/java/" + packageName.replace('.', '/');
        Files.createDirectories(Paths.get(path));
        String content = "package " + packageName + ";\n\n" + classBody;
        Path filePath = Paths.get(path + "/" + className + ".java");
        Files.write(filePath, content.getBytes());
        System.out.println("  Created/Updated: " + packageName + "." + className);
    }
}