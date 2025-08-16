#include "cmod/cmod_manager.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <cstring>
#include <chrono>
#include <iostream>

namespace chtl {

// ========== CRC32计算表 ==========
static const uint32_t CRC32_TABLE[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

// ========== SimpleZipCompressor 实现 ==========

SimpleZipCompressor::SimpleZipCompressor() {
}

SimpleZipCompressor::~SimpleZipCompressor() {
}

bool SimpleZipCompressor::addFile(const std::string& path, const std::vector<uint8_t>& data) {
    CmodEntry entry;
    entry.path = path;
    entry.data = data;
    entry.isDirectory = false;
    entry.size = data.size();
    entry.modifiedTime = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    
    entries_.push_back(std::move(entry));
    return true;
}

bool SimpleZipCompressor::addDirectory(const std::string& path) {
    CmodEntry entry;
    entry.path = path + (path.empty() || path.back() != '/' ? "/" : "");
    entry.isDirectory = true;
    entry.size = 0;
    entry.modifiedTime = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    
    entries_.push_back(std::move(entry));
    return true;
}

bool SimpleZipCompressor::writeToFile(const std::string& outputPath) {
    std::vector<uint8_t> output;
    std::vector<uint32_t> localHeaderOffsets;
    
    // 写入所有本地文件头和数据
    for (const auto& entry : entries_) {
        localHeaderOffsets.push_back(static_cast<uint32_t>(output.size()));
        
        uint32_t crc32 = entry.isDirectory ? 0 : calculateCRC32(entry.data);
        writeLocalFileHeader(output, entry, crc32);
        
        if (!entry.isDirectory) {
            output.insert(output.end(), entry.data.begin(), entry.data.end());
        }
    }
    
    // 记录中央目录开始位置
    uint32_t centralDirOffset = static_cast<uint32_t>(output.size());
    
    // 写入中央目录
    for (size_t i = 0; i < entries_.size(); ++i) {
        const auto& entry = entries_[i];
        uint32_t crc32 = entry.isDirectory ? 0 : calculateCRC32(entry.data);
        writeCentralDirectoryHeader(output, entry, crc32, localHeaderOffsets[i]);
    }
    
    // 记录中央目录大小
    uint32_t centralDirSize = static_cast<uint32_t>(output.size()) - centralDirOffset;
    
    // 写入中央目录结束记录
    writeEndOfCentralDirectory(output, centralDirSize, centralDirOffset, 
                              static_cast<uint16_t>(entries_.size()));
    
    // 写入文件
    std::ofstream file(outputPath, std::ios::binary);
    if (!file) {
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(output.data()), output.size());
    return file.good();
}

void SimpleZipCompressor::clear() {
    entries_.clear();
}

size_t SimpleZipCompressor::getEntryCount() const {
    return entries_.size();
}

uint32_t SimpleZipCompressor::calculateCRC32(const std::vector<uint8_t>& data) {
    uint32_t crc = 0xFFFFFFFF;
    for (uint8_t byte : data) {
        crc = CRC32_TABLE[(crc ^ byte) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFF;
}

void SimpleZipCompressor::writeLocalFileHeader(std::vector<uint8_t>& output, const CmodEntry& entry, uint32_t crc32) {
    // Local file header signature (0x04034b50)
    output.insert(output.end(), {0x50, 0x4b, 0x03, 0x04});
    
    // Version needed to extract (20)
    output.insert(output.end(), {20, 0});
    
    // General purpose bit flag (0)
    output.insert(output.end(), {0, 0});
    
    // Compression method (0 = stored)
    output.insert(output.end(), {0, 0});
    
    // Last mod file time and date (simplified)
    output.insert(output.end(), {0, 0, 0, 0});
    
    // CRC-32
    output.push_back(crc32 & 0xFF);
    output.push_back((crc32 >> 8) & 0xFF);
    output.push_back((crc32 >> 16) & 0xFF);
    output.push_back((crc32 >> 24) & 0xFF);
    
    // Compressed size
    uint32_t size = entry.isDirectory ? 0 : static_cast<uint32_t>(entry.size);
    output.push_back(size & 0xFF);
    output.push_back((size >> 8) & 0xFF);
    output.push_back((size >> 16) & 0xFF);
    output.push_back((size >> 24) & 0xFF);
    
    // Uncompressed size
    output.push_back(size & 0xFF);
    output.push_back((size >> 8) & 0xFF);
    output.push_back((size >> 16) & 0xFF);
    output.push_back((size >> 24) & 0xFF);
    
    // File name length
    uint16_t nameLen = static_cast<uint16_t>(entry.path.length());
    output.push_back(nameLen & 0xFF);
    output.push_back((nameLen >> 8) & 0xFF);
    
    // Extra field length (0)
    output.insert(output.end(), {0, 0});
    
    // File name
    output.insert(output.end(), entry.path.begin(), entry.path.end());
}

void SimpleZipCompressor::writeCentralDirectoryHeader(std::vector<uint8_t>& output, const CmodEntry& entry, 
                                                     uint32_t crc32, uint32_t localHeaderOffset) {
    // Central directory header signature (0x02014b50)
    output.insert(output.end(), {0x50, 0x4b, 0x01, 0x02});
    
    // Version made by (20)
    output.insert(output.end(), {20, 0});
    
    // Version needed to extract (20)
    output.insert(output.end(), {20, 0});
    
    // General purpose bit flag (0)
    output.insert(output.end(), {0, 0});
    
    // Compression method (0 = stored)
    output.insert(output.end(), {0, 0});
    
    // Last mod file time and date (simplified)
    output.insert(output.end(), {0, 0, 0, 0});
    
    // CRC-32
    output.push_back(crc32 & 0xFF);
    output.push_back((crc32 >> 8) & 0xFF);
    output.push_back((crc32 >> 16) & 0xFF);
    output.push_back((crc32 >> 24) & 0xFF);
    
    // Compressed size
    uint32_t size = entry.isDirectory ? 0 : static_cast<uint32_t>(entry.size);
    output.push_back(size & 0xFF);
    output.push_back((size >> 8) & 0xFF);
    output.push_back((size >> 16) & 0xFF);
    output.push_back((size >> 24) & 0xFF);
    
    // Uncompressed size
    output.push_back(size & 0xFF);
    output.push_back((size >> 8) & 0xFF);
    output.push_back((size >> 16) & 0xFF);
    output.push_back((size >> 24) & 0xFF);
    
    // File name length
    uint16_t nameLen = static_cast<uint16_t>(entry.path.length());
    output.push_back(nameLen & 0xFF);
    output.push_back((nameLen >> 8) & 0xFF);
    
    // Extra field length (0)
    output.insert(output.end(), {0, 0});
    
    // File comment length (0)
    output.insert(output.end(), {0, 0});
    
    // Disk number start (0)
    output.insert(output.end(), {0, 0});
    
    // Internal file attributes (0)
    output.insert(output.end(), {0, 0});
    
    // External file attributes (directory flag)
    uint32_t extAttr = entry.isDirectory ? 0x10 : 0x00;
    output.push_back(extAttr & 0xFF);
    output.push_back((extAttr >> 8) & 0xFF);
    output.push_back((extAttr >> 16) & 0xFF);
    output.push_back((extAttr >> 24) & 0xFF);
    
    // Relative offset of local header
    output.push_back(localHeaderOffset & 0xFF);
    output.push_back((localHeaderOffset >> 8) & 0xFF);
    output.push_back((localHeaderOffset >> 16) & 0xFF);
    output.push_back((localHeaderOffset >> 24) & 0xFF);
    
    // File name
    output.insert(output.end(), entry.path.begin(), entry.path.end());
}

void SimpleZipCompressor::writeEndOfCentralDirectory(std::vector<uint8_t>& output, uint32_t centralDirSize, 
                                                    uint32_t centralDirOffset, uint16_t entryCount) {
    // End of central directory signature (0x06054b50)
    output.insert(output.end(), {0x50, 0x4b, 0x05, 0x06});
    
    // Number of this disk (0)
    output.insert(output.end(), {0, 0});
    
    // Number of the disk with the start of the central directory (0)
    output.insert(output.end(), {0, 0});
    
    // Total number of entries in the central directory on this disk
    output.push_back(entryCount & 0xFF);
    output.push_back((entryCount >> 8) & 0xFF);
    
    // Total number of entries in the central directory
    output.push_back(entryCount & 0xFF);
    output.push_back((entryCount >> 8) & 0xFF);
    
    // Size of the central directory
    output.push_back(centralDirSize & 0xFF);
    output.push_back((centralDirSize >> 8) & 0xFF);
    output.push_back((centralDirSize >> 16) & 0xFF);
    output.push_back((centralDirSize >> 24) & 0xFF);
    
    // Offset of start of central directory
    output.push_back(centralDirOffset & 0xFF);
    output.push_back((centralDirOffset >> 8) & 0xFF);
    output.push_back((centralDirOffset >> 16) & 0xFF);
    output.push_back((centralDirOffset >> 24) & 0xFF);
    
    // ZIP file comment length (0)
    output.insert(output.end(), {0, 0});
}

// ========== SimpleZipDecompressor 实现 ==========

SimpleZipDecompressor::SimpleZipDecompressor() {
}

SimpleZipDecompressor::~SimpleZipDecompressor() {
}

bool SimpleZipDecompressor::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        return false;
    }
    
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    fileData_.resize(fileSize);
    file.read(reinterpret_cast<char*>(fileData_.data()), fileSize);
    
    if (!file.good()) {
        return false;
    }
    
    return parseZipFile();
}

bool SimpleZipDecompressor::extractAll(const std::string& outputDir) {
    std::filesystem::create_directories(outputDir);
    
    for (const auto& entry : entries_) {
        std::filesystem::path fullPath = std::filesystem::path(outputDir) / entry.path;
        
        if (entry.isDirectory) {
            std::filesystem::create_directories(fullPath);
        } else {
            std::filesystem::create_directories(fullPath.parent_path());
            
            std::ofstream outFile(fullPath, std::ios::binary);
            if (!outFile) {
                return false;
            }
            
            outFile.write(reinterpret_cast<const char*>(entry.data.data()), entry.data.size());
            if (!outFile.good()) {
                return false;
            }
        }
    }
    
    return true;
}

bool SimpleZipDecompressor::extractFile(const std::string& entryPath, const std::string& outputPath) {
    for (const auto& entry : entries_) {
        if (entry.path == entryPath && !entry.isDirectory) {
            std::filesystem::create_directories(std::filesystem::path(outputPath).parent_path());
            
            std::ofstream outFile(outputPath, std::ios::binary);
            if (!outFile) {
                return false;
            }
            
            outFile.write(reinterpret_cast<const char*>(entry.data.data()), entry.data.size());
            return outFile.good();
        }
    }
    
    return false;
}

const std::vector<CmodEntry>& SimpleZipDecompressor::getEntries() const {
    return entries_;
}

void SimpleZipDecompressor::clear() {
    entries_.clear();
    fileData_.clear();
}

bool SimpleZipDecompressor::parseZipFile() {
    if (fileData_.size() < 22) { // 最小ZIP文件大小
        return false;
    }
    
    uint32_t centralDirOffset;
    uint16_t entryCount;
    
    if (!findEndOfCentralDirectory(centralDirOffset, entryCount)) {
        return false;
    }
    
    return readCentralDirectory(centralDirOffset, entryCount);
}

bool SimpleZipDecompressor::findEndOfCentralDirectory(uint32_t& centralDirOffset, uint16_t& entryCount) {
    // 从文件末尾开始搜索End of Central Directory
    for (size_t i = fileData_.size() - 22; i < fileData_.size(); --i) {
        if (readUint32(i) == 0x06054b50) { // End of central directory signature
            entryCount = readUint16(i + 10); // Total number of entries
            centralDirOffset = readUint32(i + 16); // Offset of start of central directory
            return true;
        }
        if (i == 0) break;
    }
    
    return false;
}

bool SimpleZipDecompressor::readCentralDirectory(size_t offset, uint16_t entryCount) {
    entries_.clear();
    entries_.reserve(entryCount);
    
    for (uint16_t i = 0; i < entryCount; ++i) {
        if (offset + 46 > fileData_.size()) {
            return false;
        }
        
        if (readUint32(offset) != 0x02014b50) { // Central directory header signature
            return false;
        }
        
        CmodEntry entry;
        
        // 读取文件信息
        uint16_t nameLen = readUint16(offset + 28);
        uint16_t extraLen = readUint16(offset + 30);
        uint16_t commentLen = readUint16(offset + 32);
        uint32_t localHeaderOffset = readUint32(offset + 42);
        
        // 读取文件名
        if (offset + 46 + nameLen > fileData_.size()) {
            return false;
        }
        
        entry.path = std::string(reinterpret_cast<const char*>(&fileData_[offset + 46]), nameLen);
        entry.isDirectory = !entry.path.empty() && entry.path.back() == '/';
        
        // 读取文件数据
        if (!entry.isDirectory) {
            size_t dataOffset;
            if (!readLocalFileHeader(dataOffset = localHeaderOffset, entry)) {
                return false;
            }
        }
        
        entries_.push_back(std::move(entry));
        
        // 移动到下一个中央目录条目
        offset += 46 + nameLen + extraLen + commentLen;
    }
    
    return true;
}

bool SimpleZipDecompressor::readLocalFileHeader(size_t& offset, CmodEntry& entry) {
    if (offset + 30 > fileData_.size()) {
        return false;
    }
    
    if (readUint32(offset) != 0x04034b50) { // Local file header signature
        return false;
    }
    
    uint32_t compressedSize = readUint32(offset + 18);
    uint16_t nameLen = readUint16(offset + 26);
    uint16_t extraLen = readUint16(offset + 28);
    
    // 跳过文件头
    offset += 30 + nameLen + extraLen;
    
    if (offset + compressedSize > fileData_.size()) {
        return false;
    }
    
    // 读取文件数据 (假设没有压缩)
    entry.data.resize(compressedSize);
    std::memcpy(entry.data.data(), &fileData_[offset], compressedSize);
    entry.size = compressedSize;
    
    return true;
}

uint32_t SimpleZipDecompressor::readUint32(size_t offset) {
    if (offset + 4 > fileData_.size()) {
        return 0;
    }
    return static_cast<uint32_t>(fileData_[offset]) |
           (static_cast<uint32_t>(fileData_[offset + 1]) << 8) |
           (static_cast<uint32_t>(fileData_[offset + 2]) << 16) |
           (static_cast<uint32_t>(fileData_[offset + 3]) << 24);
}

uint16_t SimpleZipDecompressor::readUint16(size_t offset) {
    if (offset + 2 > fileData_.size()) {
        return 0;
    }
    return static_cast<uint16_t>(fileData_[offset]) |
           (static_cast<uint16_t>(fileData_[offset + 1]) << 8);
}

// ========== CmodManager 实现 ==========

CmodManager::CmodManager() 
    : verbose_(false), 
      compressor_(std::make_unique<SimpleZipCompressor>()),
      decompressor_(std::make_unique<SimpleZipDecompressor>()),
      totalPackedFiles_(0), totalPackedSize_(0),
      totalUnpackedFiles_(0), totalUnpackedSize_(0) {
}

CmodManager::~CmodManager() = default;

CmodPackResult CmodManager::packModule(const std::string& sourceDir, const std::string& outputPath) {
    CmodPackResult result;
    
    log("开始打包Cmod模块: " + sourceDir);
    
    // 验证模块结构
    auto validation = validateModuleStructure(sourceDir);
    if (!validation.isValid) {
        result.errorMessage = validation.errorMessage;
        return result;
    }
    
    log("模块结构验证通过");
    
    compressor_->clear();
    
    // 收集所有文件
    auto files = collectFiles(sourceDir, sourceDir);
    
    log("收集到 " + std::to_string(files.size()) + " 个文件");
    
    for (const auto& filePath : files) {
        std::string relativePath = getRelativePath(filePath, sourceDir);
        
        if (std::filesystem::is_directory(filePath)) {
            compressor_->addDirectory(relativePath);
            log("添加目录: " + relativePath);
        } else {
            std::ifstream file(filePath, std::ios::binary);
            if (!file) {
                result.errorMessage = "无法读取文件: " + filePath;
                return result;
            }
            
            std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)),
                                     std::istreambuf_iterator<char>());
            
            compressor_->addFile(relativePath, data);
            log("添加文件: " + relativePath + " (" + std::to_string(data.size()) + " bytes)");
            
            result.totalSize += data.size();
            result.totalFiles++;
        }
    }
    
    // 写入ZIP文件
    if (!compressor_->writeToFile(outputPath)) {
        result.errorMessage = "写入Cmod文件失败: " + outputPath;
        return result;
    }
    
    result.success = true;
    result.outputPath = outputPath;
    
    totalPackedFiles_ += result.totalFiles;
    totalPackedSize_ += result.totalSize;
    
    log("Cmod打包完成: " + outputPath);
    log("总文件数: " + std::to_string(result.totalFiles));
    log("总大小: " + std::to_string(result.totalSize) + " bytes");
    
    return result;
}

