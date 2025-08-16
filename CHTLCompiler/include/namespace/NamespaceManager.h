#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace chtl {

// 命名空间冲突类型
enum class NamespaceConflictType {
    NONE,                   // 无冲突
    NAME_COLLISION,         // 名称冲突
    TYPE_COLLISION,         // 类型冲突
    IMPORT_COLLISION,       // 导入冲突
    EXPORT_COLLISION,       // 导出冲突
    CIRCULAR_DEPENDENCY,    // 循环依赖
    INVALID_REFERENCE       // 无效引用
};

// 命名空间冲突信息
struct NamespaceConflict {
    NamespaceConflictType type;         // 冲突类型
    std::string namespace_name;         // 命名空间名称
    std::string conflicting_name;       // 冲突的名称
    std::string source_file;            // 源文件
    size_t line;                        // 行号
    size_t column;                      // 列号
    std::string description;            // 冲突描述
    bool is_resolved;                   // 是否已解决
    
    NamespaceConflict(NamespaceConflictType t, const std::string& ns, const std::string& cn,
                     const std::string& src = "", size_t l = 0, size_t c = 0)
        : type(t), namespace_name(ns), conflicting_name(cn), source_file(src), 
          line(l), column(c), is_resolved(false) {}
};

// 命名空间项类型
enum class NamespaceItemType {
    TEMPLATE,               // 模板
    CUSTOM,                 // 自定义
    ORIGIN,                 // 原始嵌入
    IMPORT,                 // 导入
    EXPORT,                 // 导出
    CONSTRAINT,             // 约束
    NAMESPACE,              // 子命名空间
    UNKNOWN                 // 未知类型
};

// 命名空间项信息
struct NamespaceItem {
    NamespaceItemType type;             // 项类型
    std::string name;                   // 项名称
    std::string full_name;              // 完整名称（包含命名空间路径）
    std::string source_file;            // 源文件
    size_t line;                        // 行号
    size_t column;                      // 列号
    std::unordered_map<std::string, std::string> metadata; // 元数据
    bool is_public;                     // 是否公开
    bool is_exported;                   // 是否导出
    std::vector<std::string> dependencies; // 依赖列表
    
    NamespaceItem(NamespaceItemType t, const std::string& n, const std::string& fn,
                  const std::string& src = "", size_t l = 0, size_t c = 0)
        : type(t), name(n), full_name(fn), source_file(src), line(l), column(c),
          is_public(true), is_exported(false) {}
};

// 命名空间信息
struct NamespaceInfo {
    std::string name;                   // 命名空间名称
    std::string full_path;              // 完整路径
    std::string parent_namespace;       // 父命名空间
    std::vector<std::string> child_namespaces; // 子命名空间
    std::vector<NamespaceItem> items;   // 命名空间项
    std::vector<std::string> imports;   // 导入列表
    std::vector<std::string> exports;   // 导出列表
    std::vector<std::string> constraints; // 约束列表
    std::string source_file;            // 源文件
    size_t start_line;                  // 开始行号
    size_t end_line;                    // 结束行号
    bool is_merged;                     // 是否已合并
    bool is_valid;                      // 是否有效
    
    NamespaceInfo(const std::string& n, const std::string& fp = "", const std::string& src = "")
        : name(n), full_path(fp), source_file(src), start_line(0), end_line(0),
          is_merged(false), is_valid(true) {}
};

// 命名空间管理器配置
struct NamespaceManagerConfig {
    bool enable_auto_merge;             // 是否启用自动合并
    bool enable_conflict_detection;     // 是否启用冲突检测
    bool enable_circular_detection;     // 是否启用循环依赖检测
    bool enable_validation;             // 是否启用验证
    size_t max_namespace_depth;         // 最大命名空间深度
    bool enable_cache;                  // 是否启用缓存
    std::chrono::seconds cache_timeout; // 缓存超时时间
    
    NamespaceManagerConfig() : 
        enable_auto_merge(true), enable_conflict_detection(true), 
        enable_circular_detection(true), enable_validation(true), max_namespace_depth(20),
        enable_cache(true), cache_timeout(300) {}
};

// 命名空间管理器类
class NamespaceManager {
public:
    NamespaceManager(const NamespaceManagerConfig& config = NamespaceManagerConfig());
    ~NamespaceManager();
    
    // 命名空间管理
    bool addNamespace(const NamespaceInfo& namespace_info);
    bool mergeNamespaces(const std::string& namespace_name);
    bool mergeAllNamespaces();
    void clearNamespaces();
    
    // 命名空间查询
    std::vector<NamespaceInfo> getNamespaces() const;
    NamespaceInfo* getNamespace(const std::string& name);
    NamespaceInfo* getNamespaceByPath(const std::string& path);
    bool hasNamespace(const std::string& name) const;
    std::vector<std::string> getNamespacePaths() const;
    
    // 命名空间项管理
    bool addNamespaceItem(const std::string& namespace_name, const NamespaceItem& item);
    bool removeNamespaceItem(const std::string& namespace_name, const std::string& item_name);
    std::vector<NamespaceItem> getNamespaceItems(const std::string& namespace_name) const;
    NamespaceItem* getNamespaceItem(const std::string& namespace_name, const std::string& item_name);
    
    // 冲突检测
    bool detectConflicts();
    std::vector<NamespaceConflict> getConflicts() const;
    bool hasConflicts() const;
    bool resolveConflict(const std::string& namespace_name, const std::string& conflicting_name);
    bool resolveAllConflicts();
    
    // 循环依赖检测
    bool detectCircularDependencies();
    std::vector<std::vector<std::string>> getCircularDependencies() const;
    bool hasCircularDependencies() const;
    
    // 命名空间验证
    bool validateNamespace(const std::string& namespace_name);
    bool validateAllNamespaces();
    std::vector<std::string> getValidationErrors() const;
    
    // 导入导出管理
    bool addImport(const std::string& namespace_name, const std::string& import_path);
    bool addExport(const std::string& namespace_name, const std::string& export_name);
    std::vector<std::string> getImports(const std::string& namespace_name) const;
    std::vector<std::string> getExports(const std::string& namespace_name) const;
    
    // 约束管理
    bool addConstraint(const std::string& namespace_name, const std::string& constraint);
    bool removeConstraint(const std::string& namespace_name, const std::string& constraint);
    std::vector<std::string> getConstraints(const std::string& namespace_name) const;
    bool validateConstraints(const std::string& namespace_name);
    
    // 路径解析
    std::string resolveNamespacePath(const std::string& path);
    std::string getFullNamespacePath(const std::string& namespace_name);
    std::vector<std::string> getChildNamespaces(const std::string& namespace_name) const;
    
    // 配置管理
    void setConfig(const NamespaceManagerConfig& config);
    NamespaceManagerConfig getConfig() const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error_message);
    
    // 调试和诊断
    std::string getDebugInfo() const;
    std::string getNamespaceGraph() const;
    std::string getConflictReport() const;
    void enableDebugMode(bool enable);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
    bool validateNamespaceInfo(const NamespaceInfo& namespace_info);
    bool checkNamespaceConflict(const std::string& namespace_name, const std::string& item_name);
    bool checkCircularDependency(const std::string& namespace_name, const std::string& current_path);
    std::string normalizeNamespacePath(const std::string& path);
    std::vector<std::string> splitNamespacePath(const std::string& path);
    bool isValidNamespaceName(const std::string& name);
    void updateNamespaceCache();
    void cleanupCache();
    bool mergeNamespaceItems(NamespaceInfo& target, const NamespaceInfo& source);
    void resolveNamespaceReferences();
};

} // namespace chtl