package com.chtl.compiler.chtljs.lexer;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.chtl.compiler.chtl.CHTLParser.Token;


import com.chtl.compiler.chtljs.token.CHTLJSToken;
import com.chtl.compiler.chtljs.token.CHTLJSTokenType;

import java.util.*;

/**
 * CHTL JS词法分析器
 * 严格根据CHTL语法文档的CHTL JS部分实现
 */
public class CHTLJSLexer {
    
    private final String input;
    private int position;
    private int line;
    private int column;
    
    // CHTL JS特殊方法
    private static final Set<String> CHTL_JS_METHODS = new HashSet<>();
    // 动画关键字
    private static final Set<String> ANIMATION_KEYWORDS = new HashSet<>();
    // JavaScript关键字
    private static final Set<String> JS_KEYWORDS = new HashSet<>();
    
    static {
        // CHTL JS方法
        CHTL_JS_METHODS.add("listen");
        CHTL_JS_METHODS.add("delegate");
        CHTL_JS_METHODS.add("animate");
        
        // 动画相关关键字
        ANIMATION_KEYWORDS.add("duration");
        ANIMATION_KEYWORDS.add("easing");
        ANIMATION_KEYWORDS.add("begin");
        ANIMATION_KEYWORDS.add("end");
        ANIMATION_KEYWORDS.add("when");
        ANIMATION_KEYWORDS.add("at");
        ANIMATION_KEYWORDS.add("loop");
        ANIMATION_KEYWORDS.add("direction");
        ANIMATION_KEYWORDS.add("delay");
        ANIMATION_KEYWORDS.add("callback");
        ANIMATION_KEYWORDS.add("target");
        
        // JavaScript基础关键字
        JS_KEYWORDS.add("function");
        JS_KEYWORDS.add("const");
        JS_KEYWORDS.add("let");
        JS_KEYWORDS.add("var");
        JS_KEYWORDS.add("return");
        JS_KEYWORDS.add("if");
        JS_KEYWORDS.add("else");
        JS_KEYWORDS.add("for");
        JS_KEYWORDS.add("while");
    }
    
    public CHTLJSLexer(String input) {
        this.input = input;
        this.position = 0;
        this.line = 1;
        this.column = 1;
    }
    
    /**
     * 获取所有Token
     */
    public List<CHTLJSToken> tokenize() {
        List<CHTLJSToken> tokens = new ArrayList<>();
        
        while (!isAtEnd()) {
            // 跳过空白字符
            skipWhitespace();
            
            if (isAtEnd()) {
                break;
            }
            
            CHTLJSToken token = nextToken();
            if (token != null) {
                tokens.add(token);
            }
        }
        
        tokens.add(new CHTLJSToken(CHTLJSTokenType.EOF, "", line, column, position));
        return tokens;
    }
    
