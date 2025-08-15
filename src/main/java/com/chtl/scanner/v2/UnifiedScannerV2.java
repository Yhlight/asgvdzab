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
                // 局部script：特殊存在，支持CHTL + CHTL JS + JS
                return processLocalScript(block);
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
     * 处理局部script块（支持CHTL + CHTL JS + JS）
     */
    private List<CodeFragment> processLocalScript(BlockScanner.RawBlock block) {
        // 局部script是特殊的，需要识别所有CHTL语法
        String content = block.getContent();
        List<CodeFragment> fragments = new ArrayList<>();
        
        // 提取script标签部分
        int scriptStart = content.indexOf('{');
        if (scriptStart != -1) {
            // script { 部分作为CHTL
            fragments.add(new CodeFragment(
                FragmentType.CHTL,
                content.substring(0, scriptStart + 1),
                block.getStartPosition(),
                scriptStart + 1
            ));
            
            // 处理内容
            int contentEnd = content.lastIndexOf('}');
            if (contentEnd > scriptStart + 1) {
                String scriptContent = content.substring(scriptStart + 1, contentEnd);
                fragments.addAll(processLocalScriptContent(scriptContent, block.getStartPosition() + scriptStart + 1));
            }
            
            // 结束的}
            if (contentEnd != -1 && contentEnd < content.length()) {
                fragments.add(new CodeFragment(
                    FragmentType.CHTL,
                    content.substring(contentEnd),
                    block.getStartPosition() + contentEnd,
                    content.length() - contentEnd
                ));
            }
        } else {
            // 没有大括号，整体作为CHTL处理
            fragments.add(new CodeFragment(
                FragmentType.CHTL,
                content,
                block.getStartPosition(),
                content.length()
            ));
        }
        
        return fragments;
    }
    
    /**
     * 处理局部script内容（支持完整CHTL语法）
     */
    private List<CodeFragment> processLocalScriptContent(String content, int offset) {
        List<CodeFragment> fragments = new ArrayList<>();
        int pos = 0;
        
        while (pos < content.length()) {
            // 检查CHTL特殊语法
            if (checkCHTLSyntax(content, pos)) {
                // 找到CHTL语法的结束
                int chtlEnd = findCHTLSyntaxEnd(content, pos);
                fragments.add(new CodeFragment(
                    FragmentType.CHTL,
                    content.substring(pos, chtlEnd),
                    offset + pos,
                    chtlEnd - pos
                ));
                pos = chtlEnd;
                continue;
            }
            
            // 否则按CHTL JS处理
            List<CodeFragment> jsFragments = processCHTLJSContent(
                content.substring(pos), 
                offset + pos
            );
            
            // 合并片段
            for (CodeFragment frag : jsFragments) {
                fragments.add(frag);
            }
            break;
        }
        
        return fragments;
    }
    
    /**
     * 检查是否是CHTL特有语法（不是JS或CHTL JS）
     */
    private boolean checkCHTLSyntax(String content, int pos) {
        // 检查CHTL特有的语法标记
        String[] chtlMarkers = {
            "[Template]", "[Custom]", "[Configuration]", 
            "[Import]", "[Namespace]", "[Constraint]",
            "text ", "slot ", "style {", 
            "@Element", "@Style", "@Var"
        };
        
        for (String marker : chtlMarkers) {
            if (pos + marker.length() <= content.length() &&
                content.substring(pos).startsWith(marker)) {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * 找到CHTL语法结束位置
     */
    private int findCHTLSyntaxEnd(String content, int start) {
        // 简化处理：找到下一个分号或大括号结束
        int pos = start;
        int braceDepth = 0;
        
        while (pos < content.length()) {
            char c = content.charAt(pos);
            
            if (c == '{') {
                braceDepth++;
            } else if (c == '}') {
                if (braceDepth > 0) {
                    braceDepth--;
                    if (braceDepth == 0) {
                        return pos + 1;
                    }
                } else {
                    return pos;
                }
            } else if (c == ';' && braceDepth == 0) {
                return pos + 1;
            } else if (c == '\n' && braceDepth == 0) {
                // 某些CHTL语法以换行结束
                return pos + 1;
            }
            
            pos++;
        }
        
        return content.length();
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
     * 处理script内容中的CHTL JS语法 - 最小单元切割
     */
    private List<CodeFragment> processCHTLJSContent(String content, int offset) {
        List<CodeFragment> fragments = new ArrayList<>();
        int pos = 0;
        int lastPos = 0;
        
        // 字符级别扫描，确保最小单元切割
        while (pos < content.length()) {
            char c = content.charAt(pos);
            
            // 检查原始嵌入 - 可以在任意位置
            if (pos + 8 <= content.length() && content.substring(pos, pos + 8).equals("[Origin]")) {
                // 添加之前的内容
                if (lastPos < pos) {
                    addJSFragment(fragments, content, lastPos, pos, offset);
                }
                
                // 处理原始嵌入
                int originEnd = findOriginEnd(content, pos);
                fragments.add(new CodeFragment(
                    FragmentType.CHTL,
                    content.substring(pos, originEnd),
                    offset + pos,
                    originEnd - pos
                ));
                
                lastPos = originEnd;
                pos = originEnd;
                continue;
            }
            
            // 检查增强选择器 {{
            if (c == '{' && pos + 1 < content.length() && content.charAt(pos + 1) == '{') {
                // 添加之前的JS
                if (lastPos < pos) {
                    addJSFragment(fragments, content, lastPos, pos, offset);
                }
                
                // 找到选择器结束
                int selectorEnd = findEnhancedSelectorEnd(content, pos);
                fragments.add(new CodeFragment(
                    FragmentType.CHTL_JS,
                    content.substring(pos, selectorEnd),
                    offset + pos,
                    selectorEnd - pos
                ));
                
                lastPos = selectorEnd;
                pos = selectorEnd;
                continue;
            }
            
            // 检查箭头操作符 ->
            if (c == '-' && pos + 1 < content.length() && content.charAt(pos + 1) == '>') {
                // 添加之前的JS
                if (lastPos < pos) {
                    addJSFragment(fragments, content, lastPos, pos, offset);
                }
                
                // 添加箭头操作符
                fragments.add(new CodeFragment(
                    FragmentType.CHTL_JS,
                    "->",
                    offset + pos,
                    2
                ));
                
                lastPos = pos + 2;
                pos = pos + 2;
                continue;
            }
            
            // 检查CHTL JS函数调用
            if (Character.isLetter(c) || c == '.') {
                String word = extractWord(content, pos);
                
                // 检查是否是CHTL JS函数
                if (isCHTLJSFunction(word, content, pos)) {
                    // 添加之前的JS
                    if (lastPos < pos) {
                        addJSFragment(fragments, content, lastPos, pos, offset);
                    }
                    
                    // 找到函数调用结束
                    int funcEnd = findFunctionCallEnd(content, pos);
                    fragments.add(new CodeFragment(
                        FragmentType.CHTL_JS,
                        content.substring(pos, funcEnd),
                        offset + pos,
                        funcEnd - pos
                    ));
                    
                    lastPos = funcEnd;
                    pos = funcEnd;
                    continue;
                }
            }
            
            // 跳过字符串
            if (c == '"' || c == '\'' || c == '`') {
                pos = skipString(content, pos);
                continue;
            }
            
            // 跳过注释
            if (c == '/' && pos + 1 < content.length()) {
                char next = content.charAt(pos + 1);
                if (next == '/') {
                    // 单行注释
                    pos = skipToLineEnd(content, pos);
                    continue;
                } else if (next == '*') {
                    // 多行注释
                    pos = skipBlockComment(content, pos);
                    continue;
                }
            }
            
            pos++;
        }
        
        // 添加剩余的JS
        if (lastPos < content.length()) {
            addJSFragment(fragments, content, lastPos, content.length(), offset);
        }
        
        return fragments;
    }
    
    /**
     * 添加JS片段（可能包含原始嵌入）
     */
    private void addJSFragment(List<CodeFragment> fragments, String content, 
                               int start, int end, int offset) {
        if (start >= end) return;
        
        String jsContent = content.substring(start, end);
        
        // 再次检查是否包含原始嵌入
        if (jsContent.contains("[Origin]")) {
            // 递归处理可能包含的原始嵌入
            List<CodeFragment> subFragments = processJSWithOrigin(jsContent, offset + start);
            fragments.addAll(subFragments);
        } else {
            // 纯JS片段
            fragments.add(new CodeFragment(
                FragmentType.JS,
                jsContent,
                offset + start,
                end - start
            ));
        }
    }
    
    /**
     * 处理可能包含原始嵌入的JS代码
     */
    private List<CodeFragment> processJSWithOrigin(String content, int offset) {
        List<CodeFragment> fragments = new ArrayList<>();
        int pos = 0;
        
        while (pos < content.length()) {
            int originStart = content.indexOf("[Origin]", pos);
            if (originStart == -1) {
                // 剩余部分是纯JS
                if (pos < content.length()) {
                    fragments.add(new CodeFragment(
                        FragmentType.JS,
                        content.substring(pos),
                        offset + pos,
                        content.length() - pos
                    ));
                }
                break;
            }
            
            // 添加原始嵌入之前的JS
            if (pos < originStart) {
                fragments.add(new CodeFragment(
                    FragmentType.JS,
                    content.substring(pos, originStart),
                    offset + pos,
                    originStart - pos
                ));
            }
            
            // 处理原始嵌入
            int originEnd = findOriginEnd(content, originStart);
            fragments.add(new CodeFragment(
                FragmentType.CHTL,
                content.substring(originStart, originEnd),
                offset + originStart,
                originEnd - originStart
            ));
            
            pos = originEnd;
        }
        
        return fragments;
    }
    
    /**
     * 提取单词
     */
    private String extractWord(String content, int start) {
        int pos = start;
        while (pos < content.length() && 
               (Character.isLetterOrDigit(content.charAt(pos)) || 
                content.charAt(pos) == '.' || 
                content.charAt(pos) == '_')) {
            pos++;
        }
        return content.substring(start, pos);
    }
    
    /**
     * 检查是否是CHTL JS函数
     */
    private boolean isCHTLJSFunction(String word, String content, int pos) {
        // 检查是否是 .listen( 或 .delegate(
        if (word.endsWith(".listen") || word.endsWith(".delegate")) {
            int afterWord = pos + word.length();
            return afterWord < content.length() && content.charAt(afterWord) == '(';
        }
        
        // 检查是否是 animate(
        if (word.equals("animate")) {
            int afterWord = pos + word.length();
            return afterWord < content.length() && content.charAt(afterWord) == '(';
        }
        
        return false;
    }
    
    /**
     * 查找增强选择器结束位置
     */
    private int findEnhancedSelectorEnd(String content, int start) {
        int pos = start + 2; // 跳过 {{
        int braceDepth = 2;
        
        while (pos < content.length() && braceDepth > 0) {
            char c = content.charAt(pos);
            if (c == '{') {
                braceDepth++;
            } else if (c == '}') {
                braceDepth--;
                if (braceDepth == 0) {
                    return pos + 1;
                }
            } else if (c == '"' || c == '\'' || c == '`') {
                pos = skipString(content, pos);
                continue;
            }
            pos++;
        }
        
        return pos;
    }
    
    /**
     * 跳过到行尾
     */
    private int skipToLineEnd(String content, int start) {
        int pos = start;
        while (pos < content.length() && content.charAt(pos) != '\n') {
            pos++;
        }
        return pos + 1;
    }
    
    /**
     * 跳过块注释
     */
    private int skipBlockComment(String content, int start) {
        int pos = start + 2; // 跳过 /*
        while (pos + 1 < content.length()) {
            if (content.charAt(pos) == '*' && content.charAt(pos + 1) == '/') {
                return pos + 2;
            }
            pos++;
        }
        return content.length();
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