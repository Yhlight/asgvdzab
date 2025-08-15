package com.chtl.parser;

import com.chtl.ast.DocumentNode;
import com.chtl.ast.TemplateNodes.ElementNode;
import com.chtl.ast.TemplateNodes.TextNode;
import com.chtl.ast.TemplateNodes.ScriptNode;
import com.chtl.lexer.*;

import java.util.ArrayList;
import java.util.List;

public class CHTLParser {
	private final List<Token> tokens;
	private int i = 0;

	public CHTLParser(String src, GlobalMap map) {
		this.tokens = new CHTLLexer(src, map).lex();
	}

	public DocumentNode parseDocument() {
		DocumentNode doc = new DocumentNode();
		while (!isAtEnd()) {
			AstOrNone n = parseElementOrText();
			if (n != null && n.node != null) doc.addChild(n.node);
			else advance();
		}
		return doc;
	}

	private static class AstOrNone { final com.chtl.ast.AstNode node; AstOrNone(com.chtl.ast.AstNode n){this.node=n;} }

	private AstOrNone parseElementOrText() {
		// element: IDENT '{' ... '}'
		if (matchType(CHTLTokenType.IDENT)) {
			String tag = prev().getLexeme();
			if (matchType(CHTLTokenType.LBRACE)) {
				ElementNode el = new ElementNode(tag);
				parseElementBody(el);
				return new AstOrNone(el);
			}
		}
		// text { STRING | LITERAL }
		if (matchType(CHTLTokenType.KW_TEXT) && matchType(CHTLTokenType.LBRACE)) {
			String content = collectUntil(CHTLTokenType.RBRACE);
			consumeType(CHTLTokenType.RBRACE);
			return new AstOrNone(new TextNode(content.trim()));
		}
		return null;
	}

	private void parseElementBody(ElementNode el) {
		while (!isAtEnd() && !checkType(CHTLTokenType.RBRACE)) {
			if (matchType(CHTLTokenType.KW_STYLE) && matchType(CHTLTokenType.LBRACE)) {
				// 收集 style 内联文本，后续生成器再解析属性
				String styleText = collectUntil(CHTLTokenType.RBRACE);
				consumeType(CHTLTokenType.RBRACE);
				// 暂存为文本节点，生成阶段再解释为内联样式
				el.getTexts().add(new TextNode("/*style*/" + styleText));
				continue;
			}
			if (matchType(CHTLTokenType.KW_SCRIPT) && matchType(CHTLTokenType.LBRACE)) {
				String js = collectUntil(CHTLTokenType.RBRACE);
				consumeType(CHTLTokenType.RBRACE);
				el.getScripts().add(new ScriptNode(js));
				continue;
			}
			AstOrNone child = parseElementOrText();
			if (child != null && child.node != null) {
				if (child.node instanceof ElementNode) {
					el.getChildren().add((ElementNode) child.node);
				} else if (child.node instanceof TextNode) {
					el.getTexts().add((TextNode) child.node);
				} else {
					// 其他类型暂不支持，前进避免死循环
					advance();
				}
			} else advance();
		}
		consumeType(CHTLTokenType.RBRACE);
	}

	private String collectUntil(CHTLTokenType end) {
		StringBuilder sb = new StringBuilder();
		int depth = 0;
		while (!isAtEnd()) {
			if (checkType(CHTLTokenType.LBRACE)) { depth++; sb.append(advance().getLexeme()); continue; }
			if (checkType(end) && depth == 0) break;
			if (checkType(CHTLTokenType.RBRACE)) { depth--; sb.append(advance().getLexeme()); continue; }
			sb.append(advance().getLexeme());
		}
		return sb.toString();
	}

	private boolean isAtEnd() { return i >= tokens.size(); }
	private Token advance() { return tokens.get(i++); }
	private Token prev() { return tokens.get(i-1); }
	private boolean checkType(CHTLTokenType t) { return !isAtEnd() && tokens.get(i).getType() == t; }
	private boolean matchType(CHTLTokenType t) { if (checkType(t)) { advance(); return true; } return false; }
	private void consumeType(CHTLTokenType t) { if (!matchType(t)) throw new IllegalStateException("Expect: " + t + " at " + i); }
}