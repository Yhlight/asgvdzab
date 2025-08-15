import com.chtl.scanner.CHTLUnifiedScanner;
import com.chtl.scanner.v2.UnifiedScannerV2;
import com.chtl.model.CodeFragment;
import java.util.List;

public class ScannerPerformanceTest {
    
    public static void main(String[] args) {
        System.out.println("=== 扫描器性能对比测试 ===\n");
        
        // 构建测试代码
        String testCode = buildTestCode();
        System.out.printf("测试代码大小: %d 字符\n\n", testCode.length());
        
        // 预热
        warmup(testCode);
        
        // 测试V1扫描器
        long v1Time = testV1Scanner(testCode);
        
        // 测试V2扫描器
        long v2Time = testV2Scanner(testCode);
        
        // 结果对比
        System.out.println("\n=== 性能对比结果 ===");
        System.out.printf("V1 扫描器: %d ms\n", v1Time);
        System.out.printf("V2 扫描器: %d ms\n", v2Time);
        System.out.printf("性能提升: %.2fx\n", (double)v1Time / v2Time);
    }
    
    private static String buildTestCode() {
        StringBuilder code = new StringBuilder();
        
        // 添加大量混合代码
        for (int i = 0; i < 50; i++) {
            code.append("""
                [Template] @Style Theme%d {
                    color: rgba(0, 0, 0, 0.87);
                    font-size: 14px;
                }
                
                div.component%d {
                    style {
                        @Style Theme%d;
                        background: Colors(bg%d);
                        padding: 20px;
                        
                        &:hover {
                            transform: scale(1.02);
                            box-shadow: 0 4px 8px rgba(0,0,0,0.1);
                        }
                    }
                    
                    h1 {
                        text { Component %d Title }
                    }
                    
                    script {
                        // This is pure JavaScript
                        const data = {
                            id: %d,
                            name: 'Component %d',
                            color: 'ThemeColor(primary)'
                        };
                        
                        function handleClick() {
                            console.log('Clicked component %d');
                        }
                    }
                }
                
                """.formatted(i, i, i, i, i, i, i, i));
        }
        
        return code.toString();
    }
    
    private static void warmup(String testCode) {
        System.out.println("预热中...");
        CHTLUnifiedScanner v1 = new CHTLUnifiedScanner();
        UnifiedScannerV2 v2 = new UnifiedScannerV2();
        
        for (int i = 0; i < 10; i++) {
            v1.scan(testCode);
            v2.scan(testCode);
        }
        System.out.println("预热完成\n");
    }
    
    private static long testV1Scanner(String testCode) {
        System.out.println("测试 V1 扫描器...");
        CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
        
        long start = System.currentTimeMillis();
        List<CodeFragment> fragments = null;
        
        for (int i = 0; i < 100; i++) {
            fragments = scanner.scan(testCode);
        }
        
        long end = System.currentTimeMillis();
        
        System.out.printf("V1 结果: %d 个片段\n", fragments.size());
        return end - start;
    }
    
    private static long testV2Scanner(String testCode) {
        System.out.println("测试 V2 扫描器...");
        UnifiedScannerV2 scanner = new UnifiedScannerV2();
        
        long start = System.currentTimeMillis();
        List<CodeFragment> fragments = null;
        
        for (int i = 0; i < 100; i++) {
            fragments = scanner.scan(testCode);
        }
        
        long end = System.currentTimeMillis();
        
        System.out.printf("V2 结果: %d 个片段\n", fragments.size());
        return end - start;
    }
}