package com.chtl.scanner;

import com.chtl.context.CompilationContext;
import com.chtl.context.CompilationError;
import java.util.*;

/**
 * CHTL扫描器状态机
 * 用于词法分析和标记识别
 */
public class ScannerStateMachine {
    
    /**
     * 扫描器状态
     */
    public enum State {
        INITIAL,
        IN_TAG,
        IN_ATTRIBUTE,
        IN_TEXT,
        IN_COMMENT,
        IN_TEMPLATE,
        IN_CUSTOM,
        IN_STYLE,
        IN_SCRIPT,
        IN_ORIGIN,
        IN_STRING,
        IN_EXPRESSION,
        COMPLETED
    }
    
    /**
     * 标记类型
     */
    public enum TokenType {
        // 基本标记
        TAG_OPEN,           // <
        TAG_CLOSE,          // >
        TAG_END,            // </
        SELF_CLOSING,       // />
        
        // 特殊标记
        TEMPLATE_OPEN,      // [Template]
        CUSTOM_OPEN,        // [Custom]
        
        // 块标记
        TEXT_BLOCK,         // text{
        STYLE_BLOCK,        // style{
        SCRIPT_BLOCK,       // script{
        ORIGIN_BLOCK,       // origin{
        
        // 其他
        ATTRIBUTE_NAME,
        ATTRIBUTE_VALUE,
        TEXT,
        COMMENT,
        WHITESPACE,
        
        // 特殊字符
        LEFT_BRACE,         // {
        RIGHT_BRACE,        // }
        LEFT_BRACKET,       // [
        RIGHT_BRACKET,      // ]
        AT_SIGN,            // @
        EQUALS,             // =
        QUOTE,              // "
        
        // 关键字
        KEYWORD_EXCEPT,     // except
        KEYWORD_IMPORT,     // import
        KEYWORD_INHERIT,    // inherit
        KEYWORD_DELETE,     // delete
        KEYWORD_INSERT,     // insert
        
        // 标识符和字面量
        IDENTIFIER,
        STRING_LITERAL,
        NUMBER_LITERAL,
        
        EOF
    }
    
    /**
     * 标记
     */
    public static class Token {
        private final TokenType type;
        private final String value;
        private final int line;
        private final int column;
        
        public Token(TokenType type, String value, int line, int column) {
            this.type = type;
            this.value = value;
            this.line = line;
            this.column = column;
        }
        
        public TokenType getType() {
            return type;
        }
        
        public String getValue() {
            return value;
        }
        
        public int getLine() {
            return line;
        }
        
        public int getColumn() {
            return column;
        }
        
        @Override
        public String toString() {
            return String.format("Token[%s, '%s' at %d:%d]", type, value, line, column);
        }
    }
    
    private final CompilationContext context;
    private State currentState;
    private final List<Token> tokens;
    private String input;
    private int position;
    private int line;
    private int column;
    private final StringBuilder currentToken;
    
    public ScannerStateMachine(CompilationContext context) {
        this.context = context;
        this.currentState = State.INITIAL;
        this.tokens = new ArrayList<>();
        this.currentToken = new StringBuilder();
        this.position = 0;
        this.line = 1;
        this.column = 1;
    }
    
    /**
     * 扫描输入字符串
     */
    public List<Token> scan(String input) {
        this.input = input;
        this.position = 0;
        this.line = 1;
        this.column = 1;
        this.tokens.clear();
        this.currentState = State.INITIAL;
        
        while (position < input.length()) {
            char ch = input.charAt(position);
            processChar(ch);
        }
        
        // 处理最后的标记
        finalizeCurrentToken();
        
        // 添加EOF标记
        tokens.add(new Token(TokenType.EOF, "", line, column));
        
        return tokens;
    }
    
    /**
     * 处理单个字符
     */
    private void processChar(char ch) {
        switch (currentState) {
            case INITIAL:
                processInitial(ch);
                break;
            case IN_TAG:
                processInTag(ch);
                break;
            case IN_TEXT:
                processInText(ch);
                break;
            case IN_STRING:
                processInString(ch);
                break;
            case IN_COMMENT:
                processInComment(ch);
                break;
            default:
                processDefault(ch);
                break;
        }
    }
    
    /**
     * 处理初始状态
     */
    private void processInitial(char ch) {
        if (ch == '<') {
            // 检查是否是注释
            if (lookAhead("<!--")) {
                currentState = State.IN_COMMENT;
                advance(4);
            } else {
                emit(TokenType.TAG_OPEN, "<");
                currentState = State.IN_TAG;
                advance();
            }
        } else if (ch == '[') {
            // 检查是否是模板或自定义标记
            if (lookAhead("[Template]")) {
                emit(TokenType.TEMPLATE_OPEN, "[Template]");
                advance(10);
            } else if (lookAhead("[Custom]")) {
                emit(TokenType.CUSTOM_OPEN, "[Custom]");
                advance(8);
            } else {
                emit(TokenType.LEFT_BRACKET, "[");
                advance();
            }
        } else if (Character.isWhitespace(ch)) {
            skipWhitespace();
        } else {
            currentState = State.IN_TEXT;
            currentToken.append(ch);
            advance();
        }
    }
    
    /**
     * 处理标签内部
     */
    private void processInTag(char ch) {
        if (ch == '>') {
            finalizeCurrentToken();
            emit(TokenType.TAG_CLOSE, ">");
            currentState = State.INITIAL;
            advance();
        } else if (ch == '/' && lookAhead("/>")) {
            finalizeCurrentToken();
            emit(TokenType.SELF_CLOSING, "/>");
            currentState = State.INITIAL;
            advance(2);
        } else if (ch == '"') {
            finalizeCurrentToken();
            currentState = State.IN_STRING;
            advance();
        } else if (ch == '=') {
            finalizeCurrentToken();
            emit(TokenType.EQUALS, "=");
            advance();
        } else if (Character.isWhitespace(ch)) {
            finalizeCurrentToken();
            skipWhitespace();
        } else {
            currentToken.append(ch);
            advance();
        }
    }
    
