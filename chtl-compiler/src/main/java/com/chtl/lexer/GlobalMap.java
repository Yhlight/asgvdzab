package com.chtl.lexer;

import java.util.*;

public class GlobalMap {
	public static class Config {
		public int indexInitialCount = 0;
		public boolean disableNameGroup = true;
		public boolean debugMode = false;
		public int optionCount = 3;
	}

	private final Config config;
	private final Map<String, Set<String>> nameGroup = new HashMap<>();

	public GlobalMap() { this(new Config()); }

	public GlobalMap(Config config) {
		this.config = config;
		initializeDefaults();
	}

	private void initializeDefaults() {
		put("CUSTOM_STYLE", set("@Style", "@style", "@CSS", "@Css", "@css"));
		put("CUSTOM_ELEMENT", set("@Element"));
		put("CUSTOM_VAR", set("@Var"));
		put("TEMPLATE_STYLE", set("@Style"));
		put("TEMPLATE_ELEMENT", set("@Element"));
		put("TEMPLATE_VAR", set("@Var"));
		put("ORIGIN_HTML", set("@Html"));
		put("ORIGIN_STYLE", set("@Style"));
		put("ORIGIN_JAVASCRIPT", set("@JavaScript"));
		put("IMPORT_HTML", set("@Html"));
		put("IMPORT_STYLE", set("@Style"));
		put("IMPORT_JAVASCRIPT", set("@JavaScript"));
		put("IMPORT_CHTL", set("@Chtl"));
		put("IMPOER_CRMOD", set("@CJmod"));
		put("KEYWORD_INHERIT", set("inherit"));
		put("KEYWORD_DELETE", set("delete"));
		put("KEYWORD_INSERT", set("insert"));
		put("KEYWORD_AFTER", set("after"));
		put("KEYWORD_BEFORE", set("before"));
		put("KEYWORD_REPLACE", set("replace"));
		put("KEYWORD_ATTOP", set("at top"));
		put("KEYWORD_ATBOTTOM", set("at bottom"));
		put("KEYWORD_FROM", set("from"));
		put("KEYWORD_AS", set("as"));
		put("KEYWORD_EXCEPT", set("except"));
		put("KEYWORD_TEXT", set("text"));
		put("KEYWORD_STYLE", set("style"));
		put("KEYWORD_SCRIPT", set("script"));
		put("KEYWORD_CUSTOM", set("[Custom]"));
		put("KEYWORD_TEMPLATE", set("[Template]"));
		put("KEYWORD_ORIGIN", set("[Origin]"));
		put("KEYWORD_IMPORT", set("[Import]"));
		put("KEYWORD_NAMESPACE", set("[Namespace]"));
	}

	private void put(String key, Set<String> values) { nameGroup.put(key, values); }
	private static Set<String> set(String... v) { LinkedHashSet<String> s = new LinkedHashSet<>(); Collections.addAll(s, v); return s; }

	public Config getConfig() { return config; }
	public Map<String, Set<String>> getNameGroup() { return nameGroup; }

	public boolean matches(String groupKey, String candidate) { Set<String> set = nameGroup.get(groupKey); return set != null && set.contains(candidate); }

	public void overrideNameEntry(String key, List<String> values) {
		if (values == null) return;
		int limit = Math.max(0, config.optionCount);
		LinkedHashSet<String> set = new LinkedHashSet<>();
		int count = 0;
		for (String v : values) {
			if (limit > 0 && count >= limit) break;
			if (v != null && !v.isEmpty()) { set.add(v); count++; }
		}
		if (!set.isEmpty()) {
			nameGroup.put(key, set);
		}
	}
}