package com.example.chtl.parsers.chtljs;

import com.example.chtl.ast.chtljs.ArrowCallNode;
import com.example.chtl.ast.chtljs.ChtlJsScript;
import com.example.chtl.ast.chtljs.SelectorNode;
import com.example.chtl.compilers.chtljs.ChtlJsToken;
import com.example.chtl.compilers.chtljs.ChtlJsTokenType;

import java.util.List;

public class ChtlJsParser {
	private final String source;
	private final List<ChtlJsToken> tokens;
	private int pos = 0;

	public ChtlJsParser(String source, List<ChtlJsToken> tokens) {
		this.source = source;
		this.tokens = tokens;
	}

	public ChtlJsScript parseScript() {
		ChtlJsScript script = new ChtlJsScript(0, source.length());
		while (!isAtEnd()) {
			// 允许空表达式分隔
			if (match(ChtlJsTokenType.SEMICOLON)) continue;

			SelectorNode selector = null;
			if (match(ChtlJsTokenType.SELECTOR_OPEN)) {
				selector = parseSelector();
				consume(ChtlJsTokenType.SELECTOR_CLOSE);
			}
			ArrowCallNode calls = parseArrowCalls();
			if (selector == null && calls == null) {
				// 无效或空白，跳过一个 token 防止死循环
				advance();
				continue;
			}
			int start = selector != null ? selector.startOffset() : (calls != null ? calls.startOffset() : currentStart());
			int end = calls != null ? calls.endOffset() : (selector != null ? selector.endOffset() : currentEnd());
			script.add(new ChtlJsScript.Expr(start, end, selector, calls));
			match(ChtlJsTokenType.SEMICOLON); // 可选分号
		}
		return script;
	}

	private SelectorNode parseSelector() {
		int start = previous().start;
		SelectorNode node = new SelectorNode(start, start);
		while (!check(ChtlJsTokenType.SELECTOR_CLOSE) && !isAtEnd()) {
			SelectorNode.Part.Kind kind;
			String name;
			Integer index = null;
			if (match(ChtlJsTokenType.DOT)) {
				kind = SelectorNode.Part.Kind.CLASS;
				name = consumeIdentLike();
			} else if (match(ChtlJsTokenType.HASH)) {
				kind = SelectorNode.Part.Kind.ID;
				name = consumeIdentLike();
			} else if (check(ChtlJsTokenType.IDENT)) {
				kind = SelectorNode.Part.Kind.TAG;
				name = consumeIdentLike();
			} else {
				break;
			}
			// 可选索引
			if (match(ChtlJsTokenType.LBRACKET)) {
				String num = consume(ChtlJsTokenType.NUMBER).lexeme;
				consume(ChtlJsTokenType.RBRACKET);
				try { index = Integer.parseInt(num.contains(".") ? num.substring(0, num.indexOf('.')) : num); } catch (Exception ignored) { index = 0; }
			}
			node.add(new SelectorNode.Part(kind, name, index));
		}
		// 复制链并更新端位
		SelectorNode newNode = new SelectorNode(start, currentEnd());
		for (var p : node.chain()) newNode.add(p);
		return newNode;
	}

	private ArrowCallNode parseArrowCalls() {
		if (!check(ChtlJsTokenType.ARROW)) return null;
		int start = peek().start;
		ArrowCallNode node = new ArrowCallNode(start, start);
		while (match(ChtlJsTokenType.ARROW)) {
			String method = null;
			if (match(ChtlJsTokenType.KW_LISTEN)) method = "listen";
			else if (match(ChtlJsTokenType.KW_DELEGATE)) method = "delegate";
			else if (match(ChtlJsTokenType.KW_ANIMATE)) method = "animate";
			else method = consume(ChtlJsTokenType.IDENT).lexeme;

			String argsRaw = null;
			if (match(ChtlJsTokenType.LPAREN)) {
				int lparenStart = previous().start;
				int rparenEnd = findMatchingParenEndIndex();
				if (rparenEnd < 0) {
					argsRaw = "";
				} else {
					// 提取括号内原始文本
					argsRaw = source.substring(lparenStart + 1, rparenEnd - 1);
					// 将 pos 移动到右括号之后
					while (!isAtEnd() && peek().end <= rparenEnd) advance();
				}
			}
			node.add(new ArrowCallNode.Call(method, argsRaw));
		}
		// 复制链并更新端位
		ArrowCallNode newNode = new ArrowCallNode(start, currentEnd());
		for (var c : node.chain()) newNode.add(c);
		return newNode;
	}

	// 在 token 流上找到与最近 '(' 匹配的 ')' 的 end 偏移
	private int findMatchingParenEndIndex() {
		int depthParen = 1;
		int depthBrace = 0;
		int depthBracket = 0;
		int i = pos;
		while (i < tokens.size()) {
			ChtlJsToken t = tokens.get(i);
			switch (t.type) {
				case LPAREN: depthParen++; break;
				case RPAREN: depthParen--; if (depthParen == 0) return t.end; break;
				case LBRACE: depthBrace++; break;
				case RBRACE: if (depthBrace>0) depthBrace--; break;
				case LBRACKET: depthBracket++; break;
				case RBRACKET: if (depthBracket>0) depthBracket--; break;
				default: break;
			}
			i++;
		}
		return -1;
	}

	private boolean match(ChtlJsTokenType type) { if (check(type)) { advance(); return true; } return false; }
	private boolean check(ChtlJsTokenType type) { if (isAtEnd()) return false; return peek().type == type; }
	private ChtlJsToken consume(ChtlJsTokenType type) { if (!check(type)) throw error("期待: " + type + ", 实际: " + peek().type); return advance(); }
	private String consumeIdentLike() { return consume(ChtlJsTokenType.IDENT).lexeme; }
	private ChtlJsToken advance() { if (!isAtEnd()) pos++; return previous(); }
	private boolean isAtEnd() { return peek().type == ChtlJsTokenType.EOF; }
	private ChtlJsToken peek() { return tokens.get(pos); }
	private ChtlJsToken previous() { return tokens.get(pos - 1); }
	private int currentStart() { return peek().start; }
	private int currentEnd() { return previous().end; }
	private RuntimeException error(String msg) { return new RuntimeException(msg); }
}