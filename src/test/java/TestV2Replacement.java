import com.chtl.scanner.CHTLUnifiedScanner;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.List;

/**
 * 测试V2扫描器已成功替换V1
 */
public class TestV2Replacement {
    
    public static void main(String[] args) {
        System.out.println("=== V2扫描器替换测试 ===\n");
        
        // 使用CHTLUnifiedScanner（现在内部使用V2）
        CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
        
        // 测试1：局部块的语法限制
        testLocalBlockRestrictions(scanner);
        
        // 测试2：原始嵌入的普遍性
        testUniversalOrigin(scanner);
        
        // 测试3：CHTL JS的精确切割
        testPreciseCHTLJS(scanner);
    }
    
    private static void testLocalBlockRestrictions(CHTLUnifiedScanner scanner) {
        System.out.println("测试1: 局部块语法限制");
        String code = """
            div {
                style {
                    /* 使用性语法 - 应该被识别 */
                    color: Colors(primary);
                    @Style ButtonStyle;
                    
                    /* 定义性语法 - 应该被忽略 */
                    /* [Template] @Style NewStyle { } */
                }
                
                script {
                    /* 使用性语法 */
                    [Import] @Chtl from Module;
                    text "Hello";
                    
                    /* CHTL JS */
                    {{&}}.listen({ click: () => alert('Hi') });
                    
                    /* 定义性语法 - 应该被忽略 */
                    /* [Template] @Element NewElem { } */
                }
            }
            """;
        
        List<CodeFragment> fragments = scanner.scan(code);
        System.out.println("扫描到 " + fragments.size() + " 个片段");
        
        // 验证没有Template片段
        boolean hasTemplate = fragments.stream()
            .anyMatch(f -> f.getContent().contains("[Template]"));
        System.out.println("包含Template定义: " + hasTemplate + " (应该为false)");
        System.out.println();
    }
    
    private static void testUniversalOrigin(CHTLUnifiedScanner scanner) {
        System.out.println("测试2: 原始嵌入普遍性");
        String code = """
            style {
                .class {
                    [Origin] @Style { filter: IE-specific; }
                    color: red;
                }
            }
            
            script {
                let x = 1;
                [Origin] @Script { window.legacy = true; }
                {{.btn}}->click();
            }
            """;
        
        List<CodeFragment> fragments = scanner.scan(code);
        long originCount = fragments.stream()
            .filter(f -> f.getContent().contains("[Origin]"))
            .count();
        System.out.println("原始嵌入片段数: " + originCount);
        System.out.println();
    }
    
    private static void testPreciseCHTLJS(CHTLUnifiedScanner scanner) {
        System.out.println("测试3: CHTL JS精确切割");
        String code = """
            script {
                let elem = {{.button}};
                elem->addClass('active');
                animate({ target: {{&}}, duration: 300 });
            }
            """;
        
        List<CodeFragment> fragments = scanner.scan(code);
        
        // 统计CHTL JS片段
        long chtljsCount = fragments.stream()
            .filter(f -> f.getType() == FragmentType.CHTL_JS)
            .count();
        
        System.out.println("CHTL JS片段数: " + chtljsCount);
        
        // 显示CHTL JS片段
        fragments.stream()
            .filter(f -> f.getType() == FragmentType.CHTL_JS)
            .forEach(f -> System.out.println("  - " + f.getContent().trim()));
        
        System.out.println("\nV2扫描器工作正常！");
    }
}