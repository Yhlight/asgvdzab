#pragma once

#include "common/types.hpp"
#include <memory>
#include <stack>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <mutex>

namespace chtl {

/**
 * CHTL语法状态枚举
 * 严格按照CHTL语法文档.md定义，不得私自扩展
 */
enum class CHTLGrammarState {
    // 根状态
    INITIAL,                        // 初始状态
    PROGRAM_ROOT,                   // 程序根节点
    
    // 注释状态 (三种类型)
    LINE_COMMENT,                   // // 注释
    MULTILINE_COMMENT,              // /* */ 注释  
    GENERATOR_COMMENT,              // -- 生成器注释
    
    // 文本节点状态
    TEXT_DECLARATION,               // text 关键字
    TEXT_CONTENT,                   // text { } 内容
    
    // 字面量状态
    DOUBLE_QUOTED_LITERAL,          // "双引号字符串"
    SINGLE_QUOTED_LITERAL,          // '单引号字符串'
    UNQUOTED_LITERAL,               // 无修饰字面量
    
    // 元素节点状态
    ELEMENT_DECLARATION,            // 元素声明 (div, span等)
    ELEMENT_BODY,                   // 元素体内容
    ELEMENT_ATTRIBUTES,             // 元素属性区域
    
    // 属性状态
    ATTRIBUTE_NAME,                 // 属性名
    ATTRIBUTE_COLON_EQUALS,         // CE对等式 (: 或 =)
    ATTRIBUTE_VALUE,                // 属性值
    
    // 局部样式块状态
    STYLE_DECLARATION,              // style 关键字
    STYLE_BODY,                     // style { } 内容
    INLINE_STYLE,                   // 内联样式属性
    CLASS_SELECTOR,                 // .className 自动化类名
    ID_SELECTOR,                    // #idName 自动化ID
    CONTEXT_REFERENCE,              // & 上下文推导
    PSEUDO_CLASS,                   // &:hover 伪类选择器
    PSEUDO_ELEMENT,                 // &::before 伪元素选择器
    
    // 模板状态 ([Template])
    TEMPLATE_DECLARATION,           // [Template] 声明
    TEMPLATE_STYLE,                 // [Template] @Style
    TEMPLATE_ELEMENT,               // [Template] @Element
    TEMPLATE_VAR,                   // [Template] @Var
    TEMPLATE_BODY,                  // 模板体内容
    TEMPLATE_USAGE,                 // 模板使用 (@Style Name)
    TEMPLATE_INHERITANCE,           // inherit 显式继承
    
    // 自定义状态 ([Custom])
    CUSTOM_DECLARATION,             // [Custom] 声明
    CUSTOM_STYLE,                   // [Custom] @Style
    CUSTOM_ELEMENT,                 // [Custom] @Element
    CUSTOM_VAR,                     // [Custom] @Var
    CUSTOM_BODY,                    // 自定义体内容
    CUSTOM_USAGE,                   // 自定义使用
    CUSTOM_SPECIALIZATION,          // 特例化块
    
    // 自定义特例化操作状态
    DELETE_OPERATION,               // delete 操作
    INSERT_OPERATION,               // insert 操作
    INSERT_POSITION,                // 插入位置 (after, before, replace, at top, at bottom)
    INDEX_ACCESS,                   // [index] 索引访问
    
    // 变量组状态
    VAR_GROUP_DECLARATION,          // 变量组声明
    VAR_PROPERTY,                   // 变量属性
    VAR_USAGE,                      // 变量使用 VarName(prop)
    VAR_SPECIALIZATION,             // 变量特例化 VarName(prop = value)
    
    // 原始嵌入状态 ([Origin])
    ORIGIN_DECLARATION,             // [Origin] 声明
    ORIGIN_HTML,                    // [Origin] @Html
    ORIGIN_STYLE,                   // [Origin] @Style
    ORIGIN_JAVASCRIPT,              // [Origin] @JavaScript
    ORIGIN_CONTENT,                 // 原始内容
    
    // 导入状态 ([Import])
    IMPORT_DECLARATION,             // [Import] 声明
    IMPORT_HTML,                    // [Import] @Html
    IMPORT_STYLE,                   // [Import] @Style
    IMPORT_JAVASCRIPT,              // [Import] @JavaScript
    IMPORT_CHTL,                    // [Import] @Chtl
    IMPORT_CUSTOM,                  // [Import] [Custom]
    IMPORT_TEMPLATE,                // [Import] [Template]
    IMPORT_CJMOD,                   // [Import] @CJmod
    IMPORT_FROM,                    // from 关键字
    IMPORT_AS,                      // as 关键字
    IMPORT_PATH,                    // 导入路径
    
    // 命名空间状态 ([Namespace])
    NAMESPACE_DECLARATION,          // [Namespace] 声明
    NAMESPACE_BODY,                 // 命名空间体
    NAMESPACE_NESTED,               // 嵌套命名空间
    NAMESPACE_USAGE,                // from namespace 使用
    