CmodValidationResult CmodManager::validateModuleStructure(const std::string& sourceDir) {
    CmodValidationResult result;
    
    if (!std::filesystem::exists(sourceDir) || !std::filesystem::is_directory(sourceDir)) {
        result.errorMessage = "源目录不存在或不是目录: " + sourceDir;
        return result;
    }
    
    std::string moduleName = std::filesystem::path(sourceDir).filename().string();
    if (!isValidModuleName(moduleName)) {
        result.errorMessage = "无效的模块名称: " + moduleName;
        return result;
    }
    
    // 验证src目录
    std::string srcDir = sourceDir + "/src";
    if (!validateSourceDirectory(srcDir, result)) {
        return result;
    }
    
    // 验证info目录
    std::string infoDir = sourceDir + "/info";
    if (!validateInfoDirectory(infoDir, result)) {
        return result;
    }
    
    result.isValid = true;
    return result;
}

CmodUnpackResult CmodManager::unpackModule(const std::string& cmodPath, const std::string& outputDir) {
    CmodUnpackResult result;
    
    log("开始解包Cmod模块: " + cmodPath);
    
    if (!std::filesystem::exists(cmodPath)) {
        result.errorMessage = "Cmod文件不存在: " + cmodPath;
        return result;
    }
    
    decompressor_->clear();
    
    if (!decompressor_->loadFromFile(cmodPath)) {
        result.errorMessage = "加载Cmod文件失败: " + cmodPath;
        return result;
    }
    
    log("Cmod文件加载成功");
    
    if (!decompressor_->extractAll(outputDir)) {
        result.errorMessage = "解压Cmod文件失败";
        return result;
    }
    
    // 收集解压的文件列表
    for (const auto& entry : decompressor_->getEntries()) {
        if (!entry.isDirectory) {
            result.extractedFiles.push_back(entry.path);
            totalUnpackedFiles_++;
            totalUnpackedSize_ += entry.size;
        }
    }
    
    // 读取模块信息
    std::string infoPath = outputDir + "/info";
    if (std::filesystem::exists(infoPath)) {
        for (const auto& entry : std::filesystem::directory_iterator(infoPath)) {
            if (entry.path().extension() == ".chtl") {
                result.moduleInfo = parseInfoFile(entry.path().string());
                break;
            }
        }
    }
    
    result.success = true;
    result.outputDir = outputDir;
    
    log("Cmod解包完成: " + outputDir);
    log("解压文件数: " + std::to_string(result.extractedFiles.size()));
    
    return result;
}

