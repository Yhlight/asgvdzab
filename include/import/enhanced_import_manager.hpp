#pragma once

#include "common/types.hpp"
#include "ast/chtl_ast.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <filesystem>
#include <mutex>

namespace chtl {

/**
 * 导入类型枚举
 * 严格按照CHTL语法文档定义
 */
enum class ImportType {
    HTML,           // @Html
    STYLE,          // @Style  
    JAVASCRIPT,     // @JavaScript
    CHTL,           // @Chtl
    CJMOD,          // @CJmod
    CUSTOM,         // [Custom]
    TEMPLATE        // [Template]
};

/**
 * 导入路径类型
 */
enum class ImportPathType {
    NAME_ONLY,          // 仅名称(不带后缀)
    NAME_WITH_EXT,      // 名称(带后缀)
    SPECIFIC_PATH,      // 具体路径(指向文件)
    FOLDER_PATH,        // 文件夹路径
    WILDCARD_ALL,       // 通配符 * 或 .*
    WILDCARD_EXT,       // 通配符 *.ext 或 .*.ext
    SUBMODULE_ALL,      // 子模块通配符 Module.*
    SUBMODULE_SPECIFIC  // 子模块具体 Module.SubModule
};

/**
 * 导入路径信息
 */
struct ImportPathInfo {
    std::string originalPath;       // 原始路径字符串
    std::string normalizedPath;     // 标准化路径
    ImportPathType pathType;        // 路径类型
    std::string extension;          // 文件扩展名(如果有)
    std::string moduleName;         // 模块名(如果是模块导入)
    std::string subModuleName;      // 子模块名(如果有)
    bool isWildcard;               // 是否为通配符导入
    
    ImportPathInfo() : pathType(ImportPathType::NAME_ONLY), isWildcard(false) {}
};

/**
 * 导入记录
 */
struct ImportRecord {
    std::string importId;           // 导入唯一标识
    ImportType type;                // 导入类型
    ImportPathInfo pathInfo;        // 路径信息
    std::string asName;             // as 别名(如果有)
    std::string resolvedPath;       // 解析后的实际文件路径
    Position position;              // 导入语句位置
    std::string sourceFile;         // 源文件路径
    bool hasAsClause;              // 是否有as语句
    
    ImportRecord() : type(ImportType::CHTL), hasAsClause(false) {}
};

/**
 * 导入搜索路径配置
 */
struct ImportSearchConfig {
    std::string officialModulePath;     // 官方模块路径
    std::string currentDirectory;       // 当前目录
    std::string moduleDirectory;        // 模块目录
    std::vector<std::string> searchPaths; // 额外搜索路径
    
    ImportSearchConfig() {
        officialModulePath = "module";
        moduleDirectory = "module";
    }
};

/**
 * 导入结果
 */
struct ImportResult {
    bool success;                       // 是否成功
    std::string errorMessage;           // 错误信息
    std::vector<std::string> resolvedFiles; // 解析的文件列表
    std::vector<ImportRecord> imports;  // 导入记录
    
    ImportResult() : success(false) {}
};

/**
 * 循环依赖检测器
 */
class CircularDependencyDetector {
public:
    CircularDependencyDetector();
    
    /**
     * 添加依赖关系
     * @param from 源文件
     * @param to 目标文件
     * @return 是否成功添加(false表示会造成循环依赖)
     */
    bool addDependency(const std::string& from, const std::string& to);
    
    /**
     * 检查是否存在循环依赖
     * @param from 源文件
     * @param to 目标文件
     * @return 是否存在循环依赖
     */
    bool hasCircularDependency(const std::string& from, const std::string& to) const;
    
    /**
     * 获取循环依赖路径
     * @param from 源文件
     * @param to 目标文件
     * @return 循环依赖路径
     */
    std::vector<std::string> getCircularPath(const std::string& from, const std::string& to) const;
    
    /**
     * 清空依赖图
     */
    void clear();
    
    /**
     * 获取依赖图信息
     * @return 依赖图字符串表示
     */
    std::string getDependencyGraph() const;

private:
    std::unordered_map<std::string, std::unordered_set<std::string>> dependencies_;
    mutable std::mutex mutex_;
    
