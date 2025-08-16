#pragma once

#include "common/types.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <filesystem>

namespace chtl {

/**
 * CJmod模块信息
 * 严格按照CHTL语法文档定义
 */
struct CjmodInfo {
    std::string name;           // 模块名称
    std::string version;        // 版本
    std::string description;    // 描述
    std::string author;         // 作者
    std::string license;        // 许可证
    std::string dependencies;   // 依赖
    std::string category;       // 分类
    std::string minCHTLVersion; // 最小CHTL版本要求
    
    // CJmod特有信息
    std::string cppStandard;    // C++标准 (如 "C++17")
    std::string compilerFlags;  // 编译器标志
    std::vector<std::string> includePaths;  // 包含路径
    std::vector<std::string> libraries;     // 链接库
    std::vector<std::string> jsExtensions;  // 扩展的JS语法
    
    // 额外信息
    std::string homepage;       // 主页
    std::string repository;     // 仓库
    std::vector<std::string> keywords; // 关键词
    
    CjmodInfo() : license("MIT"), minCHTLVersion("1.0.0"), cppStandard("C++17") {}
};

/**
 * CJmod文件条目
 */
struct CjmodEntry {
    std::string path;           // 文件路径
    std::vector<uint8_t> data;  // 文件数据
    bool isDirectory;           // 是否为目录
    uint64_t size;              // 文件大小
    uint64_t modifiedTime;      // 修改时间
    std::string fileType;       // 文件类型 (cpp, h, chtl)
    
    CjmodEntry() : isDirectory(false), size(0), modifiedTime(0) {}
};

/**
 * CJmod打包结果
 */
struct CjmodPackResult {
    bool success;               // 是否成功
    std::string errorMessage;   // 错误信息
    std::string outputPath;     // 输出路径
    size_t totalFiles;          // 总文件数
    size_t totalSize;           // 总大小
    size_t cppFiles;            // C++源文件数
    size_t headerFiles;         // 头文件数
    size_t infoFiles;           // 信息文件数
    
    CjmodPackResult() : success(false), totalFiles(0), totalSize(0), 
                       cppFiles(0), headerFiles(0), infoFiles(0) {}
};

/**
 * CJmod解包结果
 */
struct CjmodUnpackResult {
    bool success;               // 是否成功
    std::string errorMessage;   // 错误信息
    std::string outputDir;      // 输出目录
    std::vector<std::string> extractedFiles; // 解压的文件列表
    CjmodInfo moduleInfo;       // 模块信息
    
    CjmodUnpackResult() : success(false) {}
};

/**
 * CJmod验证结果
 */
struct CjmodValidationResult {
    bool isValid;               // 是否有效
    std::string errorMessage;   // 错误信息
    std::vector<std::string> warnings; // 警告信息
    std::vector<std::string> missingFiles; // 缺失文件
    CjmodInfo moduleInfo;       // 解析的模块信息
    
    CjmodValidationResult() : isValid(false) {}
};

/**
 * 官方模块前缀处理器
 * 处理 chtl:: 前缀的官方模块
 */
class OfficialModuleResolver {
public:
    OfficialModuleResolver();
    ~OfficialModuleResolver();
    
    /**
     * 检查是否为官方模块前缀
     * @param moduleName 模块名称
     * @return 是否为官方模块
     */
    static bool isOfficialModule(const std::string& moduleName);
    
    /**
     * 提取官方模块名称（去除chtl::前缀）
     * @param moduleName 完整模块名称
     * @return 去除前缀的模块名称
     */
    static std::string extractOfficialModuleName(const std::string& moduleName);
    
    /**
     * 获取官方模块路径
     * @param moduleName 模块名称（不含前缀）
     * @param moduleType 模块类型（cmod或cjmod）
     * @return 官方模块路径
     */
    std::string getOfficialModulePath(const std::string& moduleName, const std::string& moduleType);
    
    /**
     * 设置官方模块根目录
     * @param path 官方模块根目录路径
     */
    void setOfficialModuleRoot(const std::string& path);
    
    /**
     * 获取官方模块根目录
     * @return 官方模块根目录路径
     */
    const std::string& getOfficialModuleRoot() const;
    
