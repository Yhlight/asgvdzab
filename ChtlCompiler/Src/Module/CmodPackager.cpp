#include "Module/CmodPackager.h"
#include <fstream>
#include <filesystem>
#include <chrono>
#include <cstring>
#include <iostream>

namespace fs = std::filesystem;

namespace Chtl {

// CRC32表
static uint32_t crc32_table[256];
static bool crc32_table_initialized = false;

static void initCRC32Table() {
    if (crc32_table_initialized) return;
    
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (int j = 0; j < 8; j++) {
            c = (c & 1) ? (0xEDB88320 ^ (c >> 1)) : (c >> 1);
        }
        crc32_table[i] = c;
    }
    crc32_table_initialized = true;
}

CmodPackager::CmodPackager() 
    : useCompression_(false),
      compressionLevel_(6) {
    initCRC32Table();
}

CmodPackager::~CmodPackager() {
}

bool CmodPackager::packDirectory(const std::string& sourceDir, const std::string& outputFile) {
    CmodMetadata metadata;
    metadata.name = fs::path(sourceDir).filename().string();
    metadata.version = "1.0.0";
    metadata.author = "Unknown";
    metadata.description = "CMOD package";
    
    return packWithMetadata(sourceDir, outputFile, metadata);
}

bool CmodPackager::packWithMetadata(const std::string& sourceDir, const std::string& outputFile, 
                                   const CmodMetadata& metadata) {
    // 验证源目录结构
    if (!validateCmodStructure(sourceDir)) {
        return false;
    }
    
    // 收集所有文件
    std::vector<std::string> files;
    if (!collectFiles(sourceDir, files)) {
        return false;
    }
    
    // 创建输出文件
    std::ofstream out(outputFile, std::ios::binary);
    if (!out.is_open()) {
        lastError_ = "Cannot create output file: " + outputFile;
        return false;
    }
    
    // 准备头信息
    CmodHeader header;
    std::memset(&header, 0, sizeof(header));
    std::memcpy(header.magic, "CMOD", 4);
    header.version = 1;
    header.fileCount = static_cast<uint32_t>(files.size());
    header.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    
    // 复制元数据到头信息
    std::strncpy(header.moduleName, metadata.name.c_str(), 
                 std::min(metadata.name.length(), sizeof(header.moduleName) - 1));
    std::strncpy(header.author, metadata.author.c_str(), 
                 std::min(metadata.author.length(), sizeof(header.author) - 1));
    std::strncpy(header.description, metadata.description.c_str(), 
                 std::min(metadata.description.length(), sizeof(header.description) - 1));
    
    // 写入头信息（稍后更新）
    auto headerPos = out.tellp();
    if (!writeHeader(out, header)) {
        return false;
    }
    
    // 写入文件列表
    std::vector<CmodFileEntry> entries;
    auto fileListPos = out.tellp();
    
    for (const auto& file : files) {
        CmodFileEntry entry;
        entry.path = getRelativePath(sourceDir, file);
        entries.push_back(entry);
        
        // 预留空间
        out.write(reinterpret_cast<const char*>(&entry), sizeof(uint32_t) * 5);
        uint32_t pathLen = static_cast<uint32_t>(entry.path.length());
        out.write(reinterpret_cast<const char*>(&pathLen), sizeof(pathLen));
        out.write(entry.path.c_str(), pathLen);
    }
    
    // 写入文件内容
    for (size_t i = 0; i < files.size(); ++i) {
        entries[i].offset = static_cast<uint32_t>(out.tellp());
        if (!writeFileContent(out, files[i], entries[i])) {
            return false;
        }
    }
    
    // 更新文件总大小
    header.totalSize = static_cast<uint32_t>(out.tellp());
    
    // 回写更新的头信息
    out.seekp(headerPos);
    if (!writeHeader(out, header)) {
        return false;
    }
    
    // 回写更新的文件列表
    out.seekp(fileListPos);
    for (const auto& entry : entries) {
        out.write(reinterpret_cast<const char*>(&entry.offset), sizeof(entry.offset));
        out.write(reinterpret_cast<const char*>(&entry.size), sizeof(entry.size));
        out.write(reinterpret_cast<const char*>(&entry.compressedSize), sizeof(entry.compressedSize));
        out.write(reinterpret_cast<const char*>(&entry.checksum), sizeof(entry.checksum));
        uint8_t compressed = entry.isCompressed ? 1 : 0;
        out.write(reinterpret_cast<const char*>(&compressed), sizeof(compressed));
        
        uint32_t pathLen = static_cast<uint32_t>(entry.path.length());
        out.write(reinterpret_cast<const char*>(&pathLen), sizeof(pathLen));
        out.write(entry.path.c_str(), pathLen);
    }
    
    out.close();
    return true;
}

