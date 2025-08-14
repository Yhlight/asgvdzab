package com.example.chtl.parsers.chtl;

import com.example.chtl.ast.chtl.*;
import com.example.chtl.compilers.chtl.ChtlToken;
import com.example.chtl.compilers.chtl.ChtlTokenType;
import com.example.chtl.compilers.chtl.ChtlState;

import java.util.*;

public class ChtlParser {
	private final String source;
	private final List<ChtlToken> tokens;
	private int pos = 0;
	private final ChtlState state;

	// 模板与自定义注册表（支持命名空间前缀）
	private final Map<String, TemplateNodes.StyleTemplate> styleTemplates = new HashMap<>();
	private final Map<String, TemplateNodes.ElementTemplate> elementTemplates = new HashMap<>();
	private final Map<String, TemplateNodes.VarTemplate> varTemplates = new HashMap<>();

	public ChtlParser(String source, List<ChtlToken> tokens){ this(source, tokens, new ChtlState()); }
	public ChtlParser(String source, List<ChtlToken> tokens, ChtlState state){ this.source=source; this.tokens=tokens; this.state=state; }

	public ChtlDocument parseDocument(){
		ChtlDocument doc = new ChtlDocument(0, source.length());
		while (!isAtEnd()) {
			if (check(ChtlTokenType.EOF)) break;
			ChtlNode n = parseTopLevel();
			if (n != null) doc.addItem(n); else advance();
		}
		return doc;
	}

	private ChtlNode parseTopLevel(){
		if (check(ChtlTokenType.LBRACKET)) return parseBracketDirective();
		if (check(ChtlTokenType.IDENT)) return parseElement();
		return null;
	}

	// [Template]/[Custom]/[Namespace]/[Import] 等
	private ChtlNode parseBracketDirective(){
		consume(ChtlTokenType.LBRACKET);
		ChtlToken head = advance(); // 可能是 KW_TEMPLATE/KW_CUSTOM/KW_NAMESPACE/KW_IMPORT 或 IDENT
		consume(ChtlTokenType.RBRACKET);
		String kw;
		switch (head.type) {
			case KW_TEMPLATE -> kw = "Template";
			case KW_CUSTOM -> kw = "Custom";
			case KW_NAMESPACE -> kw = "Namespace";
			case KW_IMPORT -> kw = "Import";
			default -> kw = head.lexeme;
		}
		return switch (kw) {
			case "Template", "template" -> parseTemplateDirective();
			case "Custom", "custom" -> parseCustomDirective();
			case "Namespace", "namespace" -> parseNamespaceDirective();
			case "Import", "import" -> parseImportDirective();
			default -> null;
		};
	}

