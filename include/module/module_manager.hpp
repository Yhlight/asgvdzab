#ifndef CHTL_MODULE_MANAGER_HPP
#define CHTL_MODULE_MANAGER_HPP

#include "core/types.hpp"
#include "utils/simple_zip.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <functional>
#include <any>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace CHTL {
namespace Module {

// 模块类型
enum class ModuleType {
    CMOD,          // CHTL模块
    CJMOD          // CHTL JS扩展模块
};

// 模块信息结构
struct ModuleInfo {
    std::string name;
    std::string version;
    std::string description;
    std::string author;
    std::string license;
    std::vector<std::string> dependencies;
    std::string category;
    std::string minCHTLVersion;
    std::string maxCHTLVersion;
    ModuleType type;
    std::string sourcePath;
    bool isLoaded = false;
    
    // 验证版本兼容性
    bool isVersionCompatible(const std::string& currentVersion) const;
    
    // 从CHTL文件解析模块信息
    static std::unique_ptr<ModuleInfo> parseFromChtl(const std::string& chtlContent, ModuleType type);
};

// 模块导出表
struct ModuleExports {
    std::unordered_set<std::string> styles;
    std::unordered_set<std::string> elements;
    std::unordered_set<std::string> variables;
    std::unordered_set<std::string> functions; // 仅CJMOD使用
    
    // 合并导出表
    void merge(const ModuleExports& other);
    
    // 检查是否有指定导出
    bool hasStyle(const std::string& name) const;
    bool hasElement(const std::string& name) const;
    bool hasVariable(const std::string& name) const;
    bool hasFunction(const std::string& name) const;
};

// CMOD（CHTL模块）
class CmodModule {
private:
    std::unique_ptr<ModuleInfo> info_;
    std::unique_ptr<ModuleExports> exports_;
    std::unordered_map<std::string, std::string> sourceFiles_; // 文件名 -> 内容
    std::unordered_map<std::string, std::unique_ptr<CmodModule>> subModules_;
    std::string modulePath_;
    
public:
    CmodModule(std::unique_ptr<ModuleInfo> info, const std::string& modulePath);
    ~CmodModule() = default;
    
    // 禁用拷贝和移动
    CmodModule(const CmodModule&) = delete;
    CmodModule& operator=(const CmodModule&) = delete;
    CmodModule(CmodModule&&) = delete;
    CmodModule& operator=(CmodModule&&) = delete;
    
    // 基本信息
    const ModuleInfo& getInfo() const { return *info_; }
    const ModuleExports& getExports() const { return *exports_; }
    const std::string& getModulePath() const { return modulePath_; }
    
    // 文件管理
    void addSourceFile(const std::string& filename, const std::string& content);
    std::string getSourceFile(const std::string& filename) const;
    std::vector<std::string> getSourceFileNames() const;
    bool hasSourceFile(const std::string& filename) const;
    
    // 子模块管理
    void addSubModule(const std::string& name, std::unique_ptr<CmodModule> subModule);
    const CmodModule* getSubModule(const std::string& name) const;
    CmodModule* getSubModuleMutable(const std::string& name);
    std::vector<std::string> getSubModuleNames() const;
    bool hasSubModule(const std::string& name) const;
    
    // 打包到.cmod文件
    bool packToFile(const std::string& outputPath) const;
    
    // 从.cmod文件加载
    static std::unique_ptr<CmodModule> loadFromFile(const std::string& cmodPath);
    
    // 从目录构建
    static std::unique_ptr<CmodModule> buildFromDirectory(const std::string& dirPath);
    
    // 解析导出表
    void parseExports();
    
    // 验证模块结构
    bool validate() const;
    
    // 获取模块的完整导出（包括子模块）
    ModuleExports getFullExports() const;

private:
    // 递归加载目录内容
    bool loadDirectoryRecursive(const std::filesystem::path& basePath, const std::filesystem::path& currentPath);
    
    // 解析info文件
    bool parseInfoFile(const std::string& content);
    
    // 生成导出表内容
    std::string generateExportTable() const;
    
    // 生成info文件内容
    std::string generateInfoContent() const;
    
    // 从内容解析导出
    void parseExportsFromContent(const std::string& content);
};

// CJMOD函数接口类型
using CJModFunction = std::function<std::any(const std::vector<std::any>&)>;

// CJMOD（CHTL JS扩展模块）
class CjmodModule {
private:
    std::unique_ptr<ModuleInfo> info_;
    std::unique_ptr<ModuleExports> exports_;
    std::unordered_map<std::string, CJModFunction> functions_;
    std::unordered_map<std::string, std::unique_ptr<CjmodModule>> subModules_;
    std::string modulePath_;
    void* libraryHandle_ = nullptr; // 动态库句柄
    
public:
    CjmodModule(std::unique_ptr<ModuleInfo> info, const std::string& modulePath);
    ~CjmodModule();
    
