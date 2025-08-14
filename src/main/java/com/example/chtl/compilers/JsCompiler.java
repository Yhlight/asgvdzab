package com.example.chtl.compilers;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class JsCompiler {
	private static final Logger log = LoggerFactory.getLogger(JsCompiler.class);

	public String compile(String jsSource) {
		log.debug("编译 JS 源，长度={} 字符", jsSource.length());
		return jsSource; // 占位：后续替换为 ANTLR 解析与生成
	}
}