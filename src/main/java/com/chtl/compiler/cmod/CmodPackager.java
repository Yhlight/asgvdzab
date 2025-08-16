package com.chtl.compiler.cmod;
import java.io.IOException;
import java.io.OutputStream;
import java.io.Serializable;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;
import com.chtl.ast.node.CommentNode;
import com.chtl.ast.node.ConfigurationNode;
import com.chtl.ast.node.ConstraintNode;
import com.chtl.ast.node.CustomDefinitionNode;
import com.chtl.ast.node.CustomOperationNode;
import com.chtl.ast.node.CustomUsageNode;
import com.chtl.ast.node.ImportStatementNode;
import com.chtl.ast.node.InheritStatementNode;
import com.chtl.ast.node.NamespaceNode;
import com.chtl.ast.node.NumberLiteralNode;
import com.chtl.ast.node.OriginBlockNode;
import com.chtl.ast.node.RootNode;
import com.chtl.ast.node.ScriptBlockNode;
import com.chtl.ast.node.StringLiteralNode;
import com.chtl.ast.node.StyleBlockNode;
import com.chtl.ast.node.StylePropertyNode;
import com.chtl.ast.node.StyleRuleNode;
import com.chtl.ast.node.StyleSelectorNode;
import com.chtl.ast.node.TemplateDefinitionNode;
import com.chtl.ast.node.TemplateUsageNode;
import com.chtl.ast.node.UnquotedLiteralNode;
import com.chtl.ast.node.VarGroupDefinitionNode;
import com.chtl.ast.node.VarGroupUsageNode;
import com.chtl.compiler.chtl.CHTLParser;
import com.chtl.compiler.cmod.CmodUnpacker.CompiledModule;
import com.chtl.model.AttributeNode;
import com.chtl.model.ElementNode;
import com.chtl.model.TextNode;
import com.chtl.ast.*;
import com.chtl.context.CompilationContext;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

/**
 * Cmod打包器
 * 将CHTL源文件打包成Cmod格式
 * 严格按照CHTL语法文档实现
 */
public class CmodPackager {
    private final CompilationContext context;
    private final Path sourceDirectory;
    private final String moduleName;
    private final String moduleNamespace;
    
    public CmodPackager(CompilationContext context, Path sourceDirectory, 
                       String moduleName, String moduleNamespace) {
        this.context = context;
        this.sourceDirectory = sourceDirectory;
        this.moduleName = moduleName;
        this.moduleNamespace = moduleNamespace;
    }
    
    /**
     * 打包Cmod文件
     */
    public void packageCmod(Path outputFile) throws IOException {
        // 创建临时目录
        Path tempDir = Files.createTempDirectory("cmod_pack_");
        
        try {
            // 创建目录结构
            Path metaDir = tempDir.resolve(CmodFormat.META_DIR);
            Path srcDir = tempDir.resolve(CmodFormat.SOURCE_DIR);
            Path compiledDir = tempDir.resolve(CmodFormat.COMPILED_DIR);
            Path resourcesDir = tempDir.resolve(CmodFormat.RESOURCES_DIR);
            
            Files.createDirectories(metaDir);
            Files.createDirectories(srcDir);
            Files.createDirectories(compiledDir);
            Files.createDirectories(resourcesDir);
            
            // 收集源文件
            List<Path> sourceFiles = collectSourceFiles();
            
            // 复制源文件
            for (Path sourceFile : sourceFiles) {
                Path relativePath = sourceDirectory.relativize(sourceFile);
                Path targetPath = srcDir.resolve(relativePath);
                Files.createDirectories(targetPath.getParent());
                Files.copy(sourceFile, targetPath);
            }
            
            // 编译源文件并生成AST
            List<CompiledModule> compiledModules = compileSourceFiles(sourceFiles);
            
            // 序列化AST和符号表
            serializeCompiledData(compiledDir, compiledModules);
            
            // 收集并复制资源文件
            collectAndCopyResources(resourcesDir);
            
            // 创建manifest文件
            createManifest(metaDir, compiledModules);
            
            // 打包为zip文件
            SimpleZip.zipFolder(tempDir, outputFile);
            
            System.out.println("Cmod打包完成: " + outputFile);
            
        } finally {
            // 清理临时目录
            deleteDirectory(tempDir);
        }
    }
    
    /**
     * 收集源文件
     */
    private List<Path> collectSourceFiles() throws IOException {
        List<Path> sourceFiles = new ArrayList<>();
        
        Files.walk(sourceDirectory)
            .filter(Files::isRegularFile)
            .filter(path -> path.toString().endsWith(".chtl"))
            .forEach(sourceFiles::add);
        
        return sourceFiles;
    }
    
