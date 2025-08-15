package com.chtl.ast;

import java.util.ArrayList;
import java.util.List;

public class DocumentNode implements AstNode {
	private final List<AstNode> children = new ArrayList<>();
	private final List<CssRule> globalCssRules = new ArrayList<>();

	public List<AstNode> getChildren() { return children; }

	public void addChild(AstNode node) { this.children.add(node); }

	public List<CssRule> getGlobalCssRules() { return globalCssRules; }
}