package com.example.chtl;

import com.example.chtl.core.CHTLUnifiedScanner;
import com.example.chtl.core.ScanResult;
import com.example.chtl.compilers.chtl.ChtlLexer;
import com.example.chtl.parsers.chtl.ChtlParser;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class ScannerParserTests {
	@Test
	public void scanDynamicKeywords() {
		String src = "[Origin] @Style { .x { a: b; } } script { {{box}}->listen({click:fn}); }";
		ScanResult r = new CHTLUnifiedScanner().scan(src);
		Assertions.assertTrue(r.getFragments().stream().anyMatch(f->f.type().name().equals("CSS")));
		Assertions.assertTrue(r.getFragments().stream().anyMatch(f->f.type().name().equals("CHTL_JS")));
	}

	@Test
	public void parseTemplateVarNamespace() {
		String src = "[Template] @Var Theme { color: red; } [Namespace] space { div { style { color: Theme(color); } } }";
		var toks = new ChtlLexer(src).lex();
		var doc = new ChtlParser(src, toks).parseDocument();
		Assertions.assertFalse(doc.items().isEmpty());
	}
}