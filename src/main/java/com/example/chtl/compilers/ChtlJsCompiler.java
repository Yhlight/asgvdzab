package com.example.chtl.compilers;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ChtlJsCompiler {
	private static final Logger log = LoggerFactory.getLogger(ChtlJsCompiler.class);

	public String compile(String scriptWithChtlJs) {
		log.debug("编译 CHTL JS 脚本，长度={} 字符", scriptWithChtlJs.length());
		return "/* CHTL-JS-Transpiled (占位) */\n" + scriptWithChtlJs + "\n/* /CHTL-JS-Transpiled */";
	}
}