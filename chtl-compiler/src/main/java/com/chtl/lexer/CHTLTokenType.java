package com.chtl.lexer;

public enum CHTLTokenType {
	LBRACE,        // {
	RBRACE,        // }
	LBRACKET,      // [
	RBRACKET,      // ]
	LPAREN,        // (
	RPAREN,        // )
	COLON,         // :
	EQUAL,         // =
	SEMICOLON,     // ;
	COMMA,         // ,
	ARROW,         // -> (用于CHTL JS明确语法)
	AT,            // @

	KW_TEMPLATE,   // [Template]
	KW_CUSTOM,     // [Custom]
	KW_ORIGIN,     // [Origin]
	KW_IMPORT,     // [Import]
	KW_NAMESPACE,  // [Namespace]
	KW_STYLE,      // style 局部样式块关键字
	KW_SCRIPT,     // script 局部脚本块关键字
	KW_TEXT,       // text 文本节点
	KW_INHERIT,    // inherit
	KW_DELETE,     // delete
	KW_INSERT,     // insert
	KW_AFTER,      // after
	KW_BEFORE,     // before
	KW_REPLACE,    // replace
	KW_AT_TOP,     // at top
	KW_AT_BOTTOM,  // at bottom
	KW_FROM,       // from
	KW_AS,         // as
	KW_EXCEPT,     // except

	IDENT,         // 标识符（元素名、变量名等）
	NUMBER,        // 数字
	STRING,        // '...' 或 "..."
	LITERAL,       // 无修饰字面量（不含空白和分隔符）

	LINE_COMMENT,  // //...
	BLOCK_COMMENT, // /*...*/
	GEN_COMMENT,   // -- 生成器识别注释
	WHITESPACE,
	NEWLINE,
	UNKNOWN
}