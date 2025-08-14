package com.example.chtl;

import com.example.chtl.cli.ChtlCli;
import org.junit.jupiter.api.Test;
import picocli.CommandLine;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

public class ImportPerfBenchmarkTest {
	@Test
	public void wildcardImportBenchmark() throws Exception {
		Path tmp = Files.createTempDirectory("perf");
		Path mod = tmp.resolve("module/lib"); Files.createDirectories(mod);
		for (int i=0;i<50;i++) {
			Files.writeString(mod.resolve("t"+i+".chtl"), "[Template] @Element X"+i+" { div{} }", StandardCharsets.UTF_8);
		}
		Path input = tmp.resolve("main.chtl");
		Files.writeString(input, "[Import] @Chtl from lib/*.chtl; body{}", StandardCharsets.UTF_8);
		long t0 = System.currentTimeMillis();
		int code = new CommandLine(new ChtlCli()).execute("-o", tmp.resolve("out.html").toString(), input.toString());
		long t1 = System.currentTimeMillis();
		System.out.println("Wildcard import elapsed: "+(t1-t0)+" ms, code="+code);
	}
}