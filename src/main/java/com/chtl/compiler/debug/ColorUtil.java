package com.chtl.compiler.debug;

/**
 * 终端颜色工具类
 */
public class ColorUtil {
    // ANSI颜色代码
    private static final String RESET = "\u001B[0m";
    private static final String RED = "\u001B[31m";
    private static final String GREEN = "\u001B[32m";
    private static final String YELLOW = "\u001B[33m";
    private static final String BLUE = "\u001B[34m";
    private static final String PURPLE = "\u001B[35m";
    private static final String CYAN = "\u001B[36m";
    private static final String GRAY = "\u001B[90m";
    
    // 是否启用颜色
    private static boolean colorEnabled = true;
    
    public static String red(String text) {
        return colorEnabled ? RED + text + RESET : text;
    }
    
    public static String green(String text) {
        return colorEnabled ? GREEN + text + RESET : text;
    }
    
    public static String yellow(String text) {
        return colorEnabled ? YELLOW + text + RESET : text;
    }
    
    public static String blue(String text) {
        return colorEnabled ? BLUE + text + RESET : text;
    }
    
    public static String purple(String text) {
        return colorEnabled ? PURPLE + text + RESET : text;
    }
    
    public static String cyan(String text) {
        return colorEnabled ? CYAN + text + RESET : text;
    }
    
    public static String gray(String text) {
        return colorEnabled ? GRAY + text + RESET : text;
    }
    
    public static void setColorEnabled(boolean enabled) {
        colorEnabled = enabled;
    }
    
    public static boolean isColorEnabled() {
        return colorEnabled;
    }
}