package com.example.chtl.ast.chtl;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class ImportNode implements ChtlNode {
	private final int startOffset, endOffset; private final String targetType; private final String path; private final String alias;
	public ImportNode(int s,int e,String t,String p,String a){ this.startOffset=s; this.endOffset=e; this.targetType=t; this.path=p; this.alias=a; }
	public String targetType(){ return targetType; }
	public String path(){ return path; }
	public String alias(){ return alias; }
	public int startOffset(){ return startOffset; }
	public int endOffset(){ return endOffset; }

	public static class NamespaceNode implements ChtlNode {
		private final int startOffset, endOffset; private final String name; private final List<ChtlNode> body=new ArrayList<>();
		public NamespaceNode(int s,int e,String n){ this.startOffset=s; this.endOffset=e; this.name=n; }
		public String name(){ return name; }
		public void add(ChtlNode n){ body.add(n); }
		public List<ChtlNode> body(){ return Collections.unmodifiableList(body); }
		public int startOffset(){ return startOffset; }
		public int endOffset(){ return endOffset; }
	}

	public static class ConstraintNode implements ChtlNode {
		private final int startOffset, endOffset; private final List<String> targets = new ArrayList<>();
		public ConstraintNode(int s,int e){ this.startOffset=s; this.endOffset=e; }
		public void addTarget(String t){ targets.add(t); }
		public List<String> targets(){ return Collections.unmodifiableList(targets); }
		public int startOffset(){ return startOffset; }
		public int endOffset(){ return endOffset; }
	}
}