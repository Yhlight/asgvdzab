package com.chtl.scanner;

import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import com.chtl.compiler.AutoAddManager;
import java.util.*;

/**
 * 精确扫描器
 * 基于最小单元的字符级语言边界识别
 * 
 * 核心思想：
 * 1. {{selector}} 是CHTL JS的边界标记
 * 2. function() { ... } 内部是JS
 * 3. CHTL JS函数调用（如printMylove）的参数部分保持为CHTL JS
 * 4. 其他部分根据上下文判断
 */
public class PrecisionScanner {
    
    private String input;
    private int position;
    private int line;
    private int column;
    private LanguageContextManager contextManager;
    private AutoAddManager autoAddManager;
    private List<CodeFragment> fragments;
    
    // 当前正在构建的片段
    private StringBuilder currentBuffer;
    private FragmentType currentFragmentType;
    private int currentFragmentStart;
    private int currentFragmentStartLine;
    private int currentFragmentStartColumn;
    
    // 函数体大括号计数
    private int functionBraceDepth = 0;
    
    // CHTL JS内置函数列表（这些函数的调用保持为CHTL JS）
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
        this.functionBraceDepth = 0;
        
        // 重置上下文
        contextManager.reset();
        autoAddManager.reset();
        
        // 初始片段类型基于上下文
        currentFragmentType = determineInitialFragmentType();
        
        while (!isAtEnd()) {
            scanNextToken();
        }
        
        // 处理最后的缓冲区
        flushCurrentFragment();
        
