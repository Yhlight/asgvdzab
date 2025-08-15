package com.chtl.scanner;

import com.chtl.context.CompilationContext;
import com.chtl.context.CompilationError;
import com.chtl.context.CompilationWarning;
import com.chtl.model.FragmentType;
import com.chtl.model.CodeFragment;
import java.util.*;

/**
 * 扫描器状态机
 * 与CHTLUnifiedScanner配合使用，提供更精确的状态管理
 * 严格遵循CHTL语法文档的宽判严判机制
 */
public class ScannerStateMachine {
    private final CompilationContext context;
    private ScanState currentState;
    private final Stack<ScanState> stateStack;
    private final List<StateTransition> transitions;
    
    // 当前扫描位置
    private int position;
    private int line;
    private int column;
    
    // 缓冲区
    private final StringBuilder buffer;
    
    // 嵌套深度
    private int braceDepth;
    private int parenDepth;
    private int bracketDepth;
    
    /**
     * 扫描状态
     */
    public enum ScanState {
        INITIAL,                    // 初始状态
        IN_CHTL_ELEMENT,            // CHTL元素内部
        IN_CHTL_ATTRIBUTE,          // CHTL属性
        IN_TEXT_BLOCK,              // text块
        IN_STYLE_BLOCK,             // style块（局部）
        IN_GLOBAL_STYLE_BLOCK,      // style块（全局）
        IN_SCRIPT_BLOCK,            // script块（局部）
        IN_GLOBAL_SCRIPT_BLOCK,     // script块（全局）
        IN_TEMPLATE_DEFINITION,     // 模板定义
        IN_CUSTOM_DEFINITION,       // 自定义定义
        IN_ORIGIN_BLOCK,            // 原始嵌入块
        IN_CHTL_JS_SELECTOR,        // CHTL JS选择器 {{}}
        IN_CHTL_JS_ARROW,           // CHTL JS箭头操作
        IN_CHTL_JS_METHOD,          // CHTL JS方法调用
        IN_CSS_RULE,                // CSS规则
        IN_CSS_SELECTOR,            // CSS选择器
        IN_CSS_PROPERTY,            // CSS属性
        IN_JS_EXPRESSION,           // JavaScript表达式
        IN_JS_STRING,               // JavaScript字符串
        IN_JS_FUNCTION,             // JavaScript函数
        IN_COMMENT_LINE,            // 行注释
        IN_COMMENT_BLOCK,           // 块注释
        IN_STRING_SINGLE,           // 单引号字符串
        IN_STRING_DOUBLE,           // 双引号字符串
        IN_STRING_TEMPLATE          // 模板字符串
    }
    
    /**
     * 状态转换
     */
    public static class StateTransition {
        private final ScanState from;
        private final ScanState to;
        private final TransitionCondition condition;
        private final TransitionAction action;
        
        public StateTransition(ScanState from, ScanState to, 
                               TransitionCondition condition, TransitionAction action) {
            this.from = from;
            this.to = to;
            this.condition = condition;
            this.action = action;
        }
        
        public boolean canTransition(ScannerStateMachine machine, char ch) {
            return machine.currentState == from && condition.test(machine, ch);
        }
        
        public void execute(ScannerStateMachine machine) {
            if (action != null) {
                action.execute(machine);
            }
            machine.currentState = to;
        }
    }
    
    /**
     * 转换条件接口
     */
    @FunctionalInterface
    public interface TransitionCondition {
        boolean test(ScannerStateMachine machine, char ch);
    }
    
    /**
     * 转换动作接口
     */
    @FunctionalInterface
    public interface TransitionAction {
        void execute(ScannerStateMachine machine);
    }
    
    public ScannerStateMachine(CompilationContext context) {
        this.context = context;
        this.currentState = ScanState.INITIAL;
        this.stateStack = new Stack<>();
        this.transitions = new ArrayList<>();
        this.buffer = new StringBuilder();
        this.position = 0;
        this.line = 1;
        this.column = 1;
        this.braceDepth = 0;
        this.parenDepth = 0;
        this.bracketDepth = 0;
        
        initializeTransitions();
    }
    
