import com.chtl.compiler.importer.ImportManager;
import com.chtl.compiler.namespace.NamespaceManager;
import com.chtl.context.*;
import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.parser.CHTLParser;
import com.chtl.generator.CHTLGenerator;

import java.nio.file.*;
import java.io.IOException;

/**
 * 综合测试增强的CHTL编译器功能
 * 包括：导入管理、命名空间合并、RAII状态管理
 */
public class TestEnhancedCompiler {
    public static void main(String[] args) throws IOException {
        System.out.println("=== 增强CHTL编译器综合测试 ===\n");
        
        // 创建测试环境
        setupTestEnvironment();
        
        // 编译示例
        compileExample();
        
        // 清理环境
        cleanupTestEnvironment();
    }
    
    private static void setupTestEnvironment() throws IOException {
        System.out.println("设置测试环境...");
        
        // 创建目录结构
        Files.createDirectories(Paths.get("test-project"));
        Files.createDirectories(Paths.get("test-project/module"));
        Files.createDirectories(Paths.get("test-project/assets"));
        
        // 创建共享样式文件
        Files.writeString(Paths.get("test-project/assets/theme.css"), """
            :root {
                --primary-color: #007bff;
                --secondary-color: #6c757d;
                --font-family: Arial, sans-serif;
            }
            
            body {
                font-family: var(--font-family);
                margin: 0;
                padding: 0;
            }
            """);
        
        // 创建共享脚本文件
        Files.writeString(Paths.get("test-project/assets/utils.js"), """
            const Utils = {
                formatDate: function(date) {
                    return new Intl.DateTimeFormat('zh-CN').format(date);
                },
                
                debounce: function(func, wait) {
                    let timeout;
                    return function(...args) {
                        clearTimeout(timeout);
                        timeout = setTimeout(() => func.apply(this, args), wait);
                    };
                }
            };
            """);
        
        // 创建CHTL模块文件
        Files.writeString(Paths.get("test-project/module/components.chtl"), """
            [Namespace] UI {
                [Template] @Element Button {
                    button {
                        class: btn;
                        @Var ButtonText;
                        
                        style {
                            padding: 10px 20px;
                            border: none;
                            border-radius: 4px;
                            cursor: pointer;
                            
                            &:hover {
                                opacity: 0.8;
                            }
                        }
                    }
                }
                
                [Template] @Style ButtonTheme {
                    background-color: var(--primary-color);
                    color: white;
                }
                
                [Custom] @Style CardStyle {
                    background-color, border-color, box-shadow;
                }
            }
            """);
        
        Files.writeString(Paths.get("test-project/module/layout.chtl"), """
            [Namespace] UI {
                [Template] @Element Container {
                    div {
                        class: container;
                        
                        style {
                            max-width: 1200px;
                            margin: 0 auto;
                            padding: 0 15px;
                        }
                    }
                }
                
                [Template] @Element Grid {
                    div {
                        class: grid;
                        
                        style {
                            display: grid;
                            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
                            gap: 20px;
                        }
                    }
                }
            }
            """);
        
        System.out.println("测试环境设置完成\n");
    }
    
