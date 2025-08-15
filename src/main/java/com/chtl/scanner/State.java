package com.chtl.scanner;

public enum State {
    INITIAL,
    CHTL,
    CHTL_CONTENT,
    STYLE,
    SCRIPT,
    LOCAL_STYLE,
    LOCAL_SCRIPT,
    JAVASCRIPT,
    CSS,
    CHTL_JS,
    COMMENT,
    STRING,
    TEMPLATE
}
