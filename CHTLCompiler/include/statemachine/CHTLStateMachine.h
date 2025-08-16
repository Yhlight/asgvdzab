#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <stack>
#include <functional>
#include <chrono>
#include <variant>

namespace chtl {

// 状态机状态枚举 - 严格按照CHTL语法文档
enum class CHTLState {
    // 基础状态
    INITIAL,                    // 初始状态
    IDLE,                       // 空闲状态
    
    // 注释状态
    IN_SINGLE_LINE_COMMENT,     // 在单行注释中 (//)
    IN_MULTI_LINE_COMMENT,      // 在多行注释中 (/* */)
    IN_GENERATOR_COMMENT,       // 在生成器注释中 (--)
    
    // 文本节点状态
    EXPECTING_TEXT_KEYWORD,     // 期望text关键字
    IN_TEXT_BLOCK,              // 在text块中
    EXPECTING_TEXT_CONTENT,     // 期望文本内容
    EXPECTING_TEXT_BRACE_CLOSE, // 期望text块结束大括号
    
    // 字面量状态
    EXPECTING_LITERAL_VALUE,    // 期望字面量值
    IN_QUOTED_STRING,           // 在引号字符串中
    IN_UNQUOTED_STRING,         // 在无引号字符串中
    
    // 元素状态
    EXPECTING_ELEMENT_NAME,     // 期望元素名称
    IN_ELEMENT_DECLARATION,     // 在元素声明中
    EXPECTING_ELEMENT_ATTRIBUTES, // 期望元素属性
    EXPECTING_ELEMENT_BRACE_OPEN, // 期望元素开始大括号
    IN_ELEMENT_BODY,            // 在元素体中
    EXPECTING_ELEMENT_BRACE_CLOSE, // 期望元素结束大括号
    
    // 属性状态
    EXPECTING_ATTRIBUTE_NAME,   // 期望属性名
    EXPECTING_ATTRIBUTE_SEPARATOR, // 期望属性分隔符 (: 或 =)
    EXPECTING_ATTRIBUTE_VALUE,  // 期望属性值
    EXPECTING_ATTRIBUTE_TERMINATOR, // 期望属性结束符 (;)
    
    // 样式块状态
    EXPECTING_STYLE_KEYWORD,    // 期望style关键字
    IN_STYLE_BLOCK,             // 在样式块中
    EXPECTING_CSS_SELECTOR,     // 期望CSS选择器
    IN_CSS_RULE,                // 在CSS规则中
    EXPECTING_CSS_PROPERTY,     // 期望CSS属性
    EXPECTING_CSS_VALUE,        // 期望CSS值
    EXPECTING_CSS_TERMINATOR,   // 期望CSS结束符 (;)
    IN_CONTEXT_REFERENCE,       // 在上下文引用中 (&)
    
    // 模板状态
    EXPECTING_TEMPLATE_KEYWORD, // 期望[Template]关键字
    IN_TEMPLATE_DECLARATION,    // 在模板声明中
    EXPECTING_TEMPLATE_TYPE,    // 期望模板类型 (@Style, @Element, @Var)
    EXPECTING_TEMPLATE_NAME,    // 期望模板名称
    IN_TEMPLATE_BODY,           // 在模板体中
    EXPECTING_TEMPLATE_INHERITANCE, // 期望模板继承
    
    // 自定义状态
    EXPECTING_CUSTOM_KEYWORD,   // 期望[Custom]关键字
    IN_CUSTOM_DECLARATION,      // 在自定义声明中
    EXPECTING_CUSTOM_TYPE,      // 期望自定义类型
    EXPECTING_CUSTOM_NAME,      // 期望自定义名称
    IN_CUSTOM_BODY,             // 在自定义体中
    EXPECTING_CUSTOM_SPECIALIZATION, // 期望自定义特例化
    IN_DELETE_OPERATION,        // 在删除操作中
    IN_INSERT_OPERATION,        // 在插入操作中
    IN_REPLACE_OPERATION,       // 在替换操作中
    
    // 原始嵌入状态
    EXPECTING_ORIGIN_KEYWORD,   // 期望[Origin]关键字
    IN_ORIGIN_DECLARATION,      // 在原始嵌入声明中
    EXPECTING_ORIGIN_TYPE,      // 期望原始嵌入类型
    IN_ORIGIN_CONTENT,          // 在原始嵌入内容中
    
    // 配置组状态
    EXPECTING_CONFIGURATION_KEYWORD, // 期望[Configuration]关键字
    IN_CONFIGURATION_DECLARATION,    // 在配置组声明中
    EXPECTING_CONFIG_OPTION,         // 期望配置选项
    EXPECTING_NAME_KEYWORD,          // 期望[Name]关键字
    IN_NAME_DECLARATION,             // 在名称声明中
    
