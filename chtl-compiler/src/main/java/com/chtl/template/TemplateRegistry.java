package com.chtl.template;

import com.chtl.ast.TemplateNodes.ElementNode;
import com.chtl.ast.TemplateNodes.StyleProperty;

import java.util.*;

public class TemplateRegistry {
	public static class StyleTemplateDef {
		public final String name;
		public final String rawBody; // 原始样式体文本，保序解析
		public StyleTemplateDef(String name, String rawBody) { this.name = name; this.rawBody = rawBody; }
	}

	public static class ElementTemplateDef {
		public final String name;
		public final List<ElementNode> elements;
		public ElementTemplateDef(String name, List<ElementNode> elements) { this.name = name; this.elements = elements; }
	}

	public static class VarTemplateDef {
		public final String name;
		public final Map<String, String> variables;
		public VarTemplateDef(String name, Map<String, String> variables) { this.name = name; this.variables = variables; }
	}

	private final Map<String, StyleTemplateDef> styleTemplates = new HashMap<>();
	private final Map<String, ElementTemplateDef> elementTemplates = new HashMap<>();
	private final Map<String, VarTemplateDef> varTemplates = new HashMap<>();

	public void defineStyle(String name, String rawBody) { styleTemplates.put(name, new StyleTemplateDef(name, rawBody)); }
	public void defineElement(String name, List<ElementNode> nodes) { elementTemplates.put(name, new ElementTemplateDef(name, nodes)); }
	public void defineVar(String name, Map<String, String> vars) { varTemplates.put(name, new VarTemplateDef(name, vars)); }

	public ElementTemplateDef getElement(String name) { return elementTemplates.get(name); }
	public VarTemplateDef getVar(String name) { return varTemplates.get(name); }

	public List<StyleProperty> resolveStyleInlineProperties(String name, Set<String> visiting) {
		StyleTemplateDef def = styleTemplates.get(name);
		if (def == null) return Collections.emptyList();
		if (visiting.contains(name)) return Collections.emptyList();
		visiting.add(name);
		List<StyleProperty> props = new ArrayList<>();
		parseStyleBody(def.rawBody, props, visiting);
		visiting.remove(name);
		return props;
	}

	private void parseStyleBody(String body, List<StyleProperty> out, Set<String> visiting) {
		int p = 0, n = body.length();
		while (p < n) {
			while (p < n && Character.isWhitespace(body.charAt(p))) p++;
			if (p >= n) break;
			if (body.startsWith("inherit", p)) {
				p += "inherit".length();
				while (p < n && Character.isWhitespace(body.charAt(p))) p++;
				if (p < n && body.charAt(p) == '@') p++;
				int kStart = p; while (p < n && Character.isLetter(body.charAt(p))) p++;
				String kw = body.substring(kStart, p);
				while (p < n && Character.isWhitespace(body.charAt(p))) p++;
				int nameStart = p; while (p < n && (Character.isLetterOrDigit(body.charAt(p)) || body.charAt(p) == '_' || body.charAt(p) == '-')) p++;
				String name = body.substring(nameStart, p);
				// 结束分号
				while (p < n && body.charAt(p) != ';') p++;
				if (p < n && body.charAt(p) == ';') p++;
				if ("Style".equals(kw)) {
					out.addAll(resolveStyleInlineProperties(name, visiting));
				}
				continue;
			}
			if (p < n && body.charAt(p) == '@') {
				p++;
				int kStart = p; while (p < n && Character.isLetter(body.charAt(p))) p++;
				String kw = body.substring(kStart, p);
				while (p < n && Character.isWhitespace(body.charAt(p))) p++;
				int nameStart = p; while (p < n && (Character.isLetterOrDigit(body.charAt(p)) || body.charAt(p) == '_' || body.charAt(p) == '-')) p++;
				String name = body.substring(nameStart, p);
				while (p < n && body.charAt(p) != ';') p++;
				if (p < n && body.charAt(p) == ';') p++;
				if ("Style".equals(kw)) {
					out.addAll(resolveStyleInlineProperties(name, visiting));
				}
				continue;
			}
			// 属性
			int kStart = p; while (p < n && (Character.isLetterOrDigit(body.charAt(p)) || body.charAt(p) == '-' || body.charAt(p) == '_')) p++;
			String key = body.substring(kStart, p).trim();
			while (p < n && Character.isWhitespace(body.charAt(p))) p++;
			if (p < n && (body.charAt(p) == ':' || body.charAt(p) == '=')) p++;
			while (p < n && Character.isWhitespace(body.charAt(p))) p++;
			int vStart = p; boolean inStr = false; char q = '\0';
			while (p < n) {
				char c = body.charAt(p);
				if (!inStr && (c == ';' || c == '\n' || c == '}')) break;
				if (c == '"' || c == '\'') { if (!inStr) { inStr = true; q = c; } else if (q == c) { inStr = false; } }
				p++;
			}
			String val = body.substring(vStart, p).trim();
			if (p < n && body.charAt(p) == ';') p++;
			if (!key.isEmpty() && !val.isEmpty()) out.add(new StyleProperty(key, val));
		}
	}

	public String substituteVarCalls(String value) {
		// 形如 Name(key)
		int p = 0, n = value.length();
		StringBuilder out = new StringBuilder();
		while (p < n) {
			if (Character.isLetter(value.charAt(p))) {
				int nameStart = p; while (p < n && (Character.isLetterOrDigit(value.charAt(p)) || value.charAt(p) == '_' || value.charAt(p) == '-')) p++;
				String name = value.substring(nameStart, p);
				if (p < n && value.charAt(p) == '(') {
					p++;
					int argStart = p; while (p < n && value.charAt(p) != ')') p++;
					String arg = value.substring(argStart, Math.min(p, n)).trim();
					if (p < n && value.charAt(p) == ')') p++;
					VarTemplateDef v = varTemplates.get(name);
					if (v != null) {
						String subst = v.variables.getOrDefault(arg, "");
						out.append(subst);
						continue;
					}
					out.append(name).append('(').append(arg).append(')');
					continue;
				}
				out.append(name);
				continue;
			}
			out.append(value.charAt(p)); p++;
		}
		return out.toString();
	}
}