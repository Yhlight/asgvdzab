package com.chtl.compiler.chtljs.lexer;

import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.Token;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


import com.chtl.compiler.chtljs.token.CHTLJSToken;
import com.chtl.compiler.chtljs.token.CHTLJSTokenType;

import java.util.*;

/**
 * CHTL JS状态机词法分析器
 * 负责识别CHTL JS特有的语法标记
 * 严格遵循CHTL语法文档
 */
public class CHTLJSStateMachineLexer {
    private final String input;
    private int position;
    private int line;
    private int column;
    private final List<CHTLJSToken> tokens;
    
    // 词法分析状态
    private enum LexState {
        START,
        IN_DOUBLE_BRACE,      // 在{{内部
        IN_ARROW,             // 识别->
        IN_IDENTIFIER,        // 标识符
        IN_NUMBER,            // 数字
        IN_STRING_SINGLE,     // 单引号字符串
        IN_STRING_DOUBLE,     // 双引号字符串
        IN_COMMENT_LINE,      // 行注释
        IN_COMMENT_BLOCK      // 块注释
    }
    
    // 关键字映射
    private static final Map<String, CHTLJSTokenType> KEYWORDS = new HashMap<>();
    static {
        KEYWORDS.put("listen", CHTLJSTokenType.LISTEN);
        KEYWORDS.put("delegate", CHTLJSTokenType.DELEGATE);
        KEYWORDS.put("animate", CHTLJSTokenType.ANIMATE);
        KEYWORDS.put("duration", CHTLJSTokenType.DURATION);
        KEYWORDS.put("easing", CHTLJSTokenType.EASING);
        KEYWORDS.put("begin", CHTLJSTokenType.BEGIN);
        KEYWORDS.put("end", CHTLJSTokenType.END);
        KEYWORDS.put("when", CHTLJSTokenType.WHEN);
        KEYWORDS.put("at", CHTLJSTokenType.AT);
        KEYWORDS.put("loop", CHTLJSTokenType.LOOP);
        KEYWORDS.put("direction", CHTLJSTokenType.DIRECTION);
        KEYWORDS.put("delay", CHTLJSTokenType.DELAY);
        KEYWORDS.put("callback", CHTLJSTokenType.CALLBACK);
        KEYWORDS.put("target", CHTLJSTokenType.TARGET);
        KEYWORDS.put("true", CHTLJSTokenType.TRUE);
        KEYWORDS.put("false", CHTLJSTokenType.FALSE);
        KEYWORDS.put("null", CHTLJSTokenType.NULL);
        KEYWORDS.put("function", CHTLJSTokenType.FUNCTION);
    }
    
    // 当前状态
    private LexState currentState;
    private StringBuilder currentTokenValue;
    
    public CHTLJSStateMachineLexer(String input) {
        this.input = input;
        this.position = 0;
        this.line = 1;
        this.column = 1;
        this.tokens = new ArrayList<>();
        this.currentState = LexState.START;
        this.currentTokenValue = new StringBuilder();
    }
    
    /**
     * 执行词法分析
     */
    public List<CHTLJSToken> tokenize() {
        while (position < input.length()) {
            char ch = current();
            
            switch (currentState) {
                case START:
                    handleStartState(ch);
                    break;
                case IN_DOUBLE_BRACE:
                    handleDoubleBraceState(ch);
                    break;
                case IN_ARROW:
                    handleArrowState(ch);
                    break;
                case IN_IDENTIFIER:
                    handleIdentifierState(ch);
                    break;
                case IN_NUMBER:
                    handleNumberState(ch);
                    break;
                case IN_STRING_SINGLE:
                    handleStringSingleState(ch);
                    break;
                case IN_STRING_DOUBLE:
                    handleStringDoubleState(ch);
                    break;
                case IN_COMMENT_LINE:
                    handleCommentLineState(ch);
                    break;
                case IN_COMMENT_BLOCK:
                    handleCommentBlockState(ch);
                    break;
            }
        }
        
        // 处理最后的token
        finalizeCurrentToken();
        
        // 添加EOF token
        addToken(CHTLJSTokenType.EOF, "");
        
        return tokens;
    }
    
