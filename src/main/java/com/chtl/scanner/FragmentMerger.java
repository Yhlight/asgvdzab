package com.chtl.scanner;

import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import java.util.*;

/**
 * 片段合并器
 * 用于优化扫描器产生的片段，合并相邻的同类型片段
 * 避免过度细分的片段影响性能和可读性
 */
public class FragmentMerger {
    
    /**
     * 合并策略配置
     */
    public static class MergeConfig {
        private boolean mergeAdjacentSameType = true;  // 合并相邻同类型片段
        private boolean mergeShortFragments = true;     // 合并过短的片段
        private int shortFragmentThreshold = 5;         // 短片段阈值
        private boolean preserveEnhancedSelectors = true; // 保留增强选择器边界
        private boolean preserveFunctionBoundaries = true; // 保留函数边界
        
        // Getters and setters
        public boolean isMergeAdjacentSameType() {
            return mergeAdjacentSameType;
        }
        
        public void setMergeAdjacentSameType(boolean mergeAdjacentSameType) {
            this.mergeAdjacentSameType = mergeAdjacentSameType;
        }
        
        public boolean isMergeShortFragments() {
            return mergeShortFragments;
        }
        
        public void setMergeShortFragments(boolean mergeShortFragments) {
            this.mergeShortFragments = mergeShortFragments;
        }
        
        public int getShortFragmentThreshold() {
            return shortFragmentThreshold;
        }
        
        public void setShortFragmentThreshold(int shortFragmentThreshold) {
            this.shortFragmentThreshold = shortFragmentThreshold;
        }
    }
    
    private final MergeConfig config;
    
    public FragmentMerger() {
        this.config = new MergeConfig();
    }
    
    public FragmentMerger(MergeConfig config) {
        this.config = config;
    }
    
    /**
     * 合并片段列表
     */
    public List<CodeFragment> merge(List<CodeFragment> fragments) {
        if (fragments == null || fragments.isEmpty()) {
            return fragments;
        }
        
        List<CodeFragment> merged = new ArrayList<>();
        CodeFragment current = null;
        
        for (CodeFragment fragment : fragments) {
            if (current == null) {
                current = fragment;
            } else if (shouldMerge(current, fragment)) {
                // 合并片段
                current = mergeFragments(current, fragment);
            } else {
                // 添加当前片段，开始新片段
                merged.add(current);
                current = fragment;
            }
        }
        
        // 添加最后一个片段
        if (current != null) {
            merged.add(current);
        }
        
        // 后处理：处理短片段
        if (config.mergeShortFragments) {
            merged = mergeShortFragments(merged);
        }
        
        return merged;
    }
    
    /**
     * 判断是否应该合并两个片段
     */
    private boolean shouldMerge(CodeFragment first, CodeFragment second) {
        // 不同类型不合并
        if (first.getType() != second.getType()) {
            return false;
        }
        
        // 检查是否允许合并相邻同类型片段
        if (!config.mergeAdjacentSameType) {
            return false;
        }
        
        // 保留增强选择器边界
        if (config.preserveEnhancedSelectors) {
            if (containsEnhancedSelector(first) || containsEnhancedSelector(second)) {
                return false;
            }
        }
        
        // 保留函数边界
        if (config.preserveFunctionBoundaries) {
            if (containsFunctionBoundary(first) || containsFunctionBoundary(second)) {
                return false;
            }
        }
        
        // 检查片段是否相邻
        int firstEnd = first.getStartPosition() + first.getLength();
        return firstEnd == second.getStartPosition();
    }
    
    /**
     * 合并两个片段
     */
    private CodeFragment mergeFragments(CodeFragment first, CodeFragment second) {
        String mergedContent = first.getContent() + second.getContent();
        int totalLength = first.getLength() + second.getLength();
        
        CodeFragment merged = new CodeFragment(
            first.getType(),
            mergedContent,
            first.getStartPosition(),
            totalLength
        );
        
        // 复制第一个片段的元数据
        merged.setLine(first.getLine());
        merged.setColumn(first.getColumn());
        
        // 合并元数据 - 暂时跳过，因为CodeFragment的元数据API不支持批量操作
        
        return merged;
    }
    
