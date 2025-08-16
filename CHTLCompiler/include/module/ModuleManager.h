#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <functional>
#include <chrono>

namespace chtl {

// 模块类型枚举
enum class ModuleType {
    CMOD,           // Cmod模块
    CJMOD,          // CJmod模块
    UNKNOWN         // 未知类型
};

// 模块状态枚举
enum class ModuleStatus {
    UNLOADED,       // 未加载
    LOADING,        // 加载中
    LOADED,         // 已加载
    ERROR,          // 错误状态
    CORRUPTED       // 损坏状态
};

// 模块信息结构
struct ModuleInfo {
    std::string name;                   // 模块名称
    std::string path;                   // 模块路径
    ModuleType type;                    // 模块类型
    ModuleStatus status;                // 模块状态
    std::string version;                // 模块版本
    std::string description;            // 模块描述
    std::string author;                 // 作者
    std::string license;                // 许可证
    std::vector<std::string> dependencies; // 依赖列表
    std::vector<std::string> exports;   // 导出列表
    std::vector<std::string> imports;   // 导入列表
    std::chrono::system_clock::time_point created_time; // 创建时间
    std::chrono::system_clock::time_point modified_time; // 修改时间
    std::chrono::system_clock::time_point loaded_time;  // 加载时间
    size_t file_size;                   // 文件大小
    std::string checksum;               // 校验和
    bool is_official;                   // 是否官方模块
    bool is_loaded;                     // 是否已加载
    bool is_valid;                      // 是否有效
    
    ModuleInfo(const std::string& n, const std::string& p, ModuleType t)
        : name(n), path(p), type(t), status(ModuleStatus::UNLOADED), file_size(0), 
          is_official(false), is_loaded(false), is_valid(true) {
        created_time = std::chrono::system_clock::now();
        modified_time = created_time;
    }
};

// 模块内容结构
struct ModuleContent {
    std::string module_name;            // 模块名称
    std::string module_type;            // 模块类型字符串
    std::string version;                // 版本
    std::string description;            // 描述
    std::string author;                 // 作者
    std::string license;                // 许可证
    std::vector<std::string> dependencies; // 依赖
    std::vector<std::string> exports;   // 导出
    std::vector<std::string> imports;   // 导入
    std::unordered_map<std::string, std::string> files; // 文件内容映射
    std::unordered_map<std::string, std::string> metadata; // 元数据
    std::chrono::system_clock::time_point created_time; // 创建时间
    std::chrono::system_clock::time_point modified_time; // 修改时间
};

// 模块管理器配置
struct ModuleManagerConfig {
    std::string official_module_path;   // 官方模块路径
    std::string local_module_path;      // 本地模块路径
    std::string temp_directory;         // 临时目录
    bool enable_compression;            // 是否启用压缩
    bool enable_encryption;             // 是否启用加密
    bool enable_validation;             // 是否启用验证
    bool enable_cache;                  // 是否启用缓存
    size_t max_module_size;             // 最大模块大小
    size_t max_cache_size;              // 最大缓存大小
    std::chrono::seconds cache_timeout; // 缓存超时时间
    std::string compression_level;      // 压缩级别
    std::string encryption_key;         // 加密密钥
    
    ModuleManagerConfig() : 
        official_module_path("module"), local_module_path("module"), temp_directory("temp"),
        enable_compression(true), enable_encryption(false), enable_validation(true),
        enable_cache(true), max_module_size(100 * 1024 * 1024), max_cache_size(1000),
        cache_timeout(3600), compression_level("6"), encryption_key("") {}
};

// 模块管理器类
class ModuleManager {
public:
    ModuleManager(const ModuleManagerConfig& config = ModuleManagerConfig());
    ~ModuleManager();
    
    // 模块管理
    bool loadModule(const std::string& module_path);
    bool unloadModule(const std::string& module_name);
    bool reloadModule(const std::string& module_name);
    void unloadAllModules();
    
    // 模块查询
    std::vector<ModuleInfo> getModules() const;
    std::vector<ModuleInfo> getModulesByType(ModuleType type) const;
    std::vector<ModuleInfo> getOfficialModules() const;
    std::vector<ModuleInfo> getLocalModules() const;
    ModuleInfo* getModule(const std::string& name);
    bool hasModule(const std::string& name) const;
    
    // 模块打包
    bool packModule(const std::string& source_directory, const std::string& output_path, ModuleType type);
    bool packCmodModule(const std::string& source_directory, const std::string& output_path);
    bool packCJmodModule(const std::string& source_directory, const std::string& output_path);
    
    // 模块解包
    bool unpackModule(const std::string& module_path, const std::string& output_directory);
    bool unpackCmodModule(const std::string& module_path, const std::string& output_directory);
    bool unpackCJmodModule(const std::string& module_path, const std::string& output_directory);
    
    // 模块验证
    bool validateModule(const std::string& module_path);
    bool validateModuleContent(const ModuleContent& content);
    std::vector<std::string> getValidationErrors() const;
    
    // 模块内容访问
    ModuleContent* getModuleContent(const std::string& module_name);
    std::string getModuleFile(const std::string& module_name, const std::string& file_path);
    std::vector<std::string> getModuleFiles(const std::string& module_name);
    
    // 官方模块管理
    bool registerOfficialModule(const std::string& module_path);
    bool unregisterOfficialModule(const std::string& module_name);
    std::vector<std::string> getOfficialModuleNames() const;
    bool isOfficialModule(const std::string& module_name) const;
    
    // 依赖管理
    bool resolveDependencies(const std::string& module_name);
    std::vector<std::string> getModuleDependencies(const std::string& module_name);
    bool hasCircularDependencies() const;
    std::vector<std::vector<std::string>> getCircularDependencies() const;
    
    // 配置管理
    void setConfig(const ModuleManagerConfig& config);
    ModuleManagerConfig getConfig() const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error_message);
    
    // 调试和诊断
    std::string getDebugInfo() const;
    std::string getModuleGraph() const;
    std::string getModuleReport() const;
    void enableDebugMode(bool enable);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
    bool validateModulePath(const std::string& module_path);
    bool validateModuleFormat(const std::string& module_path, ModuleType type);
    std::string calculateModuleChecksum(const std::string& module_path);
    bool isModuleCorrupted(const std::string& module_path);
    void updateModuleCache();
    void cleanupCache();
    bool checkModuleCompatibility(const ModuleInfo& module_info);
    std::string getModuleTypeString(ModuleType type);
    ModuleType getModuleTypeFromString(const std::string& type_string);
};

} // namespace chtl