#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <stack>
#include "common/types.hpp"
#include "lexer/chtl_tokens.hpp"
#include "lexer/chtl_js_tokens.hpp"
#include "lexer/global_map.hpp"

namespace chtl {

/**
 * 上下文类型
 */
enum class ContextType {
    ROOT,                   // 根上下文
    NAMESPACE,              // 命名空间上下文
    TEMPLATE,               // 模板上下文
    CUSTOM,                 // 自定义上下文
    ELEMENT,                // 元素上下文
    ATTRIBUTE,              // 属性上下文
    STYLE,                  // 样式上下文
    SCRIPT,                 // 脚本上下文
    TEXT,                   // 文本上下文
    ORIGIN,                 // 原始嵌入上下文
    IMPORT,                 // 导入上下文
    CONFIGURATION           // 配置上下文
};

/**
 * 解析上下文
 */
struct ParseContext {
    ContextType type;                           // 上下文类型
    std::string name;                           // 上下文名称
    Position startPosition;                     // 开始位置
    Position endPosition;                       // 结束位置
    
    // 层次结构
    std::shared_ptr<ParseContext> parent;       // 父上下文
    std::vector<std::shared_ptr<ParseContext>> children; // 子上下文
    
    // 符号信息
    std::string symbolType;                     // 符号类型 (@Style, @Element等)
    std::string symbolName;                     // 符号名称
    
    // 作用域信息
    std::string currentNamespace;               // 当前命名空间
    std::unordered_map<std::string, std::string> localSymbols; // 局部符号表
    std::unordered_map<std::string, std::string> attributes;   // 属性映射
    
    // 状态信息
    bool isComplete;                            // 是否解析完成
    bool hasErrors;                             // 是否有错误
    std::vector<std::string> errors;            // 错误列表
    
    // 特例化信息（Custom上下文）
    struct SpecializationInfo {
        std::vector<std::string> deletedProperties;    // 删除的属性
        std::vector<std::string> deletedInheritances;  // 删除的继承
        std::unordered_map<std::string, std::string> insertions; // 插入操作
        std::unordered_map<std::string, std::string> replacements; // 替换操作
    } specialization;
    
    // 继承信息
    std::vector<std::string> inheritanceChain;  // 继承链
    
    ParseContext(ContextType t = ContextType::ROOT, const std::string& n = "")
        : type(t), name(n), isComplete(false), hasErrors(false) {}
    
    // 获取完整路径
    std::string getFullPath() const;
    
    // 添加子上下文
    void addChild(std::shared_ptr<ParseContext> child);
    
    // 查找子上下文
    std::shared_ptr<ParseContext> findChild(const std::string& name) const;
    
    // 添加错误
    void addError(const std::string& error, Position pos = Position{});
    
    // 设置属性
    void setAttribute(const std::string& key, const std::string& value);
    
    // 获取属性
    std::string getAttribute(const std::string& key, const std::string& defaultValue = "") const;
    
    // 检查是否有属性
    bool hasAttribute(const std::string& key) const;
    
    std::string toString() const;
};

/**
 * 上下文管理器
 */
class ContextManager {
public:
    ContextManager();
    
    // === 上下文栈管理 ===
    
    /**
     * 推入新上下文
     */
    void pushContext(ContextType type, const std::string& name = "", Position pos = Position{});
    
    /**
     * 弹出当前上下文
     */
    bool popContext(Position pos = Position{});
    
    /**
     * 获取当前上下文
     */
    std::shared_ptr<ParseContext> getCurrentContext() const;
    
    /**
     * 获取根上下文
     */
    std::shared_ptr<ParseContext> getRootContext() const;
    
    /**
     * 获取上下文栈深度
     */
    size_t getStackDepth() const;
    
    // === 上下文查询 ===
    
    /**
     * 查找指定类型的上下文
     */
    std::shared_ptr<ParseContext> findContextByType(ContextType type) const;
    
