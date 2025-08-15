package com.chtl.ast;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class TemplateNodes {
	public interface TemplateNode extends AstNode {}

	public static class TemplateStyle implements TemplateNode {
		private final String name;
		private final List<StyleProperty> properties = new ArrayList<>();
		private final List<String> inheritRefs = new ArrayList<>();

		public TemplateStyle(String name) { this.name = name; }
		public String getName() { return name; }
		public List<StyleProperty> getProperties() { return properties; }
		public List<String> getInheritRefs() { return inheritRefs; }
	}

	public static class TemplateElement implements TemplateNode {
		private final String name;
		private final List<ElementNode> elements = new ArrayList<>();
		private final List<String> inheritRefs = new ArrayList<>();

		public TemplateElement(String name) { this.name = name; }
		public String getName() { return name; }
		public List<ElementNode> getElements() { return elements; }
		public List<String> getInheritRefs() { return inheritRefs; }
	}

	public static class TemplateVar implements TemplateNode {
		private final String name;
		private final Map<String, String> variables;
		public TemplateVar(String name, Map<String, String> variables) {
			this.name = name;
			this.variables = variables;
		}
		public String getName() { return name; }
		public Map<String, String> getVariables() { return variables; }
	}

	public static class CustomStyle implements TemplateNode {
		private final String name;
		private final List<String> propertyNames = new ArrayList<>();
		private final List<StyleGroupUse> styleGroupUses = new ArrayList<>();
		public CustomStyle(String name) { this.name = name; }
		public String getName() { return name; }
		public List<String> getPropertyNames() { return propertyNames; }
		public List<StyleGroupUse> getStyleGroupUses() { return styleGroupUses; }
	}

	public static class CustomElement implements TemplateNode {
		private final String name;
		private final List<ElementNode> elements = new ArrayList<>();
		public CustomElement(String name) { this.name = name; }
		public String getName() { return name; }
		public List<ElementNode> getElements() { return elements; }
	}

	public static class CustomVar implements TemplateNode {
		private final String name;
		private final Map<String, String> variables;
		public CustomVar(String name, Map<String, String> variables) { this.name = name; this.variables = variables; }
		public String getName() { return name; }
		public Map<String, String> getVariables() { return variables; }
	}

	public static class StyleProperty {
		private final String key;
		private final String value; // 可为无修饰字面量/字符串
		public StyleProperty(String key, String value) { this.key = key; this.value = value; }
		public String getKey() { return key; }
		public String getValue() { return value; }
	}

	public static class StyleGroupUse {
		private final String name;
		private final List<StyleProperty> overrides = new ArrayList<>();
		public StyleGroupUse(String name) { this.name = name; }
		public String getName() { return name; }
		public List<StyleProperty> getOverrides() { return overrides; }
	}

	public static class ElementNode implements AstNode {
		private final String tagName; // HTML 标签
		private final List<StyleProperty> inlineStyles = new ArrayList<>();
		private final List<StyleGroupUse> styleGroupUses = new ArrayList<>();
		private final List<ElementNode> children = new ArrayList<>();
		private final List<ScriptNode> scripts = new ArrayList<>();
		private final List<TextNode> texts = new ArrayList<>();
		private final List<AstNode> specials = new ArrayList<>(); // 例如 Origin/Import/Namespace 中的节点

		public ElementNode(String tagName) { this.tagName = tagName; }
		public String getTagName() { return tagName; }
		public List<StyleProperty> getInlineStyles() { return inlineStyles; }
		public List<StyleGroupUse> getStyleGroupUses() { return styleGroupUses; }
		public List<ElementNode> getChildren() { return children; }
		public List<ScriptNode> getScripts() { return scripts; }
		public List<TextNode> getTexts() { return texts; }
		public List<AstNode> getSpecials() { return specials; }
	}

	public static class TextNode implements AstNode {
		private final String content; // 支持无修饰字面量/字符串
		public TextNode(String content) { this.content = content; }
		public String getContent() { return content; }
	}

	public static class ScriptNode implements AstNode {
		private final String code; // 原始 JS 文本或 CHTL JS 源
		public ScriptNode(String code) { this.code = code; }
		public String getCode() { return code; }
	}
}