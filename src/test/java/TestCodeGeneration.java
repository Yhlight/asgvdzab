import com.chtl.generator.CHTLGenerator;
import com.chtl.ast.node.*;
import com.chtl.generator.GenerationResult;

/**
 * 代码生成测试
 */
public class TestCodeGeneration {
    
    public static void main(String[] args) {
        TestCodeGeneration test = new TestCodeGeneration();
        
        System.out.println("=== 代码生成测试 ===\n");
        
        // 测试[Origin]生成
        test.testOriginGeneration();
        
        // 测试基本HTML生成
        test.testBasicHTMLGeneration();
        
        System.out.println("\n=== 测试完成 ===");
    }
    
    /**
     * 测试[Origin]生成
     */
    private void testOriginGeneration() {
        System.out.println("1. 测试[Origin]代码生成");
        
        // 创建AST
        RootNode root = new RootNode();
        
        // 添加Origin HTML
        OriginBlockNode htmlOrigin = new OriginBlockNode(OriginBlockNode.OriginType.HTML);
        htmlOrigin.setContent("<div class='raw'>原始HTML内容</div>");
        root.addChild(htmlOrigin);
        
        // 添加Origin Style
        OriginBlockNode styleOrigin = new OriginBlockNode(OriginBlockNode.OriginType.STYLE);
        styleOrigin.setContent(".raw { color: red; font-weight: bold; }");
        root.addChild(styleOrigin);
        
        // 添加Origin JavaScript
        OriginBlockNode jsOrigin = new OriginBlockNode(OriginBlockNode.OriginType.JAVASCRIPT);
        jsOrigin.setContent("console.log('原始JavaScript代码');");
        root.addChild(jsOrigin);
        
        // 生成代码
        CHTLGenerator generator = new CHTLGenerator();
        GenerationResult result = generator.generate(root);
        
        System.out.println("  生成的HTML:");
        System.out.println("  " + result.getHtml().trim());
        System.out.println("\n  生成的CSS:");
        System.out.println("  " + result.getCss().trim());
        System.out.println("\n  生成的JavaScript:");
        System.out.println("  " + result.getJavaScript().trim());
        
        // 验证
        if (result.getHtml().contains("原始HTML内容")) {
            System.out.println("\n  ✓ HTML原始内容正确生成");
        } else {
            System.out.println("\n  ✗ HTML原始内容生成失败");
        }
        
        if (result.getCss().contains(".raw { color: red;")) {
            System.out.println("  ✓ CSS原始内容正确生成");
        } else {
            System.out.println("  ✗ CSS原始内容生成失败");
        }
        
        if (result.getJavaScript().contains("console.log('原始JavaScript代码')")) {
            System.out.println("  ✓ JavaScript原始内容正确生成");
        } else {
            System.out.println("  ✗ JavaScript原始内容生成失败");
        }
        
        System.out.println();
    }
    
    /**
     * 测试基本HTML生成
     */
    private void testBasicHTMLGeneration() {
        System.out.println("2. 测试基本HTML生成");
        
        // 创建AST
        RootNode root = new RootNode();
        
        // 创建一个div元素
        ElementNode div = new ElementNode("div");
        div.addAttribute("class", "container");
        
        // 添加文本内容
        TextNode text = new TextNode("Hello CHTL!");
        div.addChild(text);
        
        // 添加局部样式
        StyleBlockNode styleBlock = new StyleBlockNode(true);
        StyleRuleNode rule = new StyleRuleNode();
        StyleSelectorNode selector = new StyleSelectorNode(StyleSelectorNode.SelectorType.SELF_REFERENCE);
        rule.setSelector(selector);
        
        StylePropertyNode colorProp = new StylePropertyNode("color", "blue");
        rule.addProperty(colorProp);
        styleBlock.addRule(rule);
        div.addChild(styleBlock);
        
        root.addChild(div);
        
        // 生成代码
        CHTLGenerator generator = new CHTLGenerator();
        GenerationResult result = generator.generate(root);
        
        System.out.println("  生成的HTML:");
        System.out.println("  " + result.getHtml().trim());
        System.out.println("\n  生成的CSS:");
        System.out.println("  " + result.getCss().trim());
        
        // 验证
        if (result.getHtml().contains("Hello CHTL!")) {
            System.out.println("\n  ✓ HTML内容正确生成");
        }
        if (result.getCss().contains("color: blue")) {
            System.out.println("  ✓ CSS样式正确生成");
        }
        
        System.out.println();
    }
}