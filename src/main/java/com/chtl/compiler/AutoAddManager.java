package com.chtl.compiler;

import com.chtl.scanner.LanguageContextManager;
import java.util.*;

/**
 * 自动添加管理器
 * 协调局部样式块和局部脚本块的自动类/ID添加规则
 */
public class AutoAddManager {
    
    /**
     * 元素作用域信息
     */
    public static class ElementScope {
        private String elementName;
        private Set<String> explicitClasses;     // 显式声明的类
        private Set<String> explicitIds;         // 显式声明的ID
        private Set<String> autoAddedClasses;    // 自动添加的类
        private Set<String> autoAddedIds;        // 自动添加的ID
        private boolean hasLocalStyle;           // 是否有局部样式块
        private boolean hasLocalScript;          // 是否有局部脚本块
        
        public ElementScope(String elementName) {
            this.elementName = elementName;
            this.explicitClasses = new HashSet<>();
            this.explicitIds = new HashSet<>();
            this.autoAddedClasses = new HashSet<>();
            this.autoAddedIds = new HashSet<>();
        }
        
        public boolean hasClass(String className) {
            return explicitClasses.contains(className) || autoAddedClasses.contains(className);
        }
        
        public boolean hasId(String id) {
            return explicitIds.contains(id) || autoAddedIds.contains(id);
        }
        
        public void addExplicitClass(String className) {
            explicitClasses.add(className);
        }
        
        public void addExplicitId(String id) {
            explicitIds.add(id);
        }
        
        public void addAutoClass(String className) {
            autoAddedClasses.add(className);
        }
        
        public void addAutoId(String id) {
            autoAddedIds.add(id);
        }
    }
    
    private Stack<ElementScope> scopeStack;
    private ElementScope currentScope;
    private LanguageContextManager contextManager;
    private int autoIdCounter;
    private String idPrefix;
    
    public AutoAddManager(LanguageContextManager contextManager) {
        this.scopeStack = new Stack<>();
        this.contextManager = contextManager;
        this.autoIdCounter = 0;
        this.idPrefix = "chtl-auto-";
    }
    
    /**
     * 进入新的元素作用域
     */
    public void enterElementScope(String elementName) {
        if (currentScope != null) {
            scopeStack.push(currentScope);
        }
        currentScope = new ElementScope(elementName);
        contextManager.setCurrentElementScope(elementName);
    }
    
    /**
     * 退出当前元素作用域
     */
    public void exitElementScope() {
        if (!scopeStack.isEmpty()) {
            currentScope = scopeStack.pop();
            if (currentScope != null) {
                contextManager.setCurrentElementScope(currentScope.elementName);
            }
        } else {
            currentScope = null;
            contextManager.setCurrentElementScope(null);
        }
    }
    
    /**
     * 处理局部样式块中的选择器
     * 根据规则自动添加类或ID
     */
    public String processStyleSelector(String selector) {
        if (currentScope == null) {
            return selector;
        }
        
        currentScope.hasLocalStyle = true;
        
        // 处理类选择器
        if (selector.startsWith(".")) {
            String className = selector.substring(1);
            if (!currentScope.hasClass(className)) {
                currentScope.addAutoClass(className);
                contextManager.recordAutoAddedClass(className);
            }
            return selector;
        }
        
        // 处理ID选择器
        if (selector.startsWith("#")) {
            String id = selector.substring(1);
            if (!currentScope.hasId(id)) {
                currentScope.addAutoId(id);
                contextManager.recordAutoAddedId(id);
            }
            return selector;
        }
        
        // 处理标签选择器（不需要自动添加）
        return selector;
    }
    
    /**
     * 处理局部脚本块中的{{&}}选择器
     * ID优先原则
     */
    public String processSelfReferenceSelector() {
        if (currentScope == null) {
            return generateAutoId(); // 默认生成ID
        }
        
        currentScope.hasLocalScript = true;
        
        // ID优先原则
        // 1. 如果已经有任何ID（显式或自动），使用第一个
        if (!currentScope.explicitIds.isEmpty()) {
            return "#" + currentScope.explicitIds.iterator().next();
        }
        if (!currentScope.autoAddedIds.isEmpty()) {
            return "#" + currentScope.autoAddedIds.iterator().next();
        }
        
        // 2. 如果没有ID但有类，使用第一个类
        if (!currentScope.explicitClasses.isEmpty()) {
            return "." + currentScope.explicitClasses.iterator().next();
        }
        if (!currentScope.autoAddedClasses.isEmpty()) {
            return "." + currentScope.autoAddedClasses.iterator().next();
        }
        
        // 3. 都没有，自动生成ID
        String autoId = generateAutoId();
        currentScope.addAutoId(autoId);
        contextManager.recordAutoAddedId(autoId);
        return "#" + autoId;
    }
    
    /**
     * 处理增强选择器
     * 判断是否需要自动添加
     */
    public String processEnhancedSelector(String selector) {
        if (currentScope == null || !contextManager.isInLocalScript()) {
            return selector;
        }
        
        // 如果是{{&}}，特殊处理
        if ("&".equals(selector)) {
            return processSelfReferenceSelector();
        }
        
        // 检查是否需要自动添加
        if (selector.startsWith(".")) {
            String className = selector.substring(1);
            if (!currentScope.hasClass(className)) {
                currentScope.addAutoClass(className);
                contextManager.recordAutoAddedClass(className);
            }
        } else if (selector.startsWith("#")) {
            String id = selector.substring(1);
            if (!currentScope.hasId(id)) {
                currentScope.addAutoId(id);
                contextManager.recordAutoAddedId(id);
            }
        } else {
            // 标签选择器，检查是否需要添加ID
            if (!currentScope.hasId(selector) && !currentScope.hasClass(selector)) {
                // ID优先
                String autoId = selector + "-" + generateAutoIdSuffix();
                currentScope.addAutoId(autoId);
                contextManager.recordAutoAddedId(autoId);
                return "#" + autoId;
            }
        }
        
        return selector;
    }
    
    /**
     * 获取当前元素的所有自动添加的属性
     */
    public Map<String, Set<String>> getAutoAddedAttributes() {
        Map<String, Set<String>> attributes = new HashMap<>();
        
        if (currentScope != null) {
            if (!currentScope.autoAddedClasses.isEmpty()) {
                attributes.put("class", new HashSet<>(currentScope.autoAddedClasses));
            }
            if (!currentScope.autoAddedIds.isEmpty()) {
                // ID只能有一个，取第一个
                Set<String> ids = new HashSet<>();
                if (!currentScope.autoAddedIds.isEmpty()) {
                    ids.add(currentScope.autoAddedIds.iterator().next());
                }
                attributes.put("id", ids);
            }
        }
        
        return attributes;
    }
    
    /**
     * 生成自动ID
     */
    private String generateAutoId() {
        return idPrefix + generateAutoIdSuffix();
    }
    
    /**
     * 生成自动ID后缀
     */
    private String generateAutoIdSuffix() {
        return String.valueOf(++autoIdCounter);
    }
    
    /**
     * 设置ID前缀
     */
    public void setIdPrefix(String prefix) {
        this.idPrefix = prefix;
    }
    
    /**
     * 重置管理器
     */
    public void reset() {
        scopeStack.clear();
        currentScope = null;
        autoIdCounter = 0;
    }
}