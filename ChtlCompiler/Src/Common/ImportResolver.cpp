#include "Common/ImportResolver.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace Chtl {

ImportResolver::ImportResolver() {
}

ImportResolver::~ImportResolver() {
}

void ImportResolver::setModuleSearchPaths(const ModuleSearchPaths& paths) {
    searchPaths_ = paths;
}

void ImportResolver::setCurrentFilePath(const std::string& filePath) {
    currentFilePath_ = normalizePath(filePath);
    currentDirectory_ = getDirectory(currentFilePath_);
}

std::string ImportResolver::resolveImportPath(const ImportInfo& importInfo) {
    std::string path = importInfo.path;
    
    // 将 . 替换为 / （根据语法文档）
    std::replace(path.begin(), path.end(), '.', '/');
    
    // 根据导入类型处理
    switch (importInfo.type) {
        case ImportStatementNode::HTML:
        case ImportStatementNode::STYLE:
        case ImportStatementNode::JAVASCRIPT:
            return resolveResourceImport(path, importInfo.type);
            
        case ImportStatementNode::CHTL:
            return resolveChtlImport(path);
            
        case ImportStatementNode::CJMOD:
            return resolveCJmodImport(path);
    }
    
    return "";
}

bool ImportResolver::checkCircularDependency(const std::string& fromPath, const std::string& toPath) {
    std::string normalizedFrom = normalizePath(fromPath);
    std::string normalizedTo = normalizePath(toPath);
    
    // 如果是同一个文件，肯定是循环依赖
    if (normalizedFrom == normalizedTo) {
        return true;
    }
    
    // 使用DFS检查是否存在从toPath到fromPath的路径
    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> recursionStack;
    
    return hasCircularDependencyDFS(normalizedTo, normalizedFrom, visited, recursionStack);
}

void ImportResolver::recordImport(const std::string& fromPath, const std::string& toPath) {
    std::string normalizedFrom = normalizePath(fromPath);
    std::string normalizedTo = normalizePath(toPath);
    
    dependencyGraph_[normalizedFrom].insert(normalizedTo);
    importedFiles_[normalizedFrom].insert(normalizedTo);
}

bool ImportResolver::isAlreadyImported(const std::string& fromPath, const std::string& importPath) {
    std::string normalizedFrom = normalizePath(fromPath);
    std::string normalizedImport = normalizePath(importPath);
    
    auto it = importedFiles_.find(normalizedFrom);
    if (it != importedFiles_.end()) {
        return it->second.find(normalizedImport) != it->second.end();
    }
    return false;
}

std::string ImportResolver::normalizePath(const std::string& path) {
    try {
        fs::path p(path);
        // 规范化路径，解析 . 和 ..
        fs::path normalized = fs::weakly_canonical(p);
        return normalized.string();
    } catch (const std::exception&) {
        // 如果路径不存在，至少规范化路径分隔符
        std::string result = path;
        std::replace(result.begin(), result.end(), '\\', '/');
        return result;
    }
}

std::string ImportResolver::resolveResourceImport(const std::string& path, ImportStatementNode::ImportType type) {
    // 获取预期的文件扩展名
    std::vector<std::string> extensions;
    switch (type) {
        case ImportStatementNode::HTML:
            extensions = {".html", ".htm"};
            break;
        case ImportStatementNode::STYLE:
            extensions = {".css"};
            break;
        case ImportStatementNode::JAVASCRIPT:
            extensions = {".js"};
            break;
        default:
            return "";
    }
    
    // 如果路径已经包含扩展名
    if (hasFileExtension(path)) {
        std::string fullPath = joinPath(currentDirectory_, path);
        if (fileExists(fullPath)) {
            return normalizePath(fullPath);
        }
        return "";  // 文件不存在
    }
    
    // 如果路径不包含扩展名，搜索对应类型的文件
    for (const auto& ext : extensions) {
        std::string fullPath = joinPath(currentDirectory_, path + ext);
        if (fileExists(fullPath)) {
            return normalizePath(fullPath);
        }
    }
    
    return "";  // 未找到文件
}

