package com.chtl.scanner;

import java.util.*;

/**
 * 大括号平衡计数器
 * 用于精确跟踪各种语言上下文中的大括号平衡
 */
public class BraceBalancer {
    
    /**
     * 大括号类型
     */
    public enum BraceType {
        CURLY,      // {}
        SQUARE,     // []
        PARENTHESIS // ()
    }
    
    /**
     * 大括号上下文信息
     */
    public static class BraceContext {
        private final BraceType type;
        private final int position;
        private final int line;
        private final int column;
        private final LanguageContextManager.Language language;
        private final LanguageContextManager.ContextType contextType;
        
        public BraceContext(BraceType type, int position, int line, int column,
                           LanguageContextManager.Language language,
                           LanguageContextManager.ContextType contextType) {
            this.type = type;
            this.position = position;
            this.line = line;
            this.column = column;
            this.language = language;
            this.contextType = contextType;
        }
        
        public BraceType getType() {
            return type;
        }
        
        public int getPosition() {
            return position;
        }
        
        public LanguageContextManager.Language getLanguage() {
            return language;
        }
        
        public LanguageContextManager.ContextType getContextType() {
            return contextType;
        }
    }
    
    private Stack<BraceContext> braceStack;
    private Map<LanguageContextManager.Language, Integer> languageBraceCounts;
    private Map<LanguageContextManager.ContextType, Integer> contextBraceCounts;
    
    // 特殊标记计数
    private int enhancedSelectorDepth = 0;  // {{}} 嵌套深度
    private boolean inStringLiteral = false;
    private char stringDelimiter = '\0';
    
    public BraceBalancer() {
        this.braceStack = new Stack<>();
        this.languageBraceCounts = new HashMap<>();
        this.contextBraceCounts = new HashMap<>();
        initializeCounts();
    }
    
    private void initializeCounts() {
        for (LanguageContextManager.Language lang : LanguageContextManager.Language.values()) {
            languageBraceCounts.put(lang, 0);
        }
        for (LanguageContextManager.ContextType ctx : LanguageContextManager.ContextType.values()) {
            contextBraceCounts.put(ctx, 0);
        }
    }
    
    /**
     * 处理开始大括号
     */
    public void pushBrace(BraceType type, int position, int line, int column,
                         LanguageContextManager.Language language,
                         LanguageContextManager.ContextType contextType) {
        if (inStringLiteral) return;
        
        BraceContext context = new BraceContext(type, position, line, column, language, contextType);
        braceStack.push(context);
        
        if (type == BraceType.CURLY) {
            languageBraceCounts.merge(language, 1, Integer::sum);
            contextBraceCounts.merge(contextType, 1, Integer::sum);
        }
    }
    
    /**
     * 处理结束大括号
     */
    public BraceContext popBrace(BraceType type) {
        if (inStringLiteral || braceStack.isEmpty()) return null;
        
        // 查找匹配的开始大括号
        BraceContext matched = null;
        Stack<BraceContext> temp = new Stack<>();
        
        while (!braceStack.isEmpty()) {
            BraceContext context = braceStack.pop();
            if (context.getType() == type) {
                matched = context;
                break;
            }
            temp.push(context);
        }
        
        // 恢复临时弹出的大括号
        while (!temp.isEmpty()) {
            braceStack.push(temp.pop());
        }
        
        if (matched != null && type == BraceType.CURLY) {
            languageBraceCounts.merge(matched.getLanguage(), -1, Integer::sum);
            contextBraceCounts.merge(matched.getContextType(), -1, Integer::sum);
        }
        
        return matched;
    }
    
    /**
     * 处理增强选择器开始
     */
    public void enterEnhancedSelector() {
        enhancedSelectorDepth++;
    }
    
    /**
     * 处理增强选择器结束
     */
    public void exitEnhancedSelector() {
        if (enhancedSelectorDepth > 0) {
            enhancedSelectorDepth--;
        }
    }
    
    /**
     * 进入字符串字面量
     */
    public void enterString(char delimiter) {
        inStringLiteral = true;
        stringDelimiter = delimiter;
    }
    
    /**
     * 退出字符串字面量
     */
    public void exitString() {
        inStringLiteral = false;
        stringDelimiter = '\0';
    }
    
    /**
     * 检查是否在字符串中
     */
    public boolean isInString() {
        return inStringLiteral;
    }
    
    /**
     * 获取当前大括号深度
     */
    public int getCurrentDepth(BraceType type) {
        return (int) braceStack.stream()
            .filter(ctx -> ctx.getType() == type)
            .count();
    }
    
    /**
     * 获取特定语言的大括号计数
     */
    public int getLanguageBraceCount(LanguageContextManager.Language language) {
        return languageBraceCounts.getOrDefault(language, 0);
    }
    
    /**
     * 获取特定上下文的大括号计数
     */
    public int getContextBraceCount(LanguageContextManager.ContextType contextType) {
        return contextBraceCounts.getOrDefault(contextType, 0);
    }
    
    /**
     * 检查是否平衡
     */
    public boolean isBalanced() {
        return braceStack.isEmpty() && enhancedSelectorDepth == 0;
    }
    
    /**
     * 获取最近的未匹配大括号
     */
    public BraceContext getTopUnmatched() {
        return braceStack.isEmpty() ? null : braceStack.peek();
    }
    
    /**
     * 检查是否在增强选择器中
     */
    public boolean isInEnhancedSelector() {
        return enhancedSelectorDepth > 0;
    }
    
    /**
     * 重置状态
     */
    public void reset() {
        braceStack.clear();
        initializeCounts();
        enhancedSelectorDepth = 0;
        inStringLiteral = false;
        stringDelimiter = '\0';
    }
    
    /**
     * 获取调试信息
     */
    public String getDebugInfo() {
        StringBuilder info = new StringBuilder();
        info.append("Brace Stack Depth: ").append(braceStack.size()).append("\n");
        info.append("Enhanced Selector Depth: ").append(enhancedSelectorDepth).append("\n");
        info.append("In String: ").append(inStringLiteral).append("\n");
        
        info.append("\nLanguage Brace Counts:\n");
        for (Map.Entry<LanguageContextManager.Language, Integer> entry : languageBraceCounts.entrySet()) {
            if (entry.getValue() != 0) {
                info.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
            }
        }
        
        info.append("\nContext Brace Counts:\n");
        for (Map.Entry<LanguageContextManager.ContextType, Integer> entry : contextBraceCounts.entrySet()) {
            if (entry.getValue() != 0) {
                info.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
            }
        }
        
        return info.toString();
    }
}