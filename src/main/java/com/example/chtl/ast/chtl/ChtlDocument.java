package com.example.chtl.ast.chtl;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class ChtlDocument implements ChtlNode {
	private final int startOffset;
	private final int endOffset;
	private final List<ChtlNode> items = new ArrayList<>();

	public ChtlDocument(int startOffset, int endOffset) {
		this.startOffset = startOffset;
		this.endOffset = endOffset;
	}

	@Override
	public int startOffset() { return startOffset; }

	@Override
	public int endOffset() { return endOffset; }

	public void addItem(ChtlNode node) { items.add(node); }

	public List<ChtlNode> items() { return Collections.unmodifiableList(items); }
}