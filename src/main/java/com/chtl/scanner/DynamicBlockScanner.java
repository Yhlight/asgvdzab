package com.chtl.scanner;

import com.chtl.scanner.DynamicBlockScanner.DynamicBlock;


import java.util.*;

/**
 * 动态块扫描器 - 使用可变切割长度实现高效准确的扫描
 * 
 * 核心策略：
 * 1. 动态调整块大小（32-4096字符）
 * 2. 智能边界检测避免切断语法结构
 * 3. 前瞻机制确保完整性
 */
public class DynamicBlockScanner {
    
    // 块大小常量
    private static final int MIN_BLOCK_SIZE = 32;
    private static final int DEFAULT_BLOCK_SIZE = 256;
    private static final int MAX_BLOCK_SIZE = 4096;
    
    // 边界检测模式
    private static final String[] BOUNDARY_MARKERS = {
        "{{", "}}", "->", ".listen", ".delegate", "animate",
        "[Origin]", "script", "style", "{", "}", ";", "\n"
    };
    
    private final String source;
    private int position = 0;
    private int currentBlockSize = DEFAULT_BLOCK_SIZE;
    
    public DynamicBlockScanner(String source) {
        this.source = source;
    }
    
    /**
     * 扫描下一个动态块
     */
    public DynamicBlock scanNext() {
        if (position >= source.length()) {
            return null;
        }
        
        // 计算本次扫描的块大小
        int blockSize = calculateOptimalBlockSize();
        int endPos = Math.min(position + blockSize, source.length());
        
        // 智能调整结束位置，避免切断语法结构
        endPos = adjustEndPosition(endPos);
        
        // 提取块内容
        String content = source.substring(position, endPos);
        DynamicBlock block = new DynamicBlock(content, position, detectBlockType(content));
        
        // 更新位置
        position = endPos;
        
        return block;
    }
    
    /**
     * 计算最优块大小
     */
    private int calculateOptimalBlockSize() {
        // 基于剩余内容和历史性能动态调整
        int remaining = source.length() - position;
        
        if (remaining < MIN_BLOCK_SIZE) {
            return remaining;
        }
        
        // 检查前方是否有特殊结构
        String preview = source.substring(position, 
            Math.min(position + 100, source.length()));
            
        // 如果检测到CHTL JS特征，使用较小块以提高精度
        if (containsCHTLJSFeatures(preview)) {
            currentBlockSize = Math.max(MIN_BLOCK_SIZE, currentBlockSize / 2);
        } 
        // 如果是纯JS/CSS，使用较大块以提高性能
        else if (isPureJSOrCSS(preview)) {
            currentBlockSize = Math.min(MAX_BLOCK_SIZE, currentBlockSize * 2);
        }
        
        return Math.min(currentBlockSize, remaining);
    }
    
    /**
     * 调整结束位置，确保不切断语法结构
     */
    private int adjustEndPosition(int endPos) {
        if (endPos >= source.length()) {
            return source.length();
        }
        
        // 向前查找安全的切割点
        int safePos = findSafeCutPoint(endPos);
        
        // 如果找不到安全点，扩展块大小
        if (safePos == -1) {
            // 尝试扩展到下一个边界
            safePos = findNextBoundary(endPos);
            if (safePos == -1 || safePos - position > MAX_BLOCK_SIZE) {
                // 如果扩展太大，强制在最大大小处切割
                return Math.min(position + MAX_BLOCK_SIZE, source.length());
            }
        }
        
        return safePos;
    }
    
    /**
     * 查找安全的切割点
     */
    private int findSafeCutPoint(int preferredPos) {
        // 检查是否在字符串中
        if (isInString(preferredPos)) {
            // 找到字符串结束
            int stringEnd = findStringEnd(preferredPos);
            if (stringEnd != -1 && stringEnd - position <= MAX_BLOCK_SIZE) {
                return stringEnd + 1;
            }
        }
        
        // 检查是否在注释中
        if (isInComment(preferredPos)) {
            int commentEnd = findCommentEnd(preferredPos);
            if (commentEnd != -1 && commentEnd - position <= MAX_BLOCK_SIZE) {
                return commentEnd + 1;
            }
        }
        
        // 查找最近的边界标记
        int bestPos = -1;
        int minDistance = Integer.MAX_VALUE;
        
        for (String marker : BOUNDARY_MARKERS) {
            int markerPos = source.lastIndexOf(marker, preferredPos);
            if (markerPos > position) {
                int distance = preferredPos - markerPos;
                if (distance < minDistance && distance < 50) {
                    minDistance = distance;
                    bestPos = markerPos + marker.length();
                }
            }
        }
        
        return bestPos;
    }
    
