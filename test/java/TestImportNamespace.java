import com.chtl.compiler.importer.ImportManager;
import com.chtl.compiler.namespace.NamespaceManager;
import com.chtl.context.CompilationContext;
import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.parser.CHTLParser;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.Files;
import java.io.IOException;

/**
 * 测试增强的导入和命名空间功能
 */
public class TestImportNamespace {
    public static void main(String[] args) throws IOException {
        System.out.println("=== 导入和命名空间功能测试 ===\n");
        
        // 创建测试文件
        createTestFiles();
        
        // 测试导入功能
        System.out.println("1. 测试导入功能:");
        testImportFeatures();
        
        // 测试命名空间功能
        System.out.println("\n2. 测试命名空间功能:");
        testNamespaceFeatures();
        
        // 清理测试文件
        cleanupTestFiles();
    }
    
    private static void createTestFiles() throws IOException {
        System.out.println("创建测试文件...");
        
        // 创建测试目录
        Files.createDirectories(Paths.get("test-imports"));
        Files.createDirectories(Paths.get("test-imports/module"));
        
        // 创建HTML文件
        Files.writeString(Paths.get("test-imports/template.html"), 
            "<div class=\"template\">\n  <h1>Template</h1>\n</div>");
        
        // 创建CSS文件
        Files.writeString(Paths.get("test-imports/styles.css"), 
            ".template {\n  background: white;\n  padding: 20px;\n}");
        
        // 创建JavaScript文件
        Files.writeString(Paths.get("test-imports/script.js"), 
            "function initTemplate() {\n  console.log('Template initialized');\n}");
        
        // 创建CHTL模块文件
        Files.writeString(Paths.get("test-imports/module/utils.chtl"), 
            "[Template] @Element Card {\n  div {\n    class: card;\n  }\n}");
        
        Files.writeString(Paths.get("test-imports/module/utils.cmod"), 
            "// Compiled CHTL module");
        
        System.out.println("测试文件创建完成");
    }
    
    private static void testImportFeatures() {
        CompilationContext context = new CompilationContext();
        Path currentDir = Paths.get("test-imports");
        ImportManager importManager = new ImportManager(context, currentDir);
        
        System.out.println("\n测试资源导入:");
        
        // 测试HTML导入（有as）
        ImportStatementNode htmlImport = new ImportStatementNode(ImportStatementNode.ImportType.HTML);
        htmlImport.setFromPath("template.html");
        htmlImport.setAsName("MainTemplate");
        
        ImportManager.ImportResult result1 = importManager.processImport(htmlImport);
        System.out.println("  HTML导入(有as): " + (result1.isSuccess() ? "成功" : "失败"));
        if (result1.isSuccess() && result1.getNode() != null) {
            OriginBlockNode origin = (OriginBlockNode) result1.getNode();
            System.out.println("    创建原始嵌入节点: " + origin.getName());
        }
        
        // 测试CSS导入（无as）
        ImportStatementNode cssImport = new ImportStatementNode(ImportStatementNode.ImportType.STYLE);
        cssImport.setFromPath("styles.css");
        
        ImportManager.ImportResult result2 = importManager.processImport(cssImport);
        System.out.println("  CSS导入(无as): " + (result2.isSuccess() ? "成功 - " + result2.getMessage() : "失败"));
        
        // 测试JavaScript导入（有as）
        ImportStatementNode jsImport = new ImportStatementNode(ImportStatementNode.ImportType.JAVASCRIPT);
        jsImport.setFromPath("script");  // 不带扩展名
        jsImport.setAsName("TemplateScript");
        
        ImportManager.ImportResult result3 = importManager.processImport(jsImport);
        System.out.println("  JS导入(不带扩展名): " + (result3.isSuccess() ? "成功" : "失败"));
        
        System.out.println("\n测试CHTL模块导入:");
        
        // 测试CHTL导入（cmod优先）
        ImportStatementNode chtlImport = new ImportStatementNode(ImportStatementNode.ImportType.CHTL);
        chtlImport.setFromPath("utils");  // 不带扩展名
        
        ImportManager.ImportResult result4 = importManager.processImport(chtlImport);
        System.out.println("  CHTL导入(cmod优先): " + (result4.isSuccess() ? "成功" : "失败"));
        
        // 测试通配符导入
        ImportStatementNode wildcardImport = new ImportStatementNode(ImportStatementNode.ImportType.CHTL);
        wildcardImport.setFromPath("module/*");
        
        ImportManager.ImportResult result5 = importManager.processImport(wildcardImport);
        System.out.println("  通配符导入: " + (result5.isSuccess() ? "成功 - 导入 " + result5.getNodes().size() + " 个文件" : "失败"));
        
        // 测试循环依赖检测
        System.out.println("\n测试循环依赖检测:");
        ImportStatementNode circularImport = new ImportStatementNode(ImportStatementNode.ImportType.CHTL);
        circularImport.setFromPath("template.html"); // 模拟已导入的路径
        
        ImportManager.ImportResult result6 = importManager.processImport(htmlImport); // 重复导入
        System.out.println("  重复导入检测: " + (result6.isSuccess() ? "成功 - " + result6.getMessage() : "失败"));
        
        // 显示错误
        if (context.hasErrors()) {
            System.out.println("\n导入错误:");
            context.getErrors().forEach(error -> System.out.println("  " + error));
        }
    }
    
