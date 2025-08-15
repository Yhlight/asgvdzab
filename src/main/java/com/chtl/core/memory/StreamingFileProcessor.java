package com.chtl.core.memory;
import java.nio.MappedByteBuffer;

import com.chtl.scanner.Fragment;
import com.chtl.scanner.FragmentType;
import com.chtl.context.CompilationContext;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.util.*;
import java.util.concurrent.*;
import java.util.function.Consumer;

/**
 * 流式文件处理器
 * 优化大文件处理的内存使用
 */
public class StreamingFileProcessor {
    private static final int DEFAULT_BUFFER_SIZE = 8 * 1024;        // 8KB
    private static final int LARGE_FILE_THRESHOLD = 10 * 1024 * 1024; // 10MB
    private static final int CHUNK_SIZE = 1024 * 1024;             // 1MB chunks
    
    private final ExecutorService executorService;
    private final int bufferSize;
    
    public StreamingFileProcessor() {
        this(DEFAULT_BUFFER_SIZE);
    }
    
    public StreamingFileProcessor(int bufferSize) {
        this.bufferSize = bufferSize;
        this.executorService = ForkJoinPool.commonPool();
    }
    
    /**
     * 流式处理CHTL文件
     */
    public CompilationResult processFile(Path filePath, CompilationContext context) throws IOException {
        long fileSize = Files.size(filePath);
        
        if (fileSize < LARGE_FILE_THRESHOLD) {
            // 小文件直接读取
            return processSmallFile(filePath, context);
        } else {
            // 大文件流式处理
            return processLargeFile(filePath, context);
        }
    }
    
    /**
     * 处理小文件
     */
    private CompilationResult processSmallFile(Path filePath, CompilationContext context) throws IOException {
        String content = Files.readString(filePath);
        return processContent(content, filePath.toString(), context);
    }
    
    /**
     * 流式处理大文件
     */
    private CompilationResult processLargeFile(Path filePath, CompilationContext context) throws IOException {
        CompilationResult result = new CompilationResult();
        
        try (RandomAccessFile file = new RandomAccessFile(filePath.toFile(), "r");
             FileChannel channel = file.getChannel()) {
            
            long fileSize = channel.size();
            int numChunks = (int) Math.ceil((double) fileSize / CHUNK_SIZE);
            
            // 并行处理各个块
            List<Future<ChunkResult>> futures = new ArrayList<>();
            
            for (int i = 0; i < numChunks; i++) {
                long start = (long) i * CHUNK_SIZE;
                long size = Math.min(CHUNK_SIZE, fileSize - start);
                
                futures.add(executorService.submit(() -> 
                    processChunk(channel, start, size, filePath.toString(), context)));
            }
            
            // 收集结果
            List<ChunkResult> chunkResults = new ArrayList<>();
            for (Future<ChunkResult> future : futures) {
                try {
                    chunkResults.add(future.get());
                } catch (InterruptedException | ExecutionException e) {
                    throw new IOException("处理文件块失败", e);
                }
            }
            
            // 合并块结果
            mergeChunkResults(chunkResults, result);
        }
        
        return result;
    }
    
    /**
     * 处理文件块
     */
    private ChunkResult processChunk(FileChannel channel, long start, long size, 
                                   String filePath, CompilationContext context) throws IOException {
        ByteBuffer buffer = ByteBuffer.allocate((int) size);
        channel.read(buffer, start);
        buffer.flip();
        
        String content = StandardCharsets.UTF_8.decode(buffer).toString();
        
        // 查找块的完整边界
        int adjustedStart = 0;
        int adjustedEnd = content.length();
        
        // 如果不是第一个块，找到第一个完整的语句开始
        if (start > 0) {
            adjustedStart = findStatementStart(content);
        }
        
        // 如果不是最后一个块，找到最后一个完整的语句结束
        if (start + size < channel.size()) {
            adjustedEnd = findStatementEnd(content);
        }
        
        String adjustedContent = content.substring(adjustedStart, adjustedEnd);
        
        // 处理调整后的内容
        ChunkResult result = new ChunkResult();
        result.fragments = scanContent(adjustedContent, filePath, context);
        result.startOffset = start + adjustedStart;
        result.endOffset = start + adjustedEnd;
        
        return result;
    }
    
    /**
     * 查找语句开始位置
     */
    private int findStatementStart(String content) {
        // 查找第一个完整的CHTL语句开始
        // 简化实现：查找第一个非空白字符开始的行
        int pos = 0;
        while (pos < content.length()) {
            if (content.charAt(pos) == '\n') {
                pos++;
                // 跳过空白
                while (pos < content.length() && Character.isWhitespace(content.charAt(pos))) {
                    pos++;
                }
                return pos;
            }
            pos++;
        }
        return 0;
    }
    
    /**
     * 查找语句结束位置
     */
    private int findStatementEnd(String content) {
        // 查找最后一个完整的CHTL语句结束
        // 简化实现：查找最后一个闭合大括号或分号
        int lastComplete = content.length();
        int braceCount = 0;
        
        for (int i = content.length() - 1; i >= 0; i--) {
            char ch = content.charAt(i);
            if (ch == '}') {
                braceCount++;
            } else if (ch == '{') {
                braceCount--;
                if (braceCount == 0 && i > 0) {
                    // 找到完整的块结束
                    return i + 1;
                }
            } else if (ch == ';' && braceCount == 0) {
                // 找到语句结束
                return i + 1;
            }
        }
        
        return lastComplete;
    }
    
