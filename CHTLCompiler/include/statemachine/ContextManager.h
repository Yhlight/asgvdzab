#pragma once

#include "CHTLStateMachine.h"
#include "CHTLJSStateMachine.h"
#include "ast/ASTNode.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <stack>
#include <functional>
#include <chrono>

namespace chtl {

// 上下文类型枚举
enum class ContextType {
    GLOBAL,                     // 全局上下文
    ELEMENT,                    // 元素上下文
    ATTRIBUTE,                  // 属性上下文
    STYLE_BLOCK,               // 样式块上下文
    CSS_RULE,                  // CSS规则上下文
    TEMPLATE,                  // 模板上下文
    CUSTOM,                    // 自定义上下文
    ORIGIN,                    // 原始嵌入上下文
    IMPORT,                    // 导入上下文
    NAMESPACE,                 // 命名空间上下文
    CONSTRAINT,                // 约束上下文
    SCRIPT_BLOCK,              // 脚本块上下文
    ENHANCED_SELECTOR,         // 增强选择器上下文
    CHAIN_ACCESS,              // 链式访问上下文
    LISTEN_DECLARATION,        // 监听器声明上下文
    DELEGATE_DECLARATION,      // 事件委托声明上下文
    ANIMATE_DECLARATION,       // 动画声明上下文
    FUNCTION_DECLARATION,      // 函数声明上下文
    VARIABLE_DECLARATION,      // 变量声明上下文
    CONTROL_FLOW,              // 控制流上下文
    EXPRESSION,                // 表达式上下文
    OBJECT_LITERAL,            // 对象字面量上下文
    ARRAY_LITERAL,             // 数组字面量上下文
    COMMENT,                   // 注释上下文
    TEXT_NODE,                 // 文本节点上下文
    LITERAL                    // 字面量上下文
};

// 上下文信息结构
struct ContextInfo {
    ContextType type;                          // 上下文类型
    std::string name;                          // 上下文名称
    std::string namespace_path;                // 命名空间路径
    size_t line;                               // 开始行号
    size_t column;                             // 开始列号
    size_t start_position;                     // 开始位置
    size_t end_position;                       // 结束位置
    std::chrono::steady_clock::time_point start_time; // 开始时间
    std::chrono::steady_clock::time_point end_time;   // 结束时间
    std::unordered_map<std::string, std::string> metadata; // 元数据
    std::vector<std::string> constraints;      // 约束列表
    bool is_active;                            // 是否激活
    bool is_complete;                          // 是否完成
    
    ContextInfo(ContextType t, const std::string& n = "", const std::string& ns = "",
                size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : type(t), name(n), namespace_path(ns), line(l), column(c), 
          start_position(start), end_position(end), is_active(true), is_complete(false) {
        start_time = std::chrono::steady_clock::now();
    }
};

// 上下文栈项
struct ContextStackItem {
    std::shared_ptr<ContextInfo> context;      // 上下文信息
    std::shared_ptr<ASTNode> ast_node;         // 关联的AST节点
    std::vector<std::string> pending_operations; // 待执行操作
    size_t nesting_level;                      // 嵌套级别
    
    ContextStackItem(std::shared_ptr<ContextInfo> ctx, std::shared_ptr<ASTNode> node = nullptr, size_t level = 0)
        : context(ctx), ast_node(node), nesting_level(level) {}
};

// 上下文管理配置
struct ContextManagerConfig {
    bool enable_context_tracking;       // 是否启用上下文跟踪
    bool enable_context_validation;     // 是否启用上下文验证
    bool enable_context_history;        // 是否启用上下文历史
    bool enable_performance_tracking;   // 是否启用性能跟踪
    size_t max_context_stack_depth;     // 最大上下文栈深度
    size_t max_context_history_size;    // 最大上下文历史大小
    std::chrono::milliseconds context_timeout; // 上下文超时时间
    bool enable_auto_cleanup;           // 是否启用自动清理
    
    ContextManagerConfig() : 
        enable_context_tracking(true), enable_context_validation(true), enable_context_history(true),
        enable_performance_tracking(true), max_context_stack_depth(100), max_context_history_size(1000),
        context_timeout(10000ms), enable_auto_cleanup(true) {}
};

// 上下文管理协助器类
class ContextManager {
public:
    ContextManager(const ContextManagerConfig& config = ContextManagerConfig());
    ~ContextManager();
    
    // 上下文生命周期管理
    void enterContext(ContextType type, const std::string& name = "", 
                     const std::string& namespace_path = "", size_t line = 0, size_t column = 0, 
                     size_t start_position = 0, size_t end_position = 0);
    void exitContext();
    void completeContext();
    void cancelContext();
    
