import com.chtl.scanner.v2.UnifiedScannerV2;
import com.chtl.scanner.v2.CHTLVariableRegistry;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.List;

public class TestCSSFunctionFix {
    
    public static void main(String[] args) {
        System.out.println("=== CSS函数识别修复测试 ===\n");
        
        // 测试变量注册表
        System.out.println("变量注册表测试:");
        System.out.println("translateY 是CHTL变量? " + CHTLVariableRegistry.isCHTLVariable("translateY"));
        System.out.println("Y 是CHTL变量? " + CHTLVariableRegistry.isCHTLVariable("Y"));
        System.out.println("Colors 是CHTL变量? " + CHTLVariableRegistry.isCHTLVariable("Colors"));
        System.out.println();
        
        // 测试CSS函数识别
        String code = """
            style {
                .test {
                    /* CSS函数应保持完整 */
                    transform: translateY(-10px) scale(1.1);
                    
                    /* CHTL变量应被识别 */
                    color: Colors(primary);
                    background: ThemeColor(background);
                    
                    /* 混合使用 */
                    background-image: linear-gradient(45deg, Colors(start), Colors(end));
                    filter: blur(5px);
                }
            }
            """;
        
        UnifiedScannerV2 scanner = new UnifiedScannerV2();
        List<CodeFragment> fragments = scanner.scan(code);
        
        System.out.println("扫描结果:");
        for (CodeFragment fragment : fragments) {
            String content = fragment.getContent().trim();
            if (content.isEmpty()) continue;
            
            // 显示所有片段以调试
            System.out.printf("[%s]: %s\n", 
                fragment.getType().getDisplayName(),
                content.replace("\n", " "));
            
            // 特别检查是否有被错误分割的translateY
            if (content.contains("Y(")) {
                System.out.println("  ^^^ 警告: 发现被错误识别的Y函数!");
            }
        }
    }
}