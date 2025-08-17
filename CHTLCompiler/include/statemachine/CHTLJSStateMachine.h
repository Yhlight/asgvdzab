#pragma once

#include "CHTLStateMachine.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <stack>
#include <functional>
#include <chrono>

namespace chtl {

// CHTL JS状态机状态枚举 - 严格按照语法文档
enum class CHTLJSState {
    // 继承基础状态
    INITIAL = static_cast<int>(CHTLState::INITIAL),
    IDLE = static_cast<int>(CHTLState::IDLE),
    ERROR = static_cast<int>(CHTLState::ERROR),
    RECOVERY = static_cast<int>(CHTLState::RECOVERY),
    
    // CHTL JS特定状态
    IN_SCRIPT_BLOCK,                    // 在script块中
    
    // 增强选择器状态
    EXPECTING_ENHANCED_SELECTOR_START,  // 期望增强选择器开始 ({{)
    IN_ENHANCED_SELECTOR,               // 在增强选择器中
    EXPECTING_ENHANCED_SELECTOR_END,    // 期望增强选择器结束 (}})
    EXPECTING_CSS_SELECTOR_CONTENT,     // 期望CSS选择器内容
    EXPECTING_ARRAY_ACCESS,             // 期望数组访问 ([)
    IN_ARRAY_ACCESS,                    // 在数组访问中
    EXPECTING_ARRAY_INDEX,              // 期望数组索引
    EXPECTING_ARRAY_ACCESS_END,         // 期望数组访问结束 (])
    
    // 链式访问状态
    EXPECTING_CHAIN_ACCESS_START,       // 期望链式访问开始 (-> 或 .)
    IN_CHAIN_ACCESS,                    // 在链式访问中
    EXPECTING_METHOD_NAME,              // 期望方法名
    EXPECTING_METHOD_ARGUMENTS,         // 期望方法参数
    IN_METHOD_ARGUMENTS,                // 在方法参数中
    EXPECTING_METHOD_ARGUMENT,          // 期望方法参数
    EXPECTING_METHOD_ARGUMENT_SEPARATOR, // 期望方法参数分隔符 (,)
    
    // 增强监听器状态
    EXPECTING_LISTEN_KEYWORD,           // 期望listen关键字
    IN_LISTEN_DECLARATION,              // 在listen声明中
    EXPECTING_LISTEN_TARGET,            // 期望listen目标
    EXPECTING_LISTEN_EVENTS,            // 期望listen事件
    IN_LISTEN_EVENTS,                   // 在listen事件中
    EXPECTING_EVENT_TYPE,               // 期望事件类型
    EXPECTING_EVENT_HANDLER,            // 期望事件处理器
    EXPECTING_EVENT_SEPARATOR,          // 期望事件分隔符 (,)
    
    // 事件委托状态
    EXPECTING_DELEGATE_KEYWORD,         // 期望delegate关键字
    IN_DELEGATE_DECLARATION,            // 在delegate声明中
    EXPECTING_DELEGATE_PARENT,          // 期望委托父元素
    EXPECTING_DELEGATE_TARGET,          // 期望委托目标
    IN_DELEGATE_TARGETS,                // 在委托目标中
    EXPECTING_DELEGATE_EVENTS,          // 期望委托事件
    IN_DELEGATE_EVENTS,                 // 在委托事件中
    
    // 动画状态
    EXPECTING_ANIMATE_KEYWORD,          // 期望animate关键字
    IN_ANIMATE_DECLARATION,             // 在animate声明中
    EXPECTING_ANIMATION_DURATION,       // 期望动画持续时间
    EXPECTING_ANIMATION_EASING,         // 期望动画缓动函数
    EXPECTING_ANIMATION_BEGIN,          // 期望动画开始状态
    IN_ANIMATION_BEGIN,                 // 在动画开始状态中
    EXPECTING_ANIMATION_WHEN,           // 期望动画中间状态
    IN_ANIMATION_WHEN,                 // 在动画中间状态中
    EXPECTING_ANIMATION_KEYFRAME,       // 期望动画关键帧
    IN_ANIMATION_KEYFRAME,             // 在动画关键帧中
    EXPECTING_ANIMATION_TIME,           // 期望动画时间点
    EXPECTING_ANIMATION_PROPERTIES,     // 期望动画属性
    IN_ANIMATION_PROPERTIES,            // 在动画属性中
    EXPECTING_ANIMATION_END,            // 期望动画结束状态
    IN_ANIMATION_END,                   // 在动画结束状态中
    EXPECTING_ANIMATION_LOOP,           // 期望动画循环次数
    EXPECTING_ANIMATION_DIRECTION,      // 期望动画方向
    EXPECTING_ANIMATION_DELAY,          // 期望动画延迟
    EXPECTING_ANIMATION_CALLBACK,       // 期望动画回调函数
    
