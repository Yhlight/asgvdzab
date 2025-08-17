#include "utils/simple_zip.hpp"
#include <iostream>
#include <algorithm>
#include <ctime>

namespace CHTL {
namespace Utils {

// =============================================================================
// CRC32实现
// =============================================================================

uint32_t CRC32::crcTable_[256];
bool CRC32::tableInitialized_ = false;

void CRC32::initTable() {
    if (tableInitialized_) return;
    
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
        crcTable_[i] = crc;
    }
    tableInitialized_ = true;
}

uint32_t CRC32::calculate(const void* data, size_t length) {
    initTable();
    
    uint32_t crc = 0xFFFFFFFF;
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    
    for (size_t i = 0; i < length; i++) {
        crc = crcTable_[(crc ^ bytes[i]) & 0xFF] ^ (crc >> 8);
    }
    
    return crc ^ 0xFFFFFFFF;
}

uint32_t CRC32::calculate(const std::vector<uint8_t>& data) {
    return calculate(data.data(), data.size());
}

uint32_t CRC32::calculate(const std::string& data) {
    return calculate(data.data(), data.size());
}

uint32_t CRC32::update(uint32_t crc, const void* data, size_t length) {
    initTable();
    
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    
    for (size_t i = 0; i < length; i++) {
        crc = crcTable_[(crc ^ bytes[i]) & 0xFF] ^ (crc >> 8);
    }
    
    return crc;
}

// =============================================================================
// ZipEntry实现
// =============================================================================

ZipEntry::ZipEntry(const std::string& name, const std::vector<uint8_t>& fileData)
    : filename(name), data(fileData) {
    calculateCrc32();
}

ZipEntry::ZipEntry(const std::string& name, const std::string& textData)
    : filename(name) {
    data.assign(textData.begin(), textData.end());
    calculateCrc32();
}

void ZipEntry::calculateCrc32() {
    crc32 = CRC32::calculate(data);
}

// =============================================================================
// ZIP结构体读写实现
// =============================================================================

void ZipFileHeader::write(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&signature), 4);
    out.write(reinterpret_cast<const char*>(&version), 2);
    out.write(reinterpret_cast<const char*>(&flags), 2);
    out.write(reinterpret_cast<const char*>(&compression), 2);
    out.write(reinterpret_cast<const char*>(&modTime), 2);
    out.write(reinterpret_cast<const char*>(&modDate), 2);
    out.write(reinterpret_cast<const char*>(&crc32), 4);
    out.write(reinterpret_cast<const char*>(&compressedSize), 4);
    out.write(reinterpret_cast<const char*>(&uncompressedSize), 4);
    out.write(reinterpret_cast<const char*>(&filenameLength), 2);
    out.write(reinterpret_cast<const char*>(&extraFieldLength), 2);
}

bool ZipFileHeader::read(std::istream& in) {
    in.read(reinterpret_cast<char*>(&signature), 4);
    in.read(reinterpret_cast<char*>(&version), 2);
    in.read(reinterpret_cast<char*>(&flags), 2);
    in.read(reinterpret_cast<char*>(&compression), 2);
    in.read(reinterpret_cast<char*>(&modTime), 2);
    in.read(reinterpret_cast<char*>(&modDate), 2);
    in.read(reinterpret_cast<char*>(&crc32), 4);
    in.read(reinterpret_cast<char*>(&compressedSize), 4);
    in.read(reinterpret_cast<char*>(&uncompressedSize), 4);
    in.read(reinterpret_cast<char*>(&filenameLength), 2);
    in.read(reinterpret_cast<char*>(&extraFieldLength), 2);
    
    return in.good() && signature == 0x04034b50;
}

