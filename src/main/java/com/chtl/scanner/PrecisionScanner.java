package com.chtl.scanner;

import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.*;

/**
 * 精确扫描器
 * 字符级别的语言边界识别，支持CHTL、CHTL JS和JS的嵌套
 */
public class PrecisionScanner {
    
    private String input;
    private int position;
    private int line;
    private int column;
    private LanguageContextManager contextManager;
    private AutoAddManager autoAddManager;
    private List<CodeFragment> fragments;
    private StringBuilder currentBuffer;
    private int currentFragmentStart;
    
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
        
        // 重置上下文
        contextManager.reset();
        autoAddManager.reset();
        
        while (!isAtEnd()) {
            scanNext();
        }
        
        // 处理最后的缓冲区
        flushCurrentFragment();
        
        return fragments;
    }
    
    /**
     * 扫描下一个语言片段
     */
    private void scanNext() {
        char ch = current();
        
        switch (contextManager.getCurrentLanguage()) {
            case CHTL:
                scanCHTL();
                break;
                
            case CHTL_JS:
                scanCHTLJS();
                break;
                
            case JAVASCRIPT:
                scanJavaScript();
                break;
                
            case CSS:
                scanCSS();
                break;
                
            default:
                advance();
        }
    }
    
    /**
     * 扫描CHTL代码
     */
    private void scanCHTL() {
        // 检查局部样式块
        if (matchKeyword("style") && peekNext() == '{') {
            flushCurrentFragment();
            contextManager.enterContext(
                LanguageContextManager.Language.CSS,
                LanguageContextManager.ContextType.LOCAL_STYLE,
                position, line, column
            );
            consumeKeyword("style");
            consume('{');
            currentFragmentStart = position;
            return;
        }
        
        // 检查局部脚本块
        if (matchKeyword("script") && peekNext() == '{') {
            flushCurrentFragment();
            contextManager.enterContext(
                LanguageContextManager.Language.CHTL_JS,
                LanguageContextManager.ContextType.LOCAL_SCRIPT,
                position, line, column
            );
            consumeKeyword("script");
            consume('{');
            currentFragmentStart = position;
            return;
        }
        
        // 检查元素开始
        if (isIdentifierStart(current()) && lookAheadForElementStart()) {
            String elementName = scanIdentifier();
            currentBuffer.append(elementName);
            autoAddManager.enterElementScope(elementName);
        } else {
            currentBuffer.append(current());
            advance();
        }
    }
    
    /**
     * 扫描CHTL JS代码
     */
    private void scanCHTLJS() {
        // 检查增强选择器
        if (current() == '{' && peek() == '{') {
            currentBuffer.append("{{");
            advance();
            advance();
            
            // 处理{{&}}
            if (current() == '&' && peek() == '}' && peekNext() == '}') {
                String resolved = autoAddManager.processSelfReferenceSelector();
                currentBuffer.append(resolved);
                advance(); // 跳过&
                advance(); // 跳过}
                advance(); // 跳过}
                currentBuffer.append("}}");
            } else {
                contextManager.enterContext(
                    LanguageContextManager.Language.CHTL_JS,
                    LanguageContextManager.ContextType.ENHANCED_SELECTOR,
                    position, line, column
                );
            }
            return;
        }
        
        // 检查JS函数体（function关键字后）
        if (matchKeyword("function")) {
            currentBuffer.append("function");
            consumeKeyword("function");
            
            // 跳过参数列表
            skipWhitespace();
            if (current() == '(') {
                int parenCount = 0;
                do {
                    if (current() == '(') parenCount++;
                    if (current() == ')') parenCount--;
                    currentBuffer.append(current());
                    advance();
                } while (parenCount > 0 && !isAtEnd());
            }
            
            skipWhitespace();
            if (current() == '{') {
                // 进入JS函数体上下文
                flushCurrentFragment();
                contextManager.enterContext(
                    LanguageContextManager.Language.JAVASCRIPT,
                    LanguageContextManager.ContextType.JS_FUNCTION_BODY,
                    position, line, column
                );
                currentBuffer.append('{');
                advance();
                currentFragmentStart = position;
            }
            return;
        }
        
        // 检查闭合大括号
        if (current() == '}') {
            if (contextManager.getCurrentContextType() == LanguageContextManager.ContextType.LOCAL_SCRIPT) {
                flushCurrentFragment();
                contextManager.exitContext();
                currentBuffer.append('}');
                advance();
                currentFragmentStart = position;
                return;
            }
        }
        
        currentBuffer.append(current());
        advance();
    }
    
    /**
     * 扫描JavaScript代码
     */
    private void scanJavaScript() {
        // 在JS上下文中，支持所有JS语法，包括=>
        
        // 检查字符串字面量
        if (current() == '"' || current() == '\'' || current() == '`') {
            char quote = current();
            currentBuffer.append(quote);
            advance();
            
            while (!isAtEnd() && current() != quote) {
                if (current() == '\\') {
                    currentBuffer.append(current());
                    advance();
                    if (!isAtEnd()) {
                        currentBuffer.append(current());
                        advance();
                    }
                } else {
                    // 检查模板字符串中的嵌入表达式
                    if (quote == '`' && current() == '$' && peek() == '{') {
                        // TODO: 处理模板字符串中的表达式
                    }
                    currentBuffer.append(current());
                    advance();
                }
            }
            
            if (current() == quote) {
                currentBuffer.append(quote);
                advance();
            }
            return;
        }
        
        // 检查函数体结束
        if (current() == '}') {
            if (contextManager.getCurrentContextType() == LanguageContextManager.ContextType.JS_FUNCTION_BODY) {
                flushCurrentFragment();
                contextManager.exitContext();
                currentBuffer.append('}');
                advance();
                currentFragmentStart = position;
                return;
            }
        }
        
        currentBuffer.append(current());
        advance();
    }
    
    /**
     * 扫描CSS代码
     */
    private void scanCSS() {
        // 检查局部样式块结束
        if (current() == '}') {
            if (contextManager.getCurrentContextType() == LanguageContextManager.ContextType.LOCAL_STYLE) {
                // 检查是否是嵌套的大括号
                int braceCount = countNestedBraces();
                if (braceCount == 0) {
                    flushCurrentFragment();
                    contextManager.exitContext();
                    currentBuffer.append('}');
                    advance();
                    currentFragmentStart = position;
                    return;
                }
            }
        }
        
        // 处理CSS选择器中的自动添加
        if (contextManager.getCurrentContextType() == LanguageContextManager.ContextType.LOCAL_STYLE) {
            if (current() == '.' || current() == '#') {
                char prefix = current();
                currentBuffer.append(prefix);
                advance();
                
                String identifier = scanCSSIdentifier();
                currentBuffer.append(identifier);
                
                // 处理自动添加
                String selector = String.valueOf(prefix) + identifier;
                autoAddManager.processStyleSelector(selector);
                return;
            }
        }
        
        currentBuffer.append(current());
        advance();
    }
    
    /**
     * 刷新当前片段
     */
    private void flushCurrentFragment() {
        if (currentBuffer.length() == 0) {
            return;
        }
        
        FragmentType type = mapLanguageToFragmentType(contextManager.getCurrentLanguage());
        String content = currentBuffer.toString();
        
        // 添加自动生成的属性
        Map<String, Set<String>> autoAttributes = autoAddManager.getAutoAddedAttributes();
        
        CodeFragment fragment = new CodeFragment(
            type,
            content,
            currentFragmentStart,
            position - currentFragmentStart
        );
        
        // 如果有自动添加的属性，记录到fragment中
        if (!autoAttributes.isEmpty()) {
            fragment.setMetadata("autoAttributes", autoAttributes);
        }
        
        fragments.add(fragment);
        currentBuffer.setLength(0);
        currentFragmentStart = position;
    }
    
    /**
     * 映射语言类型到片段类型
     */
    private FragmentType mapLanguageToFragmentType(LanguageContextManager.Language language) {
        switch (language) {
            case CHTL:
                return FragmentType.CHTL;
            case CHTL_JS:
                return FragmentType.CHTL_JS;
            case JAVASCRIPT:
                return FragmentType.JS;
            case CSS:
                return FragmentType.CSS;
            default:
                return FragmentType.CHTL;
        }
    }
    
    // 辅助方法
    
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
    
    private void consume(char expected) {
        if (current() == expected) {
            currentBuffer.append(expected);
            advance();
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
            return !isIdentifierPart(next);
        }
        
        return true;
    }
    
    private void consumeKeyword(String keyword) {
        for (int i = 0; i < keyword.length(); i++) {
            currentBuffer.append(current());
            advance();
        }
    }
    
    private String scanIdentifier() {
        StringBuilder id = new StringBuilder();
        while (isIdentifierPart(current())) {
            id.append(current());
            advance();
        }
        return id.toString();
    }
    
    private String scanCSSIdentifier() {
        StringBuilder id = new StringBuilder();
        while (isCSSIdentifierPart(current())) {
            id.append(current());
            advance();
        }
        return id.toString();
    }
    
    private void skipWhitespace() {
        while (Character.isWhitespace(current())) {
            currentBuffer.append(current());
            advance();
        }
    }
    
    private boolean isIdentifierStart(char ch) {
        return Character.isLetter(ch) || ch == '_' || ch == '$';
    }
    
    private boolean isIdentifierPart(char ch) {
        return Character.isLetterOrDigit(ch) || ch == '_' || ch == '$';
    }
    
    private boolean isCSSIdentifierPart(char ch) {
        return isIdentifierPart(ch) || ch == '-';
    }
    
    private boolean lookAheadForElementStart() {
        int savePos = position;
        int saveCol = column;
        int saveLine = line;
        
        // 跳过标识符
        while (isIdentifierPart(current())) {
            advance();
        }
        
        // 跳过空白
        while (Character.isWhitespace(current())) {
            advance();
        }
        
        boolean result = current() == '{';
        
        // 恢复位置
        position = savePos;
        column = saveCol;
        line = saveLine;
        
        return result;
    }
    
    private int countNestedBraces() {
        // 简化实现，实际应该追踪完整的括号栈
        return 0;
    }
}