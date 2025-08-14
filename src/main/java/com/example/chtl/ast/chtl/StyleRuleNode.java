package com.example.chtl.ast.chtl;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class StyleRuleNode implements ChtlNode {
	private final int startOffset;
	private final int endOffset;
	private final String selector;
	private final List<AttributeNode> declarations = new ArrayList<>();

	public StyleRuleNode(int startOffset, int endOffset, String selector) {
		this.startOffset = startOffset;
		this.endOffset = endOffset;
		this.selector = selector;
	}

	@Override
	public int startOffset() { return startOffset; }

	@Override
	public int endOffset() { return endOffset; }

	public String selector() { return selector; }
	public void addDecl(AttributeNode decl){ declarations.add(decl); }
	public List<AttributeNode> declarations(){ return Collections.unmodifiableList(declarations); }
}