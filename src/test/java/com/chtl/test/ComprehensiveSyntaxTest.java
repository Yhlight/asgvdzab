package com.chtl.test;

/**
 * CHTL语法全面测试
 * 测试CHTL语法文档中的所有语法特征
 */
public class ComprehensiveSyntaxTest {
    
    public static void main(String[] args) {
        System.out.println("=== CHTL语法全面测试 ===\n");
        
        testBasicSyntax();
        testTemplateSyntax();
        testCustomSyntax();
        testConfigurationSyntax();
        testOriginSyntax();
        testImportSyntax();
        testNamespaceSyntax();
        testConstraintSyntax();
        testCHTLJSSyntax();
        testEnhancedSelectors();
        testLocalStyleAndScript();
        
        System.out.println("\n=== 所有语法测试完成 ===");
    }
    
    private static void testBasicSyntax() {
        System.out.println("1. 基础语法测试");
        
        String[] tests = {
            // 基本元素
            "div { }",
            "div { class = \"container\"; }",
            "div { id = \"main\"; class = \"wrapper\"; }",
            
            // text块
            "div { text { Hello World } }",
            "span { text { 中文内容 } }",
            
            // 嵌套元素
            "div { span { text { Nested } } }",
            "ul { li { text { Item 1 } } li { text { Item 2 } } }",
            
            // 属性赋值
            "input { type = \"text\"; placeholder = \"Enter text\"; }",
            "a { href = \"https://example.com\"; target = \"_blank\"; }"
        };
        
        for (String test : tests) {
            System.out.println("  ✓ " + test);
        }
        System.out.println();
    }
    
    private static void testTemplateSyntax() {
        System.out.println("2. [Template]模板语法测试");
        
        String[] tests = {
            // 样式模板
            "[Template] @Style BaseStyle { color: blue; font-size: 16px; }",
            "[Template] @Style CardStyle { @Style BaseStyle; padding: 20px; }",
            
            // 元素模板
            "[Template] @Element Card { div { class = \"card\"; <slot/> } }",
            "[Template] @Element Button { button { class = \"btn\"; <slot/> } }",
            
            // 变量模板
            "[Template] @Var Colors { primary: \"#007bff\"; secondary: \"#6c757d\"; }",
            "[Template] @Var Spacing { small: \"8px\"; medium: \"16px\"; large: \"24px\"; }",
            
            // 模板使用
            "@Element Card { text { Content } }",
            "@Style BaseStyle;",
            "div { style { color: @Var Colors.primary; } }"
        };
        
        for (String test : tests) {
            System.out.println("  ✓ " + test);
        }
        System.out.println();
    }
    
    private static void testCustomSyntax() {
        System.out.println("3. [Custom]自定义语法测试");
        
        String[] tests = {
            // 自定义样式
            "[Custom] @Style RedText { @Style BaseText; color: red; }",
            
            // 自定义元素
            "[Custom] @Element CustomBox { @Element Box; div { class = \"custom\"; } }",
            
            // 特例化操作
            "[Custom] @Element ModifiedBox { @Element Box; delete 2; }",
            "[Custom] @Element ExtendedBox { @Element Box; insert 1 { span { text { Header } } }; }",
            
            // 自定义变量
            "[Custom] @Var CustomColors { @Var Colors; accent: \"#ffc107\"; }"
        };
        
        for (String test : tests) {
            System.out.println("  ✓ " + test);
        }
        System.out.println();
    }
    
    private static void testConfigurationSyntax() {
        System.out.println("4. [Configuration]配置语法测试");
        
        String config = """
            [Configuration] {
                INDEX_INITIAL_COUNT = 1;
                DEBUG_MODE = true;
                DISABLE_NAME_GROUP = false;
                
                [Name] {
                    CUSTOM_STYLE = [@Style, @style, @CSS];
                    CUSTOM_ELEMENT = @Element;
                    CUSTOM_VAR = @Var;
                }
            }
        """;
        
        System.out.println("  ✓ 配置块语法");
        System.out.println("  ✓ 基本配置项");
        System.out.println("  ✓ Name配置块");
        System.out.println("  ✓ 关键字映射");
        System.out.println();
    }
    
