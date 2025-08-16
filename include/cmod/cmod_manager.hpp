#pragma once

#include "common/types.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <filesystem>

namespace chtl {

/**
 * Cmod模块信息
 * 严格按照CHTL语法文档定义
 */
struct CmodInfo {
    std::string name;           // 模块名称
    std::string version;        // 版本
    std::string description;    // 描述
    std::string author;         // 作者
    std::string license;        // 许可证
    std::string dependencies;   // 依赖
    std::string category;       // 分类
    std::string minCHTLVersion; // 最小CHTL版本要求
    
    // 额外信息
    std::string homepage;       // 主页
    std::string repository;     // 仓库
    std::vector<std::string> keywords; // 关键词
    
    CmodInfo() : license("MIT"), minCHTLVersion("1.0.0") {}
};

/**
 * Cmod文件条目
 */
struct CmodEntry {
    std::string path;           // 文件路径
    std::vector<uint8_t> data;  // 文件数据
    bool isDirectory;           // 是否为目录
    uint64_t size;              // 文件大小
    uint64_t modifiedTime;      // 修改时间
    
    CmodEntry() : isDirectory(false), size(0), modifiedTime(0) {}
};

/**
 * Cmod打包结果
 */
struct CmodPackResult {
    bool success;               // 是否成功
    std::string errorMessage;   // 错误信息
    std::string outputPath;     // 输出路径
    size_t totalFiles;          // 总文件数
    size_t totalSize;           // 总大小
    
    CmodPackResult() : success(false), totalFiles(0), totalSize(0) {}
};

/**
 * Cmod解包结果
 */
struct CmodUnpackResult {
    bool success;               // 是否成功
    std::string errorMessage;   // 错误信息
    std::string outputDir;      // 输出目录
    std::vector<std::string> extractedFiles; // 解压的文件列表
    CmodInfo moduleInfo;        // 模块信息
    
    CmodUnpackResult() : success(false) {}
};

/**
 * Cmod验证结果
 */
struct CmodValidationResult {
    bool isValid;               // 是否有效
    std::string errorMessage;   // 错误信息
    std::vector<std::string> warnings; // 警告信息
    std::vector<std::string> missingFiles; // 缺失文件
    CmodInfo moduleInfo;        // 解析的模块信息
    
    CmodValidationResult() : isValid(false) {}
};

/**
 * 简单Zip压缩器
 * 基本的ZIP格式实现，用于Cmod打包
 */
class SimpleZipCompressor {
public:
    SimpleZipCompressor();
    ~SimpleZipCompressor();
    
    /**
     * 添加文件到压缩包
     * @param path 文件路径（在压缩包中的路径）
     * @param data 文件数据
     * @return 是否成功
     */
    bool addFile(const std::string& path, const std::vector<uint8_t>& data);
    
    /**
     * 添加目录到压缩包
     * @param path 目录路径
     * @return 是否成功
     */
    bool addDirectory(const std::string& path);
    
    /**
     * 写入压缩包到文件
     * @param outputPath 输出文件路径
     * @return 是否成功
     */
    bool writeToFile(const std::string& outputPath);
    
    /**
     * 清空所有条目
     */
    void clear();
    
    /**
     * 获取条目数量
     * @return 条目数量
     */
    size_t getEntryCount() const;

private:
    std::vector<CmodEntry> entries_;
    
    // ZIP格式相关方法
    uint32_t calculateCRC32(const std::vector<uint8_t>& data);
    void writeLocalFileHeader(std::vector<uint8_t>& output, const CmodEntry& entry, uint32_t crc32);
    void writeCentralDirectoryHeader(std::vector<uint8_t>& output, const CmodEntry& entry, 
                                   uint32_t crc32, uint32_t localHeaderOffset);
    void writeEndOfCentralDirectory(std::vector<uint8_t>& output, uint32_t centralDirSize, 
                                  uint32_t centralDirOffset, uint16_t entryCount);
};

/**
 * 简单Zip解压器
 * 基本的ZIP格式解压，用于Cmod解包
 */
class SimpleZipDecompressor {
public:
    SimpleZipDecompressor();
    ~SimpleZipDecompressor();
    
