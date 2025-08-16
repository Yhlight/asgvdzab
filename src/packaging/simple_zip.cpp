#include "packaging/simple_zip.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cstring>

namespace chtl {

// CRC32查找表
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

// ZIP文件签名
static const uint32_t LOCAL_FILE_HEADER_SIGNATURE = 0x04034b50;
static const uint32_t CENTRAL_DIRECTORY_SIGNATURE = 0x02014b50;
static const uint32_t END_OF_CENTRAL_DIR_SIGNATURE = 0x06054b50;

// ========== SimpleZipCreator 实现 ==========

SimpleZipCreator::SimpleZipCreator() {
}

SimpleZipCreator::~SimpleZipCreator() = default;

bool SimpleZipCreator::addFile(const std::string& fileName, const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    // 读取文件内容
    std::vector<uint8_t> data;
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    data.resize(fileSize);
    file.read(reinterpret_cast<char*>(data.data()), fileSize);
    file.close();
    
    return addData(fileName, data);
}

bool SimpleZipCreator::addData(const std::string& fileName, const std::vector<uint8_t>& data) {
    ZipEntry entry;
    entry.fileName = fileName;
    entry.data = data;
    entry.uncompressedSize = data.size();
    entry.crc32 = calculateCRC32(data);
    entry.modificationTime = std::time(nullptr);
    entry.isDirectory = false;
    entry.compressionMethod = 0; // 无压缩 (简化实现)
    entry.compressedSize = entry.uncompressedSize;
    
    entries_.push_back(entry);
    return true;
}

bool SimpleZipCreator::addText(const std::string& fileName, const std::string& text) {
    std::vector<uint8_t> data(text.begin(), text.end());
    return addData(fileName, data);
}

bool SimpleZipCreator::addDirectory(const std::string& dirName) {
    ZipEntry entry;
    entry.fileName = dirName + "/";
    entry.uncompressedSize = 0;
    entry.compressedSize = 0;
    entry.crc32 = 0;
    entry.modificationTime = std::time(nullptr);
    entry.isDirectory = true;
    entry.compressionMethod = 0;
    
    entries_.push_back(entry);
    return true;
}

bool SimpleZipCreator::addDirectoryRecursive(const std::string& dirPath, const std::string& zipPrefix) {
    if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath)) {
        return false;
    }
    
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath)) {
            std::string relativePath = std::filesystem::relative(entry.path(), dirPath).string();
            std::string zipPath = zipPrefix.empty() ? relativePath : zipPrefix + "/" + relativePath;
            
            if (entry.is_directory()) {
                addDirectory(zipPath);
            } else if (entry.is_regular_file()) {
                addFile(zipPath, entry.path().string());
            }
        }
        return true;
    } catch (const std::filesystem::filesystem_error&) {
        return false;
    }
}

