import com.chtl.compiler.chtljs.lexer.CHTLJSLexer;
import com.chtl.compiler.chtljs.token.CHTLJSToken;
import java.util.List;

public class TestCHTLJSLexerUnquoted {
    public static void main(String[] args) {
        String code = "easing: ease-in-out,";
        
        CHTLJSLexer lexer = new CHTLJSLexer(code);
        List<CHTLJSToken> tokens = lexer.tokenize();
        
        System.out.println("Tokens:");
        for (CHTLJSToken token : tokens) {
            System.out.println(token.getType() + ": " + token.getValue());
        }
    }
}