    /**
     * 查找下一个边界
     */
    private int findNextBoundary(int fromPos) {
        int nearestPos = -1;
        
        for (String marker : BOUNDARY_MARKERS) {
            int pos = source.indexOf(marker, fromPos);
            if (pos != -1 && (nearestPos == -1 || pos < nearestPos)) {
                nearestPos = pos + marker.length();
            }
        }
        
        return nearestPos;
    }
    
    /**
     * 检查位置是否在字符串中
     */
    private boolean isInString(int pos) {
        int quoteCount = 0;
        char quoteChar = 0;
        
        for (int i = position; i < pos; i++) {
            char c = source.charAt(i);
            if (i > 0 && source.charAt(i - 1) == '\\') {
                continue; // 跳过转义字符
            }
            
            if (quoteChar == 0 && (c == '"' || c == '\'' || c == '`')) {
                quoteChar = c;
                quoteCount++;
            } else if (c == quoteChar) {
                quoteChar = 0;
                quoteCount++;
            }
        }
        
        return quoteChar != 0;
    }
    
    /**
     * 查找字符串结束位置
     */
    private int findStringEnd(int fromPos) {
        // 先找到字符串开始
        char quoteChar = 0;
        int startPos = fromPos;
        
        for (int i = fromPos; i >= position; i--) {
            char c = source.charAt(i);
            if (c == '"' || c == '\'' || c == '`') {
                if (i == 0 || source.charAt(i - 1) != '\\') {
                    quoteChar = c;
                    startPos = i;
                    break;
                }
            }
        }
        
        if (quoteChar == 0) return -1;
        
        // 找到匹配的结束引号
        for (int i = startPos + 1; i < source.length(); i++) {
            if (source.charAt(i) == quoteChar) {
                if (source.charAt(i - 1) != '\\') {
                    return i;
                }
            }
        }
        
        return -1;
    }
    
    /**
     * 检查是否在注释中
     */
    private boolean isInComment(int pos) {
        // 简化实现：检查单行注释
        int lineStart = source.lastIndexOf('\n', pos);
        if (lineStart == -1) lineStart = position;
        
        int commentPos = source.indexOf("//", lineStart);
        return commentPos != -1 && commentPos < pos;
    }
    
    /**
     * 查找注释结束
     */
    private int findCommentEnd(int fromPos) {
        return source.indexOf('\n', fromPos);
    }
    
    /**
     * 检查是否包含CHTL JS特征
     */
    private boolean containsCHTLJSFeatures(String content) {
        return content.contains("{{") || 
               content.contains("->") ||
               content.contains(".listen") ||
               content.contains(".delegate") ||
               content.contains("animate(");
    }
    
    /**
     * 检查是否是纯JS或CSS
     */
    private boolean isPureJSOrCSS(String content) {
        // 简单启发式：如果没有CHTL特征，认为是纯JS/CSS
        return !content.contains("{{") && 
               !content.contains("[") &&
               !content.contains("@");
    }
    
    /**
     * 检测块类型
     */
    private BlockType detectBlockType(String content) {
        if (content.trim().startsWith("script")) {
            return BlockType.SCRIPT;
        } else if (content.trim().startsWith("style")) {
            return BlockType.STYLE;
        } else if (content.contains("{{")) {
            return BlockType.CHTL_JS;
        } else if (content.contains("[Origin]")) {
            return BlockType.ORIGIN;
        } else {
            return BlockType.MIXED;
        }
    }
    
    /**
     * 动态块
     */
    public static class DynamicBlock {
        private final String content;
        private final int startPos;
        private final BlockType type;
        
        public DynamicBlock(String content, int startPos, BlockType type) {
            this.content = content;
            this.startPos = startPos;
            this.type = type;
        }
        
        public String getContent() { return content; }
        public int getStartPos() { return startPos; }
        public BlockType getType() { return type; }
    }
    
    /**
     * 块类型
     */
    public enum BlockType {
        SCRIPT, STYLE, CHTL_JS, ORIGIN, MIXED
    }
}