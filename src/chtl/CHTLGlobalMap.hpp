#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

namespace chtl {

/**
 * 符号类型
 */
enum class SymbolType {
    TEMPLATE_STYLE,     // 样式组模板
    TEMPLATE_ELEMENT,   // 元素模板
    TEMPLATE_VAR,       // 变量组模板
    CUSTOM_STYLE,       // 自定义样式组
    CUSTOM_ELEMENT,     // 自定义元素
    CUSTOM_VAR,         // 自定义变量组
    NAMESPACE,          // 命名空间
    ORIGIN_HTML,        // 原始HTML
    ORIGIN_STYLE,       // 原始样式
    ORIGIN_JAVASCRIPT,  // 原始JavaScript
    CONFIGURATION,      // 配置
    HTML_ELEMENT,       // HTML元素
    VARIABLE            // 变量
};

/**
 * 符号信息
 */
struct SymbolInfo {
    std::string name;
    SymbolType type;
    std::string namespace_;  // 所属命名空间
    std::string filePath;    // 定义文件
    size_t line;
    size_t column;
    std::shared_ptr<void> data;  // 关联数据（AST节点等）
    
    SymbolInfo(const std::string& n, SymbolType t, const std::string& ns = "")
        : name(n), type(t), namespace_(ns), line(0), column(0) {}
};

/**
 * 作用域类型
 */
enum class ScopeType {
    GLOBAL,     // 全局作用域
    NAMESPACE,  // 命名空间作用域
    ELEMENT,    // 元素作用域
    BLOCK       // 块作用域
};

/**
 * 作用域
 */
class Scope {
public:
    Scope(ScopeType type, const std::string& name = "", Scope* parent = nullptr)
        : type_(type), name_(name), parent_(parent) {}
    
    // 添加符号
    void addSymbol(const std::string& name, std::shared_ptr<SymbolInfo> symbol);
    
    // 查找符号（包括父作用域）
    std::shared_ptr<SymbolInfo> findSymbol(const std::string& name) const;
    
    // 查找符号（仅当前作用域）
    std::shared_ptr<SymbolInfo> findLocalSymbol(const std::string& name) const;
    
    // 获取所有符号
    std::vector<std::shared_ptr<SymbolInfo>> getAllSymbols() const;
    
    // 获取父作用域
    Scope* getParent() const { return parent_; }
    
    // 获取作用域类型
    ScopeType getType() const { return type_; }
    
    // 获取作用域名称
    const std::string& getName() const { return name_; }
    
private:
    ScopeType type_;
    std::string name_;
    Scope* parent_;
    std::unordered_map<std::string, std::shared_ptr<SymbolInfo>> symbols_;
};

/**
 * 全局符号映射
 */
class CHTLGlobalMap {
public:
    CHTLGlobalMap();
    ~CHTLGlobalMap();
    
    // 作用域管理
    void enterScope(ScopeType type, const std::string& name = "");
    void exitScope();
    Scope* getCurrentScope() const { return currentScope_; }
    
    // 符号管理
    void addSymbol(const std::string& name, SymbolType type, const std::string& namespace_ = "");
    std::shared_ptr<SymbolInfo> findSymbol(const std::string& name) const;
    std::shared_ptr<SymbolInfo> findSymbol(const std::string& name, const std::string& namespace_) const;
    
    // 命名空间管理
    void enterNamespace(const std::string& name);
    void exitNamespace();
    std::string getCurrentNamespace() const;
    
    // 导入管理
    void addImport(const std::string& moduleName, const std::string& alias = "");
    bool hasImport(const std::string& moduleName) const;
    std::string getImportAlias(const std::string& moduleName) const;
    
    // 配置管理
    void setConfiguration(const std::string& key, const std::string& value);
    std::string getConfiguration(const std::string& key) const;
    
    // 清空
    void clear();
    
private:
    std::unique_ptr<Scope> globalScope_;
    Scope* currentScope_;
    std::vector<std::string> namespaceStack_;
    std::unordered_map<std::string, std::string> imports_;
    std::unordered_map<std::string, std::string> configurations_;
    
    // 获取完整的命名空间路径
    std::string getFullNamespacePath() const;
};

} // namespace chtl