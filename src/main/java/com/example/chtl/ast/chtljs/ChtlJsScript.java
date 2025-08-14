package com.example.chtl.ast.chtljs;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class ChtlJsScript implements ChtlJsNode {
	private final int startOffset, endOffset;
	public static class Expr implements ChtlJsNode {
		private final int startOffset, endOffset; private final SelectorNode selector; private final ArrowCallNode calls;
		public Expr(int s,int e, SelectorNode sel, ArrowCallNode calls){ this.startOffset=s; this.endOffset=e; this.selector=sel; this.calls=calls; }
		public int startOffset(){ return startOffset; }
		public int endOffset(){ return endOffset; }
		public SelectorNode selector(){ return selector; }
		public ArrowCallNode calls(){ return calls; }
	}
	private final List<Expr> expressions = new ArrayList<>();
	public ChtlJsScript(int s,int e){ this.startOffset=s; this.endOffset=e; }
	public void add(Expr e){ expressions.add(e); }
	public List<Expr> expressions(){ return Collections.unmodifiableList(expressions); }
	public int startOffset(){ return startOffset; }
	public int endOffset(){ return endOffset; }
}