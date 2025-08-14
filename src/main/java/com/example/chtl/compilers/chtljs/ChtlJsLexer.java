package com.example.chtl.compilers.chtljs;

import java.util.ArrayList;
import java.util.List;

public class ChtlJsLexer {
	private final String src;
	private final int n;
	private int i = 0;
	private int line = 1;
	private int col = 1;

	public ChtlJsLexer(String src) { this.src = src; this.n = src.length(); }

	public List<ChtlJsToken> lex() {
		List<ChtlJsToken> tokens = new ArrayList<>();
		while (true) {
			skipWsAndComments();
			if (eof()) break;
			int start = i, sl=line, sc=col;
			char c = peek();
			if (c=='{' && i+1<n && src.charAt(i+1)=='{') { tokens.add(tok(ChtlJsTokenType.SELECTOR_OPEN, consumeStr(2), start, sl, sc)); continue; }
			if (c=='}' && i+1<n && src.charAt(i+1)=='}') { tokens.add(tok(ChtlJsTokenType.SELECTOR_CLOSE, consumeStr(2), start, sl, sc)); continue; }
			if (c=='-' && i+1<n && src.charAt(i+1)=='>') { tokens.add(tok(ChtlJsTokenType.ARROW, consumeStr(2), start, sl, sc)); continue; }
			switch (c) {
				case '{': tokens.add(tok(ChtlJsTokenType.LBRACE, consumeStr(1), start, sl, sc)); break;
				case '}': tokens.add(tok(ChtlJsTokenType.RBRACE, consumeStr(1), start, sl, sc)); break;
				case '[': tokens.add(tok(ChtlJsTokenType.LBRACKET, consumeStr(1), start, sl, sc)); break;
				case ']': tokens.add(tok(ChtlJsTokenType.RBRACKET, consumeStr(1), start, sl, sc)); break;
				case '(': tokens.add(tok(ChtlJsTokenType.LPAREN, consumeStr(1), start, sl, sc)); break;
				case ')': tokens.add(tok(ChtlJsTokenType.RPAREN, consumeStr(1), start, sl, sc)); break;
				case '.': tokens.add(tok(ChtlJsTokenType.DOT, consumeStr(1), start, sl, sc)); break;
				case '#': tokens.add(tok(ChtlJsTokenType.HASH, consumeStr(1), start, sl, sc)); break;
				case ':': tokens.add(tok(ChtlJsTokenType.COLON, consumeStr(1), start, sl, sc)); break;
				case ';': tokens.add(tok(ChtlJsTokenType.SEMICOLON, consumeStr(1), start, sl, sc)); break;
				case ',': tokens.add(tok(ChtlJsTokenType.COMMA, consumeStr(1), start, sl, sc)); break;
				case '\'': tokens.add(readString('\'')); break;
				case '"': tokens.add(readString('"')); break;
				default:
					if (isDigit(c)) { tokens.add(readNumber()); }
					else if (isIdentStart(c)) { tokens.add(asKeyword(readIdent())); }
					else { tokens.add(readLoose()); }
			}
		}
		tokens.add(new ChtlJsToken(ChtlJsTokenType.EOF, "", i, i, line, col));
		return tokens;
	}

	private void skipWsAndComments() {
		while (!eof()) {
			char c = peek();
			if (c==' '||c=='\t'||c=='\r'||c=='\n') { advance(); continue; }
			if (c=='/' && i+1<n && src.charAt(i+1)=='/') { while (!eof() && peek()!='\n') advance(); continue; }
			if (c=='/' && i+1<n && src.charAt(i+1)=='*') { advance(); advance(); while (!eof()) { if (peek()=='*' && i+1<n && src.charAt(i+1)=='/') { advance(); advance(); break; } advance(); } continue; }
			break;
		}
	}

	private ChtlJsToken readString(char quote) {
		int s=i, sl=line, sc=col; advance(); StringBuilder sb=new StringBuilder();
		while (!eof()) { char c=peek(); if (c=='\\') { sb.append(c); advance(); if(!eof()){ sb.append(peek()); advance(); } continue; } if(c==quote){ advance(); break; } sb.append(c); advance(); }
		return new ChtlJsToken(ChtlJsTokenType.STRING, sb.toString(), s, i, sl, sc);
	}

	private ChtlJsToken readNumber() {
		int s=i, sl=line, sc=col; StringBuilder sb=new StringBuilder();
		while (!eof() && (isDigit(peek())||peek()=='.')) { sb.append(peek()); advance(); }
		return new ChtlJsToken(ChtlJsTokenType.NUMBER, sb.toString(), s, i, sl, sc);
	}

	private ChtlJsToken readIdent() {
		int s=i, sl=line, sc=col; StringBuilder sb=new StringBuilder();
		while (!eof() && isIdentPart(peek())) { sb.append(peek()); advance(); }
		return new ChtlJsToken(ChtlJsTokenType.IDENT, sb.toString(), s, i, sl, sc);
	}

	private ChtlJsToken readLoose() {
		int s=i, sl=line, sc=col; char c=peek(); advance();
		return new ChtlJsToken(ChtlJsTokenType.IDENT, String.valueOf(c), s, i, sl, sc);
	}

	private ChtlJsToken asKeyword(ChtlJsToken t) {
		String s = t.lexeme;
		switch (s) {
			case "listen": return new ChtlJsToken(ChtlJsTokenType.KW_LISTEN, s, t.start, t.end, t.line, t.column);
			case "delegate": return new ChtlJsToken(ChtlJsTokenType.KW_DELEGATE, s, t.start, t.end, t.line, t.column);
			case "animate": return new ChtlJsToken(ChtlJsTokenType.KW_ANIMATE, s, t.start, t.end, t.line, t.column);
			default: return t;
		}
	}

	private boolean eof(){ return i>=n; }
	private char peek(){ return src.charAt(i); }
	private String consumeStr(int len){ String s=src.substring(i,i+len); for(int k=0;k<len;k++) advance(); return s; }
	private void advance(){ if (peek()=='\n'){ line++; col=1; } else { col++; } i++; }
	private static boolean isDigit(char c){ return c>='0'&&c<='9'; }
	private static boolean isIdentStart(char c){ return Character.isLetter(c) || c=='_' || c=='.' || c=='#'; }
	private static boolean isIdentPart(char c){ return Character.isLetterOrDigit(c) || c=='_' || c=='-' || c=='.' || c=='#'; }

	private ChtlJsToken tok(ChtlJsTokenType type, String lexeme, int start, int startLine, int startCol){
		return new ChtlJsToken(type, lexeme, start, i, startLine, startCol);
	}
}