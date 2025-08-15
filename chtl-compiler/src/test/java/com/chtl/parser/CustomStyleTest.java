package com.chtl.parser;

import com.chtl.ast.DocumentNode;
import com.chtl.generator.CHTLGenerator;
import com.chtl.lexer.GlobalMap;
import org.junit.Test;

import static org.junit.Assert.*;

public class CustomStyleTest {
	@Test
	public void testAndSelectorWithAutoClass() {
		String src = "div{ style{ .box{ width: 100px; } &:hover{ color: red; } } }";
		DocumentNode doc = new CHTLParser(src, new GlobalMap()).parseDocument();
		String html = new CHTLGenerator().toHtml(doc);
		assertTrue(html.contains(" class=\"box\""));
		assertTrue(html.contains(".box:hover{"));
	}
}