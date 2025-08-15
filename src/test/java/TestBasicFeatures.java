import com.chtl.compiler.chtljs.lexer.CHTLJSLexer;
import com.chtl.compiler.chtljs.token.CHTLJSToken;
import com.chtl.compiler.chtljs.token.CHTLJSTokenType;
import com.chtl.compiler.chtl.lexer.CHTLLexer;
import com.chtl.compiler.chtl.token.CHTLToken;
import com.chtl.compiler.chtl.token.CHTLTokenType;
import java.util.List;

/**
 * 基础功能测试
 */
public class TestBasicFeatures {
    
    public static void main(String[] args) {
        TestBasicFeatures test = new TestBasicFeatures();
        
        System.out.println("=== 基础功能测试 ===\n");
        
        // 测试->操作符词法分析
        test.testArrowOperatorLexing();
        
        // 测试[Custom]词法分析
        test.testCustomLexing();
        
        // 测试[Configuration]词法分析
        test.testConfigurationLexing();
        
        // 测试[Origin]词法分析  
        test.testOriginLexing();
        
        System.out.println("\n=== 测试完成 ===");
    }
    
    /**
     * 测试->操作符词法分析
     */
    private void testArrowOperatorLexing() {
        System.out.println("1. 测试->操作符词法分析");
        
        String code = "element->method()";
        
        try {
            CHTLJSLexer lexer = new CHTLJSLexer(code);
            CHTLJSToken token;
            boolean foundArrow = false;
            
            while ((token = lexer.scanToken()) != null && 
                   token.getType() != CHTLJSTokenType.EOF) {
                if (token.getType() == CHTLJSTokenType.ARROW) {
                    foundArrow = true;
                    System.out.println("  ✓ 找到->操作符: " + token.getValue());
                }
            }
            
            if (!foundArrow) {
                System.out.println("  ✗ 未找到->操作符");
            }
            
        } catch (Exception e) {
            System.out.println("  ✗ 错误: " + e.getMessage());
        }
        
        System.out.println();
    }
    
    /**
     * 测试[Custom]词法分析
     */
    private void testCustomLexing() {
        System.out.println("2. 测试[Custom]词法分析");
        
        String code = "[Custom] @Style RedText { color: red; }";
        
        try {
            CHTLLexer lexer = new CHTLLexer(code);
            CHTLToken token;
            boolean foundCustom = false;
            boolean foundDelete = false;
            boolean foundInsert = false;
            
            // 测试delete/insert关键字
            String code2 = "delete 1; insert 2 { div {} };";
            CHTLLexer lexer2 = new CHTLLexer(code2);
            
            while ((token = lexer.scanToken()) != null && token.getType() != CHTLTokenType.EOF) {
                if (token.getType() == CHTLTokenType.CUSTOM) {
                    foundCustom = true;
                    System.out.println("  ✓ 找到[Custom]关键字");
                }
            }
            
            while ((token = lexer2.scanToken()) != null && token.getType() != CHTLTokenType.EOF) {
                if (token.getType() == CHTLTokenType.DELETE) {
                    foundDelete = true;
                    System.out.println("  ✓ 找到delete关键字");
                }
                if (token.getType() == CHTLTokenType.INSERT) {
                    foundInsert = true;
                    System.out.println("  ✓ 找到insert关键字");
                }
            }
            
            if (!foundCustom) {
                System.out.println("  ✗ 未找到[Custom]关键字");
            }
            
        } catch (Exception e) {
            System.out.println("  ✗ 错误: " + e.getMessage());
            e.printStackTrace();
        }
        
        System.out.println();
    }
    
    /**
     * 测试[Configuration]词法分析
     */
    private void testConfigurationLexing() {
        System.out.println("3. 测试[Configuration]词法分析");
        
        String code = "[Configuration] { DEBUG_MODE = true; }";
        
        try {
            CHTLLexer lexer = new CHTLLexer(code);
            CHTLToken token;
            boolean foundConfig = false;
            
            while ((token = lexer.scanToken()) != null && token.getType() != CHTLTokenType.EOF) {
                if (token.getType() == CHTLTokenType.CONFIGURATION) {
                    foundConfig = true;
                    System.out.println("  ✓ 找到[Configuration]关键字");
                }
            }
            
            if (!foundConfig) {
                System.out.println("  ✗ 未找到[Configuration]关键字");
            }
            
        } catch (Exception e) {
            System.out.println("  ✗ 错误: " + e.getMessage());
        }
        
        System.out.println();
    }
    
    /**
     * 测试[Origin]词法分析
     */
    private void testOriginLexing() {
        System.out.println("4. 测试[Origin]词法分析");
        
        String code = "[Origin] @Html { <div>test</div> }";
        
        try {
            CHTLLexer lexer = new CHTLLexer(code);
            CHTLToken token;
            boolean foundOrigin = false;
            
            while ((token = lexer.scanToken()) != null && token.getType() != CHTLTokenType.EOF) {
                if (token.getType() == CHTLTokenType.ORIGIN) {
                    foundOrigin = true;
                    System.out.println("  ✓ 找到[Origin]关键字");
                }
            }
            
            if (!foundOrigin) {
                System.out.println("  ✗ 未找到[Origin]关键字");
            }
            
        } catch (Exception e) {
            System.out.println("  ✗ 错误: " + e.getMessage());
        }
        
        System.out.println();
    }
}