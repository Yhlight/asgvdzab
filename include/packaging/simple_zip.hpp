#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <memory>

namespace chtl {

/**
 * ZIP文件条目
 */
struct ZipEntry {
    std::string fileName;           // 文件名
    std::vector<uint8_t> data;      // 文件数据
    uint32_t crc32;                 // CRC32校验和
    uint32_t compressedSize;        // 压缩后大小
    uint32_t uncompressedSize;      // 原始大小
    uint16_t compressionMethod;     // 压缩方法 (0=无压缩, 8=deflate)
    std::time_t modificationTime;   // 修改时间
    bool isDirectory;               // 是否为目录
    
    ZipEntry() : crc32(0), compressedSize(0), uncompressedSize(0), 
                 compressionMethod(0), modificationTime(0), isDirectory(false) {}
};

/**
 * 简单ZIP创建器
 * 实现基本的ZIP文件格式支持
 */
class SimpleZipCreator {
public:
    SimpleZipCreator();
    ~SimpleZipCreator();
    
    /**
     * 添加文件到ZIP
     * @param fileName ZIP内的文件路径
     * @param filePath 本地文件路径
     * @return 是否成功
     */
    bool addFile(const std::string& fileName, const std::string& filePath);
    
    /**
     * 添加数据到ZIP
     * @param fileName ZIP内的文件路径
     * @param data 文件数据
     * @return 是否成功
     */
    bool addData(const std::string& fileName, const std::vector<uint8_t>& data);
    
    /**
     * 添加文本数据到ZIP
     * @param fileName ZIP内的文件路径
     * @param text 文本内容
     * @return 是否成功
     */
    bool addText(const std::string& fileName, const std::string& text);
    
    /**
     * 添加目录到ZIP
     * @param dirName 目录名
     * @return 是否成功
     */
    bool addDirectory(const std::string& dirName);
    
    /**
     * 递归添加目录及其内容
     * @param dirPath 本地目录路径
     * @param zipPrefix ZIP内的前缀路径
     * @return 是否成功
     */
    bool addDirectoryRecursive(const std::string& dirPath, const std::string& zipPrefix = "");
    
    /**
     * 保存ZIP文件
     * @param zipPath ZIP文件路径
     * @return 是否成功
     */
    bool save(const std::string& zipPath);
    
    /**
     * 获取ZIP条目数量
     * @return 条目数量
     */
    size_t getEntryCount() const { return entries_.size(); }
    
    /**
     * 清空所有条目
     */
    void clear();

private:
    std::vector<ZipEntry> entries_;
    
    /**
     * 计算CRC32校验和
     * @param data 数据
     * @return CRC32值
     */
    uint32_t calculateCRC32(const std::vector<uint8_t>& data);
    
    /**
     * 写入本地文件头
     * @param file 文件流
     * @param entry ZIP条目
     * @return 写入的字节数
     */
    size_t writeLocalFileHeader(std::ofstream& file, const ZipEntry& entry);
    
    /**
     * 写入中央目录条目
     * @param file 文件流
     * @param entry ZIP条目
     * @param localHeaderOffset 本地文件头偏移
     * @return 写入的字节数
     */
    size_t writeCentralDirectoryEntry(std::ofstream& file, const ZipEntry& entry, uint32_t localHeaderOffset);
    
    /**
     * 写入中央目录结尾记录
     * @param file 文件流
     * @param centralDirOffset 中央目录偏移
     * @param centralDirSize 中央目录大小
     * @return 写入的字节数
     */
    size_t writeEndOfCentralDirectory(std::ofstream& file, uint32_t centralDirOffset, uint32_t centralDirSize);
    
    /**
     * DOS时间转换
     * @param time 时间戳
     * @return DOS时间格式
     */
    uint32_t toDosTime(std::time_t time);
    
    /**
     * 写入小端格式的16位整数
     */
    void writeUint16LE(std::ofstream& file, uint16_t value);
    
    /**
     * 写入小端格式的32位整数
     */
    void writeUint32LE(std::ofstream& file, uint32_t value);
};

/**
 * 简单ZIP解压器
 */
class SimpleZipExtractor {
public:
    SimpleZipExtractor();
    ~SimpleZipExtractor();
    
    /**
     * 打开ZIP文件
     * @param zipPath ZIP文件路径
     * @return 是否成功
     */
    bool open(const std::string& zipPath);
    
    /**
     * 关闭ZIP文件
     */
    void close();
    
    /**
     * 获取ZIP中的文件列表
     * @return 文件名列表
     */
    std::vector<std::string> getFileList() const;
    
    /**
     * 提取文件到指定路径
     * @param fileName ZIP中的文件名
     * @param outputPath 输出文件路径
     * @return 是否成功
     */
    bool extractFile(const std::string& fileName, const std::string& outputPath);
    
    /**
     * 提取文件数据
     * @param fileName ZIP中的文件名
     * @return 文件数据
     */
    std::vector<uint8_t> extractData(const std::string& fileName);
    
    /**
     * 提取文本内容
     * @param fileName ZIP中的文件名
     * @return 文本内容
     */
    std::string extractText(const std::string& fileName);
    
    /**
     * 提取所有文件到目录
     * @param outputDir 输出目录
     * @return 是否成功
     */
    bool extractAll(const std::string& outputDir);
    
    /**
     * 检查文件是否存在
     * @param fileName 文件名
     * @return 是否存在
     */
    bool hasFile(const std::string& fileName) const;
    
    /**
     * 获取文件信息
     * @param fileName 文件名
     * @return 文件信息（如果不存在返回nullptr）
     */
    const ZipEntry* getFileInfo(const std::string& fileName) const;

private:
    std::string zipPath_;
    std::unordered_map<std::string, ZipEntry> entries_;
    bool isOpen_;
    
    /**
     * 读取中央目录
     * @return 是否成功
     */
    bool readCentralDirectory();
    
    /**
     * 查找中央目录结尾记录
     * @param file 文件流
     * @return 偏移位置（-1表示未找到）
     */
    long findEndOfCentralDirectory(std::ifstream& file);
    
    /**
     * 读取小端格式的16位整数
     */
    uint16_t readUint16LE(std::ifstream& file);
    
    /**
     * 读取小端格式的32位整数
     */
    uint32_t readUint32LE(std::ifstream& file);
    
    /**
     * 从DOS时间转换
     * @param dosTime DOS时间格式
     * @return 时间戳
     */
    std::time_t fromDosTime(uint32_t dosTime);
};

/**
 * ZIP工具类
 * 提供便捷的打包和解压功能
 */
class ZipUtils {
public:
    /**
     * 打包目录为ZIP文件
     * @param dirPath 目录路径
     * @param zipPath ZIP文件路径
     * @return 是否成功
     */
    static bool packDirectory(const std::string& dirPath, const std::string& zipPath);
    
    /**
     * 解压ZIP文件到目录
     * @param zipPath ZIP文件路径
     * @param outputDir 输出目录
     * @return 是否成功
     */
    static bool unpackToDirectory(const std::string& zipPath, const std::string& outputDir);
    
    /**
     * 检查ZIP文件有效性
     * @param zipPath ZIP文件路径
     * @return 是否有效
     */
    static bool isValidZip(const std::string& zipPath);
    
    /**
     * 获取ZIP文件信息
     * @param zipPath ZIP文件路径
     * @return 信息字符串
     */
    static std::string getZipInfo(const std::string& zipPath);
    
    /**
     * 创建空的ZIP文件
     * @param zipPath ZIP文件路径
     * @return 是否成功
     */
    static bool createEmptyZip(const std::string& zipPath);
};

} // namespace chtl