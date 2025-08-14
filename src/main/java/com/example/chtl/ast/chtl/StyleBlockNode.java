package com.example.chtl.ast.chtl;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class StyleBlockNode implements ChtlNode {
	private final int startOffset;
	private final int endOffset;
	private final List<AttributeNode> inlineStyles = new ArrayList<>();
	private final List<StyleRuleNode> globalRules = new ArrayList<>();

	public StyleBlockNode(int startOffset, int endOffset) {
		this.startOffset = startOffset;
		this.endOffset = endOffset;
	}

	@Override
	public int startOffset() { return startOffset; }

	@Override
	public int endOffset() { return endOffset; }

	public void addInline(AttributeNode attr){ inlineStyles.add(attr); }
	public void addRule(StyleRuleNode rule){ globalRules.add(rule); }

	public List<AttributeNode> inlineStyles(){ return Collections.unmodifiableList(inlineStyles); }
	public List<StyleRuleNode> globalRules(){ return Collections.unmodifiableList(globalRules); }
}