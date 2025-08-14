package com.example.chtl.merge;

import com.example.chtl.dispatch.CompilationResult;

public class ResultMerger {
	public static String mergeToHtmlDocument(CompilationResult result) {
		StringBuilder sb = new StringBuilder();
		sb.append("<!DOCTYPE html>\n");
		sb.append("<html lang=\"zh-CN\">\n");
		sb.append("<head>\n");
		sb.append("  <meta charset=\"UTF-8\"/>\n");
		sb.append("  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>\n");
		if (!result.getGlobalCss().isBlank()) {
			sb.append("  <style>\n");
			sb.append(result.getGlobalCss());
			sb.append("  </style>\n");
		}
		sb.append("</head>\n");
		sb.append("<body>\n");
		sb.append(result.getHtmlBody()).append('\n');
		if (!result.getGlobalJs().isBlank()) {
			sb.append("  <script>\n");
			sb.append(result.getGlobalJs());
			sb.append("  </script>\n");
		}
		sb.append("</body>\n");
		sb.append("</html>\n");
		return sb.toString();
	}
}