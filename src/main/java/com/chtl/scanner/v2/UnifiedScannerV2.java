package com.chtl.scanner.v2;

import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.*;

/**
 * 统一扫描器 V2
 * 基于块扫描的新架构
 */
public class UnifiedScannerV2 {
    
    private final BlockScanner blockScanner;
    private final Map<BlockScanner.BlockType, BlockProcessor> processors;
    
    public UnifiedScannerV2() {
        this.blockScanner = new BlockScanner();
        this.processors = initializeProcessors();
    }
    
    /**
     * 扫描源代码，返回代码片段列表
     */
    public List<CodeFragment> scan(String source) {
        // 预处理：扫描并注册变量定义
        CHTLVariableRegistry.preloadCommonVariables();
        CHTLVariableRegistry.scanAndRegister(source);
        
        // 第一阶段：块扫描
        List<BlockScanner.RawBlock> blocks = blockScanner.scanBlocks(source);
        
        // 第二阶段：块处理
        List<CodeFragment> fragments = new ArrayList<>();
        for (BlockScanner.RawBlock block : blocks) {
            fragments.addAll(processBlock(block));
        }
        
        // 第三阶段：优化合并
        return optimizeFragments(fragments);
    }
    
    /**
     * 初始化块处理器
     */
    private Map<BlockScanner.BlockType, BlockProcessor> initializeProcessors() {
        Map<BlockScanner.BlockType, BlockProcessor> map = new HashMap<>();
        
        // Style块处理器
        map.put(BlockScanner.BlockType.STYLE_GLOBAL, new BlockProcessor() {
            @Override
            public List<CodeFragment> process(BlockScanner.RawBlock block) {
                StyleProcessor processor = new StyleProcessor(false);
                return processor.process(block.getContent());
            }
        });
        
        map.put(BlockScanner.BlockType.STYLE_LOCAL, new BlockProcessor() {
            @Override
            public List<CodeFragment> process(BlockScanner.RawBlock block) {
                StyleProcessor processor = new StyleProcessor(true);
                return processor.process(block.getContent());
            }
        });
        
        // Script块处理器
        map.put(BlockScanner.BlockType.SCRIPT_GLOBAL, new BlockProcessor() {
            @Override
            public List<CodeFragment> process(BlockScanner.RawBlock block) {
                // 全局script：支持CHTL JS语法 + 原始嵌入[Origin]
                return processGlobalScript(block);
            }
        });
        
        map.put(BlockScanner.BlockType.SCRIPT_LOCAL, new BlockProcessor() {
            @Override
            public List<CodeFragment> process(BlockScanner.RawBlock block) {
                // 局部script：作为CHTL的一部分，完全支持CHTL语法
                return Collections.singletonList(new CodeFragment(
                    FragmentType.CHTL,
                    block.getContent(),
                    block.getStartPosition(),
                    block.getContent().length()
                ));
            }
        });
        
        // CHTL元素处理器
        map.put(BlockScanner.BlockType.CHTL_ELEMENT, new BlockProcessor() {
            @Override
            public List<CodeFragment> process(BlockScanner.RawBlock block) {
                return Collections.singletonList(new CodeFragment(
                    FragmentType.CHTL,
                    block.getContent(),
                    block.getStartPosition(),
                    block.getContent().length()
                ));
            }
        });
        
        // 其他块类型的处理器
        map.put(BlockScanner.BlockType.TEMPLATE, block -> 
            Collections.singletonList(new CodeFragment(
                FragmentType.CHTL,
                block.getContent(),
                block.getStartPosition(),
                block.getContent().length()
            ))
        );
        
        return map;
    }
    
    /**
     * 处理单个块
     */
    private List<CodeFragment> processBlock(BlockScanner.RawBlock block) {
        BlockProcessor processor = processors.get(block.getType());
        if (processor != null) {
            return processor.process(block);
        }
        
        // 默认处理
        return Collections.singletonList(new CodeFragment(
            FragmentType.CHTL,
            block.getContent(),
            block.getStartPosition(),
            block.getContent().length()
        ));
    }
    
    /**
     * 处理全局script块（支持CHTL JS语法 + 原始嵌入）
     */
    private List<CodeFragment> processGlobalScript(BlockScanner.RawBlock block) {
        String content = block.getContent();
        List<CodeFragment> fragments = new ArrayList<>();
        
        // 检查是否包含原始嵌入
        if (content.contains("[Origin]")) {
            return processScriptWithOrigin(block);
        }
        
        // 否则处理CHTL JS语法
        return processCHTLJS(block);
    }
    