    private static void compileExample() throws IOException {
        System.out.println("编译CHTL项目...\n");
        
        // 主CHTL文件内容
        String mainChtl = """
            // 导入资源
            [Import] @Style from assets/theme.css as GlobalTheme;
            [Import] @JavaScript from assets/utils as UtilsLib;
            
            // 导入CHTL模块
            [Import] @Chtl from module/*;
            
            // 使用导入的命名空间
            div {
                id: app;
                
                @Element UI.Container {
                    h1 {
                        text {
                            "增强CHTL编译器演示"
                        }
                        
                        style {
                            color: var(--primary-color);
                            text-align: center;
                        }
                    }
                    
                    @Element UI.Grid {
                        div {
                            class: card;
                            
                            style {
                                @Style UI.CardStyle {
                                    background-color: white;
                                    border-color: #dee2e6;
                                    box-shadow: 0 2px 4px rgba(0,0,0,0.1);
                                }
                                
                                padding: 20px;
                                border-radius: 8px;
                            }
                            
                            h3 {
                                text { "功能卡片" }
                            }
                            
                            @Element UI.Button {
                                @Var ButtonText: "点击测试";
                                
                                style {
                                    @Style UI.ButtonTheme;
                                }
                                
                                script {
                                    {{button}}->listen({
                                        click: function() {
                                            console.log('Button clicked at:', UtilsLib.formatDate(new Date()));
                                        }
                                    });
                                }
                            }
                        }
                    }
                }
                
                // 原始嵌入块
                [Origin] @Style GlobalTheme;
                [Origin] @JavaScript UtilsLib;
            }
            """;
        
        // 创建编译上下文
        CompilationContext context = new CompilationContext();
        Path currentDir = Paths.get("test-project");
        
        // 创建管理器
        ImportManager importManager = new ImportManager(context, currentDir);
        NamespaceManager namespaceManager = new NamespaceManager(context);
        
        // 使用RAII管理编译状态
        try (var compilationGuard = context.enterState(CompilationContext.State.ROOT)) {
            System.out.println("1. 解析主文件...");
            CHTLParser parser = new CHTLParser();
            RootNode ast = parser.parse(mainChtl);
            
            if (!parser.getErrors().isEmpty()) {
                System.err.println("解析错误:");
                parser.getErrors().forEach(err -> System.err.println("  " + err));
                return;
            }
            System.out.println("   解析成功");
            
            // 处理导入
            System.out.println("\n2. 处理导入...");
            processImports(ast, importManager, context);
            
            // 处理命名空间
            System.out.println("\n3. 处理命名空间...");
            processNamespaces(ast, namespaceManager, context);
            
            // 合并命名空间
            System.out.println("\n4. 合并命名空间...");
            namespaceManager.mergeAllNamespaces();
            System.out.println("   合并完成");
            
            // 生成代码
            System.out.println("\n5. 生成代码...");
            CHTLGenerator generator = new CHTLGenerator();
            CHTLGenerator.GenerationResult result = generator.generate(ast);
            
            // 保存输出
            Files.writeString(Paths.get("test-project/output.html"), 
                buildHtmlDocument(result));
            System.out.println("   生成完成: test-project/output.html");
            
            // 显示统计
            System.out.println("\n6. 编译统计:");
            System.out.println("   HTML行数: " + countLines(result.getHtml()));
            System.out.println("   CSS行数: " + countLines(result.getCss()));
            System.out.println("   JavaScript行数: " + countLines(result.getJavascript()));
            System.out.println("   错误数: " + context.getErrors().size());
            System.out.println("   警告数: " + context.getWarnings().size());
            
            if (context.hasErrors()) {
                System.err.println("\n编译错误:");
                context.getErrors().forEach(err -> System.err.println("  " + err));
            }
            
            if (!context.getWarnings().isEmpty()) {
                System.out.println("\n编译警告:");
                context.getWarnings().forEach(warn -> System.out.println("  " + warn));
            }
        }
        
        System.out.println("\n编译完成！");
    }
    
    private static void processImports(CHTLASTNode node, ImportManager importManager, 
                                      CompilationContext context) {
        if (node instanceof ImportStatementNode) {
            ImportStatementNode importNode = (ImportStatementNode) node;
            
            try (var importGuard = context.enterState(CompilationContext.State.IMPORT_STATEMENT)) {
                ImportManager.ImportResult result = importManager.processImport(importNode);
                
                if (result.isSuccess()) {
                    System.out.println("   导入成功: " + importNode.getFromPath());
                    
                    // 处理导入的节点
                    if (result.getNode() != null) {
                        // 单个节点
                        processImportedNode(result.getNode(), context);
                    } else if (!result.getNodes().isEmpty()) {
                        // 多个节点
                        for (CHTLASTNode imported : result.getNodes()) {
                            processImportedNode(imported, context);
                        }
                    }
                } else {
                    System.err.println("   导入失败: " + importNode.getFromPath());
                }
            }
        }
        
        // 递归处理子节点
        for (CHTLASTNode child : node.getChildren()) {
            processImports(child, importManager, context);
        }
    }
    
