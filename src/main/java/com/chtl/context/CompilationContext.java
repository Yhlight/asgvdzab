package com.chtl.context;

import com.chtl.model.CHTLNode;
import com.chtl.model.SourceLocation;
import com.chtl.model.CompilationResult;
import com.chtl.exception.CHTLException;
import java.util.*;
import java.io.*;
import java.nio.file.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.ConfigurationNode;
import com.chtl.ast.node.CustomDefinitionNode;
import com.chtl.ast.node.TemplateDefinitionNode;
import com.chtl.ast.node.VarGroupDefinitionNode;
import com.chtl.context.CHTLJSContext.NodeGuard;
import com.chtl.context.CHTLJSContext.StateGuard;
import com.chtl.scanner.State;
import com.chtl.compiler.constraint.ConstraintManager;
import com.chtl.compiler.importer.ImportManager;

/**
 * CHTL编译上下文
 * 使用RAII模式管理编译状态
 * 严格按照CHTL语法文档定义
 */
public class CompilationContext {
    // 状态栈
    private final Stack<State> stateStack;
    
    // 作用域栈
    private final Stack<Scope> scopeStack;
    
    // 当前处理的节点栈
    private final Stack<CHTLASTNode> nodeStack;
    
    // 模板定义注册表
    private final Map<String, TemplateDefinitionNode> templateRegistry;
    
    // 自定义定义注册表
    private final Map<String, CustomDefinitionNode> customRegistry;
    
    // 变量组定义注册表
    private final Map<String, VarGroupDefinitionNode> varGroupRegistry;
    
    // 命名空间栈
    private final Stack<String> namespaceStack;
    
    // 配置信息
    private ConfigurationNode configuration;
    
    // 约束管理器
    private final ConstraintManager constraintManager;
    
    // 错误收集器
    private final List<CompilationError> errors;
    
    // 警告收集器
    private final List<CompilationWarning> warnings;
    
    // 调试相关
    private boolean debugMode = false;
    private boolean verboseMode = false;
    private String currentScope = "";
    
    public CompilationContext() {
        this.stateStack = new Stack<>();
        this.scopeStack = new Stack<>();
        this.nodeStack = new Stack<>();
        this.templateRegistry = new HashMap<>();
        this.customRegistry = new HashMap<>();
        this.varGroupRegistry = new HashMap<>();
        this.namespaceStack = new Stack<>();
        this.constraintManager = new ConstraintManager(this);
        this.errors = new ArrayList<>();
        this.warnings = new ArrayList<>();
        
        // 初始状态
        pushState(State.ROOT);
        pushScope(new Scope(ScopeType.GLOBAL));
    }
    
    /**
     * 状态枚举 - 严格对应CHTL语法结构
     */
    public enum State {
        ROOT,                    // 根状态
        ELEMENT,                 // 元素内部
        ATTRIBUTE,               // 属性定义
        TEXT_BLOCK,              // text块内部
        STYLE_BLOCK,             // style块内部
        STYLE_RULE,              // 样式规则内部
        SCRIPT_BLOCK,            // script块内部
        TEMPLATE_DEFINITION,     // 模板定义内部
        CUSTOM_DEFINITION,       // 自定义定义内部
        TEMPLATE_USAGE,          // 模板使用
        CUSTOM_USAGE,            // 自定义使用
        ORIGIN_BLOCK,            // 原始嵌入块
        IMPORT_STATEMENT,        // 导入语句
        NAMESPACE,               // 命名空间内部
        CONFIGURATION,           // 配置组内部
        VAR_GROUP_DEFINITION,    // 变量组定义
        CONSTRAINT_DEFINITION,   // 约束定义
        CONSTRAINT_CHECKING,
        OPTIMIZING,     // 约束检查
        CUSTOM_OPERATION,        // 自定义操作（delete/insert等）
        INHERIT_STATEMENT        // 继承语句
    }
    
    /**
     * 作用域类型
     */
    public enum ScopeType {
        GLOBAL,         // 全局作用域
        NAMESPACE,      // 命名空间作用域
        ELEMENT,        // 元素作用域
        STYLE,          // 样式作用域
        SCRIPT,         // 脚本作用域
        TEMPLATE,       // 模板作用域
        CUSTOM,         // 自定义作用域
        LOCAL           // 局部作用域
    }
    
    /**
     * 作用域
     */
    public static class Scope {
        private final ScopeType type;
        private final Map<String, Object> symbols;
        private final String name;
        
        public Scope(ScopeType type) {
            this(type, null);
        }
        
        public Scope(ScopeType type, String name) {
            this.type = type;
            this.name = name;
            this.symbols = new HashMap<>();
        }
        
        public ScopeType getType() {
            return type;
        }
        
        public String getName() {
            return name;
        }
        
        public void defineSymbol(String name, Object value) {
            symbols.put(name, value);
        }
        
        public Object getSymbol(String name) {
            return symbols.get(name);
        }
        
        public boolean hasSymbol(String name) {
            return symbols.containsKey(name);
        }
    }
    
