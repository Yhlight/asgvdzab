package com.chtl.compiler.debug;

import java.io.*;
import java.nio.file.*;
import java.util.*;

/**
 * 源代码管理器
 * 负责读取和缓存源代码
 */
public class SourceCodeManager {
    // 缓存已读取的文件
    private final Map<String, List<String>> fileCache = new HashMap<>();
    
    /**
     * 获取代码片段
     */
    public SourceCodeSnippet getSnippet(String sourcePath, int errorLine, int errorColumn) {
        if (sourcePath == null || errorLine <= 0) {
            return null;
        }
        
        List<String> lines = getFileLines(sourcePath);
        if (lines == null || errorLine > lines.size()) {
            return null;
        }
        
        SourceCodeSnippet snippet = new SourceCodeSnippet(sourcePath, errorLine, errorColumn);
        
        // 获取错误行前后的几行
        int contextLines = 3;
        int startLine = Math.max(1, errorLine - contextLines);
        int endLine = Math.min(lines.size(), errorLine + contextLines);
        
        for (int i = startLine; i <= endLine; i++) {
            snippet.addLine(i, lines.get(i - 1));
        }
        
        return snippet;
    }
    
    /**
     * 获取文件行
     */
    private List<String> getFileLines(String sourcePath) {
        // 检查缓存
        if (fileCache.containsKey(sourcePath)) {
            return fileCache.get(sourcePath);
        }
        
        try {
            Path path = Paths.get(sourcePath);
            if (Files.exists(path)) {
                List<String> lines = Files.readAllLines(path);
                fileCache.put(sourcePath, lines);
                return lines;
            }
        } catch (IOException e) {
            // 读取失败，返回null
        }
        
        return null;
    }
    
    /**
     * 清除缓存
     */
    public void clearCache() {
        fileCache.clear();
    }
}