    /**
     * 查找指定名称的上下文
     */
    std::shared_ptr<ParseContext> findContextByName(const std::string& name) const;
    
    /**
     * 获取当前命名空间
     */
    std::string getCurrentNamespace() const;
    
    /**
     * 获取当前符号路径
     */
    std::string getCurrentSymbolPath() const;
    
    /**
     * 检查是否在指定上下文中
     */
    bool isInContext(ContextType type) const;
    
    /**
     * 检查是否在任何指定上下文中
     */
    bool isInAnyContext(const std::vector<ContextType>& types) const;
    
    // === 符号解析 ===
    
    /**
     * 解析符号引用
     */
    std::pair<SymbolType, std::string> resolveSymbolReference(const std::string& reference) const;
    
    /**
     * 查找符号定义
     */
    const SymbolDefinition* findSymbolDefinition(const std::string& name) const;
    
    /**
     * 注册局部符号
     */
    void registerLocalSymbol(const std::string& name, const std::string& type);
    
    /**
     * 查找局部符号
     */
    std::string findLocalSymbol(const std::string& name) const;
    
    // === 属性管理 ===
    
    /**
     * 设置当前上下文的属性
     */
    void setContextAttribute(const std::string& key, const std::string& value);
    
    /**
     * 获取当前上下文的属性
     */
    std::string getContextAttribute(const std::string& key, const std::string& defaultValue = "") const;
    
    /**
     * 获取继承的属性
     */
    std::string getInheritedAttribute(const std::string& key, const std::string& defaultValue = "") const;
    
    // === 继承处理 ===
    
    /**
     * 添加继承关系
     */
    void addInheritance(const std::string& parentSymbol);
    
    /**
     * 解析继承链
     */
    std::vector<std::string> resolveInheritanceChain() const;
    
    /**
     * 检查循环继承
     */
    bool hasCircularInheritance() const;
    
    // === 特例化处理 ===
    
    /**
     * 开始特例化
     */
    void beginSpecialization();
    
    /**
     * 结束特例化
     */
    void endSpecialization();
    
    /**
     * 添加删除操作
     */
    void addDeletion(const std::string& target);
    
    /**
     * 添加插入操作
     */
    void addInsertion(const std::string& position, const std::string& content);
    
    /**
     * 添加替换操作
     */
    void addReplacement(const std::string& target, const std::string& replacement);
    
    // === 错误处理 ===
    
    /**
     * 添加错误到当前上下文
     */
    void addError(const std::string& error, Position pos = Position{});
    
    /**
     * 获取所有错误
     */
    std::vector<std::string> getAllErrors() const;
    
    /**
     * 检查是否有错误
     */
    bool hasErrors() const;
    
    /**
     * 清除所有错误
     */
    void clearErrors();
    
    // === 验证 ===
    
    /**
     * 验证当前上下文的完整性
     */
    bool validateCurrentContext() const;
    
    /**
     * 验证整个上下文树
     */
    bool validateContextTree() const;
    
    /**
     * 验证符号引用
     */
    bool validateSymbolReferences() const;
    
    // === 调试和诊断 ===
    
    /**
     * 获取上下文栈的字符串表示
     */
    std::string getContextStackTrace() const;
    
    /**
     * 获取上下文树的字符串表示
     */
    std::string getContextTreeString() const;
    
    /**
     * 导出上下文信息到字符串
     */
    std::string exportContextInfo() const;
    
    /**
     * 重置所有上下文
     */
    void reset();
    
    // === GlobalMap 集成 ===
    
    /**
     * 设置全局映射
     */
    void setGlobalMap(std::shared_ptr<GlobalMap> globalMap);
    
    /**
     * 获取全局映射
     */
    std::shared_ptr<GlobalMap> getGlobalMap() const;

private:
    std::shared_ptr<ParseContext> rootContext_;      // 根上下文
    std::stack<std::shared_ptr<ParseContext>> contextStack_; // 上下文栈
    std::shared_ptr<GlobalMap> globalMap_;           // 全局映射
    
