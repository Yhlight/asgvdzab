package com.example.chtl.ast.chtl;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class ElementNode implements ChtlNode {
	private final int startOffset;
	private final int endOffset;
	private final String tagName;
	private final List<AttributeNode> attributes = new ArrayList<>();
	private final List<ChtlNode> children = new ArrayList<>();
	private final List<ImportNode.ConstraintNode> constraints = new ArrayList<>();

	public ElementNode(int startOffset, int endOffset, String tagName) {
		this.startOffset = startOffset;
		this.endOffset = endOffset;
		this.tagName = tagName;
	}

	@Override
	public int startOffset() { return startOffset; }

	@Override
	public int endOffset() { return endOffset; }

	public String tagName() { return tagName; }

	public void addAttribute(AttributeNode attr) { attributes.add(attr); }
	public void addChild(ChtlNode child) { children.add(child); }
	public void addConstraint(ImportNode.ConstraintNode c){ constraints.add(c); }

	public List<AttributeNode> attributes() { return Collections.unmodifiableList(attributes); }
	public List<ChtlNode> children() { return Collections.unmodifiableList(children); }
	public List<ImportNode.ConstraintNode> constraints(){ return Collections.unmodifiableList(constraints); }
}