void ZipCentralDirEntry::write(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&signature), 4);
    out.write(reinterpret_cast<const char*>(&versionMadeBy), 2);
    out.write(reinterpret_cast<const char*>(&versionNeeded), 2);
    out.write(reinterpret_cast<const char*>(&flags), 2);
    out.write(reinterpret_cast<const char*>(&compression), 2);
    out.write(reinterpret_cast<const char*>(&modTime), 2);
    out.write(reinterpret_cast<const char*>(&modDate), 2);
    out.write(reinterpret_cast<const char*>(&crc32), 4);
    out.write(reinterpret_cast<const char*>(&compressedSize), 4);
    out.write(reinterpret_cast<const char*>(&uncompressedSize), 4);
    out.write(reinterpret_cast<const char*>(&filenameLength), 2);
    out.write(reinterpret_cast<const char*>(&extraFieldLength), 2);
    out.write(reinterpret_cast<const char*>(&commentLength), 2);
    out.write(reinterpret_cast<const char*>(&diskNumberStart), 2);
    out.write(reinterpret_cast<const char*>(&internalFileAttribs), 2);
    out.write(reinterpret_cast<const char*>(&externalFileAttribs), 4);
    out.write(reinterpret_cast<const char*>(&localHeaderOffset), 4);
}

bool ZipCentralDirEntry::read(std::istream& in) {
    in.read(reinterpret_cast<char*>(&signature), 4);
    in.read(reinterpret_cast<char*>(&versionMadeBy), 2);
    in.read(reinterpret_cast<char*>(&versionNeeded), 2);
    in.read(reinterpret_cast<char*>(&flags), 2);
    in.read(reinterpret_cast<char*>(&compression), 2);
    in.read(reinterpret_cast<char*>(&modTime), 2);
    in.read(reinterpret_cast<char*>(&modDate), 2);
    in.read(reinterpret_cast<char*>(&crc32), 4);
    in.read(reinterpret_cast<char*>(&compressedSize), 4);
    in.read(reinterpret_cast<char*>(&uncompressedSize), 4);
    in.read(reinterpret_cast<char*>(&filenameLength), 2);
    in.read(reinterpret_cast<char*>(&extraFieldLength), 2);
    in.read(reinterpret_cast<char*>(&commentLength), 2);
    in.read(reinterpret_cast<char*>(&diskNumberStart), 2);
    in.read(reinterpret_cast<char*>(&internalFileAttribs), 2);
    in.read(reinterpret_cast<char*>(&externalFileAttribs), 4);
    in.read(reinterpret_cast<char*>(&localHeaderOffset), 4);
    
    return in.good() && signature == 0x02014b50;
}

void ZipEndOfCentralDir::write(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&signature), 4);
    out.write(reinterpret_cast<const char*>(&diskNumber), 2);
    out.write(reinterpret_cast<const char*>(&centralDirDisk), 2);
    out.write(reinterpret_cast<const char*>(&centralDirEntriesOnDisk), 2);
    out.write(reinterpret_cast<const char*>(&totalCentralDirEntries), 2);
    out.write(reinterpret_cast<const char*>(&centralDirSize), 4);
    out.write(reinterpret_cast<const char*>(&centralDirOffset), 4);
    out.write(reinterpret_cast<const char*>(&commentLength), 2);
}

bool ZipEndOfCentralDir::read(std::istream& in) {
    in.read(reinterpret_cast<char*>(&signature), 4);
    in.read(reinterpret_cast<char*>(&diskNumber), 2);
    in.read(reinterpret_cast<char*>(&centralDirDisk), 2);
    in.read(reinterpret_cast<char*>(&centralDirEntriesOnDisk), 2);
    in.read(reinterpret_cast<char*>(&totalCentralDirEntries), 2);
    in.read(reinterpret_cast<char*>(&centralDirSize), 4);
    in.read(reinterpret_cast<char*>(&centralDirOffset), 4);
    in.read(reinterpret_cast<char*>(&commentLength), 2);
    
    return in.good() && signature == 0x06054b50;
}

// =============================================================================
// SimpleZipWriter实现
// =============================================================================

SimpleZipWriter::SimpleZipWriter(const std::string& outputPath) : outputPath_(outputPath) {}

