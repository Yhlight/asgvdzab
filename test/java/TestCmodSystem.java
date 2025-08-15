import com.chtl.context.CompilationContext;
import com.chtl.compiler.cmod.*;

import java.nio.file.*;
import java.io.IOException;

/**
 * 测试Cmod打包和解包系统
 */
public class TestCmodSystem {
    public static void main(String[] args) throws IOException {
        System.out.println("=== Cmod系统测试 ===\n");
        
        // 创建测试项目
        createTestProject();
        
        // 测试打包
        System.out.println("1. 测试Cmod打包...");
        testPackaging();
        
        // 测试解包
        System.out.println("\n2. 测试Cmod解包...");
        testUnpacking();
        
        // 测试信息读取
        System.out.println("\n3. 测试Cmod信息读取...");
        testInfoReading();
        
        // 清理
        cleanup();
        
        System.out.println("\n测试完成！");
    }
    
    private static void createTestProject() throws IOException {
        System.out.println("创建测试项目...");
        
        // 创建项目目录
        Files.createDirectories(Paths.get("test-cmod-project"));
        
        // 创建CHTL源文件
        Files.writeString(Paths.get("test-cmod-project/components.chtl"), """
            // 简单的UI组件
            div {
                class: button;
                
                text {
                    "Click Me"
                }
                
                style {
                    padding: 10px 20px;
                    background: #007bff;
                    color: white;
                    border: none;
                    border-radius: 4px;
                    cursor: pointer;
                }
            }
            """);
        
        Files.writeString(Paths.get("test-cmod-project/layouts.chtl"), """
            // 布局组件
            div {
                class: container;
                
                style {
                    max-width: 1200px;
                    margin: 0 auto;
                    padding: 0 15px;
                }
            }
            """);
        
        // 创建资源文件
        Files.writeString(Paths.get("test-cmod-project/theme.css"), """
            /* UI主题样式 */
            :root {
                --ui-primary: #007bff;
                --ui-secondary: #6c757d;
                --ui-success: #28a745;
                --ui-danger: #dc3545;
                --ui-warning: #ffc107;
                --ui-info: #17a2b8;
            }
            
            .ui-theme-dark {
                --ui-background: #1a1a1a;
                --ui-text: #ffffff;
            }
            
            .ui-theme-light {
                --ui-background: #ffffff;
                --ui-text: #333333;
            }
            """);
        
        Files.writeString(Paths.get("test-cmod-project/utils.js"), """
            // UI工具函数
            const UIUtils = {
                // 防抖函数
                debounce: function(func, wait) {
                    let timeout;
                    return function(...args) {
                        clearTimeout(timeout);
                        timeout = setTimeout(() => func.apply(this, args), wait);
                    };
                },
                
                // 节流函数
                throttle: function(func, wait) {
                    let lastTime = 0;
                    return function(...args) {
                        const now = Date.now();
                        if (now - lastTime >= wait) {
                            lastTime = now;
                            func.apply(this, args);
                        }
                    };
                }
            };
            """);
        
        System.out.println("测试项目创建完成");
    }
    
    private static void testPackaging() throws IOException {
        CompilationContext context = new CompilationContext();
        
        CmodPackager packager = new CmodPackager(
            context,
            Paths.get("test-cmod-project"),
            "UIComponents",
            "UI"
        );
        
        Path outputFile = Paths.get("ui-components.cmod");
        packager.packageCmod(outputFile);
        
        // 验证文件
        if (Files.exists(outputFile)) {
            long size = Files.size(outputFile);
            System.out.println("  Cmod文件创建成功");
            System.out.println("  文件大小: " + formatSize(size));
            
            // 检查是否是有效的zip文件
            try {
                boolean hasManifest = SimpleZip.containsEntry(outputFile, 
                    CmodFormat.META_DIR + "/" + CmodFormat.MANIFEST_FILE);
                System.out.println("  包含manifest: " + (hasManifest ? "是" : "否"));
            } catch (Exception e) {
                System.err.println("  验证失败: " + e.getMessage());
            }
        } else {
            System.err.println("  打包失败：文件未创建");
        }
    }
    
    private static void testUnpacking() throws IOException {
        CompilationContext context = new CompilationContext();
        
        CmodUnpacker unpacker = new CmodUnpacker(context);
        
        Path cmodFile = Paths.get("ui-components.cmod");
        Path targetDir = Paths.get("unpacked-cmod");
        
        if (!Files.exists(cmodFile)) {
            System.err.println("  Cmod文件不存在");
            return;
        }
        
        CmodUnpacker.UnpackResult result = unpacker.unpackCmod(cmodFile, targetDir);
        
        System.out.println("  解包成功");
        System.out.println("  模块名: " + result.getModuleName());
        System.out.println("  命名空间: " + result.getModuleNamespace());
        System.out.println("  版本: " + result.getVersion());
        System.out.println("  源文件数: " + result.getSourceFiles().size());
        System.out.println("  资源文件数: " + result.getResourceFiles().size());
        
        // 列出解包的文件
        System.out.println("\n  解包的源文件:");
        for (Path file : result.getSourceFiles()) {
            System.out.println("    - " + targetDir.relativize(file));
        }
        
        System.out.println("\n  解包的资源文件:");
        for (Path file : result.getResourceFiles()) {
            System.out.println("    - " + targetDir.relativize(file));
        }
    }
    
    private static void testInfoReading() throws IOException {
        CompilationContext context = new CompilationContext();
        
        CmodUnpacker unpacker = new CmodUnpacker(context);
        
        Path cmodFile = Paths.get("ui-components.cmod");
        
        if (!Files.exists(cmodFile)) {
            System.err.println("  Cmod文件不存在");
            return;
        }
        
        CmodUnpacker.CmodInfo info = unpacker.readCmodInfo(cmodFile);
        
        System.out.println("  Cmod信息读取成功");
        System.out.println("  " + info.toString());
        System.out.println("  创建时间: " + info.getCreated());
        System.out.println("  编译器版本: " + info.getCompilerVersion());
    }
    
    private static void cleanup() throws IOException {
        System.out.println("\n清理测试文件...");
        
        // 删除测试项目
        deleteDirectory(Paths.get("test-cmod-project"));
        
        // 删除生成的Cmod文件
        Files.deleteIfExists(Paths.get("ui-components.cmod"));
        
        // 删除解包目录
        deleteDirectory(Paths.get("unpacked-cmod"));
        
        System.out.println("清理完成");
    }
    
    private static void deleteDirectory(Path directory) throws IOException {
        if (!Files.exists(directory)) {
            return;
        }
        
        Files.walk(directory)
            .sorted(java.util.Comparator.reverseOrder())
            .map(Path::toFile)
            .forEach(java.io.File::delete);
    }
    
    private static String formatSize(long bytes) {
        if (bytes < 1024) {
            return bytes + " B";
        } else if (bytes < 1024 * 1024) {
            return String.format("%.2f KB", bytes / 1024.0);
        } else {
            return String.format("%.2f MB", bytes / (1024.0 * 1024));
        }
    }
}