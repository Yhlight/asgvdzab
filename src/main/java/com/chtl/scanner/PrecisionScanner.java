package com.chtl.scanner;

import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import com.chtl.compiler.AutoAddManager;
import java.util.*;

/**
 * 精确扫描器 - 超细致版本
 * 实现最小单元的字符级语言边界识别
 * 
 * 核心原则：
 * 1. 每次语言切换都产生新片段
 * 2. {{selector}} 总是CHTL JS
 * 3. function() { ... } 内部是JS
 * 4. CHTL JS函数调用保持为CHTL JS
 * 5. 在任何语言中都可能出现{{}}嵌套
 */
public class PrecisionScanner {
    
    private String input;
    private int position;
    private int line;
    private int column;
    private LanguageContextManager contextManager;
    private AutoAddManager autoAddManager;
    private BraceBalancer braceBalancer;
    private FragmentMerger fragmentMerger;
    private List<CodeFragment> fragments;
    
    // 当前正在构建的片段
    private StringBuilder currentBuffer;
    private FragmentType currentFragmentType;
    private int currentFragmentStart;
    private int currentFragmentStartLine;
    private int currentFragmentStartColumn;
    
    // CHTL JS内置函数列表
    private static final Set<String> CHTLJS_FUNCTIONS = new HashSet<>(Arrays.asList(
        "printMylove",
        "createFeatherRain",
        "createGradientText",
        "listen",
        "delegate",
        "animate"
    ));
    
    public PrecisionScanner(LanguageContextManager contextManager) {
        this.contextManager = contextManager;
        this.autoAddManager = new AutoAddManager(contextManager);
        this.braceBalancer = new BraceBalancer();
        this.fragmentMerger = new FragmentMerger();
        this.fragments = new ArrayList<>();
        this.currentBuffer = new StringBuilder();
    }
    
    /**
     * 扫描输入代码
     */
    public List<CodeFragment> scan(String input) {
        this.input = input;
        this.position = 0;
        this.line = 1;
        this.column = 1;
        this.fragments.clear();
        this.currentBuffer.setLength(0);
        this.currentFragmentStart = 0;
        this.currentFragmentStartLine = 1;
        this.currentFragmentStartColumn = 1;
        
        // 重置上下文
        contextManager.reset();
        autoAddManager.reset();
        braceBalancer.reset();
        
        // 确定初始语言类型
        currentFragmentType = determineInitialFragmentType();
        
        // 主扫描循环
        while (!isAtEnd()) {
            scanNextUnit();
        }
        
        // 处理最后的缓冲区
        flushCurrentFragment();
        
        // 应用片段合并优化
        List<CodeFragment> optimized = fragmentMerger.merge(fragments);
        
        // 打印合并统计信息（调试用）
        if (!fragments.isEmpty()) {
            FragmentMerger.MergeStats stats = new FragmentMerger.MergeStats(fragments, optimized);
            System.err.println(stats);
        }
        
        return optimized;
    }
    
    /**
     * 扫描下一个最小单元
     */
    private void scanNextUnit() {
        // 无论在什么语言中，{{}} 都要特殊处理
        if (current() == '{' && peek() == '{') {
            if (currentFragmentType != FragmentType.CHTL_JS) {
                // 需要切换到CHTL JS
                flushCurrentFragment();
                currentFragmentType = FragmentType.CHTL_JS;
            }
            scanEnhancedSelector();
            return;
        }
        
        // 根据当前语言类型决定如何扫描
        switch (currentFragmentType) {
            case CHTL:
                scanInCHTL();
                break;
                
            case CHTL_JS:
                scanInCHTLJS();
                break;
                
            case JS:
                scanInJS();
                break;
                
            case CSS:
                scanInCSS();
                break;
                
            default:
                consumeChar();
        }
    }
    
