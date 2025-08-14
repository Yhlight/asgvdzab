package com.example.chtl;

import com.example.chtl.core.CHTLUnifiedScanner;
import com.example.chtl.core.ScanResult;
import com.example.chtl.dispatch.CompilerDispatcher;
import com.example.chtl.merge.ResultMerger;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class BasicRenderTest {
	@Test
	public void rendersElementsAttributesAndText() {
		String src = "body{ id: root; div{ class: box; text{Hello} span{ } } }";
		CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
		ScanResult scan = scanner.scan(src);
		var result = new CompilerDispatcher().dispatch(scan);
		String html = ResultMerger.mergeToHtmlDocument(result);
		Assertions.assertTrue(html.contains("<body"));
		Assertions.assertTrue(html.contains("<div"));
		Assertions.assertTrue(html.contains("Hello"));
	}
}