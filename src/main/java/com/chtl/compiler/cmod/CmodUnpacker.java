package com.chtl.compiler.cmod;
import java.io.IOException;
import java.io.InputStream;
import java.io.Serializable;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import com.chtl.ast.node.RootNode;
import com.chtl.ast.*;
import com.chtl.context.CompilationContext;

/**
 * Cmod解包器
 * 解包Cmod格式文件
 * 严格按照CHTL语法文档实现
 */
public class CmodUnpacker {
    private final CompilationContext context;
    
    public CmodUnpacker(CompilationContext context) {
        this.context = context;
    }
    
    /**
     * 解包Cmod文件
     */
    public UnpackResult unpackCmod(Path cmodFile, Path targetDirectory) throws IOException {
        if (!Files.exists(cmodFile)) {
            throw new FileNotFoundException("Cmod文件不存在: " + cmodFile);
        }
        
        // 创建目标目录
        Files.createDirectories(targetDirectory);
        
        // 解压文件
        SimpleZip.unzipFile(cmodFile, targetDirectory);
        
        // 读取manifest
        Path manifestPath = targetDirectory.resolve(CmodFormat.META_DIR)
                                          .resolve(CmodFormat.MANIFEST_FILE);
        Properties manifest = loadManifest(manifestPath);
        
        // 读取编译数据
        Path compiledDir = targetDirectory.resolve(CmodFormat.COMPILED_DIR);
        List<CompiledModule> modules = loadCompiledModules(compiledDir);
        
        // 创建解包结果
        UnpackResult result = new UnpackResult(
            manifest.getProperty(CmodFormat.MANIFEST_NAME),
            manifest.getProperty(CmodFormat.MANIFEST_NAMESPACE),
            manifest.getProperty(CmodFormat.MANIFEST_VERSION),
            modules
        );
        
        // 读取源文件
        Path srcDir = targetDirectory.resolve(CmodFormat.SOURCE_DIR);
        if (Files.exists(srcDir)) {
            result.setSourceFiles(collectFiles(srcDir, ".chtl"));
        }
        
        // 读取资源文件
        Path resourcesDir = targetDirectory.resolve(CmodFormat.RESOURCES_DIR);
        if (Files.exists(resourcesDir)) {
            result.setResourceFiles(collectAllFiles(resourcesDir));
        }
        
        return result;
    }
    
    /**
     * 快速读取Cmod信息（不完全解包）
     */
    public CmodInfo readCmodInfo(Path cmodFile) throws IOException {
        // 读取manifest而不解包整个文件
        byte[] manifestData = SimpleZip.readEntry(cmodFile, 
            CmodFormat.META_DIR + "/" + CmodFormat.MANIFEST_FILE);
        
        Properties manifest = new Properties();
        manifest.load(new ByteArrayInputStream(manifestData));
        
        return new CmodInfo(
            manifest.getProperty(CmodFormat.MANIFEST_NAME),
            manifest.getProperty(CmodFormat.MANIFEST_NAMESPACE),
            manifest.getProperty(CmodFormat.MANIFEST_VERSION),
            manifest.getProperty(CmodFormat.MANIFEST_EXPORTS),
            manifest.getProperty(CmodFormat.MANIFEST_DEPENDENCIES),
            manifest.getProperty(CmodFormat.MANIFEST_CREATED),
            manifest.getProperty(CmodFormat.MANIFEST_COMPILER_VERSION)
        );
    }
    
    /**
     * 从Cmod中提取特定文件
     */
    public byte[] extractFile(Path cmodFile, String entryPath) throws IOException {
        return SimpleZip.readEntry(cmodFile, entryPath);
    }
    
    /**
     * 加载manifest文件
     */
    private Properties loadManifest(Path manifestPath) throws IOException {
        Properties manifest = new Properties();
        try (InputStream in = Files.newInputStream(manifestPath)) {
            manifest.load(in);
        }
        return manifest;
    }
    