    private static void testNamespaceFeatures() {
        CompilationContext context = new CompilationContext();
        NamespaceManager namespaceManager = new NamespaceManager(context);
        
        // 创建测试命名空间
        String chtlCode1 = """
            [Namespace] MyApp {
                [Template] @Element Button {
                    button {
                        class: btn;
                    }
                }
                
                [Custom] @Style Theme {
                    color, background;
                }
            }
            """;
        
        String chtlCode2 = """
            [Namespace] MyApp {
                [Template] @Element Card {
                    div {
                        class: card;
                    }
                }
                
                [Template] @Element Button {
                    button {
                        class: btn-primary;
                    }
                }
            }
            """;
        
        // 解析CHTL代码
        CHTLParser parser = new CHTLParser();
        RootNode root1 = parser.parse(chtlCode1);
        RootNode root2 = parser.parse(chtlCode2);
        
        // 处理第一个命名空间
        System.out.println("处理第一个MyApp命名空间:");
        for (CHTLASTNode child : root1.getChildren()) {
            if (child instanceof NamespaceNode) {
                NamespaceNode ns = (NamespaceNode) child;
                namespaceManager.enterNamespace(ns);
                
                // 注册定义
                for (CHTLASTNode nsChild : ns.getChildren()) {
                    if (nsChild instanceof TemplateDefinitionNode) {
                        TemplateDefinitionNode template = (TemplateDefinitionNode) nsChild;
                        NamespaceManager.DefinitionType type = mapTemplateType(template.getTemplateType());
                        namespaceManager.registerDefinition("MyApp", template.getName(), template, type);
                        System.out.println("  注册模板: " + template.getName());
                    } else if (nsChild instanceof CustomDefinitionNode) {
                        CustomDefinitionNode custom = (CustomDefinitionNode) nsChild;
                        NamespaceManager.DefinitionType type = mapCustomType(custom.getCustomType());
                        namespaceManager.registerDefinition("MyApp", custom.getName(), custom, type);
                        System.out.println("  注册自定义: " + custom.getName());
                    }
                }
                
                namespaceManager.exitNamespace();
            }
        }
        
        // 处理第二个命名空间（同名）
        System.out.println("\n处理第二个MyApp命名空间（同名）:");
        for (CHTLASTNode child : root2.getChildren()) {
            if (child instanceof NamespaceNode) {
                NamespaceNode ns = (NamespaceNode) child;
                namespaceManager.enterNamespace(ns);
                
                // 注册定义
                for (CHTLASTNode nsChild : ns.getChildren()) {
                    if (nsChild instanceof TemplateDefinitionNode) {
                        TemplateDefinitionNode template = (TemplateDefinitionNode) nsChild;
                        NamespaceManager.DefinitionType type = mapTemplateType(template.getTemplateType());
                        namespaceManager.registerDefinition("MyApp", template.getName(), template, type);
                        System.out.println("  注册模板: " + template.getName());
                    }
                }
                
                namespaceManager.exitNamespace();
            }
        }
        
        // 合并命名空间
        System.out.println("\n合并同名命名空间:");
        namespaceManager.mergeAllNamespaces();
        
        // 查找定义
        System.out.println("\n查找定义:");
        CHTLASTNode foundButton = namespaceManager.findDefinition("Button", NamespaceManager.DefinitionType.TEMPLATE_ELEMENT);
        System.out.println("  查找Button模板: " + (foundButton != null ? "找到" : "未找到"));
        
        CHTLASTNode foundCard = namespaceManager.findDefinition("Card", NamespaceManager.DefinitionType.TEMPLATE_ELEMENT);
        System.out.println("  查找Card模板: " + (foundCard != null ? "找到" : "未找到"));
        
        CHTLASTNode foundTheme = namespaceManager.findDefinition("Theme", NamespaceManager.DefinitionType.CUSTOM_STYLE);
        System.out.println("  查找Theme自定义: " + (foundTheme != null ? "找到" : "未找到"));
        
        // 显示冲突
        if (context.hasErrors()) {
            System.out.println("\n命名空间冲突:");
            context.getErrors().forEach(error -> System.out.println("  " + error));
        }
        
        if (!context.getWarnings().isEmpty()) {
            System.out.println("\n命名空间警告:");
            context.getWarnings().forEach(warning -> System.out.println("  " + warning));
        }
        
        // 显示合并的命名空间
        System.out.println("\n合并后的命名空间:");
        for (NamespaceManager.MergedNamespace merged : namespaceManager.getAllNamespaces()) {
            System.out.println("  " + merged.getName() + " (包含 " + merged.getNodes().size() + " 个定义)");
        }
    }
    
