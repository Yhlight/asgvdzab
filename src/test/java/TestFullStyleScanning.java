import com.chtl.scanner.PrecisionScanner;
import com.chtl.scanner.LanguageContextManager;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.List;

public class TestFullStyleScanning {
    
    public static void main(String[] args) {
        System.out.println("=== 完整样式语法扫描测试 ===\n");
        
        // 测试1: 全局样式中的完整CHTL语法
        testGlobalStyleWithFullCHTL();
        
        // 测试2: 混合全局和局部样式
        testMixedGlobalAndLocalStyles();
        
        // 测试3: 原始嵌入在样式中的使用
        testOriginInStyles();
    }
    
    private static void testGlobalStyleWithFullCHTL() {
        System.out.println("测试1: 全局样式中的完整CHTL语法");
        String code = """
            [Template] @Style DefaultTheme {
                color: rgba(0, 0, 0, 0.87);
                font-family: "Roboto", sans-serif;
            }
            
            [Template] @Var Colors {
                primary: "#007bff";
                secondary: "#6c757d";
            }
            
            style {
                body {
                    @Style DefaultTheme;  // 使用样式组模板
                    background: Colors(primary);  // 使用变量
                }
                
                .container {
                    max-width: 1200px;
                    color: Colors(secondary);
                }
                
                [Origin] @Style {
                    /* 原始CSS代码 */
                    @media (max-width: 768px) {
                        .container { padding: 0 15px; }
                    }
                }
            }
            """;
        
        printFragments(code);
    }
    
    private static void testMixedGlobalAndLocalStyles() {
        System.out.println("\n测试2: 混合全局和局部样式");
        String code = """
            style {
                .global-theme {
                    @Style BaseTheme;
                    color: ThemeColor(text);
                }
            }
            
            div {
                style {
                    // 局部样式
                    @Style CardStyle;  // 也可以使用样式组
                    background: ThemeColor(cardBg);
                    
                    &:hover {  // 局部样式特有
                        transform: translateY(-2px);
                    }
                    
                    .inner {  // 自动添加类名
                        padding: 20px;
                    }
                }
            }
            """;
        
        printFragments(code);
    }
    
    private static void testOriginInStyles() {
        System.out.println("\n测试3: 原始嵌入在样式中的使用");
        String code = """
            style {
                .wrapper {
                    display: flex;
                    gap: 20px;
                }
                
                [Origin] @Style responsive {
                    @media (max-width: 768px) {
                        .wrapper {
                            flex-direction: column;
                        }
                    }
                }
                
                // 使用命名的原始嵌入
                [Origin] @Style responsive;
                
                .card {
                    @Style CardDefaults;
                    border: 1px solid Colors(border);
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
            
            // 限制显示长度
            if (content.length() > 80) {
                content = content.substring(0, 77) + "...";
            }
            
            System.out.printf("片段%d [%s]: \"%s\"\n", 
                i + 1, type, content);
        }
        
        // 统计
        int chtlCount = 0, cssCount = 0;
        for (CodeFragment fragment : fragments) {
            if (fragment.getType() == FragmentType.CHTL) chtlCount++;
            else if (fragment.getType() == FragmentType.CSS) cssCount++;
        }
        
        System.out.printf("\n统计: CHTL片段=%d, CSS片段=%d\n", chtlCount, cssCount);
    }
}