package com.example.chtl.compilers;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ChtlCompiler {
	private static final Logger log = LoggerFactory.getLogger(ChtlCompiler.class);

	public static class Output {
		public final String htmlBody;
		public final String emittedGlobalCss;
		public final String emittedGlobalJs;

		public Output(String htmlBody, String emittedGlobalCss, String emittedGlobalJs) {
			this.htmlBody = htmlBody;
			this.emittedGlobalCss = emittedGlobalCss;
			this.emittedGlobalJs = emittedGlobalJs;
		}
	}

	public Output compile(String chtlSource) {
		// 极简占位：当前仅输出占位 HTML，并忽略 [Origin] 与 script 块的真正处理
		log.debug("开始编译 CHTL 源，长度={} 字符", chtlSource.length());
		String htmlBody = "<!-- CHTL 编译占位输出，后续替换为真实生成的 HTML -->";
		return new Output(htmlBody, "", "");
	}
}