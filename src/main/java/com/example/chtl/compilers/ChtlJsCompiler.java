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

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
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

		String runtime = loadRuntime("/workspace/src/main/java/com/example/chtl/compilers/runtime/DelegateRegistry.js");
		StringBuilder out = new StringBuilder();
		out.append("/* CHTL-JS-Transpiled */\n");
		if (runtime != null) out.append(runtime).append("\n");
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
				Map<String, String> map = parseSimpleObject(c.argsRaw);
				String targetExpr = map.remove("target");
				if (targetExpr == null) { sb.append("/* delegate missing target */ "); continue; }
				for (var e : map.entrySet()) {
					sb.append("(function(){ window.__CHTL_DELEGATE__.register(_el, \"").append(e.getKey()).append("\", ").append(targetExpr).append(", ").append(e.getValue()).append("); })(); ");
				}
			} else if (c.method.equals("animate")) {
				sb.append("(function(){ const opt = ").append(safeJsonLike(c.argsRaw)).append("; ");
				sb.append("if(typeof opt!==\"object\"){console.warn('animate expects object');return;} ");
				sb.append("const dur=Number(opt.duration||300); if(!isFinite(dur)||dur<=0){console.warn('animate invalid duration');return;} ");
				sb.append("const start=performance.now(); function step(ts){ const t=Math.min(1,(ts-start)/dur); if(opt.props && typeof opt.props==='object'){ for(const k in opt.props){ try{ _el.style[k]=typeof opt.props[k]==='function'? opt.props[k](t): opt.props[k]; }catch(_){} } } if(t<1){ requestAnimationFrame(step);} else if(typeof opt.callback==='function'){ opt.callback(); } } requestAnimationFrame(step); })(); ");
			} else {
				sb.append("_el.").append(c.method).append("(").append(c.argsRaw==null?"":c.argsRaw).append("); ");
			}
		}
		sb.append("});");
		return sb.toString();
	}

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
			if (i>=n || s.charAt(i)!=':') break; String key=s.substring(keyStart,i).trim(); i++;
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

	private String loadRuntime(String path){ try { return Files.readString(Path.of(path), StandardCharsets.UTF_8); } catch (IOException e) { return null; } }
}