#ifndef CHTL_COMMON_STATEMANAGER_H
#define CHTL_COMMON_STATEMANAGER_H

#include <stack>
#include <memory>
#include <string>
#include <functional>
#include "Common/State.h"
#include "Common/Context.h"
#include "Lexer/Token.h"

namespace Chtl {

// RAII状态守卫 - 自动管理状态的进入和退出
class StateGuard {
public:
    StateGuard(State* state, CompilerState newState);
    ~StateGuard();
    
    // 禁止拷贝
    StateGuard(const StateGuard&) = delete;
    StateGuard& operator=(const StateGuard&) = delete;
    
    // 允许移动
    StateGuard(StateGuard&& other) noexcept;
    StateGuard& operator=(StateGuard&& other) noexcept;
    
    // 提前退出
    void release();
    
private:
    State* state_;
    bool released_;
};

// RAII元素守卫 - 自动管理元素栈
class ElementGuard {
public:
    ElementGuard(State* state, const ElementState& element);
    ~ElementGuard();
    
    // 禁止拷贝
    ElementGuard(const ElementGuard&) = delete;
    ElementGuard& operator=(const ElementGuard&) = delete;
    
    // 允许移动
    ElementGuard(ElementGuard&& other) noexcept;
    ElementGuard& operator=(ElementGuard&& other) noexcept;
    
    // 获取当前元素状态
    ElementState& getElement();
    const ElementState& getElement() const;
    
    // 提前退出
    void release();
    
private:
    State* state_;
    bool released_;
};

// RAII作用域守卫 - 自动管理作用域
class ScopeGuard {
public:
    ScopeGuard(Context* context, const std::string& scopeName = "");
    ~ScopeGuard();
    
    // 禁止拷贝
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    
    // 允许移动
    ScopeGuard(ScopeGuard&& other) noexcept;
    ScopeGuard& operator=(ScopeGuard&& other) noexcept;
    
    // 提前退出
    void release();
    
private:
    Context* context_;
    bool released_;
};

// 状态机管理器 - 协调状态管理
class StateManager {
public:
    StateManager(Context* context);
    ~StateManager();
    
    // 创建状态守卫
    [[nodiscard]] StateGuard enterState(CompilerState state);
    
    // 创建元素守卫
    [[nodiscard]] ElementGuard enterElement(const std::string& tagName);
    [[nodiscard]] ElementGuard enterElement(const ElementState& element);
    
    // 创建作用域守卫
    [[nodiscard]] ScopeGuard enterScope(const std::string& scopeName = "");
    
    // 状态查询
    bool isInElement() const;
    bool isInStyleBlock() const;
    bool isInScriptBlock() const;
    bool isInTemplate() const;
    bool isInCustom() const;
    bool isInLocalContext() const;  // 局部样式或脚本
    
    // 获取当前上下文信息
    std::string getCurrentElementTag() const;
    std::string getCurrentElementId() const;
    std::vector<std::string> getCurrentElementClasses() const;
    
    // 状态验证
    bool canEnterState(CompilerState state) const;
    bool validateStateTransition(CompilerState from, CompilerState to) const;
    
    // 错误恢复
    void recoverToStableState();
    
    // 状态快照
    struct StateSnapshot {
        std::stack<CompilerState> stateStack;
        std::stack<ElementState> elementStack;
        CssState cssState;
        std::string currentScope;
        int braceCount;
    };
    
    StateSnapshot captureSnapshot() const;
    void restoreSnapshot(const StateSnapshot& snapshot);
    
private:
    Context* context_;
    State* state_;
    
    // 验证状态转换的合法性
    bool isValidTransition(CompilerState from, CompilerState to) const;
    
    // 获取稳定状态
    CompilerState findStableState() const;
};

// 解析上下文助手 - 提供更高级的上下文操作
class ParseContextHelper {
public:
    ParseContextHelper(StateManager* stateManager, Context* context);
    
    // 进入特定语法结构
    [[nodiscard]] StateGuard enterTextBlock();
    [[nodiscard]] StateGuard enterStyleBlock(bool isLocal = true);
    [[nodiscard]] StateGuard enterScriptBlock(bool isLocal = true);
    [[nodiscard]] StateGuard enterTemplate(const std::string& type);
    [[nodiscard]] StateGuard enterCustom(const std::string& type);
    [[nodiscard]] StateGuard enterOriginBlock(const std::string& type);
    
    // 样式相关
    void beginCSSRule(const std::string& selector);
    void endCSSRule();
    void setAutoClassName(const std::string& className);
    std::string generateAutoClassIfNeeded();
    
    // 脚本相关
    void markChtlJsSyntax();
    bool hasChtlJsSyntax() const;
    
    // 模板和自定义相关
    void registerTemplate(const std::string& type, const std::string& name);
    void registerCustom(const std::string& type, const std::string& name);
    bool isInheritanceContext() const;
    
    // 错误处理
    void reportError(const std::string& message, const Token& token);
    void reportWarning(const std::string& message, const Token& token);
    
    // 位置跟踪
    void updatePosition(const Token& token);
    
private:
    StateManager* stateManager_;
    Context* context_;
};

// 生成上下文助手 - 为生成器提供状态管理
class GenerateContextHelper {
public:
    GenerateContextHelper(Context* context);
    
    // 输出流管理
    enum class OutputTarget {
        HTML,
        CSS,
        JAVASCRIPT
    };
    
    void setOutputTarget(OutputTarget target);
    OutputTarget getCurrentTarget() const;
    
    // 样式处理
    bool shouldGenerateAutoClass() const;
    std::string getCurrentAutoClass() const;
    void processLocalStyleContext(const std::string& selector);
    
    // 模板展开
    bool isTemplateExpansion() const;
    void beginTemplateExpansion(const std::string& templateName);
    void endTemplateExpansion();
    
    // 变量替换
    std::string resolveVariable(const std::string& groupName, const std::string& varName);
    bool hasVariable(const std::string& groupName, const std::string& varName) const;
    
    // 原始嵌入处理
    OutputTarget determineOriginTarget(const std::string& originType) const;
    
private:
    Context* context_;
    OutputTarget currentTarget_;
    std::stack<std::string> templateExpansionStack_;
};

} // namespace Chtl

#endif // CHTL_COMMON_STATEMANAGER_H