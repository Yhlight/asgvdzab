package com.example.chtl.ast.json;

import com.example.chtl.ast.chtl.*;
import com.example.chtl.core.config.NameGroup;

import java.util.List;
import java.util.stream.Collectors;

public class AstJsonSerializer {
	public static String toJson(ChtlDocument doc) {
		StringBuilder sb = new StringBuilder();
		sb.append("{\n  \"type\": \"Document\",\n  \"start\": ").append(doc.startOffset()).append(", \"end\": ").append(doc.endOffset()).append(",\n");
		sb.append("  \"items\": [\n");
		for (int i = 0; i < doc.items().size(); i++) {
			sb.append(indent(2)).append(serializeNode(doc.items().get(i), 2));
			if (i < doc.items().size() - 1) sb.append(',');
			sb.append('\n');
		}
		sb.append("  ]\n}");
		return sb.toString();
	}

	public static String toJson(ChtlDocument doc, NameGroup ng) {
		StringBuilder sb = new StringBuilder();
		sb.append("{\n  \"type\": \"Document\",\n  \"start\": ").append(doc.startOffset()).append(", \"end\": ").append(doc.endOffset()).append(",\n");
		sb.append("  \"nameGroup\": {");
		sb.append("\"KEYWORD_ORIGIN\": [");
		for (int i=0;i<ng.KEYWORD_ORIGIN.size();i++){ if(i>0) sb.append(','); sb.append(quote(ng.KEYWORD_ORIGIN.get(i))); }
		sb.append("], \"ORIGIN_STYLE\": ").append(quote(ng.ORIGIN_STYLE));
		sb.append(", \"ORIGIN_JAVASCRIPT\": ").append(quote(ng.ORIGIN_JAVASCRIPT));
		sb.append(", \"ORIGIN_HTML\": ").append(quote(ng.ORIGIN_HTML));
		sb.append(", \"KEYWORD_STYLE\": ").append(quote(ng.KEYWORD_STYLE));
		sb.append(", \"KEYWORD_SCRIPT\": ").append(quote(ng.KEYWORD_SCRIPT));
		sb.append(", \"KEYWORD_TEXT\": ").append(quote(ng.KEYWORD_TEXT));
		sb.append("},\n");
		sb.append("  \"items\": [\n");
		for (int i = 0; i < doc.items().size(); i++) {
			sb.append(indent(2)).append(serializeNode(doc.items().get(i), 2));
			if (i < doc.items().size() - 1) sb.append(',');
			sb.append('\n');
		}
		sb.append("  ]\n}");
		return sb.toString();
	}

	private static String serializeNode(ChtlNode n, int level) {
		if (n instanceof ElementNode el) return serializeElement(el, level);
		if (n instanceof TextNode t) return indent(level) + obj(
				"type", "Text",
				"start", String.valueOf(t.startOffset()),
				"end", String.valueOf(t.endOffset()),
				"text", escape(t.text())
		);
		if (n instanceof StyleBlockNode s) return serializeStyleBlock(s, level);
		if (n instanceof StyleRuleNode r) return serializeStyleRule(r, level);
		if (n instanceof ScriptBlockNode sc) return indent(level) + obj(
				"type", "ScriptBlock",
				"start", String.valueOf(sc.startOffset()),
				"end", String.valueOf(sc.endOffset()),
				"code", escape(sc.code())
		);
		if (n instanceof TemplateNodes.StyleTemplate st) return indent(level) + obj(
				"type", "TemplateStyle", "name", escape(st.name())
		);
		if (n instanceof TemplateNodes.ElementTemplate et) return indent(level) + obj(
				"type", "TemplateElement", "name", escape(et.name())
		);
		if (n instanceof TemplateNodes.VarTemplate vt) return indent(level) + obj(
				"type", "TemplateVar", "name", escape(vt.name())
		);
		if (n instanceof TemplateNodes.CustomStyle cs) return indent(level) + obj(
				"type", "CustomStyle", "name", escape(cs.name())
		);
		if (n instanceof TemplateNodes.CustomElement ce) return indent(level) + obj(
				"type", "CustomElement", "name", escape(ce.name())
		);
		if (n instanceof TemplateNodes.CustomVar cv) return indent(level) + obj(
				"type", "CustomVar", "name", escape(cv.name())
		);
		if (n instanceof ImportNode.NamespaceNode ns) {
			StringBuilder sb = new StringBuilder();
			sb.append(indent(level)).append("{ \"type\": \"Namespace\", \"name\": \"").append(escape(ns.name())).append("\", \"items\": [\n");
			List<ChtlNode> items = ns.body();
			for (int i=0;i<items.size();i++) {
				sb.append(serializeNode(items.get(i), level+2));
				if (i<items.size()-1) sb.append(',');
				sb.append('\n');
			}
			sb.append(indent(level)).append("] }");
			return sb.toString();
		}
		if (n instanceof ImportNode imp) return indent(level) + obj(
				"type", "Import", "target", escape(imp.targetType()), "path", escape(imp.path()), "alias", escape(imp.alias())
		);
		return indent(level) + obj("type", n.getClass().getSimpleName());
	}

