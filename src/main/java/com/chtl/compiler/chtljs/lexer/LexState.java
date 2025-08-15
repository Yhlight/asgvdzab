package com.chtl.compiler.chtljs.lexer;

public enum LexState {
    INITIAL,
    CHTL_JS,
    STRING,
    NUMBER,
    IDENTIFIER,
    OPERATOR,
    COMMENT,
    SELECTOR,
    FUNCTION_CALL,
    OBJECT,
    ARRAY
}
