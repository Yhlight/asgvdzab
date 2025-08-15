package com.chtl.scanner.v2;

import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.*;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

/**
 * Style块处理器
 * 负责识别style块中的CHTL语法
 */
public class StyleProcessor {
    
    // CHTL语法模式
    private static final Pattern VAR_FUNCTION = Pattern.compile(
        "([A-Z][a-zA-Z0-9_]*?)\\s*\\("
    );
    
    private static final Pattern STYLE_TEMPLATE = Pattern.compile(
        "@Style\\s+([A-Z][a-zA-Z0-9_]*?)\\s*;"
    );
    
    private static final Pattern ORIGIN_BLOCK = Pattern.compile(
        "\\[Origin\\]\\s*@Style"
    );
    
    // CSS函数白名单（避免误判）
    private static final Set<String> CSS_FUNCTIONS = Set.of(
        // Transform functions
        "translateX", "translateY", "translateZ", "translate", "translate3d",
        "scaleX", "scaleY", "scaleZ", "scale", "scale3d",
        "rotateX", "rotateY", "rotateZ", "rotate", "rotate3d",
        "skewX", "skewY", "skew",
        "perspective",
        "matrix", "matrix3d",
        // Color functions
        "rgb", "rgba", "hsl", "hsla", "hwb", "lab", "lch", "color",
        // Gradient functions
        "linear-gradient", "radial-gradient", "conic-gradient",
        "repeating-linear-gradient", "repeating-radial-gradient",
        // Other CSS functions
        "calc", "min", "max", "clamp", "var", "env",
        "url", "attr", "counter", "counters",
        "cubic-bezier", "steps"
    );
    
    // 用于更精确的CSS函数检测
    private static final Pattern CSS_FUNCTION_PATTERN = Pattern.compile(
        "\\b(translate[XYZ]?|scale[XYZ]?|rotate[XYZ]?|skew[XY]?|" +
        "rgb|rgba|hsl|hsla|calc|var|url|attr|" +
        "linear-gradient|radial-gradient)\\s*\\("
    );
    
    // 语法信息
    private static class SyntaxInfo {
        final SyntaxType type;
        final int start;
        final int end;
        final String content;
        
        SyntaxInfo(SyntaxType type, int start, int end, String content) {
            this.type = type;
            this.start = start;
            this.end = end;
            this.content = content;
        }
    }
    
    private enum SyntaxType {
        VARIABLE_FUNCTION,
        STYLE_TEMPLATE,
        ORIGIN_BLOCK,
        AMPERSAND  // & 符号（仅局部样式）
    }
    
    private final boolean isLocalStyle;
    
    public StyleProcessor(boolean isLocalStyle) {
        this.isLocalStyle = isLocalStyle;
    }
    
    /**
     * 处理style内容，返回片段列表
     */
    public List<CodeFragment> process(String styleContent) {
        // 首先移除style {}包装
        String content = unwrapStyleBlock(styleContent);
        
        // 构建语法位置映射
        TreeMap<Integer, SyntaxInfo> syntaxMap = findCHTLSyntax(content);
        
        // 根据语法位置切割片段
        return splitBySyntax(content, syntaxMap);
    }
    
    /**
     * 移除style {}包装
     */
    private String unwrapStyleBlock(String styleContent) {
        styleContent = styleContent.trim();
        if (styleContent.startsWith("style")) {
            styleContent = styleContent.substring(5).trim();
        }
        if (styleContent.startsWith("{") && styleContent.endsWith("}")) {
            return styleContent.substring(1, styleContent.length() - 1);
        }
        return styleContent;
    }
    
    /**
     * 查找所有CHTL语法
     */
    private TreeMap<Integer, SyntaxInfo> findCHTLSyntax(String content) {
        TreeMap<Integer, SyntaxInfo> syntaxMap = new TreeMap<>();
        
        // 首先找出所有CSS函数的位置，以便排除
        Set<Integer> cssFunctionPositions = findCSSFunctionPositions(content);
        
        // 查找变量函数调用
        Matcher varMatcher = VAR_FUNCTION.matcher(content);
        while (varMatcher.find()) {
            String functionName = varMatcher.group(1);
            int start = varMatcher.start();
            
            // 检查是否在CSS函数位置
            if (cssFunctionPositions.contains(start)) {
                continue;
            }
            
            // 排除CSS函数
            if (!CSS_FUNCTIONS.contains(functionName)) {
                // 找到完整的函数调用
                int end = findFunctionEnd(content, varMatcher.end() - 1);
                
                syntaxMap.put(start, new SyntaxInfo(
                    SyntaxType.VARIABLE_FUNCTION,
                    start,
                    end,
                    content.substring(start, end)
                ));
            }
        }
        
        // 查找样式模板使用
        Matcher styleMatcher = STYLE_TEMPLATE.matcher(content);
        while (styleMatcher.find()) {
            syntaxMap.put(styleMatcher.start(), new SyntaxInfo(
                SyntaxType.STYLE_TEMPLATE,
                styleMatcher.start(),
                styleMatcher.end(),
                styleMatcher.group()
            ));
        }
        
        // 查找原始嵌入
        Matcher originMatcher = ORIGIN_BLOCK.matcher(content);
        while (originMatcher.find()) {
            int start = originMatcher.start();
            int end = findOriginBlockEnd(content, originMatcher.end());
            
            syntaxMap.put(start, new SyntaxInfo(
                SyntaxType.ORIGIN_BLOCK,
                start,
                end,
                content.substring(start, end)
            ));
        }
        
        // 局部样式特有：查找&符号
        if (isLocalStyle) {
            findAmpersands(content, syntaxMap);
        }
        
        return syntaxMap;
    }
    
