import com.chtl.parser.CHTLJSParser;
import com.chtl.ast.chtljs.*;
import com.chtl.generator.CHTLJSGenerator;
import java.util.List;

/**
 * CHTL JS无修饰字面量测试
 */
public class TestCHTLJSUnquotedLiterals {
    
    public static void main(String[] args) {
        System.out.println("=== CHTL JS 无修饰字面量测试 ===\n");
        
        testEasingFunction();
        testAnimationDirection();
        testCSSValues();
        testCompleteAnimation();
        testInvalidUnquotedLiteral();
        
        System.out.println("\n=== 测试完成 ===");
    }
    
    /**
     * 测试缓动函数的无修饰字面量
     */
    private static void testEasingFunction() {
        System.out.println("1. 测试缓动函数:");
        
        String code = """
            animate({
                duration: 1000,
                easing: ease-in-out,
                begin: {
                    opacity: 0
                },
                end: {
                    opacity: 1
                }
            });
            """;
            
        testParse(code, "缓动函数无修饰字面量");
    }
    
    /**
     * 测试动画方向的无修饰字面量
     */
    private static void testAnimationDirection() {
        System.out.println("\n2. 测试动画方向:");
        
        String code = """
            animate({
                duration: 2000,
                direction: alternate-reverse,
                loop: -1
            });
            """;
            
        testParse(code, "动画方向无修饰字面量");
    }
    
    /**
     * 测试CSS值的无修饰字面量
     */
    private static void testCSSValues() {
        System.out.println("\n3. 测试CSS值:");
        
        String code = """
            animate({
                begin: {
                    display: none,
                    position: absolute,
                    opacity: 0
                },
                end: {
                    display: block,
                    opacity: 1
                }
            });
            """;
            
        testParse(code, "CSS值无修饰字面量");
    }
    
    /**
     * 测试完整的动画配置
     */
    private static void testCompleteAnimation() {
        System.out.println("\n4. 测试完整动画配置:");
        
        String code = """
            {{#box}}->listen({
                click: function() {
                    animate({
                        duration: 500,
                        easing: ease-out,
                        delay: 100,
                        direction: normal,
                        loop: 3,
                        
                        begin: {
                            transform: 'scale(1)',
                            opacity: 1,
                            display: block
                        },
                        
                        when: [
                            {
                                at: 0.5;
                                transform: 'scale(1.2)',
                                opacity: 0.7
                            },
                            {
                                at: 0.8;
                                transform: 'scale(0.9)'
                            }
                        ],
                        
                        end: {
                            transform: 'scale(1)',
                            opacity: 1
                        },
                        
                        callback: function() {
                            console.log('Animation completed');
                        }
                    });
                }
            });
            """;
            
        testParse(code, "完整动画配置");
    }
    
    /**
     * 测试无效的无修饰字面量
     */
    private static void testInvalidUnquotedLiteral() {
        System.out.println("\n5. 测试无效的无修饰字面量:");
        
        String code = """
            animate({
                easing: invalid-easing-function,
                direction: unknown-direction
            });
            """;
            
        testParse(code, "无效的无修饰字面量（应该报错）");
    }
    
    /**
     * 解析并测试代码
     */
    private static void testParse(String code, String testName) {
        System.out.println("测试: " + testName);
        System.out.println("代码:\n" + code);
        
        try {
            // 解析
            CHTLJSParser parser = new CHTLJSParser();
            List<CHTLJSASTNode> ast = parser.parse(code);
            
            // 检查错误
            List<String> errors = parser.getErrors();
            if (!errors.isEmpty()) {
                System.out.println("解析错误:");
                for (String error : errors) {
                    System.out.println("  - " + error);
                }
            }
            
            // 生成代码
            if (errors.isEmpty() && !ast.isEmpty()) {
                CHTLJSGenerator generator = new CHTLJSGenerator();
                String output = generator.generate(ast);
                System.out.println("生成的JavaScript:\n" + output);
            }
            
        } catch (Exception e) {
            System.out.println("异常: " + e.getMessage());
            e.printStackTrace();
        }
    }
    
    /**
     * 测试混合使用引号和无修饰字面量
     */
    private static void testMixedValues() {
        System.out.println("\n6. 测试混合使用:");
        
        String code = """
            animate({
                duration: 1000,
                easing: ease-in,              // 无修饰字面量
                begin: {
                    display: block,           // 无修饰字面量
                    background: '#ff0000',    // 字符串
                    width: '100px'           // 字符串
                }
            });
            """;
            
        testParse(code, "混合使用引号和无修饰字面量");
    }
}