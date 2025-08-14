package com.example.chtl.ast.chtljs;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class SelectorNode implements ChtlJsNode {
	public static class Part {
		public enum Kind { TAG, CLASS, ID }
		public final Kind kind; public final String name; public final Integer index;
		public Part(Kind kind, String name, Integer index){ this.kind=kind; this.name=name; this.index=index; }
	}

	private final int startOffset, endOffset;
	private final List<Part> chain = new ArrayList<>();

	public SelectorNode(int s,int e){ this.startOffset=s; this.endOffset=e; }
	public void add(Part p){ chain.add(p); }
	public List<Part> chain(){ return Collections.unmodifiableList(chain); }
	public int startOffset(){ return startOffset; }
	public int endOffset(){ return endOffset; }
}