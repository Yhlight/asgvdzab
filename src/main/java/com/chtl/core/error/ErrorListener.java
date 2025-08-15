package com.chtl.core.error;

public interface ErrorListener {
    void onError(String message, String filename, int line, int column);
    void onWarning(String message, String filename, int line, int column);
    void onInfo(String message);
}
