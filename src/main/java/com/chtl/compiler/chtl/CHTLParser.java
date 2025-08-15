package com.chtl.compiler.chtl;
import java.util.ArrayList;
import java.util.List;

import com.chtl.compiler.chtl.CHTLParser.Lexer;
import com.chtl.compiler.chtl.CHTLParser.Token;
import com.chtl.model.CHTLNode;
import com.chtl.model.CustomNode;
import com.chtl.model.ElementNode;
import com.chtl.model.ScriptNode;
import com.chtl.model.StyleNode;
import com.chtl.model.TemplateNode;
import com.chtl.model.TextNode;


import com.chtl.model.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * CHTL解析器 - 手写实现
 * 负责将CHTL代码解析成抽象语法树(AST)
 */
public class CHTLParser {
    private static final Logger logger = LoggerFactory.getLogger(CHTLParser.class);
    
    // Token类型
    private enum TokenType {
        IDENTIFIER,        // 标识符
        LEFT_BRACE,       // {
        RIGHT_BRACE,      // }
        SEMICOLON,        // ;
        COLON,            // :
        EQUALS,           // =
        STRING,           // 字符串字面量
        COMMENT,          // 注释
        KEYWORD_TEXT,     // text关键字
        KEYWORD_STYLE,    // style关键字
        KEYWORD_SCRIPT,   // script关键字
        KEYWORD_TEMPLATE, // [Template]
        KEYWORD_CUSTOM,   // [Custom]
        AT_STYLE,         // @Style
        AT_ELEMENT,       // @Element
        AT_VAR,           // @Var
        EOF               // 文件结束
    }
    
    // Token类
    private static class Token {
        TokenType type;
        String value;
        int line;
        int column;
        
        Token(TokenType type, String value, int line, int column) {
            this.type = type;
            this.value = value;
            this.line = line;
            this.column = column;
        }
    }
    
    // 词法分析器
    private class Lexer {
        private String input;
        private int position;
        private int line;
        private int column;
        
        Lexer(String input) {
            this.input = input;
            this.position = 0;
            this.line = 1;
            this.column = 1;
        }
        
        Token nextToken() {
            skipWhitespace();
            
            if (position >= input.length()) {
                return new Token(TokenType.EOF, "", line, column);
            }
            
            // 检查注释
            if (peek() == '/' && peekNext() == '/') {
                return readLineComment();
            }
            
            if (peek() == '/' && peekNext() == '*') {
                return readBlockComment();
            }
            
            // 检查关键字
            if (peek() == '[') {
                return readBracketKeyword();
            }
            
            if (peek() == '@') {
                return readAtKeyword();
            }
            
            // 检查标点符号
            char ch = peek();
            switch (ch) {
                case '{':
                    advance();
                    return new Token(TokenType.LEFT_BRACE, "{", line, column - 1);
                case '}':
                    advance();
                    return new Token(TokenType.RIGHT_BRACE, "}", line, column - 1);
                case ';':
                    advance();
                    return new Token(TokenType.SEMICOLON, ";", line, column - 1);
                case ':':
                    advance();
                    return new Token(TokenType.COLON, ":", line, column - 1);
                case '=':
                    advance();
                    return new Token(TokenType.EQUALS, "=", line, column - 1);
                case '"':
                case '\'':
                    return readString();
            }
            
            // 读取标识符或无引号字面量
            if (isIdentifierStart(ch)) {
                return readIdentifier();
            }
            
            throw new RuntimeException("意外的字符: " + ch + " at " + line + ":" + column);
        }
        
