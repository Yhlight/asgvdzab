#ifndef CHTL_MODULE_CMODPACKAGER_H
#define CHTL_MODULE_CMODPACKAGER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace Chtl {

// CMOD文件头信息
struct CmodHeader {
    char magic[4];          // "CMOD"
    uint32_t version;       // 版本号
    uint32_t fileCount;     // 文件数量
    uint32_t metadataSize;  // 元数据大小
    uint32_t totalSize;     // 总大小
    uint64_t timestamp;     // 创建时间戳
    char moduleName[64];    // 模块名称
    char author[64];        // 作者
    char description[256];  // 描述
};

// CMOD文件项
struct CmodFileEntry {
    std::string path;       // 相对路径
    uint32_t offset;        // 在包中的偏移
    uint32_t size;          // 文件大小
    uint32_t compressedSize; // 压缩后大小（如果使用压缩）
    uint32_t checksum;      // CRC32校验和
    bool isCompressed;      // 是否压缩
};

// CMOD元数据
struct CmodMetadata {
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::vector<std::string> dependencies;
    std::unordered_map<std::string, std::string> properties;
};

class CmodPackager {
public:
    CmodPackager();
    ~CmodPackager();
    
    // 打包功能
    bool packDirectory(const std::string& sourceDir, const std::string& outputFile);
    bool packWithMetadata(const std::string& sourceDir, const std::string& outputFile, 
                         const CmodMetadata& metadata);
    
    // 解包功能
    bool unpack(const std::string& cmodFile, const std::string& outputDir);
    bool unpackToMemory(const std::string& cmodFile);
    
    // 查询功能
    bool readHeader(const std::string& cmodFile, CmodHeader& header);
    bool listFiles(const std::string& cmodFile, std::vector<CmodFileEntry>& entries);
    bool extractFile(const std::string& cmodFile, const std::string& filePath, 
                    std::vector<uint8_t>& content);
    
    // 验证功能
    bool verify(const std::string& cmodFile);
    bool checkIntegrity(const std::string& cmodFile);
    
    // 获取错误信息
    const std::string& getLastError() const { return lastError_; }
    
    // 设置选项
    void setCompression(bool enable) { useCompression_ = enable; }
    void setCompressionLevel(int level) { compressionLevel_ = level; }
    
private:
    bool useCompression_;
    int compressionLevel_;
    std::string lastError_;
    
    // 内存中的文件缓存（用于unpackToMemory）
    std::unordered_map<std::string, std::vector<uint8_t>> memoryCache_;
    
    // 辅助方法
    bool validateCmodStructure(const std::string& sourceDir);
    bool collectFiles(const std::string& sourceDir, std::vector<std::string>& files);
    bool writeHeader(std::ofstream& out, const CmodHeader& header);
    bool writeFileEntry(std::ofstream& out, const CmodFileEntry& entry);
    bool writeFileContent(std::ofstream& out, const std::string& filePath, 
                         CmodFileEntry& entry);
    
    uint32_t calculateCRC32(const std::vector<uint8_t>& data);
    bool compressData(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
    bool decompressData(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
    
    std::string getRelativePath(const std::string& basePath, const std::string& fullPath);
    void createDirectories(const std::string& path);
};

} // namespace Chtl

#endif // CHTL_MODULE_CMODPACKAGER_H