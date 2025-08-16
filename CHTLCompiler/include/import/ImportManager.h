#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <functional>

namespace chtl {

// 导入类型枚举
enum class ImportType {
    HTML,           // @Html
    STYLE,          // @Style
    JAVASCRIPT,     // @JavaScript
    CHTL,           // @Chtl
    CJMOD,          // @CJmod
    UNKNOWN         // 未知类型
};

// 导入语法类型
enum class ImportSyntaxType {
    SIMPLE,         // 简单导入 (from path)
    ALIAS,          // 别名导入 (from path as name)
    WILDCARD,       // 通配符导入 (from path.*)
    SUBMODULE       // 子模块导入 (from module.submodule)
};

// 导入信息结构
struct ImportInfo {
    ImportType type;                    // 导入类型
    ImportSyntaxType syntax_type;       // 语法类型
    std::string path;                   // 导入路径
    std::string alias;                  // 别名（如果有）
    std::string original_path;          // 原始路径（未解析的）
    std::string resolved_path;          // 解析后的路径
    std::string source_file;            // 源文件路径
    size_t line;                        // 行号
    size_t column;                      // 列号
    bool is_resolved;                   // 是否已解析
    bool is_circular;                   // 是否循环依赖
    std::vector<std::string> dependencies; // 依赖列表
    
    ImportInfo(ImportType t, ImportSyntaxType st, const std::string& p, 
               const std::string& src = "", size_t l = 0, size_t c = 0)
        : type(t), syntax_type(st), path(p), original_path(p), source_file(src), 
          line(l), column(c), is_resolved(false), is_circular(false) {}
};

// 模块信息结构
struct ModuleInfo {
    std::string name;                   // 模块名称
    std::string path;                   // 模块路径
    std::string type;                   // 模块类型 (cmod/cjmod/chtl)
    std::vector<std::string> exports;   // 导出列表
    std::vector<std::string> imports;   // 导入列表
    std::chrono::system_clock::time_point last_modified; // 最后修改时间
    bool is_valid;                      // 是否有效
    
    ModuleInfo(const std::string& n, const std::string& p, const std::string& t)
        : name(n), path(p), type(t), is_valid(true) {
        last_modified = std::chrono::system_clock::now();
    }
};

// 导入管理器配置
struct ImportManagerConfig {
    std::string official_module_path;   // 官方模块路径
    std::string local_module_path;      // 本地模块路径
    std::string current_directory;      // 当前目录
    bool enable_circular_detection;     // 是否启用循环依赖检测
    bool enable_duplicate_detection;    // 是否启用重复导入检测
    bool enable_wildcard_import;        // 是否启用通配符导入
    bool enable_submodule_import;       // 是否启用子模块导入
    size_t max_import_depth;            // 最大导入深度
    bool enable_cache;                  // 是否启用缓存
    std::chrono::seconds cache_timeout; // 缓存超时时间
    
    ImportManagerConfig() : 
        official_module_path("module"), local_module_path("module"), current_directory("."),
        enable_circular_detection(true), enable_duplicate_detection(true), 
        enable_wildcard_import(true), enable_submodule_import(true), max_import_depth(10),
        enable_cache(true), cache_timeout(300) {}
};

// 导入管理器类
class ImportManager {
public:
    ImportManager(const ImportManagerConfig& config = ImportManagerConfig());
    ~ImportManager();
    
    // 导入管理
    bool addImport(const ImportInfo& import_info);
    bool resolveImport(ImportInfo& import_info);
    bool resolveAllImports();
    void clearImports();
    
    // 导入查询
    std::vector<ImportInfo> getImports() const;
    std::vector<ImportInfo> getImportsByType(ImportType type) const;
    std::vector<ImportInfo> getImportsByPath(const std::string& path) const;
    bool hasImport(const std::string& path, ImportType type) const;
    
    // 循环依赖检测
    bool detectCircularDependencies();
    std::vector<std::vector<std::string>> getCircularDependencies() const;
    bool hasCircularDependencies() const;
    
    // 重复导入检测
    bool detectDuplicateImports();
    std::vector<std::string> getDuplicateImports() const;
    bool hasDuplicateImports() const;
    
    // 路径解析
    std::string resolvePath(const std::string& path, ImportType type);
    std::vector<std::string> resolveWildcardPath(const std::string& path, ImportType type);
    std::vector<std::string> resolveSubmodulePath(const std::string& path, ImportType type);
    
    // 文件搜索
    std::string findFile(const std::string& filename, ImportType type);
    std::vector<std::string> findFilesInDirectory(const std::string& directory, ImportType type);
    std::vector<std::string> findFilesByPattern(const std::string& pattern, ImportType type);
    
    // 模块管理
    bool loadModule(const std::string& module_path);
    std::vector<ModuleInfo> getModules() const;
    ModuleInfo* getModule(const std::string& name);
    
    // 配置管理
    void setConfig(const ImportManagerConfig& config);
    ImportManagerConfig getConfig() const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error_message);
    
    // 调试和诊断
    std::string getDebugInfo() const;
    std::string getImportGraph() const;
    void enableDebugMode(bool enable);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
    bool validateImport(const ImportInfo& import_info);
    bool checkCircularDependency(const std::string& path, const std::string& current_path);
    bool checkDuplicateImport(const ImportInfo& import_info);
    std::string normalizePath(const std::string& path);
    std::string getFileExtension(const std::string& filename);
    bool isValidFileExtension(const std::string& extension, ImportType type);
    std::vector<std::string> getSearchPaths(ImportType type);
    bool isOfficialModule(const std::string& path);
    bool isLocalModule(const std::string& path);
    void updateImportCache();
    void cleanupCache();
};

} // namespace chtl