    private static NamespaceManager.DefinitionType mapTemplateType(TemplateDefinitionNode.TemplateType type) {
        switch (type) {
            case STYLE:
                return NamespaceManager.DefinitionType.TEMPLATE_STYLE;
            case ELEMENT:
                return NamespaceManager.DefinitionType.TEMPLATE_ELEMENT;
            case VAR:
                return NamespaceManager.DefinitionType.TEMPLATE_VAR;
            default:
                throw new IllegalArgumentException("Unknown template type: " + type);
        }
    }
    
    private static NamespaceManager.DefinitionType mapCustomType(TemplateDefinitionNode.TemplateType type) {
        switch (type) {
            case STYLE:
                return NamespaceManager.DefinitionType.CUSTOM_STYLE;
            case ELEMENT:
                return NamespaceManager.DefinitionType.CUSTOM_ELEMENT;
            case VAR:
                return NamespaceManager.DefinitionType.CUSTOM_VAR;
            default:
                throw new IllegalArgumentException("Unknown custom type: " + type);
        }
    }
    
    private static void cleanupTestFiles() throws IOException {
        System.out.println("\n清理测试文件...");
        
        // 删除测试文件
        Files.deleteIfExists(Paths.get("test-imports/template.html"));
        Files.deleteIfExists(Paths.get("test-imports/styles.css"));
        Files.deleteIfExists(Paths.get("test-imports/script.js"));
        Files.deleteIfExists(Paths.get("test-imports/module/utils.chtl"));
        Files.deleteIfExists(Paths.get("test-imports/module/utils.cmod"));
        Files.deleteIfExists(Paths.get("test-imports/module"));
        Files.deleteIfExists(Paths.get("test-imports"));
        
        System.out.println("清理完成");
    }
}