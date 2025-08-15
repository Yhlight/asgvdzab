#pragma once

#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <memory>
#include "common/types.hpp"
#include "lexer/chtl_tokens.hpp"
#include "lexer/chtl_js_tokens.hpp"

namespace chtl {

/**
 * 词法分析器状态类型
 */
enum class LexerStateType {
    INITIAL,                 // 初始状态
    IN_CHTL_BLOCK,          // 在CHTL块中
    IN_CHTL_JS_BLOCK,       // 在CHTL JS块中
    IN_CSS_BLOCK,           // 在CSS块中
    IN_JAVASCRIPT_BLOCK,    // 在JavaScript块中
    IN_TEXT_BLOCK,          // 在text{}块中
    IN_STYLE_BLOCK,         // 在style{}块中
    IN_SCRIPT_BLOCK,        // 在script{}块中
    IN_TEMPLATE_BLOCK,      // 在[Template]块中
    IN_CUSTOM_BLOCK,        // 在[Custom]块中
    IN_ORIGIN_BLOCK,        // 在[Origin]块中
    IN_IMPORT_BLOCK,        // 在[Import]块中
    IN_NAMESPACE_BLOCK,     // 在[Namespace]块中
    IN_CONFIGURATION_BLOCK, // 在[Configuration]块中
    IN_STRING_DOUBLE,       // 在双引号字符串中
    IN_STRING_SINGLE,       // 在单引号字符串中
    IN_STRING_UNQUOTED,     // 在无引号字符串中
    IN_COMMENT_LINE,        // 在单行注释中
    IN_COMMENT_MULTILINE,   // 在多行注释中
    IN_COMMENT_GENERATOR,   // 在生成器注释中
    IN_ENHANCED_SELECTOR,   // 在增强选择器{{}}中
    IN_ATTRIBUTE_LIST,      // 在属性列表中
    IN_ELEMENT_BODY,        // 在元素体中
    ERROR_STATE             // 错误状态
};

/**
 * 词法分析器上下文
 */
struct LexerContext {
    LexerStateType state;           // 当前状态
    std::string blockType;          // 块类型 (@Style, @Element等)
    std::string blockName;          // 块名称
    Position startPosition;         // 块开始位置
    size_t braceDepth;             // 大括号深度
    size_t bracketDepth;           // 方括号深度
    size_t parenDepth;             // 圆括号深度
    bool expectingValue;           // 是否期待值（在CE对等式后）
    bool inSpecialization;         // 是否在特例化中
    
    LexerContext() : state(LexerStateType::INITIAL), braceDepth(0), 
                     bracketDepth(0), parenDepth(0), expectingValue(false), 
                     inSpecialization(false) {}
    
    LexerContext(LexerStateType s, const std::string& type = "", const std::string& name = "")
        : state(s), blockType(type), blockName(name), braceDepth(0), 
          bracketDepth(0), parenDepth(0), expectingValue(false), 
          inSpecialization(false) {}
};

/**
 * 词法分析器状态管理器
 */
class LexerStateManager {
public:
    LexerStateManager();
    
    // === 状态管理 ===
    
    /**
     * 推入新状态
     */
    void pushState(LexerStateType state, const std::string& blockType = "", 
                   const std::string& blockName = "");
    
    /**
     * 弹出当前状态
     */
    bool popState();
    
    /**
     * 获取当前状态
     */
    LexerStateType getCurrentState() const;
    
    /**
     * 获取当前上下文
     */
    const LexerContext& getCurrentContext() const;
    
    /**
     * 获取当前上下文（可修改）
     */
    LexerContext& getCurrentContext();
    
    /**
     * 检查是否在指定状态中
     */
    bool isInState(LexerStateType state) const;
    
    /**
     * 检查是否在任何指定状态中
     */
    bool isInAnyState(const std::vector<LexerStateType>& states) const;
    
    /**
     * 获取状态栈深度
     */
    size_t getStackDepth() const;
    
    // === 括号管理 ===
    
    /**
     * 增加大括号深度
     */
    void incrementBraceDepth();
    
    /**
     * 减少大括号深度
     */
    void decrementBraceDepth();
    
    /**
     * 增加方括号深度
     */
    void incrementBracketDepth();
    
    /**
     * 减少方括号深度
     */
    void decrementBracketDepth();
    
    /**
     * 增加圆括号深度
     */
    void incrementParenDepth();
    
    /**
     * 减少圆括号深度
     */
    void decrementParenDepth();
    
    /**
     * 获取当前大括号深度
     */
    size_t getBraceDepth() const;
    
    /**
     * 获取当前方括号深度
     */
    size_t getBracketDepth() const;
    
    /**
     * 获取当前圆括号深度
     */
    size_t getParenDepth() const;
    
    // === 状态查询 ===
    
    /**
     * 是否在字符串中
     */
    bool isInString() const;
    
