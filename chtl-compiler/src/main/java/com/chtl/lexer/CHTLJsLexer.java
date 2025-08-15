package com.chtl.lexer;

import java.util.ArrayList;
import java.util.List;

public class CHTLJsLexer {
	private final String input;
	private int p = 0;

	public CHTLJsLexer(String input) { this.input = input == null ? "" : input; }

	public List<Token> lex() {
		List<Token> tokens = new ArrayList<>();
		while (!isAtEnd()) {
			int start = p;
			char c = advance();
			switch (c) {
				case '{':
					if (match('{')) tokens.add(tok(CHTLJsTokenType.MOUSTACHE_L, start, p));
					else tokens.add(tok(CHTLJsTokenType.LBRACE, start, p));
					break;
				case '}':
					if (match('}')) tokens.add(tok(CHTLJsTokenType.MOUSTACHE_R, start, p));
					else tokens.add(tok(CHTLJsTokenType.RBRACE, start, p));
					break;
				case '(': tokens.add(tok(CHTLJsTokenType.LPAREN, start, p)); break;
				case ')': tokens.add(tok(CHTLJsTokenType.RPAREN, start, p)); break;
				case '[': tokens.add(tok(CHTLJsTokenType.LBRACKET, start, p)); break;
				case ']': tokens.add(tok(CHTLJsTokenType.RBRACKET, start, p)); break;
				case ',': tokens.add(tok(CHTLJsTokenType.COMMA, start, p)); break;
				case ':': tokens.add(tok(CHTLJsTokenType.COLON, start, p)); break;
				case ';': tokens.add(tok(CHTLJsTokenType.SEMICOLON, start, p)); break;
				case '.': tokens.add(tok(CHTLJsTokenType.DOT, start, p)); break;
				case '#': tokens.add(tok(CHTLJsTokenType.HASH, start, p)); break;
				case '\n': tokens.add(tok(CHTLJsTokenType.NEWLINE, start, p)); break;
				case ' ': case '\t': case '\r':
					consumeWhitespace(start, tokens); break;
				case '-': if (match('>')) tokens.add(tok(CHTLJsTokenType.ARROW, start, p)); else tokens.add(tok(CHTLJsTokenType.UNKNOWN, start, p)); break;
				case '\'': case '"': consumeString(c, start, tokens); break;
				default:
					if (isAlpha(c) || c == '_' ) consumeIdent(start, tokens);
					else if (isDigit(c)) consumeNumber(start, tokens);
					else tokens.add(tok(CHTLJsTokenType.UNKNOWN, start, p));
			}
		}
		return tokens;
	}

	private void consumeWhitespace(int start, List<Token> tokens) {
		while (!isAtEnd()) {
			char c = peek();
			if (c == ' ' || c == '\t' || c == '\r') advance(); else break;
		}
		tokens.add(tok(CHTLJsTokenType.WHITESPACE, start, p));
	}

	private void consumeString(char quote, int start, List<Token> tokens) {
		while (!isAtEnd()) {
			char c = advance();
			if (c == '\\') { if (!isAtEnd()) advance(); continue; }
			if (c == quote) break;
		}
		tokens.add(tok(CHTLJsTokenType.STRING, start, p));
	}

	private void consumeIdent(int start, List<Token> tokens) {
		while (!isAtEnd() && (isAlphaNumeric(peek()) || peek() == '-' || peek() == '_')) advance();
		tokens.add(tok(CHTLJsTokenType.IDENT, start, p));
	}

	private void consumeNumber(int start, List<Token> tokens) {
		while (!isAtEnd() && isDigit(peek())) advance();
		if (!isAtEnd() && peek() == '.' && isDigit(peekNext())) { advance(); while (!isAtEnd() && isDigit(peek())) advance(); }
		tokens.add(tok(CHTLJsTokenType.NUMBER, start, p));
	}

	private boolean isAtEnd() { return p >= input.length(); }
	private char advance() { return input.charAt(p++); }
	private char peek() { return isAtEnd() ? '\0' : input.charAt(p); }
	private char peekNext() { return (p + 1 >= input.length()) ? '\0' : input.charAt(p + 1); }
	private boolean match(char expected) { if (isAtEnd() || input.charAt(p) != expected) return false; p++; return true; }
	private boolean isAlpha(char c) { return Character.isLetter(c); }
	private boolean isDigit(char c) { return c >= '0' && c <= '9'; }
	private boolean isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

	private Token tok(CHTLJsTokenType t, int s, int e) { return new Token(t, input.substring(s, e), s, e); }
}