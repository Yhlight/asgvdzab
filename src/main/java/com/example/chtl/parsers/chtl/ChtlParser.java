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

	// 简易模板注册表（语义展开用）
	private final Map<String, List<AttributeNode>> styleTemplates = new HashMap<>();
	private final Map<String, List<ChtlNode>> elementTemplates = new HashMap<>();
	private final Map<String, Map<String,String>> varTemplates = new HashMap<>();

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

	private ChtlNode parseBracketDirective(){
		consume(ChtlTokenType.LBRACKET);
		if (check(ChtlTokenType.KW_TEMPLATE)) {
			advance();
			consume(ChtlTokenType.RBRACKET);
			return parseTemplateDefinition();
		} else if (check(ChtlTokenType.KW_NAMESPACE)) {
			advance();
			consume(ChtlTokenType.RBRACKET);
			return parseNamespace();
		} else if (check(ChtlTokenType.KW_IMPORT)) {
			advance();
			consume(ChtlTokenType.RBRACKET);
			return parseImport();
		} else {
			// 未知的方括号指令，跳过至 ']'
			while (!isAtEnd() && !check(ChtlTokenType.RBRACKET)) advance();
			consumeOpt(ChtlTokenType.RBRACKET);
			return null;
		}
	}

	private ChtlNode parseTemplateDefinition(){
		// @Style / @Element / @Var 名称
		consume(ChtlTokenType.AT);
		String kind = consume(ChtlTokenType.IDENT).lexeme;
		String name = consume(ChtlTokenType.IDENT).lexeme;
		consume(ChtlTokenType.LBRACE);
		if (kind.equals("Style")) {
			TemplateNodes.StyleTemplate t = new TemplateNodes.StyleTemplate(previous().start, previous().end, name);
			List<AttributeNode> decl = new ArrayList<>();
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
				if (check(ChtlTokenType.IDENT)) {
					ChtlToken p = consume(ChtlTokenType.IDENT);
					if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) {
						String val = parseValueUntilSemicolon();
						consumeOpt(ChtlTokenType.SEMICOLON);
						AttributeNode a = new AttributeNode(p.start, currentEnd(), p.lexeme, val);
						t.addDecl(a); decl.add(a);
					}
				} else advance();
			}
			consume(ChtlTokenType.RBRACE);
			styleTemplates.put(name, decl);
			return t;
		} else if (kind.equals("Element")) {
			TemplateNodes.ElementTemplate t = new TemplateNodes.ElementTemplate(previous().start, previous().end, name);
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
				ChtlNode child = parseTopLevel();
				if (child != null) { t.add(child); } else advance();
			}
			consume(ChtlTokenType.RBRACE);
			elementTemplates.put(name, t.body());
			return t;
		} else if (kind.equals("Var")) {
			TemplateNodes.VarTemplate t = new TemplateNodes.VarTemplate(previous().start, previous().end, name);
			Map<String,String> map = new HashMap<>();
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
				if (check(ChtlTokenType.IDENT)) {
					ChtlToken k = consume(ChtlTokenType.IDENT);
					if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) {
						String v = parseValueUntilSemicolon();
						consumeOpt(ChtlTokenType.SEMICOLON);
						map.put(k.lexeme, v);
						t.add(new AttributeNode(k.start, currentEnd(), k.lexeme, v));
					}
				} else advance();
			}
			consume(ChtlTokenType.RBRACE);
			varTemplates.put(name, map);
			return t;
		}
		return null;
	}

	private ChtlNode parseNamespace(){
		String name = null;
		if (check(ChtlTokenType.IDENT)) name = advance().lexeme;
		ImportNode.NamespaceNode ns = new ImportNode.NamespaceNode(previous().start, previous().end, name==null?"":name);
		if (check(ChtlTokenType.LBRACE)) {
			consume(ChtlTokenType.LBRACE);
			state.namespaceStack.push(name);
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
				ChtlNode n = parseTopLevel();
				if (n!=null) ns.add(n); else advance();
			}
			consume(ChtlTokenType.RBRACE);
			state.namespaceStack.pop();
		}
		return ns;
	}

	private ChtlNode parseImport(){
		// [Import] @Style/from 等，最小解析
		consume(ChtlTokenType.AT);
		String targetType = consume(ChtlTokenType.IDENT).lexeme; // Html/Style/JavaScript/Chtl...
		String path = null; String alias = null;
		if (check(ChtlTokenType.KW_FROM)) { advance(); path = consume(ChtlTokenType.IDENT).lexeme; }
		if (check(ChtlTokenType.KW_AS)) { advance(); alias = consume(ChtlTokenType.IDENT).lexeme; }
		return new ImportNode(previous().start, previous().end, targetType, path, alias);
	}

	private ElementNode parseElement(){
		int start = peek().start; String tag = consume(ChtlTokenType.IDENT).lexeme;
		consume(ChtlTokenType.LBRACE);
		state.elementStack.push(tag);
		ElementNode el = new ElementNode(start, start, tag);
		while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
			if (check(ChtlTokenType.IDENT)) {
				// 更稳健的属性 lookahead：IDENT 后若紧跟 ':'/'=' 则为属性
				ChtlToken ident = consume(ChtlTokenType.IDENT);
				if (check(ChtlTokenType.COLON) || check(ChtlTokenType.EQUAL)) {
					advance();
					String value = parseValueUntilSemicolon();
					consumeOpt(ChtlTokenType.SEMICOLON);
					el.addAttribute(new AttributeNode(ident.start, currentEnd(), ident.lexeme, value));
				} else {
					// 不是属性，回退并解析子元素
					unread();
					el.addChild(parseElement());
				}
			} else if (matchKw(ChtlTokenType.KW_TEXT)) {
				el.addChild(parseText());
			} else if (matchKw(ChtlTokenType.KW_STYLE)) {
				el.addChild(parseStyleBlock(el));
			} else if (matchKw(ChtlTokenType.KW_SCRIPT)) {
				el.addChild(parseScriptBlock());
			} else if (check(ChtlTokenType.AT)) {
				// @Element 模板使用
				advance();
				String kind = consume(ChtlTokenType.IDENT).lexeme;
				if (kind.equals("Element")) {
					String name = consume(ChtlTokenType.IDENT).lexeme;
					consumeOpt(ChtlTokenType.SEMICOLON);
					List<ChtlNode> body = elementTemplates.getOrDefault(name, Collections.emptyList());
					for (ChtlNode cn : body) el.addChild(cloneNode(cn));
				} else {
					// 其他 @Style 在 style{} 内处理，这里忽略
				}
			} else if (match(ChtlTokenType.KW_EXCEPT)) {
				ImportNode.ConstraintNode c = new ImportNode.ConstraintNode(previous().start, previous().end);
				// 简化：收集直到分号或换行
				while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) {
					if (check(ChtlTokenType.IDENT)) c.addTarget(advance().lexeme); else advance();
				}
				consumeOpt(ChtlTokenType.SEMICOLON);
				el.addChild(c);
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
					consumeOpt(ChtlTokenType.SEMICOLON);
					node.addInline(new AttributeNode(prop.start, currentEnd(), prop.lexeme, value));
				}
			} else if (check(ChtlTokenType.AT)) {
				// 使用样式模板：@Style Name;
				advance();
				String kind = consume(ChtlTokenType.IDENT).lexeme;
				if (kind.equals("Style")) {
					String name = consume(ChtlTokenType.IDENT).lexeme;
					consumeOpt(ChtlTokenType.SEMICOLON);
					for (AttributeNode a : styleTemplates.getOrDefault(name, Collections.emptyList())) {
						node.addInline(new AttributeNode(a.startOffset(), a.endOffset(), a.name(), a.value()));
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
					if (cls != null) name = "." + cls; else if (id != null) name = "#" + id; else {
						ownerAutoClass = state.nextAutoClass();
						name = "." + ownerAutoClass;
					}
				} else {
					name = (isDot ? "." : "#") + consume(ChtlTokenType.IDENT).lexeme;
					if (isDot && owner.attributes().stream().noneMatch(a->a.name().equals("class"))) {
						ownerAutoClass = ownerAutoClass == null ? name.substring(1) : ownerAutoClass;
					}
					if (isHash && owner.attributes().stream().noneMatch(a->a.name().equals("id"))) {
						owner.addAttribute(new AttributeNode(previous().start, previous().end, "id", name.substring(1)));
					}
				}
				consume(ChtlTokenType.LBRACE);
				StyleRuleNode rule = new StyleRuleNode(previous().start, previous().end, name);
				while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
					if (check(ChtlTokenType.IDENT)) {
						ChtlToken p = consume(ChtlTokenType.IDENT);
						if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) {
							String val = parseValueUntilSemicolon();
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
		if (ownerAutoClass != null && owner.attributes().stream().noneMatch(a->a.name().equals("class"))) {
			owner.addAttribute(new AttributeNode(node.startOffset(), node.endOffset(), "class", ownerAutoClass));
		}
		return new StyleBlockNode(node.startOffset(), currentEnd()) {{
			for (var a : node.inlineStyles()) addInline(a);
			for (var r : node.globalRules()) addRule(r);
		}};
	}

	private ScriptBlockNode parseScriptBlock(){
		consume(ChtlTokenType.LBRACE);
		int start = previous().start;
		StringBuilder sb = new StringBuilder();
		while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { sb.append(tokenToText(advance())); }
		consume(ChtlTokenType.RBRACE);
		return new ScriptBlockNode(start, currentEnd(), sb.toString());
	}

	private String parseValueUntilSemicolon(){
		StringBuilder sb = new StringBuilder();
		while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) {
			ChtlToken t = advance();
			if (t.type == ChtlTokenType.STRING || t.type == ChtlTokenType.NUMBER || t.type == ChtlTokenType.IDENT) {
				sb.append(t.lexeme);
			} else {
				sb.append(tokenToText(t));
			}
		}
		return sb.toString().trim();
	}

	private ChtlNode cloneNode(ChtlNode n){
		if (n instanceof TextNode t) return new TextNode(t.startOffset(), t.endOffset(), t.text());
		if (n instanceof ScriptBlockNode s) return new ScriptBlockNode(s.startOffset(), s.endOffset(), s.code());
		if (n instanceof StyleBlockNode sb) {
			StyleBlockNode nb = new StyleBlockNode(sb.startOffset(), sb.endOffset());
			for (var a : sb.inlineStyles()) nb.addInline(new AttributeNode(a.startOffset(), a.endOffset(), a.name(), a.value()));
			for (var r : sb.globalRules()) {
				StyleRuleNode nr = new StyleRuleNode(r.startOffset(), r.endOffset(), r.selector());
				for (var d : r.declarations()) nr.addDecl(new AttributeNode(d.startOffset(), d.endOffset(), d.name(), d.value()));
				nb.addRule(nr);
			}
			return nb;
		}
		if (n instanceof ElementNode el) {
			ElementNode ne = new ElementNode(el.startOffset(), el.endOffset(), el.tagName());
			for (var a : el.attributes()) ne.addAttribute(new AttributeNode(a.startOffset(), a.endOffset(), a.name(), a.value()));
			for (var c : el.children()) ne.addChild(cloneNode(c));
			return ne;
		}
		return n;
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