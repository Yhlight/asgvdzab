import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;

public class FixANTLRAndParserIssues {
    private static int filesFixed = 0;
    
    public static void main(String[] args) throws IOException {
        System.out.println("Fixing ANTLR and parser issues...");
        
        // Fix ANTLR imports
        fixANTLRImports();
        
        // Fix parser/lexer generation issues
        fixParserLexerIssues();
        
        // Fix visitor pattern implementations
        fixVisitorImplementations();
        
        // Fix parser context usage
        fixParserContextUsage();
        
        System.out.println("\nFixed " + filesFixed + " files");
    }
    
    private static void fixANTLRImports() throws IOException {
        Map<String, String> antlrImports = new HashMap<>();
        antlrImports.put("ParserRuleContext", "import org.antlr.v4.runtime.ParserRuleContext;");
        antlrImports.put("ParseTree", "import org.antlr.v4.runtime.tree.ParseTree;");
        antlrImports.put("TerminalNode", "import org.antlr.v4.runtime.tree.TerminalNode;");
        antlrImports.put("Token", "import org.antlr.v4.runtime.Token;");
        antlrImports.put("TokenStream", "import org.antlr.v4.runtime.TokenStream;");
        antlrImports.put("CharStream", "import org.antlr.v4.runtime.CharStream;");
        antlrImports.put("CharStreams", "import org.antlr.v4.runtime.CharStreams;");
        antlrImports.put("CommonTokenStream", "import org.antlr.v4.runtime.CommonTokenStream;");
        antlrImports.put("RecognitionException", "import org.antlr.v4.runtime.RecognitionException;");
        antlrImports.put("BaseErrorListener", "import org.antlr.v4.runtime.BaseErrorListener;");
        antlrImports.put("Parser", "import org.antlr.v4.runtime.Parser;");
        antlrImports.put("Lexer", "import org.antlr.v4.runtime.Lexer;");
        
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .filter(p -> p.toString().contains("parser") || p.toString().contains("Parser") ||
                        p.toString().contains("lexer") || p.toString().contains("Lexer") ||
                        p.toString().contains("visitor") || p.toString().contains("Visitor"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    Set<String> importsToAdd = new HashSet<>();
                    boolean modified = false;
                    
                    for (Map.Entry<String, String> entry : antlrImports.entrySet()) {
                        String symbol = entry.getKey();
                        String importStmt = entry.getValue();
                        
                        if (content.contains(symbol) && !content.contains(importStmt)) {
                            importsToAdd.add(importStmt);
                            modified = true;
                        }
                    }
                    
                    if (modified) {
                        content = addImportsAfterPackage(content, importsToAdd);
                        Files.writeString(path, content);
                        filesFixed++;
                        System.out.println("Fixed ANTLR imports in: " + path.getFileName());
                    }
                } catch (IOException e) {
                    System.err.println("Error processing " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static void fixParserLexerIssues() throws IOException {
        // Fix ECMAScriptParser issues
        Path ecmaParser = Paths.get("src/main/java/com/chtl/javascript/ECMAScriptParser.java");
        if (Files.exists(ecmaParser)) {
            String content = Files.readString(ecmaParser);
            
            // Remove self-referential imports
            content = content.replaceAll("import com\\.chtl\\.javascript\\.ECMAScriptParser\\.[^;]+;\\s*", "");
            
            // Fix class declaration if needed
            if (!content.contains("public class ECMAScriptParser extends Parser")) {
                content = content.replace("public class ECMAScriptParser", 
                                        "public class ECMAScriptParser extends Parser");
            }
            
            Files.writeString(ecmaParser, content);
            filesFixed++;
            System.out.println("Fixed ECMAScriptParser.java");
        }
        
        // Fix ECMAScriptLexer issues
        Path ecmaLexer = Paths.get("src/main/java/com/chtl/javascript/ECMAScriptLexer.java");
        if (Files.exists(ecmaLexer)) {
            String content = Files.readString(ecmaLexer);
            
            // Fix class declaration if needed
            if (!content.contains("public class ECMAScriptLexer extends Lexer")) {
                content = content.replace("public class ECMAScriptLexer", 
                                        "public class ECMAScriptLexer extends Lexer");
            }
            
            Files.writeString(ecmaLexer, content);
            filesFixed++;
            System.out.println("Fixed ECMAScriptLexer.java");
        }
    }
    
    private static void fixVisitorImplementations() throws IOException {
        // Find all visitor implementations
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith("Visitor.java") || p.toString().endsWith("BaseVisitor.java"))
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    boolean modified = false;
                    
                    // Ensure proper imports
                    if (content.contains("extends") && content.contains("BaseVisitor")) {
                        if (!content.contains("import org.antlr.v4.runtime.tree.*;")) {
                            content = addImportAfterPackage(content, "import org.antlr.v4.runtime.tree.*;");
                            modified = true;
                        }
                    }
                    
                    // Fix generic type parameters
                    content = content.replaceAll("BaseVisitor<>", "BaseVisitor<Object>");
                    if (content.contains("BaseVisitor<>")) {
                        modified = true;
                    }
                    
                    if (modified) {
                        Files.writeString(path, content);
                        filesFixed++;
                        System.out.println("Fixed visitor implementation: " + path.getFileName());
                    }
                } catch (IOException e) {
                    System.err.println("Error processing " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static void fixParserContextUsage() throws IOException {
        // Fix files that use parser contexts
        Files.walk(Paths.get("src/main/java"))
            .filter(p -> p.toString().endsWith(".java"))
            .filter(p -> {
                try {
                    String content = Files.readString(p);
                    return content.contains("Context") && 
                           (content.contains("visit") || content.contains("enter") || content.contains("exit"));
                } catch (IOException e) {
                    return false;
                }
            })
            .forEach(path -> {
                try {
                    String content = Files.readString(path);
                    boolean modified = false;
                    
                    // Fix missing context imports
                    if (content.contains("ParserRuleContext") && 
                        !content.contains("import org.antlr.v4.runtime.ParserRuleContext;")) {
                        content = addImportAfterPackage(content, "import org.antlr.v4.runtime.ParserRuleContext;");
                        modified = true;
                    }
                    
                    // Fix visitor method signatures
                    Pattern visitorPattern = Pattern.compile("public\\s+\\w+\\s+visit\\w+\\(\\s*\\)");
                    Matcher matcher = visitorPattern.matcher(content);
                    if (matcher.find()) {
                        content = matcher.replaceAll(m -> {
                            String method = m.group();
                            return method.substring(0, method.length() - 1) + "ParserRuleContext ctx)";
                        });
                        modified = true;
                    }
                    
                    if (modified) {
                        Files.writeString(path, content);
                        filesFixed++;
                        System.out.println("Fixed parser context usage in: " + path.getFileName());
                    }
                } catch (IOException e) {
                    System.err.println("Error processing " + path + ": " + e.getMessage());
                }
            });
    }
    
    private static String addImportsAfterPackage(String content, Set<String> imports) {
        int packageEnd = content.indexOf(';');
        if (packageEnd > 0) {
            StringBuilder sb = new StringBuilder();
            sb.append(content.substring(0, packageEnd + 1));
            sb.append("\n");
            for (String imp : imports) {
                sb.append("\n").append(imp);
            }
            sb.append("\n");
            sb.append(content.substring(packageEnd + 1));
            return sb.toString();
        }
        return content;
    }
    
    private static String addImportAfterPackage(String content, String importStmt) {
        return addImportsAfterPackage(content, Collections.singleton(importStmt));
    }
}