package com.chtl.parser;

import com.chtl.ast.DocumentNode;
import com.chtl.ast.TemplateNodes.ElementNode;
import com.chtl.ast.TemplateNodes.TextNode;
import com.chtl.ast.TemplateNodes.ScriptNode;
import com.chtl.ast.TemplateNodes.Attribute;
import com.chtl.ast.TemplateNodes.StyleProperty;
import com.chtl.lexer.*;
import com.chtl.runtime.ContextAssistant;

import java.util.ArrayList;
import java.util.List;

public class CHTLParser {
	private final List<Token> tokens;
	private int i = 0;
	private final ContextAssistant ctx = new ContextAssistant();

	public CHTLParser(String src, GlobalMap map) {
		this.tokens = new CHTLLexer(src, map).lex();
	}

	public DocumentNode parseDocument() {
		DocumentNode doc = new DocumentNode();
		try (var g = ctx.enter(State.DEFAULT)) {
			while (!isAtEnd()) {
				AstOrNone n = parseElementOrText();
				if (n != null && n.node != null) doc.addChild(n.node);
				else advance();
			}
		}
		return doc;
	}

	private static class AstOrNone { final com.chtl.ast.AstNode node; AstOrNone(com.chtl.ast.AstNode n){this.node=n;} }

	private AstOrNone parseElementOrText() {
		if (matchType(CHTLTokenType.IDENT)) {
			String tag = prev().getLexeme();
			if (matchType(CHTLTokenType.LBRACE)) {
				ElementNode el = new ElementNode(tag);
				try (var g = ctx.enter(State.IN_ELEMENT_BLOCK)) {
					parseElementBody(el);
				}
				return new AstOrNone(el);
			}
		}
		if (matchType(CHTLTokenType.KW_TEXT) && matchType(CHTLTokenType.LBRACE)) {
			try (var g = ctx.enter(State.IN_TEXT_BLOCK)) {
				String content = collectUntil(CHTLTokenType.RBRACE);
				consumeType(CHTLTokenType.RBRACE);
				return new AstOrNone(new TextNode(content.trim()));
			}
		}
		return null;
	}

	private void parseElementBody(ElementNode el) {
		while (!isAtEnd() && !checkType(CHTLTokenType.RBRACE)) {
			if (matchType(CHTLTokenType.KW_STYLE) && matchType(CHTLTokenType.LBRACE)) {
				try (var g = ctx.enter(State.IN_STYLE_BLOCK)) {
					String styleText = collectUntil(CHTLTokenType.RBRACE);
					consumeType(CHTLTokenType.RBRACE);
					parseInlineStyle(styleText, el);
				}
				continue;
			}
			if (matchType(CHTLTokenType.KW_SCRIPT) && matchType(CHTLTokenType.LBRACE)) {
				try (var g = ctx.enter(State.IN_SCRIPT_BLOCK)) {
					String js = collectUntil(CHTLTokenType.RBRACE);
					consumeType(CHTLTokenType.RBRACE);
					el.getScripts().add(new ScriptNode(js));
				}
				continue;
			}
			// 属性：name : value ; 或 name = value ; 其中 value 可为 STRING/IDENT/LITERAL/NUMBER
			if (checkType(CHTLTokenType.IDENT)) {
				int save = i;
				String key = tokens.get(i).getLexeme(); advance();
				if (matchType(CHTLTokenType.COLON) || matchType(CHTLTokenType.EQUAL)) {
					StringBuilder v = new StringBuilder();
					while (!isAtEnd() && !checkType(CHTLTokenType.SEMICOLON) && !checkType(CHTLTokenType.RBRACE)) {
						v.append(advance().getLexeme());
					}
					if (matchType(CHTLTokenType.SEMICOLON)) {
						el.getAttributes().add(new Attribute(key, v.toString().trim()));
						continue;
					} else {
						// 回退，作为子节点解析
						i = save;
					}
				} else {
					// 回退还原
					i = save;
				}
			}
			AstOrNone child = parseElementOrText();
			if (child != null && child.node != null) {
				if (child.node instanceof ElementNode) {
					el.getChildren().add((ElementNode) child.node);
				} else if (child.node instanceof TextNode) {
					el.getTexts().add((TextNode) child.node);
				} else {
					advance();
				}
			} else advance();
		}
		consumeType(CHTLTokenType.RBRACE);
	}

	private void parseInlineStyle(String styleText, ElementNode el) {
		// 仅处理 "键 : 值;" 与 "键 = 值;"，值支持字符串与字面量
		int p = 0; int n = styleText.length();
		while (p < n) {
			// 跳过空白和换行
			while (p < n && Character.isWhitespace(styleText.charAt(p))) p++;
			if (p >= n) break;
			// 读取键
			int kStart = p;
			while (p < n && isKeyChar(styleText.charAt(p))) p++;
			String key = styleText.substring(kStart, p).trim();
			// 跳过空白
			while (p < n && Character.isWhitespace(styleText.charAt(p))) p++;
			if (p < n && (styleText.charAt(p) == ':' || styleText.charAt(p) == '=')) p++;
			while (p < n && Character.isWhitespace(styleText.charAt(p))) p++;
			// 读取值直到分号
			int vStart = p; boolean inStr = false; char q = '\0';
			while (p < n) {
				char c = styleText.charAt(p);
				if (!inStr && c == ';') break;
				if (!inStr && c == '\n') break;
				if (c == '"' || c == '\'') { if (!inStr) { inStr = true; q = c; } else if (q == c) { inStr = false; } }
				p++;
			}
			String val = styleText.substring(vStart, p).trim();
			if (p < n && styleText.charAt(p) == ';') p++;
			if (!key.isEmpty() && !val.isEmpty()) el.getInlineStyles().add(new StyleProperty(key, val));
		}
	}

	private boolean isKeyChar(char c) { return Character.isLetterOrDigit(c) || c == '-' || c == '_'; }

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