#pragma once

#include "cjmod/cjmod_manager.hpp"
#include "import/enhanced_import_manager.hpp"
#include "namespace/enhanced_namespace_manager.hpp"
#include <string>
#include <vector>
#include <memory>

namespace chtl {

/**
 * CJmod编译器命令类型
 */
enum class CjmodCompilerCommand {
    PACK,           // 打包模块
    UNPACK,         // 解包模块
    VALIDATE,       // 验证模块结构
    INFO,           // 显示模块信息
    CREATE,         // 创建模块模板
    LIST,           // 列出模块
    COMPILE,        // 编译模块
    BUILD           // 构建动态库
};

/**
 * CJmod编译器选项
 */
struct CjmodCompilerOptions {
    CjmodCompilerCommand command;       // 命令类型
    std::string sourceDir;              // 源目录
    std::string outputPath;             // 输出路径
    std::string moduleName;             // 模块名称
    bool verbose;                       // 详细输出
    bool force;                         // 强制覆盖
    bool useOfficialPrefix;             // 使用官方前缀 chtl::
    
    // 创建模块时的选项
    std::string moduleAuthor;           // 模块作者
    std::string moduleDescription;      // 模块描述
    std::string moduleVersion;          // 模块版本
    std::string moduleLicense;          // 模块许可证
    std::string moduleCategory;         // 模块分类
    std::string cppStandard;            // C++标准
    std::string compilerFlags;          // 编译器标志
    std::vector<std::string> includePaths;  // 包含路径
    std::vector<std::string> libraries;     // 链接库
    std::vector<std::string> jsExtensions;  // JS扩展
    
    CjmodCompilerOptions() 
        : command(CjmodCompilerCommand::PACK), verbose(false), force(false), useOfficialPrefix(false),
          moduleVersion("1.0.0"), moduleLicense("MIT"), moduleCategory("js-extension"),
          cppStandard("C++17") {}
};

/**
 * CJmod编译器结果
 */
struct CjmodCompilerResult {
    bool success;                       // 是否成功
    std::string message;                // 结果消息
    std::vector<std::string> outputs;   // 输出信息
    CjmodInfo moduleInfo;               // 模块信息（如果适用）
    
    CjmodCompilerResult() : success(false) {}
};

/**
 * 模块前缀增强管理器
 * 为CMOD和CJmod提供统一的官方前缀支持
 */
class UnifiedModulePrefixManager {
public:
    UnifiedModulePrefixManager();
    ~UnifiedModulePrefixManager();
    
    /**
     * 解析模块导入路径（支持chtl::前缀）
     * @param importPath 导入路径
     * @param moduleType 模块类型（cmod或cjmod）
     * @return 解析后的实际路径
     */
    std::string resolveModuleImport(const std::string& importPath, const std::string& moduleType);
    
    /**
     * 检查是否为官方模块
     * @param moduleName 模块名称
     * @return 是否为官方模块
     */
    bool isOfficialModule(const std::string& moduleName) const;
    
    /**
     * 提取官方模块名称
     * @param moduleName 完整模块名称
     * @return 去除前缀的模块名称
     */
    std::string extractOfficialModuleName(const std::string& moduleName) const;
    
    /**
     * 列出所有官方模块
     * @param moduleType 模块类型（空表示全部）
     * @return 官方模块列表
     */
    std::vector<std::string> listOfficialModules(const std::string& moduleType = "") const;
    
    /**
     * 设置官方模块根目录
     * @param path 官方模块根目录路径
     */
    void setOfficialModuleRoot(const std::string& path);
    
    /**
     * 获取官方模块根目录
     * @return 官方模块根目录路径
     */
    std::string getOfficialModuleRoot() const;

private:
    std::unique_ptr<OfficialModuleResolver> resolver_;
};

/**
 * CJmod编译器扩展
 * 集成CJmod功能到CHTL编译器中
 */
class CjmodCompilerExtension {
public:
    CjmodCompilerExtension();
    ~CjmodCompilerExtension();
    
    // ===== 命令处理 =====
    
    /**
     * 执行CJmod命令
     * @param options 命令选项
     * @return 执行结果
     */
    CjmodCompilerResult executeCommand(const CjmodCompilerOptions& options);
    
    /**
     * 解析命令行参数
     * @param args 命令行参数
     * @return 解析的选项
     */
    CjmodCompilerOptions parseCommandLine(const std::vector<std::string>& args);
    
    // ===== 具体命令实现 =====
    
    /**
     * 打包模块
     * @param options 命令选项
     * @return 执行结果
     */
    CjmodCompilerResult packModule(const CjmodCompilerOptions& options);
    
    /**
     * 解包模块
     * @param options 命令选项
     * @return 执行结果
     */
    CjmodCompilerResult unpackModule(const CjmodCompilerOptions& options);
    
    /**
     * 验证模块结构
     * @param options 命令选项
     * @return 执行结果
     */
    CjmodCompilerResult validateModule(const CjmodCompilerOptions& options);
    
