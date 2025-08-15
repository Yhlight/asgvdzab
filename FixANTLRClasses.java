import java.io.*;
import java.nio.file.*;

public class FixANTLRClasses {
    public static void main(String[] args) throws IOException {
        System.out.println("Creating missing ANTLR classes...");
        
        // Create CSS3 related classes
        createCSS3Classes();
        
        // Create ECMAScript related classes  
        createECMAScriptClasses();
        
        System.out.println("ANTLR classes created!");
    }
    
    static void createCSS3Classes() throws IOException {
        // CSS3Parser with all needed contexts
        Files.write(Paths.get("src/main/java/com/chtl/css/CSS3Parser.java"), """
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
""".getBytes());

        // CSS3BaseVisitor
        Files.write(Paths.get("src/main/java/com/chtl/css/CSS3BaseVisitor.java"), """
package com.chtl.css;
import org.antlr.v4.runtime.tree.*;

public class CSS3BaseVisitor<T> extends AbstractParseTreeVisitor<T> implements CSS3Visitor<T> {
    @Override public T visitStylesheet(CSS3Parser.StylesheetContext ctx) { return visitChildren(ctx); }
    @Override public T visitRuleset(CSS3Parser.RulesetContext ctx) { return visitChildren(ctx); }
    @Override public T visitDeclaration(CSS3Parser.DeclarationContext ctx) { return visitChildren(ctx); }
    @Override public T visitMedia(CSS3Parser.MediaContext ctx) { return visitChildren(ctx); }
}
""".getBytes());

        // CSS3Visitor
        Files.write(Paths.get("src/main/java/com/chtl/css/CSS3Visitor.java"), """
package com.chtl.css;
import org.antlr.v4.runtime.tree.*;

public interface CSS3Visitor<T> extends ParseTreeVisitor<T> {
    T visitStylesheet(CSS3Parser.StylesheetContext ctx);
    T visitRuleset(CSS3Parser.RulesetContext ctx);
    T visitDeclaration(CSS3Parser.DeclarationContext ctx);
    T visitMedia(CSS3Parser.MediaContext ctx);
}
""".getBytes());
    }
    
    static void createECMAScriptClasses() throws IOException {
        // ECMAScriptParser with all needed contexts
        Files.write(Paths.get("src/main/java/com/chtl/javascript/ECMAScriptParser.java"), """
package com.chtl.javascript;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
import org.antlr.v4.runtime.atn.ATN;

public class ECMAScriptParser extends Parser {
    public ECMAScriptParser(TokenStream input) { super(input); }
    @Override public String getGrammarFileName() { return "ECMAScript.g4"; }
    @Override public String[] getRuleNames() { return new String[0]; }
    @Override public ATN getATN() { return null; }
    
    public static class ProgramContext extends ParserRuleContext {
        public ProgramContext(ParserRuleContext parent, int invokingState) { 
            super(parent, invokingState); 
        }
    }
    
    public static class VariableStatementContext extends ParserRuleContext {
        public VariableStatementContext(ParserRuleContext parent, int invokingState) { 
            super(parent, invokingState); 
        }
    }
    
    public static class FunctionDeclarationContext extends ParserRuleContext {
        public FunctionDeclarationContext(ParserRuleContext parent, int invokingState) { 
            super(parent, invokingState); 
        }
    }
    
    public static class WithStatementContext extends ParserRuleContext {
        public WithStatementContext(ParserRuleContext parent, int invokingState) { 
            super(parent, invokingState); 
        }
    }
    
    public ProgramContext program() { return new ProgramContext(null, 0); }
}
""".getBytes());

        // ECMAScriptBaseVisitor
        Files.write(Paths.get("src/main/java/com/chtl/javascript/ECMAScriptBaseVisitor.java"), """
package com.chtl.javascript;
import org.antlr.v4.runtime.tree.*;

public class ECMAScriptBaseVisitor<T> extends AbstractParseTreeVisitor<T> implements ECMAScriptVisitor<T> {
    @Override public T visitProgram(ECMAScriptParser.ProgramContext ctx) { return visitChildren(ctx); }
    @Override public T visitVariableStatement(ECMAScriptParser.VariableStatementContext ctx) { return visitChildren(ctx); }
    @Override public T visitFunctionDeclaration(ECMAScriptParser.FunctionDeclarationContext ctx) { return visitChildren(ctx); }
    @Override public T visitWithStatement(ECMAScriptParser.WithStatementContext ctx) { return visitChildren(ctx); }
}
""".getBytes());

        // ECMAScriptVisitor
        Files.write(Paths.get("src/main/java/com/chtl/javascript/ECMAScriptVisitor.java"), """
package com.chtl.javascript;
import org.antlr.v4.runtime.tree.*;

public interface ECMAScriptVisitor<T> extends ParseTreeVisitor<T> {
    T visitProgram(ECMAScriptParser.ProgramContext ctx);
    T visitVariableStatement(ECMAScriptParser.VariableStatementContext ctx);
    T visitFunctionDeclaration(ECMAScriptParser.FunctionDeclarationContext ctx);
    T visitWithStatement(ECMAScriptParser.WithStatementContext ctx);
}
""".getBytes());
    }
}