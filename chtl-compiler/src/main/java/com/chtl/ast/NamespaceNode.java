package com.chtl.ast;

import java.util.ArrayList;
import java.util.List;

public class NamespaceNode implements AstNode {
	private final String name;
	private final List<AstNode> children = new ArrayList<>();

	public NamespaceNode(String name) {
		this.name = name;
	}

	public String getName() { return name; }
	public List<AstNode> getChildren() { return children; }
	public void addChild(AstNode node) { this.children.add(node); }
}