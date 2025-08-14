package com.example.chtl.ast.chtl;

public class TextNode implements ChtlNode {
	private final int startOffset;
	private final int endOffset;
	private final String text;

	public TextNode(int startOffset, int endOffset, String text) {
		this.startOffset = startOffset;
		this.endOffset = endOffset;
		this.text = text;
	}

	@Override
	public int startOffset() { return startOffset; }

	@Override
	public int endOffset() { return endOffset; }

	public String text() { return text; }
}