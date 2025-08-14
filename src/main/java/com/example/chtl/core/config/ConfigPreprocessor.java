package com.example.chtl.core.config;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class ConfigPreprocessor {
	public static NameGroup extractNameGroup(String source){
		NameGroup ng = NameGroup.defaults();
		int i = 0; int n = source.length();
		while (i < n) {
			int lb = source.indexOf('[', i); if (lb < 0) break;
			int rb = source.indexOf(']', lb+1); if (rb < 0) break;
			String head = source.substring(lb+1, rb).trim();
			if (!equalsIgnoreCaseAny(head, "Configuration")) { i = rb+1; continue; }
			int at = skipSpaces(source, rb+1); if (at < n && source.charAt(at) == '@') {
				at++; int end = readIdentEnd(source, at);
				String what = source.substring(at, end);
				int lbrace = source.indexOf('{', end); if (lbrace < 0) break;
				int rbrace = findMatchingBrace(source, lbrace);
				if (rbrace < 0) break;
				String body = source.substring(lbrace+1, rbrace);
				if (equalsIgnoreCaseAny(what, "NameGroup")) applyNameGroup(body, ng);
				i = rbrace + 1; continue;
			}
			i = rb + 1;
		}
		return ng;
	}

	private static void applyNameGroup(String body, NameGroup ng){
		// 解析形如 KEY: value; 支持 CSV
		for (String line : body.split(";")){
			String s = line.trim(); if (s.isEmpty()) continue;
			int c = s.indexOf(':'); if (c < 0) continue;
			String key = s.substring(0,c).trim(); String val = s.substring(c+1).trim();
			val = stripQuotes(val);
			if (equalsIgnoreCaseAny(key, "KEYWORD_ORIGIN")) {
				List<String> list = new ArrayList<>(); for (String p : val.split(",")) { String t = p.trim(); if (!t.isEmpty()) list.add(t); }
				if (!list.isEmpty()) ng.KEYWORD_ORIGIN = list;
			} else if (equalsIgnoreCaseAny(key, "ORIGIN_STYLE")) {
				if (!val.isEmpty()) ng.ORIGIN_STYLE = val;
			} else if (equalsIgnoreCaseAny(key, "ORIGIN_JAVASCRIPT")) {
				if (!val.isEmpty()) ng.ORIGIN_JAVASCRIPT = val;
			} else if (equalsIgnoreCaseAny(key, "ORIGIN_HTML")) {
				if (!val.isEmpty()) ng.ORIGIN_HTML = val;
			} else if (equalsIgnoreCaseAny(key, "KEYWORD_STYLE")) {
				if (!val.isEmpty()) ng.KEYWORD_STYLE = val;
			} else if (equalsIgnoreCaseAny(key, "KEYWORD_SCRIPT")) {
				if (!val.isEmpty()) ng.KEYWORD_SCRIPT = val;
			} else if (equalsIgnoreCaseAny(key, "KEYWORD_TEXT")) {
				if (!val.isEmpty()) ng.KEYWORD_TEXT = val;
			}
		}
	}

	private static String stripQuotes(String s){ s = s.trim(); if ((s.startsWith("\"") && s.endsWith("\"")) || (s.startsWith("'") && s.endsWith("'"))) return s.substring(1, s.length()-1); return s; }
	private static boolean equalsIgnoreCaseAny(String s, String... opts){ for (String o : opts) if (s.equalsIgnoreCase(o)) return true; return false; }
	private static int skipSpaces(String s, int i){ while (i < s.length() && Character.isWhitespace(s.charAt(i))) i++; return i; }
	private static int readIdentEnd(String s, int i){ while (i < s.length() && (Character.isLetterOrDigit(s.charAt(i)) || s.charAt(i)=='_' )) i++; return i; }
	private static int findMatchingBrace(String s, int open){ int depth = 0; for (int i=open; i<s.length(); i++){ char c=s.charAt(i); if (c=='{') depth++; else if (c=='}'){ depth--; if (depth==0) return i; } } return -1; }
}