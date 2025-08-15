import com.chtl.scanner.CHTLUnifiedScanner;
import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.List;

/**
 * 全面测试script块中CHTL JS和JS的精确切割
 */
public class TestScriptBlockCutting {
    
    public static void main(String[] args) {
        System.out.println("=== Script块切割测试 ===\n");
        
        CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
        
        // 测试1: 全局script块 - CHTL JS增强选择器
        testGlobalScriptSelectors(scanner);
        
        // 测试2: 全局script块 - CHTL JS函数
        testGlobalScriptFunctions(scanner);
        
        // 测试3: 局部script块 - {{&}}自引用
        testLocalScriptSelfReference(scanner);
        
        // 测试4: 复杂混合场景
        testComplexMixedScript(scanner);
        
        // 测试5: 原始嵌入在script中
        testOriginInScript(scanner);
    }
    
    private static void testGlobalScriptSelectors(CHTLUnifiedScanner scanner) {
        System.out.println("测试1: 全局script - 增强选择器");
        String code = """
            script {
                // CHTL JS增强选择器
                let button = {{.button}};
                let modal = {{#modal}};
                let items = {{.list-item}};
                
                // 普通JS代码
                let count = 0;
                const config = { max: 100 };
                
                // 混合使用
                {{.container}}.listen({
                    click: function() {
                        count++;
                        {{.counter}}.text(count);
                    }
                });
            }
            """;
        
        analyzeFragments(scanner.scan(code), "增强选择器");
    }
    
    private static void testGlobalScriptFunctions(CHTLUnifiedScanner scanner) {
        System.out.println("\n测试2: 全局script - CHTL JS函数");
        String code = """
            script {
                // listen函数
                element.listen({
                    click: () => console.log('clicked'),
                    hover: function() { this.addClass('hover'); }
                });
                
                // delegate函数  
                {{.parent}}.delegate({
                    target: '.child',
                    click: (e) => e.stopPropagation()
                });
                
                // animate函数
                let anim = animate({
                    target: {{.box}},
                    duration: 300,
                    easing: 'ease-out',
                    opacity: 0.5
                });
                
                // 箭头操作符
                {{.nav}}->addClass('active');
                document.querySelector('.test')->style.display = 'none';
            }
            """;
        
        analyzeFragments(scanner.scan(code), "CHTL JS函数");
    }
    
    private static void testLocalScriptSelfReference(CHTLUnifiedScanner scanner) {
        System.out.println("\n测试3: 局部script - {{&}}自引用");
        String code = """
            button {
                script {
                    // {{&}}是CHTL语法
                    {{&}}.listen({
                        click: function() {
                            {{&}}->toggleClass('active');
                            console.log('Button clicked');
                        },
                        mouseenter: () => {
                            {{&}}->animate({ scale: 1.1 });
                        }
                    });
                    
                    // 支持from语法
                    let utils from MyModule;
                    
                    // 普通JS
                    let state = { clicked: false };
                }
            }
            """;
        
        analyzeFragments(scanner.scan(code), "局部script自引用");
    }
    
    private static void testComplexMixedScript(CHTLUnifiedScanner scanner) {
        System.out.println("\n测试4: 复杂混合场景");
        String code = """
            script {
                // 复杂的对象字面量
                const handlers = {
                    onClick: function() {
                        {{.modal}}->show();
                    },
                    onHover: () => {{.tooltip}}->fadeIn(),
                    data: { items: [1, 2, 3] }
                };
                
                // 嵌套的CHTL JS
                {{.list}}.delegate({
                    target: '.item',
                    click: function(e) {
                        let index = {{.item}}.index(this);
                        animate({
                            target: this,
                            height: 0,
                            oncomplete: () => {
                                this->remove();
                            }
                        });
                    }
                });
                
                // 字符串中的假选择器（不应被识别）
                let template = "Use {{.selector}} for selection";
                let arrow = "Use -> for method calls";
            }
            """;
        
        analyzeFragments(scanner.scan(code), "复杂混合");
    }
    
    private static void testOriginInScript(CHTLUnifiedScanner scanner) {
        System.out.println("\n测试5: script中的原始嵌入");
        String code = """
            script {
                // 普通CHTL JS
                {{.app}}.listen({ load: init });
                
                // 原始嵌入
                [Origin] @Script {
                    // 第三方库代码
                    (function($) {
                        $.fn.oldPlugin = function() {
                            return this.each(function() {
                                // Legacy code
                            });
                        };
                    })(jQuery);
                }
                
                // 继续CHTL JS
                animate({ target: {{.logo}}, rotate: 360 });
            }
            """;
        
        analyzeFragments(scanner.scan(code), "原始嵌入");
    }
    
    private static void analyzeFragments(List<CodeFragment> fragments, String testName) {
        System.out.println("\n" + testName + " - 片段分析:");
        
        int chtlCount = 0, jsCount = 0, chtljsCount = 0;
        
        for (CodeFragment fragment : fragments) {
            String content = fragment.getContent().trim();
            if (content.isEmpty()) continue;
            
            // 统计
            switch (fragment.getType()) {
                case CHTL: chtlCount++; break;
                case JS: jsCount++; break;
                case CHTL_JS: chtljsCount++; break;
            }
            
            // 显示关键片段
            if (fragment.getType() == FragmentType.CHTL_JS || 
                content.contains("{{") || 
                content.contains("->") ||
                content.contains("listen") ||
                content.contains("delegate") ||
                content.contains("animate") ||
                content.contains("[Origin]")) {
                
                System.out.printf("  [%s]: %s\n",
                    fragment.getType().getDisplayName(),
                    formatFragment(content));
            }
        }
        
        System.out.printf("\n统计: CHTL=%d, JS=%d, CHTL_JS=%d\n", 
            chtlCount, jsCount, chtljsCount);
            
        // 验证切割是否合理
        if (chtljsCount == 0 && testName.contains("CHTL JS")) {
            System.out.println("⚠️  警告: 未检测到CHTL JS片段!");
        }
    }
    
    private static String formatFragment(String content) {
        content = content.replace("\n", " ").replace("\r", "");
        if (content.length() > 60) {
            return content.substring(0, 57) + "...";
        }
        return content;
    }
}