import com.chtl.parser.CHTLParser;
import com.chtl.generator.CHTLGenerator;
import com.chtl.ast.node.RootNode;

import java.io.FileWriter;
import java.io.IOException;

/**
 * 完整的CHTL编译测试
 * 从CHTL源代码到最终HTML文件
 */
public class TestFullCompilation {
    public static void main(String[] args) {
        System.out.println("=== CHTL完整编译测试 ===\n");
        
        String chtlCode = """
            // CHTL示例：一个简单的用户卡片界面
            
            [Template] @Style CardStyle {
                background-color: white;
                border-radius: 8px;
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
                padding: 20px;
                margin: 10px;
            }
            
            [Template] @Element UserCard {
                div {
                    class: user-card;
                    
                    style {
                        @Style CardStyle;
                        transition: transform 0.2s;
                        
                        &:hover {
                            transform: translateY(-2px);
                            box-shadow: 0 4px 8px rgba(0,0,0,0.15);
                        }
                    }
                    
                    img {
                        class: avatar;
                        src: "placeholder.jpg";
                        alt: "User Avatar";
                        
                        style {
                            width: 60px;
                            height: 60px;
                            border-radius: 50%;
                            margin-bottom: 10px;
                        }
                    }
                    
                    h3 {
                        class: username;
                        text {
                            "User Name"
                        }
                        
                        style {
                            margin: 0 0 5px 0;
                            color: #333;
                        }
                    }
                    
                    p {
                        class: bio;
                        text {
                            "User bio goes here..."
                        }
                        
                        style {
                            margin: 0;
                            color: #666;
                            font-size: 14px;
                        }
                    }
                    
                    button {
                        class: follow-btn;
                        text {
                            "Follow"
                        }
                        
                        style {
                            background-color: #007bff;
                            color: white;
                            border: none;
                            padding: 8px 16px;
                            border-radius: 4px;
                            cursor: pointer;
                            margin-top: 10px;
                            
                            &:hover {
                                background-color: #0056b3;
                            }
                            
                            &:active {
                                transform: scale(0.98);
                            }
                        }
                        
                        script {
                            {{.follow-btn}}->listen({
                                click: function() {
                                    if (this.textContent === 'Follow') {
                                        this.textContent = 'Following';
                                        this.style.backgroundColor = '#28a745';
                                    } else {
                                        this.textContent = 'Follow';
                                        this.style.backgroundColor = '#007bff';
                                    }
                                }
                            });
                        }
                    }
                }
            }
            
            // 主页面结构
            html {
                head {
                    meta {
                        charset: "UTF-8";
                    }
                    
                    title {
                        text {
                            "CHTL User Cards Demo"
                        }
                    }
                    
                    [Origin] @Style {
                        * {
                            box-sizing: border-box;
                        }
                        
                        body {
                            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
                            background-color: #f5f5f5;
                            margin: 0;
                            padding: 20px;
                        }
                    }
                }
                
                body {
                    div {
                        class: container;
                        
                        style {
                            max-width: 1200px;
                            margin: 0 auto;
                        }
                        
                        h1 {
                            text {
                                "User Directory"
                            }
                            
                            style {
                                text-align: center;
                                color: #333;
                                margin-bottom: 30px;
                            }
                        }
                        
                        div {
                            class: user-grid;
                            
                            style {
                                display: grid;
                                grid-template-columns: repeat(auto-fill, minmax(300px, 1fr));
                                gap: 20px;
                            }
                            
                            // 使用模板创建多个用户卡片
                            @Element UserCard;
                            @Element UserCard;
                            @Element UserCard;
                            
                            script {
                                // 动态更新用户数据
                                const cards = {{.user-card}};
                                const users = [
                                    { name: 'Alice Johnson', bio: 'Software Engineer | Coffee Lover', avatar: 'https://via.placeholder.com/60' },
                                    { name: 'Bob Smith', bio: 'Designer | Creative Mind', avatar: 'https://via.placeholder.com/60' },
                                    { name: 'Carol White', bio: 'Product Manager | Team Player', avatar: 'https://via.placeholder.com/60' }
                                ];
                                
                                cards.forEach((card, index) => {
                                    if (users[index]) {
                                        const user = users[index];
                                        card.querySelector('.username').textContent = user.name;
                                        card.querySelector('.bio').textContent = user.bio;
                                        card.querySelector('.avatar').src = user.avatar;
                                    }
                                });
                                
                                // 添加动画效果
                                animate({
                                    duration: 800,
                                    easing: 'ease-out',
                                    begin: {
                                        opacity: 0,
                                        transform: 'translateY(20px)'
                                    },
                                    end: {
                                        opacity: 1,
                                        transform: 'translateY(0)'
                                    }
                                });
                            }
                        }
                    }
                }
            }
            """;
        
        // 解析CHTL代码
        System.out.println("1. 解析CHTL代码...");
        CHTLParser parser = new CHTLParser();
        RootNode root = parser.parse(chtlCode);
        
        if (!parser.getErrors().isEmpty()) {
            System.err.println("解析错误:");
            for (String error : parser.getErrors()) {
                System.err.println("  " + error);
            }
            return;
        }
        System.out.println("   ✓ 解析成功");
        
        // 生成代码
        System.out.println("\n2. 生成HTML/CSS/JavaScript...");
        CHTLGenerator generator = new CHTLGenerator();
        CHTLGenerator.GenerationResult result = generator.generate(root);
        System.out.println("   ✓ 代码生成成功");
        
        // 输出生成的代码统计
        System.out.println("\n3. 生成代码统计:");
        System.out.println("   - HTML行数: " + countLines(result.getHtml()));
        System.out.println("   - CSS行数: " + countLines(result.getCss()));
        System.out.println("   - JavaScript行数: " + countLines(result.getJavascript()));
        
        // 保存到文件
        String outputFile = "output.html";
        System.out.println("\n4. 保存到文件: " + outputFile);
        
        try (FileWriter writer = new FileWriter(outputFile)) {
            writer.write(result.getCompleteHTML());
            System.out.println("   ✓ 文件保存成功");
            System.out.println("\n编译完成！请在浏览器中打开 " + outputFile + " 查看结果。");
        } catch (IOException e) {
            System.err.println("   ✗ 文件保存失败: " + e.getMessage());
        }
        
        // 显示生成的HTML预览
        System.out.println("\n=== 生成的HTML预览（前50行）===");
        String[] lines = result.getCompleteHTML().split("\n");
        for (int i = 0; i < Math.min(50, lines.length); i++) {
            System.out.println(lines[i]);
        }
        if (lines.length > 50) {
            System.out.println("... (还有 " + (lines.length - 50) + " 行)");
        }
    }
    
    private static int countLines(String text) {
        return text.isEmpty() ? 0 : text.split("\n").length;
    }
}