    // 私有方法
    
    /**
     * 创建新上下文
     */
    std::shared_ptr<ParseContext> createContext(ContextType type, const std::string& name, Position pos);
    
    /**
     * 构建完整路径
     */
    std::string buildFullPath(std::shared_ptr<ParseContext> context) const;
    
    /**
     * 查找上下文（递归）
     */
    std::shared_ptr<ParseContext> findContextRecursive(std::shared_ptr<ParseContext> context, 
                                                        const std::function<bool(std::shared_ptr<ParseContext>)>& predicate) const;
    
    /**
     * 收集错误（递归）
     */
    void collectErrors(std::shared_ptr<ParseContext> context, std::vector<std::string>& errors) const;
    
    /**
     * 验证上下文（递归）
     */
    bool validateContextRecursive(std::shared_ptr<ParseContext> context) const;
    
    /**
     * 构建上下文树字符串（递归）
     */
    void buildContextTreeString(std::shared_ptr<ParseContext> context, 
                                std::string& result, int depth) const;
};

/**
 * 上下文工具类
 */
class ContextUtils {
public:
    /**
     * 获取上下文类型的字符串表示
     */
    static std::string contextTypeToString(ContextType type);
    
    /**
     * 从字符串解析上下文类型
     */
    static ContextType contextTypeFromString(const std::string& str);
    
    /**
     * 检查上下文类型是否允许子上下文
     */
    static bool allowsChildren(ContextType type);
    
    /**
     * 检查上下文类型是否允许属性
     */
    static bool allowsAttributes(ContextType type);
    
    /**
     * 检查上下文类型是否允许继承
     */
    static bool allowsInheritance(ContextType type);
    
    /**
     * 检查上下文类型是否允许特例化
     */
    static bool allowsSpecialization(ContextType type);
    
    /**
     * 获取上下文的默认属性
     */
    static std::vector<std::string> getDefaultAttributes(ContextType type);
    
    /**
     * 检查属性名是否有效
     */
    static bool isValidAttributeName(const std::string& name);
    
    /**
     * 检查上下文名称是否有效
     */
    static bool isValidContextName(const std::string& name);
    
    /**
     * 规范化上下文名称
     */
    static std::string normalizeContextName(const std::string& name);
};

/**
 * 上下文监听器接口
 */
class IContextListener {
public:
    virtual ~IContextListener() = default;
    
    virtual void onContextEntered(std::shared_ptr<ParseContext> context) = 0;
    virtual void onContextExited(std::shared_ptr<ParseContext> context) = 0;
    virtual void onAttributeSet(std::shared_ptr<ParseContext> context, 
                                const std::string& key, const std::string& value) = 0;
    virtual void onErrorAdded(std::shared_ptr<ParseContext> context, 
                              const std::string& error, Position pos) = 0;
    virtual void onInheritanceAdded(std::shared_ptr<ParseContext> context, 
                                    const std::string& parentSymbol) = 0;
};

/**
 * 上下文监听器管理器
 */
class ContextListenerManager {
public:
    void addListener(std::shared_ptr<IContextListener> listener);
    void removeListener(std::shared_ptr<IContextListener> listener);
    void notifyContextEntered(std::shared_ptr<ParseContext> context);
    void notifyContextExited(std::shared_ptr<ParseContext> context);
    void notifyAttributeSet(std::shared_ptr<ParseContext> context, 
                            const std::string& key, const std::string& value);
    void notifyErrorAdded(std::shared_ptr<ParseContext> context, 
                          const std::string& error, Position pos);
    void notifyInheritanceAdded(std::shared_ptr<ParseContext> context, 
                                const std::string& parentSymbol);

private:
    std::vector<std::shared_ptr<IContextListener>> listeners_;
};

} // namespace chtl