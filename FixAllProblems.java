import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;

public class FixAllProblems {
    public static void main(String[] args) throws IOException {
        System.out.println("=== Fixing All CHTL Compiler Problems ===\n");
        
        // Step 1: Fix package/import issues
        fixPackageImportIssues();
        
        // Step 2: Create missing visitor
        createCHTLNodeVisitor();
        
        // Step 3: Fix CHTLNode hierarchy
        fixCHTLNodeHierarchy();
        
        // Step 4: Fix AST file imports
        fixASTImports();
        
        // Step 5: Create all missing node classes
        createAllMissingNodes();
        
        System.out.println("\n=== All fixes completed! ===");
    }
    
    static void fixPackageImportIssues() throws IOException {
        System.out.println("Fixing package/import issues...");
        
        // Fix files with malformed package statements
        fixFile("src/main/java/com/chtl/ast/CHTLASTNode.java", 
                "package com.chtl.ast;import com.chtl.chtljs.ast.NodeType;",
                "package com.chtl.ast;");
                
        fixFile("src/main/java/com/chtl/ast/CHTLASTVisitor.java",
                "package com.chtl.ast;import com.chtl.ast.node.CommentNode;",
                "package com.chtl.ast;");
                
        fixFile("src/main/java/com/chtl/ast/NodeType.java",
                "package com.chtl.ast;import com.chtl.chtljs.ast.NodeType;",
                "package com.chtl.ast;");
    }
    
    static void fixFile(String path, String find, String replace) throws IOException {
        Path filePath = Paths.get(path);
        if (Files.exists(filePath)) {
            String content = Files.readString(filePath);
            content = content.replace(find, replace);
            Files.write(filePath, content.getBytes());
            System.out.println("  Fixed: " + path);
        }
    }
    
    static void createCHTLNodeVisitor() throws IOException {
        System.out.println("Creating CHTLNodeVisitor...");
        
        createClass("com.chtl.model", "CHTLNodeVisitor", """
public interface CHTLNodeVisitor {
    void visitTemplate(TemplateNode node);
    void visitScript(ScriptNode node);
    void visitStyle(StyleNode node);
    void visitCustom(CustomNode node);
}
""");
    }
    
    static void fixCHTLNodeHierarchy() throws IOException {
        System.out.println("Fixing CHTLNode hierarchy...");
        
        // Replace CHTLNode interface to remove abstract method
        createClass("com.chtl.model", "CHTLNode", """
public interface CHTLNode {
    // Base interface for CHTL model nodes
    // Implementing classes should provide accept method if needed
}
""");
        
        // Update existing model classes to not have accept method
        String[] modelClasses = {"TemplateNode", "ScriptNode", "StyleNode", "CustomNode"};
        for (String className : modelClasses) {
            Path path = Paths.get("src/main/java/com/chtl/model/" + className + ".java");
            if (Files.exists(path)) {
                String content = Files.readString(path);
                // Remove accept method and its overrides
                content = content.replaceAll("@Override\\s*public\\s+void\\s+accept\\s*\\([^)]+\\)\\s*\\{[^}]*\\}", "");
                content = content.replaceAll("public\\s+void\\s+accept\\s*\\([^)]+\\)\\s*\\{[^}]*\\}", "");
                Files.write(path, content.getBytes());
            }
        }
    }
    
    static void fixASTImports() throws IOException {
        System.out.println("Fixing AST imports...");
        
        // Fix CHTLASTNode.java
        Path astNodePath = Paths.get("src/main/java/com/chtl/ast/CHTLASTNode.java");
        if (Files.exists(astNodePath)) {
            String content = Files.readString(astNodePath);
            if (!content.contains("import com.chtl.model.SourceLocation;")) {
                content = content.replace("package com.chtl.ast;", 
                    "package com.chtl.ast;\n\nimport com.chtl.model.SourceLocation;");
                Files.write(astNodePath, content.getBytes());
            }
        }
        
        // Fix NodeType.java - make it a simple enum
        createClass("com.chtl.ast", "NodeType", """
public enum NodeType {
    // Element types
    ELEMENT,
    TEXT,
    ATTRIBUTE,
    COMMENT,
    
    // Template types
    TEMPLATE_DEFINITION,
    TEMPLATE_USAGE,
    
    // Style types
    STYLE_BLOCK,
    STYLE_RULE,
    STYLE_PROPERTY,
    STYLE_SELECTOR,
    
    // Script types
    SCRIPT_BLOCK,
    
    // Custom types
    CUSTOM_DEFINITION,
    CUSTOM_USAGE,
    CUSTOM_OPERATION,
    SPECIALIZATION,
    
    // Origin types
    ORIGIN_BLOCK,
    
    // Import/Namespace types
    IMPORT_STATEMENT,
    NAMESPACE,
    
    // Configuration types
    CONFIGURATION,
    CONSTRAINT,
    
    // Variable types
    VAR_GROUP_DEFINITION,
    VAR_GROUP_USAGE,
    
    // Literal types
    STRING_LITERAL,
    NUMBER_LITERAL,
    UNQUOTED_LITERAL,
    
    // Other types
    ROOT,
    INHERIT_STATEMENT,
    
    // CHTL JS types
    CHTLJS_SELECTOR,
    CHTLJS_METHOD_CALL,
    CHTLJS_ANIMATE,
    CHTLJS_OBJECT,
    CHTLJS_ARRAY,
    CHTLJS_FUNCTION,
    CHTLJS_IDENTIFIER,
    ANIMATION_CONTROLLER,
    CHTLJS_EXPRESSION
}
""");
    }
    