bool SimpleZipCreator::save(const std::string& zipPath) {
    std::ofstream file(zipPath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    std::vector<uint32_t> localHeaderOffsets;
    
    // 写入本地文件头和数据
    for (const auto& entry : entries_) {
        localHeaderOffsets.push_back(file.tellp());
        writeLocalFileHeader(file, entry);
        
        // 写入文件数据
        if (!entry.isDirectory) {
            file.write(reinterpret_cast<const char*>(entry.data.data()), entry.data.size());
        }
    }
    
    // 记录中央目录起始位置
    uint32_t centralDirOffset = file.tellp();
    
    // 写入中央目录
    for (size_t i = 0; i < entries_.size(); ++i) {
        writeCentralDirectoryEntry(file, entries_[i], localHeaderOffsets[i]);
    }
    
    // 记录中央目录大小
    uint32_t centralDirSize = file.tellp() - centralDirOffset;
    
    // 写入中央目录结尾记录
    writeEndOfCentralDirectory(file, centralDirOffset, centralDirSize);
    
    file.close();
    return true;
}

void SimpleZipCreator::clear() {
    entries_.clear();
}

uint32_t SimpleZipCreator::calculateCRC32(const std::vector<uint8_t>& data) {
    uint32_t crc = 0xFFFFFFFF;
    for (uint8_t byte : data) {
        crc = CRC32_TABLE[(crc ^ byte) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFF;
}

size_t SimpleZipCreator::writeLocalFileHeader(std::ofstream& file, const ZipEntry& entry) {
    size_t startPos = file.tellp();
    
    writeUint32LE(file, LOCAL_FILE_HEADER_SIGNATURE);
    writeUint16LE(file, 20);  // 版本
    writeUint16LE(file, 0);   // 通用位标志
    writeUint16LE(file, entry.compressionMethod);
    writeUint32LE(file, toDosTime(entry.modificationTime));
    writeUint32LE(file, entry.crc32);
    writeUint32LE(file, entry.compressedSize);
    writeUint32LE(file, entry.uncompressedSize);
    writeUint16LE(file, entry.fileName.length());
    writeUint16LE(file, 0);   // 额外字段长度
    
    // 写入文件名
    file.write(entry.fileName.c_str(), entry.fileName.length());
    
    return file.tellp() - startPos;
}

size_t SimpleZipCreator::writeCentralDirectoryEntry(std::ofstream& file, const ZipEntry& entry, uint32_t localHeaderOffset) {
    size_t startPos = file.tellp();
    
    writeUint32LE(file, CENTRAL_DIRECTORY_SIGNATURE);
    writeUint16LE(file, 20);  // 制作版本
    writeUint16LE(file, 20);  // 提取版本
    writeUint16LE(file, 0);   // 通用位标志
    writeUint16LE(file, entry.compressionMethod);
    writeUint32LE(file, toDosTime(entry.modificationTime));
    writeUint32LE(file, entry.crc32);
    writeUint32LE(file, entry.compressedSize);
    writeUint32LE(file, entry.uncompressedSize);
    writeUint16LE(file, entry.fileName.length());
    writeUint16LE(file, 0);   // 额外字段长度
    writeUint16LE(file, 0);   // 文件注释长度
    writeUint16LE(file, 0);   // 磁盘号
    writeUint16LE(file, 0);   // 内部文件属性
    writeUint32LE(file, 0);   // 外部文件属性
    writeUint32LE(file, localHeaderOffset);
    
    // 写入文件名
    file.write(entry.fileName.c_str(), entry.fileName.length());
    
    return file.tellp() - startPos;
}

size_t SimpleZipCreator::writeEndOfCentralDirectory(std::ofstream& file, uint32_t centralDirOffset, uint32_t centralDirSize) {
    size_t startPos = file.tellp();
    
    writeUint32LE(file, END_OF_CENTRAL_DIR_SIGNATURE);
    writeUint16LE(file, 0);   // 磁盘号
    writeUint16LE(file, 0);   // 中央目录开始磁盘号
    writeUint16LE(file, entries_.size());  // 当前磁盘上的中央目录记录数
    writeUint16LE(file, entries_.size());  // 中央目录记录总数
    writeUint32LE(file, centralDirSize);
    writeUint32LE(file, centralDirOffset);
    writeUint16LE(file, 0);   // 注释长度
    
    return file.tellp() - startPos;
}

uint32_t SimpleZipCreator::toDosTime(std::time_t time) {
    struct tm* timeinfo = std::localtime(&time);
    
    uint16_t dosDate = ((timeinfo->tm_year - 80) << 9) |
                       ((timeinfo->tm_mon + 1) << 5) |
                       timeinfo->tm_mday;
    
    uint16_t dosTime = (timeinfo->tm_hour << 11) |
                       (timeinfo->tm_min << 5) |
                       (timeinfo->tm_sec / 2);
    
    return (dosDate << 16) | dosTime;
}

void SimpleZipCreator::writeUint16LE(std::ofstream& file, uint16_t value) {
    uint8_t bytes[2] = {
        static_cast<uint8_t>(value & 0xFF),
        static_cast<uint8_t>((value >> 8) & 0xFF)
    };
    file.write(reinterpret_cast<const char*>(bytes), 2);
}

void SimpleZipCreator::writeUint32LE(std::ofstream& file, uint32_t value) {
    uint8_t bytes[4] = {
        static_cast<uint8_t>(value & 0xFF),
        static_cast<uint8_t>((value >> 8) & 0xFF),
        static_cast<uint8_t>((value >> 16) & 0xFF),
        static_cast<uint8_t>((value >> 24) & 0xFF)
    };
    file.write(reinterpret_cast<const char*>(bytes), 4);
}

// ========== SimpleZipExtractor 实现 ==========

SimpleZipExtractor::SimpleZipExtractor() : isOpen_(false) {
}

SimpleZipExtractor::~SimpleZipExtractor() {
    close();
}

bool SimpleZipExtractor::open(const std::string& zipPath) {
    close();
    
    zipPath_ = zipPath;
    if (!std::filesystem::exists(zipPath)) {
        return false;
    }
    
    if (!readCentralDirectory()) {
        return false;
    }
    
    isOpen_ = true;
    return true;
}

void SimpleZipExtractor::close() {
    entries_.clear();
    zipPath_.clear();
    isOpen_ = false;
}

std::vector<std::string> SimpleZipExtractor::getFileList() const {
    std::vector<std::string> result;
    for (const auto& [name, entry] : entries_) {
        if (!entry.isDirectory) {
            result.push_back(name);
        }
    }
    return result;
}

bool SimpleZipExtractor::extractFile(const std::string& fileName, const std::string& outputPath) {
    auto data = extractData(fileName);
    if (data.empty()) {
        return false;
    }
    
    std::ofstream file(outputPath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    file.close();
    return true;
}

std::vector<uint8_t> SimpleZipExtractor::extractData(const std::string& fileName) {
    if (!isOpen_ || entries_.find(fileName) == entries_.end()) {
        return {};
    }
    
    const ZipEntry& entry = entries_[fileName];
    if (entry.isDirectory) {
        return {};
    }
    
    std::ifstream file(zipPath_, std::ios::binary);
    if (!file.is_open()) {
        return {};
    }
    
    // 跳过本地文件头
    file.seekg(30 + fileName.length(), std::ios::beg); // 简化：假设无额外字段
    
    std::vector<uint8_t> data(entry.uncompressedSize);
    file.read(reinterpret_cast<char*>(data.data()), entry.uncompressedSize);
    file.close();
    
    return data;
}

std::string SimpleZipExtractor::extractText(const std::string& fileName) {
    auto data = extractData(fileName);
    if (data.empty()) {
        return "";
    }
    
    return std::string(data.begin(), data.end());
}

bool SimpleZipExtractor::extractAll(const std::string& outputDir) {
    if (!isOpen_) {
        return false;
    }
    
    std::filesystem::create_directories(outputDir);
    
    for (const auto& [name, entry] : entries_) {
        std::filesystem::path outputPath = std::filesystem::path(outputDir) / name;
        
        if (entry.isDirectory) {
            std::filesystem::create_directories(outputPath);
        } else {
            std::filesystem::create_directories(outputPath.parent_path());
            if (!extractFile(name, outputPath.string())) {
                return false;
            }
        }
    }
    
    return true;
}

bool SimpleZipExtractor::hasFile(const std::string& fileName) const {
    return entries_.find(fileName) != entries_.end() && !entries_.at(fileName).isDirectory;
}

const ZipEntry* SimpleZipExtractor::getFileInfo(const std::string& fileName) const {
    auto it = entries_.find(fileName);
    return (it != entries_.end()) ? &it->second : nullptr;
}

bool SimpleZipExtractor::readCentralDirectory() {
    std::ifstream file(zipPath_, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    // 查找中央目录结尾记录
    long eocdOffset = findEndOfCentralDirectory(file);
    if (eocdOffset == -1) {
        return false;
    }
    
    file.seekg(eocdOffset + 16); // 跳到中央目录偏移位置
    uint32_t centralDirOffset = readUint32LE(file);
    
    file.seekg(eocdOffset + 10); // 跳到记录数位置
    uint16_t numEntries = readUint16LE(file);
    
    // 读取中央目录条目
    file.seekg(centralDirOffset);
    for (uint16_t i = 0; i < numEntries; ++i) {
        uint32_t signature = readUint32LE(file);
        if (signature != CENTRAL_DIRECTORY_SIGNATURE) {
            return false;
        }
        
        ZipEntry entry;
        
        file.seekg(4, std::ios::cur); // 跳过版本
        file.seekg(2, std::ios::cur); // 跳过标志
        entry.compressionMethod = readUint16LE(file);
        uint32_t dosTime = readUint32LE(file);
        entry.modificationTime = fromDosTime(dosTime);
        entry.crc32 = readUint32LE(file);
        entry.compressedSize = readUint32LE(file);
        entry.uncompressedSize = readUint32LE(file);
        uint16_t fileNameLength = readUint16LE(file);
        uint16_t extraFieldLength = readUint16LE(file);
        uint16_t commentLength = readUint16LE(file);
        
        file.seekg(8, std::ios::cur); // 跳过磁盘号和属性
        uint32_t localHeaderOffset = readUint32LE(file);
        
        // 读取文件名
        std::vector<char> fileName(fileNameLength);
        file.read(fileName.data(), fileNameLength);
        entry.fileName = std::string(fileName.begin(), fileName.end());
        entry.isDirectory = entry.fileName.back() == '/';
        
        // 跳过额外字段和注释
        file.seekg(extraFieldLength + commentLength, std::ios::cur);
        
        entries_[entry.fileName] = entry;
    }
    
    file.close();
    return true;
}

long SimpleZipExtractor::findEndOfCentralDirectory(std::ifstream& file) {
    file.seekg(0, std::ios::end);
    long fileSize = file.tellg();
    
    // 从文件末尾开始搜索EOCD签名
    for (long offset = fileSize - 22; offset >= 0; --offset) {
        file.seekg(offset);
        uint32_t signature = readUint32LE(file);
        if (signature == END_OF_CENTRAL_DIR_SIGNATURE) {
            return offset;
        }
    }
    
    return -1;
}

uint16_t SimpleZipExtractor::readUint16LE(std::ifstream& file) {
    uint8_t bytes[2];
    file.read(reinterpret_cast<char*>(bytes), 2);
    return bytes[0] | (bytes[1] << 8);
}

uint32_t SimpleZipExtractor::readUint32LE(std::ifstream& file) {
    uint8_t bytes[4];
    file.read(reinterpret_cast<char*>(bytes), 4);
    return bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
}

std::time_t SimpleZipExtractor::fromDosTime(uint32_t dosTime) {
    uint16_t dosDate = (dosTime >> 16) & 0xFFFF;
    uint16_t time = dosTime & 0xFFFF;
    
    struct tm timeinfo = {};
    timeinfo.tm_year = ((dosDate >> 9) & 0x7F) + 80;
    timeinfo.tm_mon = ((dosDate >> 5) & 0x0F) - 1;
    timeinfo.tm_mday = dosDate & 0x1F;
    timeinfo.tm_hour = (time >> 11) & 0x1F;
    timeinfo.tm_min = (time >> 5) & 0x3F;
    timeinfo.tm_sec = (time & 0x1F) * 2;
    
    return std::mktime(&timeinfo);
}

// ========== ZipUtils 实现 ==========

bool ZipUtils::packDirectory(const std::string& dirPath, const std::string& zipPath) {
    SimpleZipCreator creator;
    if (!creator.addDirectoryRecursive(dirPath)) {
        return false;
    }
    return creator.save(zipPath);
}

bool ZipUtils::unpackToDirectory(const std::string& zipPath, const std::string& outputDir) {
    SimpleZipExtractor extractor;
    if (!extractor.open(zipPath)) {
        return false;
    }
    return extractor.extractAll(outputDir);
}

bool ZipUtils::isValidZip(const std::string& zipPath) {
    SimpleZipExtractor extractor;
    return extractor.open(zipPath);
}

std::string ZipUtils::getZipInfo(const std::string& zipPath) {
    SimpleZipExtractor extractor;
    if (!extractor.open(zipPath)) {
        return "无效的ZIP文件";
    }
    
    auto files = extractor.getFileList();
    std::stringstream ss;
    ss << "ZIP文件信息: " << zipPath << "\n";
    ss << "文件数量: " << files.size() << "\n";
    ss << "文件列表:\n";
    for (const auto& file : files) {
        const ZipEntry* info = extractor.getFileInfo(file);
        if (info) {
            ss << "  " << file << " (" << info->uncompressedSize << " bytes)\n";
        }
    }
    
    return ss.str();
}

bool ZipUtils::createEmptyZip(const std::string& zipPath) {
    SimpleZipCreator creator;
    return creator.save(zipPath);
}

} // namespace chtl