    private static void processImportedNode(CHTLASTNode node, CompilationContext context) {
        if (node instanceof RootNode) {
            // 递归处理导入文件的内容
            for (CHTLASTNode child : node.getChildren()) {
                processImportedNode(child, context);
            }
        } else if (node instanceof NamespaceNode) {
            // 处理导入的命名空间
            NamespaceNode ns = (NamespaceNode) node;
            System.out.println("     处理命名空间: " + ns.getName());
        } else if (node instanceof OriginBlockNode) {
            // 处理原始嵌入块
            OriginBlockNode origin = (OriginBlockNode) node;
            System.out.println("     创建原始嵌入: " + origin.getName());
        }
    }
    
    private static void processNamespaces(CHTLASTNode node, NamespaceManager namespaceManager,
                                         CompilationContext context) {
        if (node instanceof NamespaceNode) {
            NamespaceNode ns = (NamespaceNode) node;
            
            try (var nsGuard = context.enterNamespace(ns.getName())) {
                namespaceManager.enterNamespace(ns);
                
                // 注册命名空间内的定义
                for (CHTLASTNode child : ns.getChildren()) {
                    registerDefinition(child, ns.getName(), namespaceManager);
                }
                
                namespaceManager.exitNamespace();
            }
        }
        
        // 递归处理子节点
        for (CHTLASTNode child : node.getChildren()) {
            processNamespaces(child, namespaceManager, context);
        }
    }
    
    private static void registerDefinition(CHTLASTNode node, String namespaceName,
                                          NamespaceManager namespaceManager) {
        if (node instanceof TemplateDefinitionNode) {
            TemplateDefinitionNode template = (TemplateDefinitionNode) node;
            NamespaceManager.DefinitionType type = mapTemplateType(template.getTemplateType());
            namespaceManager.registerDefinition(namespaceName, template.getName(), template, type);
        } else if (node instanceof CustomDefinitionNode) {
            CustomDefinitionNode custom = (CustomDefinitionNode) node;
            NamespaceManager.DefinitionType type = mapCustomType(custom.getCustomType());
            namespaceManager.registerDefinition(namespaceName, custom.getName(), custom, type);
        }
    }
    
    private static String buildHtmlDocument(CHTLGenerator.GenerationResult result) {
        StringBuilder html = new StringBuilder();
        html.append("<!DOCTYPE html>\n");
        html.append("<html lang=\"zh-CN\">\n");
        html.append("<head>\n");
        html.append("    <meta charset=\"UTF-8\">\n");
        html.append("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
        html.append("    <title>增强CHTL编译器演示</title>\n");
        
        if (!result.getCss().isEmpty()) {
            html.append("    <style>\n");
            html.append(result.getCss());
            html.append("    </style>\n");
        }
        
        html.append("</head>\n");
        html.append("<body>\n");
        html.append(result.getHtml());
        
        if (!result.getJavascript().isEmpty()) {
            html.append("    <script>\n");
            html.append(result.getJavascript());
            html.append("    </script>\n");
        }
        
        html.append("</body>\n");
        html.append("</html>\n");
        
        return html.toString();
    }
    
    private static void cleanupTestEnvironment() throws IOException {
        System.out.println("\n清理测试环境...");
        
        // 保留output.html，删除其他测试文件
        Files.deleteIfExists(Paths.get("test-project/assets/theme.css"));
        Files.deleteIfExists(Paths.get("test-project/assets/utils.js"));
        Files.deleteIfExists(Paths.get("test-project/assets"));
        Files.deleteIfExists(Paths.get("test-project/module/components.chtl"));
        Files.deleteIfExists(Paths.get("test-project/module/layout.chtl"));
        Files.deleteIfExists(Paths.get("test-project/module"));
        
        System.out.println("清理完成（保留output.html）");
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
    
    private static int countLines(String text) {
        return text.isEmpty() ? 0 : text.split("\n").length;
    }
}