    // 导入状态
    EXPECTING_IMPORT_KEYWORD,   // 期望[Import]关键字
    IN_IMPORT_DECLARATION,      // 在导入声明中
    EXPECTING_IMPORT_TYPE,      // 期望导入类型
    EXPECTING_IMPORT_PATH,      // 期望导入路径
    EXPECTING_IMPORT_NAME,      // 期望导入名称
    
    // 命名空间状态
    EXPECTING_NAMESPACE_KEYWORD, // 期望[Namespace]关键字
    IN_NAMESPACE_DECLARATION,    // 在命名空间声明中
    EXPECTING_NAMESPACE_NAME,    // 期望命名空间名称
    IN_NAMESPACE_BODY,           // 在命名空间体中
    EXPECTING_NESTED_NAMESPACE,  // 期望嵌套命名空间
    
    // 约束状态
    EXPECTING_CONSTRAINT_KEYWORD, // 期望except关键字
    IN_CONSTRAINT_DECLARATION,    // 在约束声明中
    EXPECTING_CONSTRAINT_TARGET,  // 期望约束目标
    EXPECTING_CONSTRAINT_TYPE,    // 期望约束类型
    
    // 脚本状态
    EXPECTING_SCRIPT_KEYWORD,   // 期望script关键字
    IN_SCRIPT_BLOCK,            // 在脚本块中
    
    // 模块信息状态
    EXPECTING_INFO_KEYWORD,     // 期望[Info]关键字
    IN_INFO_DECLARATION,        // 在信息声明中
    EXPECTING_INFO_PROPERTY,    // 期望信息属性
    EXPECTING_EXPORT_KEYWORD,   // 期望[Export]关键字
    IN_EXPORT_DECLARATION,      // 在导出声明中
    
    // 错误状态
    ERROR,                      // 错误状态
    RECOVERY                    // 恢复状态
};

// 状态转换事件枚举
enum class CHTLStateEvent {
    // 基础事件
    START,                      // 开始
    END,                        // 结束
    RESET,                      // 重置
    
    // 注释事件
    SINGLE_LINE_COMMENT_START,  // 单行注释开始
    MULTI_LINE_COMMENT_START,   // 多行注释开始
    GENERATOR_COMMENT_START,    // 生成器注释开始
    COMMENT_END,                // 注释结束
    
    // 文本事件
    TEXT_KEYWORD_FOUND,         // 找到text关键字
    TEXT_CONTENT_FOUND,         // 找到文本内容
    TEXT_BRACE_OPEN,            // text块开始大括号
    TEXT_BRACE_CLOSE,           // text块结束大括号
    
    // 字面量事件
    LITERAL_START,              // 字面量开始
    QUOTE_FOUND,                // 找到引号
    LITERAL_END,                // 字面量结束
    
    // 元素事件
    ELEMENT_NAME_FOUND,         // 找到元素名称
    ELEMENT_BRACE_OPEN,         // 元素开始大括号
    ELEMENT_BRACE_CLOSE,        // 元素结束大括号
    ELEMENT_SELF_CLOSING,       // 元素自闭合
    
    // 属性事件
    ATTRIBUTE_NAME_FOUND,       // 找到属性名
    ATTRIBUTE_SEPARATOR_FOUND,  // 找到属性分隔符
    ATTRIBUTE_VALUE_FOUND,      // 找到属性值
    ATTRIBUTE_TERMINATOR_FOUND, // 找到属性结束符
    
    // 样式事件
    STYLE_KEYWORD_FOUND,        // 找到style关键字
    STYLE_BRACE_OPEN,           // 样式块开始大括号
    STYLE_BRACE_CLOSE,          // 样式块结束大括号
    CSS_SELECTOR_FOUND,         // 找到CSS选择器
    CSS_PROPERTY_FOUND,         // 找到CSS属性
    CSS_VALUE_FOUND,            // 找到CSS值
    CONTEXT_REFERENCE_FOUND,    // 找到上下文引用
    
    // 模板事件
    TEMPLATE_KEYWORD_FOUND,     // 找到[Template]关键字
    TEMPLATE_TYPE_FOUND,        // 找到模板类型
    TEMPLATE_NAME_FOUND,        // 找到模板名称
    TEMPLATE_BRACE_OPEN,        // 模板体开始大括号
    TEMPLATE_BRACE_CLOSE,       // 模板体结束大括号
    TEMPLATE_INHERITANCE_FOUND, // 找到模板继承
    
    // 自定义事件
    CUSTOM_KEYWORD_FOUND,       // 找到[Custom]关键字
    CUSTOM_TYPE_FOUND,          // 找到自定义类型
    CUSTOM_NAME_FOUND,          // 找到自定义名称
    CUSTOM_BRACE_OPEN,          // 自定义体开始大括号
    CUSTOM_BRACE_CLOSE,         // 自定义体结束大括号
    DELETE_KEYWORD_FOUND,       // 找到delete关键字
    INSERT_KEYWORD_FOUND,       // 找到insert关键字
    REPLACE_KEYWORD_FOUND,      // 找到replace关键字
    
