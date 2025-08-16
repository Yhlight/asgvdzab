#pragma once

#include "state/raii_state_machine.hpp"
#include "common/types.hpp"
#include <memory>
#include <stack>
#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include <mutex>

namespace chtl {

/**
 * CHTL语法上下文类型
 * 严格按照CHTL语法文档.md定义的作用域和上下文
 */
enum class CHTLContextType {
    // 根上下文
    GLOBAL_SCOPE,                   // 全局作用域
    FILE_SCOPE,                     // 文件作用域
    
    // 结构化上下文
    ELEMENT_SCOPE,                  // 元素作用域 (div { })
    TEXT_SCOPE,                     // 文本作用域 (text { })
    STYLE_SCOPE,                    // 样式作用域 (style { })
    SCRIPT_SCOPE,                   // 脚本作用域 (script { })
    
    // 模板上下文 ([Template])
    TEMPLATE_SCOPE,                 // 模板作用域
    TEMPLATE_STYLE_SCOPE,           // 模板样式作用域
    TEMPLATE_ELEMENT_SCOPE,         // 模板元素作用域
    TEMPLATE_VAR_SCOPE,             // 模板变量作用域
    
    // 自定义上下文 ([Custom])
    CUSTOM_SCOPE,                   // 自定义作用域
    CUSTOM_STYLE_SCOPE,             // 自定义样式作用域
    CUSTOM_ELEMENT_SCOPE,           // 自定义元素作用域
    CUSTOM_VAR_SCOPE,               // 自定义变量作用域
    CUSTOM_SPECIALIZATION_SCOPE,    // 自定义特例化作用域
    
    // 原始嵌入上下文 ([Origin])
    ORIGIN_SCOPE,                   // 原始嵌入作用域
    ORIGIN_HTML_SCOPE,              // 原始HTML作用域
    ORIGIN_STYLE_SCOPE,             // 原始样式作用域
    ORIGIN_JAVASCRIPT_SCOPE,        // 原始JavaScript作用域
    
    // 导入上下文 ([Import])
    IMPORT_SCOPE,                   // 导入作用域
    
    // 命名空间上下文 ([Namespace])
    NAMESPACE_SCOPE,                // 命名空间作用域
    NESTED_NAMESPACE_SCOPE,         // 嵌套命名空间作用域
    
    // 配置上下文 ([Configuration])
    CONFIGURATION_SCOPE,            // 配置作用域
    NAME_GROUP_SCOPE,               // [Name]组作用域
    
    // 约束上下文
    EXCEPT_SCOPE,                   // except约束作用域
    
    // 局部样式专用上下文
    INLINE_STYLE_SCOPE,             // 内联样式作用域
    CLASS_SELECTOR_SCOPE,           // 类选择器作用域 (.className)
    ID_SELECTOR_SCOPE,              // ID选择器作用域 (#idName)
    CONTEXT_REFERENCE_SCOPE,        // 上下文推导作用域 (&)
    PSEUDO_SELECTOR_SCOPE,          // 伪选择器作用域 (&:hover, &::before)
    
    // CHTL JS专用上下文
    ENHANCED_SELECTOR_SCOPE,        // 增强选择器作用域 ({{selector}})
    LISTEN_CONFIG_SCOPE,            // listen配置作用域
    DELEGATE_CONFIG_SCOPE,          // delegate配置作用域
    ANIMATE_CONFIG_SCOPE,           // animate配置作用域
    
    // 属性上下文
    ATTRIBUTE_SCOPE,                // 属性作用域
    ATTRIBUTE_VALUE_SCOPE,          // 属性值作用域
    
    // 变量上下文
    VAR_DECLARATION_SCOPE,          // 变量声明作用域
    VAR_USAGE_SCOPE,                // 变量使用作用域
    VAR_SPECIALIZATION_SCOPE,       // 变量特例化作用域
    