    /**
     * 列出所有官方模块
     * @param moduleType 模块类型（cmod或cjmod，空字符串表示全部）
     * @return 官方模块列表
     */
    std::vector<std::string> listOfficialModules(const std::string& moduleType = "") const;

private:
    std::string officialModuleRoot_;    // 官方模块根目录
};

/**
 * CJmod编译接口
 * 提供编译CJmod的C++代码接口
 */
class CjmodCompilerInterface {
public:
    CjmodCompilerInterface();
    ~CjmodCompilerInterface();
    
    /**
     * 编译CJmod模块
     * @param sourceDir CJmod源目录
     * @param outputDir 输出目录
     * @param moduleInfo 模块信息
     * @return 是否编译成功
     */
    bool compileCjmodModule(const std::string& sourceDir, const std::string& outputDir, const CjmodInfo& moduleInfo);
    
    /**
     * 生成CJmod动态库
     * @param sourceDir 源目录
     * @param outputPath 输出动态库路径
     * @param moduleInfo 模块信息
     * @return 是否生成成功
     */
    bool generateDynamicLibrary(const std::string& sourceDir, const std::string& outputPath, const CjmodInfo& moduleInfo);
    
    /**
     * 验证C++代码语法
     * @param cppFiles C++文件列表
     * @return 验证结果
     */
    std::vector<std::string> validateCppSyntax(const std::vector<std::string>& cppFiles);
    
    /**
     * 检查依赖库
     * @param libraries 库列表
     * @return 缺失的库
     */
    std::vector<std::string> checkDependencies(const std::vector<std::string>& libraries);
    
    /**
     * 设置编译器路径
     * @param compilerPath 编译器路径
     */
    void setCompilerPath(const std::string& compilerPath);
    
    /**
     * 设置编译标志
     * @param flags 编译标志
     */
    void setCompileFlags(const std::vector<std::string>& flags);

private:
    std::string compilerPath_;          // 编译器路径
    std::vector<std::string> compileFlags_; // 编译标志
    
    // 内部方法
    std::string generateBuildScript(const std::string& sourceDir, const std::string& outputPath, const CjmodInfo& moduleInfo);
    bool executeBuildCommand(const std::string& command);
};

/**
 * CHTL JS语法扩展接口
 * 定义CJmod扩展CHTL JS语法的接口
 */
class ChtlJsExtensionInterface {
public:
    ChtlJsExtensionInterface();
    virtual ~ChtlJsExtensionInterface();
    
    /**
     * 初始化扩展
     * @param context CHTL JS上下文
     * @return 是否初始化成功
     */
    virtual bool initialize(void* context) = 0;
    
    /**
     * 处理自定义JS语法
     * @param syntax 语法字符串
     * @param parameters 参数
     * @return 处理结果
     */
    virtual std::string processCustomSyntax(const std::string& syntax, const std::vector<std::string>& parameters) = 0;
    
    /**
     * 注册自定义函数
     * @param functionName 函数名
     * @param implementation 实现
     * @return 是否注册成功
     */
    virtual bool registerCustomFunction(const std::string& functionName, void* implementation) = 0;
    
    /**
     * 获取扩展名称
     * @return 扩展名称
     */
    virtual std::string getExtensionName() const = 0;
    
    /**
     * 获取扩展版本
     * @return 扩展版本
     */
    virtual std::string getExtensionVersion() const = 0;
    
    /**
     * 清理扩展
     */
    virtual void cleanup() = 0;
};

/**
 * CJmod管理器
 * 提供CJmod模块的打包、解包、编译等功能
 */
class CjmodManager {
public:
    CjmodManager();
    ~CjmodManager();
    
    // ===== CJmod打包功能 =====
    
    /**
     * 打包文件夹为CJmod文件
     * @param sourceDir 源文件夹路径
     * @param outputPath 输出.cjmod文件路径
     * @return 打包结果
     */
    CjmodPackResult packModule(const std::string& sourceDir, const std::string& outputPath);
    
    /**
     * 验证文件夹是否符合CJmod格式
     * @param sourceDir 源文件夹路径
     * @return 验证结果
     */
    CjmodValidationResult validateModuleStructure(const std::string& sourceDir);
    
    // ===== CJmod解包功能 =====
    
    /**
     * 解包CJmod文件到文件夹
     * @param cjmodPath .cjmod文件路径
     * @param outputDir 输出文件夹路径
     * @return 解包结果
     */
    CjmodUnpackResult unpackModule(const std::string& cjmodPath, const std::string& outputDir);
    
