import com.chtl.parser.CHTLParser;
import com.chtl.generator.CHTLGenerator;
import com.chtl.context.*;
import com.chtl.ast.node.RootNode;
import com.chtl.scanner.CHTLUnifiedScanner;
import com.chtl.scanner.ScannerStateMachine;
import com.chtl.model.CodeFragment;

import java.util.List;

/**
 * 测试上下文管理器集成
 * 展示RAII模式在CHTL编译过程中的应用
 */
public class TestContextIntegration {
    public static void main(String[] args) {
        System.out.println("=== CHTL上下文管理集成测试 ===\n");
        
        String chtlCode = """
            // 测试RAII状态管理
            [Namespace] MyApp {
                // 全局约束
                except @Html;
                
                [Template] @Element Card {
                    div {
                        class: card;
                        
                        style {
                            background: white;
                            padding: 20px;
                            
                            &:hover {
                                background: #f0f0f0;
                            }
                        }
                        
                        h3 {
                            class: card-title;
                            text {
                                "Card Title"
                            }
                        }
                        
                        script {
                            {{.card}}->listen({
                                click: (e) => {
                                    console.log('Card clicked');
                                }
                            });
                        }
                    }
                }
                
                [Custom] @Style Theme {
                    color, background-color, border-color;
                }
            }
            
            // 使用命名空间中的定义
            div {
                id: app;
                
                @Element Card {
                    h3 {
                        text {
                            "My Card"
                        }
                    }
                }
                
                style {
                    @Style Theme {
                        color: #333;
                        background-color: #fff;
                        border-color: #ddd;
                    }
                }
            }
            """;
        
        // 创建编译上下文
        CompilationContext compilationContext = new CompilationContext();
        
        System.out.println("1. 扫描代码片段...");
        CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
        ScannerStateMachine stateMachine = new ScannerStateMachine(compilationContext);
        
        List<CodeFragment> fragments = scanner.scan(chtlCode);
        System.out.println("   扫描到 " + fragments.size() + " 个代码片段");
        
        // 显示片段信息
        for (int i = 0; i < fragments.size(); i++) {
            CodeFragment fragment = fragments.get(i);
            System.out.println("   片段 " + (i + 1) + ": " + fragment.getType() + 
                             " (位置: " + fragment.getStartPos() + "-" + fragment.getEndPos() + ")");
        }
        
        System.out.println("\n2. 解析CHTL代码...");
        CHTLParser parser = new CHTLParser();
        RootNode ast = parser.parse(chtlCode);
        
        if (!parser.getErrors().isEmpty()) {
            System.err.println("解析错误:");
            for (String error : parser.getErrors()) {
                System.err.println("  " + error);
            }
            return;
        }
        
        System.out.println("   解析成功");
        
        // 显示上下文信息
        System.out.println("\n3. 编译上下文状态:");
        System.out.println("   当前状态: " + compilationContext.getCurrentState());
        System.out.println("   当前作用域: " + compilationContext.getCurrentScope().getType());
        
        if (compilationContext.hasErrors()) {
            System.out.println("\n编译错误:");
            for (CompilationError error : compilationContext.getErrors()) {
                System.out.println("  " + error);
            }
        }
        
        if (!compilationContext.getWarnings().isEmpty()) {
            System.out.println("\n编译警告:");
            for (CompilationWarning warning : compilationContext.getWarnings()) {
                System.out.println("  " + warning);
            }
        }
        
        System.out.println("\n4. 演示RAII状态管理...");
        demonstrateRAII(compilationContext);
        
        System.out.println("\n5. 生成代码...");
        CHTLGenerator generator = new CHTLGenerator();
        CHTLGenerator.GenerationResult result = generator.generate(ast);
        
        System.out.println("   HTML行数: " + countLines(result.getHtml()));
        System.out.println("   CSS行数: " + countLines(result.getCss()));
        System.out.println("   JavaScript行数: " + countLines(result.getJavascript()));
        
        System.out.println("\n测试完成！");
    }
    
    /**
     * 演示RAII状态管理
     */
    private static void demonstrateRAII(CompilationContext context) {
        System.out.println("   初始状态: " + context.getCurrentState());
        
        // 演示嵌套状态管理
        try (var state1 = context.enterState(CompilationContext.State.ELEMENT)) {
            System.out.println("   进入ELEMENT状态: " + context.getCurrentState());
            
            try (var state2 = context.enterState(CompilationContext.State.STYLE_BLOCK)) {
                System.out.println("   进入STYLE_BLOCK状态: " + context.getCurrentState());
                
                try (var state3 = context.enterState(CompilationContext.State.STYLE_RULE)) {
                    System.out.println("   进入STYLE_RULE状态: " + context.getCurrentState());
                } // 自动退出STYLE_RULE
                
                System.out.println("   回到STYLE_BLOCK状态: " + context.getCurrentState());
            } // 自动退出STYLE_BLOCK
            
            System.out.println("   回到ELEMENT状态: " + context.getCurrentState());
        } // 自动退出ELEMENT
        
        System.out.println("   回到初始状态: " + context.getCurrentState());
        
        // 演示作用域管理
        System.out.println("\n   演示作用域管理:");
        try (var scope1 = context.enterScope(CompilationContext.ScopeType.NAMESPACE, "MyApp")) {
            System.out.println("   进入NAMESPACE作用域: " + context.getCurrentScope().getType());
            
            // 注册模板定义
            var template = new com.chtl.ast.node.TemplateDefinitionNode(
                com.chtl.ast.node.TemplateDefinitionNode.TemplateType.ELEMENT, "TestTemplate");
            context.registerTemplate(template);
            
            System.out.println("   注册模板: TestTemplate");
            
            // 查找模板
            var found = context.findTemplate(
                com.chtl.ast.node.TemplateDefinitionNode.TemplateType.ELEMENT, "TestTemplate");
            System.out.println("   查找模板结果: " + (found != null ? "找到" : "未找到"));
            
        } // 自动退出作用域
        
        System.out.println("   回到全局作用域: " + context.getCurrentScope().getType());
    }
    
    private static int countLines(String text) {
        return text.isEmpty() ? 0 : text.split("\n").length;
    }
}