    // 特殊上下文
    COMMENT_SCOPE,                  // 注释作用域
    STRING_LITERAL_SCOPE,           // 字符串字面量作用域
    ERROR_RECOVERY_SCOPE            // 错误恢复作用域
};

/**
 * 上下文信息结构
 */
struct ContextInfo {
    CHTLContextType type;           // 上下文类型
    std::string name;               // 上下文名称
    std::string identifier;         // 标识符（如元素名、模板名等）
    Position startPosition;         // 开始位置
    Position endPosition;           // 结束位置
    std::unordered_map<std::string, std::string> properties; // 上下文属性
    
    ContextInfo(CHTLContextType t, const std::string& n, const Position& start)
        : type(t), name(n), startPosition(start), endPosition(start) {}
};

/**
 * RAII上下文守卫类
 * 自动管理上下文的进入和退出
 */
class ContextGuard {
public:
    ContextGuard(class ContextManager& manager, CHTLContextType contextType, 
                const std::string& identifier = "", const std::string& info = "");
    ~ContextGuard();
    
    // 禁止拷贝和赋值
    ContextGuard(const ContextGuard&) = delete;
    ContextGuard& operator=(const ContextGuard&) = delete;
    
    // 支持移动
    ContextGuard(ContextGuard&& other) noexcept;
    ContextGuard& operator=(ContextGuard&& other) noexcept;
    
    // 提前释放守卫
    void release();
    
    // 检查守卫是否有效
    bool isValid() const { return manager_ != nullptr; }
    
    // 设置上下文属性
    void setProperty(const std::string& key, const std::string& value);
    
    // 获取上下文信息
    const ContextInfo& getContextInfo() const;

private:
    class ContextManager* manager_;
    bool released_;
    ContextInfo contextInfo_;
};

/**
 * 上下文变化监听器接口
 */
class IContextChangeListener {
public:
    virtual ~IContextChangeListener() = default;
    virtual void onContextEnter(CHTLContextType type, const std::string& identifier) = 0;
    virtual void onContextExit(CHTLContextType type, const std::string& identifier) = 0;
    virtual void onContextProperty(CHTLContextType type, const std::string& key, const std::string& value) = 0;
    virtual void onContextError(CHTLContextType type, const std::string& error) = 0;
};

/**
 * 符号表项
 */
struct SymbolEntry {
    std::string name;               // 符号名称
    CHTLContextType declaringContext; // 声明上下文
    std::string type;               // 符号类型
    Position position;              // 声明位置
    std::unordered_map<std::string, std::string> attributes; // 符号属性
    bool isVisible;                 // 是否可见
    
    SymbolEntry(const std::string& n, CHTLContextType ctx, const std::string& t, const Position& pos)
        : name(n), declaringContext(ctx), type(t), position(pos), isVisible(true) {}
};

/**
 * 符号解析规则
 */
struct SymbolResolutionRule {
    CHTLContextType contextType;    // 适用的上下文类型
    std::string pattern;            // 符号模式
    std::function<bool(const std::string&, const ContextInfo&)> resolver; // 解析函数
    int priority;                   // 优先级
    
    SymbolResolutionRule(CHTLContextType ctx, const std::string& pat, int prio = 0)
        : contextType(ctx), pattern(pat), priority(prio) {}
};

/**
 * RAII上下文管理器
 * 严格按照CHTL语法文档管理解析上下文和符号表
 */
class ContextManager {
public:
    explicit ContextManager();
    ~ContextManager();
    
    // ===== 上下文管理 =====
    
    /**
     * 获取当前上下文类型
     */
    CHTLContextType getCurrentContextType() const;
    
    /**
     * 获取当前上下文信息
     */
    const ContextInfo& getCurrentContext() const;
    
    /**
     * 创建RAII上下文守卫
     * @param contextType 上下文类型
     * @param identifier 标识符
     * @param info 附加信息
     * @return 上下文守卫对象
     */
    ContextGuard createGuard(CHTLContextType contextType, 
                           const std::string& identifier = "", 
                           const std::string& info = "");
    
    /**
     * 检查是否在指定上下文中
     * @param contextType 上下文类型
     * @return 是否在指定上下文中
     */
    bool isInContext(CHTLContextType contextType) const;
    
    /**
     * 检查是否在任一指定上下文中
     * @param contextTypes 上下文类型列表
     * @return 是否在任一上下文中
     */
    bool isInAnyContext(const std::vector<CHTLContextType>& contextTypes) const;
    
