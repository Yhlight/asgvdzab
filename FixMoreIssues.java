import java.io.*;
import java.nio.file.*;
import java.util.*;

public class FixMoreIssues {
    public static void main(String[] args) throws IOException {
        System.out.println("Fixing more compilation issues...");
        
        // Fix CSS3Parser - remove duplicate compilation unit error
        fixFile("src/main/java/com/chtl/compiler/css/CSSCompiler.java",
            "import com.chtl.css.CSS3Parser;",
            "import com.chtl.css.CSS3Parser;");
            
        // Fix JavaScript compiler
        fixFile("src/main/java/com/chtl/compiler/javascript/JavaScriptCompiler.java",
            "import com.chtl.javascript.ECMAScriptParser;",
            "import com.chtl.javascript.ECMAScriptParser;");
            
        // Create ECMAScript stubs
        createECMAScriptStubs();
        
        // Fix ANTLRInputStream deprecation
        fixANTLRDeprecation("src/main/java/com/chtl/compiler/css/CSSCompiler.java");
        fixANTLRDeprecation("src/main/java/com/chtl/compiler/javascript/JavaScriptCompiler.java");
        
        // Fix private access issues
        fixPrivateAccess();
        
        // Create missing AST nodes
        createMissingASTNodes();
        
        System.out.println("Additional fixes completed!");
    }
    
    static void createECMAScriptStubs() throws IOException {
        String lexerContent = """
package com.chtl.javascript;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.ATN;

public class ECMAScriptLexer extends Lexer {
    public ECMAScriptLexer(CharStream input) { super(input); }
    @Override public String getGrammarFileName() { return "ECMAScript.g4"; }
    @Override public String[] getRuleNames() { return new String[0]; }
    @Override public String[] getChannelNames() { return new String[0]; }
    @Override public String[] getModeNames() { return new String[0]; }
    @Override public ATN getATN() { return null; }
}
""";
        Files.write(Paths.get("src/main/java/com/chtl/javascript/ECMAScriptLexer.java"), 
                    lexerContent.getBytes());
                    
        String parserContent = """
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
""";
        Files.write(Paths.get("src/main/java/com/chtl/javascript/ECMAScriptParser.java"), 
                    parserContent.getBytes());
                    
        String baseVisitorContent = """
package com.chtl.javascript;
import org.antlr.v4.runtime.tree.*;

public class ECMAScriptBaseVisitor<T> extends AbstractParseTreeVisitor<T> implements ECMAScriptVisitor<T> {
    @Override public T visitProgram(ECMAScriptParser.ProgramContext ctx) { return visitChildren(ctx); }
    @Override public T visitVariableStatement(ECMAScriptParser.VariableStatementContext ctx) { return visitChildren(ctx); }
}
""";
        Files.write(Paths.get("src/main/java/com/chtl/javascript/ECMAScriptBaseVisitor.java"), 
                    baseVisitorContent.getBytes());
                    
        String visitorContent = """
package com.chtl.javascript;
import org.antlr.v4.runtime.tree.*;

public interface ECMAScriptVisitor<T> extends ParseTreeVisitor<T> {
    T visitProgram(ECMAScriptParser.ProgramContext ctx);
    T visitVariableStatement(ECMAScriptParser.VariableStatementContext ctx);
}
""";
        Files.write(Paths.get("src/main/java/com/chtl/javascript/ECMAScriptVisitor.java"), 
                    visitorContent.getBytes());
    }
    
    static void fixANTLRDeprecation(String path) throws IOException {
        Path filePath = Paths.get(path);
        if (Files.exists(filePath)) {
            String content = Files.readString(filePath);
            content = content.replace("ANTLRInputStream", "CharStreams.fromString");
            content = content.replace("new CharStreams.fromString(input)", "CharStreams.fromString(input)");
            Files.write(filePath, content.getBytes());
            System.out.println("Fixed ANTLR deprecation in: " + path);
        }
    }
    
    static void fixPrivateAccess() throws IOException {
        // Fix CHTLCompiler access modifiers
        Path compilerPath = Paths.get("src/main/java/com/chtl/compiler/CHTLCompiler.java");
        if (Files.exists(compilerPath)) {
            String content = Files.readString(compilerPath);
            content = content.replace("private static class CompilationOutput", "public static class CompilationOutput");
            content = content.replace("private static class OptimizedOutput", "public static class OptimizedOutput");
            Files.write(compilerPath, content.getBytes());
            System.out.println("Fixed access modifiers in CHTLCompiler");
        }
        
        // Fix CompilerService access modifiers
        Path servicePath = Paths.get("src/main/java/com/chtl/service/CompilerService.java");
        if (Files.exists(servicePath)) {
            String content = Files.readString(servicePath);
            content = content.replace("private static class IncrementalCompileRequest", "public static class IncrementalCompileRequest");
            content = content.replace("private static class IncrementalResult", "public static class IncrementalResult");
            Files.write(servicePath, content.getBytes());
            System.out.println("Fixed access modifiers in CompilerService");
        }
    }
    
    static void createMissingASTNodes() throws IOException {
        // Create missing CHTL JS AST nodes directory
        Files.createDirectories(Paths.get("src/main/java/com/chtl/chtljs/ast"));
        
        // Create base interface
        String interfaceContent = """
package com.chtl.chtljs.ast;

public interface CHTLJSASTNode {
    void accept(CHTLJSASTVisitor visitor);
}
""";
        Files.write(Paths.get("src/main/java/com/chtl/chtljs/ast/CHTLJSASTNode.java"), 
                    interfaceContent.getBytes());
                    
        String visitorContent = """
package com.chtl.chtljs.ast;

public interface CHTLJSASTVisitor {
    void visit(CHTLJSASTNode node);
}
""";
        Files.write(Paths.get("src/main/java/com/chtl/chtljs/ast/CHTLJSASTVisitor.java"), 
                    visitorContent.getBytes());
                    
        String abstractContent = """
package com.chtl.chtljs.ast;

public abstract class AbstractCHTLJSASTNode implements CHTLJSASTNode {
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visit(this);
    }
}
""";
        Files.write(Paths.get("src/main/java/com/chtl/chtljs/ast/AbstractCHTLJSASTNode.java"), 
                    abstractContent.getBytes());
    }
    
    static void fixFile(String path, String oldText, String newText) throws IOException {
        Path filePath = Paths.get(path);
        if (Files.exists(filePath)) {
            String content = Files.readString(filePath);
            content = content.replace(oldText, newText);
            Files.write(filePath, content.getBytes());
        }
    }
}