    /**
     * 加载编译模块
     */
    @SuppressWarnings("unchecked")
    private List<CompiledModule> loadCompiledModules(Path compiledDir) throws IOException {
        Path astFile = compiledDir.resolve("ast.dat");
        
        if (!Files.exists(astFile)) {
            return Collections.emptyList();
        }
        
        try (ObjectInputStream ois = new ObjectInputStream(
                new BufferedInputStream(Files.newInputStream(astFile)))) {
            return (List<CompiledModule>) ois.readObject();
        } catch (ClassNotFoundException e) {
            throw new IOException("无法加载编译数据", e);
        }
    }
    
    /**
     * 收集指定扩展名的文件
     */
    private List<Path> collectFiles(Path directory, String extension) throws IOException {
        List<Path> files = new ArrayList<>();
        
        Files.walk(directory)
            .filter(Files::isRegularFile)
            .filter(path -> path.toString().endsWith(extension))
            .forEach(files::add);
        
        return files;
    }
    
    /**
     * 收集所有文件
     */
    private List<Path> collectAllFiles(Path directory) throws IOException {
        List<Path> files = new ArrayList<>();
        
        Files.walk(directory)
            .filter(Files::isRegularFile)
            .forEach(files::add);
        
        return files;
    }
    
    /**
     * 解包结果
     */
    public static class UnpackResult {
        private final String moduleName;
        private final String moduleNamespace;
        private final String version;
        private final List<CompiledModule> compiledModules;
        private List<Path> sourceFiles;
        private List<Path> resourceFiles;
        
        public UnpackResult(String moduleName, String moduleNamespace, 
                           String version, List<CompiledModule> compiledModules) {
            this.moduleName = moduleName;
            this.moduleNamespace = moduleNamespace;
            this.version = version;
            this.compiledModules = compiledModules;
            this.sourceFiles = new ArrayList<>();
            this.resourceFiles = new ArrayList<>();
        }
        
        public String getModuleName() {
            return moduleName;
        }
        
        public String getModuleNamespace() {
            return moduleNamespace;
        }
        
        public String getVersion() {
            return version;
        }
        
        public List<CompiledModule> getCompiledModules() {
            return compiledModules;
        }
        
        public List<Path> getSourceFiles() {
            return sourceFiles;
        }
        
        public void setSourceFiles(List<Path> sourceFiles) {
            this.sourceFiles = sourceFiles;
        }
        
        public List<Path> getResourceFiles() {
            return resourceFiles;
        }
        
        public void setResourceFiles(List<Path> resourceFiles) {
            this.resourceFiles = resourceFiles;
        }
        
        public List<RootNode> getASTNodes() {
            List<RootNode> nodes = new ArrayList<>();
            for (CompiledModule module : compiledModules) {
                nodes.add(module.getAst());
            }
            return nodes;
        }
    }
    
    /**
     * Cmod信息（轻量级）
     */
    public static class CmodInfo {
        private final String name;
        private final String namespace;
        private final String version;
        private final String exports;
        private final String dependencies;
        private final String created;
        private final String compilerVersion;
        
        public CmodInfo(String name, String namespace, String version,
                       String exports, String dependencies, 
                       String created, String compilerVersion) {
            this.name = name;
            this.namespace = namespace;
            this.version = version;
            this.exports = exports;
            this.dependencies = dependencies;
            this.created = created;
            this.compilerVersion = compilerVersion;
        }
        
        public String getName() {
            return name;
        }
        
        public String getNamespace() {
            return namespace;
        }
        
        public String getVersion() {
            return version;
        }
        
        public String getExports() {
            return exports;
        }
        
        public String getDependencies() {
            return dependencies;
        }
        
        public String getCreated() {
            return created;
        }
        
        public String getCompilerVersion() {
            return compilerVersion;
        }
        
        @Override
        public String toString() {
            return String.format("CmodInfo{name='%s', namespace='%s', version='%s', exports='%s'}",
                name, namespace, version, exports);
        }
    }
    
    /**
     * 编译模块（需要与CmodPackager中的定义匹配）
     */
    public static class CompiledModule implements Serializable {
        private static final long serialVersionUID = 1L;
        
        private final String sourcePath;
        private final RootNode ast;
        private final CmodPackager.SymbolTable symbols;
        
        public CompiledModule(String sourcePath, RootNode ast, CmodPackager.SymbolTable symbols) {
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
        
        public CmodPackager.SymbolTable getSymbols() {
            return symbols;
        }
    }
}
