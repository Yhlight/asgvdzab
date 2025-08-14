package com.example.chtl.parsers.chtl;

import com.example.chtl.ast.chtl.*;
import com.example.chtl.compilers.chtl.ChtlToken;
import com.example.chtl.compilers.chtl.ChtlTokenType;
import com.example.chtl.compilers.chtl.ChtlState;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

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

	// 样式模板操作序列：DECL/INCLUDE/INHERIT/DELETE
	private static class StyleOp { enum Kind { DECL, INCLUDE, INHERIT, DELETE } Kind kind; String prop; String value; String refName; List<String> deletes; StyleOp(Kind k){ this.kind=k; } }
	private final Map<String, List<StyleOp>> styleTemplateOps = new HashMap<>();

	// 元素模板继承链（child -> [parents...]，按声明顺序）
	private final Map<String, List<String>> elementTemplateInherits = new HashMap<>();

	private final Deque<List<String>> constraintStack = new ArrayDeque<>();

	// Import 防重复与循环检测
	private final Set<Path> visitedImports = new HashSet<>();
	private final Set<String> importedKeys = new HashSet<>();

	private static final int IMPORT_THREADS = Integer.getInteger("CHTL_IMPORT_THREADS", 0);
	private static final ExecutorService IMPORT_POOL = IMPORT_THREADS > 0 ? Executors.newFixedThreadPool(IMPORT_THREADS) : null;

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
		if (check(ChtlTokenType.IDENT)) {
			// 仅在后续紧跟 { 时解析为元素，否则跳过（健壮性）
			if (pos+1 < tokens.size() && tokens.get(pos+1).type == ChtlTokenType.LBRACE) return parseElement();
			advance(); return null;
		}
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
			case KW_ORIGIN -> kw = "Origin";
			default -> kw = head.lexeme;
		}
		if (isForbiddenType(kw)) error("命中约束，禁止使用类型: " + kw);
		return switch (kw) {
			case "Template", "template" -> parseTemplateDirective();
			case "Custom", "custom" -> parseCustomDirective();
			case "Namespace", "namespace" -> parseNamespaceDirective();
			case "Import", "import" -> parseImportDirective();
			case "Origin", "origin" -> { skipOriginBlock(); yield null; }
			default -> null;
		};
	}

	private void skipOriginBlock(){ if (check(ChtlTokenType.AT)) advance(); if (check(ChtlTokenType.IDENT)) advance(); if (check(ChtlTokenType.LBRACE)) { int depth=0; do { if (check(ChtlTokenType.LBRACE)) { depth++; } else if (check(ChtlTokenType.RBRACE)) { depth--; } advance(); } while (!isAtEnd() && depth>0); } }

	private ChtlNode parseTemplateDirective(){
		consume(ChtlTokenType.AT);
		String type = consume(ChtlTokenType.IDENT).lexeme; // Style/Element/Var
		String name = consume(ChtlTokenType.IDENT).lexeme;
		consume(ChtlTokenType.LBRACE);
		int bodyStart = previous().start;
		if (type.equals("Style")) {
			TemplateNodes.StyleTemplate t = new TemplateNodes.StyleTemplate(bodyStart, bodyStart, fq(name));
			List<StyleOp> ops = new ArrayList<>();
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
				if (check(ChtlTokenType.IDENT)) {
					ChtlToken p = consume(ChtlTokenType.IDENT);
					if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) {
						String v = parseValueUntilSemicolon(); consumeOpt(ChtlTokenType.SEMICOLON);
						StyleOp op = new StyleOp(StyleOp.Kind.DECL); op.prop = p.lexeme; op.value = v; ops.add(op);
						t.addDecl(new AttributeNode(p.start, currentEnd(), p.lexeme, v));
					}
				} else if (check(ChtlTokenType.AT)) {
					advance(); String atType = consume(ChtlTokenType.IDENT).lexeme;
					if (atType.equals("Style")) { String ref = consume(ChtlTokenType.IDENT).lexeme; consumeOpt(ChtlTokenType.SEMICOLON); StyleOp op = new StyleOp(StyleOp.Kind.INCLUDE); op.refName=fq(ref); ops.add(op); }
					else { while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) advance(); consumeOpt(ChtlTokenType.SEMICOLON); }
				} else if (check(ChtlTokenType.KW_INHERIT)) {
					advance(); consume(ChtlTokenType.AT); String atType = consume(ChtlTokenType.IDENT).lexeme; if (!atType.equals("Style")) { if (strict) error("inherit 仅支持 @Style"); }
					String ref = consume(ChtlTokenType.IDENT).lexeme; consumeOpt(ChtlTokenType.SEMICOLON); StyleOp op = new StyleOp(StyleOp.Kind.INHERIT); op.refName=fq(ref); ops.add(op);
				} else if (check(ChtlTokenType.KW_DELETE)) {
					advance(); List<String> dels = new ArrayList<>();
					while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) { if (check(ChtlTokenType.IDENT)) dels.add(consume(ChtlTokenType.IDENT).lexeme); if (check(ChtlTokenType.COMMA)) advance(); else break; }
					consumeOpt(ChtlTokenType.SEMICOLON); StyleOp op = new StyleOp(StyleOp.Kind.DELETE); op.deletes=dels; ops.add(op);
				} else advance();
			}
			consume(ChtlTokenType.RBRACE);
			styleTemplates.put(fq(name), t);
			styleTemplateOps.put(fq(name), ops);
			return t;
		} else if (type.equals("Element")) {
			TemplateNodes.ElementTemplate t = new TemplateNodes.ElementTemplate(bodyStart, bodyStart, fq(name));
			List<String> inherits = new ArrayList<>();
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) {
				if (check(ChtlTokenType.KW_INHERIT)) {
					advance(); consume(ChtlTokenType.AT); String atType = consume(ChtlTokenType.IDENT).lexeme;
					if (!atType.equals("Element")) { if (strict) error("inherit 仅支持 @Element"); else { skipUntil(ChtlTokenType.SEMICOLON, ChtlTokenType.RBRACE); continue; } }
					String ref = consume(ChtlTokenType.IDENT).lexeme; consumeOpt(ChtlTokenType.SEMICOLON);
					inherits.add(fq(ref));
				} else {
					ChtlNode n = parseTopLevel(); if (n != null) t.add(n); else advance();
				}
			}
			consume(ChtlTokenType.RBRACE);
			elementTemplates.put(fq(name), t);
			if (!inherits.isEmpty()) elementTemplateInherits.put(fq(name), inherits);
			return t;
		} else if (type.equals("Var")) {
			TemplateNodes.VarTemplate t = new TemplateNodes.VarTemplate(bodyStart, bodyStart, fq(name));
			while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { if (check(ChtlTokenType.IDENT)) { ChtlToken k = consume(ChtlTokenType.IDENT); if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) { String v = parseValueUntilSemicolon(); consumeOpt(ChtlTokenType.SEMICOLON); t.add(new AttributeNode(k.start, currentEnd(), k.lexeme, v)); } } else advance(); }
			consume(ChtlTokenType.RBRACE); varTemplates.put(fq(name), t); return t;
		}
		return null;
	}

	private ChtlNode parseCustomDirective(){
		consume(ChtlTokenType.AT);
		String type = consume(ChtlTokenType.IDENT).lexeme; String name = consume(ChtlTokenType.IDENT).lexeme; consume(ChtlTokenType.LBRACE);
		int bodyStart = previous().start;
		if (type.equals("Style")) { TemplateNodes.CustomStyle n = new TemplateNodes.CustomStyle(bodyStart, bodyStart, fq(name)); while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) advance(); consume(ChtlTokenType.RBRACE); return n; }
		else if (type.equals("Element")) { TemplateNodes.CustomElement n = new TemplateNodes.CustomElement(bodyStart, bodyStart, fq(name)); while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) advance(); consume(ChtlTokenType.RBRACE); return n; }
		else if (type.equals("Var")) { TemplateNodes.CustomVar n = new TemplateNodes.CustomVar(bodyStart, bodyStart, fq(name)); while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) advance(); consume(ChtlTokenType.RBRACE); return n; }
		return null;
	}

	private ChtlNode parseNamespaceDirective(){ String name = consume(ChtlTokenType.IDENT).lexeme; boolean hasBlock = check(ChtlTokenType.LBRACE); state.namespaceStack.push(name); state.currentNamespace = String.join(".", state.namespaceStack); ImportNode.NamespaceNode node = new ImportNode.NamespaceNode(peek().start, peek().end, state.currentNamespace); if (hasBlock) { consume(ChtlTokenType.LBRACE); while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { ChtlNode n = parseTopLevel(); if (n != null) node.add(n); else advance(); } consume(ChtlTokenType.RBRACE);} state.namespaceStack.pop(); state.currentNamespace = String.join(".", state.namespaceStack); return node; }

	private ChtlNode parseImportDirective(){
		int s = peek().start;
		if (!check(ChtlTokenType.AT) && !check(ChtlTokenType.LBRACKET)) {
			// 原样保留未知导入语法
			StringBuilder sb = new StringBuilder();
			while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) sb.append(tokenToText(advance()));
			consumeOpt(ChtlTokenType.SEMICOLON);
			return new ImportNode(s, currentEnd(), "raw", sb.toString().trim(), null);
		}
		if (check(ChtlTokenType.AT)) {
			advance(); String target = consume(ChtlTokenType.IDENT).lexeme; String pathToken = null; String alias = null;
			if (match(ChtlTokenType.KW_FROM)) pathToken = readPathToken();
			if (match(ChtlTokenType.KW_AS)) alias = consume(ChtlTokenType.IDENT).lexeme;
			consumeOpt(ChtlTokenType.SEMICOLON);
			if (pathToken == null) return new ImportNode(s, currentEnd(), target, null, alias);
			// @Html/@Style/@JavaScript 导入：无 as 则跳过；有 as 则创建原始嵌入节点
			if (target.equals("Html") || target.equals("Style") || target.equals("JavaScript")) {
				if (alias == null) return new ImportNode(s, currentEnd(), target, pathToken, null);
				String content = resolveSimpleAsset(target, pathToken);
				if (content == null) { if (strict) error("资源未找到: "+pathToken); return new ImportNode(s, currentEnd(), target, pathToken, alias); }
				if (target.equals("Html")) return new OriginNodes.OriginHtmlNode(s, currentEnd(), alias, content);
				if (target.equals("Style")) return new OriginNodes.OriginStyleNode(s, currentEnd(), alias, content);
				return new OriginNodes.OriginJavaScriptNode(s, currentEnd(), alias, content);
			}
			// @Chtl 导入
			if (target.equals("Chtl")) {
				List<Path> files = resolveChtlPaths(pathToken);
				if (files.size() > 1) preloadFiles(files);
				if (files.isEmpty() && strict) error("资源未找到: "+pathToken);
				for (Path f : files) importChtlFile(f, alias);
				return new ImportNode(s, currentEnd(), target, pathToken, alias);
			}
			// @CJmod 导入
			if (target.equals("CJmod")) {
				List<Path> files = resolveCjmodPaths(pathToken);
				if (files.isEmpty() && strict) error("资源未找到: "+pathToken);
				for (Path f : files) importCjmodFile(f, alias);
				return new ImportNode(s, currentEnd(), target, pathToken, alias);
			}
			return new ImportNode(s, currentEnd(), target, pathToken, alias);
		}
		// 旧式 [Import] [Template] ... 精确引入保留
		consume(ChtlTokenType.LBRACKET); String cat = consume(ChtlTokenType.IDENT).lexeme; consume(ChtlTokenType.RBRACKET); consume(ChtlTokenType.AT); String kind = consume(ChtlTokenType.IDENT).lexeme; String name = consume(ChtlTokenType.IDENT).lexeme; String alias = null; String pathToken = null; if (match(ChtlTokenType.KW_FROM)) pathToken = readPathToken(); if (match(ChtlTokenType.KW_AS)) alias = consume(ChtlTokenType.IDENT).lexeme; consumeOpt(ChtlTokenType.SEMICOLON); if (baseDir != null && pathToken != null) { importOne(cat, kind, name, alias, baseDir.resolve(pathToken)); } return new ImportNode(s, currentEnd(), cat+"/"+kind, pathToken, alias);
	}

	private String readPathToken(){ StringBuilder sb = new StringBuilder(); while (!isAtEnd() && !check(ChtlTokenType.KW_AS) && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) { sb.append(tokenToText(advance())); } return sb.toString().trim(); }

	private void importOne(String cat, String kind, String name, String alias, Path path) { try { String src = Files.readString(path, StandardCharsets.UTF_8); var sub = new ChtlParser(src, new com.example.chtl.compilers.chtl.ChtlLexer(src).lex(), state, path.getParent(), strict).parseDocument(); String fqName = alias != null ? fq(alias) : fq(name); if ("Template".equalsIgnoreCase(cat)) { if ("Style".equalsIgnoreCase(kind)) { TemplateNodes.StyleTemplate t = collectOneStyleTemplate(sub, name); putNoConflict(styleTemplates, fqName, t); } else if ("Element".equalsIgnoreCase(kind)) { TemplateNodes.ElementTemplate t = collectOneElementTemplate(sub, name); putNoConflict(elementTemplates, fqName, t); } else if ("Var".equalsIgnoreCase(kind)) { TemplateNodes.VarTemplate t = collectOneVarTemplate(sub, name); putNoConflict(varTemplates, fqName, t); } } } catch (Exception e) { if (strict) throw new RuntimeException("Import 精确引入失败: "+e.getMessage(), e); } }

	private TemplateNodes.StyleTemplate collectOneStyleTemplate(ChtlDocument doc, String name){ for (var it : doc.items()) if (it instanceof TemplateNodes.StyleTemplate st && st.name().endsWith(name)) return st; return null; }
	private TemplateNodes.ElementTemplate collectOneElementTemplate(ChtlDocument doc, String name){ for (var it : doc.items()) if (it instanceof TemplateNodes.ElementTemplate st && st.name().endsWith(name)) return st; return null; }
	private TemplateNodes.VarTemplate collectOneVarTemplate(ChtlDocument doc, String name){ for (var it : doc.items()) if (it instanceof TemplateNodes.VarTemplate st && st.name().endsWith(name)) return st; return null; }
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
					String keyName = ns==null? fq(name) : ns+"."+name;
					TemplateNodes.ElementTemplate tpl = elementTemplates.get(keyName);
					if (tpl != null) {
						LinkedHashMap<String, List<ChtlNode>> groups = resolveElementTemplateGroups(keyName, new HashSet<>());
						List<ChtlNode> clonedAll = new ArrayList<>();
						Map<ChtlNode,String> origin = new IdentityHashMap<>();
						for (var e : groups.entrySet()) {
							List<ChtlNode> cloned = deepCloneList(e.getValue());
							markOrigin(cloned, simpleName(e.getKey()), origin);
							clonedAll.addAll(cloned);
						}
						if (check(ChtlTokenType.LBRACE)) {
							consume(ChtlTokenType.LBRACE);
							applySpecialization(clonedAll, origin);
							consume(ChtlTokenType.RBRACE);
						} else {
							consumeOpt(ChtlTokenType.SEMICOLON);
						}
						for (ChtlNode n : clonedAll) el.addChild(n);
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

	private void applySpecialization(List<ChtlNode> cloned, Map<ChtlNode,String> origin){ while (!isAtEnd() && !check(ChtlTokenType.RBRACE)) { if (check(ChtlTokenType.KW_INSERT)) { advance(); String mode = "after"; if (check(ChtlTokenType.KW_AFTER)) { advance(); mode = "after"; } else if (check(ChtlTokenType.KW_BEFORE)) { advance(); mode = "before"; } else if (check(ChtlTokenType.KW_REPLACE)) { advance(); mode = "replace"; } else if (check(ChtlTokenType.IDENT) && peek().lexeme.equals("at")) { advance(); ChtlToken t = consume(ChtlTokenType.IDENT); mode = t.lexeme.equals("top")?"at_top":"at_bottom"; } Integer targetIndex = null; String targetTag = null; if (mode.equals("after") || mode.equals("before") || mode.equals("replace")) { targetTag = consume(ChtlTokenType.IDENT).lexeme; if (match(ChtlTokenType.LBRACKET)) { String num = consume(ChtlTokenType.NUMBER).lexeme; consume(ChtlTokenType.RBRACKET); try{ targetIndex = Integer.parseInt(num.split("\\.")[0]); }catch(Exception ignored){} } } consume(ChtlTokenType.LBRACE); List<ChtlNode> payload = new ArrayList<>(); while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { ChtlNode n = parseTopLevel(); if (n!=null) payload.add(n); else advance(); } consume(ChtlTokenType.RBRACE); boolean ok = applyInsert(cloned, mode, targetTag, targetIndex, payload); if (!ok && strict) error("insert 目标未找到: "+(targetTag!=null?targetTag:mode)); } else if (check(ChtlTokenType.KW_DELETE)) { advance(); if (check(ChtlTokenType.AT)) { advance(); String what = consume(ChtlTokenType.IDENT).lexeme; if (what.equals("Element")) { String delName = consume(ChtlTokenType.IDENT).lexeme; boolean removed = deleteByOrigin(cloned, delName, origin); if (!removed && strict) error("delete @Element 未命中: "+delName); } consumeOpt(ChtlTokenType.SEMICOLON); } else { String tag = consume(ChtlTokenType.IDENT).lexeme; Integer idx=null; if (match(ChtlTokenType.LBRACKET)) { String n = consume(ChtlTokenType.NUMBER).lexeme; consume(ChtlTokenType.RBRACKET); try{ idx=Integer.parseInt(n.split("\\.")[0]); }catch(Exception ignored){} } consumeOpt(ChtlTokenType.SEMICOLON); boolean removed = deleteByTagIndex(cloned, tag, idx); if (!removed && strict) error("delete 未命中: "+tag+(idx!=null?"["+idx+"]":"")); } } else if (check(ChtlTokenType.IDENT)) { String tag = consume(ChtlTokenType.IDENT).lexeme; Integer idx=null; if (match(ChtlTokenType.LBRACKET)) { String n = consume(ChtlTokenType.NUMBER).lexeme; consume(ChtlTokenType.RBRACKET); try{ idx=Integer.parseInt(n.split("\\.")[0]); }catch(Exception ignored){} } consume(ChtlTokenType.LBRACE); ElementNode target = findTarget(cloned, tag, idx); while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { if (matchKw(ChtlTokenType.KW_STYLE)) { parseStyleBlock(target); } else advance(); } consume(ChtlTokenType.RBRACE); } else { advance(); } } }

	private boolean applyInsert(List<ChtlNode> cloned, String mode, String tag, Integer idx, List<ChtlNode> payload){ if (mode.equals("at_top")) { cloned.addAll(0, payload); return true; } if (mode.equals("at_bottom")) { cloned.addAll(payload); return true; } int pos = findIndex(cloned, tag, idx); if (pos<0) return false; switch (mode) { case "before" -> cloned.addAll(pos, payload); case "after" -> cloned.addAll(pos+1, payload); case "replace" -> { cloned.remove(pos); cloned.addAll(pos, payload); } } return true; }

	private boolean deleteByOrigin(List<ChtlNode> cloned, String tpl, Map<ChtlNode,String> origin){ int before = cloned.size(); cloned.removeIf(n -> tpl.equals(origin.get(n))); return cloned.size() != before; }
	private boolean deleteByTagIndex(List<ChtlNode> cloned, String tag, Integer idx){ int i=-1; for (int p=0;p<cloned.size();p++){ ChtlNode n=cloned.get(p); if (n instanceof ElementNode en && en.tagName().equals(tag)) { i++; if (idx==null || i==idx){ cloned.remove(p); return true; } } } return false; }
	private ElementNode findTarget(List<ChtlNode> cloned, String tag, Integer idx){ int i=-1; for (ChtlNode n:cloned){ if (n instanceof ElementNode en && en.tagName().equals(tag)){ i++; if (idx==null || i==idx) return en; } } return null; }
	private int findIndex(List<ChtlNode> cloned, String tag, Integer idx){ int i=-1; for (int p=0;p<cloned.size();p++){ ChtlNode n=cloned.get(p); if (n instanceof ElementNode en && en.tagName().equals(tag)){ i++; if (idx==null || i==idx) return p; } } return -1; }
	private void markOrigin(List<ChtlNode> list, String name, Map<ChtlNode,String> origin){ for (ChtlNode n:list){ origin.put(n,name); if (n instanceof ElementNode en){ markOrigin(en.children(), name, origin);} } }
	private String simpleName(String fq){ int k=fq.lastIndexOf('.'); return k>=0? fq.substring(k+1) : fq; }

	private List<ChtlNode> deepCloneList(List<ChtlNode> list){ List<ChtlNode> out=new ArrayList<>(); for (ChtlNode n:list) out.add(deepClone(n)); return out; }
	private ChtlNode deepClone(ChtlNode n){ if (n instanceof ElementNode el){ ElementNode c=new ElementNode(el.startOffset(), el.endOffset(), el.tagName()); for (AttributeNode a:el.attributes()) c.addAttribute(new AttributeNode(a.startOffset(), a.endOffset(), a.name(), a.value())); for (ChtlNode ch:el.children()) c.addChild(deepClone(ch)); return c;} else if (n instanceof TextNode t){ return new TextNode(t.startOffset(), t.endOffset(), t.text()); } else if (n instanceof StyleBlockNode sb){ StyleBlockNode d=new StyleBlockNode(sb.startOffset(), sb.endOffset()); for (StyleRuleNode r: sb.globalRules()){ StyleRuleNode rr=new StyleRuleNode(r.startOffset(), r.endOffset(), r.selector()); for (AttributeNode a:r.declarations()) rr.addDecl(new AttributeNode(a.startOffset(), a.endOffset(), a.name(), a.value())); d.addRule(rr);} return d;} else if (n instanceof ScriptBlockNode sc){ return new ScriptBlockNode(sc.startOffset(), sc.endOffset(), sc.code()); } return n; }

	private boolean isForbiddenType(String kw){ Set<String> s = getActiveConstraintTargets(); return s.contains("[Template]") && (kw.equalsIgnoreCase("Template")) || s.contains("[Custom]") && (kw.equalsIgnoreCase("Custom")); }
	private boolean isForbiddenTypeUsage(String type){ Set<String> s = getActiveConstraintTargets(); if (s.isEmpty()) return false; if (type.equalsIgnoreCase("Element") && (s.contains("[Template]") || s.contains("@Element"))) return true; return false; }
	private Set<String> getActiveConstraintTargets(){ Set<String> out = new HashSet<>(); for (var l : constraintStack) out.addAll(l); return out; }

	private void mergeDoc(ChtlDocument sub, String nsName){ ImportNode.NamespaceNode ns = new ImportNode.NamespaceNode(0,0,nsName); for (var i : sub.items()) ns.add(i); /* 直接注入到模板表，避免名称冲突 */ for (var it : sub.items()) { if (it instanceof TemplateNodes.StyleTemplate st) putNoConflict(styleTemplates, st.name(), st); if (it instanceof TemplateNodes.ElementTemplate et) putNoConflict(elementTemplates, et.name(), et); if (it instanceof TemplateNodes.VarTemplate vt) putNoConflict(varTemplates, vt.name(), vt); } }

	private void error(String msg){ if (strict) throw new RuntimeException(msg + " @" + peek().line + ":" + peek().column); }

	private TextNode parseText(){ consume(ChtlTokenType.LBRACE); int contentStart = currentEnd(); StringBuilder sb = new StringBuilder(); while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { sb.append(tokenToText(advance())); } consume(ChtlTokenType.RBRACE); return new TextNode(contentStart, currentEnd(), sb.toString().trim()); }

	private StyleBlockNode parseStyleBlock(ElementNode owner){ consume(ChtlTokenType.LBRACE); StyleBlockNode node = new StyleBlockNode(previous().start, previous().end); String ownerAutoClass = null; List<AttributeNode> inline = new ArrayList<>(); while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { if (check(ChtlTokenType.IDENT)) { ChtlToken prop = consume(ChtlTokenType.IDENT); if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) { String value = parseValueUntilSemicolon(); value = tryResolveVar(value); consumeOpt(ChtlTokenType.SEMICOLON); inline.add(new AttributeNode(prop.start, currentEnd(), prop.lexeme, value)); } } else if (check(ChtlTokenType.AT)) { advance(); String atType = consume(ChtlTokenType.IDENT).lexeme; if (atType.equals("Style")) { String ref = consume(ChtlTokenType.IDENT).lexeme; consumeOpt(ChtlTokenType.SEMICOLON); LinkedHashMap<String,String> props = resolveStyleTemplate(fq(ref), new HashSet<>()); for (var e : props.entrySet()) inline.add(new AttributeNode(owner.startOffset(), currentEnd(), e.getKey(), e.getValue())); } else { while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) advance(); consumeOpt(ChtlTokenType.SEMICOLON); } } else if (match(ChtlTokenType.DOT) || match(ChtlTokenType.HASH) || check(ChtlTokenType.AMP)) { boolean isDot = previous().type == ChtlTokenType.DOT; boolean isHash = previous().type == ChtlTokenType.HASH; boolean isAmp = check(ChtlTokenType.AMP); if (isAmp) advance(); String name; if (isAmp) { String cls = owner.attributes().stream().filter(a->a.name().equals("class")).map(AttributeNode::value).findFirst().orElse(null); String id = owner.attributes().stream().filter(a->a.name().equals("id")).map(AttributeNode::value).findFirst().orElse(null); if (cls != null) name = "." + cls; else if (id != null) name = "#" + id; else { ownerAutoClass = state.nextAutoClass(); name = "." + ownerAutoClass; } } else { name = (isDot ? "." : "#") + consume(ChtlTokenType.IDENT).lexeme; if (isDot && owner.attributes().stream().noneMatch(a->a.name().equals("class"))) ownerAutoClass = ownerAutoClass == null ? name.substring(1) : ownerAutoClass; if (isHash && owner.attributes().stream().noneMatch(a->a.name().equals("id"))) owner.addAttribute(new AttributeNode(previous().start, previous().end, "id", name.substring(1))); } StringBuilder sel = new StringBuilder(name); while (check(ChtlTokenType.COLON)) { int colonCount = 0; while (check(ChtlTokenType.COLON) && colonCount < 2) { advance(); sel.append(':'); colonCount++; } if (check(ChtlTokenType.IDENT)) { sel.append(consume(ChtlTokenType.IDENT).lexeme); } if (check(ChtlTokenType.LPAREN)) { advance(); sel.append('('); int depth = 1; while (!isAtEnd() && depth > 0) { if (check(ChtlTokenType.LPAREN)) { sel.append('('); advance(); depth++; } else if (check(ChtlTokenType.RPAREN)) { sel.append(')'); advance(); depth--; } else { sel.append(tokenToText(advance())); } } } } consume(ChtlTokenType.LBRACE); StyleRuleNode rule = new StyleRuleNode(previous().start, previous().end, sel.toString()); while (!check(ChtlTokenType.RBRACE) && !isAtEnd()) { if (check(ChtlTokenType.IDENT)) { ChtlToken p = consume(ChtlTokenType.IDENT); if (match(ChtlTokenType.COLON) || match(ChtlTokenType.EQUAL)) { String val = parseValueUntilSemicolon(); val = tryResolveVar(val); consumeOpt(ChtlTokenType.SEMICOLON); rule.addDecl(new AttributeNode(p.start, currentEnd(), p.lexeme, val)); } } else { advance(); } } consume(ChtlTokenType.RBRACE); node.addRule(rule); } else { advance(); } } consume(ChtlTokenType.RBRACE); if (!inline.isEmpty()) { StringBuilder sb = new StringBuilder(); for (var a : inline) { sb.append(a.name()).append(": ").append(a.value()).append(";"); } Optional<AttributeNode> styleAttr = owner.attributes().stream().filter(a->a.name().equals("style")).findFirst(); if (styleAttr.isPresent()) { String merged = styleAttr.get().value() + (styleAttr.get().value().endsWith(";")?"":";") + sb; owner.addAttribute(new AttributeNode(styleAttr.get().startOffset(), currentEnd(), "style", merged)); } else { owner.addAttribute(new AttributeNode(node.startOffset(), currentEnd(), "style", sb.toString())); } } if (ownerAutoClass != null && owner.attributes().stream().noneMatch(a->a.name().equals("class"))) owner.addAttribute(new AttributeNode(node.startOffset(), node.endOffset(), "class", ownerAutoClass)); return new StyleBlockNode(node.startOffset(), currentEnd()) {{ for (var r : node.globalRules()) addRule(r); }}; }

	private LinkedHashMap<String,String> resolveStyleTemplate(String name, Set<String> visiting){ if (visiting.contains(name)) { if (strict) error("样式模板循环引用: "+name); return new LinkedHashMap<>(); } visiting.add(name); LinkedHashMap<String,String> props = new LinkedHashMap<>(); List<StyleOp> ops = styleTemplateOps.getOrDefault(name, Collections.emptyList()); for (StyleOp op : ops) { switch (op.kind) { case INCLUDE, INHERIT -> { LinkedHashMap<String,String> sub = resolveStyleTemplate(op.refName, visiting); for (var e : sub.entrySet()) props.put(e.getKey(), e.getValue()); } case DELETE -> { for (String d : op.deletes) props.remove(d); } case DECL -> props.put(op.prop, op.value); } } visiting.remove(name); return props; }

	// 解析元素模板的继承链并输出分组（保留来源名称 -> 对应节点列表），顺序为父在前、子在后
	private LinkedHashMap<String, List<ChtlNode>> resolveElementTemplateGroups(String fqName, Set<String> visiting){
		if (visiting.contains(fqName)) { if (strict) error("元素模板循环继承: "+fqName); return new LinkedHashMap<>(); }
		visiting.add(fqName);
		LinkedHashMap<String, List<ChtlNode>> out = new LinkedHashMap<>();
		List<String> parents = elementTemplateInherits.getOrDefault(fqName, Collections.emptyList());
		for (String p : parents) {
			TemplateNodes.ElementTemplate ptpl = elementTemplates.get(p);
			if (ptpl == null) { if (strict) error("继承的元素模板不存在: "+p); else continue; }
			LinkedHashMap<String, List<ChtlNode>> g = resolveElementTemplateGroups(p, visiting);
			for (var e : g.entrySet()) out.computeIfAbsent(e.getKey(), k -> new ArrayList<>()).addAll(e.getValue());
		}
		TemplateNodes.ElementTemplate self = elementTemplates.get(fqName);
		if (self != null) out.computeIfAbsent(fqName, k -> new ArrayList<>()).addAll(self.body());
		visiting.remove(fqName);
		return out;
	}

	private ScriptBlockNode parseScriptBlock(){ consume(ChtlTokenType.LBRACE); int start = previous().start; StringBuilder sb = new StringBuilder(); int depth = 1; while (!isAtEnd() && depth > 0) { ChtlToken t = advance(); if (t.type == ChtlTokenType.LBRACE) { depth++; sb.append("{"); } else if (t.type == ChtlTokenType.RBRACE) { depth--; if (depth == 0) break; sb.append("}"); } else { sb.append(tokenToText(t)); } } return new ScriptBlockNode(start, currentEnd(), sb.toString()); }
	private String tryResolveVar(String value){ String v = value.trim(); int l = v.indexOf('('); int r = v.lastIndexOf(')'); if (l>0 && r>l && !v.contains(" ")) { String name = v.substring(0,l); String key = v.substring(l+1,r).trim(); TemplateNodes.VarTemplate tpl = varTemplates.get(fq(name)); if (tpl != null) { for (AttributeNode a : tpl.kv()) if (a.name().equals(key)) return a.value(); } } return value; }

	private String fq(String name){ return state.currentNamespace==null||state.currentNamespace.isEmpty()? name : state.currentNamespace+"."+name; }
	private String parseValueUntilSemicolon(){ StringBuilder sb = new StringBuilder(); while (!isAtEnd() && !check(ChtlTokenType.SEMICOLON) && !check(ChtlTokenType.RBRACE)) { ChtlToken t = advance(); if (t.type == ChtlTokenType.STRING || t.type == ChtlTokenType.NUMBER || t.type == ChtlTokenType.IDENT) sb.append(t.lexeme); else sb.append(tokenToText(t)); } return sb.toString().trim(); }
	private String tokenToText(ChtlToken t){ return switch (t.type) { case LBRACE -> "{"; case RBRACE -> "}"; case LBRACKET -> "["; case RBRACKET -> "]"; case LPAREN -> "("; case RPAREN -> ")"; case COLON -> ":"; case EQUAL -> "="; case SEMICOLON -> ";"; case COMMA -> ","; case DOT -> "."; case HASH -> "#"; case AT -> "@"; case AMP -> "&"; default -> t.lexeme; }; }
	private boolean matchKw(ChtlTokenType t){ return match(t); }
	private boolean match(ChtlTokenType t){ if (check(t)) { advance(); return true; } return false; }
	private boolean check(ChtlTokenType t){ if (isAtEnd()) return false; return peek().type == t; }
	private ChtlToken consume(ChtlTokenType t){ if (!check(t)) { if (strict) throw new RuntimeException("期待:"+t+" 实际:"+peek().type); else return new ChtlToken(t, "", peek().start, peek().end, peek().line, peek().column); } return advance(); }
	private void consumeOpt(ChtlTokenType t){ if (check(t)) advance(); }
	private void skipUntil(ChtlTokenType... types){ outer: while (!isAtEnd()) { for (ChtlTokenType tp : types) if (check(tp)) break outer; advance(); } }
	private ChtlToken advance(){ if (!isAtEnd()) pos++; return previous(); }
	private void unread(){ if (pos>0) pos--; }
	private boolean isAtEnd(){ return peek().type==ChtlTokenType.EOF; }
	private ChtlToken peek(){ return tokens.get(pos); }
	private ChtlToken previous(){ return tokens.get(pos-1); }
	private int currentEnd(){ return previous().end; }

	// ==== Import 扩展实现 ====
	private String resolveSimpleAsset(String target, String pathToken){ if (baseDir == null) return null; // 文件名无后缀：按类型搜索
		String ext = target.equals("Html")? ".html" : target.equals("Style")? ".css" : ".js";
		Path dir = baseDir;
		if (pathToken.contains("/") || pathToken.contains("\\")) {
			Path p = baseDir.resolve(pathToken);
			if (Files.isDirectory(p)) { if (strict) error("路径是文件夹: "+pathToken); return null; }
			if (Files.isRegularFile(p)) return safeRead(p);
			return null;
		}
		// 名称，可带或不带后缀
		Path withExt = dir.resolve(ensureExt(pathToken, ext));
		if (Files.isRegularFile(withExt)) return safeRead(withExt);
		// 不深入递归，仅当前目录
		return null;
	}

	private List<Path> resolveChtlPaths(String token){
		List<Path> out = new ArrayList<>(); if (baseDir == null || token == null) return out;
		// 模块名点号映射到路径分隔：Chtholly.Space -> Chtholly/Space, Chtholly.* -> Chtholly/*
		if (!token.contains("/") && !token.contains("\\")) {
			if (token.endsWith(".*")) token = token.substring(0, token.length()-2).replace('.', '/') + "/*";
			else if (token.endsWith(".cmod") || token.endsWith(".chtl")) {
				int d = token.lastIndexOf('.'); String base = token.substring(0,d).replace('.', '/'); String ext = token.substring(d);
				token = base + ext;
			} else {
				token = token.replace('.', '/');
			}
		}
		boolean isWildcard = token.endsWith(".*") || token.endsWith("/*") || token.endsWith("*.cmod") || token.endsWith("/*.cmod") || token.endsWith("*.chtl") || token.endsWith("/*.chtl");
		boolean hasExt = token.endsWith(".cmod") || token.endsWith(".chtl");
		Path official = Path.of("module"); Path localMod = baseDir.resolve("module");
		if (token.contains("/") || token.contains("\\")) {
			Path p = baseDir.resolve(token.replace(".*","/*").replace(".*/","/*/").replace(".cmod",".cmod").replace(".chtl",".chtl"));
			if (isWildcard) out.addAll(globFiles(p, ".cmod", ".chtl"));
			else if (Files.isRegularFile(p)) out.add(p);
			else if (!hasExt) { if (strict) error("路径不含文件信息: "+token); }
			else { if (strict) error("资源未找到: "+token); }
			return out;
		}
		// 名称
		if (isWildcard) {
			out.addAll(globFiles(official.resolve(token), ".cmod", ".chtl")); if (!out.isEmpty()) return out;
			out.addAll(globFiles(localMod.resolve(token), ".cmod", ".chtl")); if (!out.isEmpty()) return out;
			out.addAll(globFiles(baseDir.resolve(token), ".cmod", ".chtl")); return out;
		}
		if (hasExt) {
			Path p = official.resolve(token); if (Files.isRegularFile(p)) { out.add(p); return out; }
			p = localMod.resolve(token); if (Files.isRegularFile(p)) { out.add(p); return out; }
			p = baseDir.resolve(token); if (Files.isRegularFile(p)) { out.add(p); return out; }
			return out;
		}
		// 无后缀：优先 cmod，再 chtl
		Path p = official.resolve(token + ".cmod"); if (Files.isRegularFile(p)) { out.add(p); return out; }
		p = official.resolve(token + ".chtl"); if (Files.isRegularFile(p)) { out.add(p); return out; }
		p = localMod.resolve(token + ".cmod"); if (Files.isRegularFile(p)) { out.add(p); return out; }
		p = localMod.resolve(token + ".chtl"); if (Files.isRegularFile(p)) { out.add(p); return out; }
		p = baseDir.resolve(token + ".cmod"); if (Files.isRegularFile(p)) { out.add(p); return out; }
		p = baseDir.resolve(token + ".chtl"); if (Files.isRegularFile(p)) { out.add(p); return out; }
		return out;
	}

	private List<Path> resolveCjmodPaths(String token){
		List<Path> out = new ArrayList<>(); if (baseDir == null || token == null) return out;
		// 模块名点号映射到路径分隔：Chtholly.Space -> Chtholly/Space, Chtholly.*（对 cjmod 无通配示例，这里不处理 *）
		if (!token.contains("/") && !token.contains("\\")) {
			if (token.endsWith(".cjmod")) {
				int d = token.lastIndexOf('.'); String base = token.substring(0,d).replace('.', '/'); String ext = token.substring(d);
				token = base + ext;
			} else {
				token = token.replace('.', '/');
			}
		}
		Path official = Path.of("module"); Path localMod = baseDir.resolve("module");
		if (token.contains("/") || token.contains("\\")) {
			Path p = baseDir.resolve(token);
			if (Files.isDirectory(p)) { if (strict) error("路径不含文件信息: "+token); return out; }
			if (Files.isRegularFile(p)) { out.add(p); return out; }
			if (strict) error("资源未找到: "+token);
			return out;
		}
		boolean hasExt = token.endsWith(".cjmod");
		if (hasExt) {
			Path p2 = official.resolve(token); if (Files.isRegularFile(p2)) { out.add(p2); return out; }
			p2 = localMod.resolve(token); if (Files.isRegularFile(p2)) { out.add(p2); return out; }
			p2 = baseDir.resolve(token); if (Files.isRegularFile(p2)) { out.add(p2); return out; }
			return out;
		}
		// 无后缀：优先官方，其次本地 module，再当前目录
		Path p3 = official.resolve(token + ".cjmod"); if (Files.isRegularFile(p3)) { out.add(p3); return out; }
		p3 = localMod.resolve(token + ".cjmod"); if (Files.isRegularFile(p3)) { out.add(p3); return out; }
		p3 = baseDir.resolve(token + ".cjmod"); if (Files.isRegularFile(p3)) { out.add(p3); return out; }
		return out;
	}

	private List<Path> globFiles(Path dirOrPattern, String... exts){
		List<Path> list = new ArrayList<>();
		try {
			java.nio.file.Path dir = dirOrPattern;
			boolean hasPattern = dirOrPattern.toString().contains("*");
			final String pat = hasPattern ? dirOrPattern.getFileName().toString() : null;
			if (hasPattern) dir = dirOrPattern.getParent();
			if (dir == null) return list;
			final String[] extsFinal = exts;
			java.nio.file.DirectoryStream.Filter<java.nio.file.Path> filter = entry -> {
				String n = entry.getFileName().toString();
				if (pat != null) {
					String regex = pat.replace(".", "\\.").replace("*", ".*");
					if (!n.matches(regex)) return false;
				}
				if (extsFinal.length==0) return true;
				for (String e : extsFinal) if (n.endsWith(e)) return true;
				return false;
			};
			try (java.nio.file.DirectoryStream<java.nio.file.Path> ds = java.nio.file.Files.newDirectoryStream(dir, filter)) {
				for (java.nio.file.Path p : ds) if (java.nio.file.Files.isRegularFile(p)) list.add(p);
			}
		} catch (Exception ignored) {}
		return list;
	}

	private String ensureExt(String name, String ext){ return name.endsWith(ext)? name : name + ext; }
	private static final Map<Path,String> FILE_CACHE = new ConcurrentHashMap<>();
	private String safeRead(Path p){ try { return FILE_CACHE.computeIfAbsent(p.toAbsolutePath().normalize(), k -> {
		try { return Files.readString(k, StandardCharsets.UTF_8); } catch (Exception e) { return null; }
	}); } catch (Exception e) { return null; } }

	private void preloadFiles(List<Path> files){
		if (IMPORT_POOL != null) {
			for (Path f : files) { IMPORT_POOL.submit(() -> { try { safeRead(f); } catch (Exception ignored) {} }); }
			IMPORT_POOL.shutdown();
			try { IMPORT_POOL.awaitTermination(10, TimeUnit.SECONDS); } catch (InterruptedException ignored) {}
		} else {
			files.parallelStream().forEach(f -> { try { safeRead(f); } catch (Exception ignored) {} });
		}
	}

	private void importChtlFile(Path file, String alias){ if (file == null) return; if (visitedImports.contains(file)) return; visitedImports.add(file); String key = file.toAbsolutePath().normalize().toString(); if (!importedKeys.add(key)) return; try { String src = safeRead(file); if (src==null) { if (strict) error("资源未找到: "+file.toString()); return; } var sub = new ChtlParser(src, new com.example.chtl.compilers.chtl.ChtlLexer(src).lex(), state, file.getParent(), strict).parseDocument(); String nsName = alias != null ? alias : (file.getParent()!=null? file.getParent().getFileName().toString() : stripExt(file.getFileName().toString())); mergeDocWithNs(sub, nsName); } catch (Exception e) { if (strict) throw new RuntimeException("Import 失败: "+file+": "+e.getMessage(), e); } }
	private void importCjmodFile(Path file, String alias){ // cjmod: 假定为纯 JS 模块，作为命名空间下的原始 JS 注入占位
		if (file == null) return; if (visitedImports.contains(file)) return; visitedImports.add(file); String key = file.toAbsolutePath().normalize().toString(); if (!importedKeys.add(key)) return; String nsName = alias != null ? alias : stripExt(file.getFileName().toString()); String js = safeRead(file); if (js == null) return; ImportNode.NamespaceNode ns = new ImportNode.NamespaceNode(0,0,nsName); ns.add(new OriginNodes.OriginJavaScriptNode(0,0, nsName, js)); }
	private String stripExt(String n){ int i=n.lastIndexOf('.'); return i>=0? n.substring(0,i) : n; }

	private void mergeDocWithNs(ChtlDocument sub, String nsName){ if (nsName == null || nsName.isBlank()) { mergeDoc(sub, ""); return; } // 同名命名空间合并
		ImportNode.NamespaceNode ns = new ImportNode.NamespaceNode(0,0,nsName);
		for (var i : sub.items()) ns.add(i);
		// 注入模板表，使用命名空间前缀，冲突检测
		for (var it : sub.items()) {
			if (it instanceof TemplateNodes.StyleTemplate st) putNoConflict(styleTemplates, nsName+"."+st.name(), st);
			if (it instanceof TemplateNodes.ElementTemplate et) putNoConflict(elementTemplates, nsName+"."+et.name(), et);
			if (it instanceof TemplateNodes.VarTemplate vt) putNoConflict(varTemplates, nsName+"."+vt.name(), vt);
		}
	}
}