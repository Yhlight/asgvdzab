package com.example.chtl.compilers;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class CssCompiler {
	private static final Logger log = LoggerFactory.getLogger(CssCompiler.class);

	public String compile(String cssSource) {
		log.debug("编译 CSS 源，长度={} 字符", cssSource.length());
		return cssSource; // 占位：后续替换为 ANTLR 解析与生成
	}
}