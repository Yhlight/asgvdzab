package com.chtl.ast;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class OriginImportNodes {
	public static class OriginHtml implements AstNode { public final String name; public final String code; public OriginHtml(String name, String code){this.name=name;this.code=code;} }
	public static class OriginStyle implements AstNode { public final String code; public OriginStyle(String code){this.code=code;} }
	public static class OriginJavaScript implements AstNode { public final String code; public OriginJavaScript(String code){this.code=code;} }

	public static class ImportHtml implements AstNode { public final String path; public final String asName; public ImportHtml(String path, String asName){this.path=path;this.asName=asName;} }
	public static class ImportStyle implements AstNode { public final String path; public final String asName; public ImportStyle(String path, String asName){this.path=path;this.asName=asName;} }
	public static class ImportJavaScript implements AstNode { public final String path; public final String asName; public ImportJavaScript(String path, String asName){this.path=path;this.asName=asName;} }
	public static class ImportChtl implements AstNode { public final String path; public ImportChtl(String path){this.path=path;} }
	public static class ImportCJmod implements AstNode { public final String path; public ImportCJmod(String path){this.path=path;} }

	public static class Configuration implements AstNode {
		public final Map<String, String> values; // 例如 INDEX_INITIAL_COUNT, DISABLE_NAME_GROUP
		public final NameBlock nameBlock; // 可为 null
		public Configuration(Map<String, String> values, NameBlock nameBlock) { this.values = values; this.nameBlock = nameBlock; }
	}

	public static class NameBlock implements AstNode {
		public final List<NameEntry> entries = new ArrayList<>();
	}

	public static class NameEntry implements AstNode {
		public final String key;
		public final List<String> values; // 支持组（数组）或单值
		public NameEntry(String key, List<String> values) { this.key = key; this.values = values; }
	}
}