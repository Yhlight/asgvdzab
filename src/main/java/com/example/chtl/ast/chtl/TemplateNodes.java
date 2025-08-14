package com.example.chtl.ast.chtl;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class TemplateNodes {
	public static class StyleTemplate implements ChtlNode {
		private final int startOffset, endOffset; private final String name; private final List<AttributeNode> declarations = new ArrayList<>();
		public StyleTemplate(int s, int e, String n){ this.startOffset=s; this.endOffset=e; this.name=n; }
		public String name(){ return name; }
		public void addDecl(AttributeNode d){ declarations.add(d); }
		public List<AttributeNode> declarations(){ return Collections.unmodifiableList(declarations); }
		public int startOffset(){ return startOffset; }
		public int endOffset(){ return endOffset; }
	}

	public static class ElementTemplate implements ChtlNode {
		private final int startOffset, endOffset; private final String name; private final List<ChtlNode> body = new ArrayList<>();
		public ElementTemplate(int s,int e,String n){ this.startOffset=s; this.endOffset=e; this.name=n; }
		public String name(){ return name; }
		public void add(ChtlNode n){ body.add(n); }
		public List<ChtlNode> body(){ return Collections.unmodifiableList(body); }
		public int startOffset(){ return startOffset; }
		public int endOffset(){ return endOffset; }
	}

	public static class VarTemplate implements ChtlNode {
		private final int startOffset, endOffset; private final String name; private final List<AttributeNode> kv = new ArrayList<>();
		public VarTemplate(int s,int e,String n){ this.startOffset=s; this.endOffset=e; this.name=n; }
		public String name(){ return name; }
		public void add(AttributeNode a){ kv.add(a); }
		public List<AttributeNode> kv(){ return Collections.unmodifiableList(kv); }
		public int startOffset(){ return startOffset; }
		public int endOffset(){ return endOffset; }
	}

	public static class CustomStyle implements ChtlNode {
		private final int startOffset, endOffset; private final String name; private final List<String> props = new ArrayList<>(); private final List<AttributeNode> defaults = new ArrayList<>();
		public CustomStyle(int s,int e,String n){ this.startOffset=s; this.endOffset=e; this.name=n; }
		public String name(){ return name; }
		public void addProp(String p){ props.add(p); }
		public void addDefault(AttributeNode d){ defaults.add(d); }
		public List<String> props(){ return Collections.unmodifiableList(props); }
		public List<AttributeNode> defaults(){ return Collections.unmodifiableList(defaults); }
		public int startOffset(){ return startOffset; }
		public int endOffset(){ return endOffset; }
	}

	public static class CustomElement implements ChtlNode {
		private final int startOffset, endOffset; private final String name; private final List<ChtlNode> body = new ArrayList<>();
		public CustomElement(int s,int e,String n){ this.startOffset=s; this.endOffset=e; this.name=n; }
		public String name(){ return name; }
		public void add(ChtlNode n){ body.add(n); }
		public List<ChtlNode> body(){ return Collections.unmodifiableList(body); }
		public int startOffset(){ return startOffset; }
		public int endOffset(){ return endOffset; }
	}

	public static class CustomVar implements ChtlNode {
		private final int startOffset, endOffset; private final String name; private final List<String> keys = new ArrayList<>();
		public CustomVar(int s,int e,String n){ this.startOffset=s; this.endOffset=e; this.name=n; }
		public String name(){ return name; }
		public void addKey(String k){ keys.add(k); }
		public List<String> keys(){ return Collections.unmodifiableList(keys); }
		public int startOffset(){ return startOffset; }
		public int endOffset(){ return endOffset; }
	}
}