std::string ImportResolver::resolveChtlImport(const std::string& path) {
    // 检查是否是官方模块引用
    if (isOfficialModuleReference(path)) {
        std::string moduleName = extractOfficialModuleName(path);
        return resolveChtlImport(moduleName);  // 递归处理，去掉前缀
    }
    
    // 处理通配符导入
    if (path.find('*') != std::string::npos) {
        // 这个方法返回的是vector，调用者需要特殊处理
        // 这里暂时返回空，实际使用时应该调用resolveWildcardImport
        return "";
    }
    
    std::vector<std::string> searchPaths;
    std::vector<std::string> extensions;
    
    // 如果路径包含扩展名
    if (hasFileExtension(path)) {
        // 1. 先在官方模块中搜索
        searchPaths.push_back(searchPaths_.officialModulePath);
        // 2. 在本地module文件夹中搜索
        searchPaths.push_back(searchPaths_.localModulePath);
        // 3. 在当前目录中搜索
        searchPaths.push_back(currentDirectory_);
        
        // 搜索具体文件
        for (const auto& searchPath : searchPaths) {
            std::string fullPath = joinPath(searchPath, path);
            if (fileExists(fullPath)) {
                return normalizePath(fullPath);
            }
        }
    }
    else {
        // 没有扩展名，搜索.cmod和.chtl文件，cmod优先
        extensions = {".cmod", ".chtl"};
        
        // 1. 先在官方模块中搜索
        searchPaths.push_back(searchPaths_.officialModulePath);
        // 2. 在本地module文件夹中搜索
        searchPaths.push_back(searchPaths_.localModulePath);
        // 3. 在当前目录中搜索
        searchPaths.push_back(currentDirectory_);
        
        return searchFile(path, searchPaths, extensions);
    }
    
    return "";  // 未找到文件
}

std::string ImportResolver::resolveCJmodImport(const std::string& path) {
    std::vector<std::string> searchPaths;
    std::vector<std::string> extensions;
    
    // 如果路径包含扩展名
    if (hasFileExtension(path)) {
        // 1. 先在官方模块中搜索
        searchPaths.push_back(searchPaths_.officialModulePath);
        // 2. 在本地module文件夹中搜索
        searchPaths.push_back(searchPaths_.localModulePath);
        // 3. 在当前目录中搜索
        searchPaths.push_back(currentDirectory_);
        
        // 搜索具体文件
        for (const auto& searchPath : searchPaths) {
            std::string fullPath = joinPath(searchPath, path);
            if (fileExists(fullPath)) {
                return normalizePath(fullPath);
            }
        }
    }
    else {
        // 没有扩展名，搜索.cjmod文件
        extensions = {".cjmod"};
        
        // 1. 先在官方模块中搜索
        searchPaths.push_back(searchPaths_.officialModulePath);
        // 2. 在本地module文件夹中搜索
        searchPaths.push_back(searchPaths_.localModulePath);
        // 3. 在当前目录中搜索
        searchPaths.push_back(currentDirectory_);
        
        return searchFile(path, searchPaths, extensions);
    }
    
    return "";  // 未找到文件
}