	private static String serializeElement(ElementNode el, int level) {
		StringBuilder sb = new StringBuilder();
		sb.append(indent(level)).append("{ \"type\": \"Element\", \"tag\": \"").append(escape(el.tagName())).append("\",");
		sb.append(" \"attrs\": [");
		sb.append(el.attributes().stream().map(a -> "{\\\"name\\\":\\\"" + escape(a.name()) + "\\\",\\\"value\\\":\\\"" + escape(a.value()) + "\\\"}").collect(Collectors.joining(",")));
		sb.append("], \"constraints\": [");
		sb.append(el.constraints().stream().map(c -> "{\\\"targets\\\":[" + c.targets().stream().map(AstJsonSerializer::quote).collect(Collectors.joining(",")) + "]}").collect(Collectors.joining(",")));
		sb.append("], \"children\": [\n");
		for (int i=0;i<el.children().size();i++) {
			sb.append(serializeNode(el.children().get(i), level+2));
			if (i<el.children().size()-1) sb.append(',');
			sb.append('\n');
		}
		sb.append(indent(level)).append("] }");
		return sb.toString();
	}

	private static String serializeStyleBlock(StyleBlockNode s, int level) {
		StringBuilder sb = new StringBuilder();
		sb.append(indent(level)).append("{ \"type\": \"StyleBlock\", \"inline\": [");
		sb.append(s.inlineStyles().stream().map(a -> "{\\\"name\\\":\\\"" + escape(a.name()) + "\\\",\\\"value\\\":\\\"" + escape(a.value()) + "\\\"}").collect(Collectors.joining(",")));
		sb.append("], \"deleted\": [");
		sb.append(s.deletedInlineProps().stream().map(AstJsonSerializer::quote).collect(Collectors.joining(",")));
		sb.append("], \"rules\": [\n");
		for (int i=0;i<s.globalRules().size();i++) {
			sb.append(serializeStyleRule(s.globalRules().get(i), level+2));
			if (i<s.globalRules().size()-1) sb.append(',');
			sb.append('\n');
		}
		sb.append(indent(level)).append("] }");
		return sb.toString();
	}

	private static String serializeStyleRule(StyleRuleNode r, int level) {
		StringBuilder sb = new StringBuilder();
		sb.append(indent(level)).append("{ \"type\": \"StyleRule\", \"selector\": \"").append(escape(r.selector())).append("\", \"decls\": [");
		sb.append(r.declarations().stream().map(a -> "{\\\"name\\\":\\\"" + escape(a.name()) + "\\\",\\\"value\\\":\\\"" + escape(a.value()) + "\\\"}").collect(Collectors.joining(",")));
		sb.append("] }");
		return sb.toString();
	}

	private static String obj(String... kv) {
		StringBuilder sb = new StringBuilder();
		sb.append("{");
		for (int i=0;i<kv.length;i+=2) {
			if (i>0) sb.append(',');
			sb.append("\"").append(kv[i]).append("\":");
			String v = kv[i+1];
			if (v.startsWith("\"")) sb.append(v); else if (isNumeric(v)) sb.append(v); else sb.append("\"").append(v).append("\"");
		}
		sb.append("}");
		return sb.toString();
	}

	private static String quote(String s){ return "\"" + escape(s) + "\""; }
	private static boolean isNumeric(String s){ try { Double.parseDouble(s); return true; } catch (Exception e){ return false; } }
	private static String escape(String s){ if (s==null) return ""; return s.replace("\\","\\\\").replace("\"","\\\"").replace("\n","\\n"); }
	private static String indent(int n){ return "  ".repeat(n); }
}