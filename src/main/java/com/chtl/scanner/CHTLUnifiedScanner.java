package com.chtl.scanner;

import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
// import org.slf4j.Logger;
// import org.slf4j.LoggerFactory;

import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * CHTL统一扫描器 - 精准代码切割器
 * 使用宽判严判机制，将代码切割成最小可编译单元
 * 
 * 切割策略：
 * 1. 宽判：JS和CSS代码块作为整体，以CHTL/CHTL JS特征为分割点
 * 2. 严判：CHTL和CHTL JS代码以最小单元切割，但保持上下文连续性
 */
public class CHTLUnifiedScanner {
    // private static final Logger logger = LoggerFactory.getLogger(CHTLUnifiedScanner.class);
    
    // CHTL JS特征模式
    private static final Pattern CHTL_JS_SELECTOR = Pattern.compile("\\{\\{([^}]+)\\}\\}");
    private static final Pattern CHTL_JS_ARROW = Pattern.compile("->\\s*\\w+");
    private static final Pattern CHTL_JS_METHODS = Pattern.compile("->\\s*(listen|delegate|animate)\\s*\\(");
    
    // CHTL特征模式
    private static final Pattern CHTL_ELEMENT = Pattern.compile("\\b\\w+\\s*\\{");
    private static final Pattern CHTL_ATTRIBUTE = Pattern.compile("\\w+\\s*:\\s*[^;]+;");
    private static final Pattern CHTL_TEXT = Pattern.compile("\\btext\\s*\\{[^}]*\\}");
    private static final Pattern CHTL_STYLE = Pattern.compile("\\bstyle\\s*\\{");
    private static final Pattern CHTL_SCRIPT = Pattern.compile("\\bscript\\s*\\{");
    
    // 代码块边界
    private static final Pattern BLOCK_START = Pattern.compile("\\{");
    private static final Pattern BLOCK_END = Pattern.compile("\\}");
    
    private enum ScanMode {
        CHTL,           // CHTL主代码
        LOCAL_STYLE,    // 局部style块
        LOCAL_SCRIPT,   // 局部script块
        GLOBAL_STYLE,   // 全局CSS
        GLOBAL_SCRIPT   // 全局JS
    }
    
    public CHTLUnifiedScanner() {
        // logger.info("初始化CHTL统一扫描器 - 宽判严判模式");
    }
    
    /**
     * 扫描并切割CHTL源代码
     */
    public List<CodeFragment> scan(String sourceCode) {
        // logger.debug("开始扫描CHTL源代码，长度: {} 字符", sourceCode.length());
        
        // 使用V2扫描器替代V1
        com.chtl.scanner.v2.UnifiedScannerV2 scannerV2 = new com.chtl.scanner.v2.UnifiedScannerV2();
        List<CodeFragment> fragments = scannerV2.scan(sourceCode);
        
        // 创建一个虚拟的contextManager用于后处理（V2内部已经处理了上下文）
        LanguageContextManager contextManager = new LanguageContextManager();
        
        // 后处理：添加额外的元数据
        postProcessFragments(fragments, contextManager);
        
        // logger.info("扫描完成，共生成 {} 个代码片段", fragments.size());
        return fragments;
    }
    
