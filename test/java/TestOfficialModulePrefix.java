import com.chtl.compiler.importer.ImportManager;
import com.chtl.context.CompilationContext;
import com.chtl.ast.node.*;
import com.chtl.ast.*;

import java.nio.file.*;
import java.io.*;

/**
 * 测试官方模块前缀 chtl::
 */
public class TestOfficialModulePrefix {
    public static void main(String[] args) throws Exception {
        System.out.println("=== 测试官方模块前缀 chtl:: ===\n");
        
        // 创建测试环境
        Path workDir = Paths.get("test-official-modules");
        Path officialDir = workDir.resolve("module");
        Path projectDir = workDir.resolve("project");
        Path projectModuleDir = projectDir.resolve("module");
        
        // 设置官方模块目录
        System.setProperty("chtl.compiler.home", workDir.toString());
        
        // 清理并创建目录
        if (Files.exists(workDir)) {
            deleteRecursively(workDir);
        }
        Files.createDirectories(officialDir);
        Files.createDirectories(projectDir);
        Files.createDirectories(projectModuleDir);
        
        // 创建官方模块
        createOfficialModules(officialDir);
        
        // 创建项目本地模块（用于对比测试）
        createLocalModules(projectModuleDir);
        
        // 创建当前目录文件
        createCurrentDirFiles(projectDir);
        
        // 测试导入
        testImports(projectDir);
        
        // 清理
        deleteRecursively(workDir);
        
        System.out.println("\n测试完成！");
    }
    
    private static void createOfficialModules(Path officialDir) throws IOException {
        System.out.println("创建官方模块...");
        
        // 官方 DateTime.cmod
        Files.writeString(officialDir.resolve("DateTime.cmod"), 
            "// Official DateTime module (compiled)");
        
        // 官方 Utils.chtl
        Files.writeString(officialDir.resolve("Utils.chtl"), """
            [Namespace] Chtholly::Utils {
                [Template] official_helper() {
                    <div>Official Helper</div>
                }
            }
            """);
        
        // 官方 Network.cjmod
        Files.writeString(officialDir.resolve("Network.cjmod"), 
            "// Official Network CJmod module");
        
        System.out.println("  - DateTime.cmod");
        System.out.println("  - Utils.chtl");
        System.out.println("  - Network.cjmod");
    }
    
    private static void createLocalModules(Path localModuleDir) throws IOException {
        System.out.println("\n创建本地模块...");
        
        // 本地 DateTime.cmod（与官方同名）
        Files.writeString(localModuleDir.resolve("DateTime.cmod"), 
            "// Local DateTime module (compiled)");
        
        // 本地 MyUtils.chtl
        Files.writeString(localModuleDir.resolve("MyUtils.chtl"), """
            [Namespace] Local::Utils {
                [Template] local_helper() {
                    <div>Local Helper</div>
                }
            }
            """);
        
        System.out.println("  - DateTime.cmod (local version)");
        System.out.println("  - MyUtils.chtl");
    }
    
    private static void createCurrentDirFiles(Path currentDir) throws IOException {
        System.out.println("\n创建当前目录文件...");
        
        // 当前目录的 helper.chtl
        Files.writeString(currentDir.resolve("helper.chtl"), """
            [Template] current_helper() {
                <div>Current Directory Helper</div>
            }
            """);
        
        System.out.println("  - helper.chtl");
    }
    
    private static void testImports(Path projectDir) throws Exception {
        System.out.println("\n测试导入功能...");
        
        CompilationContext context = new CompilationContext();
        ImportManager importManager = new ImportManager(context, projectDir);
        
        // 测试1：使用官方模块前缀
        System.out.println("\n1. 测试 chtl:: 前缀导入官方模块");
        testImport(importManager, "[Import] @Chtl from chtl::DateTime", 
                  ImportStatementNode.ImportType.CHTL);
        testImport(importManager, "[Import] @Chtl from chtl::Utils", 
                  ImportStatementNode.ImportType.CHTL);
        testImport(importManager, "[Import] @CJmod from chtl::Network", 
                  ImportStatementNode.ImportType.CJMOD);
        
        // 测试2：不使用前缀（应该按优先级查找）
        System.out.println("\n2. 测试不带前缀的导入（优先级查找）");
        testImport(importManager, "[Import] @Chtl from DateTime", 
                  ImportStatementNode.ImportType.CHTL);
        testImport(importManager, "[Import] @Chtl from MyUtils", 
                  ImportStatementNode.ImportType.CHTL);
        testImport(importManager, "[Import] @Chtl from helper", 
                  ImportStatementNode.ImportType.CHTL);
        
        // 测试3：通配符导入
        System.out.println("\n3. 测试通配符导入");
        testImport(importManager, "[Import] @Chtl from chtl::*", 
                  ImportStatementNode.ImportType.CHTL);
        
        // 测试4：错误情况
        System.out.println("\n4. 测试错误情况");
        testImport(importManager, "[Import] @Html from chtl::style.css as mainStyle", 
                  ImportStatementNode.ImportType.HTML);
        
        // 显示所有错误
        if (!context.getErrors().isEmpty()) {
            System.out.println("\n编译错误:");
            for (var error : context.getErrors()) {
                System.out.println("  - " + error.getMessage());
            }
        }
    }
    
    private static void testImport(ImportManager manager, String importStatement, 
                                  ImportStatementNode.ImportType type) {
        // 解析导入语句（简化版）
        String fromPath = extractFromPath(importStatement);
        String asName = extractAsName(importStatement);
        
        ImportStatementNode importNode = new ImportStatementNode(type);
        importNode.setFromPath(fromPath);
        if (asName != null) {
            importNode.setAsName(asName);
        }
        
        System.out.println("\n  导入: " + importStatement);
        ImportManager.ImportResult result = manager.processImport(importNode);
        
        if (result.isSuccess()) {
            System.out.println("  ✓ 成功: " + result.getMessage());
        } else {
            System.out.println("  ✗ 失败: " + result.getMessage());
        }
    }
    
    private static String extractFromPath(String importStatement) {
        int fromIndex = importStatement.indexOf("from ");
        if (fromIndex < 0) return "";
        
        String fromPart = importStatement.substring(fromIndex + 5).trim();
        int spaceIndex = fromPart.indexOf(' ');
        if (spaceIndex > 0) {
            return fromPart.substring(0, spaceIndex);
        }
        return fromPart;
    }
    
    private static String extractAsName(String importStatement) {
        int asIndex = importStatement.indexOf(" as ");
        if (asIndex < 0) return null;
        
        return importStatement.substring(asIndex + 4).trim();
    }
    
    private static void deleteRecursively(Path path) throws IOException {
        if (!Files.exists(path)) return;
        
        Files.walk(path)
            .sorted((a, b) -> b.compareTo(a))
            .forEach(p -> {
                try {
                    Files.delete(p);
                } catch (IOException e) {
                    // 忽略
                }
            });
    }
}