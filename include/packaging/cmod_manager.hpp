#pragma once

#include "simple_zip.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <filesystem>

namespace chtl {

/**
 * CMOD模块信息
 */
struct CMODInfo {
    std::string name;               // 模块名称
    std::string version;            // 版本号
    std::string author;             // 作者
    std::string description;        // 描述
    std::vector<std::string> dependencies; // 依赖模块
    std::vector<std::string> exports;      // 导出的组件
    std::string mainFile;           // 主文件
    std::time_t createTime;         // 创建时间
    std::time_t modifyTime;         // 修改时间
    
    CMODInfo() : createTime(0), modifyTime(0) {}
};

/**
 * CMOD模块结构验证器
 */
class CMODValidator {
public:
    /**
     * 验证CMOD目录结构
     * @param dirPath 目录路径
     * @return 验证结果和错误信息
     */
    static std::pair<bool, std::string> validateDirectory(const std::string& dirPath);
    
    /**
     * 验证CMOD文件
     * @param cmodPath CMOD文件路径
     * @return 验证结果和错误信息
     */
    static std::pair<bool, std::string> validateCMODFile(const std::string& cmodPath);
    
    /**
     * 验证模块信息文件
     * @param infoContent info文件内容
     * @return 验证结果和错误信息
     */
    static std::pair<bool, std::string> validateInfoFile(const std::string& infoContent);
    
    /**
     * 生成CMOD结构检查报告
     * @param dirPath 目录路径
     * @return 检查报告
     */
    static std::string generateStructureReport(const std::string& dirPath);

private:
    /**
     * 检查必需的目录结构
     * @param dirPath 目录路径
     * @return {是否有效, 错误信息}
     */
    static std::pair<bool, std::string> checkRequiredStructure(const std::string& dirPath);
    
    /**
     * 检查CHTL文件语法
     * @param filePath 文件路径
     * @return {是否有效, 错误信息}
     */
    static std::pair<bool, std::string> checkCHTLSyntax(const std::string& filePath);
};

/**
 * CMOD打包器
 * 将符合CMOD格式的目录打包为.cmod文件
 */
class CMODPacker {
public:
    CMODPacker();
    ~CMODPacker();
    
    /**
     * 打包CMOD目录
     * @param dirPath 源目录路径
     * @param outputPath 输出的.cmod文件路径
     * @param options 打包选项
     * @return 是否成功
     */
    bool packDirectory(const std::string& dirPath, const std::string& outputPath, 
                      const std::unordered_map<std::string, std::string>& options = {});
    
    /**
     * 添加打包过滤器
     * @param pattern 文件模式（支持通配符）
     * @param include 是否包含（true）或排除（false）
     */
    void addFilter(const std::string& pattern, bool include);
    
    /**
     * 设置压缩级别
     * @param level 压缩级别（0-9）
     */
    void setCompressionLevel(int level) { compressionLevel_ = level; }
    
    /**
     * 获取打包统计信息
     * @return 统计信息字符串
     */
    std::string getPackingStatistics() const;
    
    /**
     * 清除统计信息
     */
    void clearStatistics();

private:
    struct Filter {
        std::string pattern;
        bool include;
    };
    
    std::vector<Filter> filters_;
    int compressionLevel_;
    
    // 统计信息
    size_t totalFiles_;
    size_t totalSize_;
    size_t compressedSize_;
    std::time_t packingTime_;
    
    /**
     * 应用文件过滤器
     * @param filePath 文件路径
     * @return 是否应该包含此文件
     */
    bool shouldIncludeFile(const std::string& filePath) const;
    
    /**
     * 生成模块信息文件
     * @param dirPath 源目录
     * @return 模块信息内容
     */
    std::string generateModuleInfo(const std::string& dirPath);
    
    /**
     * 模式匹配
     * @param pattern 模式
     * @param text 文本
     * @return 是否匹配
     */
    bool matchPattern(const std::string& pattern, const std::string& text) const;
};

/**
 * CMOD解包器
 * 将.cmod文件解包为目录结构
 */
class CMODUnpacker {
public:
    CMODUnpacker();
    ~CMODUnpacker();
    
    /**
     * 解包CMOD文件
     * @param cmodPath .cmod文件路径
     * @param outputDir 输出目录
     * @param options 解包选项
     * @return 是否成功
     */
    bool unpackFile(const std::string& cmodPath, const std::string& outputDir,
                   const std::unordered_map<std::string, std::string>& options = {});
    
    /**
     * 获取CMOD文件信息
     * @param cmodPath .cmod文件路径
     * @return 模块信息
     */
    CMODInfo getCMODInfo(const std::string& cmodPath);
    