    // 禁用拷贝和移动
    CjmodModule(const CjmodModule&) = delete;
    CjmodModule& operator=(const CjmodModule&) = delete;
    CjmodModule(CjmodModule&&) = delete;
    CjmodModule& operator=(CjmodModule&&) = delete;
    
    // 基本信息
    const ModuleInfo& getInfo() const { return *info_; }
    const ModuleExports& getExports() const { return *exports_; }
    const std::string& getModulePath() const { return modulePath_; }
    
    // 函数管理
    void registerFunction(const std::string& name, CJModFunction func);
    bool hasFunction(const std::string& name) const;
    std::any callFunction(const std::string& name, const std::vector<std::any>& args) const;
    std::vector<std::string> getFunctionNames() const;
    
    // 子模块管理
    void addSubModule(const std::string& name, std::unique_ptr<CjmodModule> subModule);
    const CjmodModule* getSubModule(const std::string& name) const;
    CjmodModule* getSubModuleMutable(const std::string& name);
    std::vector<std::string> getSubModuleNames() const;
    bool hasSubModule(const std::string& name) const;
    
    // 动态库管理
    bool loadDynamicLibrary(const std::string& libraryPath);
    void unloadDynamicLibrary();
    bool isDynamicLibraryLoaded() const { return libraryHandle_ != nullptr; }
    
    // 打包到.cjmod文件
    bool packToFile(const std::string& outputPath) const;
    
    // 从.cjmod文件加载
    static std::unique_ptr<CjmodModule> loadFromFile(const std::string& cjmodPath);
    
    // 从目录构建
    static std::unique_ptr<CjmodModule> buildFromDirectory(const std::string& dirPath);
    
    // 验证模块结构
    bool validate() const;
    
    // 获取模块的完整导出（包括子模块）
    ModuleExports getFullExports() const;

private:
    // 递归加载目录内容
    bool loadDirectoryRecursive(const std::filesystem::path& basePath, const std::filesystem::path& currentPath);
    
    // 解析info文件
    bool parseInfoFile(const std::string& content);
    
    // 编译C++源代码为动态库
    bool compileSources(const std::filesystem::path& sourceDir, const std::string& outputLib) const;
    
    // 加载模块初始化函数
    bool loadModuleInitializer();
};

// 模块注册表
class ModuleRegistry {
private:
    std::unordered_map<std::string, std::unique_ptr<CmodModule>> cmodModules_;
    std::unordered_map<std::string, std::unique_ptr<CjmodModule>> cjmodModules_;
    std::unordered_map<std::string, std::string> modulePathMap_; // 模块名 -> 文件路径
    std::unordered_map<std::string, std::vector<std::string>> dependencyGraph_; // 依赖关系图
    std::unordered_set<std::string> loadedModules_;
    mutable std::mutex registryMutex_;
    
public:
    ModuleRegistry() = default;
    ~ModuleRegistry() = default;
    
    // 禁用拷贝和移动
    ModuleRegistry(const ModuleRegistry&) = delete;
    ModuleRegistry& operator=(const ModuleRegistry&) = delete;
    ModuleRegistry(ModuleRegistry&&) = delete;
    ModuleRegistry& operator=(ModuleRegistry&&) = delete;
    
    // CMOD管理
    bool registerCmod(const std::string& name, std::unique_ptr<CmodModule> module);
    const CmodModule* getCmod(const std::string& name) const;
    CmodModule* getCmodMutable(const std::string& name);
    bool hasCmod(const std::string& name) const;
    bool unregisterCmod(const std::string& name);
    
    // CJMOD管理
    bool registerCjmod(const std::string& name, std::unique_ptr<CjmodModule> module);
    const CjmodModule* getCjmod(const std::string& name) const;
    CjmodModule* getCjmodMutable(const std::string& name);
    bool hasCjmod(const std::string& name) const;
    bool unregisterCjmod(const std::string& name);
    
    // 模块加载
    bool loadModule(const std::string& name, ModuleType type);
    bool loadModuleFromFile(const std::string& filePath);
    bool loadModulesFromDirectory(const std::string& dirPath);
    
    // 依赖管理
    bool resolveDependencies();
    std::vector<std::string> getLoadOrder() const;
    bool checkCircularDependencies() const;
    
    // 查询
    std::vector<std::string> getAllCmodNames() const;
    std::vector<std::string> getAllCjmodNames() const;
    std::vector<std::string> getAllModuleNames() const;
    size_t getCmodCount() const;
    size_t getCjmodCount() const;
    size_t getTotalModuleCount() const;
    
    // 搜索
    std::vector<std::string> searchModules(const std::string& keyword) const;
    std::vector<std::string> findModulesByCategory(const std::string& category) const;
    std::vector<std::string> findModulesByAuthor(const std::string& author) const;
    
    // 验证
    bool validateAllModules() const;
    std::vector<std::string> getValidationErrors() const;
    