bool CmodPackager::unpack(const std::string& cmodFile, const std::string& outputDir) {
    std::ifstream in(cmodFile, std::ios::binary);
    if (!in.is_open()) {
        lastError_ = "Cannot open CMOD file: " + cmodFile;
        return false;
    }
    
    // 读取头信息
    CmodHeader header;
    if (!in.read(reinterpret_cast<char*>(&header), sizeof(header))) {
        lastError_ = "Cannot read CMOD header";
        return false;
    }
    
    // 验证魔术字
    if (std::memcmp(header.magic, "CMOD", 4) != 0) {
        lastError_ = "Invalid CMOD file format";
        return false;
    }
    
    // 创建输出目录
    fs::create_directories(outputDir);
    
    // 读取文件列表
    std::vector<CmodFileEntry> entries;
    for (uint32_t i = 0; i < header.fileCount; ++i) {
        CmodFileEntry entry;
        
        in.read(reinterpret_cast<char*>(&entry.offset), sizeof(entry.offset));
        in.read(reinterpret_cast<char*>(&entry.size), sizeof(entry.size));
        in.read(reinterpret_cast<char*>(&entry.compressedSize), sizeof(entry.compressedSize));
        in.read(reinterpret_cast<char*>(&entry.checksum), sizeof(entry.checksum));
        
        uint8_t compressed;
        in.read(reinterpret_cast<char*>(&compressed), sizeof(compressed));
        entry.isCompressed = compressed != 0;
        
        uint32_t pathLen;
        in.read(reinterpret_cast<char*>(&pathLen), sizeof(pathLen));
        
        entry.path.resize(pathLen);
        in.read(&entry.path[0], pathLen);
        
        entries.push_back(entry);
    }
    
    // 解压文件
    for (const auto& entry : entries) {
        // 保存当前位置
        auto currentPos = in.tellg();
        
        // 跳转到文件内容
        in.seekg(entry.offset);
        
        // 读取文件内容
        std::vector<uint8_t> content(entry.isCompressed ? entry.compressedSize : entry.size);
        in.read(reinterpret_cast<char*>(content.data()), content.size());
        
        // 解压（如果需要）
        if (entry.isCompressed) {
            std::vector<uint8_t> decompressed;
            if (!decompressData(content, decompressed)) {
                lastError_ = "Failed to decompress file: " + entry.path;
                return false;
            }
            content = std::move(decompressed);
        }
        
        // 验证校验和
        uint32_t checksum = calculateCRC32(content);
        if (checksum != entry.checksum) {
            lastError_ = "Checksum mismatch for file: " + entry.path;
            return false;
        }
        
        // 创建输出文件
        std::string outputPath = fs::path(outputDir) / entry.path;
        createDirectories(fs::path(outputPath).parent_path().string());
        
        std::ofstream outFile(outputPath, std::ios::binary);
        if (!outFile.is_open()) {
            lastError_ = "Cannot create output file: " + outputPath;
            return false;
        }
        
        outFile.write(reinterpret_cast<const char*>(content.data()), content.size());
        outFile.close();
        
        // 恢复位置
        in.seekg(currentPos);
    }
    
    in.close();
    return true;
}

