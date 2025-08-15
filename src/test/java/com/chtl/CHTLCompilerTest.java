package com.chtl;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.AfterEach;
import static org.junit.jupiter.api.Assertions.*;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * CHTL编译器测试类
 */
public class CHTLCompilerTest {
    
    private CHTLCompilerMain compiler;
    
    @BeforeEach
    public void setUp() {
        compiler = new CHTLCompilerMain();
    }
    
    @AfterEach
    public void tearDown() {
        compiler.shutdown();
    }
    
    @Test
    public void testSimpleCHTLCompilation() {
        String chtlCode = """
            html {
                body {
                    div {
                        text {
                            Hello CHTL!
                        }
                    }
                }
            }
            """;
        
        String result = compiler.compile(chtlCode);
        
        assertNotNull(result);
        assertTrue(result.contains("<html"));
        assertTrue(result.contains("<body>"));
        assertTrue(result.contains("<div>"));
        assertTrue(result.contains("Hello CHTL!"));
    }
    
    @Test
    public void testLocalStyleCompilation() {
        String chtlCode = """
            div {
                style {
                    width: 100px;
                    height: 100px;
                    background-color: red;
                }
                
                text {
                    测试内容
                }
            }
            """;
        
        String result = compiler.compile(chtlCode);
        
        assertNotNull(result);
        assertTrue(result.contains("style="));
        assertTrue(result.contains("width: 100px"));
        assertTrue(result.contains("测试内容"));
    }
    
    @Test
    public void testCHTLJSCompilation() {
        String chtlCode = """
            button {
                id: testBtn;
                
                text {
                    点击
                }
                
                script {
                    {{#testBtn}}->listen({
                        click: () => {
                            console.log('按钮被点击');
                        }
                    });
                }
            }
            """;
        
        String result = compiler.compile(chtlCode);
        
        assertNotNull(result);
        assertTrue(result.contains("id=\"testBtn\""));
        assertTrue(result.contains("_chtl"));
        assertTrue(result.contains("addEventListener"));
    }
    
    @Test
    public void testFileCompilation() throws IOException {
        // 创建临时输入文件
        Path inputPath = Paths.get("target/test-input.chtl");
        Path outputPath = Paths.get("target/test-output.html");
        
        String chtlContent = """
            html {
                head {
                    title: "测试页面";
                }
                body {
                    h1 {
                        text { CHTL编译器测试 }
                    }
                }
            }
            """;
        
        Files.writeString(inputPath, chtlContent);
        
        // 编译文件
        compiler.compileFile(inputPath.toString(), outputPath.toString());
        
        // 验证输出文件
        assertTrue(Files.exists(outputPath));
        String output = Files.readString(outputPath);
        assertTrue(output.contains("<title>测试页面</title>"));
        assertTrue(output.contains("<h1>"));
        assertTrue(output.contains("CHTL编译器测试"));
        
        // 清理
        Files.deleteIfExists(inputPath);
        Files.deleteIfExists(outputPath);
    }
}