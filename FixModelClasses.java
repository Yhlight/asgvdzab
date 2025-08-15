import java.io.*;
import java.nio.file.*;

public class FixModelClasses {
    public static void main(String[] args) throws IOException {
        System.out.println("Creating model classes...\n");
        
        // Create Element model class
        createClass("com.chtl.model", "Element", """
import java.util.List;
import java.util.ArrayList;

public class Element {
    private String tagName;
    private List<Attribute> attributes;
    private List<Element> children;
    
    public Element(String tagName) {
        this.tagName = tagName;
        this.attributes = new ArrayList<>();
        this.children = new ArrayList<>();
    }
    
    public String getTagName() { return tagName; }
    public List<Attribute> getAttributes() { return attributes; }
    public List<Element> getChildren() { return children; }
    
    public void addAttribute(Attribute attr) {
        attributes.add(attr);
    }
    
    public void addChild(Element child) {
        children.add(child);
    }
}
""");

        // Create Attribute model class
        createClass("com.chtl.model", "Attribute", """
public class Attribute {
    private String name;
    private String value;
    
    public Attribute(String name, String value) {
        this.name = name;
        this.value = value;
    }
    
    public String getName() { return name; }
    public String getValue() { return value; }
}
""");

        // Create Text model class
        createClass("com.chtl.model", "Text", """
public class Text {
    private String content;
    
    public Text(String content) {
        this.content = content;
    }
    
    public String getContent() { return content; }
}
""");

        // Now fix the node classes to use model wrappers properly
        // Fix ElementNode
        fixNodeClass("ElementNode", "Element");
        fixNodeClass("AttributeNode", "Attribute");
        fixNodeClass("TextNode", "Text");
        
        System.out.println("\nModel classes created and nodes fixed!");
    }
    
    static void createClass(String packageName, String className, String classBody) throws IOException {
        String path = "src/main/java/" + packageName.replace('.', '/');
        Files.createDirectories(Paths.get(path));
        String content = "package " + packageName + ";\n\n" + classBody;
        Path filePath = Paths.get(path + "/" + className + ".java");
        Files.write(filePath, content.getBytes());
        System.out.println("Created: " + packageName + "." + className);
    }
    
    static void fixNodeClass(String nodeName, String modelName) throws IOException {
        Path path = Paths.get("src/main/java/com/chtl/ast/node/" + nodeName + ".java");
        if (Files.exists(path)) {
            String content = Files.readString(path);
            // Fix the model import
            content = content.replace("import com.chtl.model." + modelName + "Node;", 
                                    "import com.chtl.model." + modelName + ";");
            Files.write(path, content.getBytes());
            System.out.println("Fixed: " + nodeName);
        }
    }
}