    /**
     * 读取CJmod文件信息（不解包）
     * @param cjmodPath .cjmod文件路径
     * @return 模块信息
     */
    CjmodInfo readModuleInfo(const std::string& cjmodPath);
    
    // ===== CJmod信息解析 =====
    
    /**
     * 解析Info文件
     * @param infoFilePath info文件路径
     * @return 模块信息
     */
    CjmodInfo parseInfoFile(const std::string& infoFilePath);
    
    /**
     * 生成Info文件内容
     * @param info 模块信息
     * @return Info文件内容
     */
    std::string generateInfoFile(const CjmodInfo& info);
    
    // ===== 编译和构建 =====
    
    /**
     * 编译CJmod模块
     * @param sourceDir 源目录
     * @param outputDir 输出目录
     * @return 是否成功
     */
    bool compileCjmodModule(const std::string& sourceDir, const std::string& outputDir);
    
    /**
     * 构建动态库
     * @param sourceDir 源目录
     * @param outputPath 输出动态库路径
     * @return 是否成功
     */
    bool buildDynamicLibrary(const std::string& sourceDir, const std::string& outputPath);
    
    // ===== 官方模块支持 =====
    
    /**
     * 解析模块导入（支持chtl::前缀）
     * @param importPath 导入路径
     * @param moduleType 模块类型（cmod或cjmod）
     * @return 解析后的实际路径
     */
    std::string resolveModuleImport(const std::string& importPath, const std::string& moduleType);
    
    /**
     * 获取官方模块解析器
     * @return 官方模块解析器
     */
    OfficialModuleResolver* getOfficialModuleResolver();
    
    /**
     * 获取编译器接口
     * @return 编译器接口
     */
    CjmodCompilerInterface* getCompilerInterface();
    
    // ===== 文件系统工具 =====
    
    /**
     * 递归收集目录中的所有文件
     * @param directory 目录路径
     * @param baseDir 基础目录（用于计算相对路径）
     * @return 文件路径列表
     */
    std::vector<std::string> collectFiles(const std::string& directory, const std::string& baseDir = "");
    
    /**
     * 分析文件类型
     * @param filePath 文件路径
     * @return 文件类型
     */
    std::string analyzeFileType(const std::string& filePath);
    
    /**
     * 检查是否为有效的模块名称
     * @param name 模块名称
     * @return 是否有效
     */
    static bool isValidModuleName(const std::string& name);
    
    /**
     * 检查是否为有效的版本号
     * @param version 版本号
     * @return 是否有效
     */
    static bool isValidVersion(const std::string& version);
    
    /**
     * 获取文件的相对路径
     * @param filePath 文件路径
     * @param baseDir 基础目录
     * @return 相对路径
     */
    static std::string getRelativePath(const std::string& filePath, const std::string& baseDir);
    
    // ===== 调试和统计 =====
    
    /**
     * 获取打包统计信息
     * @return 统计信息字符串
     */
    std::string getPackStatistics() const;
    
    /**
     * 设置详细输出
     * @param verbose 是否详细输出
     */
    void setVerbose(bool verbose);

private:
    bool verbose_;                              // 是否详细输出
    std::unique_ptr<class SimpleZipCompressor> compressor_;   // 压缩器（复用Cmod的）
    std::unique_ptr<class SimpleZipDecompressor> decompressor_; // 解压器（复用Cmod的）
    std::unique_ptr<OfficialModuleResolver> officialResolver_; // 官方模块解析器
    std::unique_ptr<CjmodCompilerInterface> compilerInterface_; // 编译器接口
    
    // 统计信息
    mutable size_t totalPackedFiles_;
    mutable size_t totalPackedSize_;
    mutable size_t totalUnpackedFiles_;
    mutable size_t totalUnpackedSize_;
    
    // 内部工具方法
    bool validateModuleDirectory(const std::string& moduleDir, CjmodValidationResult& result);
    bool validateSourceDirectory(const std::string& srcDir, CjmodValidationResult& result);
    bool validateInfoDirectory(const std::string& infoDir, CjmodValidationResult& result);
    CjmodInfo parseInfoContent(const std::string& content);
    std::string normalizePath(const std::string& path);
    void log(const std::string& message) const;
};

} // namespace chtl