    /**
     * 从文件加载压缩包
     * @param filePath 压缩包文件路径
     * @return 是否成功
     */
    bool loadFromFile(const std::string& filePath);
    
    /**
     * 解压所有文件到目录
     * @param outputDir 输出目录
     * @return 是否成功
     */
    bool extractAll(const std::string& outputDir);
    
    /**
     * 解压指定文件
     * @param entryPath 文件路径（在压缩包中的路径）
     * @param outputPath 输出文件路径
     * @return 是否成功
     */
    bool extractFile(const std::string& entryPath, const std::string& outputPath);
    
    /**
     * 获取所有条目
     * @return 条目列表
     */
    const std::vector<CmodEntry>& getEntries() const;
    
    /**
     * 清空
     */
    void clear();

private:
    std::vector<CmodEntry> entries_;
    std::vector<uint8_t> fileData_;
    
    // ZIP格式解析方法
    bool parseZipFile();
    bool readLocalFileHeader(size_t& offset, CmodEntry& entry);
    bool readCentralDirectory(size_t offset, uint16_t entryCount);
    bool findEndOfCentralDirectory(uint32_t& centralDirOffset, uint16_t& entryCount);
    uint32_t readUint32(size_t offset);
    uint16_t readUint16(size_t offset);
};

/**
 * Cmod管理器
 * 提供Cmod模块的打包、解包、验证等功能
 */
class CmodManager {
public:
    CmodManager();
    ~CmodManager();
    
    // ===== Cmod打包功能 =====
    
    /**
     * 打包文件夹为Cmod文件
     * @param sourceDir 源文件夹路径
     * @param outputPath 输出.cmod文件路径
     * @return 打包结果
     */
    CmodPackResult packModule(const std::string& sourceDir, const std::string& outputPath);
    
    /**
     * 验证文件夹是否符合Cmod格式
     * @param sourceDir 源文件夹路径
     * @return 验证结果
     */
    CmodValidationResult validateModuleStructure(const std::string& sourceDir);
    
    // ===== Cmod解包功能 =====
    
    /**
     * 解包Cmod文件到文件夹
     * @param cmodPath .cmod文件路径
     * @param outputDir 输出文件夹路径
     * @return 解包结果
     */
    CmodUnpackResult unpackModule(const std::string& cmodPath, const std::string& outputDir);
    
    /**
     * 读取Cmod文件信息（不解包）
     * @param cmodPath .cmod文件路径
     * @return 模块信息
     */
    CmodInfo readModuleInfo(const std::string& cmodPath);
    
    // ===== Cmod信息解析 =====
    
    /**
     * 解析Info文件
     * @param infoFilePath info文件路径
     * @return 模块信息
     */
    CmodInfo parseInfoFile(const std::string& infoFilePath);
    
    /**
     * 生成Info文件内容
     * @param info 模块信息
     * @return Info文件内容
     */
    std::string generateInfoFile(const CmodInfo& info);
    
    // ===== 文件系统工具 =====
    
    /**
     * 递归收集目录中的所有文件
     * @param directory 目录路径
     * @param baseDir 基础目录（用于计算相对路径）
     * @return 文件路径列表
     */
    std::vector<std::string> collectFiles(const std::string& directory, const std::string& baseDir = "");
    
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
    std::unique_ptr<SimpleZipCompressor> compressor_;   // 压缩器
    std::unique_ptr<SimpleZipDecompressor> decompressor_; // 解压器
    
    // 统计信息
    mutable size_t totalPackedFiles_;
    mutable size_t totalPackedSize_;
    mutable size_t totalUnpackedFiles_;
    mutable size_t totalUnpackedSize_;
    
    // 内部工具方法
    bool validateModuleDirectory(const std::string& moduleDir, CmodValidationResult& result);
    bool validateSourceDirectory(const std::string& srcDir, CmodValidationResult& result);
    bool validateInfoDirectory(const std::string& infoDir, CmodValidationResult& result);
    CmodInfo parseInfoContent(const std::string& content);
    std::string normalizePath(const std::string& path);
    void log(const std::string& message) const;
};

} // namespace chtl