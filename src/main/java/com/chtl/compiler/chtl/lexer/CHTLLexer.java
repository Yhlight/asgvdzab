package com.chtl.compiler.chtl.lexer;

import com.chtl.compiler.chtl.token.CHTLToken;
import com.chtl.compiler.chtl.token.CHTLTokenType;

import java.util.*;

/**
 * CHTL词法分析器
 * 严格根据CHTL语法文档实现
 */
public class CHTLLexer {
    
    private final String input;
    private int position;
    private int line;
    private int column;
    
    // 关键字映射表
    private static final Map<String, CHTLTokenType> KEYWORDS = new HashMap<>();
    private static final Map<String, CHTLTokenType> AT_KEYWORDS = new HashMap<>();
    private static final Map<String, CHTLTokenType> BRACKET_KEYWORDS = new HashMap<>();
    
    static {
        // 基础关键字
        KEYWORDS.put("text", CHTLTokenType.TEXT);
        KEYWORDS.put("style", CHTLTokenType.STYLE);
        KEYWORDS.put("script", CHTLTokenType.SCRIPT);
        KEYWORDS.put("inherit", CHTLTokenType.INHERIT);
        KEYWORDS.put("delete", CHTLTokenType.DELETE);
        KEYWORDS.put("insert", CHTLTokenType.INSERT);
        KEYWORDS.put("after", CHTLTokenType.AFTER);
        KEYWORDS.put("before", CHTLTokenType.BEFORE);
        KEYWORDS.put("replace", CHTLTokenType.REPLACE);
        KEYWORDS.put("from", CHTLTokenType.FROM);
        KEYWORDS.put("as", CHTLTokenType.AS);
        KEYWORDS.put("except", CHTLTokenType.EXCEPT);
        
        // @关键字
        AT_KEYWORDS.put("@Style", CHTLTokenType.AT_STYLE);
        AT_KEYWORDS.put("@Element", CHTLTokenType.AT_ELEMENT);
        AT_KEYWORDS.put("@Var", CHTLTokenType.AT_VAR);
        AT_KEYWORDS.put("@Html", CHTLTokenType.AT_HTML);
        AT_KEYWORDS.put("@JavaScript", CHTLTokenType.AT_JAVASCRIPT);
        AT_KEYWORDS.put("@Chtl", CHTLTokenType.AT_CHTL);
        AT_KEYWORDS.put("@CJmod", CHTLTokenType.AT_CJMOD);
        
        // []关键字
        BRACKET_KEYWORDS.put("[Template]", CHTLTokenType.TEMPLATE);
        BRACKET_KEYWORDS.put("[Custom]", CHTLTokenType.CUSTOM);
        BRACKET_KEYWORDS.put("[Origin]", CHTLTokenType.ORIGIN);
        BRACKET_KEYWORDS.put("[Import]", CHTLTokenType.IMPORT);
        BRACKET_KEYWORDS.put("[Namespace]", CHTLTokenType.NAMESPACE);
        BRACKET_KEYWORDS.put("[Configuration]", CHTLTokenType.CONFIGURATION);
    }
    
    public CHTLLexer(String input) {
        this.input = input;
        this.position = 0;
        this.line = 1;
        this.column = 1;
    }
    
    /**
     * 获取所有Token
     */
    public List<CHTLToken> tokenize() {
        List<CHTLToken> tokens = new ArrayList<>();
        
        while (!isAtEnd()) {
            // 跳过空白字符
            skipWhitespace();
            
            if (isAtEnd()) {
                break;
            }
            
            CHTLToken token = nextToken();
            if (token != null) {
                tokens.add(token);
            }
        }
        
        tokens.add(new CHTLToken(CHTLTokenType.EOF, "", line, column, position));
        return tokens;
    }
    
