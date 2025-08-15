package com.chtl.scanner;

import java.util.*;

/**
 * 语言上下文管理器
 * 管理CHTL、CHTL JS和JavaScript之间的上下文切换
 * 支持语言嵌套和精确的边界识别
 */
public class LanguageContextManager {
    
    /**
     * 语言类型
     */
    public enum Language {
        CHTL,          // CHTL主语言
        CHTL_JS,       // CHTL JS扩展语法
        JAVASCRIPT,    // 标准JavaScript
        CSS,           // CSS样式
        HTML           // HTML内容
    }
    
    /**
     * 上下文类型
     */
    public enum ContextType {
        GLOBAL,                  // 全局上下文
        LOCAL_STYLE,            // 局部样式块 style{}
        LOCAL_SCRIPT,           // 局部脚本块 script{}
        TEMPLATE_DEFINITION,    // 模板定义
        CUSTOM_DEFINITION,      // 自定义元素/样式
        ENHANCED_SELECTOR,      // 增强选择器 {{}}
        JS_FUNCTION_BODY,       // JS函数体
        JS_STRING_LITERAL,      // JS字符串字面量
        CSS_RULE_BODY,          // CSS规则体
        ANIMATE_CONFIG          // 动画配置
    }
    
    /**
     * 语言上下文
     */
    public static class LanguageContext {
        private final Language language;
        private final ContextType contextType;
        private final int startPosition;
        private final int startLine;
        private final int startColumn;
        private Map<String, Object> attributes;
        
        public LanguageContext(Language language, ContextType contextType, 
                             int startPosition, int startLine, int startColumn) {
            this.language = language;
            this.contextType = contextType;
            this.startPosition = startPosition;
            this.startLine = startLine;
            this.startColumn = startColumn;
            this.attributes = new HashMap<>();
        }
        
        public Language getLanguage() {
            return language;
        }
        
        public ContextType getContextType() {
            return contextType;
        }
        
        public int getStartPosition() {
            return startPosition;
        }
        
        public void setAttribute(String key, Object value) {
            attributes.put(key, value);
        }
        
        public Object getAttribute(String key) {
            return attributes.get(key);
        }
        
        public boolean hasAttribute(String key) {
            return attributes.containsKey(key);
        }
    }
    
    private Stack<LanguageContext> contextStack;
    private LanguageContext currentContext;
    private Set<String> autoAddedClasses;
    private Set<String> autoAddedIds;
    private String currentElementScope;  // 当前元素作用域
    
    public LanguageContextManager() {
        this.contextStack = new Stack<>();
        this.autoAddedClasses = new HashSet<>();
        this.autoAddedIds = new HashSet<>();
        // 初始上下文为CHTL全局
        this.currentContext = new LanguageContext(Language.CHTL, ContextType.GLOBAL, 0, 1, 1);
    }
    
    /**
     * 进入新的语言上下文
     */
    public void enterContext(Language language, ContextType contextType, 
                           int position, int line, int column) {
        contextStack.push(currentContext);
        currentContext = new LanguageContext(language, contextType, position, line, column);
        
        // 处理特殊上下文的初始化
        switch (contextType) {
            case LOCAL_STYLE:
            case LOCAL_SCRIPT:
                // 设置当前元素作用域
                currentContext.setAttribute("elementScope", currentElementScope);
                break;
                
            case ENHANCED_SELECTOR:
                // 记录是否在局部脚本块中
                if (isInLocalScript()) {
                    currentContext.setAttribute("inLocalScript", true);
                }
                break;
        }
    }
    
    /**
     * 退出当前语言上下文
     */
    public void exitContext() {
        if (!contextStack.isEmpty()) {
            currentContext = contextStack.pop();
        }
    }
    
    /**
     * 获取当前语言
     */
    public Language getCurrentLanguage() {
        return currentContext.getLanguage();
    }
    
