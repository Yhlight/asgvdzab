package com.chtl.parser;

import com.chtl.ast.DocumentNode;
import com.chtl.generator.CHTLGenerator;
import com.chtl.generator.CHTLJsGenerator;
import com.chtl.lexer.GlobalMap;
import com.chtl.ast.CHTLJsNodes.Chain;
import org.junit.Test;

import static org.junit.Assert.*;

public class ParserGeneratorSmokeTest {
	@Test
	public void testChtlToHtml() {
		String src = "div{ text{ \"Hello\" } script{ {{#app}}->textContent(\"Hi\"); } }";
		DocumentNode doc = new CHTLParser(src, new GlobalMap()).parseDocument();
		String html = new CHTLGenerator().toHtml(doc);
		assertTrue(html.contains("<div>"));
		assertTrue(html.contains("<script>"));
	}

	@Test
	public void testChtlJsToJs() {
		String js = "{{#app}}->textContent(\"Hi\")";
		Chain chain = new CHTLJsParser(js).parseChain();
		String out = new CHTLJsGenerator().toJavaScript(chain);
		assertTrue(out.startsWith("document.getElementById('app')"));
		assertTrue(out.contains(".textContent("));
	}
}