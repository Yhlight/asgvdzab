package com.example.chtl;

import com.example.chtl.cli.ChtlCli;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import picocli.CommandLine;

public class StrictSpecializationErrorTest {
	@Test
	public void cliExitsWithCode2OnInsertTargetMissing() {
		String[] args = {"--strict", "-o", "target/strict.html", "src/test/resources/strict-error.chtl"};
		int code = new CommandLine(new ChtlCli()).execute(args);
		Assertions.assertEquals(2, code, "strict mode should exit with code 2 on parse/specialization error");
	}
}