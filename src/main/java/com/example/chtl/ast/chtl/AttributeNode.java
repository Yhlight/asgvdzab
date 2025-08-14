package com.example.chtl.ast.chtl;

public class AttributeNode implements ChtlNode {
	private final int startOffset;
	private final int endOffset;
	private final String name;
	private final String value; // 支持无引号/单双引号字面量

	public AttributeNode(int startOffset, int endOffset, String name, String value) {
		this.startOffset = startOffset;
		this.endOffset = endOffset;
		this.name = name;
		this.value = value;
	}

	@Override
	public int startOffset() { return startOffset; }

	@Override
	public int endOffset() { return endOffset; }

	public String name() { return name; }
	public String value() { return value; }
}