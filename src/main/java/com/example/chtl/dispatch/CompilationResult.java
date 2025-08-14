package com.example.chtl.dispatch;

public class CompilationResult {
	private final String htmlBody;
	private final String globalCss;
	private final String globalJs;

	public CompilationResult(String htmlBody, String globalCss, String globalJs) {
		this.htmlBody = htmlBody;
		this.globalCss = globalCss;
		this.globalJs = globalJs;
	}

	public String getHtmlBody() {
		return htmlBody;
	}

	public String getGlobalCss() {
		return globalCss;
	}

	public String getGlobalJs() {
		return globalJs;
	}
}