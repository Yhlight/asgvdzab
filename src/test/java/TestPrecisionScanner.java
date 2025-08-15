import com.chtl.scanner.PrecisionScanner;
import com.chtl.scanner.LanguageContextManager;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.List;

public class TestPrecisionScanner {
    
    public static void main(String[] args) {
        System.out.println("=== 测试精确扫描器 ===\n");
        
        // 测试1: {{selector}}.method() 切割
        testCase1();
        
        // 测试2: CHTL JS函数调用
        testCase2();
        
        // 测试3: function() { JS代码 }
        testCase3();
        
        // 测试4: 复杂嵌套
        testCase4();
        
        // 测试5: 局部脚本块
        testCase5();
        
        // 测试6: 最小单元切割示例
        testCase6();
    }
    
    private static void testCase1() {
        System.out.println("测试1: {{selector}}.method() 切割");
        String input = "{{box}}.addEventListener('click', () => {\n" +
                      "    console.log('Box clicked!');\n" +
                      "});";
        
        runScanner(input);
    }
    
    private static void testCase2() {
        System.out.println("\n测试2: CHTL JS函数调用");
        String input = "const str = printMylove({\n" +
                      "    url: 'image.jpg',\n" +
                      "    mode: 'ASCII',\n" +
                      "    width: 80\n" +
                      "});";
        
        runScanner(input);
    }
    
    private static void testCase3() {
        System.out.println("\n测试3: function() { JS代码 }");
        String input = "{{button}}->listen({\n" +
                      "    click: function() {\n" +
                      "        alert('Hello');\n" +
                      "        const x = 10;\n" +
                      "    }\n" +
                      "});";
        
        runScanner(input);
    }
    
    private static void testCase4() {
        System.out.println("\n测试4: 复杂嵌套");
        String input = "{{container}}->animate({\n" +
                      "    duration: 1000,\n" +
                      "    easing: ease-in-out,\n" +
                      "    callback: function() {\n" +
                      "        {{child}}.style.display = 'none';\n" +
                      "    }\n" +
                      "});";
        
        runScanner(input);
    }
    
    private static void testCase5() {
        System.out.println("\n测试5: 局部脚本块");
        String input = "div {\n" +
                      "    script {\n" +
                      "        {{&}}->listen({\n" +
                      "            click: function() {\n" +
                      "                console.log('Clicked!');\n" +
                      "            }\n" +
                      "        });\n" +
                      "        \n" +
                      "        const result = printMylove({\n" +
                      "            url: 'chtholly.jpg'\n" +
                      "        });\n" +
                      "    }\n" +
                      "}";
        
        runScanner(input);
    }
    
    private static void testCase6() {
        System.out.println("\n测试6: 最小单元切割示例");
        String input = "// 根据您的示例\n" +
                      "{{box}}.addEventListener('click', () => {\n" +
                      "    console.log('Box clicked!');\n" +
                      "});\n" +
                      "\n" +
                      "const str = printMylove({\n" +
                      "    url: 'chtholly.jpg',\n" +
                      "    mode: 'Pixel'\n" +
                      "});";
        
        runScanner(input);
        
        // 期望输出：
        // 片段1 [CHTL]: // 根据您的示例\n
        // 片段2 [CHTL_JS]: {{box}}
        // 片段3 [CHTL或JS]: .addEventListener('click', () => {\n    console.log('Box clicked!');\n});\n\n
        // 片段4 [CHTL或JS]: const str = 
        // 片段5 [CHTL_JS]: printMylove({\n    url: 'chtholly.jpg',\n    mode: 'Pixel'\n});
    }
    
    private static void runScanner(String input) {
        LanguageContextManager contextManager = new LanguageContextManager();
        PrecisionScanner scanner = new PrecisionScanner(contextManager);
        
        List<CodeFragment> fragments = scanner.scan(input);
        
        System.out.println("输入代码:");
        System.out.println("----------");
        System.out.println(input);
        System.out.println("\n扫描结果:");
        System.out.println("----------");
        
        for (int i = 0; i < fragments.size(); i++) {
            CodeFragment fragment = fragments.get(i);
            System.out.printf("片段 %d [%s]:\n", i + 1, fragment.getType());
            System.out.println(fragment.getContent());
            System.out.println("---");
        }
        
        System.out.println();
    }
}