#pragma once

#include "cmod/cmod_manager.hpp"
#include "import/enhanced_import_manager.hpp"
#include "namespace/enhanced_namespace_manager.hpp"
#include <string>
#include <vector>
#include <memory>

namespace chtl {

/**
 * Cmod编译器命令类型
 */
enum class CmodCompilerCommand {
    PACK,           // 打包模块
    UNPACK,         // 解包模块
    VALIDATE,       // 验证模块结构
    INFO,           // 显示模块信息
    CREATE,         // 创建模块模板
    LIST            // 列出模块
};

/**
 * Cmod编译器选项
 */
struct CmodCompilerOptions {
    CmodCompilerCommand command;        // 命令类型
    std::string sourceDir;              // 源目录
    std::string outputPath;             // 输出路径
    std::string moduleName;             // 模块名称
    bool verbose;                       // 详细输出
    bool force;                         // 强制覆盖
    
    // 创建模块时的选项
    std::string moduleAuthor;           // 模块作者
    std::string moduleDescription;      // 模块描述
    std::string moduleVersion;          // 模块版本
    std::string moduleLicense;          // 模块许可证
    std::string moduleCategory;         // 模块分类
    
    CmodCompilerOptions() 
        : command(CmodCompilerCommand::PACK), verbose(false), force(false),
          moduleVersion("1.0.0"), moduleLicense("MIT"), moduleCategory("general") {}
};

/**
 * Cmod编译器结果
 */
struct CmodCompilerResult {
    bool success;                       // 是否成功
    std::string message;                // 结果消息
    std::vector<std::string> outputs;   // 输出信息
    CmodInfo moduleInfo;                // 模块信息（如果适用）
    
    CmodCompilerResult() : success(false) {}
};

/**
 * Cmod编译器扩展
 * 集成Cmod功能到CHTL编译器中
 */
class CmodCompilerExtension {
public:
    CmodCompilerExtension();
    ~CmodCompilerExtension();
    
    // ===== 命令处理 =====
    
    /**
     * 执行Cmod命令
     * @param options 命令选项
     * @return 执行结果
     */
    CmodCompilerResult executeCommand(const CmodCompilerOptions& options);
    
    /**
     * 解析命令行参数
     * @param args 命令行参数
     * @return 解析的选项
     */
    CmodCompilerOptions parseCommandLine(const std::vector<std::string>& args);
    
    // ===== 具体命令实现 =====
    
    /**
     * 打包模块
     * @param options 命令选项
     * @return 执行结果
     */
    CmodCompilerResult packModule(const CmodCompilerOptions& options);
    
    /**
     * 解包模块
     * @param options 命令选项
     * @return 执行结果
     */
    CmodCompilerResult unpackModule(const CmodCompilerOptions& options);
    
    /**
     * 验证模块结构
     * @param options 命令选项
     * @return 执行结果
     */
    CmodCompilerResult validateModule(const CmodCompilerOptions& options);
    
    /**
     * 显示模块信息
     * @param options 命令选项
     * @return 执行结果
     */
    CmodCompilerResult showModuleInfo(const CmodCompilerOptions& options);
    
    /**
     * 创建模块模板
     * @param options 命令选项
     * @return 执行结果
     */
    CmodCompilerResult createModuleTemplate(const CmodCompilerOptions& options);
    
    /**
     * 列出模块
     * @param options 命令选项
     * @return 执行结果
     */
    CmodCompilerResult listModules(const CmodCompilerOptions& options);
    
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
     * 处理Cmod导入请求
     * @param moduleName 模块名称
     * @param searchPaths 搜索路径
     * @return 模块路径（如果找到）
     */
    std::string resolveCmodImport(const std::string& moduleName, const std::vector<std::string>& searchPaths);
    
    /**
     * 自动解包并缓存Cmod模块
     * @param cmodPath Cmod文件路径
     * @param cacheDir 缓存目录
     * @return 解包后的目录路径
     */
    std::string autoUnpackAndCache(const std::string& cmodPath, const std::string& cacheDir);
    
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
    void printResult(const CmodCompilerResult& result) const;

private:
    std::unique_ptr<CmodManager> cmodManager_;          // Cmod管理器
    EnhancedImportManager* importManager_;              // Import管理器（非拥有）
    EnhancedNamespaceManager* namespaceManager_;        // 命名空间管理器（非拥有）
    
    std::vector<std::string> moduleSearchPaths_;       // 模块搜索路径
    std::string cacheDirectory_;                        // 缓存目录
    
    // 内部工具方法
    bool createDirectoryStructure(const std::string& moduleDir, const CmodCompilerOptions& options);
    std::string createMainChtlFile(const CmodCompilerOptions& options);
    std::string createInfoChtlFile(const CmodCompilerOptions& options);
    std::vector<std::string> findCmodFiles(const std::string& directory);
    std::string generateCacheKey(const std::string& cmodPath);
    bool isCacheValid(const std::string& cacheDir, const std::string& cmodPath);
    void printUsage() const;
};

/**
 * Cmod命令行接口
 * 提供独立的Cmod命令行工具
 */
class CmodCommandLineInterface {
public:
    CmodCommandLineInterface();
    ~CmodCommandLineInterface();
    
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
    void setCompilerExtension(std::shared_ptr<CmodCompilerExtension> extension);

private:
    std::shared_ptr<CmodCompilerExtension> extension_;
    
    void printBanner() const;
    void printUsage() const;
    std::vector<std::string> parseArguments(int argc, char* argv[]);
};

} // namespace chtl