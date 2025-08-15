import com.chtl.*;
import java.nio.file.*;

/**
 * 简单的Cmod功能演示
 */
public class TestSimpleCmod {
    public static void main(String[] args) throws Exception {
        System.out.println("=== 简单Cmod功能演示 ===\n");
        
        // 创建简单项目
        System.out.println("1. 创建测试项目...");
        createSimpleProject();
        
        // 使用主编译器打包
        System.out.println("\n2. 打包Cmod...");
        CHTLCompilerMain.main(new String[]{"package", "simple-project", "simple.cmod", "SimpleModule", "Simple"});
        
        // 显示信息
        System.out.println("\n3. 显示Cmod信息...");
        CHTLCompilerMain.main(new String[]{"info", "simple.cmod"});
        
        // 解包
        System.out.println("\n4. 解包Cmod...");
        CHTLCompilerMain.main(new String[]{"unpack", "simple.cmod", "unpacked"});
        
        // 列出解包的文件
        System.out.println("\n5. 解包的文件:");
        Files.walk(Paths.get("unpacked"))
            .filter(Files::isRegularFile)
            .forEach(path -> System.out.println("   " + path));
        
        // 清理
        System.out.println("\n6. 清理...");
        cleanup();
        
        System.out.println("\n演示完成！");
    }
    
    private static void createSimpleProject() throws Exception {
        // 创建项目目录
        Files.createDirectories(Paths.get("simple-project"));
        
        // 创建简单的CHTL文件
        Files.writeString(Paths.get("simple-project/hello.chtl"), """
            div {
                id: hello;
                
                h1 {
                    text {
                        "Hello CHTL"
                    }
                }
                
                p {
                    text {
                        "This is a simple CHTL module"
                    }
                }
            }
            """);
        
        // 创建另一个CHTL文件
        Files.writeString(Paths.get("simple-project/button.chtl"), """
            button {
                class: btn;
                
                text {
                    "Click me"
                }
            }
            """);
        
        // 创建资源文件
        Files.writeString(Paths.get("simple-project/style.css"), """
            .btn {
                padding: 10px 20px;
                background: blue;
                color: white;
            }
            """);
        
        System.out.println("   创建了3个文件");
    }
    
    private static void cleanup() throws Exception {
        // 删除测试文件
        deleteDir(Paths.get("simple-project"));
        deleteDir(Paths.get("unpacked"));
        Files.deleteIfExists(Paths.get("simple.cmod"));
        
        System.out.println("   清理完成");
    }
    
    private static void deleteDir(Path dir) throws Exception {
        if (Files.exists(dir)) {
            Files.walk(dir)
                .sorted(java.util.Comparator.reverseOrder())
                .map(Path::toFile)
                .forEach(java.io.File::delete);
        }
    }
}