    // 配置组状态 ([Configuration])
    CONFIGURATION_DECLARATION,      // [Configuration] 声明
    CONFIGURATION_BODY,             // 配置体
    CONFIGURATION_PROPERTY,         // 配置属性
    NAME_GROUP,                     // [Name] 配置组
    NAME_GROUP_PROPERTY,            // Name组属性
    
    // 约束状态
    EXCEPT_DECLARATION,             // except 约束声明
    EXCEPT_TARGET,                  // 约束目标
    
    // 脚本状态 (局部script)
    SCRIPT_DECLARATION,             // script 关键字
    SCRIPT_BODY,                    // script { } 内容
    
    // CHTL JS特有状态
    ENHANCED_SELECTOR,              // {{selector}} 增强选择器
    ARROW_OPERATOR,                 // -> 箭头操作符
    LISTEN_CALL,                    // listen 调用
    DELEGATE_CALL,                  // delegate 调用
    ANIMATE_CALL,                   // animate 调用
    LISTEN_CONFIG,                  // listen 配置对象
    DELEGATE_CONFIG,                // delegate 配置对象
    ANIMATE_CONFIG,                 // animate 配置对象
    
    // 错误和结束状态
    ERROR_STATE,                    // 错误状态
    END_OF_INPUT                    // 输入结束
};

/**
 * 状态转换触发器类型
 */
enum class StateTransitionTrigger {
    TOKEN,                          // Token触发
    SCOPE_ENTER,                    // 作用域进入
    SCOPE_EXIT,                     // 作用域退出
    SYNTAX_RULE,                    // 语法规则
    ERROR_RECOVERY                  // 错误恢复
};

/**
 * 状态转换规则
 */
struct StateTransitionRule {
    CHTLGrammarState fromState;     // 源状态
    CHTLGrammarState toState;       // 目标状态
    StateTransitionTrigger trigger; // 触发器
    std::string condition;          // 转换条件
    std::function<bool()> validator; // 验证函数
    
    StateTransitionRule(CHTLGrammarState from, CHTLGrammarState to, 
                       StateTransitionTrigger trig, const std::string& cond = "")
        : fromState(from), toState(to), trigger(trig), condition(cond) {}
};

/**
 * 状态历史记录
 */
struct StateHistoryEntry {
    CHTLGrammarState state;         // 状态
    Position position;              // 位置
    std::string context;            // 上下文信息
    uint64_t timestamp;             // 时间戳
    
    StateHistoryEntry(CHTLGrammarState s, const Position& pos, const std::string& ctx)
        : state(s), position(pos), context(ctx), timestamp(0) {}
};

/**
 * RAII状态守卫类
 * 自动管理状态的进入和退出
 */
class StateGuard {
public:
    StateGuard(class RAIIStateMachine& machine, CHTLGrammarState newState, const std::string& context = "");
    ~StateGuard();
    
    // 禁止拷贝和赋值
    StateGuard(const StateGuard&) = delete;
    StateGuard& operator=(const StateGuard&) = delete;
    
    // 支持移动
    StateGuard(StateGuard&& other) noexcept;
    StateGuard& operator=(StateGuard&& other) noexcept;
    
    // 提前释放守卫
    void release();
    
    // 检查守卫是否有效
    bool isValid() const { return machine_ != nullptr; }

private:
    class RAIIStateMachine* machine_;
    CHTLGrammarState previousState_;
    bool released_;
};

/**
 * 状态变化监听器接口
 */
class IStateChangeListener {
public:
    virtual ~IStateChangeListener() = default;
    virtual void onStateEnter(CHTLGrammarState state, const std::string& context) = 0;
    virtual void onStateExit(CHTLGrammarState state, const std::string& context) = 0;
    virtual void onStateTransition(CHTLGrammarState from, CHTLGrammarState to, const std::string& reason) = 0;
    virtual void onStateError(CHTLGrammarState currentState, const std::string& error) = 0;
};

/**
 * RAII自动化状态机
 * 严格按照CHTL语法文档管理解析状态
 */
class RAIIStateMachine {
public:
    explicit RAIIStateMachine();
    ~RAIIStateMachine();
    
    // ===== 状态管理 =====
    
    /**
     * 获取当前状态
     */
    CHTLGrammarState getCurrentState() const;
    
    /**
     * 手动转换状态
     * @param newState 新状态
     * @param context 上下文信息
     * @return 是否转换成功
     */
    bool transitionTo(CHTLGrammarState newState, const std::string& context = "");
    
    /**
     * 创建RAII状态守卫
     * @param newState 新状态
     * @param context 上下文
     * @return 状态守卫对象
     */
    StateGuard createGuard(CHTLGrammarState newState, const std::string& context = "");
    
    /**
     * 恢复到前一个状态
     * @return 是否恢复成功
     */
    bool restorePreviousState();
    
    /**
     * 重置状态机到初始状态
     */
    void reset();
    
    // ===== 状态验证 =====
    
