package com.chtl.api;

import com.chtl.model.CodeFragment;
import com.chtl.pipeline.CompilerDispatcher;
import com.chtl.pipeline.CompilationResultMerger;
import com.chtl.scanner.CHTLUnifiedScanner;

import java.util.List;

public class CHTLCompilerService {
	private final CHTLUnifiedScanner scanner;
	private final CompilerDispatcher dispatcher;
	private final CompilationResultMerger merger;

	public CHTLCompilerService() {
		this.scanner = new CHTLUnifiedScanner();
		this.dispatcher = new CompilerDispatcher();
		this.merger = new CompilationResultMerger();
	}

	// 扫描器：
	// - JS/CSS 宽判：仅以 CHTL/CHTL JS token 为分割点，其余作为 JS_CODE/CSS_CODE 大块输出
	// - CHTL/CHTL JS 严判：以最小单元切割（例如 {{id}} 或 {{box}}->），并允许基于上下文合并相邻 token，避免过度碎片化
	public String compileToHtml(String chtlSource) {
		List<CodeFragment> fragments = scanner.scan(chtlSource);
		List<String> compiled = dispatcher.compileAll(fragments);
		return merger.merge(chtlSource, fragments, compiled);
	}
}