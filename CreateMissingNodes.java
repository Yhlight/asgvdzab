import java.io.*;
import java.nio.file.*;

public class CreateMissingNodes {
    public static void main(String[] args) throws IOException {
        System.out.println("Creating missing AST node classes...");
        
        createNode("CHTLJSAnimateNode", """
import com.chtl.ast.CHTLJSVisitor;
import java.util.Map;
import java.util.HashMap;

public class CHTLJSAnimateNode extends AbstractCHTLASTNode {
    private CHTLASTNode target;
    private Map<String, Object> properties;
    
    public CHTLJSAnimateNode() {
        this.properties = new HashMap<>();
    }
    
    public CHTLASTNode getTarget() { return target; }
    public void setTarget(CHTLASTNode target) { this.target = target; }
    
    public Map<String, Object> getProperties() { return properties; }
    public void setProperty(String key, Object value) {
        properties.put(key, value);
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSAnimate(this);
        }
    }
}
""");

        createNode("CHTLJSObjectNode", """
import com.chtl.ast.CHTLJSVisitor;
import java.util.Map;
import java.util.LinkedHashMap;

public class CHTLJSObjectNode extends AbstractCHTLASTNode {
    private Map<String, CHTLASTNode> properties;
    
    public CHTLJSObjectNode() {
        this.properties = new LinkedHashMap<>();
    }
    
    public Map<String, CHTLASTNode> getProperties() { return properties; }
    
    public void setProperty(String key, CHTLASTNode value) {
        properties.put(key, value);
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSObject(this);
        }
    }
}
""");

        createNode("CHTLJSArrayNode", """
import com.chtl.ast.CHTLJSVisitor;
import java.util.List;
import java.util.ArrayList;

public class CHTLJSArrayNode extends AbstractCHTLASTNode {
    private List<CHTLASTNode> elements;
    
    public CHTLJSArrayNode() {
        this.elements = new ArrayList<>();
    }
    
    public List<CHTLASTNode> getElements() { return elements; }
    
    public void addElement(CHTLASTNode element) {
        elements.add(element);
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSArray(this);
        }
    }
}
""");

        createNode("CHTLJSFunctionNode", """
import com.chtl.ast.CHTLJSVisitor;
import java.util.List;
import java.util.ArrayList;

public class CHTLJSFunctionNode extends AbstractCHTLASTNode {
    private String name;
    private List<String> parameters;
    private CHTLASTNode body;
    
    public CHTLJSFunctionNode(String name) {
        this.name = name;
        this.parameters = new ArrayList<>();
    }
    
    public String getName() { return name; }
    public List<String> getParameters() { return parameters; }
    public CHTLASTNode getBody() { return body; }
    
    public void addParameter(String param) {
        parameters.add(param);
    }
    
    public void setBody(CHTLASTNode body) {
        this.body = body;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSFunction(this);
        }
    }
}
""");

        createNode("CHTLJSIdentifierNode", """
import com.chtl.ast.CHTLJSVisitor;

public class CHTLJSIdentifierNode extends AbstractCHTLASTNode {
    private String name;
    
    public CHTLJSIdentifierNode(String name) {
        this.name = name;
    }
    
    public String getName() { return name; }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSIdentifier(this);
        }
    }
}
""");

        createNode("CHTLJSExpressionNode", """
public abstract class CHTLJSExpressionNode extends AbstractCHTLASTNode {
    // Base class for CHTL JS expressions
}
""");

        System.out.println("All missing nodes created!");
    }
    
    static void createNode(String className, String classBody) throws IOException {
        String path = "src/main/java/com/chtl/ast/node/" + className + ".java";
        String content = "package com.chtl.ast.node;\n\n" + classBody;
        Files.write(Paths.get(path), content.getBytes());
        System.out.println("  Created: " + className);
    }
}