package com.chtl.lexer;

public enum CHTLJsTokenType {
	LBRACE,        // {
	RBRACE,        // }
	LPAREN,        // (
	RPAREN,        // )
	LBRACKET,      // [
	RBRACKET,      // ]
	COMMA,         // ,
	COLON,         // : (仅在结构中，如 listen 对象)
	SEMICOLON,     // ;
	ARROW,         // ->
	DOT,           // .（与 -> 等价，但推荐 ->）

	MOUSTACHE_L,   // {{
	MOUSTACHE_R,   // }}

	HASH,          // #
	CLASS_DOT,     // . 用在选择器前缀（此处与 DOT 语义不同）

	IDENT,
	NUMBER,
	STRING,
	WHITESPACE,
	NEWLINE,
	UNKNOWN
}