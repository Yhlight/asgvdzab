package com.chtl.compiler.chtl.lexer;

import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.Token;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.chtl.compiler.chtljs.lexer.LexState;


import com.chtl.compiler.chtl.token.CHTLToken;
import com.chtl.compiler.chtl.token.CHTLTokenType;

import java.util.*;

/**
 * 基于状态机的CHTL词法分析器
 * 使用精确的状态转换来处理各种token
 * 严格遵循CHTL语法文档
 */
public class CHTLStateMachineLexer {
    
    // 词法分析状态
    private enum LexState {
        START,                  // 初始状态
        IN_IDENTIFIER,          // 标识符
        IN_NUMBER,              // 数字
        IN_STRING_SINGLE,       // 单引号字符串
        IN_STRING_DOUBLE,       // 双引号字符串
        IN_COMMENT_LINE,        // 行注释
        IN_COMMENT_BLOCK,       // 块注释
        IN_COMMENT_BLOCK_STAR,  // 块注释中遇到*
        IN_GENERATOR_COMMENT,   // 生成器注释
        IN_BRACKET_KEYWORD,     // []关键字
        IN_AT_KEYWORD,          // @关键字
        IN_WHITESPACE,          // 空白字符
        AFTER_SLASH,            // /之后（判断是否是注释）
        AFTER_MINUS,            // -之后（判断是否是生成器注释）
        AFTER_DOT,              // .之后（判断是否是数字）
        AFTER_COLON,            // :之后（判断是否是::）
        AFTER_EQUALS,           // =之后（判断是否是=>）
        AFTER_BRACKET_START,    // [之后（判断是否是关键字）
        END                     // 结束状态
    }
    
    private String source;
    private int position;
    private int line;
    private int column;
    private List<CHTLToken> tokens;
    private StringBuilder currentTokenValue;
    private int tokenStartPosition;
    private int tokenStartLine;
    private int tokenStartColumn;
    private LexState currentState;
    
    // 关键字映射
    private static final Map<String, CHTLTokenType> KEYWORDS = new HashMap<>();
    private static final Map<String, CHTLTokenType> BRACKET_KEYWORDS = new HashMap<>();
    private static final Map<String, CHTLTokenType> AT_KEYWORDS = new HashMap<>();
    
    static {
        // 初始化关键字
        KEYWORDS.put("style", CHTLTokenType.STYLE);
        KEYWORDS.put("script", CHTLTokenType.SCRIPT);
        KEYWORDS.put("text", CHTLTokenType.TEXT);
        KEYWORDS.put("except", CHTLTokenType.EXCEPT);
        KEYWORDS.put("delete", CHTLTokenType.DELETE);
        KEYWORDS.put("insert", CHTLTokenType.INSERT);
        KEYWORDS.put("after", CHTLTokenType.AFTER);
        KEYWORDS.put("before", CHTLTokenType.BEFORE);
        KEYWORDS.put("replace", CHTLTokenType.REPLACE);
        // at 不是单独的关键字，at top 和 at bottom 是特殊处理的
        KEYWORDS.put("top", CHTLTokenType.TOP);
        KEYWORDS.put("bottom", CHTLTokenType.BOTTOM);
        
        // []关键字
        BRACKET_KEYWORDS.put("[Configuration]", CHTLTokenType.CONFIGURATION);
        BRACKET_KEYWORDS.put("[Namespace]", CHTLTokenType.NAMESPACE);
        BRACKET_KEYWORDS.put("[Template]", CHTLTokenType.TEMPLATE);
        BRACKET_KEYWORDS.put("[Custom]", CHTLTokenType.CUSTOM);
        BRACKET_KEYWORDS.put("[Import]", CHTLTokenType.IMPORT);
        BRACKET_KEYWORDS.put("[Origin]", CHTLTokenType.ORIGIN);
        
        // @关键字
        AT_KEYWORDS.put("@Element", CHTLTokenType.AT_ELEMENT);
        AT_KEYWORDS.put("@Style", CHTLTokenType.AT_STYLE);
        AT_KEYWORDS.put("@Var", CHTLTokenType.AT_VAR);
        AT_KEYWORDS.put("@Html", CHTLTokenType.AT_HTML);
        AT_KEYWORDS.put("@JavaScript", CHTLTokenType.AT_JAVASCRIPT);
        AT_KEYWORDS.put("@Chtl", CHTLTokenType.AT_CHTL);
        AT_KEYWORDS.put("@CJmod", CHTLTokenType.AT_CJMOD);
    }
    