std::vector<std::string> ImportResolver::resolveWildcardImport(const std::string& pattern) {
    std::vector<std::string> results;
    
    // 解析模式：path/* 或 path/*.ext
    size_t starPos = pattern.find('*');
    if (starPos == std::string::npos) {
        return results;  // 没有通配符
    }
    
    std::string basePath = pattern.substr(0, starPos);
    std::string suffix = pattern.substr(starPos + 1);
    
    // 确定搜索目录
    std::string searchDir;
    if (isAbsolutePath(basePath)) {
        searchDir = basePath;
    } else {
        searchDir = joinPath(currentDirectory_, basePath);
    }
    
    // 确定要匹配的扩展名
    std::vector<std::string> targetExtensions;
    if (suffix.empty() || suffix == "") {
        // path/* - 匹配.cmod和.chtl
        targetExtensions = {".cmod", ".chtl"};
    } else if (suffix.find('.') == 0) {
        // path/*.ext - 匹配特定扩展名
        targetExtensions = {suffix};
    }
    
    // 遍历目录
    try {
        for (const auto& entry : fs::directory_iterator(searchDir)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                std::string ext = entry.path().extension().string();
                
                // 检查扩展名是否匹配
                for (const auto& targetExt : targetExtensions) {
                    if (ext == targetExt) {
                        results.push_back(normalizePath(entry.path().string()));
                        break;
                    }
                }
            }
        }
    } catch (const std::exception&) {
        // 目录不存在或无法访问
    }
    
    return results;
}

bool ImportResolver::isOfficialModuleReference(const std::string& path) {
    return path.find("chtl::") == 0;
}

std::string ImportResolver::extractOfficialModuleName(const std::string& path) {
    if (isOfficialModuleReference(path)) {
        return path.substr(6);  // 去掉 "chtl::" 前缀
    }
    return path;
}

std::string ImportResolver::getFileExtension(const std::string& path) {
    fs::path p(path);
    return p.extension().string();
}

bool ImportResolver::fileExists(const std::string& path) {
    return fs::exists(path) && fs::is_regular_file(path);
}

std::string ImportResolver::searchFile(const std::string& name, const std::vector<std::string>& searchPaths, 
                                      const std::vector<std::string>& extensions) {
    for (const auto& searchPath : searchPaths) {
        for (const auto& ext : extensions) {
            std::string fullPath = joinPath(searchPath, name + ext);
            if (fileExists(fullPath)) {
                return normalizePath(fullPath);
            }
        }
    }
    return "";
}

void ImportResolver::clear() {
    dependencyGraph_.clear();
    importChain_.clear();
    importedFiles_.clear();
}

// 私有辅助方法实现

bool ImportResolver::hasCircularDependencyDFS(const std::string& current, const std::string& target,
                                             std::unordered_set<std::string>& visited,
                                             std::unordered_set<std::string>& recursionStack) {
    visited.insert(current);
    recursionStack.insert(current);
    
    auto it = dependencyGraph_.find(current);
    if (it != dependencyGraph_.end()) {
        for (const auto& dep : it->second) {
            if (dep == target) {
                return true;  // 找到循环依赖
            }
            
            if (recursionStack.find(dep) != recursionStack.end()) {
                return true;  // 在当前路径中已经访问过，存在循环
            }
            
            if (visited.find(dep) == visited.end()) {
                if (hasCircularDependencyDFS(dep, target, visited, recursionStack)) {
                    return true;
                }
            }
        }
    }
    
    recursionStack.erase(current);
    return false;
}

std::string ImportResolver::joinPath(const std::string& base, const std::string& relative) {
    fs::path basePath(base);
    fs::path relativePath(relative);
    
    if (relativePath.is_absolute()) {
        return relative;
    }
    
    return (basePath / relativePath).string();
}

bool ImportResolver::isAbsolutePath(const std::string& path) {
    fs::path p(path);
    return p.is_absolute();
}

bool ImportResolver::hasFileExtension(const std::string& path) {
    fs::path p(path);
    return !p.extension().empty();
}

std::string ImportResolver::removeFileExtension(const std::string& path) {
    fs::path p(path);
    return p.stem().string();
}

std::string ImportResolver::getDirectory(const std::string& filePath) {
    fs::path p(filePath);
    return p.parent_path().string();
}

std::string ImportResolver::getFileName(const std::string& filePath) {
    fs::path p(filePath);
    return p.filename().string();
}

} // namespace Chtl