    // JavaScript基础语法状态
    EXPECTING_FUNCTION_DECLARATION,     // 期望函数声明
    IN_FUNCTION_DECLARATION,            // 在函数声明中
    EXPECTING_FUNCTION_NAME,            // 期望函数名
    EXPECTING_FUNCTION_PARAMETERS,      // 期望函数参数
    IN_FUNCTION_PARAMETERS,             // 在函数参数中
    EXPECTING_FUNCTION_PARAMETER,       // 期望函数参数
    EXPECTING_FUNCTION_BODY,            // 期望函数体
    IN_FUNCTION_BODY,                   // 在函数体中
    
    EXPECTING_VARIABLE_DECLARATION,     // 期望变量声明
    IN_VARIABLE_DECLARATION,            // 在变量声明中
    EXPECTING_VARIABLE_NAME,            // 期望变量名
    EXPECTING_VARIABLE_INITIALIZER,     // 期望变量初始化器
    
    EXPECTING_EXPRESSION,               // 期望表达式
    IN_EXPRESSION,                      // 在表达式中
    EXPECTING_EXPRESSION_OPERATOR,      // 期望表达式操作符
    
    EXPECTING_STATEMENT,                // 期望语句
    IN_STATEMENT,                       // 在语句中
    
    EXPECTING_IF_CONDITION,             // 期望if条件
    IN_IF_BODY,                         // 在if体中
    EXPECTING_ELSE_KEYWORD,             // 期望else关键字
    IN_ELSE_BODY,                       // 在else体中
    
    EXPECTING_FOR_INITIALIZER,          // 期望for初始化器
    EXPECTING_FOR_CONDITION,            // 期望for条件
    EXPECTING_FOR_ITERATOR,             // 期望for迭代器
    IN_FOR_BODY,                        // 在for体中
    
    EXPECTING_WHILE_CONDITION,          // 期望while条件
    IN_WHILE_BODY,                      // 在while体中
    
    EXPECTING_RETURN_VALUE,             // 期望return值
    
    EXPECTING_FUNCTION_CALL,            // 期望函数调用
    IN_FUNCTION_CALL,                   // 在函数调用中
    EXPECTING_FUNCTION_ARGUMENTS,       // 期望函数参数
    IN_FUNCTION_ARGUMENTS,              // 在函数参数中
    
    EXPECTING_OBJECT_LITERAL,           // 期望对象字面量
    IN_OBJECT_LITERAL,                  // 在对象字面量中
    EXPECTING_PROPERTY_NAME,            // 期望属性名
    EXPECTING_PROPERTY_VALUE,           // 期望属性值
    EXPECTING_PROPERTY_SEPARATOR,       // 期望属性分隔符
    
    EXPECTING_ARRAY_LITERAL,            // 期望数组字面量
    IN_ARRAY_LITERAL,                   // 在数组字面量中
    EXPECTING_ARRAY_ELEMENT,            // 期望数组元素
    EXPECTING_ARRAY_SEPARATOR,          // 期望数组分隔符
    
    EXPECTING_BINARY_EXPRESSION,        // 期望二元表达式
    EXPECTING_UNARY_EXPRESSION,         // 期望一元表达式
    
    EXPECTING_IDENTIFIER,               // 期望标识符
    EXPECTING_LITERAL_VALUE,            // 期望字面量值
    
