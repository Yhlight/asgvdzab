package com.chtl.generator;

import com.chtl.ast.DocumentNode;
import com.chtl.ast.TemplateNodes.ElementNode;
import com.chtl.ast.TemplateNodes.TextNode;
import com.chtl.ast.TemplateNodes.ScriptNode;
import com.chtl.ast.AstNode;

public class CHTLGenerator {
	public String toHtml(DocumentNode doc) {
		StringBuilder html = new StringBuilder();
		for (AstNode n : doc.getChildren()) {
			if (n instanceof ElementNode) {
				html.append(genElement((ElementNode)n));
			} else if (n instanceof TextNode) {
				html.append(escape(((TextNode)n).getContent()));
			}
		}
		return html.toString();
	}

	private String genElement(ElementNode el) {
		StringBuilder s = new StringBuilder();
		s.append('<').append(el.getTagName()).append('>');
		for (TextNode t : el.getTexts()) {
			if (t.getContent().startsWith("/*style*/")) {
				// 简化：转内联样式属性
				String content = t.getContent().substring("/*style*/".length());
				// 真实实现应解析属性到 style="..."，此处保持最小实现（不扩展）
				s.append("<!-- style inline -->");
			}
		}
		for (ElementNode c : el.getChildren()) s.append(genElement(c));
		for (ScriptNode j : el.getScripts()) s.append("<script>").append(j.getCode()).append("</script>");
		s.append("</").append(el.getTagName()).append('>');
		return s.toString();
	}

	private String escape(String s) { return s; }
}