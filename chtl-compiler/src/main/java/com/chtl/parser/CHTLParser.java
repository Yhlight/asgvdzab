package com.chtl.parser;

import com.chtl.ast.DocumentNode;
import com.chtl.ast.TemplateNodes.ElementNode;
import com.chtl.ast.TemplateNodes.TextNode;
import com.chtl.ast.TemplateNodes.ScriptNode;
import com.chtl.ast.TemplateNodes.Attribute;
import com.chtl.ast.TemplateNodes.StyleProperty;
import com.chtl.ast.CssRule;
import com.chtl.lexer.*;
import com.chtl.runtime.ContextAssistant;
import com.chtl.template.TemplateRegistry;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class CHTLParser {
	private final List<Token> tokens;
	private int i = 0;
	private final ContextAssistant ctx = new ContextAssistant();
	private final DocumentNode document;
	private final TemplateRegistry templates = new TemplateRegistry();

	public CHTLParser(String src, GlobalMap map) {
		this.tokens = new CHTLLexer(src, map).lex();
		this.document = new DocumentNode();
	}

	public CHTLParser(String src, GlobalMap map, DocumentNode doc) {
		this.tokens = new CHTLLexer(src, map).lex();
		this.document = doc;
	}

	public DocumentNode parseDocument() {
		DocumentNode doc = this.document;
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
					parseStyleBlock(styleText, el);
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
			// 属性：name : value ; 或 name = value ;
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
						i = save;
					}
				} else {
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

	private void parseStyleBlock(String styleText, ElementNode el) {
		int p = 0; int n = styleText.length();
		while (p < n) {
			while (p < n && Character.isWhitespace(styleText.charAt(p))) p++;
			if (p >= n) break;
			// 模板样式使用：@Style Name; 合并到内联样式
			if (styleText.startsWith("@Style", p)) {
				p += "@Style".length();
				while (p < n && Character.isWhitespace(styleText.charAt(p))) p++;
				int nameStart = p; while (p < n && (Character.isLetterOrDigit(styleText.charAt(p)) || styleText.charAt(p) == '_' || styleText.charAt(p) == '-')) p++;
				String name = styleText.substring(nameStart, p);
				while (p < n && styleText.charAt(p) != ';') p++;
				if (p < n && styleText.charAt(p) == ';') p++;
				Set<String> visiting = new HashSet<>();
				List<StyleProperty> props = templates.resolveStyleInlineProperties(name, visiting);
				el.getInlineStyles().addAll(props);
				continue;
			}
			if (styleText.charAt(p) == '.' || styleText.charAt(p) == '#') {
				char prefix = styleText.charAt(p);
				int selStart = p; p++;
				while (p < n && !Character.isWhitespace(styleText.charAt(p)) && styleText.charAt(p) != '{') p++;
				String selector = styleText.substring(selStart, p).trim();
				String name = selector.substring(1);
				if (prefix == '.') maybeAttachAttribute(el, "class", name);
				if (prefix == '#') maybeAttachAttribute(el, "id", name);
				while (p < n && Character.isWhitespace(styleText.charAt(p))) p++;
				if (p < n && styleText.charAt(p) == '{') {
					p++;
					String block = readUntilBraceClose(styleText, p);
					p += block.length();
					if (p < n && styleText.charAt(p) == '}') p++;
					CssRule rule = new CssRule(selector);
					parseStyleDeclarations(block, rule);
					this.document.getGlobalCssRules().add(rule);
				}
				continue;
			}
			if (styleText.charAt(p) == '&') {
				int selStart = p; p++;
				while (p < n && !Character.isWhitespace(styleText.charAt(p)) && styleText.charAt(p) != '{') p++;
				String suffix = styleText.substring(selStart + 1, p).trim();
				String base = deriveBaseSelectorFromElement(el);
				if (base != null) {
					String selector = base + suffix;
					while (p < n && Character.isWhitespace(styleText.charAt(p))) p++;
					if (p < n && styleText.charAt(p) == '{') {
						p++;
						String block = readUntilBraceClose(styleText, p);
						p += block.length();
						if (p < n && styleText.charAt(p) == '}') p++;
						CssRule rule = new CssRule(selector);
						parseStyleDeclarations(block, rule);
						this.document.getGlobalCssRules().add(rule);
					}
				}
				continue;
			}
			int kStart = p;
			while (p < n && isKeyChar(styleText.charAt(p))) p++;
			String key = styleText.substring(kStart, p).trim();
			while (p < n && Character.isWhitespace(styleText.charAt(p))) p++;
			if (p < n && (styleText.charAt(p) == ':' || styleText.charAt(p) == '=')) p++;
			while (p < n && Character.isWhitespace(styleText.charAt(p))) p++;
			int vStart = p; boolean inStr = false; char q = '\0';
			while (p < n) {
				char c = styleText.charAt(p);
				if (!inStr && (c == ';' || c == '\n' || c == '}')) break;
				if (c == '"' || c == '\'') { if (!inStr) { inStr = true; q = c; } else if (q == c) { inStr = false; } }
				p++;
			}
			String val = styleText.substring(vStart, p).trim();
			val = templates.substituteVarCalls(val);
			if (p < n && styleText.charAt(p) == ';') p++;
			if (!key.isEmpty() && !val.isEmpty()) el.getInlineStyles().add(new StyleProperty(key, val));
		}
	}

	private String readUntilBraceClose(String s, int from) {
		int p = from, n = s.length(), depth = 0; StringBuilder b = new StringBuilder();
		while (p < n) {
			char c = s.charAt(p);
			if (c == '{') { depth++; b.append(c); p++; continue; }
			if (c == '}' && depth == 0) break;
			if (c == '}' && depth > 0) { depth--; b.append(c); p++; continue; }
			b.append(c); p++;
		}
		return b.toString();
	}

	private void parseStyleDeclarations(String block, CssRule rule) {
		int p = 0; int n = block.length();
		while (p < n) {
			while (p < n && Character.isWhitespace(block.charAt(p))) p++;
			int kStart = p;
			while (p < n && isKeyChar(block.charAt(p))) p++;
			String key = block.substring(kStart, p).trim();
			while (p < n && Character.isWhitespace(block.charAt(p))) p++;
			if (p < n && (block.charAt(p) == ':' || block.charAt(p) == '=')) p++;
			while (p < n && Character.isWhitespace(block.charAt(p))) p++;
			int vStart = p; boolean inStr = false; char q = '\0';
			while (p < n) {
				char c = block.charAt(p);
				if (!inStr && (c == ';' || c == '\n' || c == '}')) break;
				if (c == '"' || c == '\'') { if (!inStr) { inStr = true; q = c; } else if (q == c) { inStr = false; } }
				p++;
			}
			String val = block.substring(vStart, p).trim();
			if (p < n && block.charAt(p) == ';') p++;
			if (!key.isEmpty() && !val.isEmpty()) rule.getDeclarations().add(new StyleProperty(key, val));
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

	private String deriveBaseSelectorFromElement(ElementNode el) {
		String cls = null;
		String id = null;
		for (Attribute a : el.getAttributes()) {
			if (a.getName().equals("class")) cls = a.getValue();
			if (a.getName().equals("id")) id = a.getValue();
		}
		if (cls != null && !cls.isEmpty()) return "." + cls;
		if (id != null && !id.isEmpty()) return "#" + id;
		return null;
	}

	private void maybeAttachAttribute(ElementNode el, String name, String value) {
		for (Attribute a : el.getAttributes()) {
			if (a.getName().equals(name)) return;
		}
		el.getAttributes().add(new Attribute(name, value));
	}
}