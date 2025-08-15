package com.chtl.scanner;
import java.util.ArrayList;
import java.util.List;

import com.chtl.scanner.BlockScanner.RawBlock;


import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.*;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

/**
 * 块扫描器 - 新一代扫描架构
 * 使用动态块扫描替代字符级扫描
 */
public class BlockScanner {
    
    // 基础块大小配置
    private static final int BASE_BLOCK_SIZE = 256;
    private static final int STYLE_BLOCK_SIZE = 512;
    private static final int SCRIPT_BLOCK_SIZE = 1024;
    private static final int MAX_BLOCK_SIZE = 4096;
    
    // 块类型
    public enum BlockType {
        CHTL_ELEMENT,    // CHTL元素
        STYLE_GLOBAL,    // 全局style块
        STYLE_LOCAL,     // 局部style块
        SCRIPT_GLOBAL,   // 全局script块
        SCRIPT_LOCAL,    // 局部script块
        TEMPLATE,        // 模板定义
        CONFIGURATION,   // 配置块
        ORIGIN,          // 原始嵌入
        IMPORT,          // 导入语句
        NAMESPACE,       // 命名空间
        TEXT             // 纯文本
    }
    
    // 原始块
    public static class RawBlock {
        private final BlockType type;
        private final String content;
        private final int startPosition;
        private final int line;
        private final int column;
        
        public RawBlock(BlockType type, String content, int startPosition, int line, int column) {
            this.type = type;
            this.content = content;
            this.startPosition = startPosition;
            this.line = line;
            this.column = column;
        }
        
        // Getters
        public BlockType getType() { return type; }
        public String getContent() { return content; }
        public int getStartPosition() { return startPosition; }
        public int getLine() { return line; }
        public int getColumn() { return column; }
    }
    
    private String source;
    private int position;
    private int line;
    private int column;
    
    /**
     * 扫描源代码，返回原始块列表
     */
    public List<RawBlock> scanBlocks(String source) {
        this.source = source;
        this.position = 0;
        this.line = 1;
        this.column = 1;
        
        List<RawBlock> blocks = new ArrayList<>();
        
        while (position < source.length()) {
            // 跳过空白
            skipWhitespace();
            if (position >= source.length()) break;
            
            // 识别并提取下一个块
            RawBlock block = extractNextBlock();
            if (block != null) {
                blocks.add(block);
            }
        }
        
        return blocks;
    }
    
    /**
     * 提取下一个块
     */
    private RawBlock extractNextBlock() {
        int startPos = position;
        int startLine = line;
        int startColumn = column;
        
        // 检查特殊块标记
        if (lookingAt("[Template]")) {
            return extractTemplateBlock(startPos, startLine, startColumn);
        }
        
        if (lookingAt("[Configuration]")) {
            return extractConfigurationBlock(startPos, startLine, startColumn);
        }
        
        if (lookingAt("[Origin]")) {
            return extractOriginBlock(startPos, startLine, startColumn);
        }
        
        if (lookingAt("[Import]")) {
            return extractImportStatement(startPos, startLine, startColumn);
        }
        
        if (lookingAt("[Namespace]")) {
            return extractNamespaceBlock(startPos, startLine, startColumn);
        }
        
        if (lookingAt("style")) {
            return extractStyleBlock(startPos, startLine, startColumn, true);
        }
        
        if (lookingAt("script")) {
            return extractScriptBlock(startPos, startLine, startColumn, true);
        }
        
        // 默认为CHTL元素
        return extractCHTLElement(startPos, startLine, startColumn);
    }
    
    /**
     * 提取style块
     */
    private RawBlock extractStyleBlock(int startPos, int startLine, int startColumn, boolean isGlobal) {
        // 消费 "style"
        advance(5);
        skipWhitespace();
        
        if (current() != '{') {
            // 不是style块，回退
            position = startPos;
            line = startLine;
            column = startColumn;
            return null;
        }
        
        // 提取整个块内容
        String blockContent = extractBraceBlock();
        
        return new RawBlock(
            isGlobal ? BlockType.STYLE_GLOBAL : BlockType.STYLE_LOCAL,
            "style " + blockContent,
            startPos,
            startLine,
            startColumn
        );
    }
    
