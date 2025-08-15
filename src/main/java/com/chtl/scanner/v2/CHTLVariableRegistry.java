package com.chtl.scanner.v2;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

/**
 * CHTL变量注册表
 * 使用哈希表高效检测变量名
 */
public class CHTLVariableRegistry {
    
    // 全局变量注册表（线程安全）
    private static final Set<String> registeredVariables = 
        ConcurrentHashMap.newKeySet();
    
    // 预定义的常见CHTL变量模式
    private static final Set<String> commonVariablePrefixes = Set.of(
        "Theme", "Color", "Font", "Size", "Spacing", 
        "Layout", "Border", "Shadow", "Animation",
        "Breakpoint", "Transition", "Typography"
    );
    
    // CSS函数哈希表（用于快速排除）
    private static final Set<String> CSS_FUNCTIONS = Set.of(
        // Transform functions
        "translate", "translateX", "translateY", "translateZ", "translate3d",
        "scale", "scaleX", "scaleY", "scaleZ", "scale3d",
        "rotate", "rotateX", "rotateY", "rotateZ", "rotate3d",
        "skew", "skewX", "skewY",
        "matrix", "matrix3d", "perspective",
        // Color functions
        "rgb", "rgba", "hsl", "hsla", "hwb", "lab", "lch", "color",
        // Gradient functions
        "linear-gradient", "radial-gradient", "conic-gradient",
        "repeating-linear-gradient", "repeating-radial-gradient",
        // Filter functions
        "blur", "brightness", "contrast", "drop-shadow",
        "grayscale", "hue-rotate", "invert", "opacity",
        "saturate", "sepia",
        // Other CSS functions
        "calc", "min", "max", "clamp", "var", "env",
        "url", "attr", "counter", "counters",
        "cubic-bezier", "steps", "path",
        // Shape functions
        "circle", "ellipse", "inset", "polygon",
        // Grid functions
        "repeat", "minmax", "fit-content"
    );
    
    // 用于从代码中提取变量定义的模式
    private static final Pattern VARIABLE_DEFINITION = Pattern.compile(
        "\\[Template\\]\\s+@Var\\s+([A-Z][a-zA-Z0-9_]*)"
    );
    
    /**
     * 注册变量名
     */
    public static void register(String variableName) {
        registeredVariables.add(variableName);
    }
    
    /**
     * 批量注册变量名
     */
    public static void registerAll(Collection<String> variableNames) {
        registeredVariables.addAll(variableNames);
    }
    
    /**
     * 从代码中扫描并注册变量定义
     */
    public static void scanAndRegister(String code) {
        Matcher matcher = VARIABLE_DEFINITION.matcher(code);
        while (matcher.find()) {
            register(matcher.group(1));
        }
    }
    
    /**
     * 检查是否是CHTL变量
     * 使用多级策略：
     * 1. 首先检查是否是CSS函数（快速排除）
     * 2. 检查是否在注册表中
     * 3. 检查是否符合常见变量模式
     */
    public static boolean isCHTLVariable(String name) {
        // 快速排除CSS函数
        if (CSS_FUNCTIONS.contains(name)) {
            return false;
        }
        
        // 检查是否是camelCase的CSS函数（如translateY）
        if (isCamelCaseCSSFunction(name)) {
            return false;
        }
        
        // 检查是否在注册表中
        if (registeredVariables.contains(name)) {
            return true;
        }
        
        // 检查是否符合常见变量模式
        return matchesVariablePattern(name);
    }
    
    /**
     * 检查是否是camelCase的CSS函数
     */
    private static boolean isCamelCaseCSSFunction(String name) {
        // 将camelCase转换为kebab-case
        String kebabCase = camelToKebab(name);
        return CSS_FUNCTIONS.contains(kebabCase);
    }
    
    /**
     * camelCase转kebab-case
     */
    private static String camelToKebab(String camel) {
        return camel.replaceAll("([a-z])([A-Z])", "$1-$2").toLowerCase();
    }
    
    /**
     * 检查是否符合常见变量模式
     */
    private static boolean matchesVariablePattern(String name) {
        // 必须以大写字母开头
        if (!Character.isUpperCase(name.charAt(0))) {
            return false;
        }
        
        // 检查是否以常见前缀开始
        for (String prefix : commonVariablePrefixes) {
            if (name.startsWith(prefix)) {
                return true;
            }
        }
        
        // 其他启发式规则
        // 1. 至少包含一个大写字母（排除全小写）
        // 2. 不包含连字符（CSS函数特征）
        // 3. 长度合理（3-30字符）
        return name.length() >= 3 && 
               name.length() <= 30 && 
               !name.contains("-") &&
               name.matches("^[A-Z][a-zA-Z0-9_]*$");
    }
    
    /**
     * 清空注册表（用于测试）
     */
    public static void clear() {
        registeredVariables.clear();
    }
    
    /**
     * 获取注册的变量数量
     */
    public static int size() {
        return registeredVariables.size();
    }
    
    /**
     * 预加载常见变量名
     */
    public static void preloadCommonVariables() {
        // 预加载一些常见的CHTL变量名
        register("ThemeColor");
        register("ThemeColors");
        register("Colors");
        register("Fonts");
        register("FontSizes");
        register("Spacing");
        register("Breakpoints");
        register("Shadows");
        register("Borders");
        register("Transitions");
        register("Animations");
    }
}