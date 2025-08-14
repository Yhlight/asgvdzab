package com.example.chtl;

import com.example.chtl.core.CHTLUnifiedScanner;
import com.example.chtl.core.ScanResult;
import com.example.chtl.dispatch.CompilerDispatcher;
import com.example.chtl.merge.ResultMerger;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class ElementTemplateInheritanceTest {
	@Test
	public void rendersParentThenChild() {
		String src = "" +
			"[Template] @Element Base { div{ text{base} } }" +
			"[Template] @Element Child { inherit @Element Base; span{ text{child} } }" +
			"body{ @Element Child; }";
		CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
		ScanResult scan = scanner.scan(src);
		var r = new CompilerDispatcher().dispatch(scan);
		String html = ResultMerger.mergeToHtmlDocument(r);
		Assertions.assertTrue(html.contains("base"), "should contain parent content");
		Assertions.assertTrue(html.contains("child"), "should contain child content");
		Assertions.assertTrue(html.contains("<div>"), "should render parent element");
		Assertions.assertTrue(html.contains("<span>"), "should render child element");
		Assertions.assertTrue(html.indexOf("base") < html.indexOf("child"), "parent content should appear before child content");
	}
}