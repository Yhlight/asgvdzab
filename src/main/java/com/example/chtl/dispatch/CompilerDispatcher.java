package com.example.chtl.dispatch;

import com.example.chtl.compilers.ChtlCompiler;
import com.example.chtl.compilers.ChtlJsCompiler;
import com.example.chtl.compilers.CssCompiler;
import com.example.chtl.compilers.JsCompiler;
import com.example.chtl.core.Fragment;
import com.example.chtl.core.FragmentType;
import com.example.chtl.core.ScanResult;

public class CompilerDispatcher {
	private final ChtlCompiler chtlCompiler = new ChtlCompiler();
	private final ChtlJsCompiler chtlJsCompiler = new ChtlJsCompiler();
	private final CssCompiler cssCompiler = new CssCompiler();
	private final JsCompiler jsCompiler = new JsCompiler();

	public CompilationResult dispatch(ScanResult scanResult) {
		// CHTL 主流程
		ChtlCompiler.Output chtlOut = chtlCompiler.compile(scanResult.getSource());

		StringBuilder globalCss = new StringBuilder();
		StringBuilder globalJs = new StringBuilder();

		// CHTL 输出的全局产物
		if (!chtlOut.emittedGlobalCss.isEmpty()) globalCss.append(chtlOut.emittedGlobalCss).append('\n');
		if (!chtlOut.emittedGlobalJs.isEmpty()) globalJs.append(chtlOut.emittedGlobalJs).append('\n');

		// 收集 Origin CSS 与 全局 CSS（来自局部style中产生的）
		for (Fragment f : scanResult.getFragmentsByType(FragmentType.CSS)) {
			String compiled = cssCompiler.compile(f.content());
			if (!compiled.isBlank()) {
				globalCss.append(compiled).append('\n');
			}
		}

		// 收集脚本：普通 JS + CHTL JS 转译为 JS
		for (Fragment f : scanResult.getFragmentsByType(FragmentType.JS)) {
			String compiled = jsCompiler.compile(f.content());
			if (!compiled.isBlank()) globalJs.append(compiled).append('\n');
		}
		for (Fragment f : scanResult.getFragmentsByType(FragmentType.CHTL_JS)) {
			String transpiled = chtlJsCompiler.compile(f.content());
			if (!transpiled.isBlank()) globalJs.append(transpiled).append('\n');
		}

		return new CompilationResult(chtlOut.htmlBody, globalCss.toString(), globalJs.toString());
	}
}