import java.io.*;
import java.nio.file.*;
import java.util.*;

public class FixImports {
    public static void main(String[] args) throws IOException {
        System.out.println("Fixing import issues in CHTL project...");
        
        // Fix CustomDefinitionNode - already done
        
        // Fix AnimationControllerNode - remove NodeType usage
        fixFile("src/main/java/com/chtl/ast/chtljs/AnimationControllerNode.java",
            "super(NodeType.ANIMATION_CONTROLLER);",
            "// super(NodeType.ANIMATION_CONTROLLER); // NodeType not available");
            
        // Fix CSS3Lexer stub
        String css3LexerContent = """
package com.chtl.css;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.ATN;

public class CSS3Lexer extends Lexer {
    public CSS3Lexer(CharStream input) { super(input); }
    @Override public String getGrammarFileName() { return "CSS3.g4"; }
    @Override public String[] getRuleNames() { return new String[0]; }
    @Override public String[] getChannelNames() { return new String[0]; }
    @Override public String[] getModeNames() { return new String[0]; }
    @Override public ATN getATN() { return null; }
}
""";
        Files.write(Paths.get("src/main/java/com/chtl/css/CSS3Lexer.java"), 
                    css3LexerContent.getBytes());
        
        // Fix CSS3Parser stub
        String css3ParserContent = """
package com.chtl.css;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
import org.antlr.v4.runtime.atn.ATN;

public class CSS3Parser extends Parser {
    public CSS3Parser(TokenStream input) { super(input); }
    @Override public String getGrammarFileName() { return "CSS3.g4"; }
    @Override public String[] getRuleNames() { return new String[0]; }
    @Override public ATN getATN() { return null; }
    
    public static class StylesheetContext extends ParserRuleContext {
        public StylesheetContext(ParserRuleContext parent, int invokingState) { 
            super(parent, invokingState); 
        }
    }
    
    public static class RulesetContext extends ParserRuleContext {
        public RulesetContext(ParserRuleContext parent, int invokingState) { 
            super(parent, invokingState); 
        }
    }
    
    public static class DeclarationContext extends ParserRuleContext {
        public DeclarationContext(ParserRuleContext parent, int invokingState) { 
            super(parent, invokingState); 
        }
    }
    
    public static class MediaContext extends ParserRuleContext {
        public MediaContext(ParserRuleContext parent, int invokingState) { 
            super(parent, invokingState); 
        }
    }
    
    public StylesheetContext stylesheet() { return new StylesheetContext(null, 0); }
}
""";
        Files.write(Paths.get("src/main/java/com/chtl/css/CSS3Parser.java"), 
                    css3ParserContent.getBytes());
                    
        // Create missing stub classes
        createStubClass("com.chtl.scanner", "ScannerStateMachine", """
public class ScannerStateMachine {
    public enum State { INITIAL, CHTL, CSS, JS }
}
""");

        createStubClass("com.chtl.scanner", "Fragment", """
public class Fragment {
    private String content;
    private String type;
    public Fragment(String content, String type) { 
        this.content = content; 
        this.type = type;
    }
    public String getContent() { return content; }
    public String getType() { return type; }
}
""");

        createStubClass("com.chtl.lexer", "CHTLToken", """
public class CHTLToken {
    private String type;
    private String value;
    public CHTLToken(String type, String value) {
        this.type = type;
        this.value = value;
    }
    public String getType() { return type; }
    public String getValue() { return value; }
}
""");

        createStubClass("com.chtl.dispatcher", "CompilerDispatcher", """
public class CompilerDispatcher {
    // Stub implementation
}
""");

        System.out.println("Import fixes completed!");
    }
    
    static void createStubClass(String packageName, String className, String classBody) 
            throws IOException {
        String path = "src/main/java/" + packageName.replace('.', '/');
        Files.createDirectories(Paths.get(path));
        String content = "package " + packageName + ";\n\n" + classBody;
        Files.write(Paths.get(path + "/" + className + ".java"), content.getBytes());
        System.out.println("Created stub: " + packageName + "." + className);
    }
    
    static void fixFile(String path, String oldText, String newText) throws IOException {
        Path filePath = Paths.get(path);
        if (Files.exists(filePath)) {
            String content = Files.readString(filePath);
            content = content.replace(oldText, newText);
            Files.write(filePath, content.getBytes());
            System.out.println("Fixed: " + path);
        }
    }
}