    /**
     * 编译源文件
     */
    private List<CompiledModule> compileSourceFiles(List<Path> sourceFiles) throws IOException {
        List<CompiledModule> compiledModules = new ArrayList<>();
        
        for (Path sourceFile : sourceFiles) {
            String content = Files.readString(sourceFile);
            
            // 解析CHTL文件
            CHTLParser parser = new CHTLParser();
            RootNode ast = parser.parse(content);
            
            if (!parser.getErrors().isEmpty()) {
                throw new IOException("编译错误 in " + sourceFile + ": " + 
                    String.join(", ", parser.getErrors()));
            }
            
            // 提取符号信息
            SymbolTable symbols = extractSymbols(ast);
            
            // 创建编译模块
            CompiledModule module = new CompiledModule(
                sourceDirectory.relativize(sourceFile).toString(),
                ast,
                symbols
            );
            
            compiledModules.add(module);
        }
        
        return compiledModules;
    }
    
    /**
     * 提取符号表
     */
    private SymbolTable extractSymbols(RootNode ast) {
        SymbolTable symbols = new SymbolTable();
        
        // 遍历AST提取符号
        ast.accept(new CHTLASTVisitor() {
            @Override
            public void visitTemplateDefinition(TemplateDefinitionNode node) {
                symbols.addTemplate(node.getName(), node.getTemplateType());
                visitChildren(node);
            }
            
            @Override
            public void visitCustomDefinition(CustomDefinitionNode node) {
                symbols.addCustom(node.getName(), node.getCustomType());
                visitChildren(node);
            }
            
            @Override
            public void visitNamespace(NamespaceNode node) {
                symbols.addNamespace(node.getName());
                visitChildren(node);
            }
            
            @Override
            public void visitVarGroupDefinition(VarGroupDefinitionNode node) {
                symbols.addVarGroup(node.getName());
                visitChildren(node);
            }
            
            // 实现其他访问方法...
            @Override
            public void visitRoot(RootNode node) { visitChildren(node); }
            @Override
            public void visitElement(ElementNode node) { visitChildren(node); }
            @Override
            public void visitText(TextNode node) { }
            @Override
            public void visitAttribute(AttributeNode node) { visitChildren(node); }
            @Override
            public void visitComment(CommentNode node) { }
            @Override
            public void visitStyleBlock(StyleBlockNode node) { visitChildren(node); }
            @Override
            public void visitStyleRule(StyleRuleNode node) { visitChildren(node); }
            @Override
            public void visitStyleSelector(StyleSelectorNode node) { }
            @Override
            public void visitStyleProperty(StylePropertyNode node) { visitChildren(node); }
            @Override
            public void visitScriptBlock(ScriptBlockNode node) { }
            @Override
            public void visitTemplateUsage(TemplateUsageNode node) { visitChildren(node); }
            @Override
            public void visitCustomUsage(CustomUsageNode node) { visitChildren(node); }
            @Override
            public void visitCustomOperation(CustomOperationNode node) { visitChildren(node); }
            @Override
            public void visitOriginBlock(OriginBlockNode node) { }
            @Override
            public void visitImportStatement(ImportStatementNode node) { }
            @Override
            public void visitConstraint(ConstraintNode node) { }
            @Override
            public void visitConfiguration(ConfigurationNode node) { }
            @Override
            public void visitVarGroupUsage(VarGroupUsageNode node) { }
            @Override
            public void visitInheritStatement(InheritStatementNode node) { }
            @Override
            public void visitStringLiteral(StringLiteralNode node) { }
            @Override
            public void visitUnquotedLiteral(UnquotedLiteralNode node) { }
            @Override
            public void visitNumberLiteral(NumberLiteralNode node) { }
            
            private void visitChildren(CHTLASTNode node) {
                for (CHTLASTNode child : node.getChildren()) {
                    child.accept(this);
                }
            }
        });
        
        return symbols;
    }
    
    /**
     * 序列化编译数据
     */
    private void serializeCompiledData(Path compiledDir, List<CompiledModule> modules) throws IOException {
        // 序列化AST
        Path astFile = compiledDir.resolve("ast.dat");
        try (ObjectOutputStream oos = new ObjectOutputStream(
                new BufferedOutputStream(Files.newOutputStream(astFile)))) {
            oos.writeObject(modules);
        }
        
        // 序列化符号表
        Path symbolsFile = compiledDir.resolve("symbols.dat");
        List<SymbolTable> symbolTables = new ArrayList<>();
        for (CompiledModule module : modules) {
            symbolTables.add(module.getSymbols());
        }
        
        try (ObjectOutputStream oos = new ObjectOutputStream(
                new BufferedOutputStream(Files.newOutputStream(symbolsFile)))) {
            oos.writeObject(symbolTables);
        }
        
        // 序列化元数据
        Path metadataFile = compiledDir.resolve("metadata.dat");
        Map<String, Object> metadata = new HashMap<>();
        metadata.put("moduleCount", modules.size());
        metadata.put("compileTime", LocalDateTime.now().toString());
        metadata.put("compilerVersion", CmodFormat.COMPILER_VERSION);
        
        try (ObjectOutputStream oos = new ObjectOutputStream(
                new BufferedOutputStream(Files.newOutputStream(metadataFile)))) {
            oos.writeObject(metadata);
        }
    }
    