    /**
     * 是否在注释中
     */
    bool isInComment() const;
    
    /**
     * 是否在CHTL块中
     */
    bool isInCHTLBlock() const;
    
    /**
     * 是否在CHTL JS块中
     */
    bool isInCHTLJSBlock() const;
    
    /**
     * 是否在CSS块中
     */
    bool isInCSSBlock() const;
    
    /**
     * 是否在JavaScript块中
     */
    bool isInJavaScriptBlock() const;
    
    /**
     * 是否在特殊结构块中（Template, Custom等）
     */
    bool isInSpecialBlock() const;
    
    /**
     * 是否期待值
     */
    bool isExpectingValue() const;
    
    /**
     * 设置期待值状态
     */
    void setExpectingValue(bool expecting);
    
    /**
     * 是否在特例化中
     */
    bool isInSpecialization() const;
    
    /**
     * 设置特例化状态
     */
    void setInSpecialization(bool inSpec);
    
    // === 状态转换 ===
    
    /**
     * 根据Token转换状态
     */
    bool transitionOnToken(const CHTLToken& token);
    
    /**
     * 根据CHTL JS Token转换状态
     */
    bool transitionOnCHTLJSToken(const CHTLJSToken& token);
    
    /**
     * 根据字符转换状态
     */
    bool transitionOnCharacter(char ch, Position pos);
    
    // === 错误处理 ===
    
    /**
     * 设置错误状态
     */
    void setErrorState(const std::string& error, Position pos);
    
    /**
     * 是否处于错误状态
     */
    bool isInErrorState() const;
    
    /**
     * 获取错误信息
     */
    const std::string& getErrorMessage() const;
    
    /**
     * 获取错误位置
     */
    Position getErrorPosition() const;
    
    /**
     * 清除错误状态
     */
    void clearError();
    
    // === 调试和诊断 ===
    
    /**
     * 获取状态栈的字符串表示
     */
    std::string getStackTrace() const;
    
    /**
     * 获取当前状态的详细信息
     */
    std::string getCurrentStateInfo() const;
    
    /**
     * 重置所有状态
     */
    void reset();
    
    /**
     * 验证状态的一致性
     */
    bool validateState() const;

private:
    std::stack<LexerContext> stateStack_;    // 状态栈
    std::string errorMessage_;               // 错误信息
    Position errorPosition_;                 // 错误位置
    
    // 私有方法
    
    /**
     * 确定块类型的状态
     */
    LexerStateType determineBlockState(const std::string& blockType) const;
    
    /**
     * 检查状态转换是否有效
     */
    bool isValidTransition(LexerStateType from, LexerStateType to) const;
    
    /**
     * 处理特殊关键字
     */
    void handleSpecialKeyword(const std::string& keyword);
    
    /**
     * 获取状态的字符串表示
     */
    std::string stateToString(LexerStateType state) const;
};

/**
 * 状态工具类
 */
class StateUtils {
public:
    /**
     * 检查状态是否为块状态
     */
    static bool isBlockState(LexerStateType state);
    
    /**
     * 检查状态是否为字符串状态
     */
    static bool isStringState(LexerStateType state);
    
    /**
     * 检查状态是否为注释状态
     */
    static bool isCommentState(LexerStateType state);
    
    /**
     * 获取状态的优先级
     */
    static int getStatePriority(LexerStateType state);
    
    /**
     * 检查状态是否允许嵌套
     */
    static bool allowsNesting(LexerStateType state);
    
    /**
     * 获取状态对应的代码段类型
     */
    static CodeSegmentType getCodeSegmentType(LexerStateType state);
    
    /**
     * 从代码段类型获取状态
     */
    static LexerStateType getStateFromCodeSegmentType(CodeSegmentType type);
};

/**
 * 状态监听器接口
 */
class IStateListener {
public:
    virtual ~IStateListener() = default;
    
    virtual void onStateEntered(LexerStateType state, const LexerContext& context) = 0;
    virtual void onStateExited(LexerStateType state, const LexerContext& context) = 0;
    virtual void onStateTransition(LexerStateType from, LexerStateType to, const LexerContext& context) = 0;
    virtual void onError(const std::string& error, Position pos) = 0;
};

/**
 * 状态监听器管理器
 */
class StateListenerManager {
public:
    void addListener(std::shared_ptr<IStateListener> listener);
    void removeListener(std::shared_ptr<IStateListener> listener);
    void notifyStateEntered(LexerStateType state, const LexerContext& context);
    void notifyStateExited(LexerStateType state, const LexerContext& context);
    void notifyStateTransition(LexerStateType from, LexerStateType to, const LexerContext& context);
    void notifyError(const std::string& error, Position pos);

private:
    std::vector<std::shared_ptr<IStateListener>> listeners_;
};

} // namespace chtl