        return fragments;
    }
    
    /**
     * 确定初始片段类型
     */
    private FragmentType determineInitialFragmentType() {
        // 检查是否以CHTL JS特征开始
        if (startsWithCHTLJS()) {
            return FragmentType.CHTL_JS;
        }
        // 默认为CHTL
        return FragmentType.CHTL;
    }
    
    /**
     * 检查是否以CHTL JS特征开始
     */
    private boolean startsWithCHTLJS() {
        int pos = 0;
        // 跳过空白
        while (pos < input.length() && Character.isWhitespace(input.charAt(pos))) {
            pos++;
        }
        
        // 检查是否以{{开始
        if (pos + 1 < input.length() && 
            input.charAt(pos) == '{' && input.charAt(pos + 1) == '{') {
            return true;
        }
        
        // 检查是否以CHTL JS函数开始
        for (String func : CHTLJS_FUNCTIONS) {
            if (input.startsWith(func, pos)) {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * 扫描下一个最小单元
     */
    private void scanNextToken() {
        // 检查是否是增强选择器 {{...}}
        if (current() == '{' && peek() == '{') {
            handleEnhancedSelector();
            return;
        }
        
        // 检查是否是函数定义 function
        if (matchKeyword("function") && lookAheadForFunctionPattern()) {
            handleFunctionKeyword();
            return;
        }
        
        // 在JS函数体内的特殊处理
        if (contextManager.getCurrentContextType() == LanguageContextManager.ContextType.JS_FUNCTION_BODY) {
            scanInJavaScript();
            return;
        }
        
        // 检查是否是CHTL JS函数调用
        String functionName = tryMatchCHTLJSFunction();
        if (functionName != null) {
            handleCHTLJSFunction(functionName);
            return;
        }
        
        // 根据语言上下文扫描
        switch (contextManager.getCurrentLanguage()) {
            case CHTL:
                scanInCHTL();
                break;
            case CHTL_JS:
                scanInCHTLJS();
                break;
            case JAVASCRIPT:
                scanInJavaScript();
                break;
            case CSS:
                scanInCSS();
                break;
            default:
                appendCurrentChar();
                advance();
        }
    }
    
    /**
     * 检查是否符合function模式
     */
    private boolean lookAheadForFunctionPattern() {
        int savedPos = position;
        int savedLine = line;
        int savedCol = column;
        
        // 跳过 "function"
        for (int i = 0; i < 8; i++) {
            if (isAtEnd()) {
                position = savedPos;
                line = savedLine;
                column = savedCol;
                return false;
            }
            advance();
        }
        
        // 跳过空白
        while (!isAtEnd() && Character.isWhitespace(current())) {
            advance();
        }
        
        // 检查是否有参数列表
        boolean hasParens = current() == '(';
        
        // 恢复位置
        position = savedPos;
        line = savedLine;
        column = savedCol;
        
        return hasParens;
    }
    
    /**
     * 处理增强选择器 {{...}}
     */
    private void handleEnhancedSelector() {
        // 切换到CHTL JS片段
        if (currentFragmentType != FragmentType.CHTL_JS) {
            flushCurrentFragment();
            currentFragmentType = FragmentType.CHTL_JS;
        }
        
        currentBuffer.append("{{");
        advance(); // 跳过第一个 {
        advance(); // 跳过第二个 {
        
        // 扫描到 }}
        while (!isAtEnd() && !(current() == '}' && peek() == '}')) {
            if (current() == '&' && peek() == '}' && peekNext() == '}') {
                // 处理 {{&}}
                String resolved = autoAddManager.processSelfReferenceSelector();
                currentBuffer.append(resolved);
                advance(); // 跳过 &
            } else {
                appendCurrentChar();
                advance();
            }
        }
        
        // 添加结束的 }}
        if (current() == '}' && peek() == '}') {
            currentBuffer.append("}}");
            advance();
            advance();
        }
    }
    
    /**
     * 处理function关键字
     */
    private void handleFunctionKeyword() {
        // function关键字属于当前语言环境
        appendKeyword("function");
        
        // 跳过空白
        skipWhitespaceWithCapture();
        
        // 扫描参数列表
        if (current() == '(') {
            scanUntilClosingParen();
        }
        
        skipWhitespaceWithCapture();
        
        // 如果后面是 {，则函数体内部切换到JS
        if (current() == '{') {
            appendCurrentChar(); // {
            advance();
            
            // 刷新当前片段，准备切换
            flushCurrentFragment();
            
            // 切换到JS，并初始化大括号深度
            currentFragmentType = FragmentType.JS;
            functionBraceDepth = 1;  // 我们已经消费了开始的 {
            contextManager.enterContext(
                LanguageContextManager.Language.JAVASCRIPT,
                LanguageContextManager.ContextType.JS_FUNCTION_BODY,
                position, line, column
            );
        }
    }
    
    /**
     * 尝试匹配CHTL JS函数
     */
    private String tryMatchCHTLJSFunction() {
        // 如果在JS函数体内，不检查CHTL JS函数
        if (contextManager.getCurrentContextType() == LanguageContextManager.ContextType.JS_FUNCTION_BODY) {
            return null;
        }
        
        // 保存当前状态
        int savedPos = position;
        int savedLine = line;
        int savedCol = column;
        
        // 跳过前导空白
        while (!isAtEnd() && Character.isWhitespace(current())) {
            advance();
        }
        
        // 尝试匹配标识符
        StringBuilder identifier = new StringBuilder();
        while (!isAtEnd() && (Character.isLetterOrDigit(current()) || current() == '_')) {
            identifier.append(current());
            advance();
        }
        
        String id = identifier.toString();
        
        // 跳过空白
        while (!isAtEnd() && Character.isWhitespace(current())) {
            advance();
        }
        
        // 检查是否后跟 (
        if (current() == '(' && CHTLJS_FUNCTIONS.contains(id)) {
            // 恢复位置，返回匹配的函数名
            position = savedPos;
            line = savedLine;
            column = savedCol;
            return id;
        }
        
        // 没有匹配，恢复位置
        position = savedPos;
        line = savedLine;
        column = savedCol;
        return null;
    }
    
    /**
     * 处理CHTL JS函数调用
     */
    private void handleCHTLJSFunction(String functionName) {
        // 如果之前有非空白内容且不是CHTL JS，先刷新
        if (currentBuffer.length() > 0 && currentFragmentType != FragmentType.CHTL_JS) {
            // 回退到函数名之前的空白
            String content = currentBuffer.toString();
            int lastNonWhitespace = content.length() - 1;
            while (lastNonWhitespace >= 0 && Character.isWhitespace(content.charAt(lastNonWhitespace))) {
                lastNonWhitespace--;
            }
            
            if (lastNonWhitespace >= 0) {
                // 保留非空白部分
                currentBuffer.setLength(lastNonWhitespace + 1);
                flushCurrentFragment();
                
                // 添加空白到新片段
                for (int i = lastNonWhitespace + 1; i < content.length(); i++) {
                    currentBuffer.append(content.charAt(i));
                }
            }
        }
        
        // 切换到CHTL JS
        currentFragmentType = FragmentType.CHTL_JS;
        
        // 添加函数名
        appendKeyword(functionName);
        skipWhitespaceWithCapture();
        
        // 处理参数列表
        if (current() == '(') {
            scanFunctionArguments();
        }
    }
    
    /**
     * 扫描函数参数（保持为CHTL JS）
     */
    private void scanFunctionArguments() {
        appendCurrentChar(); // (
        advance();
        
        int parenCount = 1;
        
        while (!isAtEnd() && parenCount > 0) {
            if (current() == '(') {
                parenCount++;
            } else if (current() == ')') {
                parenCount--;
            } else if (current() == '{' && peek() == '{') {
                // 参数中的增强选择器
                handleEnhancedSelector();
                continue;
            } else if (matchKeyword("function") && parenCount == 1) {
                // 参数中的function
                handleFunctionKeyword();
                continue;
            }
            
            appendCurrentChar();
            advance();
        }
    }
    
    /**
     * 在CHTL上下文中扫描
     */
    private void scanInCHTL() {
        if (matchKeyword("style") && peekAfterWhitespace() == '{') {
            handleStyleBlock();
        } else if (matchKeyword("script") && peekAfterWhitespace() == '{') {
            handleScriptBlock();
        } else {
            appendCurrentChar();
            advance();
        }
    }
    
    /**
     * 在CHTL JS上下文中扫描
     */
    private void scanInCHTLJS() {
        // 检查是否退出脚本块
        if (current() == '}') {
            if (contextManager.getCurrentContextType() == LanguageContextManager.ContextType.LOCAL_SCRIPT) {
                // 需要检查是否真的是脚本块的结束
                // 简化实现：假设单个}就是结束
                appendCurrentChar();
                advance();
                flushCurrentFragment();
                contextManager.exitContext();
                currentFragmentType = FragmentType.CHTL;
                return;
            }
        }
        
        appendCurrentChar();
        advance();
    }
    
    /**
     * 在JavaScript上下文中扫描
     */
    private void scanInJavaScript() {
        // 在JS中，检查是否有CHTL JS嵌套
        if (current() == '{' && peek() == '{') {
            // 刷新JS片段
            flushCurrentFragment();
            
            // 切换到CHTL JS处理增强选择器
            currentFragmentType = FragmentType.CHTL_JS;
            handleEnhancedSelector();
            
            // 之后继续JS
            flushCurrentFragment();
            currentFragmentType = FragmentType.JS;
            return;
        }
        
        // 跟踪大括号深度
        if (current() == '{') {
            functionBraceDepth++;
        } else if (current() == '}') {
            functionBraceDepth--;
            
            // 如果大括号平衡了，退出函数体
            if (functionBraceDepth == 0 && 
                contextManager.getCurrentContextType() == LanguageContextManager.ContextType.JS_FUNCTION_BODY) {
                appendCurrentChar(); // }
                advance();
                
                // 退出JS上下文
                flushCurrentFragment();
                contextManager.exitContext();
                
                // 根据父上下文决定片段类型
                updateFragmentType();
                return;
            }
        }
        
        appendCurrentChar();
        advance();
    }
    
    /**
     * 在CSS上下文中扫描
     */
    private void scanInCSS() {
        if (current() == '}') {
            if (contextManager.getCurrentContextType() == LanguageContextManager.ContextType.LOCAL_STYLE) {
                appendCurrentChar();
                advance();
                flushCurrentFragment();
                contextManager.exitContext();
                currentFragmentType = FragmentType.CHTL;
                return;
            }
        }
        
        appendCurrentChar();
        advance();
    }
    
    /**
     * 处理style块
     */
    private void handleStyleBlock() {
        flushCurrentFragment();
        currentFragmentType = FragmentType.CHTL;
        
        appendKeyword("style");
        skipWhitespaceWithCapture();
        
        if (current() == '{') {
            appendCurrentChar();
            advance();
            
            flushCurrentFragment();
            currentFragmentType = FragmentType.CSS;
            contextManager.enterContext(
                LanguageContextManager.Language.CSS,
                LanguageContextManager.ContextType.LOCAL_STYLE,
                position, line, column
            );
        }
    }
    
    /**
     * 处理script块
     */
    private void handleScriptBlock() {
        flushCurrentFragment();
        currentFragmentType = FragmentType.CHTL;
        
        appendKeyword("script");
        skipWhitespaceWithCapture();
        
        if (current() == '{') {
            appendCurrentChar();
            advance();
            
            flushCurrentFragment();
            currentFragmentType = FragmentType.CHTL_JS;
            contextManager.enterContext(
                LanguageContextManager.Language.CHTL_JS,
                LanguageContextManager.ContextType.LOCAL_SCRIPT,
                position, line, column
            );
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
    
    /**
     * 根据当前上下文更新片段类型
     */
    private void updateFragmentType() {
        switch (contextManager.getCurrentLanguage()) {
            case CHTL:
                currentFragmentType = FragmentType.CHTL;
                break;
            case CHTL_JS:
                currentFragmentType = FragmentType.CHTL_JS;
                break;
            case JAVASCRIPT:
                currentFragmentType = FragmentType.JS;
                break;
            case CSS:
                currentFragmentType = FragmentType.CSS;
                break;
            default:
                currentFragmentType = FragmentType.CHTL;
        }
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
    
    private char peekNext() {
        return position + 2 >= input.length() ? '\0' : input.charAt(position + 2);
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
    
    private void appendCurrentChar() {
        if (!isAtEnd()) {
            currentBuffer.append(current());
        }
    }
    
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
    
    private void appendKeyword(String keyword) {
        for (int i = 0; i < keyword.length(); i++) {
            appendCurrentChar();
            advance();
        }
    }
    
    private void skipWhitespaceWithCapture() {
        while (!isAtEnd() && Character.isWhitespace(current())) {
            appendCurrentChar();
            advance();
        }
    }
    
    private char peekAfterWhitespace() {
        int tempPos = position;
        while (tempPos < input.length() && Character.isWhitespace(input.charAt(tempPos))) {
            tempPos++;
        }
        return tempPos < input.length() ? input.charAt(tempPos) : '\0';
    }
    
    private void scanUntilClosingParen() {
        appendCurrentChar(); // (
        advance();
        
        int parenCount = 1;
        while (!isAtEnd() && parenCount > 0) {
            if (current() == '(') {
                parenCount++;
            } else if (current() == ')') {
                parenCount--;
            }
            appendCurrentChar();
            advance();
        }
    }
}