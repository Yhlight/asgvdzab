#ifndef CHTL_UTILS_SIMPLE_ZIP_HPP
#define CHTL_UTILS_SIMPLE_ZIP_HPP

#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <fstream>
#include <cstring>
#include <cstdint>

namespace CHTL {
namespace Utils {

// ZIP文件头结构
struct ZipFileHeader {
    uint32_t signature = 0x04034b50;           // 文件头标识 "PK\x03\x04"
    uint16_t version = 20;                     // 解压所需版本
    uint16_t flags = 0;                        // 通用标志位
    uint16_t compression = 0;                  // 压缩方法 (0 = 存储, 8 = DEFLATE)
    uint16_t modTime = 0;                      // 修改时间
    uint16_t modDate = 0;                      // 修改日期
    uint32_t crc32 = 0;                        // CRC-32校验
    uint32_t compressedSize = 0;               // 压缩后大小
    uint32_t uncompressedSize = 0;             // 压缩前大小
    uint16_t filenameLength = 0;               // 文件名长度
    uint16_t extraFieldLength = 0;             // 扩展字段长度
    
    void write(std::ostream& out) const;
    bool read(std::istream& in);
};

// ZIP中央目录条目
struct ZipCentralDirEntry {
    uint32_t signature = 0x02014b50;           // 中央目录标识 "PK\x01\x02"
    uint16_t versionMadeBy = 20;               // 压缩使用的版本
    uint16_t versionNeeded = 20;               // 解压所需版本
    uint16_t flags = 0;                        // 通用标志位
    uint16_t compression = 0;                  // 压缩方法
    uint16_t modTime = 0;                      // 修改时间
    uint16_t modDate = 0;                      // 修改日期
    uint32_t crc32 = 0;                        // CRC-32校验
    uint32_t compressedSize = 0;               // 压缩后大小
    uint32_t uncompressedSize = 0;             // 压缩前大小
    uint16_t filenameLength = 0;               // 文件名长度
    uint16_t extraFieldLength = 0;             // 扩展字段长度
    uint16_t commentLength = 0;                // 注释长度
    uint16_t diskNumberStart = 0;              // 文件开始的磁盘号
    uint16_t internalFileAttribs = 0;          // 内部文件属性
    uint32_t externalFileAttribs = 0;          // 外部文件属性
    uint32_t localHeaderOffset = 0;            // 本地文件头的相对偏移
    
    void write(std::ostream& out) const;
    bool read(std::istream& in);
};

// ZIP中央目录结束记录
struct ZipEndOfCentralDir {
    uint32_t signature = 0x06054b50;           // 结束标识 "PK\x05\x06"
    uint16_t diskNumber = 0;                   // 当前磁盘号
    uint16_t centralDirDisk = 0;               // 中央目录开始的磁盘号
    uint16_t centralDirEntriesOnDisk = 0;      // 当前磁盘上的中央目录条目数
    uint16_t totalCentralDirEntries = 0;       // 中央目录条目总数
    uint32_t centralDirSize = 0;               // 中央目录大小
    uint32_t centralDirOffset = 0;             // 中央目录偏移
    uint16_t commentLength = 0;                // 注释长度
    
    void write(std::ostream& out) const;
    bool read(std::istream& in);
};

// ZIP文件条目
struct ZipEntry {
    std::string filename;
    std::vector<uint8_t> data;
    uint32_t crc32;
    bool isDirectory = false;
    
    ZipEntry() = default;
    ZipEntry(const std::string& name, const std::vector<uint8_t>& fileData);
    ZipEntry(const std::string& name, const std::string& textData);
    
    // 计算CRC32
    void calculateCrc32();
};

// 简单ZIP创建器
class SimpleZipWriter {
private:
    std::vector<std::unique_ptr<ZipEntry>> entries_;
    std::string outputPath_;
    
public:
    explicit SimpleZipWriter(const std::string& outputPath);
    ~SimpleZipWriter() = default;
    