    /**
     * 初始化状态转换规则
     */
    private void initializeTransitions() {
        // CHTL元素识别
        addTransition(ScanState.INITIAL, ScanState.IN_CHTL_ELEMENT,
            (m, ch) -> Character.isLetter(ch) && m.lookAhead("{"),
            m -> m.startFragment(FragmentType.CHTL));
        
        // text块识别
        addTransition(ScanState.IN_CHTL_ELEMENT, ScanState.IN_TEXT_BLOCK,
            (m, ch) -> m.matchesKeyword("text") && m.lookAhead("{"),
            null);
        
        // style块识别（局部）
        addTransition(ScanState.IN_CHTL_ELEMENT, ScanState.IN_STYLE_BLOCK,
            (m, ch) -> m.matchesKeyword("style") && m.lookAhead("{") && m.isInElement(),
            m -> m.startFragment(FragmentType.CHTL_LOCAL_STYLE));
        
        // style块识别（全局）
        addTransition(ScanState.INITIAL, ScanState.IN_GLOBAL_STYLE_BLOCK,
            (m, ch) -> m.matchesKeyword("style") && m.lookAhead("{") && !m.isInElement(),
            m -> m.startFragment(FragmentType.CSS));
        
        // script块识别（局部）
        addTransition(ScanState.IN_CHTL_ELEMENT, ScanState.IN_SCRIPT_BLOCK,
            (m, ch) -> m.matchesKeyword("script") && m.lookAhead("{") && m.isInElement(),
            m -> m.checkForCHTLJS());
        
        // CHTL JS选择器识别
        addTransition(ScanState.IN_SCRIPT_BLOCK, ScanState.IN_CHTL_JS_SELECTOR,
            (m, ch) -> ch == '{' && m.lookAhead("{"),
            m -> m.startFragment(FragmentType.CHTL_JS));
        
        // CHTL JS箭头操作识别
        addTransition(ScanState.IN_CHTL_JS_SELECTOR, ScanState.IN_CHTL_JS_ARROW,
            (m, ch) -> ch == '-' && m.lookAhead(">"),
            null);
        
        // CSS选择器识别
        addTransition(ScanState.IN_STYLE_BLOCK, ScanState.IN_CSS_SELECTOR,
            (m, ch) -> (ch == '.' || ch == '#' || ch == '&' || Character.isLetter(ch)),
            null);
        
        // 字符串识别
        addTransition(ScanState.IN_JS_EXPRESSION, ScanState.IN_STRING_SINGLE,
            (m, ch) -> ch == '\'',
            null);
        
        addTransition(ScanState.IN_JS_EXPRESSION, ScanState.IN_STRING_DOUBLE,
            (m, ch) -> ch == '"',
            null);
        
        addTransition(ScanState.IN_JS_EXPRESSION, ScanState.IN_STRING_TEMPLATE,
            (m, ch) -> ch == '`',
            null);
        
        // 注释识别
        addTransition(null, ScanState.IN_COMMENT_LINE,
            (m, ch) -> ch == '/' && m.lookAhead("/"),
            null);
        
        addTransition(null, ScanState.IN_COMMENT_BLOCK,
            (m, ch) -> ch == '/' && m.lookAhead("*"),
            null);
    }
    
    /**
     * 添加状态转换
     */
    private void addTransition(ScanState from, ScanState to, 
                               TransitionCondition condition, TransitionAction action) {
        transitions.add(new StateTransition(from, to, condition, action));
    }
    
    /**
     * 处理字符
     */
    public void processChar(char ch, String source) {
        // 更新位置
        updatePosition(ch);
        
        // 检查状态转换
        for (StateTransition transition : transitions) {
            if (transition.from == null || transition.canTransition(this, ch)) {
                transition.execute(this);
                break;
            }
        }
        
        // 更新嵌套深度
        updateNestingDepth(ch);
        
        // 添加到缓冲区
        buffer.append(ch);
    }
    
    /**
     * 获取当前片段类型
     */
    public FragmentType getCurrentFragmentType() {
        switch (currentState) {
            case IN_CHTL_ELEMENT:
            case IN_CHTL_ATTRIBUTE:
            case IN_TEXT_BLOCK:
            case IN_TEMPLATE_DEFINITION:
            case IN_CUSTOM_DEFINITION:
                return FragmentType.CHTL;
                
            case IN_STYLE_BLOCK:
                return FragmentType.CHTL_LOCAL_STYLE;
                
            case IN_GLOBAL_STYLE_BLOCK:
            case IN_CSS_RULE:
            case IN_CSS_SELECTOR:
            case IN_CSS_PROPERTY:
                return FragmentType.CSS;
                
            case IN_SCRIPT_BLOCK:
                return hasUnprocessedCHTLJS() ? FragmentType.CHTL_JS : FragmentType.JAVASCRIPT;
                
            case IN_CHTL_JS_SELECTOR:
            case IN_CHTL_JS_ARROW:
            case IN_CHTL_JS_METHOD:
                return FragmentType.CHTL_JS;
                
            case IN_JS_EXPRESSION:
            case IN_JS_FUNCTION:
            case IN_GLOBAL_SCRIPT_BLOCK:
                return FragmentType.JAVASCRIPT;
                
            default:
                return FragmentType.CHTL;
        }
    }
    