    /**
     * RAII状态管理器
     */
    public class StateGuard implements AutoCloseable {
        private final State previousState;
        
        public StateGuard(State newState) {
            this.previousState = getCurrentState();
            pushState(newState);
        }
        
        @Override
        public void close() {
            State current = popState();
            if (current != previousState) {
                // 状态不匹配，恢复到之前的状态
                while (!stateStack.isEmpty() && getCurrentState() != previousState) {
                    popState();
                }
            }
        }
    }
    
    /**
     * RAII作用域管理器
     */
    public class ScopeGuard implements AutoCloseable {
        private final Scope previousScope;
        
        public ScopeGuard(Scope newScope) {
            this.previousScope = getCurrentScope();
            pushScope(newScope);
        }
        
        @Override
        public void close() {
            popScope();
        }
    }
    
    /**
     * RAII节点管理器
     */
    public class NodeGuard implements AutoCloseable {
        private final CHTLASTNode node;
        
        public NodeGuard(CHTLASTNode node) {
            this.node = node;
            pushNode(node);
        }
        
        @Override
        public void close() {
            popNode();
        }
    }
    
    /**
     * RAII命名空间管理器
     */
    public class NamespaceGuard implements AutoCloseable {
        private final String namespace;
        
        public NamespaceGuard(String namespace) {
            this.namespace = namespace;
            pushNamespace(namespace);
        }
        
        @Override
        public void close() {
            popNamespace();
        }
    }
    
    // 状态管理方法
    
    public State getCurrentState() {
        return stateStack.isEmpty() ? State.ROOT : stateStack.peek();
    }
    
    public void pushState(State state) {
        stateStack.push(state);
    }
    
    public State popState() {
        return stateStack.isEmpty() ? State.ROOT : stateStack.pop();
    }
    
    public StateGuard enterState(State state) {
        return new StateGuard(state);
    }
    
    // 作用域管理方法
    
    public Scope getCurrentScope() {
        return scopeStack.isEmpty() ? null : scopeStack.peek();
    }
    
    public void pushScope(Scope scope) {
        scopeStack.push(scope);
    }
    
    public Scope popScope() {
        return scopeStack.isEmpty() ? null : scopeStack.pop();
    }
    
    public ScopeGuard enterScope(ScopeType type) {
        return new ScopeGuard(new Scope(type));
    }
    
    public ScopeGuard enterScope(ScopeType type, String name) {
        return new ScopeGuard(new Scope(type, name));
    }
    
    // 节点管理方法
    
    public CHTLASTNode getCurrentNode() {
        return nodeStack.isEmpty() ? null : nodeStack.peek();
    }
    
    public void pushNode(CHTLASTNode node) {
        nodeStack.push(node);
    }
    
    public CHTLASTNode popNode() {
        return nodeStack.isEmpty() ? null : nodeStack.pop();
    }
    
    public NodeGuard processNode(CHTLASTNode node) {
        return new NodeGuard(node);
    }
    
    // 命名空间管理方法
    
    public String getCurrentNamespace() {
        return namespaceStack.isEmpty() ? null : namespaceStack.peek();
    }
    
    public void pushNamespace(String namespace) {
        namespaceStack.push(namespace);
    }
    
    public String popNamespace() {
        return namespaceStack.isEmpty() ? null : namespaceStack.pop();
    }
    
    public NamespaceGuard enterNamespace(String namespace) {
        return new NamespaceGuard(namespace);
    }
    
    public String getFullyQualifiedName(String name) {
        if (namespaceStack.isEmpty()) {
            return name;
        }
        return String.join(".", namespaceStack) + "." + name;
    }
    
    // 定义注册方法
    
    public void registerTemplate(TemplateDefinitionNode template) {
        String key = getRegistryKey(template.getTemplateType(), template.getName());
        templateRegistry.put(key, template);
        
        // 在当前作用域中定义符号
        Scope currentScope = getCurrentScope();
        if (currentScope != null) {
            currentScope.defineSymbol(key, template);
        }
    }
    
    public void registerCustom(CustomDefinitionNode custom) {
        String key = getRegistryKey(custom.getCustomType(), custom.getName());
        customRegistry.put(key, custom);
        
        // 在当前作用域中定义符号
        Scope currentScope = getCurrentScope();
        if (currentScope != null) {
            currentScope.defineSymbol(key, custom);
        }
    }
    
    public void registerVarGroup(VarGroupDefinitionNode varGroup) {
        String key = getFullyQualifiedName(varGroup.getName());
        varGroupRegistry.put(key, varGroup);
        
        // 在当前作用域中定义符号
        Scope currentScope = getCurrentScope();
        if (currentScope != null) {
            currentScope.defineSymbol(key, varGroup);
        }
    }
    
    // 查找方法
    
