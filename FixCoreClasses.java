import java.io.*;
import java.nio.file.*;

public class FixCoreClasses {
    public static void main(String[] args) throws IOException {
        System.out.println("Creating core classes for CHTL compiler...\n");
        
        // Create SourceLocation
        createClass("com.chtl.model", "SourceLocation", """
public class SourceLocation {
    private final String filename;
    private final int line;
    private final int column;
    private final int offset;
    
    public SourceLocation(String filename, int line, int column, int offset) {
        this.filename = filename;
        this.line = line;
        this.column = column;
        this.offset = offset;
    }
    
    public String getFilename() { return filename; }
    public int getLine() { return line; }
    public int getColumn() { return column; }
    public int getOffset() { return offset; }
    
    @Override
    public String toString() {
        return filename + ":" + line + ":" + column;
    }
}
""");

        // Create FragmentType enum
        createClass("com.chtl.model", "FragmentType", """
public enum FragmentType {
    CHTL("CHTL"),
    CSS("CSS"),
    JAVASCRIPT("JavaScript"),
    CHTL_JS("CHTL_JS"),
    HTML("HTML"),
    COMMENT("Comment"),
    ORIGIN("Origin"),
    UNKNOWN("Unknown");
    
    private final String displayName;
    
    FragmentType(String displayName) {
        this.displayName = displayName;
    }
    
    public String getDisplayName() {
        return displayName;
    }
}
""");

        // Create SelectorType enum
        createClass("com.chtl.compiler.chtljs.selector", "SelectorType", """
public enum SelectorType {
    ELEMENT,
    CLASS,
    ID,
    ATTRIBUTE,
    PSEUDO_CLASS,
    PSEUDO_ELEMENT,
    UNIVERSAL,
    DESCENDANT,
    CHILD,
    ADJACENT_SIBLING,
    GENERAL_SIBLING,
    COMPOUND,
    COMPLEX
}
""");

        // Create State enum for scanner
        createClass("com.chtl.scanner", "State", """
public enum State {
    INITIAL,
    CHTL,
    CHTL_CONTENT,
    STYLE,
    SCRIPT,
    LOCAL_STYLE,
    LOCAL_SCRIPT,
    JAVASCRIPT,
    CSS,
    CHTL_JS,
    COMMENT,
    STRING,
    TEMPLATE
}
""");

        // Create CodeFragment
        createClass("com.chtl.scanner", "CodeFragment", """
import com.chtl.model.FragmentType;

public class CodeFragment {
    private final String content;
    private final FragmentType type;
    private final int startPosition;
    private final int endPosition;
    private final int line;
    private final int column;
    
    public CodeFragment(String content, FragmentType type, int startPosition, int endPosition, int line, int column) {
        this.content = content;
        this.type = type;
        this.startPosition = startPosition;
        this.endPosition = endPosition;
        this.line = line;
        this.column = column;
    }
    
    public String getContent() { return content; }
    public FragmentType getType() { return type; }
    public int getStartPosition() { return startPosition; }
    public int getEndPosition() { return endPosition; }
    public int getLine() { return line; }
    public int getColumn() { return column; }
    public int getLength() { return endPosition - startPosition; }
}
""");

        // Create CompilationException
        createClass("com.chtl.exception", "CompilationException", """
import com.chtl.model.SourceLocation;

public class CompilationException extends Exception {
    private final SourceLocation location;
    
    public CompilationException(String message) {
        super(message);
        this.location = null;
    }
    
    public CompilationException(String message, SourceLocation location) {
        super(message);
        this.location = location;
    }
    
    public CompilationException(String message, Throwable cause) {
        super(message, cause);
        this.location = null;
    }
    
    public CompilationException(String message, SourceLocation location, Throwable cause) {
        super(message, cause);
        this.location = location;
    }
    
    public SourceLocation getLocation() {
        return location;
    }
}
""");

        // Create CompilationError
        createClass("com.chtl.context", "CompilationError", """
import com.chtl.model.SourceLocation;

public class CompilationError {
    private final String message;
    private final SourceLocation location;
    private final ErrorType type;
    
    public enum ErrorType {
        SYNTAX_ERROR,
        SEMANTIC_ERROR,
        TYPE_ERROR,
        REFERENCE_ERROR,
        CONSTRAINT_ERROR,
        IMPORT_ERROR,
        NAMESPACE_ERROR
    }
    
    public CompilationError(String message, SourceLocation location, ErrorType type) {
        this.message = message;
        this.location = location;
        this.type = type;
    }
    
    public String getMessage() { return message; }
    public SourceLocation getLocation() { return location; }
    public ErrorType getType() { return type; }
}
""");

        // Create CompilationWarning
        createClass("com.chtl.context", "CompilationWarning", """
import com.chtl.model.SourceLocation;

public class CompilationWarning {
    private final String message;
    private final SourceLocation location;
    private final WarningType type;
    
    public enum WarningType {
        DEPRECATED,
        UNUSED,
        PERFORMANCE,
        COMPATIBILITY,
        STYLE
    }
    
    public CompilationWarning(String message, SourceLocation location, WarningType type) {
        this.message = message;
        this.location = location;
        this.type = type;
    }
    
    public String getMessage() { return message; }
    public SourceLocation getLocation() { return location; }
    public WarningType getType() { return type; }
}
""");

        // Create ErrorListener interface
        createClass("com.chtl.core.error", "ErrorListener", """
public interface ErrorListener {
    void onError(String message, String filename, int line, int column);
    void onWarning(String message, String filename, int line, int column);
    void onInfo(String message);
}
""");

        // Create basic CHTLNode interface
        createClass("com.chtl.model", "CHTLNode", """
public interface CHTLNode {
    // Base interface for CHTL model nodes
}
""");

        // Create basic model classes
        createClass("com.chtl.model", "TemplateNode", """
public class TemplateNode implements CHTLNode {
    private String name;
    
    public TemplateNode(String name) {
        this.name = name;
    }
    
    public String getName() { return name; }
}
""");

        createClass("com.chtl.model", "ScriptNode", """
public class ScriptNode implements CHTLNode {
    private String content;
    
    public ScriptNode(String content) {
        this.content = content;
    }
    
    public String getContent() { return content; }
}
""");

        createClass("com.chtl.model", "StyleNode", """
public class StyleNode implements CHTLNode {
    private String content;
    
    public StyleNode(String content) {
        this.content = content;
    }
    
    public String getContent() { return content; }
    
    public static class StyleRule {
        private String selector;
        private String properties;
        
        public StyleRule(String selector, String properties) {
            this.selector = selector;
            this.properties = properties;
        }
        
        public String getSelector() { return selector; }
        public String getProperties() { return properties; }
    }
    
    public static class StyleProperty {
        private String name;
        private String value;
        
        public StyleProperty(String name, String value) {
            this.name = name;
            this.value = value;
        }
        
        public String getName() { return name; }
        public String getValue() { return value; }
    }
}
""");

        createClass("com.chtl.model", "CustomNode", """
public class CustomNode implements CHTLNode {
    private String type;
    private String content;
    
    public CustomNode(String type, String content) {
        this.type = type;
        this.content = content;
    }
    
    public String getType() { return type; }
    public String getContent() { return content; }
}
""");

        // Create lexer state enum
        createClass("com.chtl.compiler.chtljs.lexer", "LexState", """
public enum LexState {
    INITIAL,
    CHTL_JS,
    STRING,
    NUMBER,
    IDENTIFIER,
    OPERATOR,
    COMMENT,
    SELECTOR,
    FUNCTION_CALL,
    OBJECT,
    ARRAY
}
""");

        System.out.println("\nAll core classes created successfully!");
    }
    
    static void createClass(String packageName, String className, String classBody) throws IOException {
        String path = "src/main/java/" + packageName.replace('.', '/');
        Files.createDirectories(Paths.get(path));
        String content = "package " + packageName + ";\n\n" + classBody;
        Path filePath = Paths.get(path + "/" + className + ".java");
        
        // Only create if doesn't exist
        if (!Files.exists(filePath)) {
            Files.write(filePath, content.getBytes());
            System.out.println("Created: " + packageName + "." + className);
        }
    }
}