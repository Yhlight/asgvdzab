package com.example.chtl.compilers;

import com.example.chtl.compilers.chtl.ChtlContext;
import com.example.chtl.compilers.chtl.ChtlGlobalMap;
import com.example.chtl.compilers.chtl.ChtlLexer;
import com.example.chtl.compilers.chtl.ChtlState;
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
		log.debug("开始编译 CHTL 源，长度={} 字符", chtlSource.length());
		// 词法分析（暂不进入语法阶段）
		var lexer = new ChtlLexer(chtlSource);
		var tokens = lexer.lex();
		log.debug("CHTL 词法记号数: {}", tokens.size());

		// 上下文初始化
		ChtlContext ctx = new ChtlContext(new ChtlGlobalMap(), new ChtlState());
		// TODO: 语法与生成逻辑（占位）
		String htmlBody = "<!-- CHTL 编译占位输出，后续替换为真实生成的 HTML -->";
		return new Output(htmlBody, ctx.globals.getGlobalCss(), ctx.globals.getGlobalJs());
	}
}