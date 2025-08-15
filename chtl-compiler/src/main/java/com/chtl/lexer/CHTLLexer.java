package com.chtl.lexer;

import java.util.ArrayList;
import java.util.List;

public class CHTLLexer {
	private final String input;
	private final Context context;
	private int p = 0;

	public CHTLLexer(String input, GlobalMap map) {
		this.input = input == null ? "" : input;
		this.context = new Context(map);
	}

	public List<Token> lex() {
		List<Token> tokens = new ArrayList<>();
		while (!isAtEnd()) {
			int start = p;
			char c = advance();
			switch (c) {
				case '{': tokens.add(tok(CHTLTokenType.LBRACE, start, p)); break;
				case '}': tokens.add(tok(CHTLTokenType.RBRACE, start, p)); break;
				case '[': tokens.add(tok(CHTLTokenType.LBRACKET, start, p)); break;
				case ']': tokens.add(tok(CHTLTokenType.RBRACKET, start, p)); break;
				case '(': tokens.add(tok(CHTLTokenType.LPAREN, start, p)); break;
				case ')': tokens.add(tok(CHTLTokenType.RPAREN, start, p)); break;
				case ':': tokens.add(tok(CHTLTokenType.COLON, start, p)); break;
				case '=': tokens.add(tok(CHTLTokenType.EQUAL, start, p)); break;
				case ';': tokens.add(tok(CHTLTokenType.SEMICOLON, start, p)); break;
				case ',': tokens.add(tok(CHTLTokenType.COMMA, start, p)); break;
				case '@': tokens.add(tok(CHTLTokenType.AT, start, p)); break;
				case '\n': tokens.add(tok(CHTLTokenType.NEWLINE, start, p)); break;
				case ' ': case '\t': case '\r':
					consumeWhileWhitespace(start, tokens);
					break;
				case '/':
					if (match('/')) { consumeLineComment(start, tokens); }
					else if (match('*')) { consumeBlockComment(start, tokens); }
					break;
				case '-':
					if (match('-')) { consumeGenComment(start, tokens); }
					else { tokens.add(tok(CHTLTokenType.LITERAL, start, p)); }
					break;
				case '"': case '\'':
					consumeString(c, start, tokens);
					break;
				default:
					if (isAlpha(c) || c == '_' ) {
						consumeIdentifier(start, tokens);
					} else if (isDigit(c)) {
						consumeNumber(start, tokens);
					} else {
						// 无修饰字面量：直到空白或分隔符
						consumeLiteral(start, tokens);
					}
			}
		}
		return tokens;
	}

	private void consumeWhileWhitespace(int start, List<Token> tokens) {
		while (!isAtEnd()) {
			char c = peek();
			if (c == ' ' || c == '\t' || c == '\r') { advance(); }
			else break;
		}
		tokens.add(tok(CHTLTokenType.WHITESPACE, start, p));
	}

	private void consumeLineComment(int start, List<Token> tokens) {
		while (!isAtEnd() && peek() != '\n') advance();
		tokens.add(tok(CHTLTokenType.LINE_COMMENT, start, p));
	}

	private void consumeBlockComment(int start, List<Token> tokens) {
		while (!isAtEnd()) {
			if (peek() == '*' && peekNext() == '/') { advance(); advance(); break; }
			advance();
		}
		tokens.add(tok(CHTLTokenType.BLOCK_COMMENT, start, p));
	}

	private void consumeGenComment(int start, List<Token> tokens) {
		while (!isAtEnd() && peek() != '\n') advance();
		tokens.add(tok(CHTLTokenType.GEN_COMMENT, start, p));
	}

	private void consumeString(char quote, int start, List<Token> tokens) {
		while (!isAtEnd()) {
			char c = advance();
			if (c == '\\') { if (!isAtEnd()) advance(); continue; }
			if (c == quote) break;
		}
		tokens.add(tok(CHTLTokenType.STRING, start, p));
	}

	private void consumeIdentifier(int start, List<Token> tokens) {
		while (!isAtEnd() && (isAlphaNumeric(peek()) || peek() == '-' || peek() == '_')) advance();
		String lexeme = input.substring(start, p);
		CHTLTokenType type = keywordOrIdent(lexeme);
		tokens.add(tok(type, start, p));
	}

	private CHTLTokenType keywordOrIdent(String lexeme) {
		GlobalMap gm = context.getGlobalMap();
		if (gm.matches("KEYWORD_TEXT", lexeme)) return CHTLTokenType.KW_TEXT;
		if (gm.matches("KEYWORD_STYLE", lexeme)) return CHTLTokenType.KW_STYLE;
		if (gm.matches("KEYWORD_SCRIPT", lexeme)) return CHTLTokenType.KW_SCRIPT;
		if (gm.matches("KEYWORD_INHERIT", lexeme)) return CHTLTokenType.KW_INHERIT;
		if (gm.matches("KEYWORD_DELETE", lexeme)) return CHTLTokenType.KW_DELETE;
		if (gm.matches("KEYWORD_INSERT", lexeme)) return CHTLTokenType.KW_INSERT;
		if (gm.matches("KEYWORD_AFTER", lexeme)) return CHTLTokenType.KW_AFTER;
		if (gm.matches("KEYWORD_BEFORE", lexeme)) return CHTLTokenType.KW_BEFORE;
		if (gm.matches("KEYWORD_REPLACE", lexeme)) return CHTLTokenType.KW_REPLACE;
		if (gm.matches("KEYWORD_FROM", lexeme)) return CHTLTokenType.KW_FROM;
		if (gm.matches("KEYWORD_AS", lexeme)) return CHTLTokenType.KW_AS;
		if (gm.matches("KEYWORD_EXCEPT", lexeme)) return CHTLTokenType.KW_EXCEPT;
		return CHTLTokenType.IDENT;
	}

	private void consumeNumber(int start, List<Token> tokens) {
		while (!isAtEnd() && isDigit(peek())) advance();
		if (!isAtEnd() && peek() == '.' && isDigit(peekNext())) {
			advance();
			while (!isAtEnd() && isDigit(peek())) advance();
		}
		tokens.add(tok(CHTLTokenType.NUMBER, start, p));
	}

	private void consumeLiteral(int start, List<Token> tokens) {
		while (!isAtEnd()) {
			char c = peek();
			if (Character.isWhitespace(c)) break;
			if (c == '{' || c == '}' || c == '[' || c == ']' || c == '(' || c == ')' || c == ':' || c == '=' || c == ';' || c == ',' || c == '@') break;
			if (c == '/' || c == '"' || c == '\'' || c == '-') break;
			advance();
		}
		tokens.add(tok(CHTLTokenType.LITERAL, start, p));
	}

	private boolean isAtEnd() { return p >= input.length(); }
	private char advance() { return input.charAt(p++); }
	private char peek() { return isAtEnd() ? '\0' : input.charAt(p); }
	private char peekNext() { return (p + 1 >= input.length()) ? '\0' : input.charAt(p + 1); }
	private boolean match(char expected) { if (isAtEnd() || input.charAt(p) != expected) return false; p++; return true; }
	private boolean isAlpha(char c) { return Character.isLetter(c); }
	private boolean isDigit(char c) { return c >= '0' && c <= '9'; }
	private boolean isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

	private Token tok(CHTLTokenType t, int s, int e) { return new Token(t, input.substring(s, e), s, e); }
}