    /**
     * 处理开始状态
     */
    private void handleStartState(char ch) {
        // 跳过空白字符
        if (Character.isWhitespace(ch)) {
            advance();
            return;
        }
        
        // 检查双字符token
        if (ch == '{' && peek() == '{') {
            addToken(CHTLJSTokenType.DOUBLE_LEFT_BRACE, "{{");
            advance();
            advance();
        } else if (ch == '}' && peek() == '}') {
            addToken(CHTLJSTokenType.DOUBLE_RIGHT_BRACE, "}}");
            advance();
            advance();
        } else if (ch == '-' && peek() == '>') {
            addToken(CHTLJSTokenType.ARROW, "->");
            advance();
            advance();
        } else if (ch == '/' && peek() == '/') {
            currentState = LexState.IN_COMMENT_LINE;
            advance();
            advance();
        } else if (ch == '/' && peek() == '*') {
            currentState = LexState.IN_COMMENT_BLOCK;
            advance();
            advance();
        }
        // 单字符token
        else if (ch == '.') {
            addToken(CHTLJSTokenType.DOT, ".");
            advance();
        } else if (ch == '#') {
            addToken(CHTLJSTokenType.HASH, "#");
            advance();
        } else if (ch == '{') {
            addToken(CHTLJSTokenType.LEFT_BRACE, "{");
            advance();
        } else if (ch == '}') {
            addToken(CHTLJSTokenType.RIGHT_BRACE, "}");
            advance();
        } else if (ch == '(') {
            addToken(CHTLJSTokenType.LEFT_PAREN, "(");
            advance();
        } else if (ch == ')') {
            addToken(CHTLJSTokenType.RIGHT_PAREN, ")");
            advance();
        } else if (ch == '[') {
            addToken(CHTLJSTokenType.LEFT_BRACKET, "[");
            advance();
        } else if (ch == ']') {
            addToken(CHTLJSTokenType.RIGHT_BRACKET, "]");
            advance();
        } else if (ch == ',') {
            addToken(CHTLJSTokenType.COMMA, ",");
            advance();
        } else if (ch == ':') {
            addToken(CHTLJSTokenType.COLON, ":");
            advance();
        } else if (ch == ';') {
            addToken(CHTLJSTokenType.SEMICOLON, ";");
            advance();
        } else if (ch == '=') {
            addToken(CHTLJSTokenType.EQUAL, "=");
            advance();
        }
        // 字符串
        else if (ch == '\'') {
            currentState = LexState.IN_STRING_SINGLE;
            advance();
        } else if (ch == '"') {
            currentState = LexState.IN_STRING_DOUBLE;
            advance();
        }
        // 标识符
        else if (Character.isLetter(ch) || ch == '_' || ch == '$') {
            currentState = LexState.IN_IDENTIFIER;
            currentTokenValue.append(ch);
            advance();
        }
        // 数字
        else if (Character.isDigit(ch)) {
            currentState = LexState.IN_NUMBER;
            currentTokenValue.append(ch);
            advance();
        }
        // 未知字符
        else {
            advance();
        }
    }
    
    /**
     * 处理标识符状态
     */
    private void handleIdentifierState(char ch) {
        if (Character.isLetterOrDigit(ch) || ch == '_' || ch == '$') {
            currentTokenValue.append(ch);
            advance();
        } else {
            String identifier = currentTokenValue.toString();
            CHTLJSTokenType type = KEYWORDS.getOrDefault(identifier, CHTLJSTokenType.IDENTIFIER);
            addToken(type, identifier);
            currentTokenValue.setLength(0);
            currentState = LexState.START;
        }
    }
    
    /**
     * 处理数字状态
     */
    private void handleNumberState(char ch) {
        if (Character.isDigit(ch)) {
            currentTokenValue.append(ch);
            advance();
        } else if (ch == '.' && Character.isDigit(peek())) {
            currentTokenValue.append(ch);
            advance();
        } else {
            addToken(CHTLJSTokenType.NUMBER_LITERAL, currentTokenValue.toString());
            currentTokenValue.setLength(0);
            currentState = LexState.START;
        }
    }
    
    /**
     * 处理单引号字符串状态
     */
    private void handleStringSingleState(char ch) {
        if (ch == '\'') {
            addToken(CHTLJSTokenType.STRING_LITERAL, currentTokenValue.toString());
            currentTokenValue.setLength(0);
            currentState = LexState.START;
            advance();
        } else if (ch == '\\' && peek() != '\0') {
            // 转义字符
            advance();
            currentTokenValue.append(current());
            advance();
        } else {
            currentTokenValue.append(ch);
            advance();
        }
    }
    
    /**
     * 处理双引号字符串状态
     */
    private void handleStringDoubleState(char ch) {
        if (ch == '"') {
            addToken(CHTLJSTokenType.STRING_LITERAL, currentTokenValue.toString());
            currentTokenValue.setLength(0);
            currentState = LexState.START;
            advance();
        } else if (ch == '\\' && peek() != '\0') {
            // 转义字符
            advance();
            currentTokenValue.append(current());
            advance();
        } else {
            currentTokenValue.append(ch);
            advance();
        }
    }
    
    /**
     * 处理行注释状态
     */
    private void handleCommentLineState(char ch) {
        if (ch == '\n') {
            currentState = LexState.START;
        }
        advance();
    }
    
    /**
     * 处理块注释状态
     */
    private void handleCommentBlockState(char ch) {
        if (ch == '*' && peek() == '/') {
            advance();
            advance();
            currentState = LexState.START;
        } else {
            advance();
        }
    }
    
    /**
     * 处理其他状态（占位）
     */
    private void handleDoubleBraceState(char ch) {
        // 在{{内部的特殊处理
        currentState = LexState.START;
    }
    
    private void handleArrowState(char ch) {
        // ->之后的处理
        currentState = LexState.START;
    }
    
    /**
     * 完成当前token
     */
    private void finalizeCurrentToken() {
        if (currentTokenValue.length() > 0) {
            switch (currentState) {
                case IN_IDENTIFIER:
                    String identifier = currentTokenValue.toString();
                    CHTLJSTokenType type = KEYWORDS.getOrDefault(identifier, CHTLJSTokenType.IDENTIFIER);
                    addToken(type, identifier);
                    break;
                case IN_NUMBER:
                    addToken(CHTLJSTokenType.NUMBER_LITERAL, currentTokenValue.toString());
                    break;
            }
            currentTokenValue.setLength(0);
        }
    }
    
    /**
     * 添加token
     */
    private void addToken(CHTLJSTokenType type, String value) {
        tokens.add(new CHTLJSToken(type, value, line, column - value.length(), position));
    }
    
    /**
     * 获取当前字符
     */
    private char current() {
        if (position >= input.length()) {
            return '\0';
        }
        return input.charAt(position);
    }
    
    /**
     * 查看下一个字符
     */
    private char peek() {
        if (position + 1 >= input.length()) {
            return '\0';
        }
        return input.charAt(position + 1);
    }
    
    /**
     * 前进一个字符
     */
    private void advance() {
        if (position < input.length()) {
            if (current() == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            position++;
        }
    }
}