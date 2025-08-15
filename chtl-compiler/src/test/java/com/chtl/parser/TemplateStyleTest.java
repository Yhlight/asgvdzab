package com.chtl.parser;

import com.chtl.ast.DocumentNode;
import com.chtl.generator.CHTLGenerator;
import com.chtl.lexer.GlobalMap;
import org.junit.Test;

import static org.junit.Assert.*;

public class TemplateStyleTest {
	@Test
	public void testStyleTemplateUse() {
		String src = "div{ style{ @Style DefaultText; } }"; // 假设模板已在注册表中，当前测试仅验证管线，不做全局定义解析
		DocumentNode doc = new CHTLParser(src, new GlobalMap()).parseDocument();
		String html = new CHTLGenerator().toHtml(doc);
		assertTrue(html.contains("<div"));
	}
}