    /**
     * 使用内存映射处理超大文件
     */
    public void processHugeFile(Path filePath, Consumer<Fragment> fragmentConsumer) throws IOException {
        try (RandomAccessFile file = new RandomAccessFile(filePath.toFile(), "r");
             FileChannel channel = file.getChannel()) {
            
            long fileSize = channel.size();
            long position = 0;
            
            while (position < fileSize) {
                long remainingSize = fileSize - position;
                long mappingSize = Math.min(remainingSize, Integer.MAX_VALUE);
                
                ByteBuffer buffer = channel.map(FileChannel.MapMode.READ_ONLY, position, mappingSize);
                
                // 处理映射的内容
                processBuffer(buffer, filePath.toString(), fragmentConsumer);
                
                position += mappingSize;
            }
        }
    }
    
    /**
     * 处理缓冲区内容
     */
    private void processBuffer(ByteBuffer buffer, String filePath, Consumer<Fragment> consumer) {
        // 使用状态机逐字符处理
        StreamingScanner scanner = new StreamingScanner(filePath);
        
        while (buffer.hasRemaining()) {
            char ch = (char) buffer.get();
            Optional<Fragment> fragment = scanner.processChar(ch);
            fragment.ifPresent(consumer);
        }
        
        // 处理最后的片段
        scanner.finish().ifPresent(consumer);
    }
    
    /**
     * 流式扫描器
     */
    private static class StreamingScanner {
        private final String filePath;
        private final StringBuilder buffer = new StringBuilder();
        private FragmentType currentType = null;
        private int startLine = 1;
        private int currentLine = 1;
        private int startColumn = 1;
        private int currentColumn = 1;
        
        StreamingScanner(String filePath) {
            this.filePath = filePath;
        }
        
        Optional<Fragment> processChar(char ch) {
            // 更新位置
            if (ch == '\n') {
                currentLine++;
                currentColumn = 1;
            } else {
                currentColumn++;
            }
            
            // 简化的片段检测逻辑
            buffer.append(ch);
            
            // 检查是否形成完整片段
            if (isFragmentComplete()) {
                Fragment fragment = createFragment();
                resetBuffer();
                return Optional.of(fragment);
            }
            
            return Optional.empty();
        }
        
        Optional<Fragment> finish() {
            if (buffer.length() > 0) {
                return Optional.of(createFragment());
            }
            return Optional.empty();
        }
        
        private boolean isFragmentComplete() {
            // 简化实现
            String content = buffer.toString();
            return content.endsWith("}\n") || content.endsWith(";\n");
        }
        
        private Fragment createFragment() {
            Fragment fragment = new Fragment();
            fragment.setType(detectType(buffer.toString()));
            fragment.setContent(buffer.toString());
            fragment.setSourcePath(filePath);
            fragment.setStartLine(startLine);
            fragment.setEndLine(currentLine);
            return fragment;
        }
        
        private FragmentType detectType(String content) {
            if (content.contains("style {")) return FragmentType.CSS;
            if (content.contains("script {")) return FragmentType.JAVASCRIPT;
            if (content.contains("{{") || content.contains("->")) return FragmentType.CHTL_JS;
            return FragmentType.CHTL;
        }
        
        private void resetBuffer() {
            buffer.setLength(0);
            startLine = currentLine;
            startColumn = currentColumn;
        }
    }
    
    /**
     * 优化的字符串构建器（减少内存分配）
     */
    public static class OptimizedStringBuilder {
        private char[] buffer;
        private int position;
        
        public OptimizedStringBuilder(int initialCapacity) {
            this.buffer = new char[initialCapacity];
            this.position = 0;
        }
        
        public void append(char c) {
            ensureCapacity(1);
            buffer[position++] = c;
        }
        
        public void append(String str) {
            ensureCapacity(str.length());
            str.getChars(0, str.length(), buffer, position);
            position += str.length();
        }
        
        private void ensureCapacity(int additional) {
            if (position + additional > buffer.length) {
                int newCapacity = Math.max(buffer.length * 2, position + additional);
                buffer = Arrays.copyOf(buffer, newCapacity);
            }
        }
        
        public String toString() {
            return new String(buffer, 0, position);
        }
        
        public void clear() {
            position = 0;
        }
    }
    
    // 辅助方法
    private CompilationResult processContent(String content, String filePath, CompilationContext context) {
        // 简化实现
        CompilationResult result = new CompilationResult();
        result.fragments = scanContent(content, filePath, context);
        return result;
    }
    
    private List<Fragment> scanContent(String content, String filePath, CompilationContext context) {
        // 简化实现
        return new ArrayList<>();
    }
    
    private void mergeChunkResults(List<ChunkResult> chunks, CompilationResult result) {
        for (ChunkResult chunk : chunks) {
            result.fragments.addAll(chunk.fragments);
        }
    }
    
    /**
     * 编译结果
     */
    public static class CompilationResult {
        public List<Fragment> fragments = new ArrayList<>();
        public List<String> errors = new ArrayList<>();
        public List<String> warnings = new ArrayList<>();
    }
    
    /**
     * 块处理结果
     */
    private static class ChunkResult {
        List<Fragment> fragments;
        long startOffset;
        long endOffset;
    }
}