    // 清理
    void clear();
    void unloadAllModules();
    
    // 调试信息
    std::string getDebugInfo() const;
    void printModuleTree() const;

private:
    // 依赖解析的拓扑排序
    std::vector<std::string> topologicalSort() const;
    
    // 检查单个模块的循环依赖
    bool hasCyclicDependency(const std::string& module, std::unordered_set<std::string>& visited, std::unordered_set<std::string>& recursionStack) const;
};

// 模块管理器
class ModuleManager {
private:
    std::unique_ptr<ModuleRegistry> registry_;
    std::vector<std::string> searchPaths_;
    std::string currentVersion_;
    bool autoResolveDependencies_;
    
public:
    ModuleManager(const std::string& version = "1.0.0", bool autoResolve = true);
    ~ModuleManager() = default;
    
    // 禁用拷贝和移动
    ModuleManager(const ModuleManager&) = delete;
    ModuleManager& operator=(const ModuleManager&) = delete;
    ModuleManager(ModuleManager&&) = delete;
    ModuleManager& operator=(ModuleManager&&) = delete;
    
    // 获取组件
    ModuleRegistry* getRegistry() const { return registry_.get(); }
    
    // 搜索路径管理
    void addSearchPath(const std::string& path);
    void removeSearchPath(const std::string& path);
    const std::vector<std::string>& getSearchPaths() const { return searchPaths_; }
    void clearSearchPaths();
    
    // 模块操作
    bool installModule(const std::string& modulePath);
    bool uninstallModule(const std::string& moduleName);
    bool loadModule(const std::string& moduleName, ModuleType type);
    bool reloadModule(const std::string& moduleName);
    
    // 批量操作
    bool loadAllModules();
    bool loadModulesFromPaths();
    size_t scanAndLoadModules();
    
    // 模块打包
    bool packModule(const std::string& sourceDir, const std::string& outputPath, ModuleType type);
    bool extractModule(const std::string& modulePath, const std::string& outputDir);
    
    // 依赖管理
    bool resolveDependencies();
    std::vector<std::string> getMissingDependencies() const;
    
    // 查询和搜索
    bool isModuleLoaded(const std::string& name) const;
    std::vector<std::string> findModules(const std::string& pattern) const;
    const ModuleInfo* getModuleInfo(const std::string& name) const;
    
    // 配置
    void setAutoResolveDependencies(bool enable) { autoResolveDependencies_ = enable; }
    bool isAutoResolveDependenciesEnabled() const { return autoResolveDependencies_; }
    void setCurrentVersion(const std::string& version) { currentVersion_ = version; }
    const std::string& getCurrentVersion() const { return currentVersion_; }
    
    // 统计信息
    struct Statistics {
        size_t totalModules = 0;
        size_t cmodModules = 0;
        size_t cjmodModules = 0;
        size_t loadedModules = 0;
        size_t failedModules = 0;
        std::vector<std::string> searchPaths;
    };
    
    Statistics getStatistics() const;
    
    // 调试和诊断
    std::string getDebugInfo() const;
    bool validateSystem() const;
    std::vector<std::string> getSystemErrors() const;

private:
    // 解析模块路径
    std::string resolveModulePath(const std::string& name, ModuleType type) const;
    
    // 扫描目录中的模块
    std::vector<std::string> scanDirectory(const std::string& dirPath) const;
    
    // 验证模块文件
    bool isValidModuleFile(const std::string& filePath) const;
    
    // 获取模块类型
    ModuleType getModuleTypeFromPath(const std::string& path) const;
};

// 工厂函数
std::unique_ptr<ModuleManager> createModuleManager(const std::string& version = "1.0.0", bool autoResolve = true);

// 便捷函数
bool packCmodFromDirectory(const std::string& sourceDir, const std::string& outputPath);
bool packCjmodFromDirectory(const std::string& sourceDir, const std::string& outputPath);
bool extractCmodToDirectory(const std::string& cmodPath, const std::string& outputDir);
bool extractCjmodToDirectory(const std::string& cjmodPath, const std::string& outputDir);

// CJMOD初始化函数类型（由CJMOD模块导出）
extern "C" {
    typedef void (*CJModInitFunc)(CjmodModule* module);
    typedef void (*CJModCleanupFunc)();
}

// 辅助宏（供CJMOD开发者使用）
#define CJMOD_INIT(module_ptr) \
    extern "C" void cjmod_init(CHTL::Module::CjmodModule* module_ptr)

#define CJMOD_CLEANUP() \
    extern "C" void cjmod_cleanup()

#define CJMOD_REGISTER_FUNCTION(module, name, func) \
    module->registerFunction(name, [](const std::vector<std::any>& args) -> std::any { \
        return func(args); \
    })

} // namespace Module
} // namespace CHTL

#endif // CHTL_MODULE_MANAGER_HPP