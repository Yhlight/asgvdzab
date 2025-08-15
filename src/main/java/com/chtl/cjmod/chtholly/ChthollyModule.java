package com.chtl.cjmod.chtholly;

import com.chtl.cjmod.CJmodBase;
import com.chtl.cjmod.CJmodFunction;
import com.chtl.cjmod.CJmodContext;
import java.awt.image.BufferedImage;
import java.net.URL;
import javax.imageio.ImageIO;
import java.util.*;

/**
 * Chtholly CJMOD模块
 * 为CHTL JS提供珂朵莉相关的扩展功能
 */
public class ChthollyModule extends CJmodBase {
    
    // ASCII字符集，从暗到亮
    private static final String ASCII_CHARS = " .,:;i1tfLCG08@";
    private static final String BLOCK_CHARS = " ░▒▓█";
    
    public ChthollyModule() {
        super("chtholly-js", "1.0.0");
    }
    
    @Override
    public void registerFunctions() {
        // 注册printMylove函数
        register("printMylove", new CJmodFunction() {
            @Override
            public Object execute(CJmodContext context, Object... args) {
                if (args.length < 1 || !(args[0] instanceof Map)) {
                    throw new IllegalArgumentException("printMylove需要一个配置对象");
                }
                
                @SuppressWarnings("unchecked")
                Map<String, Object> config = (Map<String, Object>) args[0];
                return printMylove(config);
            }
        });
        
        // 注册createFeatherRain函数
        register("createFeatherRain", new CJmodFunction() {
            @Override
            public Object execute(CJmodContext context, Object... args) {
                if (args.length < 2) {
                    throw new IllegalArgumentException("createFeatherRain需要容器元素和数量");
                }
                
                String containerId = String.valueOf(args[0]);
                int count = ((Number) args[1]).intValue();
                
                return generateFeatherRainScript(containerId, count);
            }
        });
        
        // 注册createGradientText函数
        register("createGradientText", new CJmodFunction() {
            @Override
            public Object execute(CJmodContext context, Object... args) {
                if (args.length < 2) {
                    throw new IllegalArgumentException("createGradientText需要文本和颜色数组");
                }
                
                String text = String.valueOf(args[0]);
                List<?> colors = (List<?>) args[1];
                
                return generateGradientTextHtml(text, colors);
            }
        });
    }
    
    /**
     * 将图片转换为字符画
     */
    private String printMylove(Map<String, Object> config) {
        try {
            String url = (String) config.get("url");
            String mode = (String) config.getOrDefault("mode", "ASCII");
            int width = ((Number) config.getOrDefault("width", 80)).intValue();
            int height = ((Number) config.getOrDefault("height", 40)).intValue();
            
            // 加载图片
            BufferedImage image = loadImage(url);
            if (image == null) {
                return "// 无法加载图片: " + url;
            }
            
            // 缩放图片
            BufferedImage scaledImage = scaleImage(image, width, height);
            
            // 转换为字符画
            if ("ASCII".equalsIgnoreCase(mode)) {
                return convertToAscii(scaledImage);
            } else if ("Pixel".equalsIgnoreCase(mode)) {
                return convertToPixelBlocks(scaledImage);
            } else {
                return convertToAscii(scaledImage);
            }
            
        } catch (Exception e) {
            return "// 转换失败: " + e.getMessage();
        }
    }
    
    /**
     * 加载图片
     */
    private BufferedImage loadImage(String urlString) {
        try {
            URL url = new URL(urlString);
            return ImageIO.read(url);
        } catch (Exception e) {
            // 如果是本地路径，尝试作为资源加载
            try {
                return ImageIO.read(getClass().getResourceAsStream(urlString));
            } catch (Exception ex) {
                return null;
            }
        }
    }
    
    /**
     * 缩放图片
     */
    private BufferedImage scaleImage(BufferedImage original, int width, int height) {
        BufferedImage scaled = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
        scaled.getGraphics().drawImage(
            original.getScaledInstance(width, height, BufferedImage.SCALE_SMOOTH), 
            0, 0, null
        );
        return scaled;
    }
    
    /**
     * 转换为ASCII字符画
     */
    private String convertToAscii(BufferedImage image) {
        StringBuilder result = new StringBuilder();
        result.append("/*\n");
        result.append(" * 珂朵莉 - 世界上最幸福的女孩\n");
        result.append(" */\n");
        
        int width = image.getWidth();
        int height = image.getHeight();
        
        for (int y = 0; y < height; y++) {
            result.append("// ");
            for (int x = 0; x < width; x++) {
                int rgb = image.getRGB(x, y);
                int brightness = getBrightness(rgb);
                char asciiChar = getAsciiChar(brightness);
                result.append(asciiChar);
            }
            result.append("\n");
        }
        
        return result.toString();
    }
    
