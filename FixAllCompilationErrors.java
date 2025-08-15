import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;

public class FixAllCompilationErrors {
    public static void main(String[] args) throws IOException {
        System.out.println("Starting comprehensive compilation error fix...\n");
        
        // Step 1: Add missing imports
        fixMissingImports();
        
        // Step 2: Create missing classes
        createMissingClasses();
        
        // Step 3: Fix duplicate method definitions
        fixDuplicateMethods();
        
        // Step 4: Fix CJmod related issues
        fixCJmodIssues();
        
        // Step 5: Create missing performance classes
        createPerformanceClasses();
        
        System.out.println("\nAll fixes completed!");
    }
    
    static void fixMissingImports() throws IOException {
        System.out.println("Fixing missing imports...");
        
        // Fix CJmodErrorHandler
        addImports("src/main/java/com/chtl/cjmod/CJmodErrorHandler.java", Arrays.asList(
            "java.util.concurrent.atomic.AtomicInteger",
            "java.util.concurrent.atomic.AtomicLong"
        ));
        
        // Fix CompilerService
        addImports("src/main/java/com/chtl/service/CompilerService.java", Arrays.asList(
            "java.util.function.Consumer"
        ));
        
        // Fix ModuleSandbox
        addImports("src/main/java/com/chtl/cjmod/ModuleSandbox.java", Arrays.asList(
            "java.security.AccessController",
            "java.security.PrivilegedAction"
        ));
        
        // Fix StreamingFileProcessor
        addImports("src/main/java/com/chtl/core/memory/StreamingFileProcessor.java", Arrays.asList(
            "java.nio.channels.FileChannel",
            "java.nio.MappedByteBuffer"
        ));
        
        // Fix ParallelCompilationEngine
        addImports("src/main/java/com/chtl/core/parallel/ParallelCompilationEngine.java", Arrays.asList(
            "java.util.concurrent.ForkJoinPool",
            "java.util.concurrent.RecursiveTask"
        ));
    }
    
    static void addImports(String filePath, List<String> imports) throws IOException {
        Path path = Paths.get(filePath);
        if (!Files.exists(path)) return;
        
        String content = Files.readString(path);
        
        // Find the package declaration
        Pattern packagePattern = Pattern.compile("package\\s+[\\w.]+;");
        Matcher matcher = packagePattern.matcher(content);
        
        if (matcher.find()) {
            int insertPos = matcher.end();
            StringBuilder sb = new StringBuilder(content);
            
            // Check which imports are missing
            for (String imp : imports) {
                String importStatement = "import " + imp + ";";
                if (!content.contains(importStatement)) {
                    sb.insert(insertPos, "\nimport " + imp + ";");
                    insertPos += importStatement.length() + 1;
                }
            }
            
            Files.write(path, sb.toString().getBytes());
            System.out.println("  Fixed imports in: " + filePath);
        }
    }
    
    static void createMissingClasses() throws IOException {
        System.out.println("Creating missing classes...");
        
        // Create BlockScanResult
        createClass("com.chtl.scanner", "BlockScanResult", """
import java.util.*;

public class BlockScanResult {
    private final List<CodeFragment> fragments;
    private final Map<String, Object> metadata;
    
    public BlockScanResult(List<CodeFragment> fragments) {
        this.fragments = fragments;
        this.metadata = new HashMap<>();
    }
    
    public List<CodeFragment> getFragments() {
        return fragments;
    }
    
    public Map<String, Object> getMetadata() {
        return metadata;
    }
}
""");

        // Create CHTLJSObject
        createClass("com.chtl.ast.chtljs", "CHTLJSObject", """
import com.chtl.chtljs.ast.*;
import java.util.*;

public class CHTLJSObject extends AbstractCHTLJSASTNode {
    private Map<String, CHTLJSASTNode> properties = new HashMap<>();
    
    public CHTLJSObject() {
        super(CHTLJSASTNode.NodeType.OBJECT);
    }
    
    public void addProperty(String key, CHTLJSASTNode value) {
        properties.put(key, value);
    }
    
    public Map<String, CHTLJSASTNode> getProperties() {
        return properties;
    }
}
""");

        // Create CHTLJSArray
        createClass("com.chtl.ast.chtljs", "CHTLJSArray", """
import com.chtl.chtljs.ast.*;
import java.util.*;

public class CHTLJSArray extends AbstractCHTLJSASTNode {
    private List<CHTLJSASTNode> elements = new ArrayList<>();
    
    public CHTLJSArray() {
        super(CHTLJSASTNode.NodeType.ARRAY);
    }
    
    public void addElement(CHTLJSASTNode element) {
        elements.add(element);
    }
    
    public List<CHTLJSASTNode> getElements() {
        return elements;
    }
}
""");

        // Create CHTLJSString
        createClass("com.chtl.ast.chtljs", "CHTLJSString", """
import com.chtl.chtljs.ast.*;

public class CHTLJSString extends AbstractCHTLJSASTNode {
    private String value;
    
    public CHTLJSString(String value) {
        super(CHTLJSASTNode.NodeType.STRING);
        this.value = value;
    }
    
    public String getValue() {
        return value;
    }
}
""");

        // Create DynamicScannerState
        createClass("com.chtl.scanner", "DynamicScannerState", """
public class DynamicScannerState {
    private String currentContext;
    private int depth;
    
    public DynamicScannerState(String context) {
        this.currentContext = context;
        this.depth = 0;
    }
    
    public String getContext() { return currentContext; }
    public void setContext(String context) { this.currentContext = context; }
    public int getDepth() { return depth; }
    public void incrementDepth() { depth++; }
    public void decrementDepth() { depth--; }
}
""");

        // Create CompilerError
        createClass("com.chtl.exception", "CompilerError", """
public class CompilerError extends Exception {
    private final int line;
    private final int column;
    private final String filename;
    
    public CompilerError(String message, String filename, int line, int column) {
        super(message);
        this.filename = filename;
        this.line = line;
        this.column = column;
    }
    
    public int getLine() { return line; }
    public int getColumn() { return column; }
    public String getFilename() { return filename; }
}
""");
    }
    
