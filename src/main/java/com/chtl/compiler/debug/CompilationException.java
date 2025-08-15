package com.chtl.compiler.debug;

/**
 * 编译异常
 */
public class CompilationException extends RuntimeException {
    public CompilationException(String message) {
        super(message);
    }
    
    public CompilationException(String message, Throwable cause) {
        super(message, cause);
    }
}