    /**
     * 获取下一个Token
     */
    private CHTLJSToken nextToken() {
        int startPosition = position;
        int startLine = line;
        int startColumn = column;
        
        char ch = current();
        
        // 检查CHTL JS特有的双字符符号
        if (ch == '{' && peek() == '{') {
            advance();
            advance();
            return new CHTLJSToken(CHTLJSTokenType.DOUBLE_LEFT_BRACE, "{{", startLine, startColumn, startPosition);
        }
        
        if (ch == '}' && peek() == '}') {
            advance();
            advance();
            return new CHTLJSToken(CHTLJSTokenType.DOUBLE_RIGHT_BRACE, "}}", startLine, startColumn, startPosition);
        }
        
        if (ch == '-' && peek() == '>') {
            advance();
            advance();
            return new CHTLJSToken(CHTLJSTokenType.ARROW, "->", startLine, startColumn, startPosition);
        }
        
        // 单字符Token
        switch (ch) {
            case '{':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.LEFT_BRACE, "{", startLine, startColumn, startPosition);
            case '}':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.RIGHT_BRACE, "}", startLine, startColumn, startPosition);
            case '(':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.LEFT_PAREN, "(", startLine, startColumn, startPosition);
            case ')':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.RIGHT_PAREN, ")", startLine, startColumn, startPosition);
            case '[':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.LEFT_BRACKET, "[", startLine, startColumn, startPosition);
            case ']':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.RIGHT_BRACKET, "]", startLine, startColumn, startPosition);
            case ',':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.COMMA, ",", startLine, startColumn, startPosition);
            case ':':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.COLON, ":", startLine, startColumn, startPosition);
            case ';':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.SEMICOLON, ";", startLine, startColumn, startPosition);
            case '.':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.DOT, ".", startLine, startColumn, startPosition);
            case '#':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.HASH, "#", startLine, startColumn, startPosition);
            case '=':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.EQUALS, "=", startLine, startColumn, startPosition);
            case '+':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.PLUS, "+", startLine, startColumn, startPosition);
            case '-':
                advance();
                // 检查是否是->操作符
                if (current() == '>') {
                    advance();
                    return new CHTLJSToken(CHTLJSTokenType.ARROW, "->", startLine, startColumn, startPosition);
                }
                return new CHTLJSToken(CHTLJSTokenType.MINUS, "-", startLine, startColumn, startPosition);
            case '*':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.MULTIPLY, "*", startLine, startColumn, startPosition);
            case '/':
                // 检查是否是注释
                if (peek() == '/') {
                    return scanLineComment(startLine, startColumn, startPosition);
                } else if (peek() == '*') {
                    return scanBlockComment(startLine, startColumn, startPosition);
                } else {
                    advance();
                    return new CHTLJSToken(CHTLJSTokenType.DIVIDE, "/", startLine, startColumn, startPosition);
                }
            case '&':
                advance();
                return new CHTLJSToken(CHTLJSTokenType.AMPERSAND, "&", startLine, startColumn, startPosition);
        }
        
        // 检查字符串
        if (ch == '"' || ch == '\'') {
            return scanString(ch, startLine, startColumn, startPosition);
        }
        
        // 检查数字
        if (Character.isDigit(ch)) {
            return scanNumber(startLine, startColumn, startPosition);
        }
        
        // 检查标识符或关键字
        if (isIdentifierStart(ch)) {
            return scanIdentifier(startLine, startColumn, startPosition);
        }
        
        // 未识别的字符，跳过
        advance();
        return null;
    }
    
    /**
     * 扫描行注释
     */
    private CHTLJSToken scanLineComment(int startLine, int startColumn, int startPosition) {
        advance(); // 跳过第一个/
        advance(); // 跳过第二个/
        
        StringBuilder comment = new StringBuilder("//");
        
        while (!isAtEnd() && current() != '\n') {
            comment.append(current());
            advance();
        }
        
        return new CHTLJSToken(CHTLJSTokenType.LINE_COMMENT, comment.toString(), startLine, startColumn, startPosition);
    }
    
    /**
     * 扫描块注释
     */
    private CHTLJSToken scanBlockComment(int startLine, int startColumn, int startPosition) {
        advance(); // 跳过/
        advance(); // 跳过*
        
        StringBuilder comment = new StringBuilder("/*");
        
        while (!isAtEnd()) {
            if (current() == '*' && peek() == '/') {
                comment.append("*/");
                advance();
                advance();
                break;
            }
            comment.append(current());
            advance();
        }
        
        return new CHTLJSToken(CHTLJSTokenType.BLOCK_COMMENT, comment.toString(), startLine, startColumn, startPosition);
    }
    
    /**
     * 扫描字符串
     */
    private CHTLJSToken scanString(char quote, int startLine, int startColumn, int startPosition) {
        advance(); // 跳过引号
        StringBuilder value = new StringBuilder();
        
        while (!isAtEnd() && current() != quote) {
            if (current() == '\\') {
                advance();
                if (!isAtEnd()) {
                    // 简单的转义处理
                    char escaped = current();
                    switch (escaped) {
                        case 'n': value.append('\n'); break;
                        case 't': value.append('\t'); break;
                        case 'r': value.append('\r'); break;
                        case '\\': value.append('\\'); break;
                        case '"': value.append('"'); break;
                        case '\'': value.append('\''); break;
                        default: value.append(escaped);
                    }
                    advance();
                }
            } else {
                value.append(current());
                advance();
            }
        }
        
        if (current() == quote) {
            advance(); // 跳过结束引号
        }
        
        return new CHTLJSToken(CHTLJSTokenType.STRING_LITERAL, value.toString(), startLine, startColumn, startPosition);
    }
    
    /**
     * 扫描数字
     */
    private CHTLJSToken scanNumber(int startLine, int startColumn, int startPosition) {
        StringBuilder value = new StringBuilder();
        
        while (!isAtEnd() && Character.isDigit(current())) {
            value.append(current());
            advance();
        }
        
        // 检查小数点
        if (current() == '.' && Character.isDigit(peek())) {
            value.append('.');
            advance();
            
            while (!isAtEnd() && Character.isDigit(current())) {
                value.append(current());
                advance();
            }
        }
        
        return new CHTLJSToken(CHTLJSTokenType.NUMBER_LITERAL, value.toString(), startLine, startColumn, startPosition);
    }
    
    /**
     * 扫描标识符
     */
    private CHTLJSToken scanIdentifier(int startLine, int startColumn, int startPosition) {
        StringBuilder value = new StringBuilder();
        
        // 首先按普通标识符规则扫描
        while (!isAtEnd() && isIdentifierPart(current())) {
            value.append(current());
            advance();
        }
        
        // 如果后面跟着连字符，可能是CSS值或缓动函数
        // 检查是否是特殊的CSS相关上下文
        if (current() == '-' && !isAtEnd() && Character.isLetter(peek())) {
            // 继续扫描包含连字符的标识符
            while (!isAtEnd() && isCssIdentifierPart(current())) {
                value.append(current());
                advance();
            }
        }
        
        String identifier = value.toString();
        
        // 检查是否是CHTL JS方法
        if (CHTL_JS_METHODS.contains(identifier)) {
            switch (identifier) {
                case "listen":
                    return new CHTLJSToken(CHTLJSTokenType.LISTEN, identifier, startLine, startColumn, startPosition);
                case "delegate":
                    return new CHTLJSToken(CHTLJSTokenType.DELEGATE, identifier, startLine, startColumn, startPosition);
                case "animate":
                    return new CHTLJSToken(CHTLJSTokenType.ANIMATE, identifier, startLine, startColumn, startPosition);
            }
        }
        
        // 检查是否是动画关键字
        if (ANIMATION_KEYWORDS.contains(identifier)) {
            switch (identifier) {
                case "duration":
                    return new CHTLJSToken(CHTLJSTokenType.DURATION, identifier, startLine, startColumn, startPosition);
                case "easing":
                    return new CHTLJSToken(CHTLJSTokenType.EASING, identifier, startLine, startColumn, startPosition);
                case "begin":
                    return new CHTLJSToken(CHTLJSTokenType.BEGIN, identifier, startLine, startColumn, startPosition);
                case "end":
                    return new CHTLJSToken(CHTLJSTokenType.END, identifier, startLine, startColumn, startPosition);
                case "when":
                    return new CHTLJSToken(CHTLJSTokenType.WHEN, identifier, startLine, startColumn, startPosition);
                case "at":
                    return new CHTLJSToken(CHTLJSTokenType.AT, identifier, startLine, startColumn, startPosition);
                case "loop":
                    return new CHTLJSToken(CHTLJSTokenType.LOOP, identifier, startLine, startColumn, startPosition);
                case "direction":
                    return new CHTLJSToken(CHTLJSTokenType.DIRECTION, identifier, startLine, startColumn, startPosition);
                case "delay":
                    return new CHTLJSToken(CHTLJSTokenType.DELAY, identifier, startLine, startColumn, startPosition);
                case "callback":
                    return new CHTLJSToken(CHTLJSTokenType.CALLBACK, identifier, startLine, startColumn, startPosition);
                case "target":
                    return new CHTLJSToken(CHTLJSTokenType.TARGET, identifier, startLine, startColumn, startPosition);
            }
        }
        
        // 检查是否是JavaScript关键字
        if (JS_KEYWORDS.contains(identifier)) {
            switch (identifier) {
                case "function":
                    return new CHTLJSToken(CHTLJSTokenType.FUNCTION, identifier, startLine, startColumn, startPosition);
                case "const":
                    return new CHTLJSToken(CHTLJSTokenType.CONST, identifier, startLine, startColumn, startPosition);
                case "let":
                    return new CHTLJSToken(CHTLJSTokenType.LET, identifier, startLine, startColumn, startPosition);
                case "var":
                    return new CHTLJSToken(CHTLJSTokenType.VAR, identifier, startLine, startColumn, startPosition);
                case "return":
                    return new CHTLJSToken(CHTLJSTokenType.RETURN, identifier, startLine, startColumn, startPosition);
                case "if":
                    return new CHTLJSToken(CHTLJSTokenType.IF, identifier, startLine, startColumn, startPosition);
                case "else":
                    return new CHTLJSToken(CHTLJSTokenType.ELSE, identifier, startLine, startColumn, startPosition);
                case "for":
                    return new CHTLJSToken(CHTLJSTokenType.FOR, identifier, startLine, startColumn, startPosition);
                case "while":
                    return new CHTLJSToken(CHTLJSTokenType.WHILE, identifier, startLine, startColumn, startPosition);
            }
        }
        
        // 默认作为标识符
        return new CHTLJSToken(CHTLJSTokenType.IDENTIFIER, identifier, startLine, startColumn, startPosition);
    }
    
    /**
     * 跳过空白字符
     */
    private void skipWhitespace() {
        while (!isAtEnd() && Character.isWhitespace(current())) {
            advance();
        }
    }
    
    /**
     * 辅助方法
     */
    private boolean isAtEnd() {
        return position >= input.length();
    }
    
    private char current() {
        return isAtEnd() ? '\0' : input.charAt(position);
    }
    
    private char peek() {
        return position + 1 >= input.length() ? '\0' : input.charAt(position + 1);
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
    
    private boolean isIdentifierStart(char ch) {
        return Character.isLetter(ch) || ch == '_' || ch == '$';
    }
    
    private boolean isIdentifierPart(char ch) {
        return Character.isLetterOrDigit(ch) || ch == '_' || ch == '$';
    }
    
    /**
     * 检查是否是CSS值标识符的一部分（包括连字符）
     */
    private boolean isCssIdentifierPart(char ch) {
        return isIdentifierPart(ch) || ch == '-';
    }
}