CmodInfo CmodManager::readModuleInfo(const std::string& cmodPath) {
    CmodInfo info;
    
    decompressor_->clear();
    
    if (!decompressor_->loadFromFile(cmodPath)) {
        return info;
    }
    
    // 查找info目录中的.chtl文件
    for (const auto& entry : decompressor_->getEntries()) {
        if (entry.path.find("info/") == 0 && 
            entry.path.find(".chtl") != std::string::npos &&
            !entry.isDirectory) {
            
            std::string content(entry.data.begin(), entry.data.end());
            info = parseInfoContent(content);
            break;
        }
    }
    
    return info;
}

CmodInfo CmodManager::parseInfoFile(const std::string& infoFilePath) {
    std::ifstream file(infoFilePath);
    if (!file) {
        return CmodInfo{};
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    return parseInfoContent(content);
}

CmodInfo CmodManager::parseInfoContent(const std::string& content) {
    CmodInfo info;
    
    // 简单的Info文件解析 (严格按照CHTL语法文档)
    std::regex infoBlockRegex(R"(\[Info\]\s*\{([^}]*)\})");
    std::smatch infoMatch;
    
    if (!std::regex_search(content, infoMatch, infoBlockRegex)) {
        return info;
    }
    
    std::string infoBlock = infoMatch[1].str();
    
    // 解析各个字段
    std::regex fieldRegex("(\\w+)\\s*[:=]\\s*\"([^\"]*)\"");
    std::sregex_iterator iter(infoBlock.begin(), infoBlock.end(), fieldRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::string key = (*iter)[1].str();
        std::string value = (*iter)[2].str();
        
        if (key == "name") {
            info.name = value;
        } else if (key == "version") {
            info.version = value;
        } else if (key == "description") {
            info.description = value;
        } else if (key == "author") {
            info.author = value;
        } else if (key == "license") {
            info.license = value;
        } else if (key == "dependencies") {
            info.dependencies = value;
        } else if (key == "category") {
            info.category = value;
        } else if (key == "minCHTLVersion") {
            info.minCHTLVersion = value;
        } else if (key == "homepage") {
            info.homepage = value;
        } else if (key == "repository") {
            info.repository = value;
        }
    }
    
    return info;
}

std::string CmodManager::generateInfoFile(const CmodInfo& info) {
    std::stringstream ss;
    
    ss << "// 模块信息文件\n";
    ss << "// 严格按照CHTL语法文档格式\n\n";
    ss << "[Info]\n{\n";
    ss << "    name = \"" << info.name << "\";\n";
    ss << "    version = \"" << info.version << "\";\n";
    ss << "    description = \"" << info.description << "\";\n";
    ss << "    author = \"" << info.author << "\";\n";
    ss << "    license = \"" << info.license << "\";\n";
    ss << "    dependencies = \"" << info.dependencies << "\";\n";
    ss << "    category = \"" << info.category << "\";\n";
    ss << "    minCHTLVersion = \"" << info.minCHTLVersion << "\";\n";
    
    if (!info.homepage.empty()) {
        ss << "    homepage = \"" << info.homepage << "\";\n";
    }
    if (!info.repository.empty()) {
        ss << "    repository = \"" << info.repository << "\";\n";
    }
    
    ss << "}\n";
    
    return ss.str();
}

std::vector<std::string> CmodManager::collectFiles(const std::string& directory, const std::string& baseDir) {
    std::vector<std::string> files;
    
    if (!std::filesystem::exists(directory)) {
        return files;
    }
    
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        files.push_back(entry.path().string());
    }
    
    return files;
}

