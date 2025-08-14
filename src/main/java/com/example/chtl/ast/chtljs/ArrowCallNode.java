package com.example.chtl.ast.chtljs;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class ArrowCallNode implements ChtlJsNode {
	public static class Call {
		public final String method; public final String argsRaw;
		public Call(String m, String a){ this.method=m; this.argsRaw=a; }
	}
	private final int startOffset, endOffset;
	private final List<Call> chain = new ArrayList<>();

	public ArrowCallNode(int s,int e){ this.startOffset=s; this.endOffset=e; }
	public void add(Call c){ chain.add(c); }
	public List<Call> chain(){ return Collections.unmodifiableList(chain); }
	public int startOffset(){ return startOffset; }
	public int endOffset(){ return endOffset; }
}