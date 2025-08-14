package com.example.chtl;

import com.example.chtl.core.CHTLUnifiedScanner;
import com.example.chtl.core.ScanResult;
import com.example.chtl.core.config.Configuration;
import com.example.chtl.core.config.NameGroup;
import com.example.chtl.core.scanner.ScannerConfig;
import com.example.chtl.compilers.chtl.ChtlLexer;
import com.example.chtl.parsers.chtl.ChtlParser;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class ScannerAndParserTests {
	@Test
	public void scannerRespectsCustomScriptName() {
		Configuration conf = Configuration.defaults();
		conf.nameGroup.KEYWORD_SCRIPT = "Script"; // 自定义大小写
		ScannerConfig sc = new ScannerConfig(conf);
		String src = "div{ Script{ {{box}}->listen({click:fn}) } }";
		ScanResult r = new CHTLUnifiedScanner(sc).scan(src);
		Assertions.assertTrue(r.getFragments().stream().anyMatch(f -> f.origin().equals("Local-Script")));
	}

	@Test
	public void parserParsesStyleAndTemplateUse() {
		String src = "[Template] @Style S{ color:red; } body{ div{ style{ @Style S; .box{ width:1px; } &:hover{ opacity:0.5; } } } }";
		var lex = new ChtlLexer(src).lex();
		var doc = new ChtlParser(src, lex).parseDocument();
		Assertions.assertFalse(doc.items().isEmpty());
	}
}