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
		if (check(ChtlTokenType.IDENT)) return parseElement();
		return null;
	}

	private ElementNode parseElement(){
		int start = peek().start; String tag = consume(ChtlTokenType.IDENT).lexeme;
		consume(ChtlTokenType.LBRACE);
		state.elementStack.push(tag);
		ElementNode el = new ElementNode(start, start, tag);
		while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
			if (check(ChtlTokenType.IDENT)) {
				ChtlToken ident = consume(ChtlTokenType.IDENT);
				if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) {
					String value = parseValueUntilSemicolon();
					consumeOpt(ChtlTokenType.SEMICOLON);
					el.addAttribute(new AttributeNode(ident.start, currentEnd(), ident.lexeme, value));
				} else {
					unread();
					el.addChild(parseElement());
				}
			} else if (matchKw(ChtlTokenType.KW_TEXT)) {
				el.addChild(parseText());
			} else if (matchKw(ChtlTokenType.KW_STYLE)) {
				el.addChild(parseStyleBlock(el));
			} else if (matchKw(ChtlTokenType.KW_SCRIPT)) {
				el.addChild(parseScriptBlock());
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