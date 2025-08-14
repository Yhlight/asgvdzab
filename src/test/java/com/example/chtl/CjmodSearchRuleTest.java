package com.example.chtl;

import com.example.chtl.cli.ChtlCli;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import picocli.CommandLine;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

public class CjmodSearchRuleTest {
	@Test
	public void nameSearchFromOfficialModule() throws Exception {
		Path tmp = Files.createTempDirectory("cjmod-official");
		Path official = tmp.resolve("module"); Files.createDirectories(official);
		Files.writeString(official.resolve("Anim.cjmod"), "console.log('CJMOD');", StandardCharsets.UTF_8);
		System.setProperty("CHTL_OFFICIAL_MODULE_DIR", official.toString());
		try {
			Path input = tmp.resolve("main.chtl");
			Files.writeString(input, "[Import] @CJmod from Anim; body{}", StandardCharsets.UTF_8);
			int code = new CommandLine(new ChtlCli()).execute("-o", tmp.resolve("out.html").toString(), input.toString());
			Assertions.assertEquals(0, code);
			String out = Files.readString(tmp.resolve("out.html"), StandardCharsets.UTF_8);
			Assertions.assertTrue(out.contains("CJMOD"));
		} finally {
			System.clearProperty("CHTL_OFFICIAL_MODULE_DIR");
		}
	}
}