    // 其他状态
    EXPECTING_SEMICOLON,                // 期望分号
    EXPECTING_COMMA,                    // 期望逗号
    EXPECTING_COLON,                    // 期望冒号
    EXPECTING_EQUAL,                    // 期望等号
    EXPECTING_LEFT_PARENTHESIS,         // 期望左小括号
    EXPECTING_RIGHT_PARENTHESIS,        // 期望右小括号
    EXPECTING_LEFT_BRACE,               // 期望左大括号
    EXPECTING_RIGHT_BRACE,              // 期望右大括号
    EXPECTING_LEFT_BRACKET,             // 期望左方括号
    EXPECTING_RIGHT_BRACKET             // 期望右方括号
};

// CHTL JS状态转换事件枚举
enum class CHTLJSStateEvent {
    // 继承基础事件
    START = static_cast<int>(CHTLStateEvent::START),
    END = static_cast<int>(CHTLStateEvent::END),
    RESET = static_cast<int>(CHTLStateEvent::RESET),
    ERROR_OCCURRED = static_cast<int>(CHTLStateEvent::ERROR_OCCURRED),
    RECOVERY_ATTEMPTED = static_cast<int>(CHTLStateEvent::RECOVERY_ATTEMPTED),
    RECOVERY_SUCCEEDED = static_cast<int>(CHTLStateEvent::RECOVERY_SUCCEEDED),
    RECOVERY_FAILED = static_cast<int>(CHTLStateEvent::RECOVERY_FAILED),
    
    // CHTL JS特定事件
    SCRIPT_KEYWORD_FOUND,               // 找到script关键字
    SCRIPT_BRACE_OPEN,                  // script块开始大括号
    SCRIPT_BRACE_CLOSE,                 // script块结束大括号
    
    // 增强选择器事件
    ENHANCED_SELECTOR_START_FOUND,      // 找到增强选择器开始 ({{)
    ENHANCED_SELECTOR_END_FOUND,        // 找到增强选择器结束 (}})
    CSS_SELECTOR_CONTENT_FOUND,         // 找到CSS选择器内容
    ARRAY_ACCESS_START_FOUND,           // 找到数组访问开始 ([)
    ARRAY_INDEX_FOUND,                  // 找到数组索引
    ARRAY_ACCESS_END_FOUND,             // 找到数组访问结束 (])
    
    // 链式访问事件
    CHAIN_ACCESS_START_FOUND,           // 找到链式访问开始 (-> 或 .)
    METHOD_NAME_FOUND,                  // 找到方法名
    METHOD_ARGUMENTS_START_FOUND,       // 找到方法参数开始 (()
    METHOD_ARGUMENT_FOUND,              // 找到方法参数
    METHOD_ARGUMENT_SEPARATOR_FOUND,    // 找到方法参数分隔符 (,)
    METHOD_ARGUMENTS_END_FOUND,         // 找到方法参数结束 ())
    
    // 增强监听器事件
    LISTEN_KEYWORD_FOUND,               // 找到listen关键字
    LISTEN_TARGET_FOUND,                // 找到listen目标
    LISTEN_EVENTS_START_FOUND,          // 找到listen事件开始 (()
    EVENT_TYPE_FOUND,                   // 找到事件类型
    EVENT_HANDLER_FOUND,                // 找到事件处理器
    EVENT_SEPARATOR_FOUND,              // 找到事件分隔符 (,)
    LISTEN_EVENTS_END_FOUND,            // 找到listen事件结束 ())
    
    // 事件委托事件
    DELEGATE_KEYWORD_FOUND,             // 找到delegate关键字
    DELEGATE_PARENT_FOUND,              // 找到委托父元素
    DELEGATE_TARGETS_START_FOUND,       // 找到委托目标开始 (()
    DELEGATE_TARGET_FOUND,              // 找到委托目标
    DELEGATE_TARGET_SEPARATOR_FOUND,    // 找到委托目标分隔符 (,)
    DELEGATE_TARGETS_END_FOUND,         // 找到委托目标结束 ())
    DELEGATE_EVENTS_START_FOUND,        // 找到委托事件开始 (()
    DELEGATE_EVENT_FOUND,               // 找到委托事件
    DELEGATE_EVENTS_END_FOUND,          // 找到委托事件结束 ())
    
