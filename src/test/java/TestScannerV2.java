import com.chtl.scanner.v2.UnifiedScannerV2;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.List;

public class TestScannerV2 {
    
    public static void main(String[] args) {
        System.out.println("=== 扫描器 V2 测试 ===\n");
        
        // 测试1: Style块中的CHTL语法
        testStyleBlock();
        
        // 测试2: Script块不识别CHTL语法
        testScriptBlock();
        
        // 测试3: 混合代码
        testMixedCode();
        
        // 测试4: CSS函数正确识别
        testCSSFunctions();
    }
    
    private static void testStyleBlock() {
        System.out.println("测试1: Style块中的CHTL语法");
        String code = """
            style {
                body {
                    @Style DefaultTheme;
                    background: Colors(primary);
                    color: #333;
                }
                
                [Origin] @Style {
                    @media (max-width: 768px) {
                        body { padding: 0; }
                    }
                }
            }
            """;
        
        runTest(code);
    }
    
    private static void testScriptBlock() {
        System.out.println("\n测试2: Script块不识别CHTL语法");
        String code = """
            script {
                // 不应该识别 Colors(primary) 为CHTL语法
                const theme = {
                    background: 'Colors(primary)',
                    getColor: function() {
                        return ThemeColor(primary);
                    }
                };
                
                // @Style 也不应该被识别
                console.log('@Style DefaultTheme');
            }
            """;
        
        runTest(code);
    }
    
    private static void testMixedCode() {
        System.out.println("\n测试3: 混合代码");
        String code = """
            [Template] @Style CardStyle {
                border-radius: 8px;
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            }
            
            div {
                style {
                    @Style CardStyle;
                    background: ThemeColor(cardBg);
                    
                    &:hover {
                        transform: scale(1.02);
                    }
                }
                
                script {
                    {{.card}}.listen({
                        click: function() {
                            console.log('Card clicked');
                        }
                    });
                }
            }
            """;
        
        runTest(code);
    }
    
    private static void testCSSFunctions() {
        System.out.println("\n测试4: CSS函数正确识别");
        String code = """
            style {
                .animated {
                    transform: translateY(-10px) scale(1.1);
                    background: linear-gradient(45deg, Colors(start), Colors(end));
                    animation: fadeIn 0.3s ease-in-out;
                }
            }
            """;
        
        runTest(code);
    }
    
    private static void runTest(String code) {
        UnifiedScannerV2 scanner = new UnifiedScannerV2();
        List<CodeFragment> fragments = scanner.scan(code);
        
        System.out.println("扫描结果:");
        for (int i = 0; i < fragments.size(); i++) {
            CodeFragment fragment = fragments.get(i);
            String content = fragment.getContent()
                .replace("\n", "\\n")
                .replace("\r", "\\r");
                
            if (content.length() > 60) {
                content = content.substring(0, 57) + "...";
            }
            
            System.out.printf("片段%d [%s]: %s\n", 
                i + 1, 
                fragment.getType().getDisplayName(), 
                content
            );
        }
        
        // 统计
        System.out.println("\n统计:");
        System.out.printf("  总片段数: %d\n", fragments.size());
        System.out.printf("  CHTL片段: %d\n", 
            fragments.stream().filter(f -> f.getType() == FragmentType.CHTL).count());
        System.out.printf("  CSS片段: %d\n", 
            fragments.stream().filter(f -> f.getType() == FragmentType.CSS).count());
        System.out.printf("  JS片段: %d\n", 
            fragments.stream().filter(f -> f.getType() == FragmentType.JS).count());
        System.out.printf("  CHTL_JS片段: %d\n", 
            fragments.stream().filter(f -> f.getType() == FragmentType.CHTL_JS).count());
    }
}