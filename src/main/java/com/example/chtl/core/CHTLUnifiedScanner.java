package com.example.chtl.core;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CHTLUnifiedScanner {
	private static final Pattern ORIGIN_STYLE_PATTERN = Pattern.compile("\\[Origin\\]\\s*@Style\\s*\\{", Pattern.CASE_INSENSITIVE);
	private static final Pattern ORIGIN_JS_PATTERN = Pattern.compile("\\[Origin\\]\\s*@JavaScript\\s*\\{", Pattern.CASE_INSENSITIVE);
	private static final Pattern SCRIPT_PATTERN = Pattern.compile("\\bscript\\s*\\{", Pattern.CASE_INSENSITIVE);

	public ScanResult scan(String source) {
		ScanResult result = new ScanResult(source);

		// 整体作为一个CHTL片段，供CHTL编译器处理（其内部可忽略[Origin]与script块）
		result.addFragment(new Fragment(
				FragmentType.CHTL,
				source,
				0,
				source.length(),
				"CHTL-Source"
		));

		// 提取 [Origin] @Style
		for (Block b : findBlocks(source, ORIGIN_STYLE_PATTERN)) {
			String css = b.innerContent(source);
			result.addFragment(new Fragment(FragmentType.CSS, css, b.start, b.end, "Origin-Style"));
		}

		// 提取 [Origin] @JavaScript
		for (Block b : findBlocks(source, ORIGIN_JS_PATTERN)) {
			String js = b.innerContent(source);
			result.addFragment(new Fragment(FragmentType.JS, js, b.start, b.end, "Origin-JS"));
		}

		// 提取 script{}，作为 JS 与 CHTL_JS 双重片段交给不同编译器处理
		for (Block b : findBlocks(source, SCRIPT_PATTERN)) {
			String scriptCode = b.innerContent(source);
			result.addFragment(new Fragment(FragmentType.JS, scriptCode, b.start, b.end, "Local-Script"));
			result.addFragment(new Fragment(FragmentType.CHTL_JS, scriptCode, b.start, b.end, "Local-Script"));
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
}