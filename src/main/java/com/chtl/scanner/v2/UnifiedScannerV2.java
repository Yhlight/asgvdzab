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
        
        // Script块处理器 - 不处理CHTL语法
        map.put(BlockScanner.BlockType.SCRIPT_GLOBAL, new BlockProcessor() {
            @Override
            public List<CodeFragment> process(BlockScanner.RawBlock block) {
                return Collections.singletonList(new CodeFragment(
                    FragmentType.JS,
                    block.getContent(),
                    block.getStartPosition(),
                    block.getContent().length()
                ));
            }
        });
        
        map.put(BlockScanner.BlockType.SCRIPT_LOCAL, new BlockProcessor() {
            @Override
            public List<CodeFragment> process(BlockScanner.RawBlock block) {
                // 局部script可能包含CHTL JS语法
                return processCHTLJS(block);
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
     * 处理CHTL JS（局部script）
     */
    private List<CodeFragment> processCHTLJS(BlockScanner.RawBlock block) {
        // 检查是否包含CHTL JS特殊语法
        String content = block.getContent();
        
        // 如果包含{{}}，需要特殊处理
        if (content.contains("{{") && content.contains("}}")) {
            return processCHTLJSEnhancedSelectors(block);
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