    /**
     * 列出CMOD文件内容
     * @param cmodPath .cmod文件路径
     * @return 文件列表
     */
    std::vector<std::string> listContents(const std::string& cmodPath);
    
    /**
     * 提取特定文件
     * @param cmodPath .cmod文件路径
     * @param fileName 文件名
     * @param outputPath 输出路径
     * @return 是否成功
     */
    bool extractFile(const std::string& cmodPath, const std::string& fileName, 
                    const std::string& outputPath);
    
    /**
     * 获取解包统计信息
     * @return 统计信息字符串
     */
    std::string getUnpackingStatistics() const;

private:
    // 统计信息
    size_t extractedFiles_;
    size_t totalSize_;
    std::time_t unpackingTime_;
    
    /**
     * 解析模块信息
     * @param infoContent info文件内容
     * @return 模块信息
     */
    CMODInfo parseModuleInfo(const std::string& infoContent);
    
    /**
     * 验证解包完整性
     * @param outputDir 输出目录
     * @param expectedFiles 期望的文件列表
     * @return 是否完整
     */
    bool verifyUnpackingIntegrity(const std::string& outputDir, 
                                 const std::vector<std::string>& expectedFiles);
};

/**
 * CMOD管理器
 * 统一管理CMOD模块的生命周期
 */
class CMODManager {
public:
    CMODManager();
    ~CMODManager();
    
    /**
     * 注册CMOD搜索路径
     * @param path 搜索路径
     */
    void addSearchPath(const std::string& path);
    
    /**
     * 安装CMOD模块
     * @param cmodPath .cmod文件路径
     * @param installDir 安装目录（可选，默认使用配置的安装目录）
     * @return 是否成功
     */
    bool installCMOD(const std::string& cmodPath, const std::string& installDir = "");
    
    /**
     * 卸载CMOD模块
     * @param moduleName 模块名称
     * @return 是否成功
     */
    bool uninstallCMOD(const std::string& moduleName);
    
    /**
     * 查找CMOD模块
     * @param moduleName 模块名称
     * @return 模块路径（空字符串表示未找到）
     */
    std::string findCMOD(const std::string& moduleName);
    
    /**
     * 列出所有已安装的CMOD模块
     * @return 模块信息列表
     */
    std::vector<CMODInfo> listInstalledCMODs();
    
    /**
     * 获取模块依赖
     * @param moduleName 模块名称
     * @return 依赖列表
     */
    std::vector<std::string> getModuleDependencies(const std::string& moduleName);
    
    /**
     * 检查模块依赖
     * @param moduleName 模块名称
     * @return {是否满足依赖, 缺失的依赖列表}
     */
    std::pair<bool, std::vector<std::string>> checkDependencies(const std::string& moduleName);
    
    /**
     * 创建新的CMOD项目模板
     * @param projectPath 项目路径
     * @param moduleName 模块名称
     * @param options 创建选项
     * @return 是否成功
     */
    bool createCMODProject(const std::string& projectPath, const std::string& moduleName,
                          const std::unordered_map<std::string, std::string>& options = {});
    
    /**
     * 验证CMOD项目
     * @param projectPath 项目路径
     * @return 验证报告
     */
    std::string validateCMODProject(const std::string& projectPath);
    
    /**
     * 构建CMOD项目
     * @param projectPath 项目路径
     * @param outputPath 输出.cmod文件路径
     * @return 是否成功
     */
    bool buildCMODProject(const std::string& projectPath, const std::string& outputPath);
    
    /**
     * 获取管理器统计信息
     * @return 统计信息
     */
    std::string getManagerStatistics() const;

private:
    std::vector<std::string> searchPaths_;
    std::string defaultInstallDir_;
    std::unordered_map<std::string, CMODInfo> installedModules_;
    
    CMODPacker packer_;
    CMODUnpacker unpacker_;
    
    /**
     * 扫描已安装的模块
     */
    void scanInstalledModules();
    
    /**
     * 解析模块信息文件
     * @param infoPath info文件路径
     * @return 模块信息
     */
    CMODInfo parseInfoFile(const std::string& infoPath);
    
    /**
     * 生成默认项目结构
     * @param projectPath 项目路径
     * @param moduleName 模块名称
     * @return 是否成功
     */
    bool generateDefaultStructure(const std::string& projectPath, const std::string& moduleName);
    
    /**
     * 创建示例文件
     * @param projectPath 项目路径
     * @param moduleName 模块名称
     * @return 是否成功
     */
    bool createExampleFiles(const std::string& projectPath, const std::string& moduleName);
    
    /**
     * 检查路径安全性
     * @param path 路径
     * @return 是否安全
     */
    bool isPathSafe(const std::string& path) const;
};

} // namespace chtl