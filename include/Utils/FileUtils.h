#pragma once

#include "../Core/Types.h"
#include <filesystem>
#include <vector>

namespace CHTL {
namespace Utils {

class FileUtils {
public:
    // 文件读写
    static Core::String readFile(const Core::String& filename);
    static bool writeFile(const Core::String& filename, const Core::String& content);
    static bool fileExists(const Core::String& filename);
    static bool isDirectory(const Core::String& path);
    
    // 路径操作
    static Core::String getDirectory(const Core::String& filepath);
    static Core::String getFilename(const Core::String& filepath);
    static Core::String getExtension(const Core::String& filepath);
    static Core::String getBasename(const Core::String& filepath);
    static Core::String joinPath(const Core::String& dir, const Core::String& filename);
    static Core::String normalizePath(const Core::String& path);
    static Core::String getAbsolutePath(const Core::String& path);
    
    // 目录操作
    static bool createDirectory(const Core::String& path);
    static bool removeDirectory(const Core::String& path);
    static std::vector<Core::String> listFiles(const Core::String& directory, 
                                              const Core::String& extension = "");
    static std::vector<Core::String> listDirectories(const Core::String& directory);
    
    // 文件查找
    static Core::String findFile(const Core::String& filename, 
                                const std::vector<Core::String>& searchPaths);
    static std::vector<Core::String> findFiles(const Core::String& pattern,
                                              const Core::String& directory,
                                              bool recursive = false);
    
    // 文件信息
    static size_t getFileSize(const Core::String& filename);
    static std::time_t getModificationTime(const Core::String& filename);
    
    // 临时文件
    static Core::String createTempFile(const Core::String& prefix = "chtl_");
    static Core::String createTempDirectory(const Core::String& prefix = "chtl_");

private:
    // 辅助函数
    static bool matchesPattern(const Core::String& filename, const Core::String& pattern);
};

} // namespace Utils
} // namespace CHTL