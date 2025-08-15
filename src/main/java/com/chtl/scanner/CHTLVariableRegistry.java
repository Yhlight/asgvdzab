package com.chtl.scanner;

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
    
    // CSS函数检测器
    private static final CSSFunctionDetector cssDetector = new CSSFunctionDetector();
    
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
        // 使用CSS函数检测器排除CSS函数
        if (cssDetector.isCSSFunction(name)) {
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