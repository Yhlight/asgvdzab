package com.chtl.generator;

import com.chtl.ast.DocumentNode;
import com.chtl.ast.TemplateNodes.ElementNode;
import com.chtl.ast.TemplateNodes.TextNode;
import com.chtl.ast.TemplateNodes.ScriptNode;
import com.chtl.ast.AstNode;
import com.chtl.lexer.State;
import com.chtl.runtime.ContextAssistant;

public class CHTLGenerator {
	private final ContextAssistant ctx = new ContextAssistant();

	public String toHtml(DocumentNode doc) {
		StringBuilder html = new StringBuilder();
		try (var g = ctx.enter(State.DEFAULT)) {
			for (AstNode n : doc.getChildren()) {
				if (n instanceof ElementNode) {
					html.append(genElement((ElementNode)n));
				} else if (n instanceof TextNode) {
					html.append(escape(((TextNode)n).getContent()));
				}
			}
		}
		return html.toString();
	}

	private String genElement(ElementNode el) {
		try (var g = ctx.enter(State.IN_ELEMENT_BLOCK)) {
			StringBuilder s = new StringBuilder();
			s.append('<').append(el.getTagName()).append('>');
			for (TextNode t : el.getTexts()) {
				if (t.getContent().startsWith("/*style*/")) {
					try (var sGuard = ctx.enter(State.IN_STYLE_BLOCK)) {
						String content = t.getContent().substring("/*style*/".length());
						s.append("<!-- style inline -->");
					}
				}
			}
			for (ElementNode c : el.getChildren()) s.append(genElement(c));
			for (ScriptNode j : el.getScripts()) {
				try (var jGuard = ctx.enter(State.IN_SCRIPT_BLOCK)) {
					s.append("<script>").append(j.getCode()).append("</script>");
				}
			}
			s.append("</").append(el.getTagName()).append('>');
			return s.toString();
		}
	}

	private String escape(String s) { return s; }
}