package com.example.chtl.core.config;

import java.util.List;

public class NameGroup {
	public List<String> KEYWORD_ORIGIN = List.of("Origin");
	public String ORIGIN_STYLE = "Style";
	public String ORIGIN_JAVASCRIPT = "JavaScript";

	public String KEYWORD_STYLE = "style"; // 局部样式块
	public String KEYWORD_SCRIPT = "script"; // 局部脚本块
	public String KEYWORD_TEXT = "text";

	public static NameGroup defaults(){ return new NameGroup(); }
}