    /**
     * 获取上下文栈深度
     * @return 栈深度
     */
    size_t getContextDepth() const;
    
    /**
     * 获取完整的上下文路径
     * @return 上下文路径字符串
     */
    std::string getContextPath() const;
    
    // ===== 符号表管理 =====
    
    /**
     * 声明符号
     * @param name 符号名称
     * @param type 符号类型
     * @param position 声明位置
     * @param attributes 符号属性
     * @return 是否声明成功
     */
    bool declareSymbol(const std::string& name, const std::string& type, 
                      const Position& position, 
                      const std::unordered_map<std::string, std::string>& attributes = {});
    
    /**
     * 查找符号
     * @param name 符号名称
     * @return 符号条目，如果未找到则返回nullptr
     */
    const SymbolEntry* findSymbol(const std::string& name) const;
    
    /**
     * 查找符号（指定上下文）
     * @param name 符号名称
     * @param contextType 上下文类型
     * @return 符号条目，如果未找到则返回nullptr
     */
    const SymbolEntry* findSymbolInContext(const std::string& name, CHTLContextType contextType) const;
    
    /**
     * 获取当前上下文的所有符号
     * @return 符号列表
     */
    std::vector<const SymbolEntry*> getCurrentContextSymbols() const;
    
    /**
     * 清空当前上下文的符号表
     */
    void clearCurrentContextSymbols();
    
    // ===== 命名空间管理 =====
    
    /**
     * 进入命名空间
     * @param namespaceName 命名空间名称
     */
    void enterNamespace(const std::string& namespaceName);
    
    /**
     * 退出命名空间
     */
    void exitNamespace();
    
    /**
     * 获取当前命名空间路径
     * @return 命名空间路径
     */
    std::string getCurrentNamespace() const;
    
    /**
     * 解析带命名空间的符号
     * @param fullName 完整名称 (namespace.symbol)
     * @return 符号条目，如果未找到则返回nullptr
     */
    const SymbolEntry* resolveNamespacedSymbol(const std::string& fullName) const;
    
    // ===== 模板和自定义管理 =====
    
    /**
     * 注册模板
     * @param templateType 模板类型 (@Style, @Element, @Var)
     * @param name 模板名称
     * @param position 声明位置
     * @param content 模板内容
     */
    void registerTemplate(const std::string& templateType, const std::string& name, 
                         const Position& position, const std::string& content = "");
    
    /**
     * 注册自定义
     * @param customType 自定义类型 (@Style, @Element, @Var)
     * @param name 自定义名称
     * @param position 声明位置
     * @param content 自定义内容
     */
    void registerCustom(const std::string& customType, const std::string& name, 
                       const Position& position, const std::string& content = "");
    
    /**
     * 查找模板
     * @param templateType 模板类型
     * @param name 模板名称
     * @return 符号条目，如果未找到则返回nullptr
     */
    const SymbolEntry* findTemplate(const std::string& templateType, const std::string& name) const;
    
    /**
     * 查找自定义
     * @param customType 自定义类型
     * @param name 自定义名称
     * @return 符号条目，如果未找到则返回nullptr
     */
    const SymbolEntry* findCustom(const std::string& customType, const std::string& name) const;
    
    // ===== 监听器管理 =====
    
    /**
     * 添加上下文变化监听器
     * @param listener 监听器
     */
    void addListener(std::shared_ptr<IContextChangeListener> listener);
    
    /**
     * 移除上下文变化监听器
     * @param listener 监听器
     */
    void removeListener(std::shared_ptr<IContextChangeListener> listener);
    
    // ===== 符号解析规则 =====
    
    /**
     * 添加符号解析规则
     * @param rule 解析规则
     */
    void addResolutionRule(const SymbolResolutionRule& rule);
    
    /**
     * 移除符号解析规则
     * @param contextType 上下文类型
     * @param pattern 符号模式
     */
    void removeResolutionRule(CHTLContextType contextType, const std::string& pattern);
    
    // ===== 调试和诊断 =====
    
    /**
     * 获取上下文类型名称
     * @param contextType 上下文类型
     * @return 上下文类型名称
     */
    static std::string getContextTypeName(CHTLContextType contextType);
    