    // 动画事件
    ANIMATE_KEYWORD_FOUND,              // 找到animate关键字
    ANIMATION_DURATION_FOUND,           // 找到动画持续时间
    ANIMATION_EASING_FOUND,             // 找到动画缓动函数
    ANIMATION_BEGIN_START_FOUND,        // 找到动画开始状态开始 (()
    ANIMATION_PROPERTY_FOUND,           // 找到动画属性
    ANIMATION_PROPERTY_VALUE_FOUND,     // 找到动画属性值
    ANIMATION_BEGIN_END_FOUND,          // 找到动画开始状态结束 ())
    ANIMATION_WHEN_START_FOUND,         // 找到动画中间状态开始 (()
    ANIMATION_KEYFRAME_FOUND,           // 找到动画关键帧
    ANIMATION_TIME_FOUND,               // 找到动画时间点
    ANIMATION_KEYFRAME_PROPERTIES_FOUND, // 找到动画关键帧属性
    ANIMATION_WHEN_END_FOUND,           // 找到动画中间状态结束 ())
    ANIMATION_END_START_FOUND,          // 找到动画结束状态开始 (()
    ANIMATION_END_END_FOUND,             // 找到动画结束状态结束 ())
    ANIMATION_LOOP_FOUND,               // 找到动画循环次数
    ANIMATION_DIRECTION_FOUND,          // 找到动画方向
    ANIMATION_DELAY_FOUND,              // 找到动画延迟
    ANIMATION_CALLBACK_FOUND,            // 找到动画回调函数
    
    // JavaScript基础语法事件
    FUNCTION_KEYWORD_FOUND,             // 找到function关键字
    FUNCTION_NAME_FOUND,                // 找到函数名
    FUNCTION_PARAMETERS_START_FOUND,    // 找到函数参数开始 (()
    FUNCTION_PARAMETER_FOUND,           // 找到函数参数
    FUNCTION_PARAMETER_SEPARATOR_FOUND, // 找到函数参数分隔符 (,)
    FUNCTION_PARAMETERS_END_FOUND,      // 找到函数参数结束 ())
    FUNCTION_BODY_START_FOUND,          // 找到函数体开始 ({)
    FUNCTION_BODY_END_FOUND,            // 找到函数体结束 (})
    
    VAR_KEYWORD_FOUND,                  // 找到var/let/const关键字
    VARIABLE_NAME_FOUND,                // 找到变量名
    VARIABLE_INITIALIZER_FOUND,         // 找到变量初始化器
    
    IF_KEYWORD_FOUND,                   // 找到if关键字
    IF_CONDITION_START_FOUND,           // 找到if条件开始 (()
    IF_CONDITION_END_FOUND,             // 找到if条件结束 ())
    IF_BODY_START_FOUND,                // 找到if体开始 ({)
    IF_BODY_END_FOUND,                  // 找到if体结束 (})
    ELSE_KEYWORD_FOUND,                 // 找到else关键字
    ELSE_BODY_START_FOUND,              // 找到else体开始 ({)
    ELSE_BODY_END_FOUND,                // 找到else体结束 (})
    
    FOR_KEYWORD_FOUND,                  // 找到for关键字
    FOR_INITIALIZER_START_FOUND,        // 找到for初始化器开始 (()
    FOR_INITIALIZER_END_FOUND,          // 找到for初始化器结束 ())
    FOR_CONDITION_FOUND,                // 找到for条件
    FOR_ITERATOR_FOUND,                 // 找到for迭代器
    FOR_BODY_START_FOUND,               // 找到for体开始 ({)
    FOR_BODY_END_FOUND,                 // 找到for体结束 (})
    
    WHILE_KEYWORD_FOUND,                // 找到while关键字
    WHILE_CONDITION_START_FOUND,        // 找到while条件开始 (()
    WHILE_CONDITION_END_FOUND,          // 找到while条件结束 ())
    WHILE_BODY_START_FOUND,             // 找到while体开始 ({)
    WHILE_BODY_END_FOUND,               // 找到while体结束 (})
    
    RETURN_KEYWORD_FOUND,               // 找到return关键字
    RETURN_VALUE_FOUND,                 // 找到return值
    
    FUNCTION_CALL_START_FOUND,          // 找到函数调用开始 (()
    FUNCTION_CALL_END_FOUND,            // 找到函数调用结束 ())
    
