import com.chtl.scanner.PrecisionScanner;
import com.chtl.scanner.LanguageContextManager;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.List;

public class TestCSSFunction {
    
    public static void main(String[] args) {
        System.out.println("=== CSS函数 vs CHTL变量函数测试 ===\n");
        
        String code = """
            style {
                .test {
                    transform: translateY(-2px);  // CSS函数
                    color: ThemeColor(primary);   // CHTL变量
                    border: 1px solid Colors(border);  // CHTL变量
                    background: rgb(255, 255, 255);  // CSS函数
                    margin: calc(10px + 5%);  // CSS函数
                }
            }
            """;
        
        LanguageContextManager contextManager = new LanguageContextManager();
        PrecisionScanner scanner = new PrecisionScanner(contextManager);
        List<CodeFragment> fragments = scanner.scan(code);
        
        System.out.println("扫描结果:");
        for (int i = 0; i < fragments.size(); i++) {
            CodeFragment fragment = fragments.get(i);
            String content = fragment.getContent().replace("\n", "\\n");
            if (content.length() > 60) {
                content = content.substring(0, 57) + "...";
            }
            System.out.printf("片段%d [%s]: %s\n", i + 1, fragment.getType().getDisplayName(), content);
        }
    }
}