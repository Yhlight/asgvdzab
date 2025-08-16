#pragma once

#include <string>
#include <stack>
#include <unordered_map>
#include <vector>
#include <memory>

namespace chtl {

// 词法分析器状态枚举
enum class LexerState {
    INITIAL,                    // 初始状态
    IN_COMMENT,                // 在注释中
    IN_SINGLE_LINE_COMMENT,    // 在单行注释中
    IN_MULTI_LINE_COMMENT,     // 在多行注释中
    IN_GENERATOR_COMMENT,      // 在生成器注释中
    IN_STRING,                 // 在字符串中
    IN_SINGLE_QUOTE_STRING,    // 在单引号字符串中
    IN_DOUBLE_QUOTE_STRING,    // 在双引号字符串中
    IN_UNQUOTED_LITERAL,      // 在无引号字面量中
    IN_TEMPLATE_DECLARATION,   // 在模板声明中
    IN_CUSTOM_DECLARATION,     // 在自定义声明中
    IN_ORIGIN_DECLARATION,     // 在原始嵌入声明中
    IN_CONFIGURATION_DECLARATION, // 在配置组声明中
    IN_IMPORT_DECLARATION,     // 在导入声明中
    IN_NAMESPACE_DECLARATION,  // 在命名空间声明中
    IN_ATTRIBUTE,              // 在属性中
    IN_ATTRIBUTE_VALUE,        // 在属性值中
    IN_STYLE_BLOCK,            // 在样式块中
    IN_CSS_RULE,               // 在CSS规则中
    IN_CSS_PROPERTY,           // 在CSS属性中
    IN_CSS_VALUE,              // 在CSS值中
    IN_SCRIPT_BLOCK,           // 在脚本块中
    IN_ENHANCED_SELECTOR,      // 在增强选择器中
    IN_ANIMATION_BLOCK,        // 在动画块中
    IN_FUNCTION_CALL,          // 在函数调用中
    IN_OBJECT_LITERAL,         // 在对象字面量中
    IN_ARRAY_LITERAL,          // 在数组字面量中
    IN_PARENTHESES,            // 在括号中
    IN_BRACES,                 // 在大括号中
    IN_BRACKETS,               // 在方括号中
    EXPECTING_COLON_OR_EQUAL,  // 期望冒号或等号
    EXPECTING_SEMICOLON,       // 期望分号
    EXPECTING_BRACE,           // 期望大括号
    EXPECTING_TEMPLATE_NAME,   // 期望模板名称
    EXPECTING_CUSTOM_NAME,     // 期望自定义名称
    EXPECTING_NAMESPACE_NAME,  // 期望命名空间名称
    EXPECTING_IMPORT_PATH,     // 期望导入路径
    EXPECTING_ATTRIBUTE_NAME,  // 期望属性名
    EXPECTING_ATTRIBUTE_VALUE, // 期望属性值
    EXPECTING_CSS_SELECTOR,    // 期望CSS选择器
    EXPECTING_CSS_PROPERTY,    // 期望CSS属性
    EXPECTING_CSS_VALUE,       // 期望CSS值
    EXPECTING_FUNCTION_NAME,   // 期望函数名
    EXPECTING_PARAMETER,       // 期望参数
    EXPECTING_ANIMATION_PROPERTY, // 期望动画属性
    EXPECTING_ANIMATION_VALUE,    // 期望动画值
    ERROR                       // 错误状态
};

// 状态转换信息
struct StateTransition {
    LexerState from_state;
    std::string trigger;       // 触发转换的字符或字符串
    LexerState to_state;
    bool push_stack;           // 是否压栈
    bool pop_stack;            // 是否弹栈
    std::string stack_token;   // 栈标记
    
    StateTransition(LexerState from, const std::string& trig, LexerState to, 
                   bool push = false, bool pop = false, const std::string& token = "")
        : from_state(from), trigger(trig), to_state(to), 
          push_stack(push), pop_stack(pop), stack_token(token) {}
};

// 词法分析器状态管理器
class LexerStateManager {
public:
    LexerStateManager();
    ~LexerStateManager();
    
    // 状态管理
    LexerState getCurrentState() const;
    void setState(LexerState new_state);
    bool canTransitionTo(LexerState new_state) const;
    
    // 状态转换
    bool transition(const std::string& trigger);
    bool transitionTo(LexerState new_state);
    void resetToInitial();
    
    // 栈管理
    void pushState(LexerState state, const std::string& token = "");
    bool popState();
    LexerState peekState() const;
    size_t getStackDepth() const;
    void clearStack();
    
    // 状态历史
    void recordStateChange(LexerState old_state, LexerState new_state, const std::string& trigger);
    std::vector<std::string> getStateHistory() const;
    void clearHistory();
    
    // 状态验证
    bool isValidState(LexerState state) const;
    bool isInBlockState() const;
    bool isInDeclarationState() const;
    bool isInStringState() const;
    bool isInCommentState() const;
    
    // 状态查询
    std::string getStateDescription(LexerState state) const;
    std::vector<LexerState> getValidTransitions() const;
    std::vector<std::string> getValidTriggers() const;
    
    // 错误处理
    void setErrorState(const std::string& error_message);
    bool isInErrorState() const;
    std::string getErrorMessage() const;
    void clearError();
    
    // 调试和诊断
    std::string getDebugInfo() const;
    std::string getStateStackInfo() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
    void initializeStateTransitions();
    bool isValidTransition(LexerState from, LexerState to) const;
    std::string getStateName(LexerState state) const;
};

} // namespace chtl