    /**
     * 转换为像素块
     */
    private String convertToPixelBlocks(BufferedImage image) {
        StringBuilder result = new StringBuilder();
        result.append("/*\n");
        result.append(" * ♥ Chtholly Nota Seniorious ♥\n");
        result.append(" */\n");
        
        int width = image.getWidth();
        int height = image.getHeight();
        
        for (int y = 0; y < height; y++) {
            result.append("// ");
            for (int x = 0; x < width; x++) {
                int rgb = image.getRGB(x, y);
                int brightness = getBrightness(rgb);
                char blockChar = getBlockChar(brightness);
                result.append(blockChar);
            }
            result.append("\n");
        }
        
        return result.toString();
    }
    
    /**
     * 计算像素亮度
     */
    private int getBrightness(int rgb) {
        int r = (rgb >> 16) & 0xFF;
        int g = (rgb >> 8) & 0xFF;
        int b = rgb & 0xFF;
        return (int) (0.299 * r + 0.587 * g + 0.114 * b);
    }
    
    /**
     * 获取对应的ASCII字符
     */
    private char getAsciiChar(int brightness) {
        int index = (brightness * (ASCII_CHARS.length() - 1)) / 255;
        return ASCII_CHARS.charAt(index);
    }
    
    /**
     * 获取对应的块字符
     */
    private char getBlockChar(int brightness) {
        int index = (brightness * (BLOCK_CHARS.length() - 1)) / 255;
        return BLOCK_CHARS.charAt(index);
    }
    
    /**
     * 生成羽毛飘落效果的脚本
     */
    private String generateFeatherRainScript(String containerId, int count) {
        StringBuilder script = new StringBuilder();
        
        script.append("(function() {\n");
        script.append("  const container = document.getElementById('").append(containerId).append("');\n");
        script.append("  if (!container) return;\n\n");
        
        script.append("  // 创建羽毛\n");
        script.append("  for (let i = 0; i < ").append(count).append("; i++) {\n");
        script.append("    const feather = document.createElement('div');\n");
        script.append("    feather.className = 'chtholly-feather-rain';\n");
        script.append("    feather.style.cssText = `\n");
        script.append("      position: absolute;\n");
        script.append("      width: 20px;\n");
        script.append("      height: 30px;\n");
        script.append("      background: linear-gradient(45deg, #F0F8FF 0%, #87CEEB 100%);\n");
        script.append("      border-radius: 50% 0 50% 0;\n");
        script.append("      opacity: 0.7;\n");
        script.append("      top: ${Math.random() * -100}px;\n");
        script.append("      left: ${Math.random() * 100}%;\n");
        script.append("      animation: featherFall ${3 + Math.random() * 2}s linear infinite;\n");
        script.append("      animation-delay: ${Math.random() * 3}s;\n");
        script.append("      transform: rotate(${Math.random() * 360}deg);\n");
        script.append("    `;\n");
        script.append("    container.appendChild(feather);\n");
        script.append("  }\n\n");
        
        script.append("  // 添加动画样式\n");
        script.append("  if (!document.getElementById('chtholly-feather-style')) {\n");
        script.append("    const style = document.createElement('style');\n");
        script.append("    style.id = 'chtholly-feather-style';\n");
        script.append("    style.textContent = `\n");
        script.append("      @keyframes featherFall {\n");
        script.append("        to {\n");
        script.append("          transform: translateY(100vh) rotate(360deg);\n");
        script.append("          opacity: 0;\n");
        script.append("        }\n");
        script.append("      }\n");
        script.append("    `;\n");
        script.append("    document.head.appendChild(style);\n");
        script.append("  }\n");
        script.append("})();\n");
        
        return script.toString();
    }
    
    /**
     * 生成渐变文字HTML
     */
    private String generateGradientTextHtml(String text, List<?> colors) {
        StringBuilder html = new StringBuilder();
        
        // 生成唯一ID
        String id = "chtholly-gradient-" + System.currentTimeMillis();
        
        html.append("<span id='").append(id).append("' style='");
        html.append("background: linear-gradient(45deg");
        
        // 添加颜色
        for (Object color : colors) {
            html.append(", ").append(color);
        }
        
        html.append("); ");
        html.append("-webkit-background-clip: text; ");
        html.append("-webkit-text-fill-color: transparent; ");
        html.append("background-clip: text; ");
        html.append("text-fill-color: transparent; ");
        html.append("font-weight: bold; ");
        html.append("display: inline-block;");
        html.append("'>").append(text).append("</span>");
        
        return html.toString();
    }
}