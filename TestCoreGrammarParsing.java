import com.chtl.parser.CHTLParser;
import com.chtl.ast.*;
import com.chtl.ast.node.*;
import java.util.List;

/**
 * 测试CHTL核心语法解析
 * 重点测试模板(Template)、自定义(Custom)、变量组(Var)
 */
public class TestCoreGrammarParsing {
    public static void main(String[] args) {
        System.out.println("=== 测试CHTL核心语法解析 ===\n");
        
        // 1. 测试模板定义
        testTemplateDefinition();
        
        // 2. 测试自定义定义
        testCustomDefinition();
        
        // 3. 测试变量组定义
        testVarGroupDefinition();
        
        // 4. 测试特例化
        testSpecialization();
        
        // 5. 测试全缀名
        testFullName();
        
        System.out.println("\n测试完成！");
    }
    
    private static void testTemplateDefinition() {
        System.out.println("1. 测试模板定义");
        
        // 测试模板元素
        String code1 = """
            [Template] @Element Button {
                button {
                    style {
                        padding: 10px;
                        background: blue;
                    }
                    text {
                        Click Me
                    }
                }
            }
            """;
        testParse("模板元素", code1);
        
        // 测试模板样式组
        String code2 = """
            [Template] @Style WhiteText {
                color: white;
                font-size: 16px;
                line-height: 1.6;
                border: 1px black solid;
            }
            """;
        testParse("模板样式组", code2);
        
        // 测试模板变量组
        String code3 = """
            [Template] @Var BaseColors {
                primary: "#007bff";
                secondary: "#6c757d";
                success: "#28a745";
            }
            """;
        testParse("模板变量组", code3);
    }
    
    private static void testCustomDefinition() {
        System.out.println("\n2. 测试自定义定义");
        
        // 测试自定义样式组（无值属性）
        String code1 = """
            [Custom] @Style TextSet {
                color,
                font-size;
            }
            """;
        testParse("自定义样式组（无值属性）", code1);
        
        // 测试自定义样式组（继承）
        String code2 = """
            [Custom] @Style YellowText {
                @Style WhiteText;
                color: yellow;
            }
            """;
        testParse("自定义样式组（继承）", code2);
        
        // 测试自定义元素
        String code3 = """
            [Custom] @Element Box {
                div {
                    style {
                        border: 1px solid #ccc;
                    }
                }
                div {
                    @Element Line;
                }
                span {
                    text { "Info" }
                }
            }
            """;
        testParse("自定义元素", code3);
    }
    
    private static void testVarGroupDefinition() {
        System.out.println("\n3. 测试变量组定义");
        
        // 测试自定义变量组
        String code = """
            [Custom] @Var ThemeColor {
                tableColor: "rgb(255, 192, 203)";
                textColor: "black";
                borderWidth: 2px;
            }
            """;
        testParse("自定义变量组", code);
    }
    
    private static void testSpecialization() {
        System.out.println("\n4. 测试特例化");
        
        // 测试样式组特例化（删除属性）
        String code1 = """
            div {
                style {
                    @Style YellowText {
                        delete color, line-height;
                    }
                }
            }
            """;
        testParse("样式组特例化（删除属性）", code1);
        
        // 测试元素特例化（索引访问）
        String code2 = """
            body {
                @Element Box {
                    div[1] {
                        style {
                            color: red;
                        }
                    }
                }
            }
            """;
        testParse("元素特例化（索引访问）", code2);
        
        // 测试元素特例化（插入）
        String code3 = """
            body {
                @Element Box {
                    insert after div[0] {
                        div {
                            style {
                                margin: 10px;
                            }
                        }
                        @Element Line;
                    }
                }
            }
            """;
        testParse("元素特例化（插入）", code3);
        
        // 测试变量组特例化
        String code4 = """
            div {
                style {
                    color: ThemeColor(tableColor = rgb(145, 155, 200));
                }
            }
            """;
        testParse("变量组特例化", code4);
    }
    
    private static void testFullName() {
        System.out.println("\n5. 测试全缀名");
        
        String code = """
            body {
                @Element Box;
                [Custom] @Element Box;
                [Template] @Style WhiteText;
            }
            """;
        testParse("全缀名使用", code);
    }
    
    private static void testParse(String testName, String code) {
        System.out.println("\n测试: " + testName);
        System.out.println("代码:");
        System.out.println(code.trim());
        
        try {
            CHTLParser parser = new CHTLParser();
            RootNode ast = parser.parse(code);
            
            List<String> errors = parser.getErrors();
            if (!errors.isEmpty()) {
                System.out.println("解析错误:");
                for (String error : errors) {
                    System.out.println("  - " + error);
                }
            } else {
                System.out.println("✓ 解析成功");
                // 简单打印AST结构
                printAST(ast, 0);
            }
        } catch (Exception e) {
            System.out.println("✗ 异常: " + e.getMessage());
            e.printStackTrace();
        }
    }
    
    private static void printAST(CHTLASTNode node, int depth) {
        if (node == null) return;
        
        String indent = "  ".repeat(depth);
        String nodeInfo = node.getClass().getSimpleName();
        
        // 添加更多节点信息
        if (node instanceof TemplateDefinitionNode) {
            TemplateDefinitionNode t = (TemplateDefinitionNode) node;
            nodeInfo += " [" + t.getTemplateType() + " " + t.getName() + "]";
        } else if (node instanceof CustomDefinitionNode) {
            CustomDefinitionNode c = (CustomDefinitionNode) node;
            nodeInfo += " [" + c.getCustomType() + " " + c.getName() + "]";
        } else if (node instanceof VarGroupDefinitionNode) {
            VarGroupDefinitionNode v = (VarGroupDefinitionNode) node;
            nodeInfo += " [" + v.getName() + "]";
        } else if (node instanceof ElementNode) {
            ElementNode e = (ElementNode) node;
            nodeInfo += " [" + e.getTagName() + "]";
        }
        
        System.out.println(indent + nodeInfo);
        
        // 打印子节点
        for (CHTLASTNode child : node.getChildren()) {
            printAST(child, depth + 1);
        }
    }
}