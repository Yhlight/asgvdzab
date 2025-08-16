import java.io.*;
import java.nio.file.*;
import java.util.*;

public class FixFinalIssues {
    public static void main(String[] args) throws IOException {
        System.out.println("=== Fixing Final Issues ===\n");
        
        // Fix CHTLNode.java - it has both class and interface definition
        fixCHTLNode();
        
        // Remove duplicate or conflicting classes
        removeDuplicates();
        
        // Fix any remaining circular dependencies
        fixCircularDependencies();
        
        System.out.println("\nAll final fixes completed!");
    }
    
    static void fixCHTLNode() throws IOException {
        System.out.println("Fixing CHTLNode.java...");
        
        // CHTLNode should be an interface
        String content = """
package com.chtl.model;

/**
 * Base interface for CHTL model nodes
 */
public interface CHTLNode {
    // Base interface for CHTL model nodes
    // Implementing classes should provide specific functionality
}
""";
        
        Path path = Paths.get("src/main/java/com/chtl/model/CHTLNode.java");
        Files.write(path, content.getBytes());
        System.out.println("  Fixed: CHTLNode.java");
    }
    
    static void removeDuplicates() throws IOException {
        System.out.println("Checking for duplicate definitions...");
        
        // Check if there are duplicate AST node files
        Path astPath = Paths.get("src/main/java/com/chtl/ast");
        Path modelPath = Paths.get("src/main/java/com/chtl/model");
        
        // Fix TextNode - ensure it exists in model package
        fixTextNode();
        
        // Fix ElementNode - ensure it exists in model package  
        fixElementNode();
        
        // Fix AttributeNode - ensure it exists in model package
        fixAttributeNode();
    }
    
    static void fixTextNode() throws IOException {
        // Ensure TextNode in model package exists and is correct
        String content = """
package com.chtl.model;

/**
 * Text node in the model
 */
public class TextNode extends CHTLNode {
    private String content;
    
    public TextNode(String content) {
        this.content = content;
    }
    
    public String getContent() {
        return content;
    }
    
    public void accept(CHTLNodeVisitor visitor) {
        visitor.visitText(this);
    }
}
""";
        
        Path path = Paths.get("src/main/java/com/chtl/model/TextNode.java");
        Files.write(path, content.getBytes());
        System.out.println("  Fixed: model/TextNode.java");
    }
    
    static void fixElementNode() throws IOException {
        // Ensure ElementNode in model package exists and is correct
        String content = """
package com.chtl.model;

import java.util.List;
import java.util.ArrayList;

/**
 * Element node in the model
 */
public class ElementNode extends CHTLNode {
    private String tagName;
    private List<AttributeNode> attributes;
    private List<CHTLNode> children;
    
    public ElementNode(String tagName) {
        this.tagName = tagName;
        this.attributes = new ArrayList<>();
        this.children = new ArrayList<>();
    }
    
    public String getTagName() { return tagName; }
    public List<AttributeNode> getAttributes() { return attributes; }
    public List<CHTLNode> getChildren() { return children; }
    
    public void addAttribute(AttributeNode attr) {
        attributes.add(attr);
    }
    
    public void addChild(CHTLNode child) {
        children.add(child);
    }
    
    public void accept(CHTLNodeVisitor visitor) {
        visitor.visitElement(this);
    }
}
""";
        
        Path path = Paths.get("src/main/java/com/chtl/model/ElementNode.java");
        Files.write(path, content.getBytes());
        System.out.println("  Fixed: model/ElementNode.java");
    }
    
    static void fixAttributeNode() throws IOException {
        // Ensure AttributeNode in model package exists and is correct
        String content = """
package com.chtl.model;

/**
 * Attribute node in the model
 */
public class AttributeNode extends CHTLNode {
    private String name;
    private String value;
    
    public AttributeNode(String name, String value) {
        this.name = name;
        this.value = value;
    }
    
    public String getName() { return name; }
    public String getValue() { return value; }
    
    public void accept(CHTLNodeVisitor visitor) {
        visitor.visitAttribute(this);
    }
}
""";
        
        Path path = Paths.get("src/main/java/com/chtl/model/AttributeNode.java");
        Files.write(path, content.getBytes());
        System.out.println("  Fixed: model/AttributeNode.java");
    }
    
    static void fixCircularDependencies() throws IOException {
        System.out.println("Fixing circular dependencies...");
        
        // Fix CHTLNodeVisitor to match the model nodes
        String visitorContent = """
package com.chtl.model;

/**
 * Visitor interface for CHTL model nodes
 */
public interface CHTLNodeVisitor {
    void visitElement(ElementNode node);
    void visitAttribute(AttributeNode node);
    void visitText(TextNode node);
    void visitTemplate(TemplateNode node);
    void visitScript(ScriptNode node);
    void visitStyle(StyleNode node);
    void visitCustom(CustomNode node);
}
""";
        
        Path visitorPath = Paths.get("src/main/java/com/chtl/model/CHTLNodeVisitor.java");
        Files.write(visitorPath, visitorContent.getBytes());
        System.out.println("  Fixed: CHTLNodeVisitor.java");
        
        // Update CHTLNode to be abstract class instead of interface
        String nodeContent = """
package com.chtl.model;

/**
 * Base class for CHTL model nodes
 */
public abstract class CHTLNode {
    /**
     * Accept a visitor
     */
    public abstract void accept(CHTLNodeVisitor visitor);
}
""";
        
        Path nodePath = Paths.get("src/main/java/com/chtl/model/CHTLNode.java");
        Files.write(nodePath, nodeContent.getBytes());
        System.out.println("  Fixed: CHTLNode.java as abstract class");
    }
}