    static void createAllMissingNodes() throws IOException {
        System.out.println("Creating all missing node classes...");
        
        // Base nodes
        createNode("RootNode", "ROOT");
        createNode("CommentNode", "COMMENT", "String content");
        
        // Element nodes (extend from model)
        createElementNode("ElementNode");
        createElementNode("AttributeNode");
        createElementNode("TextNode");
        
        // Style nodes
        createNode("StyleBlockNode", "STYLE_BLOCK", "boolean isGlobal");
        createNode("StyleRuleNode", "STYLE_RULE");
        createNode("StylePropertyNode", "STYLE_PROPERTY", "String name", "String value");
        createNode("StyleSelectorNode", "STYLE_SELECTOR", "String selector");
        
        // Script nodes
        createNode("ScriptBlockNode", "SCRIPT_BLOCK", "String content", "boolean isLocal");
        
        // Template nodes
        createNode("TemplateDefinitionNode", "TEMPLATE_DEFINITION", "String type", "String name");
        createNode("TemplateUsageNode", "TEMPLATE_USAGE", "String templateName");
        
        // Custom nodes
        createNode("CustomDefinitionNode", "CUSTOM_DEFINITION", "String name");
        createNode("CustomUsageNode", "CUSTOM_USAGE", "String customName");
        createNode("CustomOperationNode", "CUSTOM_OPERATION", "String operation");
        createNode("SpecializationNode", "SPECIALIZATION");
        
        // Origin nodes
        createNode("OriginBlockNode", "ORIGIN_BLOCK", "String targetType", "String content");
        
        // Import/Namespace nodes
        createNode("ImportStatementNode", "IMPORT_STATEMENT", "String importType", "String source");
        createNode("NamespaceNode", "NAMESPACE", "String name");
        
        // Configuration nodes
        createNode("ConfigurationNode", "CONFIGURATION");
        createNode("ConstraintNode", "CONSTRAINT", "String target", "String constraint");
        
        // Variable nodes
        createNode("VarGroupDefinitionNode", "VAR_GROUP_DEFINITION", "String name");
        createNode("VarGroupUsageNode", "VAR_GROUP_USAGE", "String varGroup", "String varName");
        
        // Literal nodes
        createNode("StringLiteralNode", "STRING_LITERAL", "String value");
        createNode("NumberLiteralNode", "NUMBER_LITERAL", "double value");
        createNode("UnquotedLiteralNode", "UNQUOTED_LITERAL", "String value");
        
        // Other nodes
        createNode("InheritStatementNode", "INHERIT_STATEMENT", "String parent");
    }
    
    static void createNode(String className, String nodeType, String... fields) throws IOException {
        StringBuilder fieldDecls = new StringBuilder();
        StringBuilder constructor = new StringBuilder();
        StringBuilder getters = new StringBuilder();
        
        constructor.append("    public ").append(className).append("(");
        
        for (int i = 0; i < fields.length; i++) {
            String field = fields[i];
            String[] parts = field.split(" ");
            String type = parts[0];
            String name = parts[1];
            
            fieldDecls.append("    private ").append(field).append(";\n");
            
            if (i > 0) constructor.append(", ");
            constructor.append(type).append(" ").append(name);
            
            getters.append("    public ").append(type).append(" get")
                   .append(Character.toUpperCase(name.charAt(0)))
                   .append(name.substring(1)).append("() { return ")
                   .append(name).append("; }\n");
        }
        
        constructor.append(") {\n");
        constructor.append("        super(NodeType.").append(nodeType).append(");\n");
        
        for (String field : fields) {
            String name = field.split(" ")[1];
            constructor.append("        this.").append(name).append(" = ").append(name).append(";\n");
        }
        constructor.append("    }\n");
        
        String classBody = fieldDecls.toString() + "\n" + 
                          constructor.toString() + "\n" + 
                          getters.toString();
        
        createClass("com.chtl.ast.node", className, classBody);
    }
    
    static void createElementNode(String className) throws IOException {
        String shortName = className.replace("Node", "").toLowerCase();
        
        String classBody = """
import com.chtl.model.%s;

public class %s extends AbstractCHTLASTNode {
    private %s %s;
    
    public %s(%s %s) {
        super(NodeType.%s);
        this.%s = %s;
    }
    
    public %s get%s() {
        return %s;
    }
}
""".formatted(className.replace("Node", ""),
              className,
              className.replace("Node", ""), shortName,
              className, className.replace("Node", ""), shortName,
              className.replace("Node", "").toUpperCase(),
              shortName, shortName,
              className.replace("Node", ""), className.replace("Node", ""),
              shortName);

        createClass("com.chtl.ast.node", className, classBody);
    }
    
    static void createClass(String packageName, String className, String classBody) throws IOException {
        String path = "src/main/java/" + packageName.replace('.', '/');
        Files.createDirectories(Paths.get(path));
        
        String fullClass;
        if (classBody.contains("package ")) {
            fullClass = classBody;
        } else if (classBody.contains("import ")) {
            fullClass = "package " + packageName + ";\n\n" + classBody;
        } else {
            fullClass = "package " + packageName + ";\n\n" +
                       "import com.chtl.ast.AbstractCHTLASTNode;\n" +
                       "import com.chtl.ast.NodeType;\n\n" +
                       "public class " + className + " extends AbstractCHTLASTNode {\n" +
                       classBody + "\n}";
        }
        
        Path filePath = Paths.get(path + "/" + className + ".java");
        Files.write(filePath, fullClass.getBytes());
        System.out.println("  Created: " + packageName + "." + className);
    }
}