    static void fixDuplicateMethods() throws IOException {
        System.out.println("Fixing duplicate method definitions...");
        
        Path parserPath = Paths.get("src/main/java/com/chtl/parser/CHTLParser.java");
        if (Files.exists(parserPath)) {
            String content = Files.readString(parserPath);
            
            // Remove the second occurrence of parseDeleteOperation
            content = removeDuplicateMethod(content, "parseDeleteOperation", 1251);
            
            // Remove the second occurrence of parseInsertOperation  
            content = removeDuplicateMethod(content, "parseInsertOperation", 1275);
            
            Files.write(parserPath, content.getBytes());
            System.out.println("  Fixed duplicate methods in CHTLParser.java");
        }
    }
    
    static String removeDuplicateMethod(String content, String methodName, int lineNumber) {
        String[] lines = content.split("\n");
        StringBuilder sb = new StringBuilder();
        boolean foundFirst = false;
        boolean inDuplicateMethod = false;
        int braceCount = 0;
        
        for (int i = 0; i < lines.length; i++) {
            String line = lines[i];
            
            if (line.contains("private CHTLASTNode " + methodName + "()")) {
                if (!foundFirst) {
                    foundFirst = true;
                    sb.append(line).append("\n");
                } else {
                    // Skip this duplicate method
                    inDuplicateMethod = true;
                    braceCount = 0;
                    continue;
                }
            } else if (inDuplicateMethod) {
                if (line.contains("{")) braceCount++;
                if (line.contains("}")) braceCount--;
                if (braceCount == 0 && line.trim().equals("}")) {
                    inDuplicateMethod = false;
                }
                // Skip lines of duplicate method
            } else {
                sb.append(line).append("\n");
            }
        }
        
        return sb.toString();
    }
    
    static void fixCJmodIssues() throws IOException {
        System.out.println("Fixing CJmod issues...");
        
        // Fix ChthollyModule location and imports
        Path chthollyPath = Paths.get("src/main/java/com/chtl/module/Chtholly/CJMOD/src/ChthollyModule.java");
        if (Files.exists(chthollyPath)) {
            // Move to correct location
            Path newPath = Paths.get("src/main/java/com/chtl/cjmod/modules/ChthollyModule.java");
            Files.createDirectories(newPath.getParent());
            
            String content = Files.readString(chthollyPath);
            content = content.replace("package com.chtl.module.Chtholly.CJMOD.src;", 
                                    "package com.chtl.cjmod.modules;");
            content = "import com.chtl.cjmod.*;\n" + content;
            
            Files.write(newPath, content.getBytes());
            Files.delete(chthollyPath);
            System.out.println("  Moved and fixed ChthollyModule.java");
        }
    }
    
    static void createPerformanceClasses() throws IOException {
        System.out.println("Creating performance-related classes...");
        
        // Create MemoryMappedFile
        createClass("com.chtl.core.memory", "MemoryMappedFile", """
import java.io.*;
import java.nio.*;
import java.nio.channels.*;

public class MemoryMappedFile implements AutoCloseable {
    private final RandomAccessFile file;
    private final FileChannel channel;
    private final MappedByteBuffer buffer;
    
    public MemoryMappedFile(File file, long size) throws IOException {
        this.file = new RandomAccessFile(file, "rw");
        this.channel = this.file.getChannel();
        this.buffer = channel.map(FileChannel.MapMode.READ_WRITE, 0, size);
    }
    
    public MappedByteBuffer getBuffer() {
        return buffer;
    }
    
    @Override
    public void close() throws IOException {
        channel.close();
        file.close();
    }
}
""");

        // Create CompilationTask
        createClass("com.chtl.core.parallel", "CompilationTask", """
import java.util.concurrent.RecursiveTask;

public class CompilationTask extends RecursiveTask<CompilationResult> {
    private final String source;
    
    public CompilationTask(String source) {
        this.source = source;
    }
    
    @Override
    protected CompilationResult compute() {
        // Stub implementation
        return new CompilationResult(true, "");
    }
}
""");

        // Create CompilationResult
        createClass("com.chtl.core.parallel", "CompilationResult", """
public class CompilationResult {
    private final boolean success;
    private final String output;
    
    public CompilationResult(boolean success, String output) {
        this.success = success;
        this.output = output;
    }
    
    public boolean isSuccess() { return success; }
    public String getOutput() { return output; }
}
""");
    }
    
    static void createClass(String packageName, String className, String classBody) throws IOException {
        String path = "src/main/java/" + packageName.replace('.', '/');
        Files.createDirectories(Paths.get(path));
        String content = "package " + packageName + ";\n\n" + classBody;
        Path filePath = Paths.get(path + "/" + className + ".java");
        Files.write(filePath, content.getBytes());
        System.out.println("  Created: " + packageName + "." + className);
    }
}