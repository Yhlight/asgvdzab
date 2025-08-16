#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <cstdint>
#include <chrono> // Added for std::chrono::system_clock::time_point

namespace chtl {

// ZIP文件头结构
struct ZipFileHeader {
    uint32_t signature;                 // 文件头签名 (0x04034b50)
    uint16_t version_needed;            // 解压所需版本
    uint16_t flags;                     // 通用位标志
    uint16_t compression_method;        // 压缩方法
    uint16_t last_mod_time;             // 最后修改时间
    uint16_t last_mod_date;             // 最后修改日期
    uint32_t crc32;                     // CRC32校验
    uint32_t compressed_size;           // 压缩后大小
    uint32_t uncompressed_size;         // 未压缩大小
    uint16_t filename_length;           // 文件名长度
    uint16_t extra_field_length;        // 扩展字段长度
};

// ZIP中央目录文件头结构
struct ZipCentralHeader {
    uint32_t signature;                 // 中央目录签名 (0x02014b50)
    uint16_t version_made_by;           // 制作版本
    uint16_t version_needed;            // 解压所需版本
    uint16_t flags;                     // 通用位标志
    uint16_t compression_method;        // 压缩方法
    uint16_t last_mod_time;             // 最后修改时间
    uint16_t last_mod_date;             // 最后修改日期
    uint32_t crc32;                     // CRC32校验
    uint32_t compressed_size;           // 压缩后大小
    uint32_t uncompressed_size;         // 未压缩大小
    uint16_t filename_length;           // 文件名长度
    uint16_t extra_field_length;        // 扩展字段长度
    uint16_t file_comment_length;       // 文件注释长度
    uint16_t disk_number_start;         // 磁盘开始号
    uint16_t internal_file_attributes;  // 内部文件属性
    uint32_t external_file_attributes;  // 外部文件属性
    uint32_t local_header_offset;       // 本地文件头偏移
};

// ZIP文件结束记录结构
struct ZipEndRecord {
    uint32_t signature;                 // 结束记录签名 (0x06054b50)
    uint16_t disk_number;               // 当前磁盘号
    uint16_t central_dir_disk;          // 中央目录磁盘号
    uint16_t central_dir_entries;       // 中央目录条目数
    uint16_t total_entries;             // 总条目数
    uint32_t central_dir_size;          // 中央目录大小
    uint32_t central_dir_offset;        // 中央目录偏移
    uint16_t comment_length;            // 注释长度
};

// ZIP文件条目信息
struct ZipEntry {
    std::string filename;               // 文件名
    uint32_t crc32;                     // CRC32校验
    uint32_t compressed_size;           // 压缩后大小
    uint32_t uncompressed_size;         // 未压缩大小
    uint16_t compression_method;        // 压缩方法
    uint32_t offset;                    // 文件偏移
    std::vector<uint8_t> data;          // 文件数据
    bool is_directory;                  // 是否为目录
    std::chrono::system_clock::time_point modified_time; // 修改时间
};

// 压缩方法枚举
enum class CompressionMethod {
    STORED = 0,                         // 存储（不压缩）
    DEFLATE = 8,                        // 压缩
    UNKNOWN = 0xFFFF                    // 未知方法
};

// 简单ZIP库类
class SimpleZip {
public:
    SimpleZip();
    ~SimpleZip();
    
    // 创建ZIP文件
    bool createZip(const std::string& zip_path);
    bool addFile(const std::string& file_path, const std::string& internal_path);
    bool addDirectory(const std::string& dir_path, const std::string& internal_path);
    bool addData(const std::vector<uint8_t>& data, const std::string& internal_path);
    bool closeZip();
    
    // 读取ZIP文件
    bool openZip(const std::string& zip_path);
    std::vector<std::string> getFileList() const;
    bool extractFile(const std::string& internal_path, const std::string& output_path);
    std::vector<uint8_t> extractData(const std::string& internal_path);
    bool extractAll(const std::string& output_directory);
    void closeZip();
    
    // 压缩和解压
    std::vector<uint8_t> compressData(const std::vector<uint8_t>& data, int level = 6);
    std::vector<uint8_t> decompressData(const std::vector<uint8_t>& compressed_data);
    
    // 工具方法
    uint32_t calculateCRC32(const std::vector<uint8_t>& data);
    std::string getLastError() const;
    bool hasErrors() const;
    void clearErrors();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
    bool writeFileHeader(std::ofstream& file, const ZipFileHeader& header);
    bool writeCentralHeader(std::ofstream& file, const ZipCentralHeader& header);
    bool writeEndRecord(std::ofstream& file, const ZipEndRecord& record);
    bool readFileHeader(std::ifstream& file, ZipFileHeader& header);
    bool readCentralHeader(std::ifstream& file, ZipCentralHeader& header);
    bool readEndRecord(std::ifstream& file, ZipEndRecord& record);
    std::vector<uint8_t> deflateCompress(const std::vector<uint8_t>& data, int level);
    std::vector<uint8_t> deflateDecompress(const std::vector<uint8_t>& compressed_data);
    std::string normalizePath(const std::string& path);
    bool isValidZipFile(const std::string& zip_path);
    void addError(const std::string& error_message);
};

} // namespace chtl