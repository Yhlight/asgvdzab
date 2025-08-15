import com.chtl.scanner.PrecisionScanner;
import com.chtl.scanner.LanguageContextManager;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.List;

public class TestEnhancedScanning {
    
    public static void main(String[] args) {
        System.out.println("=== 增强扫描器测试 ===\n");
        
        // 测试1: 局部style中的变量函数调用
        testLocalStyleVariables();
        
        // 测试2: CHTL JS精确切割
        testCHTLJSPrecision();
        
        // 测试3: 混合代码的最小单元切割
        testMinimalUnitCutting();
        
        // 测试4: 全局style保持纯CSS
        testGlobalStylePureCSS();
    }
    
    private static void testLocalStyleVariables() {
        System.out.println("测试1: 局部style中的变量函数调用");
        String code = """
            div {
                style {
                    color: ThemeColor(primary);
                    background: @Var ThemeColor.secondary;
                    font-size: 14px;
                }
            }
            """;
        
        printFragments(code);
    }
    
    private static void testCHTLJSPrecision() {
        System.out.println("\n测试2: CHTL JS精确切割");
        String code = """
            script {
                {{.button}}.listen({
                    click: function() {
                        console.log('clicked');
                    },
                    hover: function() {
                        {{.tooltip}}.show();
                    }
                });
                
                animate({
                    target: {{.box}},
                    duration: 500,
                    easing: ease-in-out
                });
            }
            """;
        
        printFragments(code);
    }
    
    private static void testMinimalUnitCutting() {
        System.out.println("\n测试3: 混合代码的最小单元切割");
        String code = """
            div {
                {{&}}.listen({
                    click: function() {
                        const data = getData();
                        {{.result}}->html(data);
                    }
                });
                
                style {
                    &:hover {
                        color: ThemeColor(hover);
                    }
                }
            }
            """;
        
        printFragments(code);
    }
    
    private static void testGlobalStylePureCSS() {
        System.out.println("\n测试4: 全局style保持纯CSS");
        String code = """
            style {
                .container {
                    width: 100%;
                    max-width: 1200px;
                }
                
                /* 不应该识别为CHTL语法 */
                .theme {
                    color: #333;
                    background: white;
                }
            }
            """;
        
        printFragments(code);
    }
    
    private static void printFragments(String code) {
        LanguageContextManager contextManager = new LanguageContextManager();
        PrecisionScanner scanner = new PrecisionScanner(contextManager);
        List<CodeFragment> fragments = scanner.scan(code);
        
        System.out.println("输入代码:");
        System.out.println(code);
        System.out.println("\n扫描结果:");
        
        for (int i = 0; i < fragments.size(); i++) {
            CodeFragment fragment = fragments.get(i);
            String content = fragment.getContent();
            String type = fragment.getType().getDisplayName();
            
            // 去除换行符以便显示
            content = content.replace("\n", "\\n").replace("\r", "\\r");
            
            System.out.printf("片段%d [%s]: \"%s\"\n", 
                i + 1, type, content);
        }
        
        System.out.println();
    }
}