	private ChtlNode parseTemplateDirective(){
		consume(ChtlTokenType.AT);
		String type = consume(ChtlTokenType.IDENT).lexeme; // Style/Element/Var
		String name = consume(ChtlTokenType.IDENT).lexeme;
		consume(ChtlTokenType.LBRACE);
		int bodyStart = previous().start;
		if (type.equals("Style")) {
			TemplateNodes.StyleTemplate t = new TemplateNodes.StyleTemplate(bodyStart, bodyStart, fq(name));
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
				if (check(ChtlTokenType.IDENT)) {
					ChtlToken p = consume(ChtlTokenType.IDENT);
					if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) {
						String v = parseValueUntilSemicolon();
						consumeOpt(ChtlTokenType.SEMICOLON);
						t.addDecl(new AttributeNode(p.start, currentEnd(), p.lexeme, v));
					}
				} else advance();
			}
			consume(ChtlTokenType.RBRACE);
			styleTemplates.put(fq(name), t);
			return t;
		} else if (type.equals("Element")) {
			TemplateNodes.ElementTemplate t = new TemplateNodes.ElementTemplate(bodyStart, bodyStart, fq(name));
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
				ChtlNode n = parseTopLevel();
				if (n != null) t.add(n); else advance();
			}
			consume(ChtlTokenType.RBRACE);
			elementTemplates.put(fq(name), t);
			return t;
		} else if (type.equals("Var")) {
			TemplateNodes.VarTemplate t = new TemplateNodes.VarTemplate(bodyStart, bodyStart, fq(name));
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
				if (check(ChtlTokenType.IDENT)) {
					ChtlToken k = consume(ChtlTokenType.IDENT);
					if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) {
						String v = parseValueUntilSemicolon();
						consumeOpt(ChtlTokenType.SEMICOLON);
						t.add(new AttributeNode(k.start, currentEnd(), k.lexeme, v));
					}
				} else advance();
			}
			consume(ChtlTokenType.RBRACE);
			varTemplates.put(fq(name), t);
			return t;
		}
		return null;
	}

	private ChtlNode parseCustomDirective(){
		// 先解析到类型与名称，然后解析体为占位（保留到 AST），但不做复杂语义，后续可扩展
		consume(ChtlTokenType.AT);
		String type = consume(ChtlTokenType.IDENT).lexeme; // Style/Element/Var
		String name = consume(ChtlTokenType.IDENT).lexeme;
		consume(ChtlTokenType.LBRACE);
		int bodyStart = previous().start;
		if (type.equals("Style")) {
			TemplateNodes.CustomStyle n = new TemplateNodes.CustomStyle(bodyStart, bodyStart, fq(name));
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) advance();
			consume(ChtlTokenType.RBRACE);
			return n;
		} else if (type.equals("Element")) {
			TemplateNodes.CustomElement n = new TemplateNodes.CustomElement(bodyStart, bodyStart, fq(name));
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) advance();
			consume(ChtlTokenType.RBRACE);
			return n;
		} else if (type.equals("Var")) {
			TemplateNodes.CustomVar n = new TemplateNodes.CustomVar(bodyStart, bodyStart, fq(name));
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) advance();
			consume(ChtlTokenType.RBRACE);
			return n;
		}
		return null;
	}

	private ChtlNode parseNamespaceDirective(){
		String name = consume(ChtlTokenType.IDENT).lexeme;
		boolean hasBlock = check(ChtlTokenType.LBRACE);
		TemplateNodes.ElementTemplate marker = null;
		state.namespaceStack.push(name);
		state.currentNamespace = String.join(".", state.namespaceStack);
		ImportNode.NamespaceNode node = new ImportNode.NamespaceNode(peek().start, peek().end, state.currentNamespace);
		if (hasBlock) {
			consume(ChtlTokenType.LBRACE);
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
				ChtlNode n = parseTopLevel();
				if (n != null) node.add(n); else advance();
			}
			consume(ChtlTokenType.RBRACE);
		}
		state.namespaceStack.pop();
		state.currentNamespace = String.join(".", state.namespaceStack);
		return node;
	}

	private ChtlNode parseImportDirective(){
		// [Import] ... 语法多样，这里仅记录到 AST，不做加载
		int s = peek().start;
		StringBuilder sb = new StringBuilder();
		while (!isAtEnd() && !check(ChtlTokenType.LBRACE) && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) {
			sb.append(tokenToText(advance())).append(' ');
		}
		consumeOpt(ChtlTokenType.SEMICOLON);
		return new ImportNode(s, currentEnd(), "raw", sb.toString().trim(), null);
	}

	private ElementNode parseElement(){
		int start = peek().start; String tag = consume(ChtlTokenType.IDENT).lexeme;
		consume(ChtlTokenType.LBRACE);
		state.elementStack.push(tag);
		ElementNode el = new ElementNode(start, start, tag);
		while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
			if (check(ChtlTokenType.IDENT)) {
				ChtlToken ident = consume(ChtlTokenType.IDENT);
				// lookahead: 如果紧接的是 ':'/'=' 视为属性，否则如果后面是 '{' 视为子元素，否则尝试属性（兼容无分号）
				if (check(ChtlTokenType.COLON) || check(ChtlTokenType.EQUAL)) {
					advance();
					String value = parseValueUntilSemicolon();
					consumeOpt(ChtlTokenType.SEMICOLON);
					el.addAttribute(new AttributeNode(ident.start, currentEnd(), ident.lexeme, value));
				} else if (check(ChtlTokenType.LBRACE)) {
					// 子元素
					unread();
					el.addChild(parseElement());
				} else {
					// 尝试属性行，不存在则忽略
					el.addAttribute(new AttributeNode(ident.start, currentEnd(), ident.lexeme, ""));
				}
			} else if (matchKw(ChtlTokenType.KW_TEXT)) {
				el.addChild(parseText());
			} else if (matchKw(ChtlTokenType.KW_STYLE)) {
				el.addChild(parseStyleBlock(el));
			} else if (matchKw(ChtlTokenType.KW_SCRIPT)) {
				el.addChild(parseScriptBlock());
			} else if (check(ChtlTokenType.AT)) {
				// @Element/@Style 等用法
				advance();
				String type = consume(ChtlTokenType.IDENT).lexeme;
				if (type.equals("Element")) {
					String name = consume(ChtlTokenType.IDENT).lexeme;
					String ns = null;
					if (match(ChtlTokenType.KW_FROM)) { // from 命名空间
						ns = consume(ChtlTokenType.IDENT).lexeme;
					}
					consumeOpt(ChtlTokenType.SEMICOLON);
					TemplateNodes.ElementTemplate tpl = elementTemplates.get(ns==null? fq(name) : ns+"."+name);
					if (tpl != null) {
						for (ChtlNode n : tpl.body()) el.addChild(n);
					}
				} else {
					// 其他 @ 指令忽略
					while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) advance();
					consumeOpt(ChtlTokenType.SEMICOLON);
				}
			} else if (check(ChtlTokenType.LBRACKET)) {
				// 允许内嵌模板/自定义定义
				ChtlNode def = parseBracketDirective();
				if (def != null) el.addChild(def);
			} else {
				advance();
			}
		}
		consume(ChtlTokenType.RBRACE);
		state.elementStack.pop();
		return new ElementNode(start, currentEnd(), tag) {{
			for (AttributeNode a : el.attributes()) addAttribute(a);
			for (ChtlNode c : el.children()) addChild(c);
		}};
	}

	private TextNode parseText(){
		consume(ChtlTokenType.LBRACE);
		int contentStart = currentEnd();
		StringBuilder sb = new StringBuilder();
		while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { sb.append(tokenToText(advance())); }
		consume(ChtlTokenType.RBRACE);
		return new TextNode(contentStart, currentEnd(), sb.toString().trim());
	}

	private StyleBlockNode parseStyleBlock(ElementNode owner){
		consume(ChtlTokenType.LBRACE);
		StyleBlockNode node = new StyleBlockNode(previous().start, previous().end);
		String ownerAutoClass = null;
		while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
			if (check(ChtlTokenType.IDENT)) {
				ChtlToken prop = consume(ChtlTokenType.IDENT);
				if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) {
					String value = parseValueUntilSemicolon();
					value = tryResolveVar(value); // 变量组值替换
					consumeOpt(ChtlTokenType.SEMICOLON);
					node.addInline(new AttributeNode(prop.start, currentEnd(), prop.lexeme, value));
				}
			} else if (check(ChtlTokenType.AT)) {
				advance();
				String atType = consume(ChtlTokenType.IDENT).lexeme; // Style
				if (atType.equals("Style")) {
					String name = consume(ChtlTokenType.IDENT).lexeme;
					consumeOpt(ChtlTokenType.SEMICOLON);
					TemplateNodes.StyleTemplate tpl = styleTemplates.get(fq(name));
					if (tpl != null) for (var d : tpl.declarations()) node.addInline(d);
				} else {
					while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) advance();
					consumeOpt(ChtlTokenType.SEMICOLON);
				}
			} else if (match(ChtlTokenType.DOT) || match(ChtlTokenType.HASH) || check(ChtlTokenType.AMP)) {
				boolean isDot = previous().type == ChtlTokenType.DOT;
				boolean isHash = previous().type == ChtlTokenType.HASH;
				boolean isAmp = check(ChtlTokenType.AMP);
				if (isAmp) advance();
				String name;
				if (isAmp) {
					String cls = owner.attributes().stream().filter(a->a.name().equals("class")).map(AttributeNode::value).findFirst().orElse(null);
					String id = owner.attributes().stream().filter(a->a.name().equals("id")).map(AttributeNode::value).findFirst().orElse(null);
					if (cls != null) name = "." + cls; else if (id != null) name = "#" + id; else { ownerAutoClass = state.nextAutoClass(); name = "." + ownerAutoClass; }
				} else {
					name = (isDot ? "." : "#") + consume(ChtlTokenType.IDENT).lexeme;
					if (isDot && owner.attributes().stream().noneMatch(a->a.name().equals("class"))) ownerAutoClass = ownerAutoClass == null ? name.substring(1) : ownerAutoClass;
					if (isHash && owner.attributes().stream().noneMatch(a->a.name().equals("id"))) owner.addAttribute(new AttributeNode(previous().start, previous().end, "id", name.substring(1)));
				}
				// 处理伪类/伪元素后缀，如 &:hover 或 ::before
				StringBuilder sel = new StringBuilder(name);
				while (check(ChtlTokenType.COLON)) {
					int colonCount = 0;
					while (check(ChtlTokenType.COLON) && colonCount < 2) { advance(); sel.append(':'); colonCount++; }
					if (check(ChtlTokenType.IDENT)) { sel.append(consume(ChtlTokenType.IDENT).lexeme); }
					if (check(ChtlTokenType.LPAREN)) {
						// 简化收集括号内容
						advance(); sel.append('(');
						int depth = 1;
						while (!isAtEnd() && depth > 0) {
							if (check(ChtlTokenType.LPAREN)) { sel.append('('); advance(); depth++; }
							else if (check(ChtlTokenType.RPAREN)) { sel.append(')'); advance(); depth--; }
							else { sel.append(tokenToText(advance())); }
						}
					}
				}
				consume(ChtlTokenType.LBRACE);
				StyleRuleNode rule = new StyleRuleNode(previous().start, previous().end, sel.toString());
				while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
					if (check(ChtlTokenType.IDENT)) {
						ChtlToken p = consume(ChtlTokenType.IDENT);
						if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) {
							String val = parseValueUntilSemicolon();
							val = tryResolveVar(val);
							consumeOpt(ChtlTokenType.SEMICOLON);
							rule.addDecl(new AttributeNode(p.start, currentEnd(), p.lexeme, val));
						}
					} else { advance(); }
				}
				consume(ChtlTokenType.RBRACE);
				node.addRule(rule);
			} else { advance(); }
		}
		consume(ChtlTokenType.RBRACE);
		if (ownerAutoClass != null && owner.attributes().stream().noneMatch(a->a.name().equals("class"))) owner.addAttribute(new AttributeNode(node.startOffset(), node.endOffset(), "class", ownerAutoClass));
		return new StyleBlockNode(node.startOffset(), currentEnd()) {{ for (var a : node.inlineStyles()) addInline(a); for (var r : node.globalRules()) addRule(r); }};
	}

	private ScriptBlockNode parseScriptBlock(){
		consume(ChtlTokenType.LBRACE);
		int start = previous().start;
		StringBuilder sb = new StringBuilder();
		while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { sb.append(tokenToText(advance())); }
		consume(ChtlTokenType.RBRACE);
		return new ScriptBlockNode(start, currentEnd(), sb.toString());
	}

	private String tryResolveVar(String value){
		String v = value.trim();
		int l = v.indexOf('('); int r = v.lastIndexOf(')');
		if (l>0 && r>l && !v.contains(" ")) {
			String name = v.substring(0,l);
			String key = v.substring(l+1,r).trim();
			TemplateNodes.VarTemplate tpl = varTemplates.get(fq(name));
			if (tpl != null) {
				for (AttributeNode a : tpl.kv()) if (a.name().equals(key)) return a.value();
			}
		}
		return value;
	}

	private String fq(String name){ return state.currentNamespace==null||state.currentNamespace.isEmpty()? name : state.currentNamespace+"."+name; }

	private String parseValueUntilSemicolon(){
		StringBuilder sb = new StringBuilder();
		while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) {
			ChtlToken t = advance();
			if (t.type == ChtlTokenType.STRING || t.type == ChtlTokenType.NUMBER || t.type == ChtlTokenType.IDENT) { sb.append(t.lexeme); }
			else { sb.append(tokenToText(t)); }
		}
		return sb.toString().trim();
	}

	private String tokenToText(ChtlToken t){
		return switch (t.type) {
			case LBRACE -> "{"; case RBRACE -> "}"; case LBRACKET -> "["; case RBRACKET -> "]";
			case LPAREN -> "("; case RPAREN -> ")"; case COLON -> ":"; case EQUAL -> "="; case SEMICOLON -> ";"; case COMMA -> ",";
			case DOT -> "."; case HASH -> "#"; case AT -> "@"; case AMP -> "&";
			default -> t.lexeme;
		};
	}

	private boolean matchKw(ChtlTokenType t){ return match(t); }
	private boolean match(ChtlTokenType t){ if (check(t)) { advance(); return true; } return false; }
	private boolean check(ChtlTokenType t){ if (isAtEnd()) return false; return peek().type == t; }
	private ChtlToken consume(ChtlTokenType t){ if (!check(t)) throw new RuntimeException("期待:"+t+" 实际:"+peek().type); return advance(); }
	private void consumeOpt(ChtlTokenType t){ if (check(t)) advance(); }
	private ChtlToken advance(){ if (!isAtEnd()) pos++; return previous(); }
	private void unread(){ if (pos>0) pos--; }
	private boolean isAtEnd(){ return peek().type==ChtlTokenType.EOF; }
	private ChtlToken peek(){ return tokens.get(pos); }
	private ChtlToken previous(){ return tokens.get(pos-1); }
	private int currentEnd(){ return previous().end; }
}