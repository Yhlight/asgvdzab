package com.chtl.generator;

import com.chtl.ast.DocumentNode;
import com.chtl.ast.TemplateNodes.ElementNode;
import com.chtl.ast.TemplateNodes.TextNode;
import com.chtl.ast.TemplateNodes.ScriptNode;
import com.chtl.ast.TemplateNodes.Attribute;
import com.chtl.ast.TemplateNodes.StyleProperty;
import com.chtl.ast.AstNode;
import com.chtl.ast.CssRule;
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
			if (!doc.getGlobalCssRules().isEmpty()) {
				html.append("<style>");
				for (CssRule r : doc.getGlobalCssRules()) {
					html.append(r.getSelector()).append("{");
					for (StyleProperty p : r.getDeclarations()) {
						html.append(p.getKey()).append(":").append(p.getValue()).append(";");
					}
					html.append("}");
				}
				html.append("</style>");
			}
		}
		return html.toString();
	}

	private String genElement(ElementNode el) {
		try (var g = ctx.enter(State.IN_ELEMENT_BLOCK)) {
			StringBuilder s = new StringBuilder();
			s.append('<').append(el.getTagName());
			for (Attribute a : el.getAttributes()) {
				s.append(' ').append(a.getName()).append("=\"").append(escapeAttr(a.getValue())).append("\"");
			}
			if (!el.getInlineStyles().isEmpty()) {
				StringBuilder style = new StringBuilder();
				for (StyleProperty p : el.getInlineStyles()) {
					if (style.length() > 0) style.append(' ');
					style.append(p.getKey()).append(':').append(p.getValue()).append(';');
				}
				s.append(" style=\"").append(escapeAttr(style.toString())).append("\"");
			}
			s.append('>');
			for (TextNode t : el.getTexts()) s.append(escape(t.getContent()));
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
	private String escapeAttr(String s) { return s.replace("\"", "&quot;"); }
}