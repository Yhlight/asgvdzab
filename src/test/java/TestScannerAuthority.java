import com.chtl.scanner.v2.UnifiedScannerV2;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.List;

/**
 * 测试扫描器的职权分明设计
 * - CHTL语法：只用于CHTL和CSS
 * - CHTL JS语法：只用于JS和CHTL JS
 */
public class TestScannerAuthority {
    
    public static void main(String[] args) {
        System.out.println("=== 扫描器职权分明测试 ===\n");
        
        testGlobalStyle();
        testLocalStyle();
        testGlobalScript();
        testLocalScript();
    }
    
    private static void testGlobalStyle() {
        System.out.println("测试1: 全局style块（应支持CHTL语法）");
        String code = """
            style {
                /* CHTL变量引用 */
                .theme {
                    color: ThemeColor(primary);
                    background: Colors(background);
                }
                
                /* 样式组模板 */
                @Style DefaultButton;
                
                /* 原始嵌入 */
                [Origin] @Style {
                    /* 自定义CSS */
                    @keyframes slide {
                        from { transform: translateX(-100%); }
                        to { transform: translateX(0); }
                    }
                }
            }
            """;
        
        runTest(code, "全局style");
    }
    
    private static void testLocalStyle() {
        System.out.println("\n测试2: 局部style块（应支持CHTL语法+局部特性）");
        String code = """
            div {
                style {
                    /* CHTL变量 */
                    background: Colors(primary);
                    
                    /* 局部样式特性 & */
                    &:hover {
                        transform: scale(1.1);
                    }
                    
                    /* 样式组 */
                    @Style CardStyle;
                }
            }
            """;
        
        runTest(code, "局部style");
    }
    
    private static void testGlobalScript() {
        System.out.println("\n测试3: 全局script块（支持CHTL JS + 原始嵌入）");
        String code = """
            script {
                // CHTL JS语法
                {{.button}}.listen({
                    click: function() {
                        {{.modal}}->show();
                    }
                });
                
                animate({
                    target: {{.box}},
                    duration: 500
                });
                
                // 原始嵌入
                [Origin] @Script {
                    // 第三方库代码
                    window.customLibrary = {
                        init: function() {
                            console.log('Custom library initialized');
                        }
                    };
                }
                
                // 不应该支持CHTL变量
                // 下面这行应该作为普通JS处理
                var color = "Colors(primary)";
            }
            """;
        
        runTest(code, "全局script");
    }
    
    private static void testLocalScript() {
        System.out.println("\n测试4: 局部script块（作为CHTL的一部分）");
        String code = """
            button {
                script {
                    {{&}}.listen({
                        click: function() {
                            console.log('Clicked!');
                        }
                    });
                }
            }
            """;
        
        runTest(code, "局部script");
    }
    
    private static void runTest(String code, String testName) {
        UnifiedScannerV2 scanner = new UnifiedScannerV2();
        List<CodeFragment> fragments = scanner.scan(code);
        
        System.out.println("\n片段类型统计:");
        int chtl = 0, css = 0, js = 0, chtljs = 0;
        
        for (CodeFragment fragment : fragments) {
            switch (fragment.getType()) {
                case CHTL: chtl++; break;
                case CSS: css++; break;
                case JS: js++; break;
                case CHTL_JS: chtljs++; break;
            }
            
            // 显示关键片段
            String content = fragment.getContent().trim();
            if (content.contains("Colors") || 
                content.contains("Theme") ||
                content.contains("@Style") ||
                content.contains("[Origin]") ||
                content.contains("{{") ||
                content.contains("->") ||
                content.contains("animate") ||
                content.contains("listen")) {
                
                System.out.printf("  [%s]: %s\n",
                    fragment.getType().getDisplayName(),
                    content.replace("\n", " ").substring(0, Math.min(50, content.length())));
            }
        }
        
        System.out.printf("  总计: CHTL=%d, CSS=%d, JS=%d, CHTL_JS=%d\n", 
            chtl, css, js, chtljs);
    }
}