    /**
     * 在CHTL中扫描
     */
    private void scanInCHTL() {
        // 检查是否进入特殊块
        if (matchKeyword("script") && lookAheadFor('{')) {
            scanUntilBlockStart("script");
            flushCurrentFragment();
            currentFragmentType = FragmentType.CHTL_JS;
            contextManager.enterContext(
                LanguageContextManager.Language.CHTL_JS,
                LanguageContextManager.ContextType.LOCAL_SCRIPT,
                position, line, column
            );
            return;
        }
        
        if (matchKeyword("style") && lookAheadFor('{')) {
            scanUntilBlockStart("style");
            flushCurrentFragment();
            currentFragmentType = FragmentType.CSS;
            contextManager.enterContext(
                LanguageContextManager.Language.CSS,
                LanguageContextManager.ContextType.LOCAL_STYLE,
                position, line, column
            );
            return;
        }
        
        // 检查是否是CHTL JS函数调用
        String chtljsFunc = lookAheadForCHTLJSFunction();
        if (chtljsFunc != null) {
            // 刷新当前内容
            flushCurrentFragment();
            currentFragmentType = FragmentType.CHTL_JS;
            // 不要在这里消耗函数名，让CHTL JS处理
            return;
        }
        
        // 普通字符
        consumeChar();
    }
    
    /**
     * 在CHTL JS中扫描
     */
    private void scanInCHTLJS() {
        // 检查是否遇到增强选择器
        if (current() == '{' && peek() == '{') {
            // 不刷新当前片段，让{{}}作为CHTL JS的一部分
            scanEnhancedSelector();
            return;
        }
        
        // 检查CHTL JS特殊函数：listen、delegate、animate
        if (isAtCHTLJSFunction()) {
            scanCHTLJSFunction();
            return;
        }
        
        // 检查->操作符
        if (current() == '-' && peek() == '>') {
            consumeChar(); // -
            consumeChar(); // >
            return;
        }
        
        // 检查是否是function关键字
        if (matchKeyword("function") && lookAheadFor('(')) {
            // 扫描function关键字和参数列表
            consumeKeyword("function");
            skipWhitespace();
            if (current() == '(') {
                scanParentheses();
            }
            skipWhitespace();
            
            // 如果后面是{，则准备切换到JS
            if (current() == '{') {
                consumeChar(); // {
                flushCurrentFragment();
                currentFragmentType = FragmentType.JS;
                return;
            }
        }
        
        // 检查是否退出CHTL JS上下文
        if (shouldExitCHTLJS()) {
            flushCurrentFragment();
            currentFragmentType = FragmentType.CHTL;
            return;
        }
        
        // 检查是否退出script块
        if (current() == '}' && 
            contextManager.getCurrentContextType() == LanguageContextManager.ContextType.LOCAL_SCRIPT) {
            consumeChar(); // }
            flushCurrentFragment();
            contextManager.exitContext();
            currentFragmentType = FragmentType.CHTL;
            return;
        }
        
        // 普通字符
        consumeChar();
    }
    
    /**
     * 在JS中扫描
     */
    private void scanInJS() {
        // 在JS中也要检查{{}}
        if (current() == '{' && peek() == '{') {
            flushCurrentFragment();
            currentFragmentType = FragmentType.CHTL_JS;
            scanEnhancedSelector();
            
            // 检查{{}}后是否继续JS
            if (!isAtEnd() && !isCHTLJSContinuation()) {
                flushCurrentFragment();
                currentFragmentType = FragmentType.JS;
            }
            return;
        }
        
        // 处理字符串字面量，避免误判
        if (current() == '"' || current() == '\'' || current() == '`') {
            scanStringLiteral();
            return;
        }
        
        // 检查是否退出JS函数体
        if (current() == '}' && shouldExitJSFunction()) {
            consumeChar(); // }
            flushCurrentFragment();
            currentFragmentType = FragmentType.CHTL_JS;
            return;
        }
        
        // 普通字符
        consumeChar();
    }
    