    /**
     * 合并短片段
     */
    private List<CodeFragment> mergeShortFragments(List<CodeFragment> fragments) {
        List<CodeFragment> result = new ArrayList<>();
        
        for (int i = 0; i < fragments.size(); i++) {
            CodeFragment current = fragments.get(i);
            
            // 如果是短片段，尝试与前后片段合并
            if (current.getLength() < config.shortFragmentThreshold) {
                // 优先与前一个片段合并
                if (i > 0 && canMergeShort(fragments.get(i-1), current)) {
                    CodeFragment prev = result.remove(result.size() - 1);
                    result.add(mergeFragments(prev, current));
                    continue;
                }
                
                // 尝试与后一个片段合并
                if (i < fragments.size() - 1 && canMergeShort(current, fragments.get(i+1))) {
                    i++; // 跳过下一个片段
                    result.add(mergeFragments(current, fragments.get(i)));
                    continue;
                }
            }
            
            result.add(current);
        }
        
        return result;
    }
    
    /**
     * 检查短片段是否可以合并
     */
    private boolean canMergeShort(CodeFragment first, CodeFragment second) {
        // 只合并相邻的片段
        int firstEnd = first.getStartPosition() + first.getLength();
        if (firstEnd != second.getStartPosition()) {
            return false;
        }
        
        // 如果其中一个是标点符号或空白，可以合并
        if (isPunctuationOrWhitespace(first) || isPunctuationOrWhitespace(second)) {
            return true;
        }
        
        // 同类型可以合并
        return first.getType() == second.getType();
    }
    
    /**
     * 检查是否包含增强选择器
     */
    private boolean containsEnhancedSelector(CodeFragment fragment) {
        String content = fragment.getContent();
        return content.contains("{{") || content.contains("}}");
    }
    
    /**
     * 检查是否包含函数边界
     */
    private boolean containsFunctionBoundary(CodeFragment fragment) {
        String content = fragment.getContent().trim();
        return content.equals("function") || 
               content.startsWith("function(") ||
               content.contains("function ") ||
               (content.equals("{") && fragment.getType() == FragmentType.JS) ||
               (content.equals("}") && fragment.getType() == FragmentType.JS);
    }
    
    /**
     * 检查是否是标点符号或空白
     */
    private boolean isPunctuationOrWhitespace(CodeFragment fragment) {
        String content = fragment.getContent().trim();
        if (content.isEmpty()) {
            return true;
        }
        
        // 检查是否全是标点符号
        return content.matches("[\\p{Punct}\\s]+");
    }
    
    /**
     * 获取合并统计信息
     */
    public static class MergeStats {
        private int originalCount;
        private int mergedCount;
        private Map<FragmentType, Integer> typeCountBefore;
        private Map<FragmentType, Integer> typeCountAfter;
        
        public MergeStats(List<CodeFragment> original, List<CodeFragment> merged) {
            this.originalCount = original.size();
            this.mergedCount = merged.size();
            this.typeCountBefore = countByType(original);
            this.typeCountAfter = countByType(merged);
        }
        
        private Map<FragmentType, Integer> countByType(List<CodeFragment> fragments) {
            Map<FragmentType, Integer> counts = new HashMap<>();
            for (CodeFragment fragment : fragments) {
                counts.merge(fragment.getType(), 1, Integer::sum);
            }
            return counts;
        }
        
        public int getReductionCount() {
            return originalCount - mergedCount;
        }
        
        public double getReductionPercentage() {
            if (originalCount == 0) return 0;
            return (double) getReductionCount() / originalCount * 100;
        }
        
        @Override
        public String toString() {
            StringBuilder sb = new StringBuilder();
            sb.append("Merge Statistics:\n");
            sb.append("  Original fragments: ").append(originalCount).append("\n");
            sb.append("  Merged fragments: ").append(mergedCount).append("\n");
            sb.append("  Reduction: ").append(getReductionCount())
              .append(" (").append(String.format("%.1f%%", getReductionPercentage())).append(")\n");
            
            sb.append("  Type distribution before:\n");
            for (Map.Entry<FragmentType, Integer> entry : typeCountBefore.entrySet()) {
                sb.append("    ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
            }
            
            sb.append("  Type distribution after:\n");
            for (Map.Entry<FragmentType, Integer> entry : typeCountAfter.entrySet()) {
                sb.append("    ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
            }
            
            return sb.toString();
        }
    }
}