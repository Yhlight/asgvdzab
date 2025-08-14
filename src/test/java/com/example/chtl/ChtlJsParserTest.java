package com.example.chtl;

import com.example.chtl.ast.chtljs.ChtlJsScript;
import com.example.chtl.compilers.chtljs.ChtlJsLexer;
import com.example.chtl.parsers.chtljs.ChtlJsParser;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class ChtlJsParserTest {
	@Test
	public void parseSelectorAndArrowChain() {
		String src = "{{.box button[0]}}->listen({click:()=>{}})->animate({duration:100});";
		var lexer = new ChtlJsLexer(src);
		var tokens = lexer.lex();
		var parser = new ChtlJsParser(src, tokens);
		ChtlJsScript script = parser.parseScript();
		Assertions.assertEquals(1, script.expressions().size());
		var expr = script.expressions().get(0);
		Assertions.assertNotNull(expr.selector());
		Assertions.assertNotNull(expr.calls());
		Assertions.assertTrue(expr.calls().chain().size() >= 2);
	}
}