package com.example.chtl.compilers;

import com.example.chtl.ast.chtljs.ArrowCallNode;
import com.example.chtl.ast.chtljs.ChtlJsScript;
import com.example.chtl.ast.chtljs.SelectorNode;
import com.example.chtl.compilers.chtljs.ChtlJsContext;
import com.example.chtl.compilers.chtljs.ChtlJsGlobalMap;
import com.example.chtl.compilers.chtljs.ChtlJsLexer;
import com.example.chtl.compilers.chtljs.ChtlJsState;
import com.example.chtl.parsers.chtljs.ChtlJsParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.LinkedHashMap;
import java.util.Map;

public class ChtlJsCompiler {
	private static final Logger log = LoggerFactory.getLogger(ChtlJsCompiler.class);

	public String compile(String scriptWithChtlJs) {
		var lexer = new ChtlJsLexer(scriptWithChtlJs);
		var tokens = lexer.lex();
		var parser = new ChtlJsParser(scriptWithChtlJs, tokens);
		ChtlJsScript ast = parser.parseScript();
		ChtlJsContext ctx = new ChtlJsContext(new ChtlJsGlobalMap(), new ChtlJsState());

		StringBuilder out = new StringBuilder();
		out.append("/* CHTL-JS-Transpiled */\n");
		for (var expr : ast.expressions()) {
			String dom = compileSelector(expr.selector());
			String calls = compileCalls(expr.calls());
			if (dom != null) {
				out.append("(function(){ const $sel = ").append(dom).append("; ").append(calls).append("})();\n");
			} else if (calls != null) {
				out.append(calls).append("\n");
			}
		}
		out.append("/* /CHTL-JS-Transpiled */");
		return out.toString();
	}

	private String compileSelector(SelectorNode sel) {
		if (sel == null) return null;
		StringBuilder css = new StringBuilder();
		for (var p : sel.chain()) {
			if (css.length() > 0) css.append(' ');
			switch (p.kind) {
				case TAG -> css.append(p.name);
				case CLASS -> css.append('.').append(p.name);
				case ID -> css.append('#').append(p.name);
			}
			if (p.index != null) css.append(":nth-of-type(").append(p.index + 1).append(')');
		}
		return "document.querySelectorAll(\"" + css + "\")";
	}

	private String compileCalls(ArrowCallNode calls) {
		if (calls == null) return null;
		StringBuilder sb = new StringBuilder();
		sb.append("$sel.forEach(function(_el){ ");
		for (var c : calls.chain()) {
			if (c.method.equals("listen")) {
				Map<String, String> map = parseSimpleObject(c.argsRaw);
				for (var e : map.entrySet()) {
					sb.append("_el.addEventListener(\"").append(e.getKey()).append("\", ").append(e.getValue()).append("); ");
				}
			} else if (c.method.equals("delegate")) {
				// 约定参数：{ target: selectorExpr, <event>: handler, ... }
				Map<String, String> map = parseSimpleObject(c.argsRaw);
				String targetExpr = map.remove("target");
				if (targetExpr == null) { sb.append("/* delegate missing target */ "); continue; }
				sb.append("_el.addEventListener(\"click\", function(evt){ const matches = document.querySelectorAll(")
					.append(targetExpr)
					.append("); let t = evt.target; while (t && t !== _el) { for (const m of matches) { if (m===t) { ");
				for (var e : map.entrySet()) {
					// 仅对 click/mouseenter/mouseleave 等常见事件展开；其余可扩展
					sb.append("if (evt.type===\"").append(e.getKey()).append("\") ").append(e.getValue()).append(".call(t, evt); ");
				}
				sb.append(" } } t = t.parentNode; } }); ");
			} else if (c.method.equals("animate")) {
				// 约定：{ duration, begin, when:[{at,...}], end, loop, direction, delay, callback }
				sb.append("(function(){ const opt = ").append(safeJsonLike(c.argsRaw)).append("; const start=performance.now(); const dur=opt.duration||300; function step(ts){ const t=(ts-start)/dur; /* TODO: apply CSS properties via style */ if(t<1){ requestAnimationFrame(step);} else if(typeof opt.callback==='function'){ opt.callback(); } } requestAnimationFrame(step); })(); ");
			} else {
				sb.append("_el.").append(c.method).append("(").append(c.argsRaw==null?"":c.argsRaw).append("); ");
			}
		}
		sb.append("});");
		return sb.toString();
	}

	// 解析形如 { click: handler, mouseenter: fn, target: "..." } 的简单对象（不支持嵌套复杂表达式）
	private Map<String,String> parseSimpleObject(String raw){
		Map<String,String> map = new LinkedHashMap<>();
		if (raw == null) return map;
		String s = raw.trim();
		if (s.startsWith("{")) s = s.substring(1);
		if (s.endsWith("}")) s = s.substring(0, s.length()-1);
		int i=0; int n=s.length();
		while (i<n){
			while (i<n && Character.isWhitespace(s.charAt(i))) i++;
			int keyStart=i; while (i<n && s.charAt(i)!=':' && s.charAt(i)!=',') i++;
			if (i>=n || s.charAt(i)!=':') break; String key=s.substring(keyStart,i).trim(); i++; // skip ':'
			int valStart=i; int depth=0; boolean inStr=false; char quote=0;
			while (i<n){ char c=s.charAt(i); if (inStr){ if (c=='\\') { i+=2; continue;} if (c==quote){ inStr=false; i++; continue;} i++; continue; } if (c=='\''||c=='\"'){ inStr=true; quote=c; i++; continue;} if (c=='{'||c=='('||c=='['){ depth++; i++; continue;} if (c=='}'||c==')'||c==']'){ if (depth==0) break; depth--; i++; continue;} if (c==',' && depth==0) break; i++; }
			String val=s.substring(valStart,i).trim();
			map.put(cleanKey(key), val);
			if (i<n && s.charAt(i)==',') i++;
		}
		return map;
	}

	private String cleanKey(String k){ k=k.trim(); if ((k.startsWith("\"")&&k.endsWith("\""))||(k.startsWith("'")&&k.endsWith("'"))) return k.substring(1,k.length()-1); return k; }
	private String safeJsonLike(String s){ return s==null?"{}":s; }
}