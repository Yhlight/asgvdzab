package com.example.chtl;

import com.example.chtl.core.CHTLUnifiedScanner;
import com.example.chtl.core.ScanResult;
import com.example.chtl.core.config.ConfigPreprocessor;
import com.example.chtl.core.config.Configuration;
import com.example.chtl.core.scanner.ScannerConfig;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class ConfigurationNameGroupTest {
	@Test
	public void customKeywordsAreRecognized() {
		String src = "[Configuration] @NameGroup{ KEYWORD_ORIGIN: Or, Origin, H; ORIGIN_STYLE: @S; ORIGIN_JAVASCRIPT: @J; ORIGIN_HTML: @H; KEYWORD_STYLE: st; KEYWORD_SCRIPT: sc; KEYWORD_TEXT: tx; }"
			+ "body{st{.a{w:1;}}sc{{{a}}->x();}[Or] @S{:root{--x:1;}}[Origin] @J{console.log(1);}[H] @H{<b id='k'></b>}}";
		var ng = ConfigPreprocessor.extractNameGroup(src);
		Configuration cfg = Configuration.defaults(); cfg.nameGroup = ng;
		CHTLUnifiedScanner scanner = new CHTLUnifiedScanner(new ScannerConfig(cfg));
		ScanResult scan = scanner.scan(src);
		Assertions.assertTrue(scan.getFragments().stream().anyMatch(f->f.origin().equals("Origin-Style")));
		Assertions.assertTrue(scan.getFragments().stream().anyMatch(f->f.origin().equals("Origin-JS")));
		Assertions.assertTrue(scan.getFragments().stream().anyMatch(f->f.origin().equals("Origin-Html")));
	}
}