    /**
     * 处理包含原始嵌入的script块
     */
    private List<CodeFragment> processScriptWithOrigin(BlockScanner.RawBlock block) {
        List<CodeFragment> fragments = new ArrayList<>();
        String content = block.getContent();
        int position = 0;
        
        while (position < content.length()) {
            // 查找[Origin]块
            int originStart = content.indexOf("[Origin]", position);
            if (originStart == -1) {
                // 剩余部分作为CHTL JS处理
                String remaining = content.substring(position);
                if (!remaining.trim().isEmpty()) {
                    BlockScanner.RawBlock subBlock = new BlockScanner.RawBlock(
                        BlockScanner.BlockType.SCRIPT_GLOBAL,
                        remaining,
                        block.getStartPosition() + position,
                        block.getLine(),
                        block.getColumn()
                    );
                    fragments.addAll(processCHTLJS(subBlock));
                }
                break;
            }
            
            // 添加[Origin]之前的内容
            if (originStart > position) {
                String before = content.substring(position, originStart);
                BlockScanner.RawBlock subBlock = new BlockScanner.RawBlock(
                    BlockScanner.BlockType.SCRIPT_GLOBAL,
                    before,
                    block.getStartPosition() + position,
                    block.getLine(),
                    block.getColumn()
                );
                fragments.addAll(processCHTLJS(subBlock));
            }
            
            // 找到[Origin]块的结束
            int originEnd = findOriginEnd(content, originStart);
            
            // 添加[Origin]块作为CHTL
            fragments.add(new CodeFragment(
                FragmentType.CHTL,
                content.substring(originStart, originEnd),
                block.getStartPosition() + originStart,
                originEnd - originStart
            ));
            
            position = originEnd;
        }
        
        return fragments;
    }
    
    /**
     * 查找[Origin]块的结束位置
     */
    private int findOriginEnd(String content, int start) {
        // 跳过[Origin]标记
        int pos = start + 8;
        while (pos < content.length() && Character.isWhitespace(content.charAt(pos))) {
            pos++;
        }
        
        // 必须跟着@Script
        if (!content.startsWith("@Script", pos)) {
            return pos; // 错误的语法，返回当前位置
        }
        
        pos += 7; // 跳过@Script
        while (pos < content.length() && Character.isWhitespace(content.charAt(pos))) {
            pos++;
        }
        
        // 找到开始的{
        if (pos < content.length() && content.charAt(pos) == '{') {
            // 匹配大括号
            int braceDepth = 1;
            pos++;
            
            while (pos < content.length() && braceDepth > 0) {
                char c = content.charAt(pos);
                if (c == '{') {
                    braceDepth++;
                } else if (c == '}') {
                    braceDepth--;
                } else if (c == '"' || c == '\'') {
                    // 跳过字符串
                    pos = skipString(content, pos);
                }
                pos++;
            }
            
            return pos;
        }
        
        return pos;
    }
    
    /**
     * 处理CHTL JS（支持增强选择器等CHTL JS语法）
     */
    private List<CodeFragment> processCHTLJS(BlockScanner.RawBlock block) {
        String content = block.getContent();
        List<CodeFragment> fragments = new ArrayList<>();
        
        // 检查是否包含CHTL JS特殊语法
        boolean hasEnhancedSelector = content.contains("{{") && content.contains("}}");
        boolean hasListenOrDelegate = content.contains(".listen(") || content.contains(".delegate(");
        boolean hasAnimate = content.contains("animate(");
        boolean hasArrowOperator = content.contains("->");
        
        // 如果包含任何CHTL JS语法，进行精细处理
        if (hasEnhancedSelector || hasListenOrDelegate || hasAnimate || hasArrowOperator) {
            return processCHTLJSWithSyntax(block);
        }
        
        // 否则作为纯JS处理
        return Collections.singletonList(new CodeFragment(
            FragmentType.JS,
            block.getContent(),
            block.getStartPosition(),
            block.getContent().length()
        ));
    }
    