bool SimpleZipWriter::addFile(const std::string& filename, const std::vector<uint8_t>& data) {
    auto entry = std::make_unique<ZipEntry>(normalizePath(filename), data);
    entries_.push_back(std::move(entry));
    return true;
}

bool SimpleZipWriter::addFile(const std::string& filename, const std::string& textData) {
    auto entry = std::make_unique<ZipEntry>(normalizePath(filename), textData);
    entries_.push_back(std::move(entry));
    return true;
}

bool SimpleZipWriter::addFileFromDisk(const std::string& filename, const std::string& diskPath) {
    std::ifstream file(diskPath, std::ios::binary);
    if (!file) {
        return false;
    }
    
    // 读取文件内容
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> data(fileSize);
    file.read(reinterpret_cast<char*>(data.data()), fileSize);
    
    return addFile(filename, data);
}

bool SimpleZipWriter::addDirectory(const std::string& dirName) {
    std::string normalizedDir = normalizePath(dirName);
    if (!normalizedDir.empty() && normalizedDir.back() != '/') {
        normalizedDir += '/';
    }
    
    auto entry = std::make_unique<ZipEntry>();
    entry->filename = normalizedDir;
    entry->isDirectory = true;
    entry->crc32 = 0;
    entries_.push_back(std::move(entry));
    return true;
}

bool SimpleZipWriter::addDirectoryRecursive(const std::string& basePath, const std::string& zipPath) {
    std::filesystem::path baseDir(basePath);
    if (!std::filesystem::exists(baseDir) || !std::filesystem::is_directory(baseDir)) {
        return false;
    }
    
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(baseDir)) {
            std::filesystem::path relativePath = std::filesystem::relative(entry.path(), baseDir);
            std::string zipFilePath = zipPath.empty() ? relativePath.string() : zipPath + "/" + relativePath.string();
            
            if (entry.is_directory()) {
                addDirectory(zipFilePath);
            } else if (entry.is_regular_file()) {
                addFileFromDisk(zipFilePath, entry.path().string());
            }
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error adding directory recursively: " << e.what() << std::endl;
        return false;
    }
}

bool SimpleZipWriter::write() {
    std::ofstream out(outputPath_, std::ios::binary);
    if (!out) {
        return false;
    }
    
    std::vector<std::pair<ZipCentralDirEntry, std::string>> centralEntries;
    uint32_t currentOffset = 0;
    
    // 写入所有本地文件头和数据
    for (const auto& entry : entries_) {
        ZipCentralDirEntry centralEntry;
        centralEntry.localHeaderOffset = currentOffset;
        centralEntry.filenameLength = static_cast<uint16_t>(entry->filename.length());
        centralEntry.crc32 = entry->crc32;
        centralEntry.compressedSize = static_cast<uint32_t>(entry->data.size());
        centralEntry.uncompressedSize = static_cast<uint32_t>(entry->data.size());
        
        if (entry->isDirectory) {
            centralEntry.externalFileAttribs = 0x10; // 目录属性
        }
        
        writeLocalFileHeader(out, *entry, currentOffset);
        centralEntries.emplace_back(centralEntry, entry->filename);
    }
    
    // 记录中央目录的开始位置
    uint32_t centralDirOffset = currentOffset;
    
    // 写入中央目录
    writeCentralDirectory(out, centralEntries);
    
    // 写入中央目录结束记录
    ZipEndOfCentralDir endRecord;
    endRecord.centralDirEntriesOnDisk = static_cast<uint16_t>(centralEntries.size());
    endRecord.totalCentralDirEntries = static_cast<uint16_t>(centralEntries.size());
    endRecord.centralDirSize = currentOffset - centralDirOffset;
    endRecord.centralDirOffset = centralDirOffset;
    
    endRecord.write(out);
    
    return out.good();
}

void SimpleZipWriter::clear() {
    entries_.clear();
}

