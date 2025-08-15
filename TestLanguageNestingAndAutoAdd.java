import com.chtl.scanner.*;
import com.chtl.compiler.AutoAddManager;
import com.chtl.model.CodeFragment;
import java.util.*;

/**
 * 测试语言嵌套和自动添加功能
 */
public class TestLanguageNestingAndAutoAdd {
    
    public static void main(String[] args) {
        System.out.println("=== 语言嵌套和自动添加功能测试 ===\n");
        
        testCHTLJSInJS();
        testJSInCHTLJS();
        testAutoAddInLocalStyle();
        testAutoAddInLocalScript();
        testSelfReferenceSelector();
        testComplexNesting();
        
        System.out.println("\n=== 测试完成 ===");
    }
    
    /**
     * 测试JS中包含CHTL JS代码
     */
    private static void testCHTLJSInJS() {
        System.out.println("1. 测试JS中包含CHTL JS代码:");
        
        String code = """
            script {
                // CHTL JS代码
                {{#button}}->listen({
                    click: function() {
                        // 这里是纯JS，可以使用箭头函数
                        const handler = () => {
                            console.log('Clicked!');
                        };
                        handler();
                        
                        // 又回到CHTL JS
                        {{.container}}->animate({
                            duration: 500,
                            easing: ease-out
                        });
                    }
                });
            }
            """;
            
        scanAndPrint(code, "JS中包含CHTL JS");
    }
    
    /**
     * 测试CHTL JS中包含JS代码
     */
    private static void testJSInCHTLJS() {
        System.out.println("\n2. 测试CHTL JS中包含JS代码:");
        
        String code = """
            script {
                // 使用CHTL JS的listen方法
                {{button}}->listen({
                    click: function() {
                        // 纯JS代码，支持所有JS特性
                        const items = [1, 2, 3];
                        items.forEach(item => {
                            console.log(`Item: ${item}`);
                        });
                    },
                    
                    mouseenter: function(event) {
                        // 更多JS代码
                        this.style.background = 'blue';
                    }
                });
            }
            """;
            
        scanAndPrint(code, "CHTL JS中包含JS");
    }
    
    /**
     * 测试局部样式块的自动添加
     */
    private static void testAutoAddInLocalStyle() {
        System.out.println("\n3. 测试局部样式块的自动添加:");
        
        String code = """
            div {
                style {
                    .container {
                        width: 100%;
                    }
                    
                    #header {
                        height: 60px;
                    }
                }
                
                text {
                    这是一个容器
                }
            }
            """;
            
        scanAndPrint(code, "局部样式块自动添加");
    }
    
    /**
     * 测试局部脚本块的自动添加
     */
    private static void testAutoAddInLocalScript() {
        System.out.println("\n4. 测试局部脚本块的自动添加:");
        
        String code = """
            button {
                text { 点击我 }
                
                script {
                    // 使用{{&}}引用当前元素
                    {{&}}->listen({
                        click: function() {
                            console.log('Button clicked!');
                        }
                    });
                    
                    // 使用其他选择器
                    {{.active}}->listen({
                        click: function() {
                            console.log('Active element clicked!');
                        }
                    });
                }
            }
            """;
            
        scanAndPrint(code, "局部脚本块自动添加");
    }
    
    /**
     * 测试{{&}}自引用选择器
     */
    private static void testSelfReferenceSelector() {
        System.out.println("\n5. 测试{{&}}自引用选择器:");
        
        String code = """
            div {
                // 情况1：没有任何类或ID
                script {
                    {{&}}->listen({ // 应该自动生成ID
                        click: function() { }
                    });
                }
            }
            
            div class="container" {
                // 情况2：有显式类
                script {
                    {{&}}->listen({ // 应该使用.container
                        click: function() { }
                    });
                }
            }
            
            div id="main" {
                // 情况3：有显式ID
                script {
                    {{&}}->listen({ // 应该使用#main
                        click: function() { }
                    });
                }
            }
            
            div {
                style {
                    .box { /* 自动添加类 */ }
                }
                // 情况4：局部样式块已添加类
                script {
                    {{&}}->listen({ // 应该使用.box
                        click: function() { }
                    });
                }
            }
            """;
            
        scanAndPrint(code, "{{&}}自引用选择器");
    }
    
    /**
     * 测试复杂嵌套场景
     */
    private static void testComplexNesting() {
        System.out.println("\n6. 测试复杂嵌套场景:");
        
        String code = """
            div {
                style {
                    .container {
                        display: flex;
                    }
                    
                    #sidebar {
                        width: 200px;
                    }
                }
                
                script {
                    // CHTL JS上下文
                    {{&}}->listen({
                        click: function(event) {
                            // JS上下文，可以使用ES6+
                            const target = event.target;
                            const classes = [...target.classList];
                            
                            // 箭头函数
                            classes.forEach(cls => {
                                console.log(`Class: ${cls}`);
                            });
                            
                            // 异步操作
                            setTimeout(() => {
                                // 在JS中再次使用CHTL JS
                                {{#sidebar}}->animate({
                                    duration: 300,
                                    easing: ease-in-out,
                                    begin: {
                                        transform: 'translateX(-200px)'
                                    },
                                    end: {
                                        transform: 'translateX(0)'
                                    }
                                });
                            }, 100);
                        }
                    });
                }
            }
            """;
            
        scanAndPrint(code, "复杂嵌套场景");
    }
    
    /**
     * 扫描并打印结果
     */
    private static void scanAndPrint(String code, String testName) {
        System.out.println("测试: " + testName);
        System.out.println("代码:\n" + code);
        
        try {
            // 创建上下文管理器
            LanguageContextManager contextManager = new LanguageContextManager();
            
            // 创建精确扫描器
            PrecisionScanner scanner = new PrecisionScanner(contextManager);
            
            // 扫描代码
            List<CodeFragment> fragments = scanner.scan(code);
            
            // 打印结果
            System.out.println("扫描结果:");
            for (int i = 0; i < fragments.size(); i++) {
                CodeFragment fragment = fragments.get(i);
                System.out.println(String.format(
                    "  片段 %d: %s (位置: %d, 长度: %d)",
                    i + 1,
                    fragment.getType(),
                    fragment.getStart(),
                    fragment.getLength()
                ));
                
                // 检查自动添加的属性
                @SuppressWarnings("unchecked")
                Map<String, Set<String>> autoAttrs = 
                    (Map<String, Set<String>>) fragment.getMetadata("autoAttributes");
                if (autoAttrs != null && !autoAttrs.isEmpty()) {
                    System.out.println("    自动添加的属性:");
                    for (Map.Entry<String, Set<String>> entry : autoAttrs.entrySet()) {
                        System.out.println("      " + entry.getKey() + ": " + 
                                         String.join(", ", entry.getValue()));
                    }
                }
                
                // 打印内容摘要
                String content = fragment.getContent();
                if (content.length() > 50) {
                    content = content.substring(0, 47) + "...";
                }
                System.out.println("    内容: " + content.replace("\n", " "));
            }
            
            System.out.println();
            
        } catch (Exception e) {
            System.out.println("错误: " + e.getMessage());
            e.printStackTrace();
        }
    }
}