    public CHTLStateMachineLexer(String source) {
        this.source = source;
        this.position = 0;
        this.line = 1;
        this.column = 1;
        this.tokens = new ArrayList<>();
        this.currentTokenValue = new StringBuilder();
        this.currentState = LexState.START;
    }
    
    /**
     * 执行词法分析
     */
    public List<CHTLToken> tokenize() {
        while (position < source.length()) {
            processNextCharacter();
        }
        
        // 处理最后的token
        finalizeCurrentToken();
        
        // 添加EOF token
        tokens.add(new CHTLToken(CHTLTokenType.EOF, "", line, column, position));
        
        return tokens;
    }
    
    /**
     * 处理下一个字符
     */
    private void processNextCharacter() {
        char ch = currentChar();
        
        switch (currentState) {
            case START:
                handleStartState(ch);
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
                
            case IN_COMMENT_BLOCK_STAR:
                handleCommentBlockStarState(ch);
                break;
                
            case IN_GENERATOR_COMMENT:
                handleGeneratorCommentState(ch);
                break;
                
            case IN_BRACKET_KEYWORD:
                handleBracketKeywordState(ch);
                break;
                
            case IN_AT_KEYWORD:
                handleAtKeywordState(ch);
                break;
                
            case IN_WHITESPACE:
                handleWhitespaceState(ch);
                break;
                
            case AFTER_SLASH:
                handleAfterSlashState(ch);
                break;
                
            case AFTER_MINUS:
                handleAfterMinusState(ch);
                break;
                
            case AFTER_BRACKET_START:
                handleAfterBracketStartState(ch);
                break;
                
            default:
                advance(); // 跳过未知状态
        }
    }
    
    /**
     * 处理初始状态
     */
    private void handleStartState(char ch) {
        // 记录token开始位置
        tokenStartPosition = position;
        tokenStartLine = line;
        tokenStartColumn = column;
        currentTokenValue.setLength(0);
        
        if (Character.isWhitespace(ch)) {
            currentState = LexState.IN_WHITESPACE;
            advance();
        } else if (Character.isLetter(ch) || ch == '_') {
            currentState = LexState.IN_IDENTIFIER;
            currentTokenValue.append(ch);
            advance();
        } else if (Character.isDigit(ch)) {
            currentState = LexState.IN_NUMBER;
            currentTokenValue.append(ch);
            advance();
        } else if (ch == '"') {
            currentState = LexState.IN_STRING_DOUBLE;
            currentTokenValue.append(ch);
            advance();
        } else if (ch == '\'') {
            currentState = LexState.IN_STRING_SINGLE;
            currentTokenValue.append(ch);
            advance();
        } else if (ch == '/') {
            currentState = LexState.AFTER_SLASH;
            advance();
        } else if (ch == '-') {
            currentState = LexState.AFTER_MINUS;
            advance();
        } else if (ch == '[') {
            currentState = LexState.AFTER_BRACKET_START;
            advance();
        } else if (ch == '@') {
            currentState = LexState.IN_AT_KEYWORD;
            currentTokenValue.append(ch);
            advance();
        } else {
            // 单字符token
            emitSingleCharToken(ch);
            advance();
        }
    }
    
    /**
     * 处理标识符状态
     */
    private void handleIdentifierState(char ch) {
        if (Character.isLetterOrDigit(ch) || ch == '_') {
            currentTokenValue.append(ch);
            advance();
        } else {
            // 标识符结束
            String identifier = currentTokenValue.toString();
            CHTLTokenType type = KEYWORDS.getOrDefault(identifier, CHTLTokenType.IDENTIFIER);
            emitToken(type, identifier);
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
        } else if (ch == '.') {
            currentTokenValue.append(ch);
            advance();
        } else {
            // 数字结束
            emitToken(CHTLTokenType.NUMBER, currentTokenValue.toString());
            currentState = LexState.START;
        }
    }
    
    /**
     * 处理[之后的状态
     */
    private void handleAfterBracketStartState(char ch) {
        if (Character.isLetter(ch)) {
            // 可能是[]关键字，如[Template]
            currentState = LexState.IN_BRACKET_KEYWORD;
            currentTokenValue.append('[');
            currentTokenValue.append(ch);
            advance();
        } else {
            // 只是普通的[
            emitToken(CHTLTokenType.LEFT_BRACKET, "[");
            currentState = LexState.START;
        }
    }
    
