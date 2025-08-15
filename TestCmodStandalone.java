import com.chtl.compiler.cmod.*;
import com.chtl.context.CompilationContext;
import com.chtl.ast.node.*;
import com.chtl.ast.*;

import java.nio.file.*;
import java.io.IOException;
import java.util.*;

/**
 * 独立的Cmod功能测试
 * 不依赖完整编译器，直接使用Cmod组件
 */
public class TestCmodStandalone {
    public static void main(String[] args) throws IOException {
        System.out.println("=== 独立Cmod功能测试 ===\n");
        
        // 测试Zip功能
        System.out.println("1. 测试Zip功能...");
        testZipFunctionality();
        
        // 测试Cmod格式
        System.out.println("\n2. 测试Cmod格式...");
        testCmodFormat();
        
        // 测试打包功能（使用模拟数据）
        System.out.println("\n3. 测试Cmod打包（模拟数据）...");
        testCmodPackaging();
        
        // 清理
        cleanup();
        
        System.out.println("\n测试完成！");
    }
    
    private static void testZipFunctionality() throws IOException {
        // 创建测试目录和文件
        Path testDir = Paths.get("test-zip");
        Files.createDirectories(testDir);
        Files.writeString(testDir.resolve("file1.txt"), "Hello World");
        Files.writeString(testDir.resolve("file2.txt"), "Test Content");
        
        Path subDir = testDir.resolve("subdir");
        Files.createDirectories(subDir);
        Files.writeString(subDir.resolve("file3.txt"), "Nested File");
        
        // 测试打包
        Path zipFile = Paths.get("test.zip");
        SimpleZip.zipFolder(testDir, zipFile);
        System.out.println("  创建zip文件: " + zipFile);
        System.out.println("  文件大小: " + Files.size(zipFile) + " 字节");
        
        // 测试解包
        Path unzipDir = Paths.get("test-unzip");
        SimpleZip.unzipFile(zipFile, unzipDir);
        System.out.println("  解压到: " + unzipDir);
        
        // 验证解压内容
        boolean file1Exists = Files.exists(unzipDir.resolve("file1.txt"));
        boolean file3Exists = Files.exists(unzipDir.resolve("subdir/file3.txt"));
        System.out.println("  验证文件: " + (file1Exists && file3Exists ? "成功" : "失败"));
        
        // 测试读取条目
        String content = new String(SimpleZip.readEntry(zipFile, "file1.txt"));
        System.out.println("  读取条目内容: " + content);
        
        // 清理
        deleteDirectory(testDir);
        deleteDirectory(unzipDir);
        Files.deleteIfExists(zipFile);
    }
    
    private static void testCmodFormat() {
        System.out.println("  Cmod版本: " + CmodFormat.CMOD_VERSION);
        System.out.println("  编译器版本: " + CmodFormat.COMPILER_VERSION);
        System.out.println("  Manifest文件: " + CmodFormat.MANIFEST_FILE);
        System.out.println("  元信息目录: " + CmodFormat.META_DIR);
        System.out.println("  源代码目录: " + CmodFormat.SOURCE_DIR);
        System.out.println("  编译目录: " + CmodFormat.COMPILED_DIR);
        System.out.println("  资源目录: " + CmodFormat.RESOURCES_DIR);
    }
    
    private static void testCmodPackaging() throws IOException {
        // 创建测试项目
        Path projectDir = Paths.get("test-cmod-pack");
        Files.createDirectories(projectDir);
        
        // 创建模拟的CHTL文件
        String chtlContent = """
            div {
                id: test;
                text { "Test" }
            }
            """;
        Files.writeString(projectDir.resolve("test.chtl"), chtlContent);
        
        // 创建编译上下文
        CompilationContext context = new CompilationContext();
        
        // 手动创建Cmod结构（绕过解析器）
        Path tempDir = Files.createTempDirectory("manual-cmod");
        try {
            // 创建目录结构
            Path metaDir = tempDir.resolve(CmodFormat.META_DIR);
            Path srcDir = tempDir.resolve(CmodFormat.SOURCE_DIR);
            Path compiledDir = tempDir.resolve(CmodFormat.COMPILED_DIR);
            
            Files.createDirectories(metaDir);
            Files.createDirectories(srcDir);
            Files.createDirectories(compiledDir);
            
            // 复制源文件
            Files.copy(projectDir.resolve("test.chtl"), srcDir.resolve("test.chtl"));
            
            // 创建manifest
            Properties manifest = new Properties();
            manifest.setProperty(CmodFormat.MANIFEST_VERSION, CmodFormat.CMOD_VERSION);
            manifest.setProperty(CmodFormat.MANIFEST_NAME, "TestModule");
            manifest.setProperty(CmodFormat.MANIFEST_NAMESPACE, "Test");
            manifest.setProperty(CmodFormat.MANIFEST_COMPILER_VERSION, CmodFormat.COMPILER_VERSION);
            manifest.setProperty(CmodFormat.MANIFEST_CREATED, java.time.LocalDateTime.now().toString());
            manifest.setProperty(CmodFormat.MANIFEST_EXPORTS, "TestSymbol1,TestSymbol2");
            
            Path manifestFile = metaDir.resolve(CmodFormat.MANIFEST_FILE);
            try (var out = Files.newOutputStream(manifestFile)) {
                manifest.store(out, "Test CHTL Module");
            }
            
            // 创建模拟的编译数据
            Map<String, Object> metadata = new HashMap<>();
            metadata.put("test", "data");
            metadata.put("version", 1);
            
            Path metadataFile = compiledDir.resolve("metadata.dat");
            try (var oos = new java.io.ObjectOutputStream(Files.newOutputStream(metadataFile))) {
                oos.writeObject(metadata);
            }
            
            // 打包为Cmod
            Path cmodFile = Paths.get("test-module.cmod");
            SimpleZip.zipFolder(tempDir, cmodFile);
            
            System.out.println("  创建Cmod文件: " + cmodFile);
            System.out.println("  文件大小: " + Files.size(cmodFile) + " 字节");
            
            // 测试读取Cmod信息
            CmodUnpacker unpacker = new CmodUnpacker(context);
            CmodUnpacker.CmodInfo info = unpacker.readCmodInfo(cmodFile);
            System.out.println("  模块信息: " + info);
            
            // 清理
            Files.deleteIfExists(cmodFile);
            
        } finally {
            deleteDirectory(tempDir);
            deleteDirectory(projectDir);
        }
    }
    
    private static void cleanup() throws IOException {
        System.out.println("\n清理测试文件...");
        // 确保所有测试文件都被清理
        System.out.println("  清理完成");
    }
    
    private static void deleteDirectory(Path dir) throws IOException {
        if (Files.exists(dir)) {
            Files.walk(dir)
                .sorted(Comparator.reverseOrder())
                .map(Path::toFile)
                .forEach(java.io.File::delete);
        }
    }
}