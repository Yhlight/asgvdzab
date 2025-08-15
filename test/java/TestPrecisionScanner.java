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
        
        // 测试7: 超细致切割 - 混合代码
        testCase7();
        
        // 测试8: 超细致切割 - 深度嵌套
        testCase8();
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
    }
    
    private static void testCase7() {
        System.out.println("\n测试7: 超细致切割 - 混合代码");
        String input = "const data = {{#myDiv}}.dataset.value;\n" +
                      "{{button}}->animate({\n" +
                      "    duration: 500,\n" +
                      "    callback: function() {\n" +
                      "        const elem = {{.container}};\n" +
                      "        elem.innerHTML = printMylove({\n" +
                      "            url: data\n" +
                      "        });\n" +
                      "    }\n" +
                      "});\n" +
                      "console.log('done');";
        
        runScanner(input);
    }
    
    private static void testCase8() {
        System.out.println("\n测试8: 超细致切割 - 深度嵌套");
        String input = "{{outer}}->listen({\n" +
                      "    mouseover: function() {\n" +
                      "        {{inner}}->animate({\n" +
                      "            opacity: 1,\n" +
                      "            callback: function() {\n" +
                      "                const result = printMylove({\n" +
                      "                    url: {{img}}.src,\n" +
                      "                    mode: 'ASCII'\n" +
                      "                });\n" +
                      "                {{output}}.textContent = result;\n" +
                      "            }\n" +
                      "        });\n" +
                      "    }\n" +
                      "});";
        
        runScanner(input);
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
            
            // 显示内容，转义换行符以便查看
            String content = fragment.getContent();
            if (content.contains("\n")) {
                System.out.println(content.replace("\n", "\\n"));
            } else {
                System.out.println(content);
            }
            System.out.println("---");
        }
        
        System.out.println();
    }
}