    /**
     * 提取script块
     */
    private RawBlock extractScriptBlock(int startPos, int startLine, int startColumn, boolean isGlobal) {
        // 消费 "script"
        advance(6);
        skipWhitespace();
        
        if (current() != '{') {
            // 不是script块，回退
            position = startPos;
            line = startLine;
            column = startColumn;
            return null;
        }
        
        // 提取整个块内容
        String blockContent = extractBraceBlock();
        
        return new RawBlock(
            isGlobal ? BlockType.SCRIPT_GLOBAL : BlockType.SCRIPT_LOCAL,
            "script " + blockContent,
            startPos,
            startLine,
            startColumn
        );
    }
    
    /**
     * 提取花括号包围的块
     */
    private String extractBraceBlock() {
        StringBuilder content = new StringBuilder();
        int braceDepth = 0;
        
        do {
            char c = current();
            content.append(c);
            
            if (c == '{') {
                braceDepth++;
            } else if (c == '}') {
                braceDepth--;
            } else if (c == '"' || c == '\'') {
                // 处理字符串，避免内部的花括号干扰
                content.append(extractString(c));
            }
            
            advance();
        } while (position < source.length() && braceDepth > 0);
        
        return content.toString();
    }
    
    /**
     * 提取字符串
     */
    private String extractString(char quote) {
        StringBuilder str = new StringBuilder();
        advance(); // 跳过开始引号
        
        while (position < source.length() && current() != quote) {
            if (current() == '\\') {
                str.append(current());
                advance();
                if (position < source.length()) {
                    str.append(current());
                    advance();
                }
            } else {
                str.append(current());
                advance();
            }
        }
        
        if (position < source.length()) {
            advance(); // 跳过结束引号
        }
        
        return str.toString();
    }
    
    /**
     * 提取CHTL元素
     */
    private RawBlock extractCHTLElement(int startPos, int startLine, int startColumn) {
        StringBuilder content = new StringBuilder();
        
        // 提取元素名
        while (position < source.length() && isIdentifierChar(current())) {
            content.append(current());
            advance();
        }
        
        skipWhitespace();
        
        // 检查是否有属性或块
        if (current() == '{') {
            content.append(' ').append(extractBraceBlock());
        } else {
            // 简单元素，提取到分号或换行
            while (position < source.length() && current() != ';' && current() != '\n') {
                content.append(current());
                advance();
            }
            if (current() == ';') {
                content.append(';');
                advance();
            }
        }
        
        return new RawBlock(
            BlockType.CHTL_ELEMENT,
            content.toString(),
            startPos,
            startLine,
            startColumn
        );
    }
    
    /**
     * 提取模板块
     */
    private RawBlock extractTemplateBlock(int startPos, int startLine, int startColumn) {
        StringBuilder content = new StringBuilder();
        
        // 提取 [Template] 及其内容
        while (position < source.length()) {
            content.append(current());
            
            if (current() == '{') {
                advance();
                content.append(extractBraceBlock());
                break;
            }
            
            advance();
        }
        
        return new RawBlock(
            BlockType.TEMPLATE,
            content.toString(),
            startPos,
            startLine,
            startColumn
        );
    }
    
    // 辅助方法
    
    private void skipWhitespace() {
        while (position < source.length() && Character.isWhitespace(current())) {
            advance();
        }
    }
    
    private boolean lookingAt(String keyword) {
        if (position + keyword.length() > source.length()) {
            return false;
        }
        return source.substring(position, position + keyword.length()).equals(keyword);
    }
    
    private char current() {
        return position < source.length() ? source.charAt(position) : '\0';
    }
    
    private void advance() {
        if (position < source.length()) {
            if (source.charAt(position) == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            position++;
        }
    }
    
    private void advance(int count) {
        for (int i = 0; i < count; i++) {
            advance();
        }
    }
    
    private boolean isIdentifierChar(char c) {
        return Character.isLetterOrDigit(c) || c == '_' || c == '-';
    }
    
    // 其他块类型的提取方法...
    private RawBlock extractConfigurationBlock(int startPos, int startLine, int startColumn) {
        // TODO: 实现
        return null;
    }
    
    private RawBlock extractOriginBlock(int startPos, int startLine, int startColumn) {
        // TODO: 实现
        return null;
    }
    
    private RawBlock extractImportStatement(int startPos, int startLine, int startColumn) {
        // TODO: 实现
        return null;
    }
    
    private RawBlock extractNamespaceBlock(int startPos, int startLine, int startColumn) {
        // TODO: 实现
        return null;
    }
}