    /**
     * 后处理片段，添加额外的元数据
     */
    private void postProcessFragments(List<CodeFragment> fragments, LanguageContextManager contextManager) {
        for (CodeFragment fragment : fragments) {
            // 为CHTL JS片段添加函数类型元数据
            if (fragment.getType() == FragmentType.CHTL_JS) {
                String content = fragment.getContent();
                if (content.contains(".listen")) {
                    fragment.setMetadata("function", "listen");
                } else if (content.contains(".delegate")) {
                    fragment.setMetadata("function", "delegate");
                } else if (content.contains("animate(")) {
                    fragment.setMetadata("function", "animate");
                } else if (content.contains("{{") && content.contains("}}")) {
                    fragment.setMetadata("syntax", "enhanced-selector");
                } else if (content.contains("->")) {
                    fragment.setMetadata("syntax", "arrow-operator");
                }
            }
            
            // 为CHTL片段添加变量使用元数据
            if (fragment.getType() == FragmentType.CHTL) {
                String content = fragment.getContent();
                if (content.matches(".*[A-Z]\\w*\\s*\\(.*\\).*")) {
                    fragment.setMetadata("syntax", "variable-function");
                } else if (content.contains("@Var")) {
                    fragment.setMetadata("syntax", "variable-reference");
                }
            }
            
            // 为CSS片段添加上下文信息
            if (fragment.getType() == FragmentType.CSS) {
                // 根据片段内容判断是否在局部style中
                if (fragment.getMetadata("context") == null) {
                    fragment.setMetadata("context", "css");
                }
            }
        }
    }
    
    /**
     * 扫描下一个代码片段
     */
    private CodeFragment scanNextFragment(ScanContext context) {
        context.skipWhitespace();
        
        if (context.isEnd()) {
            return null;
        }
        
        // 检测当前位置的代码类型
        FragmentInfo info = detectFragmentType(context);
        
        switch (info.type) {
            case CHTL:
                return scanCHTLFragment(context);
                
            case CHTL_LOCAL_STYLE:
                return scanLocalStyleFragment(context);
                
            case CHTL_JS:
                return scanCHTLJSFragment(context);
                
            case CSS:
                return scanCSSFragment(context);
                
            case JAVASCRIPT:
                return scanJavaScriptFragment(context);
                
            default:
                // 未知类型，作为CHTL处理
                return scanCHTLFragment(context);
        }
    }
    
    /**
     * 检测代码片段类型
     */
    private FragmentInfo detectFragmentType(ScanContext context) {
        String ahead = context.peekAhead(100);
        
        // 检查是否在style块内
        if (context.isInStyleBlock()) {
            // 检查是否包含CHTL特征
            if (containsCHTLStyleFeatures(ahead)) {
                return new FragmentInfo(FragmentType.CHTL_LOCAL_STYLE);
            }
            return new FragmentInfo(FragmentType.CSS);
        }
        
        // 检查是否在script块内
        if (context.isInScriptBlock()) {
            // 检查是否包含CHTL JS特征
            if (containsCHTLJSFeatures(ahead)) {
                return new FragmentInfo(FragmentType.CHTL_JS);
            }
            return new FragmentInfo(FragmentType.JAVASCRIPT);
        }
        
        // 检查全局样式标记
        if (ahead.startsWith("[Origin]") && ahead.contains("@Style")) {
            return new FragmentInfo(FragmentType.CSS);
        }
        
        // 默认为CHTL
        return new FragmentInfo(FragmentType.CHTL);
    }
    
    /**
     * 扫描CHTL片段（严判模式）
     */
    private CodeFragment scanCHTLFragment(ScanContext context) {
        int startPos = context.position;
        StringBuilder content = new StringBuilder();
        
        // 扫描CHTL最小单元，但保持连续性
        while (!context.isEnd()) {
            // 检查是否遇到style或script块
            if (context.lookingAt("style\\s*\\{")) {
                if (content.length() > 0) {
                    // 返回已收集的CHTL内容
                    break;
                }
                // 处理style块
                return scanLocalStyleFragment(context);
            }
            
            if (context.lookingAt("script\\s*\\{")) {
                if (content.length() > 0) {
                    // 返回已收集的CHTL内容
                    break;
                }
                // 处理script块
                return scanLocalScriptFragment(context);
            }
            
            // 扫描一个完整的CHTL元素或属性
            String element = scanCHTLElement(context);
            if (element != null) {
                content.append(element);
                continue;
            }
            
            // 扫描单个字符
            content.append(context.current());
            context.advance();
            
            // 检查是否应该结束当前片段
            if (shouldEndCHTLFragment(context, content)) {
                break;
            }
        }
        
        return new CodeFragment(FragmentType.CHTL, content.toString(), startPos);
    }
    
