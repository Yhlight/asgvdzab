package com.example.chtl.compilers;

import com.example.chtl.compilers.chtljs.ChtlJsContext;
import com.example.chtl.compilers.chtljs.ChtlJsGlobalMap;
import com.example.chtl.compilers.chtljs.ChtlJsLexer;
import com.example.chtl.compilers.chtljs.ChtlJsState;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ChtlJsCompiler {
	private static final Logger log = LoggerFactory.getLogger(ChtlJsCompiler.class);

	public String compile(String scriptWithChtlJs) {
		log.debug("编译 CHTL JS 脚本，长度={} 字符", scriptWithChtlJs.length());
		var lexer = new ChtlJsLexer(scriptWithChtlJs);
		var tokens = lexer.lex();
		log.debug("CHTL JS 词法记号数: {}", tokens.size());
		ChtlJsContext ctx = new ChtlJsContext(new ChtlJsGlobalMap(), new ChtlJsState());
		// TODO: 语法与转译逻辑（占位）
		return "/* CHTL-JS-Transpiled (占位) */\n" + scriptWithChtlJs + "\n/* /CHTL-JS-Transpiled */";
	}
}