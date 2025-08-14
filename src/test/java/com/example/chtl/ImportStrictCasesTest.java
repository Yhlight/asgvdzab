package com.example.chtl;

import com.example.chtl.cli.ChtlCli;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import picocli.CommandLine;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

public class ImportStrictCasesTest {
	@Test
	public void wildcardMixedConflictShouldFailInStrict() throws Exception {
		Path tmp = Files.createTempDirectory("chtl-wc");
		Path mod = tmp.resolve("module"); Files.createDirectories(mod);
		// 两个文件定义同名模板，模拟冲突
		Files.writeString(mod.resolve("A.chtl"), "[Template] @Element T { div{} }", StandardCharsets.UTF_8);
		Files.writeString(mod.resolve("B.chtl"), "[Template] @Element T { span{} }", StandardCharsets.UTF_8);
		Path input = tmp.resolve("main.chtl");
		Files.writeString(input, "[Import] @Chtl from *.chtl; body{}", StandardCharsets.UTF_8);
		int code = new CommandLine(new ChtlCli()).execute("--strict", "-o", tmp.resolve("out.html").toString(), input.toString());
		Assertions.assertEquals(2, code, "同名模板冲突应在严格模式失败");
	}

	@Test
	public void cjmodNegativeCases() throws Exception {
		Path tmp = Files.createTempDirectory("cjmod-neg");
		Path input = tmp.resolve("main.chtl");
		// 目录路径
		Files.writeString(input, "[Import] @CJmod from some/dir; body{}", StandardCharsets.UTF_8);
		int code1 = new CommandLine(new ChtlCli()).execute("--strict", "-o", tmp.resolve("out1.html").toString(), input.toString());
		Assertions.assertEquals(2, code1);
		// 无文件信息（等价上面，确保覆盖）
		Files.writeString(input, "[Import] @CJmod from some; body{}", StandardCharsets.UTF_8);
		int code2 = new CommandLine(new ChtlCli()).execute("--strict", "-o", tmp.resolve("out2.html").toString(), input.toString());
		Assertions.assertEquals(2, code2);
		// 未找到资源
		Files.writeString(input, "[Import] @CJmod from none.cjmod; body{}", StandardCharsets.UTF_8);
		int code3 = new CommandLine(new ChtlCli()).execute("--strict", "-o", tmp.resolve("out3.html").toString(), input.toString());
		Assertions.assertEquals(2, code3);
	}

	@Test
	public void sameNamespaceMergeNoDuplicate() throws Exception {
		Path tmp = Files.createTempDirectory("ns-merge");
		Path mod = tmp.resolve("module/MyNs"); Files.createDirectories(mod);
		Files.writeString(mod.resolve("a.chtl"), "[Template] @Element A { div{} }", StandardCharsets.UTF_8);
		Files.writeString(mod.resolve("b.chtl"), "[Template] @Element B { span{} }", StandardCharsets.UTF_8);
		Path input = tmp.resolve("main.chtl");
		Files.writeString(input, "[Import] @Chtl from MyNs/*.chtl; body{ @Element A from MyNs; @Element B from MyNs; }", StandardCharsets.UTF_8);
		int code = new CommandLine(new ChtlCli()).execute("-o", tmp.resolve("out.html").toString(), input.toString());
		Assertions.assertEquals(0, code);
		String out = Files.readString(tmp.resolve("out.html"), StandardCharsets.UTF_8);
		Assertions.assertTrue(out.contains("<!DOCTYPE html>"));
	}
}