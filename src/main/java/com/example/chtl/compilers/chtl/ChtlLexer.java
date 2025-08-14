package com.example.chtl.compilers.chtl;

import java.util.ArrayList;
import java.util.List;

public class ChtlLexer {
	private final String src;
	private final int n;
	private int i = 0;
	private int line = 1;
	private int col = 1;

	public ChtlLexer(String src) {
		this.src = src;
		this.n = src.length();
	}

	public List<ChtlToken> lex() {
		List<ChtlToken> tokens = new ArrayList<>();
		while (true) {
			skipWhitespaceAndComments();
			if (eof()) break;
			int start = i;
			int startLine = line;
			int startCol = col;
			char c = peek();
			switch (c) {
				case '{': tokens.add(tok(ChtlTokenType.LBRACE, consumeStr(1), start, startLine, startCol)); break;
				case '}': tokens.add(tok(ChtlTokenType.RBRACE, consumeStr(1), start, startLine, startCol)); break;
				case '[': tokens.add(tok(ChtlTokenType.LBRACKET, consumeStr(1), start, startLine, startCol)); break;
				case ']': tokens.add(tok(ChtlTokenType.RBRACKET, consumeStr(1), start, startLine, startCol)); break;
				case '(': tokens.add(tok(ChtlTokenType.LPAREN, consumeStr(1), start, startLine, startCol)); break;
				case ')': tokens.add(tok(ChtlTokenType.RPAREN, consumeStr(1), start, startLine, startCol)); break;
				case ':': tokens.add(tok(ChtlTokenType.COLON, consumeStr(1), start, startLine, startCol)); break;
				case '=': tokens.add(tok(ChtlTokenType.EQUAL, consumeStr(1), start, startLine, startCol)); break;
				case ';': tokens.add(tok(ChtlTokenType.SEMICOLON, consumeStr(1), start, startLine, startCol)); break;
				case ',': tokens.add(tok(ChtlTokenType.COMMA, consumeStr(1), start, startLine, startCol)); break;
				case '.': tokens.add(tok(ChtlTokenType.DOT, consumeStr(1), start, startLine, startCol)); break;
				case '#': tokens.add(tok(ChtlTokenType.HASH, consumeStr(1), start, startLine, startCol)); break;
				case '@': tokens.add(tok(ChtlTokenType.AT, consumeStr(1), start, startLine, startCol)); break;
				case '\'': tokens.add(readString('\'')); break;
				case '"': tokens.add(readString('"')); break;
				default:
					if (isDigit(c)) {
						tokens.add(readNumber());
					} else if (isIdentStart(c)) {
						ChtlToken ident = readIdent();
						tokens.add(asKeyword(ident));
					} else {
						// 其他字符，作为标识符的一部分处理（宽容）
						tokens.add(readLooseIdent());
					}
			}
		}
		tokens.add(new ChtlToken(ChtlTokenType.EOF, "", i, i, line, col));
		return tokens;
	}

	private void skipWhitespaceAndComments() {
		while (!eof()) {
			char c = peek();
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') { advance(); continue; }
			if (c == '/' && i + 1 < n && src.charAt(i + 1) == '/') { // // 注释
				while (!eof() && peek() != '\n') advance();
				continue;
			}
			if (c == '/' && i + 1 < n && src.charAt(i + 1) == '*') { // 多行注释
				advance(); advance();
				while (!eof()) { if (peek() == '*' && i + 1 < n && src.charAt(i + 1) == '/') { advance(); advance(); break; } advance(); }
				continue;
			}
			if (c == '-' && i + 1 < n && src.charAt(i + 1) == '-') { // -- 注释（文档）
				while (!eof() && peek() != '\n') advance();
				continue;
			}
			break;
		}
	}

	private ChtlToken readString(char quote) {
		int start = i; int sl = line; int sc = col; advance();
		StringBuilder sb = new StringBuilder();
		while (!eof()) {
			char c = peek();
			if (c == '\\') { sb.append(c); advance(); if (!eof()) { sb.append(peek()); advance(); } continue; }
			if (c == quote) { advance(); break; }
			sb.append(c); advance();
		}
		return new ChtlToken(ChtlTokenType.STRING, sb.toString(), start, i, sl, sc);
	}

	private ChtlToken readNumber() {
		int start = i; int sl = line; int sc = col; StringBuilder sb = new StringBuilder();
		while (!eof() && (isDigit(peek()) || peek()=='.')) { sb.append(peek()); advance(); }
		return new ChtlToken(ChtlTokenType.NUMBER, sb.toString(), start, i, sl, sc);
	}