    /**
     * 检查是否应该切割片段
     */
    public boolean shouldSplitFragment() {
        FragmentType currentType = getCurrentFragmentType();
        
        // 严判：CHTL和CHTL JS需要在特定边界切割
        if (currentType == FragmentType.CHTL || currentType == FragmentType.CHTL_JS) {
            return isAtFragmentBoundary();
        }
        
        // 宽判：CSS和JavaScript只在遇到CHTL/CHTL JS特征时切割
        return hasUpcomingCHTLFeature();
    }
    
    /**
     * 检查是否在片段边界
     */
    private boolean isAtFragmentBoundary() {
        switch (currentState) {
            case IN_CHTL_ELEMENT:
                return braceDepth == 0;
            case IN_CHTL_JS_SELECTOR:
                return buffer.toString().endsWith("}}");
            case IN_CHTL_JS_ARROW:
                return buffer.toString().endsWith("->");
            case IN_TEXT_BLOCK:
            case IN_STYLE_BLOCK:
            case IN_SCRIPT_BLOCK:
                return braceDepth == 1 && buffer.toString().endsWith("}");
            default:
                return false;
        }
    }
    
    /**
     * 检查是否有即将到来的CHTL特征
     */
    private boolean hasUpcomingCHTLFeature() {
        String preview = getPreview(10);
        return preview.contains("{{") || preview.contains("->") || 
               preview.matches("\\b(text|style|script)\\s*\\{");
    }
    
    /**
     * 检查是否包含未处理的CHTL JS
     */
    private boolean hasUnprocessedCHTLJS() {
        String content = buffer.toString();
        return content.contains("{{") || content.contains("->");
    }
    
    /**
     * 辅助方法
     */
    
    private void updatePosition(char ch) {
        position++;
        if (ch == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }
    
    private void updateNestingDepth(char ch) {
        switch (ch) {
            case '{': braceDepth++; break;
            case '}': braceDepth--; break;
            case '(': parenDepth++; break;
            case ')': parenDepth--; break;
            case '[': bracketDepth++; break;
            case ']': bracketDepth--; break;
        }
    }
    
    private boolean lookAhead(String pattern) {
        // 实现向前查看逻辑
        return false; // 简化实现
    }
    
    private boolean matchesKeyword(String keyword) {
        // 实现关键字匹配逻辑
        return false; // 简化实现
    }
    
    private boolean isInElement() {
        return context.isInElement();
    }
    
    private void startFragment(FragmentType type) {
        // 开始新片段的逻辑
    }
    
    private void checkForCHTLJS() {
        // 检查脚本块是否包含CHTL JS的逻辑
    }
    
    private String getPreview(int length) {
        // 获取预览字符串的逻辑
        return ""; // 简化实现
    }
    
    /**
     * RAII状态保存器
     */
    public class StateGuard implements AutoCloseable {
        private final ScanState savedState;
        private final int savedPosition;
        
        public StateGuard() {
            this.savedState = currentState;
            this.savedPosition = position;
            stateStack.push(currentState);
        }
        
        @Override
        public void close() {
            if (!stateStack.isEmpty()) {
                stateStack.pop();
            }
            // 如果需要，可以恢复状态
        }
    }
    
    /**
     * 创建状态保存点
     */
    public StateGuard saveState() {
        return new StateGuard();
    }
    
    /**
     * 获取当前状态信息
     */
    public StateInfo getStateInfo() {
        return new StateInfo(currentState, position, line, column, 
                             braceDepth, parenDepth, bracketDepth);
    }
    
    /**
     * 状态信息
     */
    public static class StateInfo {
        public final ScanState state;
        public final int position;
        public final int line;
        public final int column;
        public final int braceDepth;
        public final int parenDepth;
        public final int bracketDepth;
        
        public StateInfo(ScanState state, int position, int line, int column,
                        int braceDepth, int parenDepth, int bracketDepth) {
            this.state = state;
            this.position = position;
            this.line = line;
            this.column = column;
            this.braceDepth = braceDepth;
            this.parenDepth = parenDepth;
            this.bracketDepth = bracketDepth;
        }
    }
}