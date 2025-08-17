#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <variant>

namespace chtl {

// 全局符号类型
enum class SymbolType {
    TEMPLATE_STYLE,      // 样式组模板
    TEMPLATE_ELEMENT,    // 元素模板
    TEMPLATE_VAR,        // 变量组模板
    CUSTOM_STYLE,        // 自定义样式组
    CUSTOM_ELEMENT,      // 自定义元素
    CUSTOM_VAR,          // 自定义变量组
    NAMESPACE,           // 命名空间
    IMPORTED_MODULE,     // 导入的模块
    CSS_CLASS,           // CSS类
    CSS_ID,              // CSS ID
    CSS_PROPERTY,        // CSS属性
    HTML_ELEMENT,        // HTML元素
    FUNCTION,            // 函数
    VARIABLE             // 变量
};

// 符号信息结构
struct SymbolInfo {
    SymbolType type;
    std::string name;
    std::string namespace_path;  // 命名空间路径
    std::string source_file;     // 源文件
    size_t line;                 // 定义行号
    size_t column;               // 定义列号
    std::string value;           // 符号值
    std::vector<std::string> dependencies;  // 依赖关系
    
    SymbolInfo(SymbolType t, const std::string& n, const std::string& ns = "",
               const std::string& src = "", size_t l = 0, size_t c = 0)
        : type(t), name(n), namespace_path(ns), source_file(src), line(l), column(c) {}
    
    // 默认构造函数
    SymbolInfo() : type(SymbolType::VARIABLE), line(0), column(0) {}
};

// 全局符号表类
class GlobalMap {
public:
    GlobalMap();
    ~GlobalMap();
    
    // 符号管理
    bool addSymbol(const SymbolInfo& symbol);
    bool removeSymbol(const std::string& name, const std::string& namespace_path = "");
    SymbolInfo* findSymbol(const std::string& name, const std::string& namespace_path = "");
    std::vector<SymbolInfo*> findSymbolsByType(SymbolType type);
    
    // 命名空间管理
    bool addNamespace(const std::string& name, const std::string& parent = "");
    bool removeNamespace(const std::string& name);
    std::vector<std::string> getNamespaces() const;
    std::vector<std::string> getNamespaceHierarchy(const std::string& name) const;
    
    // 模板管理
    bool addTemplate(const SymbolInfo& template_info);
    bool removeTemplate(const std::string& name, const std::string& namespace_path = "");
    SymbolInfo* findTemplate(const std::string& name, const std::string& namespace_path = "");
    
    // 自定义对象管理
    bool addCustomObject(const SymbolInfo& custom_info);
    bool removeCustomObject(const std::string& name, const std::string& namespace_path = "");
    SymbolInfo* findCustomObject(const std::string& name, const std::string& namespace_path = "");
    
    // 导入管理
    bool addImportedModule(const std::string& module_name, const std::string& path);
    bool removeImportedModule(const std::string& module_name);
    std::vector<std::string> getImportedModules() const;
    
    // 依赖管理
    bool addDependency(const std::string& symbol_name, const std::string& dependency);
    std::vector<std::string> getDependencies(const std::string& symbol_name) const;
    std::vector<std::string> getDependents(const std::string& symbol_name) const;
    
    // 查询和统计
    size_t getSymbolCount() const;
    size_t getNamespaceCount() const;
    size_t getTemplateCount() const;
    size_t getCustomObjectCount() const;
    
    // 清理和重置
    void clear();
    void clearNamespace(const std::string& namespace_path);
    
    // 序列化和反序列化
    std::string serialize() const;
    bool deserialize(const std::string& data);
    
    // 调试和诊断
    std::string getDebugInfo() const;
    std::vector<std::string> getConflicts() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部辅助方法
    std::string normalizeNamespacePath(const std::string& path) const;
    bool isValidSymbolName(const std::string& name) const;
    bool isValidNamespacePath(const std::string& path) const;
    std::string resolveNamespacePath(const std::string& name, const std::string& current_namespace) const;
};

} // namespace chtl