std::string SimpleZipWriter::normalizePath(const std::string& path) {
    std::string normalized = path;
    
    // 替换反斜杠为正斜杠
    std::replace(normalized.begin(), normalized.end(), '\\', '/');
    
    // 移除开头的斜杠
    if (!normalized.empty() && normalized[0] == '/') {
        normalized = normalized.substr(1);
    }
    
    return normalized;
}

void SimpleZipWriter::writeLocalFileHeader(std::ostream& out, const ZipEntry& entry, uint32_t& offset) {
    ZipFileHeader header;
    header.crc32 = entry.crc32;
    header.compressedSize = static_cast<uint32_t>(entry.data.size());
    header.uncompressedSize = static_cast<uint32_t>(entry.data.size());
    header.filenameLength = static_cast<uint16_t>(entry.filename.length());
    
    // 设置当前时间
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    
    header.modTime = static_cast<uint16_t>(
        (localTime->tm_hour << 11) | (localTime->tm_min << 5) | (localTime->tm_sec / 2));
    header.modDate = static_cast<uint16_t>(
        ((localTime->tm_year - 80) << 9) | ((localTime->tm_mon + 1) << 5) | localTime->tm_mday);
    
    offset += 30 + header.filenameLength + header.extraFieldLength + header.compressedSize;
    
    header.write(out);
    out.write(entry.filename.c_str(), entry.filename.length());
    out.write(reinterpret_cast<const char*>(entry.data.data()), entry.data.size());
}

void SimpleZipWriter::writeCentralDirectory(std::ostream& out, const std::vector<std::pair<ZipCentralDirEntry, std::string>>& centralEntries) {
    for (const auto& [entry, filename] : centralEntries) {
        entry.write(out);
        out.write(filename.c_str(), filename.length());
    }
}

// =============================================================================
// SimpleZipReader实现
// =============================================================================

SimpleZipReader::SimpleZipReader(const std::string& inputPath) : inputPath_(inputPath) {}

bool SimpleZipReader::load() {
    std::ifstream in(inputPath_, std::ios::binary);
    if (!in) {
        return false;
    }
    
    entries_.clear();
    loaded_ = readCentralDirectory(in);
    return loaded_;
}

const ZipEntry* SimpleZipReader::findEntry(const std::string& filename) const {
    for (const auto& entry : entries_) {
        if (entry->filename == filename) {
            return entry.get();
        }
    }
    return nullptr;
}

std::vector<uint8_t> SimpleZipReader::extractFile(const std::string& filename) const {
    const ZipEntry* entry = findEntry(filename);
    return entry ? entry->data : std::vector<uint8_t>{};
}

std::string SimpleZipReader::extractTextFile(const std::string& filename) const {
    auto data = extractFile(filename);
    return std::string(data.begin(), data.end());
}

bool SimpleZipReader::extractFile(const std::string& filename, const std::string& outputPath) const {
    auto data = extractFile(filename);
    if (data.empty()) {
        return false;
    }
    
    // 创建输出目录
    std::filesystem::path outPath(outputPath);
    if (outPath.has_parent_path()) {
        std::filesystem::create_directories(outPath.parent_path());
    }
    
    std::ofstream out(outputPath, std::ios::binary);
    if (!out) {
        return false;
    }
    
    out.write(reinterpret_cast<const char*>(data.data()), data.size());
    return out.good();
}

bool SimpleZipReader::extractAll(const std::string& outputDir) const {
    for (const auto& entry : entries_) {
        std::filesystem::path outputPath = std::filesystem::path(outputDir) / entry->filename;
        
        if (entry->isDirectory) {
            std::filesystem::create_directories(outputPath);
        } else {
            if (!extractFile(entry->filename, outputPath.string())) {
                return false;
            }
        }
    }
    return true;
}

std::vector<std::string> SimpleZipReader::listFiles() const {
    std::vector<std::string> filenames;
    for (const auto& entry : entries_) {
        filenames.push_back(entry->filename);
    }
    return filenames;
}