    OBJECT_LITERAL_START_FOUND,         // 找到对象字面量开始 ({)
    OBJECT_LITERAL_END_FOUND,           // 找到对象字面量结束 (})
    PROPERTY_NAME_FOUND,                // 找到属性名
    PROPERTY_VALUE_FOUND,               // 找到属性值
    PROPERTY_SEPARATOR_FOUND,           // 找到属性分隔符
    
    ARRAY_LITERAL_START_FOUND,          // 找到数组字面量开始 ([)
    ARRAY_LITERAL_END_FOUND,            // 找到数组字面量结束 (])
    ARRAY_ELEMENT_FOUND,                // 找到数组元素
    ARRAY_SEPARATOR_FOUND,              // 找到数组分隔符
    
    BINARY_OPERATOR_FOUND,              // 找到二元操作符
    UNARY_OPERATOR_FOUND,               // 找到一元操作符
    
    IDENTIFIER_FOUND,                   // 找到标识符
    LITERAL_VALUE_FOUND,                // 找到字面量值
    
    // 其他事件
    SEMICOLON_FOUND,                    // 找到分号
    COMMA_FOUND,                        // 找到逗号
    COLON_FOUND,                        // 找到冒号
    EQUAL_FOUND,                        // 找到等号
    LEFT_PARENTHESIS_FOUND,             // 找到左小括号
    RIGHT_PARENTHESIS_FOUND,            // 找到右小括号
    LEFT_BRACE_FOUND,                   // 找到左大括号
    RIGHT_BRACE_FOUND,                  // 找到右大括号
    LEFT_BRACKET_FOUND,                 // 找到左方括号
    RIGHT_BRACKET_FOUND                 // 找到右方括号
};

// CHTL JS状态机配置
struct CHTLJSStateMachineConfig : public StateMachineConfig {
    bool enable_enhanced_selectors;     // 是否启用增强选择器
    bool enable_chain_access;           // 是否启用链式访问
    bool enable_listen;                 // 是否启用增强监听器
    bool enable_delegate;               // 是否启用事件委托
    bool enable_animate;                // 是否启用动画
    bool enable_js_syntax;              // 是否启用JavaScript语法
    std::string target_es_version;      // 目标ES版本
    
    CHTLJSStateMachineConfig() : 
        enable_enhanced_selectors(true), enable_chain_access(true), enable_listen(true),
        enable_delegate(true), enable_animate(true), enable_js_syntax(true), target_es_version("ES6") {}
};

// CHTL JS状态机类
class CHTLJSStateMachine : public CHTLStateMachine {
public:
    CHTLJSStateMachine(const CHTLJSStateMachineConfig& config = CHTLJSStateMachineConfig());
    ~CHTLJSStateMachine();
    
    // CHTL JS特定状态管理
    CHTLJSState getCurrentCHTLJSState() const;
    CHTLJSState getPreviousCHTLJSState() const;
    std::vector<CHTLJSState> getCHTLJSStateHistory() const;
    
    // CHTL JS特定状态转换
    bool transitionToCHTLJS(CHTLJSStateEvent event);
    bool canTransitionToCHTLJS(CHTLJSStateEvent event) const;
    std::vector<CHTLJSStateEvent> getValidCHTLJSTransitions() const;
    
    // CHTL JS特定状态验证
    bool isValidCHTLJSState(CHTLJSState state) const;
    bool isInCHTLJSState(CHTLJSState state) const;
    bool isInAnyCHTLJSState(const std::vector<CHTLJSState>& states) const;
    
    // CHTL JS特定功能检查
    bool isInEnhancedSelector() const;
    bool isInChainAccess() const;
    bool isInListenDeclaration() const;
    bool isInDelegateDeclaration() const;
    bool isInAnimateDeclaration() const;
    bool isInJavaScriptSyntax() const;
    
    // 配置管理
    void setCHTLJSConfig(const CHTLJSStateMachineConfig& config);
    CHTLJSStateMachineConfig getCHTLJSConfig() const;

private:
    class CHTLJSImpl;
    std::unique_ptr<CHTLJSImpl> chtljs_pImpl;
    
    // 内部方法
    void initializeCHTLJSStateTransitions();
    bool validateCHTLJSTransition(CHTLJSState from, CHTLJSStateEvent event, CHTLJSState to) const;
    void recordCHTLJSStateTransition(CHTLJSState from, CHTLJSStateEvent event, CHTLJSState to);
};

} // namespace chtl