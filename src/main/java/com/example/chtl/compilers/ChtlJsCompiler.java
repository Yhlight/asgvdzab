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

public class ChtlJsCompiler {
	private static final Logger log = LoggerFactory.getLogger(ChtlJsCompiler.class);

	public String compile(String scriptWithChtlJs) {
		log.debug("编译 CHTL JS 脚本，长度={} 字符", scriptWithChtlJs.length());
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
		StringBuilder sb = new StringBuilder();
		// 简化：将链式选择器转换为 querySelectorAll 组合
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
		sb.append("document.querySelectorAll(\"").append(css).append("\")");
		return sb.toString();
	}

	private String compileCalls(ArrowCallNode calls) {
		if (calls == null) return null;
		StringBuilder sb = new StringBuilder();
		sb.append("$sel.forEach(function(_el){ ");
		for (var c : calls.chain()) {
			if (c.method.equals("listen")) {
				sb.append("/*listen*/ /*args="+escape(c.argsRaw)+"*/ ");
			} else if (c.method.equals("delegate")) {
				sb.append("/*delegate*/ /*args="+escape(c.argsRaw)+"*/ ");
			} else if (c.method.equals("animate")) {
				sb.append("/*animate*/ /*args="+escape(c.argsRaw)+"*/ ");
			} else {
				sb.append("_el.").append(c.method).append("(").append(c.argsRaw==null?"":c.argsRaw).append("); ");
			}
		}
		sb.append("});");
		return sb.toString();
	}

	private static String escape(String s){ if(s==null) return ""; return s.replace("/*","/ *").replace("*/","* /"); }
}