	private ChtlToken readIdent() {
		int start = i; int sl = line; int sc = col; StringBuilder sb = new StringBuilder();
		while (!eof() && isIdentPart(peek())) { sb.append(peek()); advance(); }
		return new ChtlToken(ChtlTokenType.IDENT, sb.toString(), start, i, sl, sc);
	}

	private ChtlToken readLooseIdent() {
		int start = i; int sl = line; int sc = col; StringBuilder sb = new StringBuilder();
		char c = peek(); sb.append(c); advance();
		return new ChtlToken(ChtlTokenType.IDENT, sb.toString(), start, i, sl, sc);
	}

	private ChtlToken asKeyword(ChtlToken ident) {
		String s = ident.lexeme;
		switch (s) {
			case "text": return new ChtlToken(ChtlTokenType.KW_TEXT, s, ident.start, ident.end, ident.line, ident.column);
			case "style": return new ChtlToken(ChtlTokenType.KW_STYLE, s, ident.start, ident.end, ident.line, ident.column);
			case "script": return new ChtlToken(ChtlTokenType.KW_SCRIPT, s, ident.start, ident.end, ident.line, ident.column);
			case "Template": case "template": return new ChtlToken(ChtlTokenType.KW_TEMPLATE, s, ident.start, ident.end, ident.line, ident.column);
			case "Custom": case "custom": return new ChtlToken(ChtlTokenType.KW_CUSTOM, s, ident.start, ident.end, ident.line, ident.column);
			case "Origin": case "origin": return new ChtlToken(ChtlTokenType.KW_ORIGIN, s, ident.start, ident.end, ident.line, ident.column);
			case "Import": case "import": return new ChtlToken(ChtlTokenType.KW_IMPORT, s, ident.start, ident.end, ident.line, ident.column);
			case "Namespace": case "namespace": return new ChtlToken(ChtlTokenType.KW_NAMESPACE, s, ident.start, ident.end, ident.line, ident.column);
			case "inherit": return new ChtlToken(ChtlTokenType.KW_INHERIT, s, ident.start, ident.end, ident.line, ident.column);
			case "delete": return new ChtlToken(ChtlTokenType.KW_DELETE, s, ident.start, ident.end, ident.line, ident.column);
			case "insert": return new ChtlToken(ChtlTokenType.KW_INSERT, s, ident.start, ident.end, ident.line, ident.column);
			case "after": return new ChtlToken(ChtlTokenType.KW_AFTER, s, ident.start, ident.end, ident.line, ident.column);
			case "before": return new ChtlToken(ChtlTokenType.KW_BEFORE, s, ident.start, ident.end, ident.line, ident.column);
			case "replace": return new ChtlToken(ChtlTokenType.KW_REPLACE, s, ident.start, ident.end, ident.line, ident.column);
			case "at": // 可能是 at top / at bottom
				return ident;
			case "top": return new ChtlToken(ChtlTokenType.KW_ATTOP, s, ident.start, ident.end, ident.line, ident.column);
			case "bottom": return new ChtlToken(ChtlTokenType.KW_ATBOTTOM, s, ident.start, ident.end, ident.line, ident.column);
			case "from": return new ChtlToken(ChtlTokenType.KW_FROM, s, ident.start, ident.end, ident.line, ident.column);
			case "as": return new ChtlToken(ChtlTokenType.KW_AS, s, ident.start, ident.end, ident.line, ident.column);
			case "except": return new ChtlToken(ChtlTokenType.KW_EXCEPT, s, ident.start, ident.end, ident.line, ident.column);
			default: return ident;
		}
	}

	private boolean eof() { return i >= n; }
	private char peek() { return src.charAt(i); }
	private String consumeStr(int len) { String s = src.substring(i, i + len); for (int k=0; k<len; k++) advance(); return s; }
	private void advance() { if (peek()=='\n') { line++; col=1; } else { col++; } i++; }
	private static boolean isDigit(char c) { return c >= '0' && c <= '9'; }
	private static boolean isIdentStart(char c) { return Character.isLetter(c) || c=='_' || c=='@' || c=='#' || c=='.'; }
	private static boolean isIdentPart(char c) { return Character.isLetterOrDigit(c) || c=='_' || c=='-' || c=='@' || c=='#' || c=='.'; }

	private ChtlToken tok(ChtlTokenType type, String lexeme, int start, int startLine, int startCol) {
		return new ChtlToken(type, lexeme, start, i, startLine, startCol);
	}
}