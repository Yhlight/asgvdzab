import com.chtl.scanner.v2.UnifiedScannerV2;
import com.chtl.scanner.v2.CHTLVariableRegistry;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.List;

public class TestScannerV2Enhanced {
    
    public static void main(String[] args) {
        System.out.println("=== 扫描器 V2 增强测试 ===\n");
        
        // 测试1: CSS函数识别改进
        testCSSFunctionDetection();
        
        // 测试2: Script块中的CHTL JS语法
        testScriptCHTLJS();
        
        // 测试3: 变量注册表功能
        testVariableRegistry();
    }
    
    private static void testCSSFunctionDetection() {
        System.out.println("测试1: CSS函数识别改进");
        String code = """
            [Template] @Var Colors {
                primary: "#007bff";
                secondary: "#6c757d";
            }
            
            style {
                .animated {
                    transform: translateY(-10px) scale(1.1) rotateZ(45deg);
                    background: linear-gradient(45deg, Colors(primary), Colors(secondary));
                    filter: blur(5px) brightness(1.2);
                    transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
                }
            }
            """;
        
        runTest(code);
    }
    
    private static void testScriptCHTLJS() {
        System.out.println("\n测试2: Script块中的CHTL JS语法");
        String code = """
            script {
                // CHTL JS语法应该被识别
                {{.button}}.listen({
                    click: function() {
                        console.log('Button clicked');
                        {{.modal}}->show();
                    },
                    hover: function() {
                        animate({
                            target: {{&}},
                            duration: 300
                        });
                    }
                });
                
                // 普通JS代码
                const config = {
                    theme: 'dark',
                    colors: ['red', 'blue']
                };
                
                // CHTL变量名在字符串中不应被识别
                console.log('Using Colors(primary) theme');
            }
            """;
        
        runTest(code);
    }
    
    private static void testVariableRegistry() {
        System.out.println("\n测试3: 变量注册表功能");
        
        // 测试注册表
        CHTLVariableRegistry.clear();
        System.out.println("CSS函数检测:");
        System.out.println("  translateY: " + CHTLVariableRegistry.isCHTLVariable("translateY"));
        System.out.println("  Colors: " + CHTLVariableRegistry.isCHTLVariable("Colors"));
        System.out.println("  ThemeColor: " + CHTLVariableRegistry.isCHTLVariable("ThemeColor"));
        
        String code = """
            [Template] @Var CustomColors {
                brand: "#ff6b6b";
            }
            
            style {
                .brand {
                    color: CustomColors(brand);
                    transform: translateX(10px);
                }
            }
            """;
        
        runTest(code);
    }
    
    private static void runTest(String code) {
        UnifiedScannerV2 scanner = new UnifiedScannerV2();
        List<CodeFragment> fragments = scanner.scan(code);
        
        System.out.println("\n扫描结果:");
        int chtlCount = 0, cssCount = 0, jsCount = 0, chtljsCount = 0;
        
        for (CodeFragment fragment : fragments) {
            String content = fragment.getContent()
                .replace("\n", "\\n")
                .replace("\r", "\\r");
                
            if (content.length() > 50) {
                content = content.substring(0, 47) + "...";
            }
            
            // 只显示关键片段
            if (fragment.getType() == FragmentType.CHTL || 
                fragment.getType() == FragmentType.CHTL_JS ||
                content.contains("translate") || 
                content.contains("Colors")) {
                System.out.printf("  [%s]: %s\n", 
                    fragment.getType().getDisplayName(), 
                    content
                );
            }
            
            // 统计
            switch (fragment.getType()) {
                case CHTL: chtlCount++; break;
                case CSS: cssCount++; break;
                case JS: jsCount++; break;
                case CHTL_JS: chtljsCount++; break;
            }
        }
        
        System.out.println("\n统计:");
        System.out.printf("  CHTL: %d, CSS: %d, JS: %d, CHTL_JS: %d\n", 
            chtlCount, cssCount, jsCount, chtljsCount);
    }
}