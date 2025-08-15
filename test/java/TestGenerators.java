import com.chtl.parser.CHTLParser;
import com.chtl.parser.CHTLJSParser;
import com.chtl.generator.CHTLGenerator;
import com.chtl.generator.CHTLJSGenerator;
import com.chtl.ast.node.RootNode;
import com.chtl.ast.chtljs.CHTLJSASTNode;

import java.util.List;

/**
 * 测试CHTL和CHTL JS生成器
 */
public class TestGenerators {
    public static void main(String[] args) {
        System.out.println("=== 测试CHTL生成器 ===");
        testCHTLGenerator();
        
        System.out.println("\n=== 测试CHTL JS生成器 ===");
        testCHTLJSGenerator();
    }
    
    private static void testCHTLGenerator() {
        String chtlCode = """
            div {
                id: container;
                class: main-content;
                
                style {
                    width: 100%;
                    max-width: 1200px;
                    margin: 0 auto;
                    
                    .header {
                        background-color: #333;
                        color: white;
                        padding: 20px;
                    }
                    
                    &:hover {
                        box-shadow: 0 2px 5px rgba(0,0,0,0.1);
                    }
                }
                
                h1 {
                    class: header;
                    
                    text {
                        "Welcome to CHTL"
                    }
                }
                
                p {
                    text {
                        This is a test of the CHTL generator.
                    }
                    
                    style {
                        font-size: 16px;
                        line-height: 1.6;
                    }
                }
                
                script {
                    console.log("Page loaded");
                    {{#container}}->addEventListener('click', function(e) {
                        console.log('Container clicked');
                    });
                }
            }
            
            -- This is a generator comment
            
            [Origin] @Style {
                body {
                    font-family: Arial, sans-serif;
                    margin: 0;
                    padding: 0;
                }
            }
            
            [Origin] @JavaScript {
                function initApp() {
                    console.log('App initialized');
                }
                window.onload = initApp;
            }
            """;
        
        CHTLParser parser = new CHTLParser();
        RootNode root = parser.parse(chtlCode);
        
        if (!parser.getErrors().isEmpty()) {
            System.out.println("解析错误:");
            for (String error : parser.getErrors()) {
                System.out.println("  " + error);
            }
            return;
        }
        
        CHTLGenerator generator = new CHTLGenerator();
        CHTLGenerator.GenerationResult result = generator.generate(root);
        
        System.out.println("=== 生成的HTML ===");
        System.out.println(result.getHtml());
        
        System.out.println("\n=== 生成的CSS ===");
        System.out.println(result.getCss());
        
        System.out.println("\n=== 生成的JavaScript ===");
        System.out.println(result.getJavascript());
        
        System.out.println("\n=== 完整的HTML文档 ===");
        System.out.println(result.getCompleteHTML());
    }
    
    private static void testCHTLJSGenerator() {
        String chtljsCode = """
            {{#myButton}}->listen({
                click: () => {
                    console.log('Button clicked');
                    {{.status}}->textContent = 'Active';
                    {{.status}}->style.color = 'green';
                },
                mouseenter: function() {
                    this.style.backgroundColor = '#f0f0f0';
                },
                mouseleave: () => {
                    {{#myButton}}->style.backgroundColor = '';
                }
            });
            
            {{document.body}}->delegate({
                target: {{.btn}},
                click: (e) => {
                    e.preventDefault();
                    console.log('Delegated click on', e.target);
                }
            });
            
            animate({
                duration: 2000,
                easing: 'ease-out',
                begin: {
                    opacity: 0,
                    transform: 'translateY(20px)'
                },
                when: [
                    {
                        at: 0.5,
                        opacity: 0.8,
                        transform: 'translateY(-5px)'
                    }
                ],
                end: {
                    opacity: 1,
                    transform: 'translateY(0)'
                },
                loop: 3,
                delay: 500,
                callback: () => {
                    console.log('Animation completed');
                }
            });
            
            const userList = {{.user-list}};
            userList->innerHTML = '<li>Loading...</li>';
            
            fetch('/api/users')
                .then(response => response.json())
                .then(users => {
                    userList->innerHTML = users.map(user => 
                        `<li>${user.name}</li>`
                    ).join('');
                });
            """;
        
        CHTLJSParser parser = new CHTLJSParser();
        List<CHTLJSASTNode> statements = parser.parse(chtljsCode);
        
        if (!parser.getErrors().isEmpty()) {
            System.out.println("解析错误:");
            for (String error : parser.getErrors()) {
                System.out.println("  " + error);
            }
            return;
        }
        
        CHTLJSGenerator generator = new CHTLJSGenerator();
        String jsCode = generator.generate(statements);
        
        System.out.println("=== 生成的JavaScript ===");
        System.out.println(jsCode);
    }
}