    /**
     * 获取当前上下文类型
     */
    public ContextType getCurrentContextType() {
        return currentContext.getContextType();
    }
    
    /**
     * 判断当前是否可以使用箭头函数
     */
    public boolean canUseArrowFunction() {
        // 只有在纯JavaScript上下文中才能使用箭头函数
        return currentContext.getLanguage() == Language.JAVASCRIPT &&
               (currentContext.getContextType() == ContextType.JS_FUNCTION_BODY ||
                currentContext.getContextType() == ContextType.GLOBAL);
    }
    
    /**
     * 判断当前是否在局部脚本块中
     */
    public boolean isInLocalScript() {
        LanguageContext ctx = currentContext;
        while (ctx != null) {
            if (ctx.getContextType() == ContextType.LOCAL_SCRIPT) {
                return true;
            }
            // 检查栈中的上下文
            for (LanguageContext stackCtx : contextStack) {
                if (stackCtx.getContextType() == ContextType.LOCAL_SCRIPT) {
                    return true;
                }
            }
            break;
        }
        return false;
    }
    
    /**
     * 判断当前是否在局部样式块中
     */
    public boolean isInLocalStyle() {
        return currentContext.getContextType() == ContextType.LOCAL_STYLE;
    }
    
    /**
     * 记录自动添加的类名
     */
    public void recordAutoAddedClass(String className) {
        autoAddedClasses.add(className);
    }
    
    /**
     * 记录自动添加的ID
     */
    public void recordAutoAddedId(String id) {
        autoAddedIds.add(id);
    }
    
    /**
     * 检查是否已经有类名（显式或自动添加的）
     */
    public boolean hasClass(String className) {
        return autoAddedClasses.contains(className);
    }
    
    /**
     * 检查是否已经有ID（显式或自动添加的）
     */
    public boolean hasId(String id) {
        return autoAddedIds.contains(id);
    }
    
    /**
     * 设置当前元素作用域
     */
    public void setCurrentElementScope(String elementName) {
        this.currentElementScope = elementName;
    }
    
    /**
     * 获取当前元素作用域
     */
    public String getCurrentElementScope() {
        return currentElementScope;
    }
    
    /**
     * 判断是否应该自动添加类/ID
     * 用于{{&}}选择器
     */
    public boolean shouldAutoAddForSelector(String selector) {
        // 如果不在局部脚本块中，不自动添加
        if (!isInLocalScript()) {
            return false;
        }
        
        // 如果选择器以.开头（类选择器）
        if (selector.startsWith(".")) {
            String className = selector.substring(1);
            return !hasClass(className);
        }
        
        // 如果选择器以#开头（ID选择器）
        if (selector.startsWith("#")) {
            String id = selector.substring(1);
            return !hasId(id);
        }
        
        // 对于标签名选择器，检查是否需要添加ID
        // ID优先原则
        return !hasId(selector) && !hasClass(selector);
    }
    
    /**
     * 获取用于调试的上下文栈信息
     */
    public String getContextStackTrace() {
        StringBuilder sb = new StringBuilder();
        sb.append("当前上下文: ").append(currentContext.getLanguage())
          .append(" / ").append(currentContext.getContextType()).append("\n");
        
        if (!contextStack.isEmpty()) {
            sb.append("上下文栈:\n");
            for (int i = contextStack.size() - 1; i >= 0; i--) {
                LanguageContext ctx = contextStack.get(i);
                sb.append("  - ").append(ctx.getLanguage())
                  .append(" / ").append(ctx.getContextType())
                  .append(" (起始位置: ").append(ctx.getStartPosition()).append(")\n");
            }
        }
        
        return sb.toString();
    }
    
    /**
     * 重置上下文管理器
     */
    public void reset() {
        contextStack.clear();
        autoAddedClasses.clear();
        autoAddedIds.clear();
        currentElementScope = null;
        currentContext = new LanguageContext(Language.CHTL, ContextType.GLOBAL, 0, 1, 1);
    }
}