    /**
     * 处理包含CHTL JS语法的script块
     */
    private List<CodeFragment> processCHTLJSWithSyntax(BlockScanner.RawBlock block) {
        List<CodeFragment> fragments = new ArrayList<>();
        String content = block.getContent();
        int position = 0;
        
        // 提取script标记
        int scriptStart = content.indexOf('{');
        if (scriptStart != -1) {
            fragments.add(new CodeFragment(
                FragmentType.CHTL,
                content.substring(0, scriptStart + 1),
                block.getStartPosition(),
                scriptStart + 1
            ));
            position = scriptStart + 1;
        }
        
        // 处理script内容
        int contentEnd = content.lastIndexOf('}');
        if (contentEnd > position) {
            String scriptContent = content.substring(position, contentEnd);
            fragments.addAll(processCHTLJSContent(scriptContent, block.getStartPosition() + position));
            position = contentEnd;
        }
        
        // 添加结束的}
        if (position < content.length()) {
            fragments.add(new CodeFragment(
                FragmentType.CHTL,
                content.substring(position),
                block.getStartPosition() + position,
                content.length() - position
            ));
        }
        
        return fragments;
    }
    
    /**
     * 处理script内容中的CHTL JS语法
     */
    private List<CodeFragment> processCHTLJSContent(String content, int offset) {
        List<CodeFragment> fragments = new ArrayList<>();
        int lastPos = 0;
        int pos = 0;
        
        while (pos < content.length()) {
            // 查找CHTL JS语法
            int enhancedSelectorStart = content.indexOf("{{", pos);
            int listenStart = content.indexOf(".listen(", pos);
            int delegateStart = content.indexOf(".delegate(", pos);
            int animateStart = content.indexOf("animate(", pos);
            int arrowStart = content.indexOf("->", pos);
            
            // 找到最近的CHTL JS语法
            int nextSyntax = Integer.MAX_VALUE;
            String syntaxType = null;
            
            if (enhancedSelectorStart != -1 && enhancedSelectorStart < nextSyntax) {
                nextSyntax = enhancedSelectorStart;
                syntaxType = "selector";
            }
            if (listenStart != -1 && listenStart < nextSyntax) {
                nextSyntax = listenStart;
                syntaxType = "listen";
            }
            if (delegateStart != -1 && delegateStart < nextSyntax) {
                nextSyntax = delegateStart;
                syntaxType = "delegate";
            }
            if (animateStart != -1 && animateStart < nextSyntax) {
                nextSyntax = animateStart;
                syntaxType = "animate";
            }
            if (arrowStart != -1 && arrowStart < nextSyntax) {
                nextSyntax = arrowStart;
                syntaxType = "arrow";
            }
            
            if (syntaxType == null) {
                // 没有更多CHTL JS语法，添加剩余的JS代码
                if (lastPos < content.length()) {
                    fragments.add(new CodeFragment(
                        FragmentType.JS,
                        content.substring(lastPos),
                        offset + lastPos,
                        content.length() - lastPos
                    ));
                }
                break;
            }
            
            // 添加语法前的JS代码
            if (lastPos < nextSyntax) {
                fragments.add(new CodeFragment(
                    FragmentType.JS,
                    content.substring(lastPos, nextSyntax),
                    offset + lastPos,
                    nextSyntax - lastPos
                ));
            }
            
            // 处理CHTL JS语法
            int syntaxEnd = processSyntaxElement(content, nextSyntax, syntaxType, fragments, offset);
            
            lastPos = syntaxEnd;
            pos = syntaxEnd;
        }
        
        return fragments;
    }
    
    /**
     * 处理单个CHTL JS语法元素
     */
    private int processSyntaxElement(String content, int start, String type, 
                                    List<CodeFragment> fragments, int offset) {
        switch (type) {
            case "selector":
                // 处理增强选择器{{}}
                int selectorEnd = content.indexOf("}}", start + 2);
                if (selectorEnd != -1) {
                    fragments.add(new CodeFragment(
                        FragmentType.CHTL_JS,
                        content.substring(start, selectorEnd + 2),
                        offset + start,
                        selectorEnd + 2 - start
                    ));
                    return selectorEnd + 2;
                }
                break;
                
            case "listen":
            case "delegate":
            case "animate":
                // 处理函数调用
                int funcStart = type.equals("animate") ? start : start + 1;
                int funcEnd = findFunctionCallEnd(content, funcStart);
                if (funcEnd > content.length()) {
                    funcEnd = content.length();
                }
                fragments.add(new CodeFragment(
                    FragmentType.CHTL_JS,
                    content.substring(start, Math.min(funcEnd, content.length())),
                    offset + start,
                    Math.min(funcEnd, content.length()) - start
                ));
                return funcEnd;
                
            case "arrow":
                // 处理->操作符
                fragments.add(new CodeFragment(
                    FragmentType.CHTL_JS,
                    "->",
                    offset + start,
                    2
                ));
                return start + 2;
        }
        
        return start + 1;
    }
    
