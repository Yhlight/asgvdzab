package com.example.chtl.parsers.chtl;

import com.example.chtl.ast.chtl.*;
import com.example.chtl.compilers.chtl.ChtlToken;
import com.example.chtl.compilers.chtl.ChtlTokenType;
import com.example.chtl.compilers.chtl.ChtlState;

import java.util.ArrayList;
import java.util.List;

public class ChtlParser {
	private final String source;
	private final List<ChtlToken> tokens;
	private int pos = 0;
	private final ChtlState state;

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
		if (matchBracketedTemplate()) return parseTemplateOrCustom();
		if (matchBracketedImport()) return parseImport();
		if (matchBracketedNamespace()) return parseNamespace();
		if (check(ChtlTokenType.IDENT)) return parseElement();
		return null;
	}

	private boolean matchBracketedTemplate(){ return check(ChtlTokenType.LBRACKET) && tokens.get(pos+1).type==ChtlTokenType.KW_TEMPLATE && tokens.get(pos+2).type==ChtlTokenType.RBRACKET; }
	private boolean matchBracketedCustom(){ return check(ChtlTokenType.LBRACKET) && tokens.get(pos+1).type==ChtlTokenType.KW_CUSTOM && tokens.get(pos+2).type==ChtlTokenType.RBRACKET; }
	private boolean matchBracketedNamespace(){ return check(ChtlTokenType.LBRACKET) && tokens.get(pos+1).type==ChtlTokenType.KW_NAMESPACE; }
	private boolean matchBracketedImport(){ return check(ChtlTokenType.LBRACKET) && tokens.get(pos+1).type==ChtlTokenType.KW_IMPORT; }

	private ChtlNode parseTemplateOrCustom(){
		consume(ChtlTokenType.LBRACKET);
		boolean isTemplate = match(ChtlTokenType.KW_TEMPLATE);
		boolean isCustom = !isTemplate && match(ChtlTokenType.KW_CUSTOM);
		consume(ChtlTokenType.RBRACKET);
		consume(ChtlTokenType.AT);
		String kind = consume(ChtlTokenType.IDENT).lexeme; // Style / Element / Var
		String name = consume(ChtlTokenType.IDENT).lexeme;
		consume(ChtlTokenType.LBRACE);
		if (isTemplate) {
			if (kind.equals("Style")) {
				TemplateNodes.StyleTemplate t = new TemplateNodes.StyleTemplate(previous().start, previous().end, qname(name));
				while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
					if (check(ChtlTokenType.IDENT)) {
						var p = consume(ChtlTokenType.IDENT);
						if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) {
							String v = parseValueUntilSemicolon(); consumeOpt(ChtlTokenType.SEMICOLON);
							t.addDecl(new AttributeNode(p.start, currentEnd(), p.lexeme, v));
						}
					} else if (match(ChtlTokenType.KW_INHERIT)) {
						consume(ChtlTokenType.AT); String fromKind = consume(ChtlTokenType.IDENT).lexeme; String fromName = consume(ChtlTokenType.IDENT).lexeme; consumeOpt(ChtlTokenType.SEMICOLON);
						var base = state.globals.styleTemplates.get(qname(fromName)); if (base instanceof TemplateNodes.StyleTemplate st) { for (var d: st.declarations()) t.addDecl(d); }
					} else advance();
				}
				consume(ChtlTokenType.RBRACE);
				state.globals.styleTemplates.put(t.name(), t);
				return t;
			} else if (kind.equals("Element")) {
				TemplateNodes.ElementTemplate t = new TemplateNodes.ElementTemplate(previous().start, previous().end, qname(name));
				while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
					ChtlNode n = parseElement(); if (n!=null) t.add(n); else advance();
				}
				consume(ChtlTokenType.RBRACE);
				state.globals.elementTemplates.put(t.name(), t);
				return t;
			} else if (kind.equals("Var")) {
				TemplateNodes.VarTemplate t = new TemplateNodes.VarTemplate(previous().start, previous().end, qname(name));
				while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
					if (check(ChtlTokenType.IDENT)) {
						var p = consume(ChtlTokenType.IDENT);
						if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) {
							String v = parseValueUntilSemicolon(); consumeOpt(ChtlTokenType.SEMICOLON);
							t.add(new AttributeNode(p.start, currentEnd(), p.lexeme, v));
						}
					} else advance();
				}
				consume(ChtlTokenType.RBRACE);
				state.globals.varTemplates.put(t.name(), t);
				return t;
			}
		} else if (isCustom) {
			consume(ChtlTokenType.LBRACE);
			if (kind.equals("Style")) {
				TemplateNodes.CustomStyle c = new TemplateNodes.CustomStyle(previous().start, previous().end, qname(name));
				while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { if (check(ChtlTokenType.IDENT)) { String k = consume(ChtlTokenType.IDENT).lexeme; if (check(ChtlTokenType.COMMA) || check(ChtlTokenType.SEMICOLON)) advance(); c.addProp(k); } else { advance(); } }
				consume(ChtlTokenType.RBRACE); state.globals.customStyles.put(c.name(), c); return c;
			} else if (kind.equals("Element")) {
				TemplateNodes.CustomElement c = new TemplateNodes.CustomElement(previous().start, previous().end, qname(name));
				while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { ChtlNode n = parseElement(); if (n!=null) c.add(n); else advance(); }
				consume(ChtlTokenType.RBRACE); state.globals.customElements.put(c.name(), c); return c;
			} else if (kind.equals("Var")) {
				TemplateNodes.CustomVar c = new TemplateNodes.CustomVar(previous().start, previous().end, qname(name));
				while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { if (check(ChtlTokenType.IDENT)) { c.addKey(consume(ChtlTokenType.IDENT).lexeme); if (check(ChtlTokenType.COMMA)) advance(); } else advance(); }
				consume(ChtlTokenType.RBRACE); state.globals.customVars.put(c.name(), c); return c;
			}
		}
		return null;
	}

	private ChtlNode parseNamespace(){
		consume(ChtlTokenType.LBRACKET); consume(ChtlTokenType.KW_NAMESPACE); String nsName=""; if (!check(ChtlTokenType.RBRACKET)) { nsName = consume(ChtlTokenType.IDENT).lexeme; } consume(ChtlTokenType.RBRACKET);
		state.namespaceStack.push(nsName); state.currentNamespace = nsName;
		ImportNode.NamespaceNode ns = new ImportNode.NamespaceNode(previous().start, previous().end, nsName);
		if (check(ChtlTokenType.LBRACE)) {
			consume(ChtlTokenType.LBRACE);
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { if (matchKw(ChtlTokenType.KW_TEMPLATE) || matchBracketedTemplate()) { unread(); ns.add(parseTemplateOrCustom()); } else if (matchBracketedImport()) { ns.add(parseImport()); } else if (check(ChtlTokenType.IDENT)) { ns.add(parseElement()); } else if (check(ChtlTokenType.KW_TEMPLATE) || check(ChtlTokenType.KW_CUSTOM)) { ns.add(parseTemplateOrCustom()); } else { advance(); } }
			consume(ChtlTokenType.RBRACE);
		}
		state.namespaceStack.pop(); state.currentNamespace = state.namespaceStack.isEmpty()?"":state.namespaceStack.peek();
		return ns;
	}

	private ChtlNode parseImport(){
		consume(ChtlTokenType.LBRACKET); consume(ChtlTokenType.KW_IMPORT);
		consume(ChtlTokenType.RBRACKET); consume(ChtlTokenType.AT); String target = consume(ChtlTokenType.IDENT).lexeme; consume(ChtlTokenType.KW_FROM); String path=consume(ChtlTokenType.IDENT).lexeme; String alias=null; if (match(ChtlTokenType.KW_AS)) alias = consume(ChtlTokenType.IDENT).lexeme; consumeOpt(ChtlTokenType.SEMICOLON);
		return new ImportNode(previous().start, previous().end, target, path, alias);
	}

	private ElementNode parseElement(){
		int start = peek().start; String tag = consume(ChtlTokenType.IDENT).lexeme;
		consume(ChtlTokenType.LBRACE);
		state.elementStack.push(tag);
		ElementNode el = new ElementNode(start, start, tag);
		// 局部约束 e.g. except ...;
		while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
			if (check(ChtlTokenType.IDENT) && lookaheadIsAttribute()) {
				ChtlToken ident = consume(ChtlTokenType.IDENT);
				if (check(ChtlTokenType.COLON)) advance(); else consume(ChtlTokenType.EQUAL);
				String value = parseValueUntilSemicolon();
				consumeOpt(ChtlTokenType.SEMICOLON);
				el.addAttribute(new AttributeNode(ident.start, currentEnd(), ident.lexeme, value));
			} else if (matchKw(ChtlTokenType.KW_TEXT)) {
				el.addChild(parseText());
			} else if (matchKw(ChtlTokenType.KW_STYLE)) {
				el.addChild(parseStyleBlock(el));
			} else if (matchKw(ChtlTokenType.KW_SCRIPT)) {
				el.addChild(parseScriptBlock());
			} else if (match(ChtlTokenType.AT)) {
				// 模板使用：@Element Name; 或 @Style Name;（在 style{} 中也处理）
				String useKind = consume(ChtlTokenType.IDENT).lexeme;
				String useName = consume(ChtlTokenType.IDENT).lexeme;
				consumeOpt(ChtlTokenType.SEMICOLON);
				if (useKind.equals("Element")) {
					var t = state.globals.elementTemplates.get(qname(useName));
					if (t instanceof TemplateNodes.ElementTemplate et) {
						for (var child : et.body()) el.addChild(deepCopy(child));
					}
				}
			} else if (check(ChtlTokenType.IDENT)) {
				// 子元素
				el.addChild(parseElement());
			} else if (check(ChtlTokenType.KW_EXCEPT)) {
				// 解析约束但暂不强制执行
				advance(); while (!check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE) && !isAtEnd()) advance(); consumeOpt(ChtlTokenType.SEMICOLON);
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

	private boolean lookaheadIsAttribute(){
		// IDENT (:|=)
		if (!check(ChtlTokenType.IDENT)) return false;
		if (pos+1 < tokens.size()) {
			var t1 = tokens.get(pos+1).type;
			return t1==ChtlTokenType.COLON || t1==ChtlTokenType.EQUAL;
		}
		return false;
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
			if (check(ChtlTokenType.IDENT) && lookaheadIsAttribute()) {
				ChtlToken prop = consume(ChtlTokenType.IDENT);
				if (check(ChtlTokenType.COLON)) advance(); else consume(ChtlTokenType.EQUAL);
				String value = parseValueUntilSemicolon();
				consumeOpt(ChtlTokenType.SEMICOLON);
				node.addInline(new AttributeNode(prop.start, currentEnd(), prop.lexeme, value));
			} else if (match(ChtlTokenType.AT)) {
				// @Style TemplateName;
				String useKind = consume(ChtlTokenType.IDENT).lexeme;
				String useName = consume(ChtlTokenType.IDENT).lexeme; consumeOpt(ChtlTokenType.SEMICOLON);
				if (useKind.equals("Style")) {
					var t = state.globals.styleTemplates.get(qname(useName));
					if (t instanceof TemplateNodes.StyleTemplate st) {
						for (var d : st.declarations()) node.addInline(d);
					}
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
					if (isDot && owner.attributes().stream().noneMatch(a->a.name().equals("class"))) { ownerAutoClass = ownerAutoClass == null ? name.substring(1) : ownerAutoClass; }
					if (isHash && owner.attributes().stream().noneMatch(a->a.name().equals("id"))) { owner.addAttribute(new AttributeNode(previous().start, previous().end, "id", name.substring(1))); }
				}
				// 允许伪类/伪元素后缀，如 &:hover 或 ::before
				StringBuilder sel = new StringBuilder(name);
				while (check(ChtlTokenType.COLON)) {
					int colonCount = 0;
					while (check(ChtlTokenType.COLON) && colonCount < 2) { advance(); sel.append(':'); colonCount++; }
					if (check(ChtlTokenType.IDENT)) { sel.append(consume(ChtlTokenType.IDENT).lexeme); }
					if (check(ChtlTokenType.LPAREN)) {
						// 简化收集括号内容
						int lp = previous().end; int depth=0; sel.append('('); advance(); depth++;
						while (!isAtEnd() && depth>0) { if (check(ChtlTokenType.LPAREN)) { sel.append('('); depth++; advance(); } else if (check(ChtlTokenType.RPAREN)) { sel.append(')'); depth--; advance(); } else { sel.append(tokenToText(advance())); } }
					}
				}
				consume(ChtlTokenType.LBRACE);
				StyleRuleNode rule = new StyleRuleNode(previous().start, previous().end, sel.toString());
				while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
					if (check(ChtlTokenType.IDENT)) {
						ChtlToken p = consume(ChtlTokenType.IDENT);
						if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) { String val = parseValueUntilSemicolon(); consumeOpt(ChtlTokenType.SEMICOLON); rule.addDecl(new AttributeNode(p.start, currentEnd(), p.lexeme, val)); }
					} else { advance(); }
				}
				consume(ChtlTokenType.RBRACE);
				node.addRule(rule);
			} else if (check(ChtlTokenType.KW_EXCEPT)) { advance(); while (!check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE) && !isAtEnd()) advance(); consumeOpt(ChtlTokenType.SEMICOLON); }
			else { advance(); }
		}
		consume(ChtlTokenType.RBRACE);
		if (ownerAutoClass != null && owner.attributes().stream().noneMatch(a->a.name().equals("class"))) { owner.addAttribute(new AttributeNode(node.startOffset(), node.endOffset(), "class", ownerAutoClass)); }
		return new StyleBlockNode(node.startOffset(), currentEnd()) {{ for (var a : node.inlineStyles()) addInline(a); for (var r : node.globalRules()) addRule(r); }};
	}

	private ScriptBlockNode parseScriptBlock(){
		consume(ChtlTokenType.LBRACE);
		int start = previous().start; StringBuilder sb = new StringBuilder();
		while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { sb.append(tokenToText(advance())); }
		consume(ChtlTokenType.RBRACE);
		return new ScriptBlockNode(start, currentEnd(), sb.toString());
	}

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

	private String qname(String name){ return state.currentNamespace==null||state.currentNamespace.isEmpty()? name : state.currentNamespace+"."+name; }

	private ChtlNode deepCopy(ChtlNode n){
		if (n instanceof ElementNode el) {
			ElementNode c = new ElementNode(el.startOffset(), el.endOffset(), el.tagName());
			for (var a : el.attributes()) c.addAttribute(new AttributeNode(a.startOffset(), a.endOffset(), a.name(), a.value()));
			for (var ch : el.children()) c.addChild(deepCopy(ch));
			return c;
		} else if (n instanceof TextNode t) {
			return new TextNode(t.startOffset(), t.endOffset(), t.text());
		} else if (n instanceof StyleBlockNode sb) {
			StyleBlockNode d = new StyleBlockNode(sb.startOffset(), sb.endOffset());
			for (var a : sb.inlineStyles()) d.addInline(new AttributeNode(a.startOffset(), a.endOffset(), a.name(), a.value()));
			for (var r : sb.globalRules()) { StyleRuleNode rr = new StyleRuleNode(r.startOffset(), r.endOffset(), r.selector()); for (var a : r.declarations()) rr.addDecl(new AttributeNode(a.startOffset(), a.endOffset(), a.name(), a.value())); d.addRule(rr);} return d;
		} else if (n instanceof ScriptBlockNode sc) {
			return new ScriptBlockNode(sc.startOffset(), sc.endOffset(), sc.code());
		}
		return n;
	}
}