    /**
     * 处理[]关键字状态
     */
    private void handleBracketKeywordState(char ch) {
        if (ch == ']') {
            currentTokenValue.append(ch);
            String keyword = currentTokenValue.toString();
            CHTLTokenType type = BRACKET_KEYWORDS.get(keyword);
            if (type != null) {
                emitToken(type, keyword);
                advance();
            } else {
                // 不是已知的[]关键字，只发射[
                emitToken(CHTLTokenType.LEFT_BRACKET, "[");
                // 重置状态，让剩余内容重新开始解析
                currentTokenValue.setLength(0);
                currentTokenValue.append(keyword.substring(1, keyword.length() - 1));
                currentState = LexState.IN_IDENTIFIER;
                // 当前字符是]，需要处理
                handleIdentifierState(ch);
                return;
            }
            currentState = LexState.START;
        } else if (Character.isLetterOrDigit(ch) || ch == '_') {
            currentTokenValue.append(ch);
            advance();
        } else {
            // 不是有效的[]关键字格式
            emitToken(CHTLTokenType.LEFT_BRACKET, "[");
            currentState = LexState.START;
        }
    }
    
    /**
     * 处理@关键字状态
     */
    private void handleAtKeywordState(char ch) {
        if (Character.isLetter(ch)) {
            currentTokenValue.append(ch);
            advance();
        } else {
            String keyword = currentTokenValue.toString();
            CHTLTokenType type = AT_KEYWORDS.get(keyword);
            if (type != null) {
                emitToken(type, keyword);
            } else {
                // 不是已知的@关键字，作为标识符处理
                emitToken(CHTLTokenType.IDENTIFIER, keyword);
            }
            currentState = LexState.START;
        }
    }
    
    /**
     * 处理双引号字符串状态
     */
    private void handleStringDoubleState(char ch) {
        currentTokenValue.append(ch);
        if (ch == '"' && previousChar() != '\\') {
            String content = currentTokenValue.toString();
            // 去掉引号
            content = content.substring(1, content.length() - 1);
            emitToken(CHTLTokenType.STRING_LITERAL, content);
            advance();
            currentState = LexState.START;
        } else if (ch == '\n') {
            // 字符串不能跨行
            emitToken(CHTLTokenType.STRING_LITERAL, currentTokenValue.toString());
            currentState = LexState.START;
        } else {
            advance();
        }
    }
    
    /**
     * 处理单引号字符串状态
     */
    private void handleStringSingleState(char ch) {
        currentTokenValue.append(ch);
        if (ch == '\'' && previousChar() != '\\') {
            String content = currentTokenValue.toString();
            // 去掉引号
            content = content.substring(1, content.length() - 1);
            emitToken(CHTLTokenType.STRING_LITERAL, content);
            advance();
            currentState = LexState.START;
        } else if (ch == '\n') {
            // 字符串不能跨行
            emitToken(CHTLTokenType.STRING_LITERAL, currentTokenValue.toString());
            currentState = LexState.START;
        } else {
            advance();
        }
    }
    
    /**
     * 处理/之后的状态
     */
    private void handleAfterSlashState(char ch) {
        if (ch == '/') {
            currentState = LexState.IN_COMMENT_LINE;
            currentTokenValue.append("//");
            advance();
        } else if (ch == '*') {
            currentState = LexState.IN_COMMENT_BLOCK;
            currentTokenValue.append("/*");
            advance();
        } else {
            // 只是一个单独的/
            emitToken(CHTLTokenType.SLASH, "/");
            currentState = LexState.START;
        }
    }
    
    /**
     * 处理行注释状态
     */
    private void handleCommentLineState(char ch) {
        if (ch == '\n') {
            emitToken(CHTLTokenType.LINE_COMMENT, currentTokenValue.toString());
            advance();
            currentState = LexState.START;
        } else {
            currentTokenValue.append(ch);
            advance();
        }
    }
    
    /**
     * 处理块注释状态
     */
    private void handleCommentBlockState(char ch) {
        currentTokenValue.append(ch);
        if (ch == '*') {
            currentState = LexState.IN_COMMENT_BLOCK_STAR;
        }
        advance();
    }
    
