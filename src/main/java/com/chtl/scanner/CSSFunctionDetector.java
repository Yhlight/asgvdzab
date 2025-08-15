package com.chtl.scanner;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;


import java.util.*;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

/**
 * CSS函数检测器 - 精确识别CSS函数避免误判
 */
public class CSSFunctionDetector {
    
    // 完整的CSS函数列表
    private static final Set<String> CSS_FUNCTIONS = new HashSet<>(Arrays.asList(
        // Transform functions
        "translate", "translateX", "translateY", "translateZ", "translate3d",
        "scale", "scaleX", "scaleY", "scaleZ", "scale3d",
        "rotate", "rotateX", "rotateY", "rotateZ", "rotate3d",
        "skew", "skewX", "skewY", "matrix", "matrix3d", "perspective",
        
        // Color functions
        "rgb", "rgba", "hsl", "hsla", "hwb", "lab", "lch", "oklab", "oklch",
        "color", "color-mix", "color-contrast", "device-cmyk",
        
        // Gradient functions
        "linear-gradient", "radial-gradient", "conic-gradient",
        "repeating-linear-gradient", "repeating-radial-gradient", "repeating-conic-gradient",
        
        // Filter functions
        "blur", "brightness", "contrast", "drop-shadow", "grayscale",
        "hue-rotate", "invert", "opacity", "saturate", "sepia",
        
        // Math functions
        "calc", "min", "max", "clamp", "round", "mod", "rem",
        "sin", "cos", "tan", "asin", "acos", "atan", "atan2",
        "pow", "sqrt", "hypot", "log", "exp", "abs", "sign",
        
        // Shape functions
        "circle", "ellipse", "inset", "polygon", "path",
        
        // Grid functions
        "repeat", "minmax", "fit-content",
        
        // Other CSS functions
        "var", "env", "url", "attr", "counter", "counters",
        "cubic-bezier", "steps", "element", "image", "image-set",
        "cross-fade", "paint", "symbols"
    ));
    
    // CSS函数的正则模式（考虑camelCase）
    private static final Pattern CSS_FUNC_PATTERN = Pattern.compile(
        "\\b([a-zA-Z][a-zA-Z0-9-]*?)\\s*\\("
    );
    
    // 缓存已检测的函数
    private final Map<String, Boolean> cache = new HashMap<>();
    
    /**
     * 检查是否是CSS函数
     */
    public boolean isCSSFunction(String functionName) {
        // 检查缓存
        Boolean cached = cache.get(functionName);
        if (cached != null) {
            return cached;
        }
        
        // 直接匹配
        if (CSS_FUNCTIONS.contains(functionName)) {
            cache.put(functionName, true);
            return true;
        }
        
        // 转换camelCase到kebab-case再检查
        String kebabCase = camelToKebab(functionName);
        boolean isCSS = CSS_FUNCTIONS.contains(kebabCase);
        cache.put(functionName, isCSS);
        
        return isCSS;
    }
    
    /**
     * 在内容中查找所有CSS函数的位置
     */
    public Set<Integer> findCSSFunctionPositions(String content) {
        Set<Integer> positions = new HashSet<>();
        Matcher matcher = CSS_FUNC_PATTERN.matcher(content);
        
        while (matcher.find()) {
            String funcName = matcher.group(1);
            if (isCSSFunction(funcName)) {
                positions.add(matcher.start());
            }
        }
        
        return positions;
    }
    
    /**
     * 检查特定位置是否是CSS函数调用
     */
    public boolean isCSSFunctionAt(String content, int position) {
        // 向前查找函数名开始
        int start = position;
        while (start > 0 && Character.isLetterOrDigit(content.charAt(start - 1))) {
            start--;
        }
        
        // 向后查找括号
        int end = position;
        while (end < content.length() && 
               (Character.isLetterOrDigit(content.charAt(end)) || 
                content.charAt(end) == '-')) {
            end++;
        }
        
        // 跳过空白
        while (end < content.length() && Character.isWhitespace(content.charAt(end))) {
            end++;
        }
        
        // 检查是否跟着括号
        if (end < content.length() && content.charAt(end) == '(') {
            String funcName = content.substring(start, position + 
                (position < content.length() && Character.isLetterOrDigit(content.charAt(position)) ? 1 : 0));
            return isCSSFunction(funcName.trim());
        }
        
        return false;
    }
    
    /**
     * camelCase转kebab-case
     */
    private String camelToKebab(String camel) {
        if (camel == null || camel.isEmpty()) {
            return camel;
        }
        
        StringBuilder result = new StringBuilder();
        for (int i = 0; i < camel.length(); i++) {
            char c = camel.charAt(i);
            if (Character.isUpperCase(c)) {
                if (i > 0 && Character.isLowerCase(camel.charAt(i - 1))) {
                    result.append('-');
                }
                result.append(Character.toLowerCase(c));
            } else {
                result.append(c);
            }
        }
        
        return result.toString();
    }
    
    /**
     * 智能检测：考虑上下文
     */
    public boolean isLikelyCSSFunction(String content, int position, String funcName) {
        // 首先检查是否是已知的CSS函数
        if (isCSSFunction(funcName)) {
            return true;
        }
        
        // 检查上下文线索
        // 1. 是否在CSS属性值中（: 和 ; 之间）
        int colonBefore = content.lastIndexOf(':', position);
        int semicolonBefore = content.lastIndexOf(';', position);
        boolean inPropertyValue = colonBefore > semicolonBefore;
        
        // 2. 检查常见的CSS属性前缀
        if (inPropertyValue) {
            int propStart = Math.max(0, semicolonBefore + 1);
            String property = content.substring(propStart, colonBefore).trim();
            
            // 如果是transform相关属性，更可能是CSS函数
            if (property.contains("transform") || 
                property.contains("filter") || 
                property.contains("background")) {
                return true;
            }
        }
        
        return false;
    }
}