    // 上下文栈管理
    std::shared_ptr<ContextInfo> getCurrentContext() const;
    std::shared_ptr<ContextInfo> getParentContext() const;
    std::shared_ptr<ContextInfo> getContextAt(size_t index) const;
    size_t getContextStackDepth() const;
    std::vector<std::shared_ptr<ContextInfo>> getContextStack() const;
    
    // 上下文查询
    bool isInContext(ContextType type) const;
    bool isInContext(const std::string& name) const;
    bool isInNamespace(const std::string& namespace_path) const;
    bool isInAnyContext(const std::vector<ContextType>& types) const;
    bool isInAnyContext(const std::vector<std::string>& names) const;
    
    // 上下文验证
    bool validateContext(ContextType expected_type, const std::string& expected_name = "") const;
    bool validateContextTransition(ContextType from_type, ContextType to_type) const;
    bool validateContextConstraints(const std::vector<std::string>& constraints) const;
    
    // AST节点关联
    void associateASTNode(std::shared_ptr<ASTNode> node);
    std::shared_ptr<ASTNode> getAssociatedASTNode() const;
    void updateASTNodePosition(size_t end_position);
    
    // 约束管理
    void addConstraint(const std::string& constraint);
    void removeConstraint(const std::string& constraint);
    bool hasConstraint(const std::string& constraint) const;
    std::vector<std::string> getConstraints() const;
    void clearConstraints();
    
    // 元数据管理
    void setMetadata(const std::string& key, const std::string& value);
    std::string getMetadata(const std::string& key) const;
    bool hasMetadata(const std::string& key) const;
    void removeMetadata(const std::string& key);
    std::unordered_map<std::string, std::string> getAllMetadata() const;
    
    // 命名空间管理
    void pushNamespace(const std::string& namespace_name);
    void popNamespace();
    std::string getCurrentNamespace() const;
    std::vector<std::string> getNamespaceStack() const;
    bool isInNamespace(const std::string& namespace_name) const;
    
    // 性能跟踪
    std::chrono::milliseconds getContextDuration() const;
    std::chrono::milliseconds getTotalDuration() const;
    size_t getContextCount() const;
    std::chrono::milliseconds getAverageContextDuration() const;
    
    // 错误处理
    bool hasContextErrors() const;
    std::vector<std::string> getContextErrors() const;
    void clearContextErrors();
    void addContextError(const std::string& error_message);
    
    // 配置管理
    void setConfig(const ContextManagerConfig& config);
    ContextManagerConfig getConfig() const;
    
    // 调试和诊断
    std::string getDebugInfo() const;
    std::string getContextStackDump() const;
    std::string getContextHistoryDump() const;
    void enableDebugMode(bool enable);
    
    // 清理和重置
    void cleanup();
    void reset();
    void clearHistory();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
    void validateContextStackDepth();
    void updateContextTiming();
    void recordContextHistory();
    void performAutoCleanup();
    void validateContextIntegrity();
    std::string generateContextId() const;
    void logContextOperation(const std::string& operation);
    
    // 上下文验证规则
    bool isValidContextType(ContextType type) const;
    bool isValidContextName(const std::string& name) const;
    bool isValidNamespacePath(const std::string& namespace_path) const;
    bool isValidContextTransition(ContextType from, ContextType to) const;
    
    // 性能优化
    void optimizeContextStack();
    void cleanupInactiveContexts();
    void updatePerformanceMetrics();
};

// 上下文RAII包装器
class ContextScope {
public:
    ContextScope(ContextManager& manager, ContextType type, const std::string& name = "",
                 const std::string& namespace_path = "", size_t line = 0, size_t column = 0,
                 size_t start_position = 0, size_t end_position = 0);
    ~ContextScope();
    
    // 禁用拷贝
    ContextScope(const ContextScope&) = delete;
    ContextScope& operator=(const ContextScope&) = delete;
    
    // 允许移动
    ContextScope(ContextScope&& other) noexcept;
    ContextScope& operator=(ContextScope&& other) noexcept;
    
    // 上下文操作
    void complete();
    void cancel();
    std::shared_ptr<ContextInfo> getContext() const;
    bool isActive() const;

private:
    ContextManager* manager_;
    std::shared_ptr<ContextInfo> context_;
    bool is_active_;
};

// 命名空间RAII包装器
class NamespaceScope {
public:
    NamespaceScope(ContextManager& manager, const std::string& namespace_name);
    ~NamespaceScope();
    
    // 禁用拷贝
    NamespaceScope(const NamespaceScope&) = delete;
    NamespaceScope& operator=(const NamespaceScope&) = delete;
    
    // 允许移动
    NamespaceScope(NamespaceScope&& other) noexcept;
    NamespaceScope& operator=(NamespaceScope&& other) noexcept;

private:
    ContextManager* manager_;
    std::string namespace_name_;
};

} // namespace chtl