        private void skipWhitespace() {
            while (position < input.length() && Character.isWhitespace(peek())) {
                if (peek() == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
                position++;
            }
        }
        
        private char peek() {
            return position < input.length() ? input.charAt(position) : '\0';
        }
        
        private char peekNext() {
            return position + 1 < input.length() ? input.charAt(position + 1) : '\0';
        }
        
        private void advance() {
            if (peek() == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            position++;
        }
        
        private Token readIdentifier() {
            int startColumn = column;
            StringBuilder sb = new StringBuilder();
            
            while (position < input.length() && isIdentifierPart(peek())) {
                sb.append(peek());
                advance();
            }
            
            String value = sb.toString();
            TokenType type = TokenType.IDENTIFIER;
            
            // 检查是否是关键字
            switch (value) {
                case "text":
                    type = TokenType.KEYWORD_TEXT;
                    break;
                case "style":
                    type = TokenType.KEYWORD_STYLE;
                    break;
                case "script":
                    type = TokenType.KEYWORD_SCRIPT;
                    break;
            }
            
            return new Token(type, value, line, startColumn);
        }
        
        private Token readString() {
            int startColumn = column;
            char quote = peek();
            advance(); // 跳过引号
            
            StringBuilder sb = new StringBuilder();
            while (position < input.length() && peek() != quote) {
                if (peek() == '\\' && peekNext() == quote) {
                    advance(); // 跳过转义字符
                    sb.append(quote);
                    advance();
                } else {
                    sb.append(peek());
                    advance();
                }
            }
            
            if (peek() == quote) {
                advance(); // 跳过结束引号
            } else {
                throw new RuntimeException("未终止的字符串");
            }
            
            return new Token(TokenType.STRING, sb.toString(), line, startColumn);
        }
        
        private Token readLineComment() {
            int startColumn = column;
            advance(); // 跳过第一个/
            advance(); // 跳过第二个/
            
            StringBuilder sb = new StringBuilder("//");
            while (position < input.length() && peek() != '\n') {
                sb.append(peek());
                advance();
            }
            
            return new Token(TokenType.COMMENT, sb.toString(), line, startColumn);
        }
        
        private Token readBlockComment() {
            int startColumn = column;
            advance(); // 跳过/
            advance(); // 跳过*
            
            StringBuilder sb = new StringBuilder("/*");
            while (position < input.length() && !(peek() == '*' && peekNext() == '/')) {
                sb.append(peek());
                advance();
            }
            
            if (position < input.length()) {
                sb.append("*/");
                advance(); // 跳过*
                advance(); // 跳过/
            }
            
            return new Token(TokenType.COMMENT, sb.toString(), line, startColumn);
        }
        
        private Token readBracketKeyword() {
            int startColumn = column;
            StringBuilder sb = new StringBuilder();
            
            while (position < input.length() && peek() != ']') {
                sb.append(peek());
                advance();
            }
            
            if (peek() == ']') {
                sb.append(']');
                advance();
            }
            
            String value = sb.toString();
            TokenType type = TokenType.IDENTIFIER;
            
            if (value.equals("[Template]")) {
                type = TokenType.KEYWORD_TEMPLATE;
            } else if (value.equals("[Custom]")) {
                type = TokenType.KEYWORD_CUSTOM;
            }
            
            return new Token(type, value, line, startColumn);
        }
        
        private Token readAtKeyword() {
            int startColumn = column;
            StringBuilder sb = new StringBuilder();
            
            while (position < input.length() && (Character.isLetterOrDigit(peek()) || peek() == '@')) {
                sb.append(peek());
                advance();
            }
            
            String value = sb.toString();
            TokenType type = TokenType.IDENTIFIER;
            
            switch (value) {
                case "@Style":
                    type = TokenType.AT_STYLE;
                    break;
                case "@Element":
                    type = TokenType.AT_ELEMENT;
                    break;
                case "@Var":
                    type = TokenType.AT_VAR;
                    break;
            }
            
            return new Token(type, value, line, startColumn);
        }
        
        private boolean isIdentifierStart(char ch) {
            return Character.isLetter(ch) || ch == '_' || ch == '-';
        }
        
        private boolean isIdentifierPart(char ch) {
            return Character.isLetterOrDigit(ch) || ch == '_' || ch == '-' || ch == '.';
        }
    }
    
    // 语法分析器
    private Lexer lexer;
    private Token currentToken;
    private List<Token> tokens;
    private int tokenIndex;
    
    /**
     * 解析CHTL代码
     */
    public CHTLNode parse(String input) {
        logger.debug("开始解析CHTL代码");
        
        // 词法分析
        lexer = new Lexer(input);
        tokens = new ArrayList<>();
        Token token;
        while ((token = lexer.nextToken()).type != TokenType.EOF) {
            if (token.type != TokenType.COMMENT) { // 过滤注释
                tokens.add(token);
            }
        }
        tokens.add(token); // 添加EOF
        
        // 语法分析
        tokenIndex = 0;
        currentToken = tokens.get(0);
        
        ElementNode root = new ElementNode("root");
        
        while (currentToken.type != TokenType.EOF) {
            CHTLNode node = parseTopLevel();
            if (node != null) {
                root.addChild(node);
            }
        }
        
        logger.debug("CHTL解析完成");
        return root;
    }
    
    private Token consume(TokenType type) {
        if (currentToken.type != type) {
            throw new RuntimeException("期望 " + type + " 但得到 " + currentToken.type);
        }
        Token token = currentToken;
        advance();
        return token;
    }
    
    private void advance() {
        if (tokenIndex < tokens.size() - 1) {
            tokenIndex++;
            currentToken = tokens.get(tokenIndex);
        }
    }
    
    private CHTLNode parseTopLevel() {
        switch (currentToken.type) {
            case KEYWORD_TEMPLATE:
                return parseTemplate();
            case KEYWORD_CUSTOM:
                return parseCustom();
            case IDENTIFIER:
            case KEYWORD_TEXT:
            case KEYWORD_STYLE:
            case KEYWORD_SCRIPT:
                return parseElement();
            default:
                advance(); // 跳过无法识别的token
                return null;
        }
    }
    
    private CHTLNode parseTemplate() {
        consume(TokenType.KEYWORD_TEMPLATE);
        
        TemplateNode.TemplateType type = null;
        if (currentToken.type == TokenType.AT_STYLE) {
            type = TemplateNode.TemplateType.STYLE;
            advance();
        } else if (currentToken.type == TokenType.AT_ELEMENT) {
            type = TemplateNode.TemplateType.ELEMENT;
            advance();
        } else if (currentToken.type == TokenType.AT_VAR) {
            type = TemplateNode.TemplateType.VAR;
            advance();
        }
        
        String name = consume(TokenType.IDENTIFIER).value;
        TemplateNode template = new TemplateNode(type, name);
        
        consume(TokenType.LEFT_BRACE);
        
        // 解析模板内容
        while (currentToken.type != TokenType.RIGHT_BRACE) {
            CHTLNode child = parseElement();
            if (child != null) {
                template.addChild(child);
            }
        }
        
        consume(TokenType.RIGHT_BRACE);
        
        return template;
    }
    
    private CHTLNode parseCustom() {
        consume(TokenType.KEYWORD_CUSTOM);
        
        CustomNode.CustomType type = null;
        if (currentToken.type == TokenType.AT_STYLE) {
            type = CustomNode.CustomType.STYLE;
            advance();
        } else if (currentToken.type == TokenType.AT_ELEMENT) {
            type = CustomNode.CustomType.ELEMENT;
            advance();
        } else if (currentToken.type == TokenType.AT_VAR) {
            type = CustomNode.CustomType.VAR;
            advance();
        }
        
        String name = consume(TokenType.IDENTIFIER).value;
        CustomNode custom = new CustomNode(type, name);
        
        consume(TokenType.LEFT_BRACE);
        
        // 解析自定义内容
        while (currentToken.type != TokenType.RIGHT_BRACE) {
            CHTLNode child = parseElement();
            if (child != null) {
                custom.addChild(child);
            }
        }
        
        consume(TokenType.RIGHT_BRACE);
        
        return custom;
    }
    
    private CHTLNode parseElement() {
        if (currentToken.type == TokenType.KEYWORD_TEXT) {
            return parseTextNode();
        } else if (currentToken.type == TokenType.KEYWORD_STYLE) {
            return parseStyleNode();
        } else if (currentToken.type == TokenType.KEYWORD_SCRIPT) {
            return parseScriptNode();
        } else if (currentToken.type == TokenType.IDENTIFIER) {
            String tagName = currentToken.value;
            advance();
            
            ElementNode element = new ElementNode(tagName);
            
            consume(TokenType.LEFT_BRACE);
            
            // 解析元素内容
            while (currentToken.type != TokenType.RIGHT_BRACE) {
                if (currentToken.type == TokenType.IDENTIFIER && 
                    tokens.get(tokenIndex + 1).type == TokenType.COLON) {
                    // 解析属性
                    parseAttribute(element);
                } else {
                    // 解析子元素
                    CHTLNode child = parseElement();
                    if (child != null) {
                        element.addChild(child);
                    }
                }
            }
            
            consume(TokenType.RIGHT_BRACE);
            
            return element;
        }
        
        return null;
    }
    
    private void parseAttribute(ElementNode element) {
        String name = consume(TokenType.IDENTIFIER).value;
        consume(TokenType.COLON);
        
        String value;
        if (currentToken.type == TokenType.STRING) {
            value = currentToken.value;
            advance();
        } else if (currentToken.type == TokenType.IDENTIFIER) {
            // 无引号字面量
            value = currentToken.value;
            advance();
        } else {
            throw new RuntimeException("期望属性值");
        }
        
        consume(TokenType.SEMICOLON);
        
        element.setAttribute(name, value);
    }
    
    private TextNode parseTextNode() {
        consume(TokenType.KEYWORD_TEXT);
        consume(TokenType.LEFT_BRACE);
        
        String content;
        if (currentToken.type == TokenType.STRING) {
            content = currentToken.value;
            advance();
        } else {
            // 读取无引号文本
            StringBuilder sb = new StringBuilder();
            while (currentToken.type != TokenType.RIGHT_BRACE) {
                sb.append(currentToken.value).append(" ");
                advance();
            }
            content = sb.toString().trim();
        }
        
        consume(TokenType.RIGHT_BRACE);
        
        return new TextNode(content);
    }
    
    private StyleNode parseStyleNode() {
        consume(TokenType.KEYWORD_STYLE);
        consume(TokenType.LEFT_BRACE);
        
        StyleNode styleNode = new StyleNode(true); // 局部样式
        
        // 简化处理：将style块内容作为原始文本保存
        StringBuilder content = new StringBuilder();
        int braceCount = 1;
        
        while (braceCount > 0 && currentToken.type != TokenType.EOF) {
            if (currentToken.type == TokenType.LEFT_BRACE) {
                braceCount++;
            } else if (currentToken.type == TokenType.RIGHT_BRACE) {
                braceCount--;
                if (braceCount == 0) break;
            }
            
            content.append(currentToken.value).append(" ");
            advance();
        }
        
        consume(TokenType.RIGHT_BRACE);
        
        // 这里简化处理，实际应该解析CSS规则
        StyleNode.StyleRule rule = new StyleNode.StyleRule("inline");
        rule.addProperty(new StyleNode.StyleProperty("_raw", content.toString()));
        styleNode.addRule(rule);
        
        return styleNode;
    }
    
    private ScriptNode parseScriptNode() {
        consume(TokenType.KEYWORD_SCRIPT);
        consume(TokenType.LEFT_BRACE);
        
        // 读取script内容
        StringBuilder content = new StringBuilder();
        int braceCount = 1;
        
        while (braceCount > 0 && currentToken.type != TokenType.EOF) {
            if (currentToken.type == TokenType.LEFT_BRACE) {
                braceCount++;
            } else if (currentToken.type == TokenType.RIGHT_BRACE) {
                braceCount--;
                if (braceCount == 0) break;
            }
            
            content.append(currentToken.value).append(" ");
            advance();
        }
        
        consume(TokenType.RIGHT_BRACE);
        
        return new ScriptNode(content.toString().trim(), true);
    }
}