import com.chtl.parser.CHTLParser;
import com.chtl.parser.CHTLJSParser;
import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.ast.chtljs.*;

import java.util.List;

/**
 * 测试CHTL和CHTL JS解析器
 */
public class TestParsers {
    public static void main(String[] args) {
        System.out.println("=== 测试CHTL解析器 ===");
        testCHTLParser();
        
        System.out.println("\n=== 测试CHTL JS解析器 ===");
        testCHTLJSParser();
    }
    
    private static void testCHTLParser() {
        String chtlCode = """
            // 测试注释
            div {
                id: myDiv;
                class= container;  // 测试CE对等式
                
                style {
                    width: 100px;
                    height: 200px;
                    
                    .box {
                        color: red;
                        background-color: blue;
                    }
                    
                    &:hover {
                        opacity: 0.8;
                    }
                }
                
                text {
                    "Hello CHTL!"
                }
                
                span {
                    class: inner;
                    
                    text {
                        Welcome
                    }
                }
                
                script {
                    console.log("Local script");
                    {{#myButton}}->listen({
                        click: () => {
                            console.log("Clicked!");
                        }
                    });
                }
            }
            
            [Template] @Style BaseTheme {
                color: black;
                font-size: 14px;
            }
            
            [Custom] @Element Card {
                div {
                    class: card;
                    
                    div {
                        class: card-header;
                    }
                    
                    div {
                        class: card-body;
                    }
                }
            }
            """;
        
        CHTLParser parser = new CHTLParser();
        RootNode root = parser.parse(chtlCode);
        
        if (!parser.getErrors().isEmpty()) {
            System.out.println("解析错误:");
            for (String error : parser.getErrors()) {
                System.out.println("  " + error);
            }
        } else {
            System.out.println("解析成功!");
            printAST(root, 0);
        }
    }
    
    private static void testCHTLJSParser() {
        String chtljsCode = """
            {{#myButton}}->listen({
                click: () => {
                    console.log('Button clicked');
                    {{.status}}->textContent = 'Active';
                },
                mouseenter: function() {
                    {{.tooltip}}->style.display = 'block';
                }
            });
            
            {{document.body}}->delegate({
                target: {{.dynamic-btn}},
                click: handleDynamicClick,
                mouseover: (e) => {
                    e.target.classList.add('hover');
                }
            });
            
            animate({
                duration: 1000,
                easing: 'ease-in-out',
                begin: {
                    opacity: 0,
                    transform: 'scale(0.8)'
                },
                when: [
                    {
                        at: 0.5,
                        opacity: 0.5,
                        transform: 'scale(1.1)'
                    }
                ],
                end: {
                    opacity: 1,
                    transform: 'scale(1)'
                },
                callback: () => console.log('Animation done')
            });
            """;
        
        CHTLJSParser parser = new CHTLJSParser();
        List<CHTLJSASTNode> statements = parser.parse(chtljsCode);
        
        if (!parser.getErrors().isEmpty()) {
            System.out.println("解析错误:");
            for (String error : parser.getErrors()) {
                System.out.println("  " + error);
            }
        } else {
            System.out.println("解析成功!");
            System.out.println("语句数量: " + statements.size());
            
            for (int i = 0; i < statements.size(); i++) {
                System.out.println("\n语句 " + (i + 1) + ":");
                printCHTLJSAST(statements.get(i), 0);
            }
        }
    }
    
    private static void printAST(CHTLASTNode node, int depth) {
        StringBuilder indent = new StringBuilder();
        for (int i = 0; i < depth; i++) {
            indent.append("  ");
        }
        
        System.out.print(indent + node.getNodeType());
        
        // 打印节点特定信息
        if (node instanceof ElementNode) {
            ElementNode elem = (ElementNode) node;
            System.out.print(" <" + elem.getTagName() + ">");
        } else if (node instanceof AttributeNode) {
            AttributeNode attr = (AttributeNode) node;
            System.out.print(" " + attr.getName() + (attr.usesColon() ? ":" : "="));
            if (attr.getValue() != null) {
                System.out.print(" " + getNodeValue(attr.getValue()));
            }
        } else if (node instanceof TextNode) {
            TextNode text = (TextNode) node;
            System.out.print(" \"" + text.getContent() + "\"");
        } else if (node instanceof StyleSelectorNode) {
            StyleSelectorNode selector = (StyleSelectorNode) node;
            System.out.print(" " + selector.getFullSelector());
        } else if (node instanceof StylePropertyNode) {
            StylePropertyNode prop = (StylePropertyNode) node;
            System.out.print(" " + prop.getProperty());
        } else if (node instanceof TemplateDefinitionNode) {
            TemplateDefinitionNode template = (TemplateDefinitionNode) node;
            System.out.print(" " + template.getTemplateType() + " " + template.getName());
        } else if (node instanceof CustomDefinitionNode) {
            CustomDefinitionNode custom = (CustomDefinitionNode) node;
            System.out.print(" " + custom.getCustomType() + " " + custom.getName());
        }
        
        System.out.println();
        
        // 递归打印子节点
        for (CHTLASTNode child : node.getChildren()) {
            printAST(child, depth + 1);
        }
    }
    
    private static String getNodeValue(CHTLASTNode node) {
        if (node instanceof StringLiteralNode) {
            return "\"" + ((StringLiteralNode) node).getValue() + "\"";
        } else if (node instanceof UnquotedLiteralNode) {
            return ((UnquotedLiteralNode) node).getValue();
        } else if (node instanceof NumberLiteralNode) {
            return ((NumberLiteralNode) node).getValue();
        }
        return node.toString();
    }
    
    private static void printCHTLJSAST(CHTLJSASTNode node, int depth) {
        StringBuilder indent = new StringBuilder();
        for (int i = 0; i < depth; i++) {
            indent.append("  ");
        }
        
        System.out.print(indent + node.getNodeType());
        
        // 打印节点特定信息
        if (node instanceof EnhancedSelectorNode) {
            EnhancedSelectorNode selector = (EnhancedSelectorNode) node;
            System.out.print(" {{" + selector.getSelector() + "}} (" + selector.getSelectorType() + ")");
        } else if (node instanceof ListenCallNode) {
            ListenCallNode listen = (ListenCallNode) node;
            System.out.print(" events: " + listen.getEventHandlers().keySet());
        } else if (node instanceof DelegateCallNode) {
            DelegateCallNode delegate = (DelegateCallNode) node;
            System.out.print(" targets: " + delegate.getTargetElements().size());
        } else if (node instanceof EventHandlerNode) {
            EventHandlerNode handler = (EventHandlerNode) node;
            System.out.print(" " + handler.getEventName() + " (" + handler.getHandlerType() + ")");
        } else if (node instanceof AnimationConfigNode) {
            AnimationConfigNode config = (AnimationConfigNode) node;
            System.out.print(" duration: " + config.getDuration());
        } else if (node instanceof AnimationKeyframeNode) {
            AnimationKeyframeNode keyframe = (AnimationKeyframeNode) node;
            System.out.print(" at: " + keyframe.getAt());
        } else if (node instanceof ExpressionNode) {
            ExpressionNode expr = (ExpressionNode) node;
            System.out.print(" \"" + expr.getExpression() + "\"");
        }
        
        System.out.println();
    }
}