    /**
     * 获取下一个Token
     */
    private CHTLToken nextToken() {
        int startPosition = position;
        int startLine = line;
        int startColumn = column;
        
        char ch = current();
        
        // 单字符Token
        switch (ch) {
            case '{':
                advance();
                return new CHTLToken(CHTLTokenType.LEFT_BRACE, "{", startLine, startColumn, startPosition);
            case '}':
                advance();
                return new CHTLToken(CHTLTokenType.RIGHT_BRACE, "}", startLine, startColumn, startPosition);
            case ':':
                advance();
                return new CHTLToken(CHTLTokenType.COLON, ":", startLine, startColumn, startPosition);
            case '=':
                advance();
                return new CHTLToken(CHTLTokenType.EQUALS, "=", startLine, startColumn, startPosition);
            case ';':
                advance();
                return new CHTLToken(CHTLTokenType.SEMICOLON, ";", startLine, startColumn, startPosition);
            case ',':
                advance();
                return new CHTLToken(CHTLTokenType.COMMA, ",", startLine, startColumn, startPosition);
            case '.':
                advance();
                return new CHTLToken(CHTLTokenType.DOT, ".", startLine, startColumn, startPosition);
            case '#':
                advance();
                return new CHTLToken(CHTLTokenType.HASH, "#", startLine, startColumn, startPosition);
            case '&':
                advance();
                return new CHTLToken(CHTLTokenType.AMPERSAND, "&", startLine, startColumn, startPosition);
            case '(':
                advance();
                return new CHTLToken(CHTLTokenType.LEFT_PAREN, "(", startLine, startColumn, startPosition);
            case ')':
                advance();
                return new CHTLToken(CHTLTokenType.RIGHT_PAREN, ")", startLine, startColumn, startPosition);
            case ']':
                advance();
                return new CHTLToken(CHTLTokenType.RIGHT_BRACKET, "]", startLine, startColumn, startPosition);
        }
        
        // 检查注释
        if (ch == '/') {
            if (peek() == '/') {
                return scanLineComment(startLine, startColumn, startPosition);
            } else if (peek() == '*') {
                return scanBlockComment(startLine, startColumn, startPosition);
            }
        }
        
        // 检查生成器注释
        if (ch == '-' && peek() == '-') {
            return scanGeneratorComment(startLine, startColumn, startPosition);
        }
        
        // 检查[]关键字
        if (ch == '[') {
            return scanBracketKeyword(startLine, startColumn, startPosition);
        }
        
        // 检查@关键字
        if (ch == '@') {
            return scanAtKeyword(startLine, startColumn, startPosition);
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
        
        // 未识别的字符
        advance();
        return null;
    }
    
    /**
     * 扫描行注释
     */
    private CHTLToken scanLineComment(int startLine, int startColumn, int startPosition) {
        advance(); // 跳过第一个/
        advance(); // 跳过第二个/
        
        StringBuilder comment = new StringBuilder("//");
        
        while (!isAtEnd() && current() != '\n') {
            comment.append(current());
            advance();
        }
        
        return new CHTLToken(CHTLTokenType.LINE_COMMENT, comment.toString(), startLine, startColumn, startPosition);
    }
    
    /**
     * 扫描块注释
     */
    private CHTLToken scanBlockComment(int startLine, int startColumn, int startPosition) {
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
        
        return new CHTLToken(CHTLTokenType.BLOCK_COMMENT, comment.toString(), startLine, startColumn, startPosition);
    }
    
    /**
     * 扫描生成器注释
     */
    private CHTLToken scanGeneratorComment(int startLine, int startColumn, int startPosition) {
        advance(); // 跳过第一个-
        advance(); // 跳过第二个-
        
        StringBuilder comment = new StringBuilder("--");
        
        while (!isAtEnd() && current() != '\n') {
            comment.append(current());
            advance();
        }
        
        return new CHTLToken(CHTLTokenType.GENERATOR_COMMENT, comment.toString(), startLine, startColumn, startPosition);
    }
    
    /**
     * 扫描[]关键字
     */
    private CHTLToken scanBracketKeyword(int startLine, int startColumn, int startPosition) {
        // 先检查是否可能是[]关键字（如[Template]）
        // []关键字必须以字母开头
        advance(); // 跳过[
        
        if (!isAtEnd() && Character.isLetter(current())) {
            // 可能是[]关键字
            StringBuilder keyword = new StringBuilder("[");
            
            while (!isAtEnd() && current() != ']') {
                keyword.append(current());
                advance();
            }
            
            if (current() == ']') {
                keyword.append(']');
                advance();
            }
            
            String key = keyword.toString();
            CHTLTokenType type = BRACKET_KEYWORDS.get(key);
            
            if (type != null) {
                return new CHTLToken(type, key, startLine, startColumn, startPosition);
            } else {
                // 如果不是已知的[]关键字，回退
                // 这种情况比较复杂，暂时报错
                return new CHTLToken(CHTLTokenType.LEFT_BRACKET, "[", startLine, startColumn, startPosition);
            }
        } else {
            // 不是[]关键字，只是普通的[
            // 不需要重置position，因为我们只advance了一次（跳过[）
            return new CHTLToken(CHTLTokenType.LEFT_BRACKET, "[", startLine, startColumn, startPosition);
        }
    }
    
    /**
     * 扫描@关键字
     */
    private CHTLToken scanAtKeyword(int startLine, int startColumn, int startPosition) {
        StringBuilder keyword = new StringBuilder("@");
        advance(); // 跳过@
        
        while (!isAtEnd() && (Character.isLetterOrDigit(current()) || current() == '_')) {
            keyword.append(current());
            advance();
        }
        
        String key = keyword.toString();
        CHTLTokenType type = AT_KEYWORDS.get(key);
        
        if (type != null) {
            return new CHTLToken(type, key, startLine, startColumn, startPosition);
        } else {
            // 如果不是已知的@关键字，作为标识符处理
            return new CHTLToken(CHTLTokenType.IDENTIFIER, key, startLine, startColumn, startPosition);
        }
    }
    
    /**
     * 扫描字符串
     */
    private CHTLToken scanString(char quote, int startLine, int startColumn, int startPosition) {
        advance(); // 跳过引号
        StringBuilder value = new StringBuilder();
        
        while (!isAtEnd() && current() != quote) {
            if (current() == '\\' && peek() == quote) {
                // 转义引号
                advance();
                value.append(quote);
                advance();
            } else if (current() == '\\' && peek() == '\\') {
                // 转义反斜杠
                advance();
                value.append('\\');
                advance();
            } else {
                value.append(current());
                advance();
            }
        }
        
        if (current() == quote) {
            advance(); // 跳过结束引号
        }
        
        return new CHTLToken(CHTLTokenType.STRING_LITERAL, value.toString(), startLine, startColumn, startPosition);
    }
    
    /**
     * 扫描数字
     */
    private CHTLToken scanNumber(int startLine, int startColumn, int startPosition) {
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
        
        return new CHTLToken(CHTLTokenType.NUMBER, value.toString(), startLine, startColumn, startPosition);
    }
    
    /**
     * 扫描标识符
     */
    private CHTLToken scanIdentifier(int startLine, int startColumn, int startPosition) {
        StringBuilder value = new StringBuilder();
        
        while (!isAtEnd() && isIdentifierPart(current())) {
            value.append(current());
            advance();
        }
        
        String identifier = value.toString();
        
        // 检查是否是关键字
        CHTLTokenType type = KEYWORDS.get(identifier);
        if (type != null) {
            return new CHTLToken(type, identifier, startLine, startColumn, startPosition);
        }
        
        // 检查是否是"at top"或"at bottom"
        if ("at".equals(identifier) && skipWhitespaceAndCheck("top")) {
            advance(); advance(); advance(); // 跳过"top"
            return new CHTLToken(CHTLTokenType.AT_TOP, "at top", startLine, startColumn, startPosition);
        }
        if ("at".equals(identifier) && skipWhitespaceAndCheck("bottom")) {
            advance(); advance(); advance(); advance(); advance(); advance(); // 跳过"bottom"
            return new CHTLToken(CHTLTokenType.AT_BOTTOM, "at bottom", startLine, startColumn, startPosition);
        }
        
        // 否则作为标识符
        return new CHTLToken(CHTLTokenType.IDENTIFIER, identifier, startLine, startColumn, startPosition);
    }
    
    /**
     * 跳过空白并检查后续字符串
     */
    private boolean skipWhitespaceAndCheck(String expected) {
        int savedPosition = position;
        int savedLine = line;
        int savedColumn = column;
        
        skipWhitespace();
        
        for (int i = 0; i < expected.length(); i++) {
            if (isAtEnd() || current() != expected.charAt(i)) {
                // 恢复位置
                position = savedPosition;
                line = savedLine;
                column = savedColumn;
                return false;
            }
            if (i < expected.length() - 1) {
                advance();
            }
        }
        
        // 恢复位置以便正确处理
        position = savedPosition;
        line = savedLine;
        column = savedColumn;
        skipWhitespace();
        
        return true;
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
        return Character.isLetter(ch) || ch == '_';
    }
    
    private boolean isIdentifierPart(char ch) {
        return Character.isLetterOrDigit(ch) || ch == '_' || ch == '-';
    }
}