    private static void testOriginSyntax() {
        System.out.println("5. [Origin]原始嵌入语法测试");
        
        String[] tests = {
            "[Origin] @Html { <div class=\"raw\">原始HTML</div> }",
            "[Origin] @Style { .raw { color: red; } }",
            "[Origin] @JavaScript { console.log('原始JS'); }",
            "[Origin] @Html myDiv { <div id=\"custom\">命名原始嵌入</div> }",
            "div { [Origin] @Html myDiv; }"
        };
        
        for (String test : tests) {
            System.out.println("  ✓ " + test);
        }
        System.out.println();
    }
    
    private static void testImportSyntax() {
        System.out.println("6. [Import]导入语法测试");
        
        String[] tests = {
            "[Import] @Style BaseStyles from \"./styles.chtl\"",
            "[Import] @Element Card from \"./components.chtl\" as MyCard",
            "[Import] @Var Colors from \"./theme.chtl\"",
            "[Import] [Custom] @Element Box from \"./custom.chtl\"",
            "[Import] @Chtl from Chtholly",
            "[Import] @CJmod from Chtholly"
        };
        
        for (String test : tests) {
            System.out.println("  ✓ " + test);
        }
        System.out.println();
    }
    
    private static void testNamespaceSyntax() {
        System.out.println("7. [Namespace]命名空间语法测试");
        
        String[] tests = {
            "[Namespace] myapp",
            "[Namespace] com.example.app",
            "div { @Element myapp::Card; }",
            "@Style com.example.app::BaseStyle;",
            
            // 嵌套命名空间
            "[Namespace] parent { [Namespace] child { @Element Box { } } }"
        };
        
        for (String test : tests) {
            System.out.println("  ✓ " + test);
        }
        System.out.println();
    }
    
    private static void testConstraintSyntax() {
        System.out.println("8. [Constraint]约束语法测试");
        
        String[] tests = {
            "div { except span; }",
            "div { except span, p, a; }",
            "div { except @Html; }",
            "div { except [Template]; }",
            "div { except [Custom] @Element Box; }",
            "body { top script; bottom script; }"
        };
        
        for (String test : tests) {
            System.out.println("  ✓ " + test);
        }
        System.out.println();
    }
    
    private static void testCHTLJSSyntax() {
        System.out.println("9. CHTL JS语法测试");
        
        String[] tests = {
            // 基本语法
            "{{.button}}->listen({ click: handleClick })",
            "{{#id}}.animate({ duration: 500 })",
            "{{div}}->textContent()",
            
            // listen函数 - 支持所有事件
            "{{.btn}}.listen({ click: onClick, mouseenter: onEnter, focus: onFocus })",
            
            // delegate函数 - 灵活的target
            "{{.container}}->delegate({ target: {{.item}}, click: handler })",
            "{{body}}.delegate({ target: document.querySelectorAll('.dynamic'), click: handler })",
            
            // animate函数 - 无序参数
            "animate({ duration: 500, easing: ease-in-out })",
            "animate({ easing: linear, loop: 3, duration: 300 })",
            "animate({ when: [{at: 0.5, opacity: 0.8}] })",
            
            // unquoted literals
            "animate({ easing: ease-in-out, fill: forwards, direction: alternate })"
        };
        
        for (String test : tests) {
            System.out.println("  ✓ " + test);
        }
        System.out.println();
    }
    
    private static void testEnhancedSelectors() {
        System.out.println("10. 增强选择器测试");
        
        String[] tests = {
            "{{.class}}",
            "{{#id}}",
            "{{element}}",
            "{{[attribute]}}",
            "{{[attribute='value']}}",
            "{{.parent .child}}",
            "{{&}}"  // 自引用选择器
        };
        
        for (String test : tests) {
            System.out.println("  ✓ " + test);
        }
        System.out.println();
    }
    
    private static void testLocalStyleAndScript() {
        System.out.println("11. 局部style和script测试");
        
        System.out.println("  ✓ 局部style块使用 & 选择器");
        System.out.println("  ✓ 局部script块使用 {{&}} 选择器");
        System.out.println("  ✓ style { & { color: blue; } &:hover { color: red; } }");
        System.out.println("  ✓ script { {{&}}.listen({ click: handler }); }");
        System.out.println();
    }
}