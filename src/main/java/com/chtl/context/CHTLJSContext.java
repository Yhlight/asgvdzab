package com.chtl.context;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import com.chtl.ast.chtljs.ArrowOperationNode;
import com.chtl.ast.chtljs.CHTLJSASTNode;
import com.chtl.ast.chtljs.EnhancedSelectorNode;
import com.chtl.ast.chtljs.EventHandlerNode;
import com.chtl.ast.chtljs.PropertyAccessNode;
import com.chtl.scanner.State;
import com.chtl.ast.chtljs.*;

/**
 * CHTL JS编译上下文
 * 使用RAII模式管理CHTL JS编译状态
 * 严格按照CHTL语法文档的CHTL JS部分定义
 */
public class CHTLJSContext {
    // 状态栈
    private final Stack<State> stateStack;
    
    // 选择器缓存
    private final Map<String, String> selectorCache;
    
    // 事件委托注册表
    // private final Map<String, List<DelegationInfo>> delegationRegistry; // TODO: Fix DelegationInfo access
    
    // 当前处理的节点栈
    private final Stack<CHTLJSASTNode> nodeStack;
    
    // 错误收集器
    private final List<CompilationError> errors;
    
    // 警告收集器
    private final List<CompilationWarning> warnings;
    
    // 临时变量计数器
    private int tempVarCounter;
    
    // 选择器ID计数器
    private int selectorIdCounter;
    
    public CHTLJSContext() {
        this.stateStack = new Stack<>();
        this.selectorCache = new HashMap<Object, Object>();
        this.delegationRegistry = new HashMap<Object, Object>();
        this.nodeStack = new Stack<>();
        this.errors = new ArrayList<Object>();
        this.warnings = new ArrayList<Object>();
        this.tempVarCounter = 0;
        this.selectorIdCounter = 0;
        
        // 初始状态
        pushState(State.ROOT);
    }
    
    /**
     * CHTL JS状态枚举
     */
    public enum State {
        ROOT,                   // 根状态
        ENHANCED_SELECTOR,      // 增强选择器内部
        LISTEN_CALL,            // listen调用
        DELEGATE_CALL,          // delegate调用
        ANIMATE_CALL,           // animate调用
        EVENT_HANDLER,          // 事件处理器
        ANIMATION_CONFIG,       // 动画配置
        ANIMATION_KEYFRAME,     // 动画关键帧
        ARROW_OPERATION,        // 箭头操作
        PROPERTY_ACCESS,        // 属性访问
        EXPRESSION              // 表达式
    }
    
    /**
     * 事件委托信息
     */
    public static class DelegationInfo {
        private final String parentSelector;
        private final List<String> targetSelectors;
        private final String eventName;
        private final EventHandlerNode handler;
        
    public DelegationInfo(String parentSelector, List<String> targetSelectors,
                              String eventName, EventHandlerNode handler) {
            this.parentSelector = parentSelector;
            this.targetSelectors = new ArrayList<Object>(targetSelectors);
            this.eventName = eventName;
            this.handler = handler;
        }
        
        public String getParentSelector() {
            return parentSelector;
        }
        
        public List<String> getTargetSelectors() {
            return Collections.unmodifiableList(targetSelectors);
        }
        
        public String getEventName() {
            return eventName;
        }
        
        public EventHandlerNode getHandler() {
            return handler;
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
     * RAII节点管理器
     */
    public class NodeGuard implements AutoCloseable {
        private final CHTLJSASTNode node;
        
        public NodeGuard(CHTLJSASTNode node) {
            this.node = node;
            pushNode(node);
        }
        
        @Override
        public void close() {
            popNode();
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
    
    // 节点管理方法
    
    public CHTLJSASTNode getCurrentNode() {
        return nodeStack.isEmpty() ? null : nodeStack.peek();
    }
    
    public void pushNode(CHTLJSASTNode node) {
        nodeStack.push(node);
    }
    
    public CHTLJSASTNode popNode() {
        return nodeStack.isEmpty() ? null : nodeStack.pop();
    }
    
    public NodeGuard processNode(CHTLJSASTNode node) {
        return new NodeGuard(node);
    }
    
    // 选择器管理
    
    public String getCachedSelector(String selector) {
        return selectorCache.get(selector);
    }
    
    public void cacheSelector(String selector, String variable) {
        selectorCache.put(selector, variable);
    }
    
    public boolean hasCachedSelector(String selector) {
        return selectorCache.containsKey(selector);
    }
    
    public String generateSelectorVariable() {
        return "_sel" + (++selectorIdCounter);
    }
    
    // 事件委托管理
    
    public void registerDelegation(DelegationInfo info) { 
        delegationRegistry.computeIfAbsent(info.getParentSelector(), k -> new ArrayList<Object>()).add(info);
    }
    
    public Map<String, List<DelegationInfo>> getDelegationRegistry() { 
        return Collections.unmodifiableMap(delegationRegistry);
    }
    
    public boolean hasDelegations() {
        return !delegationRegistry.isEmpty();
    }
    
    // 临时变量管理
    
    public String generateTempVariable() {
        return "_tmp" + (++tempVarCounter);
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
    
    public boolean isInSelector() {
        return isInState(State.ENHANCED_SELECTOR);
    }
    
    public boolean isInListenCall() {
        return isInState(State.LISTEN_CALL);
    }
    
    public boolean isInDelegateCall() {
        return isInState(State.DELEGATE_CALL);
    }
    
    public boolean isInAnimateCall() {
        return isInState(State.ANIMATE_CALL);
    }
    
    public boolean isInEventHandler() {
        return isInState(State.EVENT_HANDLER);
    }
    
    // 语法验证方法
    
    public boolean canUseArrowOperator() {
        // 箭头操作符只能在选择器后使用
        CHTLJSASTNode current = getCurrentNode();
        return current instanceof EnhancedSelectorNode || 
               current instanceof PropertyAccessNode ||
               current instanceof ArrowOperationNode;
    }
    
    public boolean canUseListen() {
        // listen只能在箭头操作后使用
        return isInState(State.ARROW_OPERATION);
    }
    
    public boolean canUseDelegate() {
        // delegate只能在箭头操作后使用
        return isInState(State.ARROW_OPERATION);
    }
    
    public boolean canUseAnimate() {
        // animate可以在根级别使用
        return isInState(State.ROOT) || isInState(State.EXPRESSION);
    }
    
    // 清理方法
    
    public void reset() {
        stateStack.clear();
        selectorCache.clear();
        delegationRegistry.clear();
        nodeStack.clear();
        errors.clear();
        warnings.clear();
        tempVarCounter = 0;
        selectorIdCounter = 0;
        
        // 重新初始化
        pushState(State.ROOT);
    }
}