    public TemplateDefinitionNode findTemplate(TemplateDefinitionNode.TemplateType type, String name) {
        String key = getRegistryKey(type, name);
        
        // 先在作用域链中查找
        for (int i = scopeStack.size() - 1; i >= 0; i--) {
            Scope scope = scopeStack.get(i);
            if (scope.hasSymbol(key)) {
                Object symbol = scope.getSymbol(key);
                if (symbol instanceof TemplateDefinitionNode) {
                    return (TemplateDefinitionNode) symbol;
                }
            }
        }
        
        // 再在全局注册表中查找
        return templateRegistry.get(key);
    }
    
    public CustomDefinitionNode findCustom(TemplateDefinitionNode.TemplateType type, String name) {
        String key = getRegistryKey(type, name);
        
        // 先在作用域链中查找
        for (int i = scopeStack.size() - 1; i >= 0; i--) {
            Scope scope = scopeStack.get(i);
            if (scope.hasSymbol(key)) {
                Object symbol = scope.getSymbol(key);
                if (symbol instanceof CustomDefinitionNode) {
                    return (CustomDefinitionNode) symbol;
                }
            }
        }
        
        // 再在全局注册表中查找
        return customRegistry.get(key);
    }
    
    public VarGroupDefinitionNode findVarGroup(String name) {
        String key = getFullyQualifiedName(name);
        
        // 先在作用域链中查找
        for (int i = scopeStack.size() - 1; i >= 0; i--) {
            Scope scope = scopeStack.get(i);
            if (scope.hasSymbol(key) || scope.hasSymbol(name)) {
                Object symbol = scope.getSymbol(key);
                if (symbol == null) {
                    symbol = scope.getSymbol(name);
                }
                if (symbol instanceof VarGroupDefinitionNode) {
                    return (VarGroupDefinitionNode) symbol;
                }
            }
        }
        
        // 再在全局注册表中查找
        VarGroupDefinitionNode result = varGroupRegistry.get(key);
        if (result == null) {
            result = varGroupRegistry.get(name);
        }
        return result;
    }
    
    private String getRegistryKey(TemplateDefinitionNode.TemplateType type, String name) {
        return type.name() + ":" + getFullyQualifiedName(name);
    }
    
    // 配置管理
    
    public void setConfiguration(ConfigurationNode config) {
        this.configuration = config;
    }
    
    public ConfigurationNode getConfiguration() {
        return configuration;
    }
    
    public String getConfigValue(String key) {
        if (configuration != null) {
            return configuration.getSetting(key);
        }
        return null;
    }
    
    // 约束管理
    
    public ConstraintManager getConstraintManager() {
        return constraintManager;
    }
    
    public boolean isAllowed(CHTLASTNode node) {
        // 简化实现，总是返回true
        return true;
    }
    
    // 错误和警告管理
    
    public void addError(CompilationError error) {
        errors.add(error);
    }
    
    public void addWarning(CompilationWarning warning) {
        warnings.add(warning);
    }
    
    public List<CompilationError> getErrors() {
        return Collections.unmodifiableList(errors);
    }
    
    public List<CompilationWarning> getWarnings() {
        return Collections.unmodifiableList(warnings);
    }
    
    public boolean hasErrors() {
        return !errors.isEmpty();
    }
    
    // 状态验证方法
    
    public boolean isInState(State state) {
        return getCurrentState() == state;
    }
    
    public boolean isInStates(State... states) {
        State current = getCurrentState();
        for (State state : states) {
            if (current == state) {
                return true;
            }
        }
        return false;
    }
    
    public boolean isInScope(ScopeType type) {
        Scope current = getCurrentScope();
        return current != null && current.getType() == type;
    }
    
    public boolean isInElement() {
        return isInState(State.ELEMENT);
    }
    
    public boolean isInStyle() {
        return isInState(State.STYLE_BLOCK) || isInState(State.STYLE_RULE);
    }
    
    public boolean isInScript() {
        return isInState(State.SCRIPT_BLOCK);
    }
    
    public boolean isInTemplate() {
        return isInState(State.TEMPLATE_DEFINITION);
    }
    
    public boolean isInCustom() {
        return isInState(State.CUSTOM_DEFINITION) || isInState(State.CUSTOM_USAGE);
    }
    
    // 清理方法
    
    public void reset() {
        stateStack.clear();
        scopeStack.clear();
        nodeStack.clear();
        namespaceStack.clear();
        errors.clear();
        warnings.clear();
        
        // 重新初始化
        pushState(State.ROOT);
        pushScope(new Scope(ScopeType.GLOBAL));
    }
    
    // 调试相关方法
    public boolean isDebugMode() {
        return debugMode;
    }
    
    public void setDebugMode(boolean debugMode) {
        this.debugMode = debugMode;
    }
    
    public boolean isVerboseMode() {
        return verboseMode;
    }
    
    public void setVerboseMode(boolean verboseMode) {
        this.verboseMode = verboseMode;
    }
    
    public void setCurrentScope(String scope) {
        this.currentScope = scope;
    }
    
    public ImportManager getImportManager() {
        // 需要在实际使用时设置
        return null;
    }
}