    /**
     * 在CSS中扫描
     */
    private void scanInCSS() {
        // 检查是否退出style块
        if (current() == '}') {
            // 检查是否真的要退出CSS
            if (balancer.getCurlyDepth() == 0) {
                consumeChar(); // }
                flushCurrentFragment();
                contextManager.exitContext();
                currentFragmentType = FragmentType.CHTL;
                return;
            }
        }
        
        // 只在局部style中处理CHTL变量语法
        if (contextManager.getCurrentContextType() == LanguageContextManager.ContextType.LOCAL_STYLE) {
            // 检查变量函数调用形式：VariableName(property)
            if (Character.isUpperCase(current()) && isVariableFunctionCall()) {
                flushCurrentFragment();
                currentFragmentType = FragmentType.CHTL;
                scanVariableFunctionCall();
                flushCurrentFragment();
                currentFragmentType = FragmentType.CSS;
                return;
            }
            
            // 检查可选的@Var前缀形式
            if (current() == '@' && matchKeyword("@Var")) {
                flushCurrentFragment();
                currentFragmentType = FragmentType.CHTL;
                scanOptionalVarPrefix();
                flushCurrentFragment();
                currentFragmentType = FragmentType.CSS;
                return;
            }
        }
        
        // 处理CSS字符串，避免误判
        if (current() == '"' || current() == '\'') {
            scanCSSString();
            return;
        }
        
        // 处理CSS注释
        if (current() == '/' && peek() == '*') {
            scanCSSComment();
            return;
        }
        
        consumeChar();
    }
    
    /**
     * 扫描增强选择器
     */
    private void scanEnhancedSelector() {
        braceBalancer.enterEnhancedSelector();
        consumeChar(); // {
        consumeChar(); // {
        
        while (!isAtEnd() && !(current() == '}' && peek() == '}')) {
            if (current() == '&' && peek() == '}' && peekAt(2) == '}') {
                // 处理{{&}}
                String resolved = autoAddManager.processSelfReferenceSelector();
                for (char c : resolved.toCharArray()) {
                    currentBuffer.append(c);
                }
                advance(); // 跳过&
            } else {
                consumeChar();
            }
        }
        
        if (current() == '}' && peek() == '}') {
            consumeChar(); // }
            consumeChar(); // }
            braceBalancer.exitEnhancedSelector();
        }
        
        // 检查后续是否还是CHTL JS
        if (!isAtEnd() && !isCHTLJSContinuation()) {
            flushCurrentFragment();
            // 根据上下文决定下一个片段类型
            currentFragmentType = determineNextFragmentType();
        }
    }
    
    /**
     * 扫描字符串字面量
     */
    private void scanStringLiteral() {
        char quote = current();
        braceBalancer.enterString(quote);
        consumeChar();
        
        while (!isAtEnd() && current() != quote) {
            if (current() == '\\') {
                consumeChar(); // \
                if (!isAtEnd()) {
                    consumeChar(); // 转义字符
                }
            } else {
                consumeChar();
            }
        }
        
        if (current() == quote) {
            consumeChar();
            braceBalancer.exitString();
        }
    }
    
    /**
     * 扫描括号内容
     */
    private void scanParentheses() {
        if (current() != '(') return;
        
        consumeChar(); // (
        int depth = 1;
        
        while (!isAtEnd() && depth > 0) {
            if (current() == '(') {
                depth++;
            } else if (current() == ')') {
                depth--;
            }
            consumeChar();
        }
    }
    
    /**
     * 扫描直到块开始
     */
    private void scanUntilBlockStart(String keyword) {
        consumeKeyword(keyword);
        skipWhitespace();
        if (current() == '{') {
            consumeChar();
        }
    }
    