    /**
     * 扫描CHTL元素
     */
    private String scanCHTLElement(ScanContext context) {
        // 匹配元素模式: identifier { ... }
        if (context.lookingAt("\\w+\\s*\\{")) {
            return scanBlock(context);
        }
        
        // 匹配属性模式: identifier : value ;
        if (context.lookingAt("\\w+\\s*:")) {
            return scanAttribute(context);
        }
        
        return null;
    }
    
    /**
     * 扫描局部样式片段（严判模式）
     */
    private CodeFragment scanLocalStyleFragment(ScanContext context) {
        int startPos = context.position;
        
        // 跳过 style {
        context.skipPattern("style\\s*\\{");
        
        List<CodeFragment> styleFragments = new ArrayList<>();
        int braceCount = 1;
        
        while (!context.isEnd() && braceCount > 0) {
            if (context.current() == '{') {
                braceCount++;
            } else if (context.current() == '}') {
                braceCount--;
                if (braceCount == 0) {
                    context.advance(); // 跳过结束的 }
                    break;
                }
            }
            
            // 检测CHTL样式特征
            if (containsCHTLStyleFeatures(context.peekAhead(50))) {
                // 切割CHTL样式片段
                CodeFragment chtlStyle = scanCHTLStyleFragment(context);
                styleFragments.add(chtlStyle);
            } else {
                // 普通CSS片段
                CodeFragment css = scanPureCSSFragment(context, braceCount);
                styleFragments.add(css);
            }
        }
        
        // 合并样式片段
        return mergeStyleFragments(styleFragments, startPos);
    }
    
    /**
     * 扫描局部脚本片段
     */
    private CodeFragment scanLocalScriptFragment(ScanContext context) {
        int startPos = context.position;
        
        // 跳过 script {
        context.skipPattern("script\\s*\\{");
        
        StringBuilder content = new StringBuilder();
        int braceCount = 1;
        
        while (!context.isEnd() && braceCount > 0) {
            // 检查CHTL JS特征
            if (CHTL_JS_SELECTOR.matcher(context.peekAhead(20)).find()) {
                // 提取CHTL JS选择器
                String selector = extractCHTLJSSelector(context);
                content.append(selector);
                continue;
            }
            
            if (context.lookingAt("->")) {
                // 处理箭头操作符
                content.append(context.consume(2));
                continue;
            }
            
            char ch = context.current();
            content.append(ch);
            context.advance();
            
            if (ch == '{') {
                braceCount++;
            } else if (ch == '}') {
                braceCount--;
            }
        }
        
        return new CodeFragment(FragmentType.CHTL_JS, content.toString(), startPos);
    }
    
    /**
     * 扫描CHTL JS片段（严判模式）
     */
    private CodeFragment scanCHTLJSFragment(ScanContext context) {
        int startPos = context.position;
        StringBuilder content = new StringBuilder();
        
        // 扫描最小CHTL JS单元
        if (context.lookingAt("\\{\\{")) {
            // 扫描增强选择器
            content.append(extractCHTLJSSelector(context));
        } else if (context.lookingAt("->")) {
            // 扫描箭头操作及其后续
            content.append(scanArrowOperation(context));
        } else {
            // 扫描普通JS直到遇到CHTL JS特征
            content.append(scanUntilCHTLJSFeature(context));
        }
        
        return new CodeFragment(FragmentType.CHTL_JS, content.toString(), startPos);
    }
    
    /**
     * 扫描CSS片段（宽判模式）
     */
    private CodeFragment scanCSSFragment(ScanContext context) {
        int startPos = context.position;
        StringBuilder content = new StringBuilder();
        
        // CSS采用宽判，遇到CHTL特征才分割
        while (!context.isEnd()) {
            // 检查是否遇到CHTL特征
            if (containsCHTLFeatures(context.peekAhead(50))) {
                break;
            }
            
            content.append(context.current());
            context.advance();
        }
        
        return new CodeFragment(FragmentType.CSS, content.toString(), startPos);
    }
    
