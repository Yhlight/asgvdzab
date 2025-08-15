package com.chtl.compiler;

import com.chtl.grammar.JavaScriptLexer;
import com.chtl.grammar.JavaScriptParser;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;

public class JavaScriptCompiler {
	public String compileScriptBlock(String scriptBlock) {
		try {
			int start = scriptBlock.indexOf('>');
			int end = scriptBlock.toLowerCase().lastIndexOf("</script>");
			String js = (start >= 0 && end >= 0 && end > start) ? scriptBlock.substring(start + 1, end) : scriptBlock;

			JavaScriptLexer lexer = new JavaScriptLexer(CharStreams.fromString(js));
			CommonTokenStream tokens = new CommonTokenStream(lexer);
			JavaScriptParser parser = new JavaScriptParser(tokens);
			parser.file();
			return scriptBlock; // 暂时透传
		} catch (Exception e) {
			return scriptBlock; // 容错透传
		}
	}

	public String compileScriptInline(String js) {
		try {
			JavaScriptLexer lexer = new JavaScriptLexer(CharStreams.fromString(js));
			CommonTokenStream tokens = new CommonTokenStream(lexer);
			JavaScriptParser parser = new JavaScriptParser(tokens);
			parser.file();
			return js; // 暂时透传
		} catch (Exception e) {
			return js; // 容错透传
		}
	}
}