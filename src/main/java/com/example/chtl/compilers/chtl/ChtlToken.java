package com.example.chtl.compilers.chtl;

public final class ChtlToken {
	public final ChtlTokenType type;
	public final String lexeme;
	public final int start;
	public final int end;
	public final int line;
	public final int column;

	public ChtlToken(ChtlTokenType type, String lexeme, int start, int end, int line, int column) {
		this.type = type;
		this.lexeme = lexeme;
		this.start = start;
		this.end = end;
		this.line = line;
		this.column = column;
	}

	@Override
	public String toString() {
		return type + "('" + lexeme + "')@" + line + ":" + column;
	}
}