    // 禁用拷贝和移动
    SimpleZipWriter(const SimpleZipWriter&) = delete;
    SimpleZipWriter& operator=(const SimpleZipWriter&) = delete;
    SimpleZipWriter(SimpleZipWriter&&) = delete;
    SimpleZipWriter& operator=(SimpleZipWriter&&) = delete;
    
    // 添加文件
    bool addFile(const std::string& filename, const std::vector<uint8_t>& data);
    bool addFile(const std::string& filename, const std::string& textData);
    bool addFileFromDisk(const std::string& filename, const std::string& diskPath);
    
    // 添加目录
    bool addDirectory(const std::string& dirName);
    
    // 从文件夹添加所有文件
    bool addDirectoryRecursive(const std::string& basePath, const std::string& zipPath = "");
    
    // 写入ZIP文件
    bool write();
    
    // 获取条目数量
    size_t getEntryCount() const { return entries_.size(); }
    
    // 清空条目
    void clear();

private:
    // 标准化文件路径（转换为ZIP格式）
    std::string normalizePath(const std::string& path);
    
    // 写入本地文件头和数据
    void writeLocalFileHeader(std::ostream& out, const ZipEntry& entry, uint32_t& offset);
    
    // 写入中央目录
    void writeCentralDirectory(std::ostream& out, const std::vector<std::pair<ZipCentralDirEntry, std::string>>& centralEntries);
};

// 简单ZIP读取器
class SimpleZipReader {
private:
    std::string inputPath_;
    std::vector<std::unique_ptr<ZipEntry>> entries_;
    bool loaded_ = false;
    
public:
    explicit SimpleZipReader(const std::string& inputPath);
    ~SimpleZipReader() = default;
    
    // 禁用拷贝和移动
    SimpleZipReader(const SimpleZipReader&) = delete;
    SimpleZipReader& operator=(const SimpleZipReader&) = delete;
    SimpleZipReader(SimpleZipReader&&) = delete;
    SimpleZipReader& operator=(SimpleZipReader&&) = delete;
    
    // 加载ZIP文件
    bool load();
    
    // 获取所有条目
    const std::vector<std::unique_ptr<ZipEntry>>& getEntries() const { return entries_; }
    
    // 查找文件
    const ZipEntry* findEntry(const std::string& filename) const;
    
    // 提取文件到内存
    std::vector<uint8_t> extractFile(const std::string& filename) const;
    std::string extractTextFile(const std::string& filename) const;
    
    // 提取文件到磁盘
    bool extractFile(const std::string& filename, const std::string& outputPath) const;
    
    // 提取所有文件到目录
    bool extractAll(const std::string& outputDir) const;
    
    // 列出所有文件名
    std::vector<std::string> listFiles() const;
    
    // 检查是否已加载
    bool isLoaded() const { return loaded_; }
    
    // 获取条目数量
    size_t getEntryCount() const { return entries_.size(); }

private:
    // 读取中央目录
    bool readCentralDirectory(std::istream& in);
    
    // 读取文件数据
    std::vector<uint8_t> readFileData(std::istream& in, const ZipCentralDirEntry& entry) const;
    
    // 创建目录
    bool createDirectories(const std::string& path) const;
};

// CRC32计算器
class CRC32 {
private:
    static uint32_t crcTable_[256];
    static bool tableInitialized_;
    
    static void initTable();
    
public:
    // 计算数据的CRC32
    static uint32_t calculate(const void* data, size_t length);
    static uint32_t calculate(const std::vector<uint8_t>& data);
    static uint32_t calculate(const std::string& data);
    
    // 增量计算CRC32
    static uint32_t update(uint32_t crc, const void* data, size_t length);
};

// 工厂函数
std::unique_ptr<SimpleZipWriter> createZipWriter(const std::string& outputPath);
std::unique_ptr<SimpleZipReader> createZipReader(const std::string& inputPath);

// 便捷函数
bool createZipFromDirectory(const std::string& sourceDir, const std::string& zipPath);
bool extractZipToDirectory(const std::string& zipPath, const std::string& outputDir);

} // namespace Utils
} // namespace CHTL

#endif // CHTL_UTILS_SIMPLE_ZIP_HPP