    /**
     * 处理文本内容
     */
    private void processInText(char ch) {
        if (ch == '<' || ch == '[') {
            finalizeCurrentToken();
            currentState = State.INITIAL;
            // 不前进，让下一次循环处理这个字符
        } else if (ch == '{' && isBlockStart()) {
            finalizeCurrentToken();
            processBlockStart();
        } else {
            currentToken.append(ch);
            advance();
        }
    }
    
    /**
     * 处理字符串内部
     */
    private void processInString(char ch) {
        if (ch == '"' && !isEscaped()) {
            String value = currentToken.toString();
            currentToken.setLength(0);
            emit(TokenType.STRING_LITERAL, value);
            currentState = State.IN_TAG;
            advance();
        } else if (ch == '\\') {
            currentToken.append(ch);
            advance();
            if (position < input.length()) {
                currentToken.append(input.charAt(position));
                advance();
            }
        } else {
            currentToken.append(ch);
            advance();
        }
    }
    
    /**
     * 处理注释
     */
    private void processInComment(char ch) {
        if (ch == '-' && lookAhead("-->")) {
            String comment = currentToken.toString();
            currentToken.setLength(0);
            emit(TokenType.COMMENT, comment);
            currentState = State.INITIAL;
            advance(3);
        } else {
            currentToken.append(ch);
            advance();
        }
    }
    
    /**
     * 处理默认情况
     */
    private void processDefault(char ch) {
        currentToken.append(ch);
        advance();
    }
    
    /**
     * 检查是否是块开始
     */
    private boolean isBlockStart() {
        String text = currentToken.toString().trim();
        return text.endsWith("text") || 
               text.endsWith("style") || 
               text.endsWith("script") || 
               text.endsWith("origin");
    }
    
    /**
     * 处理块开始
     */
    private void processBlockStart() {
        String text = currentToken.toString().trim();
        currentToken.setLength(0);
        
        if (text.endsWith("text")) {
            emit(TokenType.TEXT_BLOCK, "text{");
            currentState = State.IN_TEXT;
        } else if (text.endsWith("style")) {
            emit(TokenType.STYLE_BLOCK, "style{");
            currentState = State.IN_STYLE;
        } else if (text.endsWith("script")) {
            emit(TokenType.SCRIPT_BLOCK, "script{");
            currentState = State.IN_SCRIPT;
        } else if (text.endsWith("origin")) {
            emit(TokenType.ORIGIN_BLOCK, "origin{");
            currentState = State.IN_ORIGIN;
        }
        advance(); // Skip '{'
    }
    
    /**
     * 完成当前标记
     */
    private void finalizeCurrentToken() {
        if (currentToken.length() > 0) {
            String value = currentToken.toString();
            TokenType type = determineTokenType(value);
            emit(type, value);
            currentToken.setLength(0);
        }
    }
    
    /**
     * 确定标记类型
     */
    private TokenType determineTokenType(String value) {
        // 检查关键字
        switch (value) {
            case "except":
                return TokenType.KEYWORD_EXCEPT;
            case "import":
                return TokenType.KEYWORD_IMPORT;
            case "inherit":
                return TokenType.KEYWORD_INHERIT;
            case "delete":
                return TokenType.KEYWORD_DELETE;
            case "insert":
                return TokenType.KEYWORD_INSERT;
            default:
                // 检查是否是数字
                if (value.matches("\\d+(\\.\\d+)?")) {
                    return TokenType.NUMBER_LITERAL;
                }
                // 检查是否在标签内
                if (currentState == State.IN_TAG) {
                    return TokenType.ATTRIBUTE_NAME;
                }
                // 默认为文本
                return TokenType.TEXT;
        }
    }
    
    /**
     * 发射标记
     */
    private void emit(TokenType type, String value) {
        tokens.add(new Token(type, value, line, column));
    }
    
    /**
     * 前进位置
     */
    private void advance() {
        if (position < input.length()) {
            if (input.charAt(position) == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            position++;
        }
    }
    
    /**
     * 前进多个位置
     */
    private void advance(int count) {
        for (int i = 0; i < count; i++) {
            advance();
        }
    }
    
    /**
     * 跳过空白字符
     */
    private void skipWhitespace() {
        while (position < input.length() && Character.isWhitespace(input.charAt(position))) {
            advance();
        }
    }
    
    /**
     * 向前查看
     */
    private boolean lookAhead(String text) {
        if (position + text.length() > input.length()) {
            return false;
        }
        return input.substring(position, position + text.length()).equals(text);
    }
    
    /**
     * 检查是否转义
     */
    private boolean isEscaped() {
        if (position == 0) return false;
        int backslashCount = 0;
        int pos = position - 1;
        while (pos >= 0 && input.charAt(pos) == '\\') {
            backslashCount++;
            pos--;
        }
        return backslashCount % 2 == 1;
    }
    
    /**
     * 获取当前状态
     */
    public State getCurrentState() {
        return currentState;
    }
    
    /**
     * 获取所有标记
     */
    public List<Token> getTokens() {
        return new ArrayList<>(tokens);
    }
    
    /**
     * 重置状态机
     */
    public void reset() {
        currentState = State.INITIAL;
        tokens.clear();
        currentToken.setLength(0);
        position = 0;
        line = 1;
        column = 1;
    }
}
