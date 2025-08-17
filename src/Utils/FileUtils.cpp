#include "../../include/Utils/FileUtils.h"
#include <fstream>
#include <sstream>
#include <random>
#include <iomanip>

namespace CHTL {
namespace Utils {

using namespace Core;

String FileUtils::readFile(const String& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    String content(size, '\0');
    file.read(&content[0], size);
    
    return content;
}

bool FileUtils::writeFile(const String& filename, const String& content) {
    try {
        // 确保目录存在
        auto dir = getDirectory(filename);
        if (!dir.empty() && !fileExists(dir)) {
            createDirectory(dir);
        }
        
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            return false;
        }
        
        file.write(content.c_str(), content.size());
        return file.good();
    } catch (...) {
        return false;
    }
}

bool FileUtils::fileExists(const String& filename) {
    return std::filesystem::exists(filename);
}

bool FileUtils::isDirectory(const String& path) {
    return std::filesystem::is_directory(path);
}

String FileUtils::getDirectory(const String& filepath) {
    std::filesystem::path p(filepath);
    return p.parent_path().string();
}

String FileUtils::getFilename(const String& filepath) {
    std::filesystem::path p(filepath);
    return p.filename().string();
}

String FileUtils::getExtension(const String& filepath) {
    std::filesystem::path p(filepath);
    return p.extension().string();
}

String FileUtils::getBasename(const String& filepath) {
    std::filesystem::path p(filepath);
    return p.stem().string();
}

String FileUtils::joinPath(const String& dir, const String& filename) {
    std::filesystem::path p(dir);
    p /= filename;
    return p.string();
}

String FileUtils::normalizePath(const String& path) {
    std::filesystem::path p(path);
    return std::filesystem::weakly_canonical(p).string();
}

String FileUtils::getAbsolutePath(const String& path) {
    return std::filesystem::absolute(path).string();
}

bool FileUtils::createDirectory(const String& path) {
    try {
        return std::filesystem::create_directories(path);
    } catch (...) {
        return false;
    }
}

bool FileUtils::removeDirectory(const String& path) {
    try {
        return std::filesystem::remove_all(path) > 0;
    } catch (...) {
        return false;
    }
}

std::vector<String> FileUtils::listFiles(const String& directory, const String& extension) {
    std::vector<String> files;
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                String filepath = entry.path().string();
                if (extension.empty() || getExtension(filepath) == extension) {
                    files.push_back(filepath);
                }
            }
        }
    } catch (...) {
        // 忽略错误，返回空列表
    }
    
    return files;
}

std::vector<String> FileUtils::listDirectories(const String& directory) {
    std::vector<String> dirs;
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_directory()) {
                dirs.push_back(entry.path().string());
            }
        }
    } catch (...) {
        // 忽略错误，返回空列表
    }
    
    return dirs;
}

String FileUtils::findFile(const String& filename, const std::vector<String>& searchPaths) {
    // 如果是绝对路径，直接检查
    if (std::filesystem::path(filename).is_absolute()) {
        return fileExists(filename) ? filename : "";
    }
    
    // 在搜索路径中查找
    for (const auto& searchPath : searchPaths) {
        String fullPath = joinPath(searchPath, filename);
        if (fileExists(fullPath)) {
            return fullPath;
        }
    }
    
    return "";
}

std::vector<String> FileUtils::findFiles(const String& pattern,
                                        const String& directory,
                                        bool recursive) {
    std::vector<String> files;
    
    try {
        if (recursive) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    String filepath = entry.path().string();
                    // 简单的通配符匹配（只支持 * 和 ?）
                    if (matchesPattern(getFilename(filepath), pattern)) {
                        files.push_back(filepath);
                    }
                }
            }
        } else {
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    String filepath = entry.path().string();
                    if (matchesPattern(getFilename(filepath), pattern)) {
                        files.push_back(filepath);
                    }
                }
            }
        }
    } catch (...) {
        // 忽略错误
    }
    
    return files;
}

size_t FileUtils::getFileSize(const String& filename) {
    try {
        return std::filesystem::file_size(filename);
    } catch (...) {
        return 0;
    }
}

std::time_t FileUtils::getModificationTime(const String& filename) {
    try {
        auto ftime = std::filesystem::last_write_time(filename);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
        return std::chrono::system_clock::to_time_t(sctp);
    } catch (...) {
        return 0;
    }
}

String FileUtils::createTempFile(const String& prefix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    
    std::stringstream ss;
    ss << std::filesystem::temp_directory_path().string() << "/" << prefix;
    
    for (int i = 0; i < 8; ++i) {
        ss << std::hex << dis(gen);
    }
    
    ss << ".tmp";
    return ss.str();
}

String FileUtils::createTempDirectory(const String& prefix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    
    std::stringstream ss;
    ss << std::filesystem::temp_directory_path().string() << "/" << prefix;
    
    for (int i = 0; i < 8; ++i) {
        ss << std::hex << dis(gen);
    }
    
    String tempDir = ss.str();
    createDirectory(tempDir);
    return tempDir;
}

// 私有辅助函数
bool FileUtils::matchesPattern(const String& filename, const String& pattern) {
    // 简单的通配符匹配实现
    size_t i = 0, j = 0;
    size_t starIdx = String::npos, match = 0;
    
    while (i < filename.length()) {
        if (j < pattern.length() && (pattern[j] == '?' || pattern[j] == filename[i])) {
            i++;
            j++;
        } else if (j < pattern.length() && pattern[j] == '*') {
            starIdx = j;
            match = i;
            j++;
        } else if (starIdx != String::npos) {
            j = starIdx + 1;
            match++;
            i = match;
        } else {
            return false;
        }
    }
    
    while (j < pattern.length() && pattern[j] == '*') {
        j++;
    }
    
    return j == pattern.length();
}

} // namespace Utils
} // namespace CHTL