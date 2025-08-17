#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <ctime>

namespace chtl {

// 模块类型枚举
enum class ModuleType {
    CMOD,      // CHTL模块
    CJMOD,     // CHTL JS模块
    HTML,      // HTML模块
    CSS,       // CSS模块
    JAVASCRIPT // JavaScript模块
};

// 模块状态枚举
enum class ModuleStatus {
    UNLOADED,  // 未加载
    LOADING,   // 加载中
    LOADED,    // 已加载
    ERROR,     // 错误状态
    UNLOADING  // 卸载中
};

// 模块内容结构
struct ModuleContent {
    std::string source_code;      // 源代码
    std::string compiled_code;    // 编译后代码
    std::string binary_data;      // 二进制数据
    std::unordered_map<std::string, std::string> metadata; // 元数据
};

// 模块系统信息结构
struct ModuleSystemInfo {
    std::string name;             // 模块名称
    std::string file_path;        // 文件路径
    ModuleType type;              // 模块类型
    ModuleStatus status;          // 模块状态
    ModuleContent content;        // 模块内容
    std::vector<std::string> dependencies; // 依赖列表
    std::string version;          // 版本信息
    std::string description;      // 描述信息
    size_t file_size;             // 文件大小
    std::unordered_map<std::string, std::string> properties; // 属性
};

// 模块系统配置
struct ModuleSystemConfig {
    bool enable_debug_mode = false;           // 启用调试模式
    bool enable_auto_reload = true;           // 启用自动重载
    bool enable_dependency_checking = true;   // 启用依赖检查
    bool enable_circular_dependency_detection = true; // 启用循环依赖检测
    std::string module_search_path = "./modules"; // 模块搜索路径
    size_t max_module_size = 1024 * 1024;    // 最大模块大小 (1MB)
    size_t max_dependencies = 100;            // 最大依赖数量
    std::vector<std::string> allowed_extensions = {".cmod", ".cjmod", ".chtl", ".html", ".css", ".js"}; // 允许的文件扩展名
};

// 模块系统类
class ModuleSystem {
public:
    explicit ModuleSystem(const ModuleSystemConfig& config = ModuleSystemConfig{});
    ~ModuleSystem();
    
    // 禁用拷贝构造和赋值
    ModuleSystem(const ModuleSystem&) = delete;
    ModuleSystem& operator=(const ModuleSystem&) = delete;
    
    // 模块加载和卸载
    bool loadModule(const std::string& module_name);
    bool unloadModule(const std::string& module_name);
    bool reloadModule(const std::string& module_name);
    bool isModuleLoaded(const std::string& module_name) const;
    
    // 模块管理
    void addModule(const ModuleSystemInfo& module_info);
    void removeModule(const std::string& module_name);
    ModuleSystemInfo* getModule(const std::string& name);
    std::vector<ModuleSystemInfo> getAllModules() const;
    std::vector<ModuleSystemInfo> getModulesByType(ModuleType type) const;
    bool hasModule(const std::string& module_name) const;
    
    // 模块验证
    bool validateModule(const ModuleSystemInfo& module_info);
    std::vector<std::string> validateModuleList(const std::vector<ModuleSystemInfo>& modules);
    bool validateModuleFile(const std::string& file_path) const;
    bool validateModuleContent(const ModuleContent& content) const;
    
    // 模块处理
    bool processModule(const std::string& module_name);
    bool processModuleByType(const std::string& module_name, ModuleType type);
    std::string compileModule(const std::string& module_name);
    
    // 依赖管理
    bool checkCircularDependencies(const std::string& module_name);
    std::vector<std::string> getDependencyChain(const std::string& module_name);
    bool resolveDependencies(const std::string& module_name);
    bool loadDependencies(const std::string& module_name);
    
    // 模块打包和解包
    bool packModule(const std::string& module_name, const std::string& output_path);
    bool unpackModule(const std::string& archive_path, const std::string& output_path);
    bool createModuleArchive(const std::vector<std::string>& module_names, const std::string& output_path);
    
    // 配置管理
    void setConfig(const ModuleSystemConfig& config);
    ModuleSystemConfig getConfig() const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error);
    
    // 调试信息
    std::string getDebugInfo() const;
    void enableDebugMode(bool enable);
    
    // 状态机设置
    void setStateMachine(class CHTLStateMachine* state_machine);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 私有辅助方法
    bool loadModuleContent(ModuleSystemInfo& module_info);
    bool isValidModuleName(const std::string& name) const;
    bool isValidModuleFile(const std::string& file_path) const;
    bool isValidModuleSize(size_t size) const;
    bool hasValidModuleStructure(const ModuleSystemInfo& module_info);
};

} // namespace chtl