    /**
     * 获取调试信息
     * @return 调试信息字符串
     */
    std::string getDebugInfo() const;
    
    /**
     * 转储符号表
     * @return 符号表完整信息
     */
    std::string dumpSymbolTable() const;
    
    /**
     * 转储上下文栈
     * @return 上下文栈信息
     */
    std::string dumpContextStack() const;

private:
    friend class ContextGuard;
    
    // 内部上下文管理（由ContextGuard调用）
    void enterContext(const ContextInfo& contextInfo);
    void exitContext();
    void setContextProperty(const std::string& key, const std::string& value);
    
    // 通知监听器
    void notifyContextEnter(CHTLContextType type, const std::string& identifier);
    void notifyContextExit(CHTLContextType type, const std::string& identifier);
    void notifyContextProperty(CHTLContextType type, const std::string& key, const std::string& value);
    void notifyContextError(const std::string& error);
    
    // 验证上下文转换
    bool validateContextTransition(CHTLContextType from, CHTLContextType to) const;
    
    // 初始化默认解析规则
    void initializeDefaultRules();
    
    // 符号表管理
    std::string generateSymbolKey(const std::string& name, CHTLContextType context) const;
    void cleanupContextSymbols(CHTLContextType contextType);

private:
    std::stack<ContextInfo> contextStack_;                          // 上下文栈
    std::unordered_map<std::string, SymbolEntry> symbolTable_;      // 符号表
    std::vector<std::string> namespaceStack_;                       // 命名空间栈
    std::vector<std::shared_ptr<IContextChangeListener>> listeners_; // 监听器列表
    std::vector<SymbolResolutionRule> resolutionRules_;             // 解析规则
    
    mutable std::mutex contextMutex_;                               // 上下文访问互斥锁
    Position currentPosition_;                                       // 当前位置
    bool errorRecoveryMode_;                                        // 错误恢复模式
};

/**
 * 上下文管理器工厂
 */
class ContextManagerFactory {
public:
    /**
     * 创建CHTL上下文管理器
     * @return CHTL上下文管理器实例
     */
    static std::unique_ptr<ContextManager> createCHTLContextManager();
    
    /**
     * 创建CHTL JS上下文管理器
     * @return CHTL JS上下文管理器实例
     */
    static std::unique_ptr<ContextManager> createCHTLJSContextManager();
    
    /**
     * 创建通用上下文管理器
     * @return 通用上下文管理器实例
     */
    static std::unique_ptr<ContextManager> createGenericContextManager();
};

// ===== 便利宏定义 =====

/**
 * 自动上下文守卫宏
 * 在当前作用域内自动管理上下文
 */
#define CHTL_CONTEXT_GUARD(manager, contextType) \
    auto __context_guard_##__LINE__ = (manager).createGuard(contextType, "", __func__)

/**
 * 带标识符的上下文守卫宏
 */
#define CHTL_CONTEXT_GUARD_WITH_ID(manager, contextType, identifier) \
    auto __context_guard_##__LINE__ = (manager).createGuard(contextType, identifier, __func__)

/**
 * 条件上下文守卫宏
 */
#define CHTL_CONTEXT_GUARD_IF(manager, contextType, condition) \
    auto __context_guard_##__LINE__ = (condition) ? \
        (manager).createGuard(contextType, "", __func__) : ContextGuard(manager, manager.getCurrentContextType())

/**
 * 上下文验证宏
 */
#define CHTL_VALIDATE_CONTEXT(manager, expected_context) \
    do { \
        if (!(manager).isInContext(expected_context)) { \
            throw std::runtime_error("Expected context " + ContextManager::getContextTypeName(expected_context) + \
                                    " but current context is " + ContextManager::getContextTypeName((manager).getCurrentContextType())); \
        } \
    } while(0)

/**
 * 符号声明宏
 */
#define CHTL_DECLARE_SYMBOL(manager, name, type, position) \
    (manager).declareSymbol(name, type, position)

/**
 * 符号查找宏
 */
#define CHTL_FIND_SYMBOL(manager, name) \
    (manager).findSymbol(name)

} // namespace chtl