    /**
     * 查找函数调用的结束位置
     */
    private int findFunctionCallEnd(String content, int start) {
        // 找到函数名的结束
        int parenStart = content.indexOf('(', start);
        if (parenStart == -1) return start;
        
        // 匹配括号
        int parenDepth = 1;
        int pos = parenStart + 1;
        
        while (pos < content.length() && parenDepth > 0) {
            char c = content.charAt(pos);
            if (c == '(') {
                parenDepth++;
            } else if (c == ')') {
                parenDepth--;
            } else if (c == '"' || c == '\'') {
                // 跳过字符串
                pos = skipString(content, pos);
            }
            pos++;
        }
        
        return pos;
    }
    
    /**
     * 跳过字符串
     */
    private int skipString(String content, int start) {
        char quote = content.charAt(start);
        int pos = start + 1;
        
        while (pos < content.length()) {
            if (content.charAt(pos) == quote) {
                return pos;
            }
            if (content.charAt(pos) == '\\') {
                pos++; // 跳过转义字符
            }
            pos++;
        }
        
        return pos;
    }
    
    /**
     * 处理CHTL JS增强选择器
     */
    private List<CodeFragment> processCHTLJSEnhancedSelectors(BlockScanner.RawBlock block) {
        List<CodeFragment> fragments = new ArrayList<>();
        String content = block.getContent();
        int lastPos = 0;
        int searchPos = 0;
        
        while (searchPos < content.length()) {
            int start = content.indexOf("{{", searchPos);
            if (start == -1) break;
            
            int end = content.indexOf("}}", start + 2);
            if (end == -1) break;
            
            // 添加{{之前的JS片段
            if (lastPos < start) {
                fragments.add(new CodeFragment(
                    FragmentType.JS,
                    content.substring(lastPos, start),
                    block.getStartPosition() + lastPos,
                    start - lastPos
                ));
            }
            
            // 添加{{}}片段
            fragments.add(new CodeFragment(
                FragmentType.CHTL_JS,
                content.substring(start, end + 2),
                block.getStartPosition() + start,
                end + 2 - start
            ));
            
            lastPos = end + 2;
            searchPos = end + 2;
        }
        
        // 添加剩余的JS片段
        if (lastPos < content.length()) {
            fragments.add(new CodeFragment(
                FragmentType.JS,
                content.substring(lastPos),
                block.getStartPosition() + lastPos,
                content.length() - lastPos
            ));
        }
        
        return fragments;
    }
    
    /**
     * 优化片段列表
     */
    private List<CodeFragment> optimizeFragments(List<CodeFragment> fragments) {
        if (fragments.size() <= 1) {
            return fragments;
        }
        
        List<CodeFragment> optimized = new ArrayList<>();
        CodeFragment current = fragments.get(0);
        
        for (int i = 1; i < fragments.size(); i++) {
            CodeFragment next = fragments.get(i);
            
            // 合并相邻的相同类型片段
            if (current.getType() == next.getType() && 
                canMerge(current, next)) {
                current = merge(current, next);
            } else {
                optimized.add(current);
                current = next;
            }
        }
        
        optimized.add(current);
        return optimized;
    }
    
    /**
     * 检查是否可以合并
     */
    private boolean canMerge(CodeFragment first, CodeFragment second) {
        // 检查是否是连续的片段
        return first.getStartPosition() + first.getLength() == second.getStartPosition();
    }
    
    /**
     * 合并两个片段
     */
    private CodeFragment merge(CodeFragment first, CodeFragment second) {
        return new CodeFragment(
            first.getType(),
            first.getContent() + second.getContent(),
            first.getStartPosition(),
            first.getLength() + second.getLength()
        );
    }
    
    /**
     * 块处理器接口
     */
    private interface BlockProcessor {
        List<CodeFragment> process(BlockScanner.RawBlock block);
    }
}