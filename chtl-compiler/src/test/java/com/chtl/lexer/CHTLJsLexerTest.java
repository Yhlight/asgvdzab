package com.chtl.lexer;

import org.junit.Test;
import java.util.List;
import static org.junit.Assert.*;

public class CHTLJsLexerTest {
	@Test
	public void testMoustacheAndArrow() {
		String src = "{{box}}->textContent();";
		CHTLJsLexer lexer = new CHTLJsLexer(src);
		List<Token> tokens = lexer.lex();
		assertTrue(tokens.stream().anyMatch(t -> t.getType() == CHTLJsTokenType.MOUSTACHE_L));
		assertTrue(tokens.stream().anyMatch(t -> t.getType() == CHTLJsTokenType.MOUSTACHE_R));
		assertTrue(tokens.stream().anyMatch(t -> t.getType() == CHTLJsTokenType.ARROW));
	}
}