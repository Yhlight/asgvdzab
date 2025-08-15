package com.chtl.compiler;

import com.chtl.grammar.CSSLexer;
import com.chtl.grammar.CSSParser;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;

public class CssCompiler {
	public String compileStyleBlock(String styleBlock) {
		try {
			int start = styleBlock.indexOf('>');
			int end = styleBlock.toLowerCase().lastIndexOf("</style>");
			String css = (start >= 0 && end >= 0 && end > start) ? styleBlock.substring(start + 1, end) : styleBlock;

			CSSLexer lexer = new CSSLexer(CharStreams.fromString(css));
			CommonTokenStream tokens = new CommonTokenStream(lexer);
			CSSParser parser = new CSSParser(tokens);
			parser.file();
			return styleBlock; // 暂时透传
		} catch (Exception e) {
			return styleBlock; // 容错透传
		}
	}

	public String compileStyleInline(String css) {
		try {
			CSSLexer lexer = new CSSLexer(CharStreams.fromString(css));
			CommonTokenStream tokens = new CommonTokenStream(lexer);
			CSSParser parser = new CSSParser(tokens);
			parser.file();
			return css; // 暂时透传
		} catch (Exception e) {
			return css; // 容错透传
		}
	}
}