bool CmodManager::isValidModuleName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 模块名只能包含字母、数字、下划线和点
    std::regex nameRegex(R"([a-zA-Z][a-zA-Z0-9_\.]*[a-zA-Z0-9_])");
    return std::regex_match(name, nameRegex);
}

bool CmodManager::isValidVersion(const std::string& version) {
    if (version.empty()) {
        return false;
    }
    
    // 简单的版本号验证 (major.minor.patch)
    std::regex versionRegex(R"(\d+\.\d+\.\d+)");
    return std::regex_match(version, versionRegex);
}

std::string CmodManager::getRelativePath(const std::string& filePath, const std::string& baseDir) {
    std::filesystem::path file(filePath);
    std::filesystem::path base(baseDir);
    
    try {
        return std::filesystem::relative(file, base).string();
    } catch (...) {
        return filePath;
    }
}

std::string CmodManager::getPackStatistics() const {
    std::stringstream ss;
    ss << "Cmod打包统计:\n";
    ss << "  总打包文件数: " << totalPackedFiles_ << "\n";
    ss << "  总打包大小: " << totalPackedSize_ << " bytes\n";
    ss << "  总解包文件数: " << totalUnpackedFiles_ << "\n";
    ss << "  总解包大小: " << totalUnpackedSize_ << " bytes\n";
    return ss.str();
}

