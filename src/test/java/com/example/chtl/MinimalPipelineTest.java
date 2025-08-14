package com.example.chtl;

import com.example.chtl.core.CHTLUnifiedScanner;
import com.example.chtl.core.ScanResult;
import com.example.chtl.dispatch.CompilerDispatcher;
import com.example.chtl.merge.ResultMerger;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class MinimalPipelineTest {
	@Test
	public void pipelineProducesHtml() {
		String src = "body{style{.box{width:1px;}}script{{{box}}->textContent();}[Origin] @Style{:root{--c:red;}}[Origin] @JavaScript{console.log('a');}}";
		CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
		ScanResult scan = scanner.scan(src);
		var r = new CompilerDispatcher().dispatch(scan);
		String html = ResultMerger.mergeToHtmlDocument(r);
		Assertions.assertTrue(html.contains("<style>"));
		Assertions.assertTrue(html.contains("<script>"));
		Assertions.assertTrue(html.contains("<!DOCTYPE html>"));
	}
}