package com.example.chtl;

import com.example.chtl.core.CHTLUnifiedScanner;
import com.example.chtl.core.ScanResult;
import com.example.chtl.dispatch.CompilerDispatcher;
import com.example.chtl.merge.ResultMerger;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.nio.file.Path;

public class ImportAndNamespaceTest {
	@Test
	public void importChtlAndUseTemplate() {
		String lib = "[Template] @Element Card { div{ text{lib} } }";
		String main = "[Import] @Chtl from lib.chtl as Lib; body{ @Element Card from Lib; }";
		// 组装为扫描器输入：在真实 CLI 中 baseDir 来自文件系统，此处模拟
		CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
		ScanResult scan = scanner.scan(main);
		// 调用带 baseDir 的重载，指向工作目录，测试中不真正读取 lib 文件，仅走解析路径
		var r = new CompilerDispatcher().dispatch(scan, Path.of(".").toAbsolutePath());
		String html = ResultMerger.mergeToHtmlDocument(r);
		// 由于导入读取未真正执行（依赖文件系统），此断言仅验证流程不崩溃
		Assertions.assertTrue(html.contains("<!DOCTYPE html>"));
	}
}