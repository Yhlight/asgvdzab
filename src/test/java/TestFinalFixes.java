import com.chtl.scanner.*;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.List;

/**
 * 测试最终修复：边界问题和CSS函数识别
 */
public class TestFinalFixes {
    
    public static void main(String[] args) {
        System.out.println("=== 最终修复测试 ===\n");
        
        testScriptBoundaryFix();
        testCSSFunctionDetection();
        testDynamicBlockScanning();
    }
    
    private static void testScriptBoundaryFix() {
        System.out.println("测试1: Script块边界修复");
        CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
        
        // 测试各种边界情况
        String code = """
            script {
                // 测试函数调用边界
                element.listen({
                    click: () => console.log('test')
                });
                
                // 测试嵌套结构
                {{.container}}.delegate({
                    target: '.item',
                    click: function(e) {
                        animate({
                            target: this,
                            duration: 300
                        });
                    }
                });
                
                // 测试字符串中的特殊字符
                let template = `Use {{selector}} and -> operators`;
                
                // 测试行尾
                {{.button}}->addClass('active')
            }
            """;
        
        try {
            List<CodeFragment> fragments = scanner.scan(code);
            System.out.println("✓ 扫描成功，无边界错误");
            
            // 统计CHTL JS片段
            long chtljsCount = fragments.stream()
                .filter(f -> f.getType() == FragmentType.CHTL_JS)
                .count();
            System.out.println("  检测到 " + chtljsCount + " 个CHTL JS片段");
        } catch (Exception e) {
            System.out.println("✗ 边界错误: " + e.getMessage());
        }
    }
    
    private static void testCSSFunctionDetection() {
        System.out.println("\n测试2: CSS函数识别增强");
        
        // 测试CSS函数检测器
        CSSFunctionDetector detector = new CSSFunctionDetector();
        
        String[] testCases = {
            "translateY", "translateX", "scale3d", "rotateZ",
            "Colors", "ThemeColor", "MyVariable", "Y"
        };
        
        System.out.println("CSS函数检测:");
        for (String func : testCases) {
            boolean isCSS = detector.isCSSFunction(func);
            System.out.printf("  %s: %s\n", func, isCSS ? "CSS函数" : "CHTL变量");
        }
        
        // 测试在实际代码中的识别
        String styleCode = """
            style {
                .animated {
                    transform: translateY(-10px) scale(1.1);
                    color: Colors(primary);
                    background: linear-gradient(45deg, ThemeColor(start), ThemeColor(end));
                }
            }
            """;
        
        CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
        List<CodeFragment> fragments = scanner.scan(styleCode);
        
        System.out.println("\n样式代码扫描结果:");
        for (CodeFragment fragment : fragments) {
            if (fragment.getType() == FragmentType.CHTL || 
                fragment.getType() == FragmentType.CSS) {
                String content = fragment.getContent().trim();
                if (content.contains("translate") || content.contains("Colors")) {
                    System.out.printf("  [%s]: %s\n", 
                        fragment.getType().getDisplayName(),
                        content.length() > 50 ? content.substring(0, 50) + "..." : content);
                }
            }
        }
    }
    
    private static void testDynamicBlockScanning() {
        System.out.println("\n测试3: 动态块扫描");
        
        String complexCode = """
            script {
                // 大块纯JS代码
                const config = {
                    api: 'https://example.com',
                    timeout: 5000,
                    retries: 3
                };
                
                function processData(data) {
                    return data.map(item => ({
                        id: item.id,
                        name: item.name.toUpperCase()
                    }));
                }
                
                // CHTL JS密集区域
                {{.list}}.listen({
                    click: () => {{.modal}}->show()
                });
                
                {{.items}}.delegate({
                    target: '.item',
                    hover: function() {
                        {{&}}->animate({ opacity: 0.8 });
                    }
                });
            }
            """;
        
        DynamicBlockScanner blockScanner = new DynamicBlockScanner(complexCode);
        
        System.out.println("动态块扫描结果:");
        int blockCount = 0;
        DynamicBlockScanner.DynamicBlock block;
        
        while ((block = blockScanner.scanNext()) != null) {
            blockCount++;
            System.out.printf("  块%d [%s]: %d字符\n", 
                blockCount, 
                block.getType(),
                block.getContent().length());
        }
        
        System.out.println("  总块数: " + blockCount);
    }
}