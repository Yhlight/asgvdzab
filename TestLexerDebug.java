import com.chtl.compiler.chtl.lexer.CHTLLexer;
import com.chtl.compiler.chtl.token.CHTLToken;
import java.util.List;

public class TestLexerDebug {
    public static void main(String[] args) {
        testLexer("div[1] { }");
        testLexer("[Template] @Element Box { }");
        testLexer("2px");
        testLexer("borderWidth: 2px;");
    }
    
    private static void testLexer(String code) {
        System.out.println("\nTokens for: " + code);
        CHTLLexer lexer = new CHTLLexer(code);
        List<CHTLToken> tokens = lexer.tokenize();
        
        for (CHTLToken token : tokens) {
            System.out.println("  " + token.getType() + " : '" + token.getValue() + "'");
        }
    }
}