void CmodManager::setVerbose(bool verbose) {
    verbose_ = verbose;
}

// 私有方法实现

bool CmodManager::validateModuleDirectory(const std::string& moduleDir, CmodValidationResult& result) {
    if (!std::filesystem::exists(moduleDir) || !std::filesystem::is_directory(moduleDir)) {
        result.errorMessage = "模块目录不存在: " + moduleDir;
        return false;
    }
    
    return true;
}

bool CmodManager::validateSourceDirectory(const std::string& srcDir, CmodValidationResult& result) {
    if (!std::filesystem::exists(srcDir) || !std::filesystem::is_directory(srcDir)) {
        result.errorMessage = "src目录不存在: " + srcDir;
        result.missingFiles.push_back("src/");
        return false;
    }
    
    // 检查是否有.chtl文件
    bool hasChtlFiles = false;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(srcDir)) {
        if (entry.path().extension() == ".chtl") {
            hasChtlFiles = true;
            break;
        }
    }
    
    if (!hasChtlFiles) {
        result.warnings.push_back("src目录中没有找到.chtl文件");
    }
    
    return true;
}

bool CmodManager::validateInfoDirectory(const std::string& infoDir, CmodValidationResult& result) {
    if (!std::filesystem::exists(infoDir) || !std::filesystem::is_directory(infoDir)) {
        result.errorMessage = "info目录不存在: " + infoDir;
        result.missingFiles.push_back("info/");
        return false;
    }
    
    // 检查是否有info文件
    bool hasInfoFile = false;
    for (const auto& entry : std::filesystem::directory_iterator(infoDir)) {
        if (entry.path().extension() == ".chtl") {
            hasInfoFile = true;
            
            // 解析info文件
            result.moduleInfo = parseInfoFile(entry.path().string());
            
            if (result.moduleInfo.name.empty()) {
                result.warnings.push_back("Info文件中缺少模块名称");
            }
            if (result.moduleInfo.version.empty()) {
                result.warnings.push_back("Info文件中缺少版本信息");
            }
            
            break;
        }
    }
    
    if (!hasInfoFile) {
        result.errorMessage = "info目录中没有找到.chtl文件";
        result.missingFiles.push_back("info/*.chtl");
        return false;
    }
    
    return true;
}

std::string CmodManager::normalizePath(const std::string& path) {
    std::string normalized = path;
    std::replace(normalized.begin(), normalized.end(), '\\', '/');
    return normalized;
}

void CmodManager::log(const std::string& message) const {
    if (verbose_) {
        std::cout << "[CmodManager] " << message << std::endl;
    }
}

} // namespace chtl