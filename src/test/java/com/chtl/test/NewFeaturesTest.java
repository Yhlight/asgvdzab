package com.chtl.test;

import com.chtl.compiler.CHTLCompiler;
import com.chtl.parser.CHTLParser;
import com.chtl.parser.CHTLJSParser;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.*;
import com.chtl.ast.chtljs.*;
import com.chtl.compiler.chtl.lexer.CHTLLexer;
import com.chtl.compiler.chtljs.lexer.CHTLJSLexer;
import com.chtl.compiler.chtl.token.CHTLToken;
import com.chtl.compiler.chtljs.token.CHTLJSToken;
import com.chtl.compiler.chtljs.token.CHTLJSTokenType;
import java.util.List;

/**
 * 新功能测试类
 * 测试->操作符、参数无序、[Custom]、[Configuration]、[Origin]、delegate等功能
 */
public class NewFeaturesTest {
    
    public static void main(String[] args) {
        NewFeaturesTest test = new NewFeaturesTest();
        
        System.out.println("=== 开始新功能测试 ===\n");
        
        // 测试->操作符
        test.testArrowOperator();
        
        // 测试参数无序解析
        test.testUnorderedParameters();
        
        // 测试[Custom]系统
        test.testCustomSystem();
        
        // 测试[Configuration]配置组
        test.testConfiguration();
        
        // 测试[Origin]原始嵌入
        test.testOriginBlock();
        
        // 测试delegate函数
        test.testDelegateFunction();
        
        System.out.println("\n=== 所有测试完成 ===");
    }
    
    /**
     * 测试->操作符
     */
    private void testArrowOperator() {
        System.out.println("1. 测试->操作符");
        
        String jsCode = "{{.button}}->listen({ click: function() { console.log('clicked'); } });";
        
        try {
            CHTLJSLexer lexer = new CHTLJSLexer(jsCode);
            List<CHTLJSToken> tokens = lexer.scanAllTokens();
            
            boolean foundArrow = false;
            for (CHTLJSToken token : tokens) {
                if (token.getType() == CHTLJSTokenType.ARROW) {
                    foundArrow = true;
                    System.out.println("  ✓ 找到->操作符: " + token.getValue());
                }
            }
            
            if (!foundArrow) {
                System.out.println("  ✗ 未找到->操作符");
            }
            
            // 测试解析
            CHTLJSParser parser = new CHTLJSParser(tokens);
            CHTLJSASTNode ast = parser.parse();
            System.out.println("  ✓ 成功解析->操作符");
            
        } catch (Exception e) {
            System.out.println("  ✗ 错误: " + e.getMessage());
        }
        
        System.out.println();
    }
    
    /**
     * 测试参数无序解析
     */
    private void testUnorderedParameters() {
        System.out.println("2. 测试参数无序解析");
        
        // 测试animate函数的不同参数顺序
        String[] animateCodes = {
            "animate({ duration: 500, easing: ease-in-out });",
            "animate({ easing: ease-in-out, duration: 500 });",
            "animate({ duration: 500 });",
            "animate({ easing: ease-in });",
            "animate({ loop: 3, duration: 200, direction: alternate });"
        };
        
        for (String code : animateCodes) {
            try {
                CHTLJSLexer lexer = new CHTLJSLexer(code);
                List<CHTLJSToken> tokens = lexer.scanAllTokens();
                CHTLJSParser parser = new CHTLJSParser(tokens);
                CHTLJSASTNode ast = parser.parse();
                System.out.println("  ✓ 成功解析: " + code);
            } catch (Exception e) {
                System.out.println("  ✗ 解析失败: " + code);
                System.out.println("    错误: " + e.getMessage());
            }
        }
        
        System.out.println();
    }
    
    /**
     * 测试[Custom]系统
     */
    private void testCustomSystem() {
        System.out.println("3. 测试[Custom]系统");
        
        String customCode = """
            [Custom] @Style RedText {
                @Style BaseText;
                color: red;
                font-weight: bold;
            }
            
            [Custom] @Element CustomBox {
                @Element Box;
                div {
                    class = "custom-box";
                }
                
                delete 2;
                
                insert 1 {
                    span {
                        text { Custom Header }
                    }
                };
            }
        """;
        
        try {
            CHTLLexer lexer = new CHTLLexer(customCode);
            List<CHTLToken> tokens = lexer.scanTokens();
            CHTLParser parser = new CHTLParser(tokens);
            CHTLASTNode ast = parser.parse();
            
            // 验证Custom节点
            List<CHTLASTNode> children = ast.getChildren();
            int customCount = 0;
            for (CHTLASTNode node : children) {
                if (node instanceof CustomDefinitionNode) {
                    customCount++;
                    CustomDefinitionNode custom = (CustomDefinitionNode) node;
                    System.out.println("  ✓ 找到[Custom]定义: " + custom.getName());
                    
                    // 检查特例化操作
                    if (!custom.getSpecializations().isEmpty()) {
                        System.out.println("    - 包含特例化操作: " + custom.getSpecializations().size() + "个");
                    }
                }
            }
            
            if (customCount == 0) {
                System.out.println("  ✗ 未找到[Custom]定义");
            }
            
        } catch (Exception e) {
            System.out.println("  ✗ 错误: " + e.getMessage());
            e.printStackTrace();
        }
        
        System.out.println();
    }
    
