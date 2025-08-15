import com.chtl.compiler.chtl.lexer.CHTLLexer;
import com.chtl.compiler.chtl.token.CHTLToken;
import com.chtl.compiler.chtljs.lexer.CHTLJSLexer;
import com.chtl.compiler.chtljs.token.CHTLJSToken;

import java.util.List;

public class TestLexers {
    public static void main(String[] args) {
        System.out.println("=== 测试CHTL词法分析器 ===");
        testCHTLLexer();
        
        System.out.println("\n=== 测试CHTL JS词法分析器 ===");
        testCHTLJSLexer();
    }
    
    private static void testCHTLLexer() {
        // 测试基本CHTL代码
        String chtlCode = """
            div {
                id: myDiv;
                class: container;
                
                style {
                    width: 100px;
                    .box {
                        color: red;
                    }
                    &:hover {
                        background: blue;
                    }
                }
                
                text {
                    Hello CHTL!
                }
                
                script {
                    console.log("test");
                }
            }
            """;
        
        CHTLLexer lexer = new CHTLLexer(chtlCode);
        List<CHTLToken> tokens = lexer.tokenize();
        
        System.out.println("Token数量: " + tokens.size());
        System.out.println("\n前20个Token:");
        for (int i = 0; i < Math.min(20, tokens.size()); i++) {
            CHTLToken token = tokens.get(i);
            System.out.println(String.format("%3d: %-20s '%s'", 
                i, token.getType(), token.getValue()));
        }
        
        // 测试特殊语法
        System.out.println("\n测试CE对等式:");
        String ceCode = "id: test1; class= test2;";
        lexer = new CHTLLexer(ceCode);
        tokens = lexer.tokenize();
        for (CHTLToken token : tokens) {
            if (token.getType().name().equals("COLON") || 
                token.getType().name().equals("EQUALS")) {
                System.out.println("找到: " + token.getType());
            }
        }
    }
    
    private static void testCHTLJSLexer() {
        // 测试CHTL JS代码
        String chtljsCode = """
            {{#myButton}}->listen({
                click: () => {
                    console.log('Button clicked');
                    {{.box}}->style.color = 'red';
                }
            });
            
            {{document.body}}->delegate({
                target: {{.btn}},
                click: handleClick
            });
            
            animate({
                duration: 1000,
                easing: 'ease-in-out',
                begin: { opacity: 0 },
                end: { opacity: 1 }
            });
            """;
        
        CHTLJSLexer lexer = new CHTLJSLexer(chtljsCode);
        List<CHTLJSToken> tokens = lexer.tokenize();
        
        System.out.println("Token数量: " + tokens.size());
        System.out.println("\n前30个Token:");
        for (int i = 0; i < Math.min(30, tokens.size()); i++) {
            CHTLJSToken token = tokens.get(i);
            System.out.println(String.format("%3d: %-25s '%s'", 
                i, token.getType(), token.getValue()));
        }
        
        // 统计特殊Token
        System.out.println("\n特殊Token统计:");
        int selectorCount = 0;
        int arrowCount = 0;
        int listenCount = 0;
        int delegateCount = 0;
        int animateCount = 0;
        
        for (CHTLJSToken token : tokens) {
            switch (token.getType().name()) {
                case "DOUBLE_LEFT_BRACE":
                    selectorCount++;
                    break;
                case "ARROW":
                    arrowCount++;
                    break;
                case "LISTEN":
                    listenCount++;
                    break;
                case "DELEGATE":
                    delegateCount++;
                    break;
                case "ANIMATE":
                    animateCount++;
                    break;
            }
        }
        
        System.out.println("增强选择器: " + selectorCount);
        System.out.println("箭头操作符: " + arrowCount);
        System.out.println("listen方法: " + listenCount);
        System.out.println("delegate方法: " + delegateCount);
        System.out.println("animate函数: " + animateCount);
    }
}