    /**
     * 扫描JavaScript片段（宽判模式）
     */
    private CodeFragment scanJavaScriptFragment(ScanContext context) {
        int startPos = context.position;
        StringBuilder content = new StringBuilder();
        
        // JS采用宽判，遇到CHTL JS特征才分割
        while (!context.isEnd()) {
            // 检查是否遇到CHTL JS特征
            if (containsCHTLJSFeatures(context.peekAhead(50))) {
                break;
            }
            
            content.append(context.current());
            context.advance();
        }
        
        return new CodeFragment(FragmentType.JAVASCRIPT, content.toString(), startPos);
    }
    
    /**
     * 提取CHTL JS选择器
     */
    private String extractCHTLJSSelector(ScanContext context) {
        StringBuilder selector = new StringBuilder();
        
        // 消费 {{
        selector.append(context.consume(2));
        
        // 读取到 }}
        while (!context.isEnd() && !context.lookingAt("\\}\\}")) {
            selector.append(context.current());
            context.advance();
        }
        
        // 消费 }}
        if (context.lookingAt("\\}\\}")) {
            selector.append(context.consume(2));
        }
        
        return selector.toString();
    }
    
    /**
     * 扫描箭头操作
     */
    private String scanArrowOperation(ScanContext context) {
        StringBuilder operation = new StringBuilder();
        
        // 消费 ->
        operation.append(context.consume(2));
        
        // 跳过空白
        while (!context.isEnd() && Character.isWhitespace(context.current())) {
            operation.append(context.current());
            context.advance();
        }
        
        // 读取方法名
        while (!context.isEnd() && (Character.isLetterOrDigit(context.current()) || context.current() == '_')) {
            operation.append(context.current());
            context.advance();
        }
        
        return operation.toString();
    }
    
    /**
     * 扫描直到遇到CHTL JS特征
     */
    private String scanUntilCHTLJSFeature(ScanContext context) {
        StringBuilder content = new StringBuilder();
        
        while (!context.isEnd()) {
            if (containsCHTLJSFeatures(context.peekAhead(20))) {
                break;
            }
            
            content.append(context.current());
            context.advance();
        }
        
        return content.toString();
    }
    
    /**
     * 扫描代码块
     */
    private String scanBlock(ScanContext context) {
        StringBuilder block = new StringBuilder();
        int braceCount = 0;
        
        // 读取标识符
        while (!context.isEnd() && (Character.isLetterOrDigit(context.current()) || context.current() == '_')) {
            block.append(context.current());
            context.advance();
        }
        
        // 跳过空白
        while (!context.isEnd() && Character.isWhitespace(context.current())) {
            block.append(context.current());
            context.advance();
        }
        
        // 读取块内容
        if (context.current() == '{') {
            do {
                block.append(context.current());
                if (context.current() == '{') {
                    braceCount++;
                } else if (context.current() == '}') {
                    braceCount--;
                }
                context.advance();
            } while (!context.isEnd() && braceCount > 0);
        }
        
        return block.toString();
    }
    
    /**
     * 扫描属性
     */
    private String scanAttribute(ScanContext context) {
        StringBuilder attr = new StringBuilder();
        
        // 读取到分号
        while (!context.isEnd() && context.current() != ';') {
            attr.append(context.current());
            context.advance();
        }
        
        // 包含分号
        if (context.current() == ';') {
            attr.append(context.current());
            context.advance();
        }
        
        return attr.toString();
    }
    
    /**
     * 检查是否包含CHTL特征
     */
    private boolean containsCHTLFeatures(String code) {
        return CHTL_ELEMENT.matcher(code).find() ||
               CHTL_ATTRIBUTE.matcher(code).find() ||
               CHTL_TEXT.matcher(code).find() ||
               CHTL_STYLE.matcher(code).find() ||
               CHTL_SCRIPT.matcher(code).find();
    }
    
