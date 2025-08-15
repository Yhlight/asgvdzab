import com.chtl.compiler.chtljs.lexer.CHTLJSLexer;
import com.chtl.compiler.chtljs.token.CHTLJSToken;
import java.util.List;

public class TestCHTLJSLexerDebug {
    public static void main(String[] args) {
        String code = "animate({ duration: 1000 });";
        
        CHTLJSLexer lexer = new CHTLJSLexer(code);
        List<CHTLJSToken> tokens = lexer.tokenize();
        
        System.out.println("解析代码: " + code);
        System.out.println("Token数量: " + tokens.size());
        System.out.println("\nTokens:");
        for (int i = 0; i < tokens.size(); i++) {
            CHTLJSToken token = tokens.get(i);
            System.out.println(i + ": " + token.getType() + " = '" + token.getValue() + "'");
        }
    }
}