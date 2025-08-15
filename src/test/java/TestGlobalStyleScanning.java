import com.chtl.scanner.PrecisionScanner;
import com.chtl.model.CodeFragment;
import java.util.List;

/**
 * 测试扫描器对全局style中CHTL代码的识别
 */
public class TestGlobalStyleScanning {
    
    public static void main(String[] args) {
        System.out.println("=== 测试全局style中CHTL代码识别 ===\n");
        
        testCase1();
        testCase2();
        testCase3();
        testCase4();
        
        System.out.println("\n=== 所有测试完成 ===");
    }
    
    /**
     * 测试1：全局style中使用@Var变量
     */
    private static void testCase1() {
        System.out.println("测试1：全局style中使用@Var变量");
        
        String code = """
            [Template] @Var Colors {
                primary: "#007bff";
                secondary: "#6c757d";
            }
            
            style {
                body {
                    background: @Var Colors.primary;
                    color: @Var Colors.secondary;
                }
            }
            """;
        
        scanAndPrint(code);
    }
    
    /**
     * 测试2：全局style中嵌套CHTL增强语法
     */
    private static void testCase2() {
        System.out.println("\n测试2：全局style中的选择器和变量");
        
        String code = """
            style {
                .container {
                    width: 100%;
                    max-width: 1200px;
                }
                
                /* 使用CHTL变量 */
                .theme-dark {
                    background: @Var DarkTheme.background;
                    color: @Var DarkTheme.text;
                }
            }
            """;
        
        scanAndPrint(code);
    }
    
    /**
     * 测试3：复杂的全局style结构
     */
    private static void testCase3() {
        System.out.println("\n测试3：复杂的全局style结构");
        
        String code = """
            style {
                /* 基础样式 */
                * {
                    box-sizing: border-box;
                }
                
                body {
                    margin: 0;
                    padding: 0;
                    font-family: @Var Fonts.primary;
                }
                
                /* 响应式设计 */
                @media (max-width: 768px) {
                    .container {
                        padding: @Var Spacing.mobile;
                    }
                }
                
                /* 动画 */
                @keyframes fadeIn {
                    from { opacity: 0; }
                    to { opacity: 1; }
                }
            }
            """;
        
        scanAndPrint(code);
    }
    
    /**
     * 测试4：[Origin] @Style中的内容
     */
    private static void testCase4() {
        System.out.println("\n测试4：[Origin] @Style原始CSS");
        
        String code = """
            [Origin] @Style {
                /* 这是原始CSS，不应该被解析为CHTL */
                .raw-style {
                    color: red;
                    /* @Var不应该被解析 */
                    content: "@Var test";
                }
            }
            
            style {
                /* 这是CHTL style块 */
                .chtl-style {
                    color: @Var Colors.primary;
                }
            }
            """;
        
        scanAndPrint(code);
    }
    
    private static void scanAndPrint(String code) {
        // 创建语言上下文管理器
        com.chtl.scanner.LanguageContextManager contextManager = 
            new com.chtl.scanner.LanguageContextManager();
        
        PrecisionScanner scanner = new PrecisionScanner(contextManager);
        List<CodeFragment> fragments = scanner.scan(code);
        
        System.out.println("扫描结果：");
        for (int i = 0; i < fragments.size(); i++) {
            CodeFragment fragment = fragments.get(i);
            System.out.printf("片段%d [%s]:\n", i + 1, fragment.getType());
            
            String content = fragment.getContent();
            if (content.length() > 100) {
                content = content.substring(0, 100) + "...";
            }
            
            // 高亮显示@Var使用
            content = content.replace("@Var", "【@Var】");
            
            System.out.println(content.trim());
            System.out.println("---");
        }
    }
}