    // 原始嵌入事件
    ORIGIN_KEYWORD_FOUND,       // 找到[Origin]关键字
    ORIGIN_TYPE_FOUND,          // 找到原始嵌入类型
    ORIGIN_CONTENT_START,       // 原始嵌入内容开始
    
    // 配置组事件
    CONFIGURATION_KEYWORD_FOUND, // 找到[Configuration]关键字
    NAME_KEYWORD_FOUND,          // 找到[Name]关键字
    CONFIG_OPTION_FOUND,         // 找到配置选项
    
    // 导入事件
    IMPORT_KEYWORD_FOUND,       // 找到[Import]关键字
    IMPORT_TYPE_FOUND,          // 找到导入类型
    IMPORT_PATH_FOUND,          // 找到导入路径
    IMPORT_NAME_FOUND,          // 找到导入名称
    
    // 命名空间事件
    NAMESPACE_KEYWORD_FOUND,    // 找到[Namespace]关键字
    NAMESPACE_NAME_FOUND,       // 找到命名空间名称
    NAMESPACE_BRACE_OPEN,       // 命名空间体开始大括号
    NAMESPACE_BRACE_CLOSE,      // 命名空间体结束大括号
    
    // 约束事件
    CONSTRAINT_KEYWORD_FOUND,   // 找到except关键字
    CONSTRAINT_TARGET_FOUND,    // 找到约束目标
    CONSTRAINT_TYPE_FOUND,      // 找到约束类型
    
    // 脚本事件
    SCRIPT_KEYWORD_FOUND,       // 找到script关键字
    SCRIPT_BRACE_OPEN,          // 脚本块开始大括号
    SCRIPT_BRACE_CLOSE,         // 脚本块结束大括号
    
    // 模块信息事件
    INFO_KEYWORD_FOUND,         // 找到[Info]关键字
    INFO_PROPERTY_FOUND,        // 找到信息属性
    EXPORT_KEYWORD_FOUND,       // 找到[Export]关键字
    
    // 错误事件
    ERROR_OCCURRED,             // 发生错误
    RECOVERY_ATTEMPTED,         // 尝试恢复
    RECOVERY_SUCCEEDED,         // 恢复成功
    RECOVERY_FAILED             // 恢复失败
};

// 状态转换规则
struct CHTLStateTransition {
    CHTLState from_state;
    CHTLStateEvent event;
    CHTLState to_state;
    std::string description;
    bool is_valid;
    
    CHTLStateTransition(CHTLState from, CHTLStateEvent evt, CHTLState to, 
                   const std::string& desc = "", bool valid = true)
        : from_state(from), event(evt), to_state(to), description(desc), is_valid(valid) {}
};

// 状态机配置
struct StateMachineConfig {
    bool enable_auto_recovery;      // 是否启用自动恢复
    bool enable_state_history;      // 是否启用状态历史
    bool enable_performance_tracking; // 是否启用性能跟踪
    size_t max_state_stack_depth;   // 最大状态栈深度
    size_t max_history_size;        // 最大历史记录大小
    std::chrono::milliseconds state_timeout; // 状态超时时间
    
    StateMachineConfig() : 
        enable_auto_recovery(true), enable_state_history(true), enable_performance_tracking(true),
        max_state_stack_depth(100), max_history_size(1000), state_timeout(5000) {}
};

// RAII状态机类
class CHTLStateMachine {
public:
    CHTLStateMachine(const StateMachineConfig& config = StateMachineConfig());
    ~CHTLStateMachine();
    
    // 状态管理
    CHTLState getCurrentState() const;
    CHTLState getPreviousState() const;
    std::vector<CHTLState> getStateHistory() const;
    size_t getStateStackDepth() const;
    
    // 状态转换
    bool transitionTo(CHTLStateEvent event);
    bool canTransitionTo(CHTLStateEvent event) const;
    std::vector<CHTLStateEvent> getValidTransitions() const;
    
    // 状态栈管理
    void pushState(CHTLState state);
    CHTLState popState();
    CHTLState peekState() const;
    void clearStateStack();
    
    // 状态验证
    bool isValidState(CHTLState state) const;
    bool isInState(CHTLState state) const;
    bool isInAnyState(const std::vector<CHTLState>& states) const;
    
    // 性能跟踪
    std::chrono::milliseconds getStateDuration() const;
    std::chrono::milliseconds getTotalDuration() const;
    size_t getTransitionCount() const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error_message);
    
    // 配置管理
    void setConfig(const StateMachineConfig& config);
    StateMachineConfig getConfig() const;
    
    // 调试和诊断
    std::string getDebugInfo() const;
    std::string getStateTransitionLog() const;
    void enableDebugMode(bool enable);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
    void initializeStateTransitions();
    bool validateTransition(CHTLState from, CHTLStateEvent event, CHTLState to) const;
    void recordStateTransition(CHTLState from, CHTLStateEvent event, CHTLState to);
    void updatePerformanceMetrics();
    void handleStateTimeout();
    void attemptAutoRecovery();
};

} // namespace chtl