bool CmodPackager::unpackToMemory(const std::string& cmodFile) {
    memoryCache_.clear();
    
    std::ifstream in(cmodFile, std::ios::binary);
    if (!in.is_open()) {
        lastError_ = "Cannot open CMOD file: " + cmodFile;
        return false;
    }
    
    // 读取头信息
    CmodHeader header;
    if (!in.read(reinterpret_cast<char*>(&header), sizeof(header))) {
        lastError_ = "Cannot read CMOD header";
        return false;
    }
    
    // 验证魔术字
    if (std::memcmp(header.magic, "CMOD", 4) != 0) {
        lastError_ = "Invalid CMOD file format";
        return false;
    }
    
    // 读取文件列表并解压到内存
    for (uint32_t i = 0; i < header.fileCount; ++i) {
        CmodFileEntry entry;
        
        in.read(reinterpret_cast<char*>(&entry.offset), sizeof(entry.offset));
        in.read(reinterpret_cast<char*>(&entry.size), sizeof(entry.size));
        in.read(reinterpret_cast<char*>(&entry.compressedSize), sizeof(entry.compressedSize));
        in.read(reinterpret_cast<char*>(&entry.checksum), sizeof(entry.checksum));
        
        uint8_t compressed;
        in.read(reinterpret_cast<char*>(&compressed), sizeof(compressed));
        entry.isCompressed = compressed != 0;
        
        uint32_t pathLen;
        in.read(reinterpret_cast<char*>(&pathLen), sizeof(pathLen));
        
        entry.path.resize(pathLen);
        in.read(&entry.path[0], pathLen);
        
        // 保存当前位置
        auto currentPos = in.tellg();
        
        // 读取文件内容
        in.seekg(entry.offset);
        std::vector<uint8_t> content(entry.isCompressed ? entry.compressedSize : entry.size);
        in.read(reinterpret_cast<char*>(content.data()), content.size());
        
        // 解压（如果需要）
        if (entry.isCompressed) {
            std::vector<uint8_t> decompressed;
            if (!decompressData(content, decompressed)) {
                lastError_ = "Failed to decompress file: " + entry.path;
                return false;
            }
            content = std::move(decompressed);
        }
        
        // 存储到内存缓存
        memoryCache_[entry.path] = std::move(content);
        
        // 恢复位置
        in.seekg(currentPos);
    }
    
    in.close();
    return true;
}

bool CmodPackager::validateCmodStructure(const std::string& sourceDir) {
    if (!fs::exists(sourceDir) || !fs::is_directory(sourceDir)) {
        lastError_ = "Source directory does not exist: " + sourceDir;
        return false;
    }
    
    // 检查必需的目录结构
    fs::path srcPath = fs::path(sourceDir) / "src";
    if (!fs::exists(srcPath) || !fs::is_directory(srcPath)) {
        lastError_ = "Missing required 'src' directory";
        return false;
    }
    
    // 检查是否有.chtl文件
    bool hasChtlFile = false;
    for (const auto& entry : fs::recursive_directory_iterator(srcPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".chtl") {
            hasChtlFile = true;
            break;
        }
    }
    
    if (!hasChtlFile) {
        lastError_ = "No .chtl files found in src directory";
        return false;
    }
    
    return true;
}

bool CmodPackager::collectFiles(const std::string& sourceDir, std::vector<std::string>& files) {
    files.clear();
    
    try {
        for (const auto& entry : fs::recursive_directory_iterator(sourceDir)) {
            if (entry.is_regular_file()) {
                // 跳过某些文件
                std::string filename = entry.path().filename().string();
                if (filename[0] == '.' || filename == "Thumbs.db") {
                    continue;
                }
                
                files.push_back(entry.path().string());
            }
        }
    } catch (const std::exception& e) {
        lastError_ = "Error collecting files: " + std::string(e.what());
        return false;
    }
    
    return !files.empty();
}

bool CmodPackager::writeHeader(std::ofstream& out, const CmodHeader& header) {
    out.write(reinterpret_cast<const char*>(&header), sizeof(header));
    return out.good();
}

