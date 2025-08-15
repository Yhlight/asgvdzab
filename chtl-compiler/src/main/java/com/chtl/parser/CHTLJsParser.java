package com.chtl.parser;

import com.chtl.ast.CHTLJsNodes.*;
import com.chtl.lexer.*;

import java.util.ArrayList;
import java.util.List;

public class CHTLJsParser {
	private final List<Token> tokens;
	private int i = 0;

	public CHTLJsParser(String src) { this.tokens = new CHTLJsLexer(src).lex(); }

	public Chain parseChain() {
		Selector sel = null;
		if (matchType(CHTLJsTokenType.MOUSTACHE_L)) {
			sel = parseSelectorInside();
			consumeType(CHTLJsTokenType.MOUSTACHE_R);
		}
		Chain chain = new Chain(sel);
		while (!isAtEnd()) {
			if (matchType(CHTLJsTokenType.ARROW) || matchType(CHTLJsTokenType.DOT)) {
				chain.getInvocations().add(parseInvocation());
			} else {
				advance();
			}
		}
		return chain;
	}

	private Selector parseSelectorInside() {
		StringBuilder sb = new StringBuilder();
		while (!isAtEnd() && !checkType(CHTLJsTokenType.MOUSTACHE_R)) {
			sb.append(advance().getLexeme());
		}
		return new Selector(sb.toString().trim());
	}

	private Invocation parseInvocation() {
		String name = "";
		if (matchType(CHTLJsTokenType.IDENT)) { name = prev().getLexeme(); }
		Invocation inv = new Invocation(name);
		if (matchType(CHTLJsTokenType.LPAREN)) {
			// 简化：收集到匹配的 ) 为止，按 , 分割（不扩展）
			StringBuilder sb = new StringBuilder();
			int depth = 0;
			while (!isAtEnd()) {
				if (checkType(CHTLJsTokenType.LPAREN)) { depth++; sb.append(advance().getLexeme()); continue; }
				if (checkType(CHTLJsTokenType.RPAREN) && depth == 0) break;
				if (checkType(CHTLJsTokenType.RPAREN)) { depth--; sb.append(advance().getLexeme()); continue; }
				sb.append(advance().getLexeme());
			}
			consumeType(CHTLJsTokenType.RPAREN);
			for (String a : sb.toString().split(",")) {
				String v = a.trim(); if (!v.isEmpty()) inv.getArgs().add(v);
			}
		}
		return inv;
	}

	private boolean isAtEnd() { return i >= tokens.size(); }
	private Token advance() { return tokens.get(i++); }
	private Token prev() { return tokens.get(i-1); }
	private boolean checkType(CHTLJsTokenType t) { return !isAtEnd() && tokens.get(i).getType() == t; }
	private boolean matchType(CHTLJsTokenType t) { if (checkType(t)) { advance(); return true; } return false; }
	private void consumeType(CHTLJsTokenType t) { if (!matchType(t)) throw new IllegalStateException("Expect: " + t + " at " + i); }
}