    /**
     * 检查是否包含CHTL样式特征
     */
    private boolean containsCHTLStyleFeatures(String code) {
        return code.contains("&") || // 上下文选择器
               code.matches(".*\\.\\w+\\s*\\{.*") || // 类选择器
               code.matches(".*#\\w+\\s*\\{.*"); // ID选择器
    }
    
    /**
     * 检查是否包含CHTL JS特征
     */
    private boolean containsCHTLJSFeatures(String code) {
        return CHTL_JS_SELECTOR.matcher(code).find() ||
               CHTL_JS_ARROW.matcher(code).find() ||
               CHTL_JS_METHODS.matcher(code).find();
    }
    
    /**
     * 判断是否应该结束CHTL片段
     */
    private boolean shouldEndCHTLFragment(ScanContext context, StringBuilder content) {
        // 如果内容已经足够大，考虑分割
        if (content.length() > 500) {
            // 检查是否在一个完整的结构边界
            String lastChars = content.substring(Math.max(0, content.length() - 10));
            return lastChars.contains("}") || lastChars.contains(";");
        }
        
        return false;
    }
    
    /**
     * 扫描CHTL样式片段
     */
    private CodeFragment scanCHTLStyleFragment(ScanContext context) {
        int startPos = context.position;
        StringBuilder content = new StringBuilder();
        
        // 扫描CHTL样式特征
        while (!context.isEnd() && containsCHTLStyleFeatures(context.peekAhead(50))) {
            if (context.current() == '&') {
                // 上下文选择器
                content.append(scanContextSelector(context));
            } else if (context.current() == '.' || context.current() == '#') {
                // 类或ID选择器
                content.append(scanCSSSelector(context));
            } else {
                content.append(context.current());
                context.advance();
            }
            
            // 检查是否应该结束
            if (content.toString().contains("}")) {
                break;
            }
        }
        
        return new CodeFragment(FragmentType.CHTL_LOCAL_STYLE, content.toString(), startPos);
    }
    
    /**
     * 扫描纯CSS片段
     */
    private CodeFragment scanPureCSSFragment(ScanContext context, int maxBraceCount) {
        int startPos = context.position;
        StringBuilder content = new StringBuilder();
        int braceCount = 0;
        
        while (!context.isEnd()) {
            char ch = context.current();
            
            if (ch == '{') {
                braceCount++;
            } else if (ch == '}') {
                if (braceCount == 0) {
                    // 到达style块的结束
                    break;
                }
                braceCount--;
            }
            
            // 检查是否遇到CHTL特征
            if (containsCHTLStyleFeatures(context.peekAhead(20))) {
                break;
            }
            
            content.append(ch);
            context.advance();
        }
        
        return new CodeFragment(FragmentType.CSS, content.toString(), startPos);
    }
    
    /**
     * 扫描上下文选择器
     */
    private String scanContextSelector(ScanContext context) {
        StringBuilder selector = new StringBuilder();
        
        // 消费 &
        selector.append(context.current());
        context.advance();
        
        // 读取伪类或伪元素
        while (!context.isEnd() && (context.current() == ':' || Character.isLetterOrDigit(context.current()) || context.current() == '-')) {
            selector.append(context.current());
            context.advance();
        }
        
        return selector.toString();
    }
    
    /**
     * 扫描CSS选择器
     */
    private String scanCSSSelector(ScanContext context) {
        StringBuilder selector = new StringBuilder();
        
        // 消费 . 或 #
        selector.append(context.current());
        context.advance();
        
        // 读取选择器名称
        while (!context.isEnd() && (Character.isLetterOrDigit(context.current()) || context.current() == '-' || context.current() == '_')) {
            selector.append(context.current());
            context.advance();
        }
        
        return selector.toString();
    }
    