    /**
     * 检查是否可以转换到指定状态
     * @param targetState 目标状态
     * @return 是否可以转换
     */
    bool canTransitionTo(CHTLGrammarState targetState) const;
    
    /**
     * 检查当前是否在指定状态
     * @param state 要检查的状态
     * @return 是否在指定状态
     */
    bool isInState(CHTLGrammarState state) const;
    
    /**
     * 检查当前是否在任一指定状态中
     * @param states 状态列表
     * @return 是否在任一状态中
     */
    bool isInAnyState(const std::vector<CHTLGrammarState>& states) const;
    
    // ===== 状态历史 =====
    
    /**
     * 获取状态历史
     * @return 状态历史列表
     */
    const std::vector<StateHistoryEntry>& getStateHistory() const;
    
    /**
     * 清空状态历史
     */
    void clearHistory();
    
    /**
     * 获取状态栈深度
     * @return 栈深度
     */
    size_t getStackDepth() const;
    
    // ===== 监听器管理 =====
    
    /**
     * 添加状态变化监听器
     * @param listener 监听器
     */
    void addListener(std::shared_ptr<IStateChangeListener> listener);
    
    /**
     * 移除状态变化监听器
     * @param listener 监听器
     */
    void removeListener(std::shared_ptr<IStateChangeListener> listener);
    
    // ===== 状态转换规则 =====
    
    /**
     * 添加状态转换规则
     * @param rule 转换规则
     */
    void addTransitionRule(const StateTransitionRule& rule);
    
    /**
     * 移除状态转换规则
     * @param fromState 源状态
     * @param toState 目标状态
     */
    void removeTransitionRule(CHTLGrammarState fromState, CHTLGrammarState toState);
    
    // ===== 调试和诊断 =====
    
    /**
     * 获取状态名称
     * @param state 状态
     * @return 状态名称字符串
     */
    static std::string getStateName(CHTLGrammarState state);
    
    /**
     * 获取当前状态的调试信息
     * @return 调试信息字符串
     */
    std::string getDebugInfo() const;
    
    /**
     * 转储状态机信息到字符串
     * @return 状态机完整信息
     */
    std::string dump() const;

private:
    friend class StateGuard;
    
    // 内部状态转换（由StateGuard调用）
    void enterState(CHTLGrammarState newState, const std::string& context);
    void exitState(const std::string& context);
    
    // 通知监听器
    void notifyStateEnter(CHTLGrammarState state, const std::string& context);
    void notifyStateExit(CHTLGrammarState state, const std::string& context);
    void notifyStateTransition(CHTLGrammarState from, CHTLGrammarState to, const std::string& reason);
    void notifyStateError(const std::string& error);
    
    // 验证状态转换
    bool validateTransition(CHTLGrammarState from, CHTLGrammarState to) const;
    
    // 初始化默认转换规则
    void initializeDefaultRules();

private:
    std::stack<CHTLGrammarState> stateStack_;               // 状态栈
    std::vector<StateHistoryEntry> history_;               // 状态历史
    std::vector<std::shared_ptr<IStateChangeListener>> listeners_; // 监听器列表
    std::vector<StateTransitionRule> transitionRules_;     // 转换规则
    
    mutable std::mutex stateMutex_;                        // 状态访问互斥锁
    bool errorRecoveryMode_;                               // 错误恢复模式
    Position currentPosition_;                             // 当前位置
};

/**
 * 状态机工厂
 */
class StateMachineFactory {
public:
    /**
     * 创建CHTL状态机
     * @return CHTL状态机实例
     */
    static std::unique_ptr<RAIIStateMachine> createCHTLStateMachine();
    
    /**
     * 创建CHTL JS状态机
     * @return CHTL JS状态机实例
     */
    static std::unique_ptr<RAIIStateMachine> createCHTLJSStateMachine();
    
    /**
     * 创建通用状态机
     * @return 通用状态机实例
     */
    static std::unique_ptr<RAIIStateMachine> createGenericStateMachine();
};

// ===== 便利宏定义 =====

/**
 * 自动状态守卫宏
 * 在当前作用域内自动管理状态
 */
#define CHTL_STATE_GUARD(machine, state) \
    auto __state_guard_##__LINE__ = (machine).createGuard(state, __func__)

/**
 * 条件状态守卫宏
 * 根据条件创建状态守卫
 */
#define CHTL_STATE_GUARD_IF(machine, state, condition) \
    auto __state_guard_##__LINE__ = (condition) ? \
        (machine).createGuard(state, __func__) : StateGuard(machine, machine.getCurrentState())

/**
 * 状态验证宏
 * 验证当前状态是否正确
 */
#define CHTL_VALIDATE_STATE(machine, expected_state) \
    do { \
        if (!(machine).isInState(expected_state)) { \
            throw std::runtime_error("Expected state " + RAIIStateMachine::getStateName(expected_state) + \
                                    " but current state is " + RAIIStateMachine::getStateName((machine).getCurrentState())); \
        } \
    } while(0)

} // namespace chtl