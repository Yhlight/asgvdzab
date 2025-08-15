package com.chtl.generator;

import com.chtl.ast.CHTLJsNodes.*;

public class CHTLJsGenerator {
	public String toJavaScript(Chain chain) {
		StringBuilder js = new StringBuilder();
		if (chain.getSelector() != null) {
			js.append(selectorToJs(chain.getSelector()));
		}
		for (Invocation inv : chain.getInvocations()) {
			js.append('.').append(inv.getName()).append('(');
			for (int i = 0; i < inv.getArgs().size(); i++) {
				if (i > 0) js.append(',');
				js.append(inv.getArgs().get(i));
			}
			js.append(')');
		}
		return js.toString();
	}

	private String selectorToJs(Selector s) {
		String raw = s.getRaw();
		// 严格遵循文档例子：#id 优先 getElementById；其余 querySelector(All) 不做额外扩展
		raw = raw.trim();
		if (raw.startsWith("#")) {
			return "document.getElementById('" + raw.substring(1) + "')";
		}
		return "document.querySelector('" + raw + "')";
	}
}