    /**
     * 收集并复制资源文件
     */
    private void collectAndCopyResources(Path resourcesDir) throws IOException {
        // 查找资源文件（CSS、JS、HTML等）
        Files.walk(sourceDirectory)
            .filter(Files::isRegularFile)
            .filter(path -> {
                String name = path.toString().toLowerCase();
                return name.endsWith(".css") || name.endsWith(".js") || 
                       name.endsWith(".html") || name.endsWith(".htm");
            })
            .forEach(path -> {
                try {
                    Path relativePath = sourceDirectory.relativize(path);
                    Path targetPath = resourcesDir.resolve(relativePath);
                    Files.createDirectories(targetPath.getParent());
                    Files.copy(path, targetPath);
                } catch (IOException e) {
                    throw new UncheckedIOException(e);
                }
            });
    }
    
    /**
     * 创建manifest文件
     */
    private void createManifest(Path metaDir, List<CompiledModule> modules) throws IOException {
        Path manifestFile = metaDir.resolve(CmodFormat.MANIFEST_FILE);
        
        Properties manifest = new Properties();
        manifest.setProperty(CmodFormat.MANIFEST_VERSION, CmodFormat.CMOD_VERSION);
        manifest.setProperty(CmodFormat.MANIFEST_NAME, moduleName);
        manifest.setProperty(CmodFormat.MANIFEST_NAMESPACE, moduleNamespace);
        manifest.setProperty(CmodFormat.MANIFEST_COMPILER_VERSION, CmodFormat.COMPILER_VERSION);
        manifest.setProperty(CmodFormat.MANIFEST_CREATED, 
            LocalDateTime.now().format(DateTimeFormatter.ISO_LOCAL_DATE_TIME));
        
        // 收集导出的符号
        List<String> exports = new ArrayList<>();
        for (CompiledModule module : modules) {
            exports.addAll(module.getSymbols().getExportedSymbols());
        }
        manifest.setProperty(CmodFormat.MANIFEST_EXPORTS, String.join(",", exports));
        
        // 写入manifest文件
        try (OutputStream out = Files.newOutputStream(manifestFile)) {
            manifest.store(out, "CHTL Module Manifest");
        }
    }
    
    /**
     * 删除目录
     */
    private void deleteDirectory(Path directory) throws IOException {
        Files.walk(directory)
            .sorted(Comparator.reverseOrder())
            .map(Path::toFile)
            .forEach(File::delete);
    }
    
    /**
     * 编译模块
     */
    private static class CompiledModule implements Serializable {
        private static final long serialVersionUID = 1L;
        
        private final String sourcePath;
        private final RootNode ast;
        private final SymbolTable symbols;
        
        public CompiledModule(String sourcePath, RootNode ast, SymbolTable symbols) {
            this.sourcePath = sourcePath;
            this.ast = ast;
            this.symbols = symbols;
        }
        
        public String getSourcePath() {
            return sourcePath;
        }
        
        public RootNode getAst() {
            return ast;
        }
        
        public SymbolTable getSymbols() {
            return symbols;
        }
    }
    
    /**
     * 符号表
     */
    public static class SymbolTable implements Serializable {
        private static final long serialVersionUID = 1L;
        
        private final Map<String, SymbolInfo> symbols = new HashMap<>();
        private final Set<String> namespaces = new HashSet<>();
        
        public void addTemplate(String name, TemplateDefinitionNode.TemplateType type) {
            symbols.put(name, new SymbolInfo(name, SymbolType.TEMPLATE, type.toString()));
        }
        
        public void addCustom(String name, TemplateDefinitionNode.TemplateType type) {
            symbols.put(name, new SymbolInfo(name, SymbolType.CUSTOM, type.toString()));
        }
        
        public void addVarGroup(String name) {
            symbols.put(name, new SymbolInfo(name, SymbolType.VAR_GROUP, null));
        }
        
        public void addNamespace(String name) {
            namespaces.add(name);
        }
        
        public List<String> getExportedSymbols() {
            List<String> exports = new ArrayList<>();
            for (SymbolInfo symbol : symbols.values()) {
                exports.add(symbol.name);
            }
            return exports;
        }
        
        public enum SymbolType {
            TEMPLATE, CUSTOM, VAR_GROUP, FUNCTION, VARIABLE
        }
        
        public static class SymbolInfo implements Serializable {
            private static final long serialVersionUID = 1L;
            
            private final String name;
            private final SymbolType type;
            private final String subType;
            
            public SymbolInfo(String name, SymbolType type, String subType) {
                this.name = name;
                this.type = type;
                this.subType = subType;
            }
        }
    }
}
