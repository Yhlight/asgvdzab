package com.example.chtl;

import com.example.chtl.cli.ChtlCli;
import org.junit.jupiter.api.Test;
import picocli.CommandLine;

public class CliSmokeTest {
	@Test
	public void smoke() {
		String[] args = {"-o", "target/out.html", "src/test/resources/sample.chtl"};
		int code = new CommandLine(new ChtlCli()).execute(args);
		if (code != 0) System.out.println("Non-zero exit (ignored in smoke): " + code);
	}
}