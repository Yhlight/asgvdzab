package com.example.chtl.parsers.chtl;

import com.example.chtl.ast.chtl.*;
import com.example.chtl.compilers.chtl.ChtlToken;
import com.example.chtl.compilers.chtl.ChtlTokenType;
import com.example.chtl.compilers.chtl.ChtlState;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;

public class ChtlParser {
	private final String source;
	private final List<ChtlToken> tokens;
	private int pos = 0;
	private final ChtlState state;
	private final Path baseDir;
	private final boolean strict;

	// 模板与自定义注册表（支持命名空间前缀）
	private final Map<String, TemplateNodes.StyleTemplate> styleTemplates = new HashMap<>();
	private final Map<String, TemplateNodes.ElementTemplate> elementTemplates = new HashMap<>();
	private final Map<String, TemplateNodes.VarTemplate> varTemplates = new HashMap<>();

	private final Deque<List<String>> constraintStack = new ArrayDeque<>();

	public ChtlParser(String source, List<ChtlToken> tokens){ this(source, tokens, new ChtlState(), null, false); }
	public ChtlParser(String source, List<ChtlToken> tokens, ChtlState state){ this(source, tokens, state, null, false); }
	public ChtlParser(String source, List<ChtlToken> tokens, ChtlState state, Path baseDir){ this(source, tokens, state, baseDir, false); }
	public ChtlParser(String source, List<ChtlToken> tokens, ChtlState state, Path baseDir, boolean strict){ this.source=source; this.tokens=tokens; this.state=state; this.baseDir=baseDir; this.strict=strict; }

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
		if (isForbiddenType(kw)) error("命中约束，禁止使用类型: " + kw);
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
		// 支持 [Import] [Template]/[Custom]/[Var] @Kind Name from path as Alias;
		int s = peek().start;
		if (check(ChtlTokenType.LBRACKET)) {
			consume(ChtlTokenType.LBRACKET);
			String cat = consume(ChtlTokenType.IDENT).lexeme; // Template/Custom/Var
			consume(ChtlTokenType.RBRACKET);
			consume(ChtlTokenType.AT);
			String kind = consume(ChtlTokenType.IDENT).lexeme; // Style/Element/Var
			String name = consume(ChtlTokenType.IDENT).lexeme;
			String alias = null; String pathToken = null;
			if (match(ChtlTokenType.KW_FROM)) pathToken = consume(ChtlTokenType.IDENT).lexeme;
			if (match(ChtlTokenType.KW_AS)) alias = consume(ChtlTokenType.IDENT).lexeme;
			consumeOpt(ChtlTokenType.SEMICOLON);
			if (baseDir != null && pathToken != null) {
				importOne(cat, kind, name, alias, baseDir.resolve(pathToken));
			}
			return new ImportNode(s, currentEnd(), cat+"/"+kind, pathToken, alias);
		}
		// 回退 @Chtl from path;
		if (check(ChtlTokenType.AT)) {
			advance(); String target = consume(ChtlTokenType.IDENT).lexeme; String pathToken = null; String alias = null;
			if (match(ChtlTokenType.KW_FROM)) pathToken = consume(ChtlTokenType.IDENT).lexeme;
			if (match(ChtlTokenType.KW_AS)) alias = consume(ChtlTokenType.IDENT).lexeme;
			consumeOpt(ChtlTokenType.SEMICOLON);
			if ("Chtl".equals(target) && baseDir != null && pathToken != null) {
				try {
					Path p = baseDir.resolve(pathToken);
					String src = Files.readString(p, StandardCharsets.UTF_8);
					var sub = new ChtlParser(src, new com.example.chtl.compilers.chtl.ChtlLexer(src).lex(), state, p.getParent(), strict).parseDocument();
					mergeDoc(sub, alias != null ? alias : p.getFileName().toString());
				} catch (Exception e) { if (strict) throw new RuntimeException("Import 失败: "+e.getMessage(), e); }
			}
			return new ImportNode(s, currentEnd(), target, pathToken, alias);
		}
		// 原始记录
		StringBuilder sb = new StringBuilder();
		while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) sb.append(tokenToText(advance())).append(' ');
		consumeOpt(ChtlTokenType.SEMICOLON);
		return new ImportNode(s, currentEnd(), "raw", sb.toString().trim(), null);
	}

	private void importOne(String cat, String kind, String name, String alias, Path path) {
		try {
			String src = Files.readString(path, StandardCharsets.UTF_8);
			var sub = new ChtlParser(src, new com.example.chtl.compilers.chtl.ChtlLexer(src).lex(), state, path.getParent(), strict).parseDocument();
			String fqName = alias != null ? fq(alias) : fq(name);
			if ("Template".equalsIgnoreCase(cat)) {
				if ("Style".equalsIgnoreCase(kind)) {
					TemplateNodes.StyleTemplate t = collectOneStyleTemplate(sub, name);
					putNoConflict(styleTemplates, fqName, t);
				} else if ("Element".equalsIgnoreCase(kind)) {
					TemplateNodes.ElementTemplate t = collectOneElementTemplate(sub, name);
					putNoConflict(elementTemplates, fqName, t);
				} else if ("Var".equalsIgnoreCase(kind)) {
					TemplateNodes.VarTemplate t = collectOneVarTemplate(sub, name);
					putNoConflict(varTemplates, fqName, t);
				}
			}
		} catch (Exception e) { if (strict) throw new RuntimeException("Import 精确引入失败: "+e.getMessage(), e); }
	}

	private TemplateNodes.StyleTemplate collectOneStyleTemplate(ChtlDocument doc, String name){
		for (var it : doc.items()) if (it instanceof TemplateNodes.StyleTemplate st && st.name().endsWith(name)) return st; return null;
	}
	private TemplateNodes.ElementTemplate collectOneElementTemplate(ChtlDocument doc, String name){
		for (var it : doc.items()) if (it instanceof TemplateNodes.ElementTemplate st && st.name().endsWith(name)) return st; return null;
	}
	private TemplateNodes.VarTemplate collectOneVarTemplate(ChtlDocument doc, String name){
		for (var it : doc.items()) if (it instanceof TemplateNodes.VarTemplate st && st.name().endsWith(name)) return st; return null;
	}
	private <T> void putNoConflict(Map<String,T> map, String key, T val){ if (val==null) return; if (map.containsKey(key)) { if (strict) throw new RuntimeException("命名冲突: "+key); } else { map.put(key, val); } }

	private ChtlNode parseElement(){
		int start = peek().start; String tag = consume(ChtlTokenType.IDENT).lexeme; consume(ChtlTokenType.LBRACE);
		constraintStack.push(new ArrayList<>());
		state.elementStack.push(tag);
		ElementNode el = new ElementNode(start, start, tag);
		while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
			if (check(ChtlTokenType.IDENT)) {
				ChtlToken ident = consume(ChtlTokenType.IDENT);
				if (check(ChtlTokenType.COLON) || check(ChtlTokenType.EQUAL)) { advance(); String value = parseValueUntilSemicolon(); consume(ChtlTokenType.SEMICOLON); el.addAttribute(new AttributeNode(ident.start, currentEnd(), ident.lexeme, value)); }
				else if (check(ChtlTokenType.LBRACE)) { unread(); el.addChild(parseElement()); }
				else { if (strict) error("无效的属性或子元素语法: " + ident.lexeme); else advance(); }
			} else if (matchKw(ChtlTokenType.KW_TEXT)) { el.addChild(parseText()); }
			else if (matchKw(ChtlTokenType.KW_STYLE)) { el.addChild(parseStyleBlock(el)); }
			else if (matchKw(ChtlTokenType.KW_SCRIPT)) { el.addChild(parseScriptBlock()); }
			else if (check(ChtlTokenType.KW_EXCEPT)) { advance(); ImportNode.ConstraintNode c = new ImportNode.ConstraintNode(peek().start, peek().end); while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) { if (check(ChtlTokenType.LBRACKET)) { consume(ChtlTokenType.LBRACKET); c.addTarget("[" + consume(ChtlTokenType.IDENT).lexeme + "]"); consume(ChtlTokenType.RBRACKET); } else if (check(ChtlTokenType.AT)) { advance(); c.addTarget("@" + consume(ChtlTokenType.IDENT).lexeme); } else if (check(ChtlTokenType.IDENT)) { c.addTarget(consume(ChtlTokenType.IDENT).lexeme); } if (check(ChtlTokenType.COMMA)) advance(); } consumeOpt(ChtlTokenType.SEMICOLON); el.addConstraint(c); constraintStack.peek().addAll(c.targets()); }
			else if (check(ChtlTokenType.AT)) {
				advance(); String type = consume(ChtlTokenType.IDENT).lexeme; if (isForbiddenTypeUsage(type)) error("命中约束，禁止 @"+type+" 使用");
				if (type.equals("Element")) {
					String name = consume(ChtlTokenType.IDENT).lexeme; String ns = null; if (match(ChtlTokenType.KW_FROM)) ns = consume(ChtlTokenType.IDENT).lexeme;
					TemplateNodes.ElementTemplate tpl = elementTemplates.get(ns==null? fq(name) : ns+"."+name);
					if (tpl != null) {
						List<ChtlNode> cloned = deepCloneList(tpl.body());
						Map<ChtlNode,String> origin = new IdentityHashMap<>(); markOrigin(cloned, name, origin);
						if (check(ChtlTokenType.LBRACE)) {
							consume(ChtlTokenType.LBRACE);
							applySpecialization(cloned, origin);
							consume(ChtlTokenType.RBRACE);
						} else {
							consumeOpt(ChtlTokenType.SEMICOLON);
						}
						for (ChtlNode n : cloned) el.addChild(n);
					} else { if (strict) error("未找到元素模板: "+name); else { while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) advance(); consumeOpt(ChtlTokenType.SEMICOLON);} }
				} else { while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) advance(); consumeOpt(ChtlTokenType.SEMICOLON); }
			}
			else if (check(ChtlTokenType.LBRACKET)) { ChtlNode def = parseBracketDirective(); if (def != null) el.addChild(def); }
			else { advance(); }
		}
		consume(ChtlTokenType.RBRACE);
		state.elementStack.pop(); constraintStack.pop();
		return new ElementNode(start, currentEnd(), tag) {{ for (AttributeNode a : el.attributes()) addAttribute(a); for (ChtlNode c : el.children()) addChild(c); for (ImportNode.ConstraintNode c : el.constraints()) addConstraint(c); }};
	}

	private void applySpecialization(List<ChtlNode> cloned, Map<ChtlNode,String> origin){
		while (!isAtEnd() && !check(ChtlTokenType.RBRACE)) {
			if (check(ChtlTokenType.KW_INSERT)) {
				advance();
				String mode = "after";
				if (check(ChtlTokenType.KW_AFTER)) { advance(); mode = "after"; }
				else if (check(ChtlTokenType.KW_BEFORE)) { advance(); mode = "before"; }
				else if (check(ChtlTokenType.KW_REPLACE)) { advance(); mode = "replace"; }
				else if (check(ChtlTokenType.IDENT) && peek().lexeme.equals("at")) {
					advance(); ChtlToken t = consume(ChtlTokenType.IDENT); mode = t.lexeme.equals("top")?"at_top":"at_bottom";
				}
				Integer targetIndex = null; String targetTag = null;
				if (mode.equals("after") || mode.equals("before") || mode.equals("replace")) {
					targetTag = consume(ChtlTokenType.IDENT).lexeme;
					if (match(ChtlTokenType.LBRACKET)) { String num = consume(ChtlTokenType.NUMBER).lexeme; consume(ChtlTokenType.RBRACKET); try{ targetIndex = Integer.parseInt(num.split("\\.")[0]); }catch(Exception ignored){} }
				}
				consume(ChtlTokenType.LBRACE);
				List<ChtlNode> payload = new ArrayList<>();
				while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { ChtlNode n = parseTopLevel(); if (n!=null) payload.add(n); else advance(); }
				consume(ChtlTokenType.RBRACE);
				applyInsert(cloned, mode, targetTag, targetIndex, payload);
			} else if (check(ChtlTokenType.KW_DELETE)) {
				advance();
				if (check(ChtlTokenType.AT)) { // delete @Element Name;
					advance(); String what = consume(ChtlTokenType.IDENT).lexeme; if (what.equals("Element")) { String delName = consume(ChtlTokenType.IDENT).lexeme; deleteByOrigin(cloned, delName, origin); }
					consumeOpt(ChtlTokenType.SEMICOLON);
				} else {
					String tag = consume(ChtlTokenType.IDENT).lexeme; Integer idx=null; if (match(ChtlTokenType.LBRACKET)) { String n = consume(ChtlTokenType.NUMBER).lexeme; consume(ChtlTokenType.RBRACKET); try{ idx=Integer.parseInt(n.split("\\.")[0]); }catch(Exception ignored){} }
					consumeOpt(ChtlTokenType.SEMICOLON);
					deleteByTagIndex(cloned, tag, idx);
				}
			} else if (check(ChtlTokenType.IDENT)) {
				// target element styling: tag[optionalIndex] { style{...} }
				String tag = consume(ChtlTokenType.IDENT).lexeme; Integer idx=null; if (match(ChtlTokenType.LBRACKET)) { String n = consume(ChtlTokenType.NUMBER).lexeme; consume(ChtlTokenType.RBRACKET); try{ idx=Integer.parseInt(n.split("\\.")[0]); }catch(Exception ignored){} }
				consume(ChtlTokenType.LBRACE);
				ElementNode target = findTarget(cloned, tag, idx);
				while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
					if (matchKw(ChtlTokenType.KW_STYLE)) { parseStyleBlock(target); }
					else advance();
				}
				consume(ChtlTokenType.RBRACE);
			} else { advance(); }
		}
	}

	private void applyInsert(List<ChtlNode> cloned, String mode, String tag, Integer idx, List<ChtlNode> payload){
		if (mode.equals("at_top")) { cloned.addAll(0, payload); return; }
		if (mode.equals("at_bottom")) { cloned.addAll(payload); return; }
		int pos = findIndex(cloned, tag, idx);
		if (pos<0) return; // 无目标，忽略（严格模式可改为报错）
		switch (mode) {
			case "before" -> cloned.addAll(pos, payload);
			case "after" -> cloned.addAll(pos+1, payload);
			case "replace" -> { cloned.remove(pos); cloned.addAll(pos, payload); }
		}
	}

	private void deleteByOrigin(List<ChtlNode> cloned, String tpl, Map<ChtlNode,String> origin){ cloned.removeIf(n -> tpl.equals(origin.get(n))); }
	private void deleteByTagIndex(List<ChtlNode> cloned, String tag, Integer idx){ int i=-1; for (int p=0;p<cloned.size();p++){ ChtlNode n=cloned.get(p); if (n instanceof ElementNode en && en.tagName().equals(tag)) { i++; if (idx==null || i==idx){ cloned.remove(p); return; } } } }
	private ElementNode findTarget(List<ChtlNode> cloned, String tag, Integer idx){ int i=-1; for (ChtlNode n:cloned){ if (n instanceof ElementNode en && en.tagName().equals(tag)){ i++; if (idx==null || i==idx) return en; } } return null; }
	private int findIndex(List<ChtlNode> cloned, String tag, Integer idx){ int i=-1; for (int p=0;p<cloned.size();p++){ ChtlNode n=cloned.get(p); if (n instanceof ElementNode en && en.tagName().equals(tag)){ i++; if (idx==null || i==idx) return p; } } return -1; }
	private void markOrigin(List<ChtlNode> list, String name, Map<ChtlNode,String> origin){ for (ChtlNode n:list){ origin.put(n,name); if (n instanceof ElementNode en){ markOrigin(en.children(), name, origin);} } }

	private List<ChtlNode> deepCloneList(List<ChtlNode> list){ List<ChtlNode> out=new ArrayList<>(); for (ChtlNode n:list) out.add(deepClone(n)); return out; }
	private ChtlNode deepClone(ChtlNode n){ if (n instanceof ElementNode el){ ElementNode c=new ElementNode(el.startOffset(), el.endOffset(), el.tagName()); for (AttributeNode a:el.attributes()) c.addAttribute(new AttributeNode(a.startOffset(), a.endOffset(), a.name(), a.value())); for (ChtlNode ch:el.children()) c.addChild(deepClone(ch)); return c;} else if (n instanceof TextNode t){ return new TextNode(t.startOffset(), t.endOffset(), t.text()); } else if (n instanceof StyleBlockNode sb){ StyleBlockNode d=new StyleBlockNode(sb.startOffset(), sb.endOffset()); for (StyleRuleNode r: sb.globalRules()){ StyleRuleNode rr=new StyleRuleNode(r.startOffset(), r.endOffset(), r.selector()); for (AttributeNode a:r.declarations()) rr.addDecl(new AttributeNode(a.startOffset(), a.endOffset(), a.name(), a.value())); d.addRule(rr);} return d;} else if (n instanceof ScriptBlockNode sc){ return new ScriptBlockNode(sc.startOffset(), sc.endOffset(), sc.code()); } return n; }

	private boolean isForbiddenType(String kw){ Set<String> s = getActiveConstraintTargets(); return s.contains("[Template]") && (kw.equalsIgnoreCase("Template")) || s.contains("[Custom]") && (kw.equalsIgnoreCase("Custom")); }
	private boolean isForbiddenTypeUsage(String type){ Set<String> s = getActiveConstraintTargets(); if (s.isEmpty()) return false; if (type.equalsIgnoreCase("Element") && (s.contains("[Template]") || s.contains("@Element"))) return true; return false; }
	private Set<String> getActiveConstraintTargets(){ Set<String> out = new HashSet<>(); for (var l : constraintStack) out.addAll(l); return out; }

	private void mergeDoc(ChtlDocument sub, String nsName){ ImportNode.NamespaceNode ns = new ImportNode.NamespaceNode(0,0,nsName); for (var i : sub.items()) ns.add(i); /* 直接注入到模板表，避免名称冲突 */ for (var it : sub.items()) { if (it instanceof TemplateNodes.StyleTemplate st) putNoConflict(styleTemplates, st.name(), st); if (it instanceof TemplateNodes.ElementTemplate et) putNoConflict(elementTemplates, et.name(), et); if (it instanceof TemplateNodes.VarTemplate vt) putNoConflict(varTemplates, vt.name(), vt); } }

	private void error(String msg){ if (strict) throw new RuntimeException(msg + " @pos=" + peek().start); }

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
		List<AttributeNode> inline = new ArrayList<>();
		while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
			if (check(ChtlTokenType.IDENT)) {
				ChtlToken prop = consume(ChtlTokenType.IDENT);
				if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) {
					String value = parseValueUntilSemicolon(); value = tryResolveVar(value);
					consumeOpt(ChtlTokenType.SEMICOLON);
					inline.add(new AttributeNode(prop.start, currentEnd(), prop.lexeme, value));
				}
			} else if (match(ChtlTokenType.DOT) || match(ChtlTokenType.HASH) || check(ChtlTokenType.AMP)) {
				boolean isDot = previous().type == ChtlTokenType.DOT; boolean isHash = previous().type == ChtlTokenType.HASH; boolean isAmp = check(ChtlTokenType.AMP); if (isAmp) advance();
				String name; if (isAmp) { String cls = owner.attributes().stream().filter(a->a.name().equals("class")).map(AttributeNode::value).findFirst().orElse(null); String id = owner.attributes().stream().filter(a->a.name().equals("id")).map(AttributeNode::value).findFirst().orElse(null); if (cls != null) name = "." + cls; else if (id != null) name = "#" + id; else { ownerAutoClass = state.nextAutoClass(); name = "." + ownerAutoClass; } } else { name = (isDot ? "." : "#") + consume(ChtlTokenType.IDENT).lexeme; if (isDot && owner.attributes().stream().noneMatch(a->a.name().equals("class"))) ownerAutoClass = ownerAutoClass == null ? name.substring(1) : ownerAutoClass; if (isHash && owner.attributes().stream().noneMatch(a->a.name().equals("id"))) owner.addAttribute(new AttributeNode(previous().start, previous().end, "id", name.substring(1))); }
				StringBuilder sel = new StringBuilder(name);
				while (check(ChtlTokenType.COLON)) { int colonCount = 0; while (check(ChtlTokenType.COLON) && colonCount < 2) { advance(); sel.append(':'); colonCount++; } if (check(ChtlTokenType.IDENT)) { sel.append(consume(ChtlTokenType.IDENT).lexeme); } if (check(ChtlTokenType.LPAREN)) { advance(); sel.append('('); int depth = 1; while (!isAtEnd() && depth > 0) { if (check(ChtlTokenType.LPAREN)) { sel.append('('); advance(); depth++; } else if (check(ChtlTokenType.RPAREN)) { sel.append(')'); advance(); depth--; } else { sel.append(tokenToText(advance())); } } } }
				consume(ChtlTokenType.LBRACE);
				StyleRuleNode rule = new StyleRuleNode(previous().start, previous().end, sel.toString());
				while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { if (check(ChtlTokenType.IDENT)) { ChtlToken p = consume(ChtlTokenType.IDENT); if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) { String val = parseValueUntilSemicolon(); val = tryResolveVar(val); consumeOpt(ChtlTokenType.SEMICOLON); rule.addDecl(new AttributeNode(p.start, currentEnd(), p.lexeme, val)); } } else { advance(); } }
				consume(ChtlTokenType.RBRACE); node.addRule(rule);
			} else { advance(); }
		}
		consume(ChtlTokenType.RBRACE);
		// 汇总 inline 至元素 style 属性
		if (!inline.isEmpty()) {
			StringBuilder sb = new StringBuilder();
			for (var a : inline) { sb.append(a.name()).append(": ").append(a.value()).append(";"); }
			Optional<AttributeNode> styleAttr = owner.attributes().stream().filter(a->a.name().equals("style")).findFirst();
			if (styleAttr.isPresent()) {
				String merged = styleAttr.get().value() + (styleAttr.get().value().endsWith(";")?"":";") + sb;
				owner.addAttribute(new AttributeNode(styleAttr.get().startOffset(), currentEnd(), "style", merged));
			} else {
				owner.addAttribute(new AttributeNode(node.startOffset(), currentEnd(), "style", sb.toString()));
			}
		}
		if (ownerAutoClass != null && owner.attributes().stream().noneMatch(a->a.name().equals("class"))) owner.addAttribute(new AttributeNode(node.startOffset(), node.endOffset(), "class", ownerAutoClass));
		return new StyleBlockNode(node.startOffset(), currentEnd()) {{ for (var r : node.globalRules()) addRule(r); }};
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