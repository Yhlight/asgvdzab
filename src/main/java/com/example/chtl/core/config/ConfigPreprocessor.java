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
			if (!equalsIgnoreCaseAny(head, "Configuration", "Name")) { i = rb+1; continue; }
			int at = skipSpaces(source, rb+1); if (at < n && source.charAt(at) == '@') {
				at++; int end = readIdentEnd(source, at);
				String what = source.substring(at, end);
				int lbrace = source.indexOf('{', end); if (lbrace < 0) break;
				int rbrace = findMatchingBrace(source, lbrace);
				if (rbrace < 0) break;
				String body = source.substring(lbrace+1, rbrace);
				if (equalsIgnoreCaseAny(what, "NameGroup", "Name")) applyNameGroup(body, ng);
				i = rbrace + 1; continue;
			}
			i = rb + 1;
		}
		return ng;
	}

	private static void applyNameGroup(String body, NameGroup ng){
		for (String line : body.split(";")){
			String s = line.trim(); if (s.isEmpty()) continue;
			int c = s.indexOf(':'); if (c < 0) continue;
			String key = s.substring(0,c).trim(); String val = s.substring(c+1).trim();
			val = stripQuotes(val);
			if (equalsIgnoreCaseAny(key, "KEYWORD_ORIGIN")) {
				List<String> list = new ArrayList<>();
				for (String p : val.split(",")) { String t = normalizeToken(p); if (!t.isEmpty()) list.add(t); }
				if (!list.isEmpty()) ng.KEYWORD_ORIGIN = list;
			} else if (equalsIgnoreCaseAny(key, "ORIGIN_STYLE")) {
				String t = normalizeAtToken(val); if (!t.isEmpty()) ng.ORIGIN_STYLE = t;
			} else if (equalsIgnoreCaseAny(key, "ORIGIN_JAVASCRIPT")) {
				String t = normalizeAtToken(val); if (!t.isEmpty()) ng.ORIGIN_JAVASCRIPT = t;
			} else if (equalsIgnoreCaseAny(key, "ORIGIN_HTML")) {
				String t = normalizeAtToken(val); if (!t.isEmpty()) ng.ORIGIN_HTML = t;
			} else if (equalsIgnoreCaseAny(key, "KEYWORD_STYLE")) {
				if (!val.isEmpty()) ng.KEYWORD_STYLE = val.trim();
			} else if (equalsIgnoreCaseAny(key, "KEYWORD_SCRIPT")) {
				if (!val.isEmpty()) ng.KEYWORD_SCRIPT = val.trim();
			} else if (equalsIgnoreCaseAny(key, "KEYWORD_TEXT")) {
				if (!val.isEmpty()) ng.KEYWORD_TEXT = val.trim();
			}
		}
	}

	private static String stripQuotes(String s){ s = s.trim(); if ((s.startsWith("\"") && s.endsWith("\"")) || (s.startsWith("'") && s.endsWith("'"))) return s.substring(1, s.length()-1); return s; }
	private static boolean equalsIgnoreCaseAny(String s, String... opts){ for (String o : opts) if (s.equalsIgnoreCase(o)) return true; return false; }
	private static int skipSpaces(String s, int i){ while (i < s.length() && Character.isWhitespace(s.charAt(i))) i++; return i; }
	private static int readIdentEnd(String s, int i){ while (i < s.length() && (Character.isLetterOrDigit(s.charAt(i)) || s.charAt(i)=='_' )) i++; return i; }
	private static int findMatchingBrace(String s, int open){ int depth = 0; for (int i=open; i<s.length(); i++){ char c=s.charAt(i); if (c=='{') depth++; else if (c=='}'){ depth--; if (depth==0) return i; } } return -1; }
	private static String normalizeToken(String raw){ String t = stripQuotes(raw).trim(); if (t.startsWith("[") && t.endsWith("]")) t = t.substring(1, t.length()-1).trim(); if (t.startsWith("@")) t = t.substring(1).trim(); return t; }
	private static String normalizeAtToken(String raw){ String t = stripQuotes(raw).trim(); if (t.startsWith("@")) t = t.substring(1).trim(); return t; }
}