package com.example.chtl.ast.chtl;

public class OriginNodes {
	public static class OriginHtmlNode implements ChtlNode {
		private final int startOffset; private final int endOffset;
		private final String name; private final String html;
		public OriginHtmlNode(int s,int e,String name,String html){ this.startOffset=s; this.endOffset=e; this.name=name; this.html=html; }
		@Override public int startOffset(){ return startOffset; }
		@Override public int endOffset(){ return endOffset; }
		public String name(){ return name; }
		public String html(){ return html; }
	}
	public static class OriginStyleNode implements ChtlNode {
		private final int startOffset; private final int endOffset;
		private final String name; private final String css;
		public OriginStyleNode(int s,int e,String name,String css){ this.startOffset=s; this.endOffset=e; this.name=name; this.css=css; }
		@Override public int startOffset(){ return startOffset; }
		@Override public int endOffset(){ return endOffset; }
		public String name(){ return name; }
		public String css(){ return css; }
	}
	public static class OriginJavaScriptNode implements ChtlNode {
		private final int startOffset; private final int endOffset;
		private final String name; private final String js;
		public OriginJavaScriptNode(int s,int e,String name,String js){ this.startOffset=s; this.endOffset=e; this.name=name; this.js=js; }
		@Override public int startOffset(){ return startOffset; }
		@Override public int endOffset(){ return endOffset; }
		public String name(){ return name; }
		public String js(){ return js; }
	}
}