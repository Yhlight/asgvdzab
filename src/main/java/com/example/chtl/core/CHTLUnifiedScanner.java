package com.example.chtl.core;

import com.example.chtl.core.config.NameGroup;
import com.example.chtl.core.scanner.ScannerConfig;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CHTLUnifiedScanner {
	private final ScannerConfig scannerConfig;
	private final Pattern originStylePattern;
	private final Pattern originJsPattern;
	private final Pattern originHtmlPattern;
	private final Pattern scriptPattern;

	public CHTLUnifiedScanner() { this(ScannerConfig.defaults()); }
	public CHTLUnifiedScanner(ScannerConfig config) {
		this.scannerConfig = config;
		NameGroup ng = config.configuration.nameGroup;
		this.originStylePattern = buildOriginBlockPattern(ng, ng.ORIGIN_STYLE);
		this.originJsPattern = buildOriginBlockPattern(ng, ng.ORIGIN_JAVASCRIPT);
		this.originHtmlPattern = buildOriginBlockPattern(ng, ng.ORIGIN_HTML);
		this.scriptPattern = buildKeywordBlockPattern(ng.KEYWORD_SCRIPT);
	}

	private static Pattern buildOriginBlockPattern(NameGroup ng, String atName) {
		String alt = String.join("|", ng.KEYWORD_ORIGIN.stream().map(CHTLUnifiedScanner::escapeRegex).toList());
		String at = escapeRegex(atName);
		String regex = "(?i)\\[(?:" + alt + ")\\]\\s*@" + at + "\\s*\\{";
		return Pattern.compile(regex);
	}

	private static Pattern buildKeywordBlockPattern(String keyword) {
		String k = escapeRegex(keyword);
		String regex = "(?i)\\b" + k + "\\s*\\{";
		return Pattern.compile(regex);
	}

	private static String escapeRegex(String s) {
		return s.replace("\\", "\\\\").replace("[", "\\[").replace("]", "\\]").replace("(", "\\(").replace(")", "\\)").replace(".", "\\.").replace("?", "\\?").replace("+", "\\+").replace("*", "\\*").replace("|", "\\|").replace("^", "\\^").replace("$", "\\$");
	}

	public ScanResult scan(String source) {
		ScanResult result = new ScanResult(source);

		// 整体作为一个CHTL片段
		result.addFragment(new Fragment(
				FragmentType.CHTL,
				source,
				0,
				source.length(),
				"CHTL-Source"
		));

		// 提取 [Origin] @Style
		for (Block b : findBlocks(source, originStylePattern)) {
			String css = b.innerContent(source);
			result.addFragment(new Fragment(FragmentType.CSS, css, b.start, b.end, "Origin-Style"));
		}

		// 提取 [Origin] @JavaScript
		for (Block b : findBlocks(source, originJsPattern)) {
			String js = b.innerContent(source);
			result.addFragment(new Fragment(FragmentType.JS, js, b.start, b.end, "Origin-JS"));
		}

		// 提取 [Origin] @Html
		for (Block b : findBlocks(source, originHtmlPattern)) {
			String html = b.innerContent(source);
			result.addFragment(new Fragment(FragmentType.HTML, html, b.start, b.end, "Origin-Html"));
		}

		// 提取 script{} 并进行字符级别宽判/严判切割
		for (Block b : findBlocks(source, scriptPattern)) {
			String scriptCode = b.innerContent(source);
			List<Range> chtlJsRanges = detectChtlJsRanges(scriptCode);
			if (chtlJsRanges.isEmpty()) {
				result.addFragment(new Fragment(FragmentType.JS, scriptCode, b.innerStart, b.innerEnd, "Local-Script"));
			} else {
				int cursor = 0;
				for (Range r : chtlJsRanges) {
					if (r.start > cursor) {
						String jsPart = scriptCode.substring(cursor, r.start);
						if (!jsPart.isBlank()) {
							result.addFragment(new Fragment(FragmentType.JS, jsPart, b.innerStart + cursor, b.innerStart + r.start, "Local-Script"));
						}
					}
					String chtlJsPart = scriptCode.substring(r.start, r.end);
					result.addFragment(new Fragment(FragmentType.CHTL_JS, chtlJsPart, b.innerStart + r.start, b.innerStart + r.end, "Local-Script"));
					cursor = r.end;
				}
				if (cursor < scriptCode.length()) {
					String tail = scriptCode.substring(cursor);
					if (!tail.isBlank()) {
						result.addFragment(new Fragment(FragmentType.JS, tail, b.innerStart + cursor, b.innerEnd, "Local-Script"));
					}
				}
			}
		}

		return result;
	}

	private List<Block> findBlocks(String source, Pattern beginPattern) {
		List<Block> blocks = new ArrayList<>();
		Matcher m = beginPattern.matcher(source);
		while (m.find()) {
			int openBraceIndex = source.indexOf('{', m.end() - 1);
			if (openBraceIndex < 0) continue;
			int closeBraceIndex = findMatchingBrace(source, openBraceIndex);
			if (closeBraceIndex < 0) continue;
			blocks.add(new Block(m.start(), closeBraceIndex + 1, openBraceIndex + 1, closeBraceIndex));
		}
		return blocks;
	}

	private int findMatchingBrace(String s, int openIndex) {
		int depth = 0;
		for (int i = openIndex; i < s.length(); i++) {
			char c = s.charAt(i);
			if (c == '{') depth++;
			else if (c == '}') {
				depth--;
				if (depth == 0) return i;
			}
		}
		return -1;
	}

	// 检测 script{} 中的 CHTL JS 片段：{{...}} 以及后续的 ->method 链
	private List<Range> detectChtlJsRanges(String s) {
		List<Range> ranges = new ArrayList<>();
		int i = 0;
		while (i < s.length()) {
			char c = s.charAt(i);
			// 跳过字符串与注释，避免误判
			if (c == '\'' || c == '"' || c == '`') {
				int endStr = skipString(s, i);
				if (endStr < 0) break; else { i = endStr; continue; }
			}
			if (c == '/' && i + 1 < s.length()) {
				char n = s.charAt(i + 1);
				if (n == '/') { // 单行注释
					int nl = s.indexOf('\n', i + 2);
					i = (nl < 0 ? s.length() : nl + 1);
					continue;
				} else if (n == '*') { // 多行注释
					int end = s.indexOf("*/", i + 2);
					i = (end < 0 ? s.length() : end + 2);
					continue;
				}
			}

			// {{...}} 开始
			if (i + 1 < s.length() && c == '{' && s.charAt(i + 1) == '{') {
				int start = i;
				int endSel = s.indexOf("}}", i + 2);
				if (endSel < 0) {
					// 不完整，视为普通 JS
					i += 2;
					continue;
				}
				int end = endSel + 2;
				// 严判：合并后续的 ->method 链
				int j = end;
				while (true) {
					j = skipSpaces(s, j);
					if (j + 1 < s.length() && s.charAt(j) == '-' && s.charAt(j + 1) == '>') {
						int k = j + 2;
						k = skipSpaces(s, k);
						int idStart = k;
						while (k < s.length() && (Character.isLetterOrDigit(s.charAt(k)) || s.charAt(k) == '_' )) k++;
						if (k == idStart) break; // 没有方法名，停止
						end = k;
						j = k;
						continue;
					} else {
						break;
					}
				}
				ranges.add(new Range(start, end));
				i = end;
				continue;
			}

			// 独立出现的 ->method 也认为是 CHTL JS 片段（宽容处理）
			if (i + 1 < s.length() && c == '-' && s.charAt(i + 1) == '>') {
				int start = i;
				int k = i + 2;
				k = skipSpaces(s, k);
				int idStart = k;
				while (k < s.length() && (Character.isLetterOrDigit(s.charAt(k)) || s.charAt(k) == '_' )) k++;
				if (k > idStart) {
					int end = k;
					ranges.add(new Range(start, end));
					i = end;
					continue;
				}
			}

			i++;
		}
		return mergeAdjacent(ranges);
	}

	private int skipSpaces(String s, int i) {
		while (i < s.length()) {
			char c = s.charAt(i);
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') i++; else break;
		}
		return i;
	}

	private int skipString(String s, int startQuote) {
		char quote = s.charAt(startQuote);
		int i = startQuote + 1;
		while (i < s.length()) {
			char c = s.charAt(i);
			if (c == '\\') { // 转义
				i += 2; continue;
			}
			if (c == quote) return i + 1;
			// 模板字符串中的插值 `${...}` 要正确跳过
			if (quote == '`' && c == '$' && i + 1 < s.length() && s.charAt(i + 1) == '{') {
				int close = findMatchingBrace(s, i + 1);
				i = (close < 0 ? s.length() : close + 1);
				continue;
			}
			i++;
		}
		return -1;
	}

	private List<Range> mergeAdjacent(List<Range> ranges) {
		if (ranges.isEmpty()) return ranges;
		ranges.sort((a,b) -> Integer.compare(a.start, b.start));
		List<Range> out = new ArrayList<>();
		Range cur = ranges.get(0);
		for (int idx = 1; idx < ranges.size(); idx++) {
			Range r = ranges.get(idx);
			if (r.start <= cur.end) { // 相邻或重叠
				cur = new Range(cur.start, Math.max(cur.end, r.end));
			} else {
				out.add(cur);
				cur = r;
			}
		}
		out.add(cur);
		return out;
	}

	private static class Block {
		final int start;
		final int end;
		final int innerStart;
		final int innerEnd;

		Block(int start, int end, int innerStart, int innerEnd) {
			this.start = start;
			this.end = end;
			this.innerStart = innerStart;
			this.innerEnd = innerEnd;
		}

		String innerContent(String source) {
			return source.substring(innerStart, innerEnd);
		}
	}

	private static class Range {
		final int start;
		final int end; // [start, end)
		Range(int start, int end) { this.start = start; this.end = end; }
	}
}