    bool hasPathDFS(const std::string& from, const std::string& to, 
                   std::unordered_set<std::string>& visited) const;
    void getPathDFS(const std::string& current, const std::string& target,
                   std::vector<std::string>& path, std::unordered_set<std::string>& visited,
                   std::vector<std::string>& result) const;
};

/**
 * 路径标准化器
 */
class PathNormalizer {
public:
    /**
     * 标准化路径
     * @param path 原始路径
     * @param basePath 基础路径
     * @return 标准化后的路径
     */
    static std::string normalize(const std::string& path, const std::string& basePath = "");
    
    /**
     * 解析路径信息
     * @param path 路径字符串
     * @return 路径信息
     */
    static ImportPathInfo parsePathInfo(const std::string& path);
    
    /**
     * 检查两个路径是否相同
     * @param path1 路径1
     * @param path2 路径2
     * @return 是否相同
     */
    static bool isSamePath(const std::string& path1, const std::string& path2);
    
    /**
     * 获取文件扩展名
     * @param path 文件路径
     * @return 扩展名(不含点)
     */
    static std::string getExtension(const std::string& path);
    
    /**
     * 获取文件名(不含扩展名)
     * @param path 文件路径
     * @return 文件名
     */
    static std::string getFileNameWithoutExtension(const std::string& path);
    
    /**
     * 检查路径是否为通配符
     * @param path 路径
     * @return 是否为通配符
     */
    static bool isWildcardPath(const std::string& path);
};

/**
 * 增强的Import管理器
 * 严格按照CHTL语法文档实现Import功能增强
 */
class EnhancedImportManager {
public:
    explicit EnhancedImportManager(const ImportSearchConfig& config = ImportSearchConfig());
    ~EnhancedImportManager();
    
    // ===== 配置管理 =====
    
    /**
     * 设置搜索配置
     * @param config 搜索配置
     */
    void setSearchConfig(const ImportSearchConfig& config);
    
    /**
     * 获取搜索配置
     * @return 搜索配置
     */
    const ImportSearchConfig& getSearchConfig() const;
    
    /**
     * 添加搜索路径
     * @param path 搜索路径
     */
    void addSearchPath(const std::string& path);
    
    // ===== 导入处理 =====
    
    /**
     * 处理Import声明
     * @param importNode 导入AST节点
     * @param sourceFile 源文件路径
     * @return 导入结果
     */
    ImportResult processImport(const ImportDeclarationNode* importNode, const std::string& sourceFile);
    
    /**
     * 处理Import声明(字符串参数)
     * @param importType 导入类型字符串
     * @param fromPath 导入路径
     * @param asName as别名(可选)
     * @param sourceFile 源文件路径
     * @param position 位置信息
     * @return 导入结果
     */
    ImportResult processImport(const std::string& importType, const std::string& fromPath,
                              const std::string& asName, const std::string& sourceFile,
                              const Position& position);
    
    // ===== @Html/@Style/@JavaScript 导入处理 =====
    
    /**
     * 处理原始资源导入(@Html/@Style/@JavaScript)
     * @param type 导入类型
     * @param pathInfo 路径信息
     * @param asName as别名
     * @param sourceFile 源文件
     * @return 导入结果
     */
    ImportResult processOriginImport(ImportType type, const ImportPathInfo& pathInfo,
                                    const std::string& asName, const std::string& sourceFile);
    
    // ===== @Chtl 导入处理 =====
    
    /**
     * 处理CHTL导入
     * @param pathInfo 路径信息
     * @param sourceFile 源文件
     * @return 导入结果
     */
    ImportResult processChtlImport(const ImportPathInfo& pathInfo, const std::string& sourceFile);
    
    // ===== @CJmod 导入处理 =====
    
    /**
     * 处理CJmod导入
     * @param pathInfo 路径信息
     * @param sourceFile 源文件
     * @return 导入结果
     */
    ImportResult processCjmodImport(const ImportPathInfo& pathInfo, const std::string& sourceFile);
    
    // ===== 官方模块前缀支持 =====
    
    /**
     * 解析模块导入路径（支持chtl::官方前缀）
     * @param importPath 导入路径
     * @param importType 导入类型
     * @return 解析后的实际路径
     */
    std::string resolveModuleImportPath(const std::string& importPath, ImportType importType);
    
    /**
     * 检查是否为官方模块
     * @param moduleName 模块名称
     * @return 是否为官方模块
     */
    bool isOfficialModule(const std::string& moduleName);
    
