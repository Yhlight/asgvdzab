#pragma once

#include "Types.h"
#include <functional>
#include <unordered_map>

namespace CHTL {
namespace Core {

// 状态机事件类型
enum class StateEvent {
    START_DOCUMENT,     // 开始文档
    END_DOCUMENT,       // 结束文档
    START_ELEMENT,      // 开始元素
    END_ELEMENT,        // 结束元素
    START_ATTRIBUTE,    // 开始属性
    END_ATTRIBUTE,      // 结束属性
    START_TEXT,         // 开始文本
    END_TEXT,           // 结束文本
    START_STYLE_BLOCK,  // 开始样式块
    END_STYLE_BLOCK,    // 结束样式块
    START_SCRIPT_BLOCK, // 开始脚本块
    END_SCRIPT_BLOCK,   // 结束脚本块
    START_TEMPLATE,     // 开始模板
    END_TEMPLATE,       // 结束模板
    START_CUSTOM,       // 开始自定义
    END_CUSTOM,         // 结束自定义
    START_ORIGIN,       // 开始原始嵌入
    END_ORIGIN,         // 结束原始嵌入
    START_NAMESPACE,    // 开始命名空间
    END_NAMESPACE,      // 结束命名空间
    START_IMPORT,       // 开始导入
    END_IMPORT,         // 结束导入
    ERROR_EVENT,        // 错误事件
    RESET_EVENT         // 重置事件
};

// 状态机状态
enum class StateMachineState {
    INITIAL,            // 初始状态
    DOCUMENT,           // 文档状态
    ELEMENT,            // 元素状态
    ATTRIBUTE,          // 属性状态
    TEXT,               // 文本状态
    STYLE_BLOCK,        // 样式块状态
    SCRIPT_BLOCK,       // 脚本块状态
    TEMPLATE,           // 模板状态
    CUSTOM,             // 自定义状态
    ORIGIN,             // 原始嵌入状态
    NAMESPACE,          // 命名空间状态
    IMPORT,             // 导入状态
    ERROR,              // 错误状态
    FINAL               // 最终状态
};

// 状态转换结果
struct StateTransition {
    StateMachineState newState;
    bool isValid;
    String errorMessage;
    
    StateTransition(StateMachineState state, bool valid = true, const String& error = "")
        : newState(state), isValid(valid), errorMessage(error) {}
};

// 状态动作函数类型
using StateAction = std::function<void(const StateEvent&, Context&)>;
using StateGuard = std::function<bool(const StateEvent&, const Context&)>;

// 状态机
class StateMachine {
public:
    StateMachine();
    ~StateMachine() = default;

    // 状态管理
    StateMachineState getCurrentState() const { return currentState_; }
    void setState(StateMachineState state) { currentState_ = state; }
    
    // 事件处理
    bool processEvent(StateEvent event, Context& context);
    StateTransition getTransition(StateMachineState from, StateEvent event) const;
    
    // 状态转换定义
    void defineTransition(StateMachineState from, StateEvent event, 
                         StateMachineState to, const String& description = "");
    void defineGuardedTransition(StateMachineState from, StateEvent event, 
                                StateMachineState to, StateGuard guard, 
                                const String& description = "");
    
    // 状态动作定义
    void defineEntryAction(StateMachineState state, StateAction action);
    void defineExitAction(StateMachineState state, StateAction action);
    void defineTransitionAction(StateMachineState from, StateEvent event, 
                               StateMachineState to, StateAction action);
    
    // 状态查询
    bool canTransition(StateMachineState from, StateEvent event) const;
    bool isValidState(StateMachineState state) const;
    bool isFinalState(StateMachineState state) const;
    bool isErrorState(StateMachineState state) const;
    
    // 重置状态机
    void reset();
    
    // 获取状态历史
    const std::vector<StateMachineState>& getStateHistory() const { return stateHistory_; }
    
    // 调试信息
    String getCurrentStateString() const;
    String getEventString(StateEvent event) const;
    void enableDebug(bool enable) { debugMode_ = enable; }

private:
    // 状态转换表
    using TransitionKey = std::pair<StateMachineState, StateEvent>;
    using TransitionMap = std::unordered_map<TransitionKey, StateTransition, 
                                           std::hash<std::pair<int, int>>>;
    
    // 守卫条件表
    using GuardMap = std::unordered_map<TransitionKey, StateGuard,
                                       std::hash<std::pair<int, int>>>;
    
    // 动作表
    using ActionMap = std::unordered_map<StateMachineState, StateAction>;
    using TransitionActionMap = std::unordered_map<TransitionKey, StateAction,
                                                  std::hash<std::pair<int, int>>>;

    StateMachineState currentState_;
    std::vector<StateMachineState> stateHistory_;
    
    TransitionMap transitions_;
    GuardMap guards_;
    ActionMap entryActions_;
    ActionMap exitActions_;
    TransitionActionMap transitionActions_;
    
    bool debugMode_ = false;
    
    // 初始化状态机
    void initializeStateMachine();
    
    // 执行状态动作
    void executeEntryAction(StateMachineState state, Context& context);
    void executeExitAction(StateMachineState state, Context& context);
    void executeTransitionAction(StateMachineState from, StateEvent event, 
                                StateMachineState to, Context& context);
    
    // 调试输出
    void debugLog(const String& message) const;
};

// 状态机RAII管理器
class StateMachineGuard {
public:
    StateMachineGuard(StateMachine& stateMachine, Context& context)
        : stateMachine_(stateMachine), context_(context) {
        initialState_ = stateMachine_.getCurrentState();
    }
    
    ~StateMachineGuard() {
        // 如果发生异常，恢复到初始状态
        if (std::uncaught_exceptions() > 0) {
            stateMachine_.setState(initialState_);
        }
    }
    
    // 禁止复制
    StateMachineGuard(const StateMachineGuard&) = delete;
    StateMachineGuard& operator=(const StateMachineGuard&) = delete;

private:
    StateMachine& stateMachine_;
    Context& context_;
    StateMachineState initialState_;
};

// 状态机工厂
class StateMachineFactory {
public:
    static std::unique_ptr<StateMachine> createCHTLStateMachine();
    static std::unique_ptr<StateMachine> createCHTLJSStateMachine();
    static std::unique_ptr<StateMachine> createCSSStateMachine();
    static std::unique_ptr<StateMachine> createJavaScriptStateMachine();
};

// 状态到字符串的转换
String stateToString(StateMachineState state);
String eventToString(StateEvent event);

} // namespace Core
} // namespace CHTL