bool CmodPackager::writeFileContent(std::ofstream& out, const std::string& filePath, 
                                   CmodFileEntry& entry) {
    // 读取文件内容
    std::ifstream in(filePath, std::ios::binary);
    if (!in.is_open()) {
        lastError_ = "Cannot open source file: " + filePath;
        return false;
    }
    
    in.seekg(0, std::ios::end);
    size_t fileSize = in.tellg();
    in.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> content(fileSize);
    in.read(reinterpret_cast<char*>(content.data()), fileSize);
    in.close();
    
    // 计算校验和
    entry.checksum = calculateCRC32(content);
    entry.size = static_cast<uint32_t>(fileSize);
    
    // 压缩（如果启用）
    if (useCompression_ && fileSize > 1024) {  // 只压缩大于1KB的文件
        std::vector<uint8_t> compressed;
        if (compressData(content, compressed) && compressed.size() < content.size()) {
            entry.isCompressed = true;
            entry.compressedSize = static_cast<uint32_t>(compressed.size());
            out.write(reinterpret_cast<const char*>(compressed.data()), compressed.size());
        } else {
            entry.isCompressed = false;
            entry.compressedSize = entry.size;
            out.write(reinterpret_cast<const char*>(content.data()), content.size());
        }
    } else {
        entry.isCompressed = false;
        entry.compressedSize = entry.size;
        out.write(reinterpret_cast<const char*>(content.data()), content.size());
    }
    
    return out.good();
}

uint32_t CmodPackager::calculateCRC32(const std::vector<uint8_t>& data) {
    uint32_t crc = 0xFFFFFFFF;
    
    for (uint8_t byte : data) {
        crc = crc32_table[(crc ^ byte) & 0xFF] ^ (crc >> 8);
    }
    
    return crc ^ 0xFFFFFFFF;
}

bool CmodPackager::compressData(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    // 简单的RLE压缩实现
    output.clear();
    
    if (input.empty()) return true;
    
    size_t i = 0;
    while (i < input.size()) {
        uint8_t value = input[i];
        size_t count = 1;
        
        // 计算连续相同字节的数量
        while (i + count < input.size() && input[i + count] == value && count < 255) {
            count++;
        }
        
        if (count >= 3) {
            // 使用RLE编码：0xFF + count + value
            output.push_back(0xFF);
            output.push_back(static_cast<uint8_t>(count));
            output.push_back(value);
            i += count;
        } else {
            // 直接存储
            output.push_back(value);
            i++;
        }
    }
    
    return true;
}

bool CmodPackager::decompressData(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    // 简单的RLE解压实现
    output.clear();
    
    size_t i = 0;
    while (i < input.size()) {
        if (input[i] == 0xFF && i + 2 < input.size()) {
            // RLE编码
            uint8_t count = input[i + 1];
            uint8_t value = input[i + 2];
            
            for (size_t j = 0; j < count; j++) {
                output.push_back(value);
            }
            
            i += 3;
        } else {
            // 直接复制
            output.push_back(input[i]);
            i++;
        }
    }
    
    return true;
}

std::string CmodPackager::getRelativePath(const std::string& basePath, const std::string& fullPath) {
    return fs::relative(fullPath, basePath).string();
}

void CmodPackager::createDirectories(const std::string& path) {
    fs::create_directories(path);
}

bool CmodPackager::readHeader(const std::string& cmodFile, CmodHeader& header) {
    std::ifstream in(cmodFile, std::ios::binary);
    if (!in.is_open()) {
        lastError_ = "Cannot open CMOD file: " + cmodFile;
        return false;
    }
    
    if (!in.read(reinterpret_cast<char*>(&header), sizeof(header))) {
        lastError_ = "Cannot read CMOD header";
        return false;
    }
    
    // 验证魔术字
    if (std::memcmp(header.magic, "CMOD", 4) != 0) {
        lastError_ = "Invalid CMOD file format";
        return false;
    }
    
    return true;
}

bool CmodPackager::verify(const std::string& cmodFile) {
    CmodHeader header;
    if (!readHeader(cmodFile, header)) {
        return false;
    }
    
    // TODO: 实现更详细的验证
    return true;
}

} // namespace Chtl