    /**
     * 处理块注释中*之后的状态
     */
    private void handleCommentBlockStarState(char ch) {
        currentTokenValue.append(ch);
        if (ch == '/') {
            emitToken(CHTLTokenType.BLOCK_COMMENT, currentTokenValue.toString());
            advance();
            currentState = LexState.START;
        } else if (ch == '*') {
            // 保持在*状态
            advance();
        } else {
            currentState = LexState.IN_COMMENT_BLOCK;
            advance();
        }
    }
    
    /**
     * 处理-之后的状态
     */
    private void handleAfterMinusState(char ch) {
        if (ch == '-') {
            currentState = LexState.IN_GENERATOR_COMMENT;
            currentTokenValue.append("--");
            advance();
        } else {
            // 只是一个单独的-
            emitToken(CHTLTokenType.MINUS, "-");
            currentState = LexState.START;
        }
    }
    
    /**
     * 处理生成器注释状态
     */
    private void handleGeneratorCommentState(char ch) {
        if (ch == '\n') {
            emitToken(CHTLTokenType.GENERATOR_COMMENT, currentTokenValue.toString());
            advance();
            currentState = LexState.START;
        } else {
            currentTokenValue.append(ch);
            advance();
        }
    }
    
    /**
     * 处理空白字符状态
     */
    private void handleWhitespaceState(char ch) {
        if (Character.isWhitespace(ch)) {
            advance();
        } else {
            currentState = LexState.START;
        }
    }
    
    /**
     * 发射单字符token
     */
    private void emitSingleCharToken(char ch) {
        switch (ch) {
            case '{':
                emitToken(CHTLTokenType.LEFT_BRACE, "{");
                break;
            case '}':
                emitToken(CHTLTokenType.RIGHT_BRACE, "}");
                break;
            case '(':
                emitToken(CHTLTokenType.LEFT_PAREN, "(");
                break;
            case ')':
                emitToken(CHTLTokenType.RIGHT_PAREN, ")");
                break;
            case ']':
                emitToken(CHTLTokenType.RIGHT_BRACKET, "]");
                break;
            case ':':
                emitToken(CHTLTokenType.COLON, ":");
                break;
            case ';':
                emitToken(CHTLTokenType.SEMICOLON, ";");
                break;
            case ',':
                emitToken(CHTLTokenType.COMMA, ",");
                break;
            case '.':
                emitToken(CHTLTokenType.DOT, ".");
                break;
            case '=':
                emitToken(CHTLTokenType.EQUALS, "=");
                break;
            case '#':
                emitToken(CHTLTokenType.HASH, "#");
                break;
            case '&':
                emitToken(CHTLTokenType.AMPERSAND, "&");
                break;
            case '>':
                emitToken(CHTLTokenType.GREATER_THAN, ">");
                break;
            case '<':
                emitToken(CHTLTokenType.LESS_THAN, "<");
                break;
            case '!':
                emitToken(CHTLTokenType.EXCLAMATION, "!");
                break;
            default:
                // 未知字符，跳过
                break;
        }
    }
    
    /**
     * 发射token
     */
    private void emitToken(CHTLTokenType type, String value) {
        tokens.add(new CHTLToken(type, value, tokenStartLine, tokenStartColumn, tokenStartPosition));
    }
    
    /**
     * 最终化当前token
     */
    private void finalizeCurrentToken() {
        if (currentTokenValue.length() > 0) {
            switch (currentState) {
                case IN_IDENTIFIER:
                    String identifier = currentTokenValue.toString();
                    CHTLTokenType type = KEYWORDS.getOrDefault(identifier, CHTLTokenType.IDENTIFIER);
                    emitToken(type, identifier);
                    break;
                case IN_NUMBER:
                    emitToken(CHTLTokenType.NUMBER, currentTokenValue.toString());
                    break;
                case IN_STRING_SINGLE:
                case IN_STRING_DOUBLE:
                    emitToken(CHTLTokenType.STRING_LITERAL, currentTokenValue.toString());
                    break;
                case IN_COMMENT_LINE:
                case IN_COMMENT_BLOCK:
                case IN_GENERATOR_COMMENT:
                    emitToken(CHTLTokenType.LINE_COMMENT, currentTokenValue.toString());
                    break;
            }
        }
    }
    
    // 辅助方法
    
    private char currentChar() {
        return position < source.length() ? source.charAt(position) : '\0';
    }
    
    private char previousChar() {
        return position > 0 ? source.charAt(position - 1) : '\0';
    }
    
    private char peekChar() {
        return position + 1 < source.length() ? source.charAt(position + 1) : '\0';
    }
    
    private void advance() {
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
}