    /**
     * 判断是否应该退出CHTL JS
     */
    private boolean shouldExitCHTLJS() {
        // 如果在LOCAL_SCRIPT中，由}来决定退出
        if (contextManager.getCurrentContextType() == LanguageContextManager.ContextType.LOCAL_SCRIPT) {
            return false;
        }
        
        // 语句结束
        if (current() == ';') {
            // 向前看是否还有CHTL JS特征
            int lookahead = position + 1;
            while (lookahead < input.length() && Character.isWhitespace(input.charAt(lookahead))) {
                lookahead++;
            }
            
            if (lookahead >= input.length()) {
                return true;
            }
            
            // 检查是否是CHTL JS特征
            return !isCHTLJSAt(lookahead);
        }
        
        // 检查是否是CHTL JS函数调用的结束
        if (current() == ')' && !isInParentheses()) {
            // 这可能是CHTL JS函数调用的结束
            int lookahead = position + 1;
            while (lookahead < input.length() && Character.isWhitespace(input.charAt(lookahead))) {
                lookahead++;
            }
            
            if (lookahead >= input.length() || input.charAt(lookahead) == ';') {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * 判断是否应该退出JS函数
     */
    private boolean shouldExitJSFunction() {
        // 简化版本：遇到}就退出
        // 实际应该维护大括号计数
        return true;
    }
    
    /**
     * 判断是否在括号内
     */
    private boolean isInParentheses() {
        // 简化版本
        return false;
    }
    
    /**
     * 判断是否是CHTL JS延续
     */
    private boolean isCHTLJSContinuation() {
        if (isAtEnd()) return false;
        
        char c = current();
        // . 或 -> 表示方法调用链
        return c == '.' || (c == '-' && peek() == '>');
    }
    
    /**
     * 检查指定位置是否是CHTL JS特征
     */
    private boolean isCHTLJSAt(int pos) {
        if (pos + 1 < input.length() && 
            input.charAt(pos) == '{' && input.charAt(pos + 1) == '{') {
            return true;
        }
        
        for (String func : CHTLJS_FUNCTIONS) {
            if (input.startsWith(func, pos)) {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * 确定初始片段类型
     */
    private FragmentType determineInitialFragmentType() {
        int pos = 0;
        while (pos < input.length() && Character.isWhitespace(input.charAt(pos))) {
            pos++;
        }
        
        if (isCHTLJSAt(pos)) {
            return FragmentType.CHTL_JS;
        }
        
        return FragmentType.CHTL;
    }
    
    /**
     * 确定下一个片段类型
     */
    private FragmentType determineNextFragmentType() {
        // 根据上下文栈决定
        LanguageContextManager.ContextType ctx = contextManager.getCurrentContextType();
        
        switch (ctx) {
            case LOCAL_SCRIPT:
                return FragmentType.CHTL_JS;
            case LOCAL_STYLE:
                return FragmentType.CSS;
            case JS_FUNCTION_BODY:
                return FragmentType.JS;
            default:
                return FragmentType.CHTL;
        }
    }
    
    /**
     * 向前查看CHTL JS函数
     */
    private String lookAheadForCHTLJSFunction() {
        int savedPos = position;
        
        // 跳过空白
        while (position < input.length() && Character.isWhitespace(input.charAt(position))) {
            position++;
        }
        
        // 检查每个CHTL JS函数
        for (String func : CHTLJS_FUNCTIONS) {
            if (matchKeyword(func)) {
                // 检查后面是否有(
                int tempPos = position + func.length();
                while (tempPos < input.length() && Character.isWhitespace(input.charAt(tempPos))) {
                    tempPos++;
                }
                
                if (tempPos < input.length() && input.charAt(tempPos) == '(') {
                    position = savedPos; // 恢复位置
                    return func;
                }
            }
        }
        
        position = savedPos; // 恢复位置
        return null;
    }
    
    /**
     * 检查是否匹配关键字
     */
    private boolean matchKeyword(String keyword) {
        if (position + keyword.length() > input.length()) {
            return false;
        }
        
        for (int i = 0; i < keyword.length(); i++) {
            if (input.charAt(position + i) != keyword.charAt(i)) {
                return false;
            }
        }
        
        // 检查后面是否是单词边界
        if (position + keyword.length() < input.length()) {
            char next = input.charAt(position + keyword.length());
            return !Character.isLetterOrDigit(next) && next != '_';
        }
        
        return true;
    }
    
    /**
     * 向前查看特定字符
     */
    private boolean lookAheadFor(char ch) {
        int pos = position;
        
        // 跳过关键字
        while (pos < input.length() && Character.isLetterOrDigit(input.charAt(pos))) {
            pos++;
        }
        
        // 跳过空白
        while (pos < input.length() && Character.isWhitespace(input.charAt(pos))) {
            pos++;
        }
        
        return pos < input.length() && input.charAt(pos) == ch;
    }
    
    /**
     * 消耗一个字符
     */
    private void consumeChar() {
        if (!isAtEnd()) {
            currentBuffer.append(current());
            advance();
        }
    }
    
    /**
     * 消耗关键字
     */
    private void consumeKeyword(String keyword) {
        for (int i = 0; i < keyword.length() && !isAtEnd(); i++) {
            consumeChar();
        }
    }
    
    /**
     * 跳过空白（并记录）
     */
    private void skipWhitespace() {
        while (!isAtEnd() && Character.isWhitespace(current())) {
            consumeChar();
        }
    }
    
    /**
     * 刷新当前片段
     */
    private void flushCurrentFragment() {
        if (currentBuffer.length() == 0) {
            return;
        }
        
        CodeFragment fragment = new CodeFragment(
            currentFragmentType,
            currentBuffer.toString(),
            currentFragmentStart,
            position - currentFragmentStart
        );
        
        fragment.setLine(currentFragmentStartLine);
        fragment.setColumn(currentFragmentStartColumn);
        
        fragments.add(fragment);
        
        // 重置缓冲区
        currentBuffer.setLength(0);
        currentFragmentStart = position;
        currentFragmentStartLine = line;
        currentFragmentStartColumn = column;
    }
    
    // ===== 辅助方法 =====
    
    private boolean isAtEnd() {
        return position >= input.length();
    }
    
    private char current() {
        return isAtEnd() ? '\0' : input.charAt(position);
    }
    
    private char peek() {
        return position + 1 >= input.length() ? '\0' : input.charAt(position + 1);
    }
    
    private char peekAt(int offset) {
        return position + offset >= input.length() ? '\0' : input.charAt(position + offset);
    }
    
    private void advance() {
        if (!isAtEnd()) {
            if (current() == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            position++;
        }
    }
    
    /**
     * 检查是否是变量函数调用
     */
    private boolean isVariableFunctionCall() {
        int savePos = position;
        int saveLine = line;
        int saveColumn = column;
        
        // 跳过变量名（首字母大写）
        if (!Character.isUpperCase(current())) {
            return false;
        }
        
        while (!isAtEnd() && (Character.isLetterOrDigit(current()) || current() == '_')) {
            advance();
        }
        
        skipWhitespace();
        boolean result = current() == '(';
        
        // 恢复位置
        position = savePos;
        line = saveLine;
        column = saveColumn;
        return result;
    }
    
    /**
     * 扫描变量函数调用
     */
    private void scanVariableFunctionCall() {
        // 扫描变量名
        while (!isAtEnd() && (Character.isLetterOrDigit(current()) || current() == '_')) {
            consumeChar();
        }
        
        skipWhitespace();
        
        if (current() == '(') {
            consumeChar(); // (
            
            // 扫描属性名
            while (!isAtEnd() && current() != ')') {
                consumeChar();
            }
            
            if (current() == ')') {
                consumeChar(); // )
            }
        }
    }
    
    /**
     * 扫描可选的@Var前缀
     */
    private void scanOptionalVarPrefix() {
        // 消费 @Var
        consumeKeyword("@Var");
        skipWhitespace();
        
        // 扫描变量路径 (e.g., ThemeColor.tableColor)
        while (!isAtEnd() && (Character.isLetterOrDigit(current()) || current() == '.' || current() == '_')) {
            consumeChar();
        }
    }
    
    /**
     * 扫描CSS字符串
     */
    private void scanCSSString() {
        char quote = current();
        consumeChar(); // 开始引号
        
        while (!isAtEnd() && current() != quote) {
            if (current() == '\\') {
                consumeChar(); // 转义字符
                if (!isAtEnd()) {
                    consumeChar(); // 被转义的字符
                }
            } else {
                consumeChar();
            }
        }
        
        if (!isAtEnd()) {
            consumeChar(); // 结束引号
        }
    }
    
    /**
     * 扫描CSS注释
     */
    private void scanCSSComment() {
        consumeChar(); // /
        consumeChar(); // *
        
        while (!isAtEnd()) {
            if (current() == '*' && peek() == '/') {
                consumeChar(); // *
                consumeChar(); // /
                break;
            }
            consumeChar();
        }
    }
    
    /**
     * 检查是否在CHTL JS函数位置
     */
    private boolean isAtCHTLJSFunction() {
        // 检查.listen、.delegate
        if (current() == '.') {
            int savePos = position;
            advance(); // skip .
            boolean result = matchKeyword("listen") || matchKeyword("delegate");
            position = savePos; // restore
            return result;
        }
        
        // 检查全局animate函数
        if (matchKeyword("animate") && lookAheadFor('(')) {
            return true;
        }
        
        return false;
    }
    
    /**
     * 扫描CHTL JS函数
     */
    private void scanCHTLJSFunction() {
        if (current() == '.') {
            consumeChar(); // .
        }
        
        // 扫描函数名
        if (matchKeyword("listen")) {
            consumeKeyword("listen");
        } else if (matchKeyword("delegate")) {
            consumeKeyword("delegate");
        } else if (matchKeyword("animate")) {
            consumeKeyword("animate");
        }
        
        skipWhitespace();
        
        // 扫描参数
        if (current() == '(') {
            scanFunctionArguments();
        }
    }
    
    /**
     * 扫描函数参数，处理嵌套的对象和函数
     */
    private void scanFunctionArguments() {
        consumeChar(); // (
        int parenDepth = 1;
        
        while (!isAtEnd() && parenDepth > 0) {
            if (current() == '(') {
                parenDepth++;
            } else if (current() == ')') {
                parenDepth--;
            } else if (current() == '{') {
                // 可能是对象字面量或函数体
                scanPossibleObjectOrFunction();
                continue;
            } else if (current() == '"' || current() == '\'') {
                scanJSString();
                continue;
            }
            
            consumeChar();
        }
    }
    
    /**
     * 扫描可能的对象字面量或函数体
     */
    private void scanPossibleObjectOrFunction() {
        int braceDepth = 0;
        boolean inFunction = false;
        boolean inCHTLJSContext = currentFragmentType == FragmentType.CHTL_JS;
        
        do {
            if (current() == '{') {
                // 检查是否是增强选择器
                if (peek() == '{' && inCHTLJSContext) {
                    scanEnhancedSelector();
                    continue;
                }
                
                braceDepth++;
                consumeChar();
                
                // 检查是否进入函数体
                if (braceDepth == 1 && lookBackFor("function")) {
                    flushCurrentFragment();
                    currentFragmentType = FragmentType.JS;
                    inFunction = true;
                }
            } else if (current() == '}') {
                consumeChar();
                braceDepth--;
                
                // 退出函数体
                if (braceDepth == 0 && inFunction) {
                    flushCurrentFragment();
                    currentFragmentType = FragmentType.CHTL_JS;
                    inFunction = false;
                }
            } else if (current() == '"' || current() == '\'') {
                scanJSString();
            } else if (inCHTLJSContext && !inFunction) {
                // 在CHTL JS上下文中检查特殊语法
                if (current() == '-' && peek() == '>') {
                    consumeChar(); // -
                    consumeChar(); // >
                } else if (current() == '.') {
                    int savePos = position;
                    advance(); // skip .
                    boolean isFunction = matchKeyword("listen") || matchKeyword("delegate");
                    position = savePos; // restore
                    if (isFunction) {
                        // 切割点：方法调用前
                        flushCurrentFragment();
                        scanCHTLJSFunction();
                    } else {
                        consumeChar();
                    }
                } else {
                    consumeChar();
                }
            } else {
                consumeChar();
            }
        } while (!isAtEnd() && braceDepth > 0);
    }
    
    /**
     * 扫描JS字符串
     */
    private void scanJSString() {
        char quote = current();
        consumeChar(); // 开始引号
        
        while (!isAtEnd() && current() != quote) {
            if (current() == '\\') {
                consumeChar(); // 转义字符
                if (!isAtEnd()) {
                    consumeChar(); // 被转义的字符
                }
            } else {
                consumeChar();
            }
        }
        
        if (!isAtEnd()) {
            consumeChar(); // 结束引号
        }
    }
    
    /**
     * 向后查找关键字
     */
    private boolean lookBackFor(String keyword) {
        int len = keyword.length();
        if (position < len) {
            return false;
        }
        
        // 检查源代码中的关键字
        for (int i = 0; i < len; i++) {
            if (input.charAt(position - len + i) != keyword.charAt(i)) {
                return false;
            }
        }
        
        // 确保关键字前面是空白或非字母
        if (position > len) {
            char before = input.charAt(position - len - 1);
            if (Character.isLetterOrDigit(before)) {
                return false;
            }
        }
        
        return true;
    }
    
}