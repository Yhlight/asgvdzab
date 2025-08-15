package com.chtl.ast;

import com.chtl.ast.TemplateNodes.StyleProperty;

import java.util.ArrayList;
import java.util.List;

public class CssRule implements AstNode {
	private final String selector;
	private final List<StyleProperty> declarations = new ArrayList<>();

	public CssRule(String selector) { this.selector = selector; }
	public String getSelector() { return selector; }
	public List<StyleProperty> getDeclarations() { return declarations; }
}