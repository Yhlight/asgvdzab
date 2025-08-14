package com.example.chtl.ast.chtl;

public class ScriptBlockNode implements ChtlNode {
	private final int startOffset;
	private final int endOffset;
	private final String code;

	public ScriptBlockNode(int startOffset, int endOffset, String code) {
		this.startOffset = startOffset;
		this.endOffset = endOffset;
		this.code = code;
	}

	@Override
	public int startOffset() { return startOffset; }

	@Override
	public int endOffset() { return endOffset; }

	public String code() { return code; }
}