package com.chtl.exception;

import com.chtl.model.SourceLocation;

public class CompilationException extends Exception {
    private final SourceLocation location;

    public CompilationException(String message) {
        super(message);
        this.location = null;
    }

    public CompilationException(String message, SourceLocation location) {
        super(message);
        this.location = location;
    }

    public CompilationException(String message, Throwable cause) {
        super(message, cause);
        this.location = null;
    }

    public CompilationException(String message, SourceLocation location, Throwable cause) {
        super(message, cause);
        this.location = location;
    }

    public SourceLocation getLocation() {
        return location;
    }
}