    /**
     * 测试[Configuration]配置组
     */
    private void testConfiguration() {
        System.out.println("4. 测试[Configuration]配置组");
        
        String configCode = """
            [Configuration] {
                INDEX_INITIAL_COUNT = 1;
                DEBUG_MODE = true;
                DISABLE_NAME_GROUP = false;
                
                [Name] {
                    CUSTOM_STYLE = [@Style, @style, @CSS];
                    CUSTOM_ELEMENT = @Element;
                }
            }
        """;
        
        try {
            CHTLLexer lexer = new CHTLLexer(configCode);
            List<CHTLToken> tokens = lexer.scanTokens();
            CHTLParser parser = new CHTLParser(tokens);
            CHTLASTNode ast = parser.parse();
            
            // 查找Configuration节点
            ConfigurationNode config = null;
            for (CHTLASTNode node : ast.getChildren()) {
                if (node instanceof ConfigurationNode) {
                    config = (ConfigurationNode) node;
                    break;
                }
            }
            
            if (config != null) {
                System.out.println("  ✓ 找到[Configuration]块");
                System.out.println("    - INDEX_INITIAL_COUNT: " + config.getSetting("INDEX_INITIAL_COUNT"));
                System.out.println("    - DEBUG_MODE: " + config.getSetting("DEBUG_MODE"));
                
                String[] customStyle = config.getNameMapping("CUSTOM_STYLE");
                if (customStyle != null) {
                    System.out.println("    - CUSTOM_STYLE映射: " + String.join(", ", customStyle));
                }
            } else {
                System.out.println("  ✗ 未找到[Configuration]块");
            }
            
        } catch (Exception e) {
            System.out.println("  ✗ 错误: " + e.getMessage());
        }
        
        System.out.println();
    }
    
    /**
     * 测试[Origin]原始嵌入
     */
    private void testOriginBlock() {
        System.out.println("5. 测试[Origin]原始嵌入");
        
        String[] originCodes = {
            "[Origin] @Html { <div>原始HTML</div> }",
            "[Origin] @Style { .raw { color: blue; } }",
            "[Origin] @JavaScript { console.log('原始JS'); }",
            "[Origin] @Html myDiv { <div id='test'>增强原始嵌入</div> }"
        };
        
        for (String code : originCodes) {
            try {
                CHTLLexer lexer = new CHTLLexer(code);
                List<CHTLToken> tokens = lexer.scanTokens();
                CHTLParser parser = new CHTLParser(tokens);
                CHTLASTNode ast = parser.parse();
                
                // 查找Origin节点
                OriginBlockNode origin = null;
                for (CHTLASTNode node : ast.getChildren()) {
                    if (node instanceof OriginBlockNode) {
                        origin = (OriginBlockNode) node;
                        break;
                    }
                }
                
                if (origin != null) {
                    System.out.println("  ✓ 成功解析[Origin] " + origin.getOriginType() + 
                        (origin.getName() != null ? " (名称: " + origin.getName() + ")" : ""));
                } else {
                    System.out.println("  ✗ 未找到[Origin]块: " + code);
                }
                
            } catch (Exception e) {
                System.out.println("  ✗ 解析失败: " + code);
                System.out.println("    错误: " + e.getMessage());
            }
        }
        
        System.out.println();
    }
    
    /**
     * 测试delegate函数
     */
    private void testDelegateFunction() {
        System.out.println("6. 测试delegate函数");
        
        String[] delegateCodes = {
            "{{.container}}->delegate({ target: {{.item}}, click: handleClick });",
            "{{#parent}}->delegate({ target: [{{.btn1}}, {{.btn2}}], mouseenter: onHover, click: onClick });",
            "{{body}}->delegate({ click: globalClick, target: {{.dynamic}} });"
        };
        
        for (String code : delegateCodes) {
            try {
                CHTLJSLexer lexer = new CHTLJSLexer(code);
                List<CHTLJSToken> tokens = lexer.scanAllTokens();
                CHTLJSParser parser = new CHTLJSParser(tokens);
                CHTLJSASTNode ast = parser.parse();
                System.out.println("  ✓ 成功解析delegate: " + code);
            } catch (Exception e) {
                System.out.println("  ✗ 解析失败: " + code);
                System.out.println("    错误: " + e.getMessage());
            }
        }
        
        System.out.println();
    }
}