    /**
     * 显示模块信息
     * @param options 命令选项
     * @return 执行结果
     */
    CjmodCompilerResult showModuleInfo(const CjmodCompilerOptions& options);
    
    /**
     * 创建模块模板
     * @param options 命令选项
     * @return 执行结果
     */
    CjmodCompilerResult createModuleTemplate(const CjmodCompilerOptions& options);
    
    /**
     * 列出模块
     * @param options 命令选项
     * @return 执行结果
     */
    CjmodCompilerResult listModules(const CjmodCompilerOptions& options);
    
    /**
     * 编译模块
     * @param options 命令选项
     * @return 执行结果
     */
    CjmodCompilerResult compileModule(const CjmodCompilerOptions& options);
    
    /**
     * 构建动态库
     * @param options 命令选项
     * @return 执行结果
     */
    CjmodCompilerResult buildDynamicLibrary(const CjmodCompilerOptions& options);
    
    // ===== 编译器集成 =====
    
    /**
     * 注册到Import管理器
     * @param importManager Import管理器
     */
    void registerWithImportManager(EnhancedImportManager* importManager);
    
    /**
     * 注册到命名空间管理器
     * @param namespaceManager 命名空间管理器
     */
    void registerWithNamespaceManager(EnhancedNamespaceManager* namespaceManager);
    
    /**
     * 处理CJmod导入请求
     * @param moduleName 模块名称
     * @param searchPaths 搜索路径
     * @return 模块路径（如果找到）
     */
    std::string resolveCjmodImport(const std::string& moduleName, const std::vector<std::string>& searchPaths);
    
    /**
     * 自动解包并编译CJmod模块
     * @param cjmodPath CJmod文件路径
     * @param cacheDir 缓存目录
     * @return 编译后的动态库路径
     */
    std::string autoUnpackAndCompile(const std::string& cjmodPath, const std::string& cacheDir);
    
    /**
     * 获取统一模块前缀管理器
     * @return 模块前缀管理器
     */
    UnifiedModulePrefixManager* getModulePrefixManager();
    
    // ===== 实用工具 =====
    
    /**
     * 获取帮助信息
     * @return 帮助文本
     */
    std::string getHelpText() const;
    
    /**
     * 获取版本信息
     * @return 版本文本
     */
    std::string getVersionText() const;
    
    /**
     * 设置模块搜索路径
     * @param paths 搜索路径列表
     */
    void setModuleSearchPaths(const std::vector<std::string>& paths);
    
    /**
     * 获取模块搜索路径
     * @return 搜索路径列表
     */
    const std::vector<std::string>& getModuleSearchPaths() const;
    
    /**
     * 设置缓存目录
     * @param cacheDir 缓存目录路径
     */
    void setCacheDirectory(const std::string& cacheDir);
    
    /**
     * 清理缓存
     * @return 是否成功
     */
    bool cleanCache();
    
    /**
     * 打印结果
     * @param result 结果
     */
    void printResult(const CjmodCompilerResult& result) const;

private:
    std::unique_ptr<CjmodManager> cjmodManager_;            // CJmod管理器
    std::unique_ptr<UnifiedModulePrefixManager> prefixManager_; // 统一前缀管理器
    EnhancedImportManager* importManager_;                  // Import管理器（非拥有）
    EnhancedNamespaceManager* namespaceManager_;            // 命名空间管理器（非拥有）
    
    std::vector<std::string> moduleSearchPaths_;           // 模块搜索路径
    std::string cacheDirectory_;                            // 缓存目录
    
    // 内部工具方法
    bool createDirectoryStructure(const std::string& moduleDir, const CjmodCompilerOptions& options);
    std::string createMainCppFile(const CjmodCompilerOptions& options);
    std::string createHeaderFile(const CjmodCompilerOptions& options);
    std::string createInfoChtlFile(const CjmodCompilerOptions& options);
    std::vector<std::string> findCjmodFiles(const std::string& directory);
    std::string generateCacheKey(const std::string& cjmodPath);
    bool isCacheValid(const std::string& cacheDir, const std::string& cjmodPath);
    void printUsage() const;
};

/**
 * CJmod命令行接口
 * 提供独立的CJmod命令行工具
 */
class CjmodCommandLineInterface {
public:
    CjmodCommandLineInterface();
    ~CjmodCommandLineInterface();
    
    /**
     * 运行命令行接口
     * @param argc 参数数量
     * @param argv 参数数组
     * @return 退出码
     */
    int run(int argc, char* argv[]);
    
    /**
     * 设置编译器扩展
     * @param extension 编译器扩展
     */
    void setCompilerExtension(std::shared_ptr<CjmodCompilerExtension> extension);

private:
    std::shared_ptr<CjmodCompilerExtension> extension_;
    
    void printBanner() const;
    void printUsage() const;
    std::vector<std::string> parseArguments(int argc, char* argv[]);
};

} // namespace chtl