    /**
     * 查找所有CSS函数的位置
     */
    private Set<Integer> findCSSFunctionPositions(String content) {
        Set<Integer> positions = new HashSet<>();
        
        Matcher cssMatcher = CSS_FUNCTION_PATTERN.matcher(content);
        while (cssMatcher.find()) {
            positions.add(cssMatcher.start());
        }
        
        return positions;
    }
    
    /**
     * 查找函数调用的结束位置
     */
    private int findFunctionEnd(String content, int parenStart) {
        int parenDepth = 1;
        int pos = parenStart + 1;
        
        while (pos < content.length() && parenDepth > 0) {
            char c = content.charAt(pos);
            if (c == '(') {
                parenDepth++;
            } else if (c == ')') {
                parenDepth--;
            } else if (c == '"' || c == '\'') {
                // 跳过字符串
                pos = skipString(content, pos);
            }
            pos++;
        }
        
        return pos;
    }
    
    /**
     * 查找原始嵌入块的结束位置
     */
    private int findOriginBlockEnd(String content, int start) {
        // 跳过空白找到 {
        while (start < content.length() && content.charAt(start) != '{') {
            start++;
        }
        
        if (start >= content.length()) {
            return content.length();
        }
        
        // 找到匹配的 }
        int braceDepth = 0;
        int pos = start;
        
        while (pos < content.length()) {
            char c = content.charAt(pos);
            if (c == '{') {
                braceDepth++;
            } else if (c == '}') {
                braceDepth--;
                if (braceDepth == 0) {
                    return pos + 1;
                }
            } else if (c == '"' || c == '\'') {
                pos = skipString(content, pos);
            }
            pos++;
        }
        
        return pos;
    }
    
    /**
     * 查找&符号（仅局部样式）
     */
    private void findAmpersands(String content, TreeMap<Integer, SyntaxInfo> syntaxMap) {
        for (int i = 0; i < content.length(); i++) {
            if (content.charAt(i) == '&') {
                // 确保不在字符串中
                if (!isInString(content, i)) {
                    // 检查是否是独立的&（不是&&）
                    if (i + 1 >= content.length() || content.charAt(i + 1) != '&') {
                        syntaxMap.put(i, new SyntaxInfo(
                            SyntaxType.AMPERSAND,
                            i,
                            i + 1,
                            "&"
                        ));
                    }
                }
            }
        }
    }
    
    /**
     * 根据语法位置切割片段
     */
    private List<CodeFragment> splitBySyntax(String content, TreeMap<Integer, SyntaxInfo> syntaxMap) {
        List<CodeFragment> fragments = new ArrayList<>();
        int lastPos = 0;
        
        // 添加开始的style标记
        fragments.add(new CodeFragment(
            FragmentType.CHTL,
            "style {",
            0,
            7
        ));
        
        for (SyntaxInfo syntax : syntaxMap.values()) {
            // 添加语法前的CSS片段
            if (lastPos < syntax.start) {
                String cssContent = content.substring(lastPos, syntax.start);
                if (!cssContent.trim().isEmpty()) {
                    fragments.add(new CodeFragment(
                        FragmentType.CSS,
                        cssContent,
                        lastPos,
                        cssContent.length()
                    ));
                }
            }
            
            // 添加CHTL语法片段
            fragments.add(new CodeFragment(
                FragmentType.CHTL,
                syntax.content,
                syntax.start,
                syntax.content.length()
            ));
            
            lastPos = syntax.end;
        }
        
        // 添加最后的CSS片段
        if (lastPos < content.length()) {
            String cssContent = content.substring(lastPos);
            if (!cssContent.trim().isEmpty()) {
                fragments.add(new CodeFragment(
                    FragmentType.CSS,
                    cssContent,
                    lastPos,
                    cssContent.length()
                ));
            }
        }
        
        // 添加结束的}
        fragments.add(new CodeFragment(
            FragmentType.CHTL,
            "}",
            content.length(),
            1
        ));
        
        return fragments;
    }
    
    /**
     * 跳过字符串
     */
    private int skipString(String content, int start) {
        char quote = content.charAt(start);
        int pos = start + 1;
        
        while (pos < content.length()) {
            char c = content.charAt(pos);
            if (c == quote) {
                return pos;
            }
            if (c == '\\') {
                pos++; // 跳过转义字符
            }
            pos++;
        }
        
        return pos;
    }
    
    /**
     * 检查位置是否在字符串中
     */
    private boolean isInString(String content, int position) {
        boolean inString = false;
        char stringChar = '\0';
        
        for (int i = 0; i < position; i++) {
            char c = content.charAt(i);
            if (!inString && (c == '"' || c == '\'')) {
                inString = true;
                stringChar = c;
            } else if (inString && c == stringChar) {
                // 检查是否是转义的引号
                if (i > 0 && content.charAt(i - 1) != '\\') {
                    inString = false;
                }
            }
        }
        
        return inString;
    }
}