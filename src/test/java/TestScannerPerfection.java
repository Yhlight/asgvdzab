import com.chtl.scanner.v2.UnifiedScannerV2;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.List;

/**
 * 测试扫描器的完美性
 * - 最小单元切割
 * - 原始嵌入的普遍性
 * - 职权分明
 */
public class TestScannerPerfection {
    
    public static void main(String[] args) {
        System.out.println("=== 扫描器完美性测试 ===\n");
        
        testMinimalUnitCutting();
        testUniversalOriginEmbed();
        testComplexMixedCode();
    }
    
    private static void testMinimalUnitCutting() {
        System.out.println("测试1: 最小单元切割（CHTL JS）");
        String code = """
            script {
                // 测试精确切割
                let elem = {{.button}};
                elem.listen({
                    click: () => {{.modal}}->show()
                });
                
                // 混合代码
                let config = {
                    selector: "{{.box}}",
                    animation: animate({
                        target: {{&}},
                        duration: 500
                    })
                };
                
                // 箭头操作符
                {{.nav}}->addClass('active');
                document.querySelector('.test')->style.display = 'none';
            }
            """;
        
        runTest(code, "最小单元切割");
    }
    
    private static void testUniversalOriginEmbed() {
        System.out.println("\n测试2: 原始嵌入的普遍性");
        String code = """
            style {
                /* 在CSS中使用原始嵌入 */
                .custom {
                    color: Colors(primary);
                    [Origin] @Style {
                        /* 自定义CSS */
                        filter: progid:DXImageTransform.Microsoft.gradient();
                    }
                    background: ThemeColor(bg);
                }
            }
            
            script {
                // 在JS中使用原始嵌入
                let data = {{.data}};
                
                [Origin] @Script {
                    // 第三方代码
                    window.jQuery = window.$ = jQuery;
                }
                
                data->process();
            }
            
            div {
                /* CHTL元素中的原始嵌入 */
                [Origin] @HTML {
                    <custom-element></custom-element>
                }
                
                text "Hello";
            }
            """;
        
        runTest(code, "原始嵌入普遍性");
    }
    
    private static void testComplexMixedCode() {
        System.out.println("\n测试3: 复杂混合代码");
        String code = """
            [Template] @Style ComplexStyle {
                color: #333;
                [Origin] @Style {
                    -webkit-appearance: none;
                }
            }
            
            div {
                style {
                    @Style ComplexStyle;
                    &:hover {
                        transform: translateY(-2px);
                        color: Colors(hover);
                    }
                }
                
                script {
                    // 局部script - 完全CHTL语法
                    {{&}}.listen({
                        mouseenter: function() {
                            [Origin] @Script {
                                console.log('Legacy code');
                            }
                            {{&}}->animate({duration: 200});
                        }
                    });
                }
            }
            """;
        
        runTest(code, "复杂混合");
    }
    
    private static void runTest(String code, String testName) {
        UnifiedScannerV2 scanner = new UnifiedScannerV2();
        List<CodeFragment> fragments = scanner.scan(code);
        
        System.out.println("\n扫描结果片段:");
        
        // 显示所有片段，特别关注切割精度
        int fragCount = 0;
        for (CodeFragment fragment : fragments) {
            String content = fragment.getContent();
            
            // 显示有意义的片段
            if (!content.trim().isEmpty() && 
                (fragment.getType() == FragmentType.CHTL || 
                 fragment.getType() == FragmentType.CHTL_JS ||
                 content.contains("Origin") ||
                 content.contains("{{") ||
                 content.contains("->") ||
                 content.contains("Colors") ||
                 content.contains("animate"))) {
                
                fragCount++;
                System.out.printf("  #%d [%s]: %s\n",
                    fragCount,
                    fragment.getType().getDisplayName(),
                    formatContent(content, 60));
            }
        }
        
        // 统计
        int[] counts = new int[4];
        for (CodeFragment fragment : fragments) {
            switch (fragment.getType()) {
                case CHTL: counts[0]++; break;
                case CSS: counts[1]++; break;
                case JS: counts[2]++; break;
                case CHTL_JS: counts[3]++; break;
            }
        }
        
        System.out.printf("\n片段统计: CHTL=%d, CSS=%d, JS=%d, CHTL_JS=%d\n", 
            counts[0], counts[1], counts[2], counts[3]);
    }
    
    private static String formatContent(String content, int maxLen) {
        content = content.replace("\n", "\\n").replace("\r", "\\r").trim();
        if (content.length() > maxLen) {
            return content.substring(0, maxLen - 3) + "...";
        }
        return content;
    }
}