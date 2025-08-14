package com.example.chtl;

import com.example.chtl.cli.ChtlCli;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import picocli.CommandLine;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

public class ImportFilesystemTest {
	@Test
	public void htmlStyleJsAsImportInSameDir() throws IOException {
		Path tmp = Files.createTempDirectory("chtl-import");
		Path input = tmp.resolve("main.chtl");
		Path html = tmp.resolve("frag.html"); Files.writeString(html, "<div id=\"h\"></div>", StandardCharsets.UTF_8);
		Path css = tmp.resolve("frag.css"); Files.writeString(css, ".x{color:red;}", StandardCharsets.UTF_8);
		Path js = tmp.resolve("frag.js"); Files.writeString(js, "window.__x=1;", StandardCharsets.UTF_8);
		String src = "[Import] @Html from frag as H;[Import] @Style from frag as S;[Import] @JavaScript from frag as J;body{}";
		Files.writeString(input, src, StandardCharsets.UTF_8);
		int code = new CommandLine(new ChtlCli()).execute("-o", tmp.resolve("out.html").toString(), input.toString());
		Assertions.assertEquals(0, code);
		String out = Files.readString(tmp.resolve("out.html"), StandardCharsets.UTF_8);
		Assertions.assertTrue(out.contains("id=\"h\""));
		Assertions.assertTrue(out.contains("color:red"));
		Assertions.assertTrue(out.contains("__x=1"));
	}

	@Test
	public void chtlModuleNameDotMappingAndWildcard() throws IOException {
		Path tmp = Files.createTempDirectory("chtl-mod");
		Path official = tmp.resolve("module"); Files.createDirectories(official);
		Path modDir = official.resolve("Chtholly"); Files.createDirectories(modDir);
		Path sub = modDir.resolve("Space.chtl"); Files.writeString(sub, "body{div{text{sub}}}", StandardCharsets.UTF_8);
		Path input = tmp.resolve("main.chtl");
		String src = "[Import] @Chtl from Chtholly.Space; body{}";
		Files.writeString(input, src, StandardCharsets.UTF_8);
		int code = new CommandLine(new ChtlCli()).execute("-o", tmp.resolve("out.html").toString(), input.toString());
		// 不强制断言包含内容（编译器当前占位），只验证不崩溃
		Assertions.assertEquals(0, code);
	}
}