    /**
     * 设置官方模块根目录
     * @param path 官方模块根目录路径
     */
    void setOfficialModuleRoot(const std::string& path);
    
    // ===== 路径搜索和解析 =====
    
    /**
     * 搜索文件
     * @param pathInfo 路径信息
     * @param type 导入类型
     * @param sourceFile 源文件
     * @return 找到的文件列表
     */
    std::vector<std::string> searchFiles(const ImportPathInfo& pathInfo, ImportType type,
                                        const std::string& sourceFile);
    
    /**
     * 搜索模块文件(CHTL/CJmod)
     * @param pathInfo 路径信息
     * @param type 导入类型
     * @param sourceFile 源文件
     * @return 找到的文件列表
     */
    std::vector<std::string> searchModuleFiles(const ImportPathInfo& pathInfo, ImportType type,
                                              const std::string& sourceFile);
    
    /**
     * 搜索通配符文件
     * @param pathInfo 路径信息
     * @param type 导入类型
     * @param sourceFile 源文件
     * @return 找到的文件列表
     */
    std::vector<std::string> searchWildcardFiles(const ImportPathInfo& pathInfo, ImportType type,
                                                const std::string& sourceFile);
    
    // ===== 重复导入和循环依赖检测 =====
    
    /**
     * 检查是否重复导入
     * @param normalizedPath 标准化路径
     * @param sourceFile 源文件
     * @return 是否重复导入
     */
    bool isDuplicateImport(const std::string& normalizedPath, const std::string& sourceFile);
    
    /**
     * 添加导入记录
     * @param record 导入记录
     * @return 是否成功添加
     */
    bool addImportRecord(const ImportRecord& record);
    
    /**
     * 获取所有导入记录
     * @return 导入记录列表
     */
    const std::vector<ImportRecord>& getImportRecords() const;
    
    /**
     * 清空导入记录
     */
    void clearImportRecords();
    
    // ===== 实用工具 =====
    
    /**
     * 解析导入类型
     * @param typeString 类型字符串
     * @return 导入类型
     */
    static ImportType parseImportType(const std::string& typeString);
    
    /**
     * 获取导入类型字符串
     * @param type 导入类型
     * @return 类型字符串
     */
    static std::string getImportTypeString(ImportType type);
    
    /**
     * 获取导入类型的文件扩展名
     * @param type 导入类型
     * @return 扩展名列表
     */
    static std::vector<std::string> getImportTypeExtensions(ImportType type);
    
    /**
     * 检查文件是否存在
     * @param path 文件路径
     * @return 是否存在
     */
    static bool fileExists(const std::string& path);
    
    /**
     * 检查目录是否存在
     * @param path 目录路径
     * @return 是否存在
     */
    static bool directoryExists(const std::string& path);
    
    /**
     * 列出目录中的文件
     * @param directory 目录路径
     * @param extensions 文件扩展名过滤(空表示所有文件)
     * @return 文件列表
     */
    static std::vector<std::string> listFiles(const std::string& directory,
                                             const std::vector<std::string>& extensions = {});
    
    // ===== 调试和统计 =====
    
    /**
     * 获取导入统计信息
     * @return 统计信息字符串
     */
    std::string getImportStatistics() const;
    
    /**
     * 获取依赖图信息
     * @return 依赖图字符串
     */
    std::string getDependencyGraph() const;
    
    /**
     * 转储导入记录
     * @return 导入记录字符串
     */
    std::string dumpImportRecords() const;

private:
    ImportSearchConfig config_;                     // 搜索配置
    std::vector<ImportRecord> importRecords_;       // 导入记录
    std::unordered_set<std::string> duplicateChecker_; // 重复导入检查器
    std::unique_ptr<CircularDependencyDetector> circularDetector_; // 循环依赖检测器
    std::string officialModuleRoot_;                // 官方模块根目录
    mutable std::mutex mutex_;                      // 线程安全互斥锁
    
    // 内部工具方法
    std::string resolveRelativePath(const std::string& path, const std::string& basePath);
    std::string generateImportId(const ImportRecord& record);
    bool validateImportRecord(const ImportRecord& record);
    void updateDependencies(const std::string& sourceFile, const std::vector<std::string>& targetFiles);
};

} // namespace chtl