bool SimpleZipReader::readCentralDirectory(std::istream& in) {
    // 查找中央目录结束记录（从文件末尾向前搜索）
    in.seekg(0, std::ios::end);
    size_t fileSize = in.tellg();
    
    // 最多搜索64KB + EOCD大小
    size_t searchSize = std::min(fileSize, size_t(65536 + 22));
    in.seekg(fileSize - searchSize, std::ios::beg);
    
    std::vector<char> buffer(searchSize);
    in.read(buffer.data(), searchSize);
    
    // 查找EOCD签名
    size_t eocdPos = searchSize;
    for (size_t i = searchSize - 4; i > 0; --i) {
        if (buffer[i] == 'P' && buffer[i+1] == 'K' && buffer[i+2] == 0x05 && buffer[i+3] == 0x06) {
            eocdPos = i;
            break;
        }
    }
    
    if (eocdPos >= searchSize) {
        return false; // 未找到EOCD
    }
    
    // 读取EOCD
    in.seekg(fileSize - searchSize + eocdPos, std::ios::beg);
    ZipEndOfCentralDir eocd;
    if (!eocd.read(in)) {
        return false;
    }
    
    // 读取中央目录
    in.seekg(eocd.centralDirOffset, std::ios::beg);
    
    for (uint16_t i = 0; i < eocd.totalCentralDirEntries; ++i) {
        ZipCentralDirEntry centralEntry;
        if (!centralEntry.read(in)) {
            return false;
        }
        
        // 读取文件名
        std::string filename(centralEntry.filenameLength, '\0');
        in.read(&filename[0], centralEntry.filenameLength);
        
        // 跳过扩展字段和注释
        in.seekg(centralEntry.extraFieldLength + centralEntry.commentLength, std::ios::cur);
        
        // 读取文件数据
        auto data = readFileData(in, centralEntry);
        
        auto entry = std::make_unique<ZipEntry>();
        entry->filename = filename;
        entry->data = data;
        entry->crc32 = centralEntry.crc32;
        entry->isDirectory = filename.back() == '/';
        
        entries_.push_back(std::move(entry));
    }
    
    return true;
}

std::vector<uint8_t> SimpleZipReader::readFileData(std::istream& in, const ZipCentralDirEntry& entry) const {
    // 保存当前位置
    auto currentPos = in.tellg();
    
    // 跳转到本地文件头
    in.seekg(entry.localHeaderOffset, std::ios::beg);
    
    // 读取本地文件头
    ZipFileHeader localHeader;
    if (!localHeader.read(in)) {
        in.seekg(currentPos);
        return {};
    }
    
    // 跳过文件名和扩展字段
    in.seekg(localHeader.filenameLength + localHeader.extraFieldLength, std::ios::cur);
    
    // 读取文件数据
    std::vector<uint8_t> data(entry.uncompressedSize);
    in.read(reinterpret_cast<char*>(data.data()), entry.uncompressedSize);
    
    // 恢复位置
    in.seekg(currentPos);
    
    return data;
}

bool SimpleZipReader::createDirectories(const std::string& path) const {
    try {
        std::filesystem::create_directories(std::filesystem::path(path).parent_path());
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

// =============================================================================
// 工厂函数和便捷函数
// =============================================================================

std::unique_ptr<SimpleZipWriter> createZipWriter(const std::string& outputPath) {
    return std::make_unique<SimpleZipWriter>(outputPath);
}

std::unique_ptr<SimpleZipReader> createZipReader(const std::string& inputPath) {
    return std::make_unique<SimpleZipReader>(inputPath);
}

bool createZipFromDirectory(const std::string& sourceDir, const std::string& zipPath) {
    auto writer = createZipWriter(zipPath);
    if (!writer->addDirectoryRecursive(sourceDir)) {
        return false;
    }
    return writer->write();
}

bool extractZipToDirectory(const std::string& zipPath, const std::string& outputDir) {
    auto reader = createZipReader(zipPath);
    if (!reader->load()) {
        return false;
    }
    return reader->extractAll(outputDir);
}

} // namespace Utils
} // namespace CHTL