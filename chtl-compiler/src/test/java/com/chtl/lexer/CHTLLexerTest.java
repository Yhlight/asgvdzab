package com.chtl.lexer;

import org.junit.Test;
import java.util.List;
import static org.junit.Assert.*;

public class CHTLLexerTest {
	@Test
	public void testBasicTokens() {
		String src = "div{\n  style{ width: 100px; }\n  text{ \"你好\" }\n}\n";
		GlobalMap map = new GlobalMap();
		CHTLLexer lexer = new CHTLLexer(src, map);
		List<Token> tokens = lexer.lex();
		assertTrue(tokens.stream().anyMatch(t -> t.getType() == CHTLTokenType.KW_STYLE));
		assertTrue(tokens.stream().anyMatch(t -> t.getType() == CHTLTokenType.KW_TEXT));
	}
}