    /**
     * 合并样式片段
     */
    private CodeFragment mergeStyleFragments(List<CodeFragment> fragments, int startPos) {
        if (fragments.isEmpty()) {
            return new CodeFragment(FragmentType.CHTL_LOCAL_STYLE, "", startPos);
        }
        
        // 如果只有一个片段，直接返回
        if (fragments.size() == 1) {
            return fragments.get(0);
        }
        
        // 合并多个片段
        StringBuilder merged = new StringBuilder();
        FragmentType primaryType = FragmentType.CHTL_LOCAL_STYLE;
        
        for (CodeFragment fragment : fragments) {
            merged.append(fragment.getContent());
            if (fragment.getType() == FragmentType.CHTL_LOCAL_STYLE) {
                primaryType = FragmentType.CHTL_LOCAL_STYLE;
            }
        }
        
        return new CodeFragment(primaryType, merged.toString(), startPos);
    }
    
    /**
     * 合并连续的相同类型片段
     */
    private List<CodeFragment> mergeFragments(List<CodeFragment> fragments) {
        if (fragments.size() <= 1) {
            return fragments;
        }
        
        List<CodeFragment> merged = new ArrayList<>();
        CodeFragment current = fragments.get(0);
        
        for (int i = 1; i < fragments.size(); i++) {
            CodeFragment next = fragments.get(i);
            
            // 如果类型相同且是JS或CSS，考虑合并
            if (current.getType() == next.getType() && 
                (current.getType() == FragmentType.CSS || current.getType() == FragmentType.JAVASCRIPT)) {
                // 合并
                current = new CodeFragment(
                    current.getType(),
                    current.getContent() + next.getContent(),
                    current.getSourcePosition()
                );
            } else {
                // 不合并，添加当前片段
                merged.add(current);
                current = next;
            }
        }
        
        merged.add(current);
        return merged;
    }
    
    /**
     * 扫描上下文
     */
    private static class ScanContext {
        private final String source;
        private int position;
        private int line;
        private int column;
        private ScanMode mode;
        
        ScanContext(String source) {
            this.source = source;
            this.position = 0;
            this.line = 1;
            this.column = 1;
            this.mode = ScanMode.CHTL;
        }
        
        boolean isEnd() {
            return position >= source.length();
        }
        
        char current() {
            return position < source.length() ? source.charAt(position) : '\0';
        }
        
        void advance() {
            if (position < source.length()) {
                if (source.charAt(position) == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
                position++;
            }
        }
        
        String consume(int count) {
            StringBuilder result = new StringBuilder();
            for (int i = 0; i < count && !isEnd(); i++) {
                result.append(current());
                advance();
            }
            return result.toString();
        }
        
        String peekAhead(int count) {
            int end = Math.min(position + count, source.length());
            return source.substring(position, end);
        }
        
        boolean lookingAt(String pattern) {
            return peekAhead(pattern.length() + 10).matches("^" + pattern + ".*");
        }
        
        void skipWhitespace() {
            while (!isEnd() && Character.isWhitespace(current())) {
                advance();
            }
        }
        
        void skipPattern(String pattern) {
            if (lookingAt(pattern)) {
                Pattern p = Pattern.compile("^" + pattern);
                Matcher m = p.matcher(peekAhead(100));
                if (m.find()) {
                    consume(m.end());
                }
            }
        }
        
        boolean isInStyleBlock() {
            return mode == ScanMode.LOCAL_STYLE || mode == ScanMode.GLOBAL_STYLE;
        }
        
        boolean isInScriptBlock() {
            return mode == ScanMode.LOCAL_SCRIPT || mode == ScanMode.GLOBAL_SCRIPT;
        }
    }
    
    /**
     * 片段信息
     */
    private static class FragmentInfo {
        final FragmentType type;
        final int priority;
        
        FragmentInfo(FragmentType type) {
            this(type, 0);
        }
        
        FragmentInfo(FragmentType type, int priority) {
            this.type = type;
            this.priority = priority;
        }
    }
}