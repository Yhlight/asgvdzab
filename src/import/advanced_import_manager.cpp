#include "import/advanced_import_manager.hpp"
#include <algorithm>
#include <sstream>
#include <regex>
#include <fstream>

namespace chtl {

// ========== CircularDependencyDetector 实现 ==========

CircularDependencyDetector::CircularDependencyDetector() {
}

bool CircularDependencyDetector::addDependency(const std::string& from, const std::string& to) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // 检查是否会造成循环依赖 (使用内部方法避免重复加锁)
    std::unordered_set<std::string> visited;
    if (hasPathDFS(to, from, visited)) {
        return false;
    }
    
    dependencies_[from].insert(to);
    return true;
}

bool CircularDependencyDetector::hasCircularDependency(const std::string& from, const std::string& to) const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::unordered_set<std::string> visited;
    return hasPathDFS(from, to, visited);
}

bool CircularDependencyDetector::hasPathDFS(const std::string& from, const std::string& to, 
                                           std::unordered_set<std::string>& visited) const {
    if (from == to) {
        return true;
    }
    
    if (visited.count(from)) {
        return false;
    }
    
    visited.insert(from);
    
    auto it = dependencies_.find(from);
    if (it != dependencies_.end()) {
        for (const auto& dependency : it->second) {
            if (hasPathDFS(dependency, to, visited)) {
                return true;
            }
        }
    }
    
    return false;
}

std::vector<std::string> CircularDependencyDetector::getCircularPath(const std::string& from, const std::string& to) const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<std::string> path;
    std::unordered_set<std::string> visited;
    std::vector<std::string> result;
    
    path.push_back(from);
    getPathDFS(from, to, path, visited, result);
    
    return result;
}

void CircularDependencyDetector::getPathDFS(const std::string& current, const std::string& target,
                                           std::vector<std::string>& path, std::unordered_set<std::string>& visited,
                                           std::vector<std::string>& result) const {
    if (current == target) {
        result = path;
        result.push_back(target);
        return;
    }
    
    if (visited.count(current)) {
        return;
    }
    
    visited.insert(current);
    
    auto it = dependencies_.find(current);
    if (it != dependencies_.end()) {
        for (const auto& dependency : it->second) {
            path.push_back(dependency);
            getPathDFS(dependency, target, path, visited, result);
            if (!result.empty()) {
                return;
            }
            path.pop_back();
        }
    }
}

void CircularDependencyDetector::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    dependencies_.clear();
}

std::string CircularDependencyDetector::getDependencyGraph() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::stringstream ss;
    
    ss << "Dependency Graph:\n";
    for (const auto& [from, dependencies] : dependencies_) {
        ss << "  " << from << " -> ";
        bool first = true;
        for (const auto& to : dependencies) {
            if (!first) ss << ", ";
            ss << to;
            first = false;
        }
        ss << "\n";
    }
    
    return ss.str();
}

// ========== PathNormalizer 实现 ==========

std::string PathNormalizer::normalize(const std::string& path, const std::string& basePath) {
    if (path.empty()) {
        return path;
    }
    
    std::filesystem::path fsPath(path);
    
    // 如果是相对路径，基于basePath解析
    if (fsPath.is_relative() && !basePath.empty()) {
        fsPath = std::filesystem::path(basePath) / fsPath;
    }
    
    // 标准化路径
    return fsPath.lexically_normal().string();
}

ImportPathInfo PathNormalizer::parsePathInfo(const std::string& path) {
    ImportPathInfo info;
    info.originalPath = path;
    
    // 检查通配符
    if (path.find('*') != std::string::npos) {
        info.isWildcard = true;
        
        if (path.length() >= 2 && path.substr(path.length() - 2) == ".*") {
            info.pathType = ImportPathType::WILDCARD_ALL;
            info.normalizedPath = path.substr(0, path.length() - 2);
        } else if (path.length() >= 2 && path.substr(path.length() - 2) == "/*") {
            info.pathType = ImportPathType::WILDCARD_ALL;
            info.normalizedPath = path.substr(0, path.length() - 2);
        } else {
            // *.ext 格式
            size_t slashPos = path.find_last_of("/\\");
            size_t dotPos = path.find_last_of('.');
            
            if (dotPos != std::string::npos && dotPos > slashPos) {
                info.pathType = ImportPathType::WILDCARD_EXT;
                info.extension = path.substr(dotPos + 1);
                
                if (slashPos != std::string::npos) {
                    info.normalizedPath = path.substr(0, slashPos);
                } else {
                    info.normalizedPath = ".";
                }
            } else {
                info.pathType = ImportPathType::WILDCARD_ALL;
                if (slashPos != std::string::npos) {
                    info.normalizedPath = path.substr(0, slashPos);
                } else {
                    info.normalizedPath = ".";
                }
            }
        }
    } else {
        // 检查是否为子模块语法 (Module.SubModule)
        size_t dotPos = path.find('.');
        if (dotPos != std::string::npos && path.find('/') == std::string::npos && path.find('\\') == std::string::npos) {
            info.moduleName = path.substr(0, dotPos);
            info.subModuleName = path.substr(dotPos + 1);
            info.pathType = ImportPathType::SUBMODULE_SPECIFIC;
            info.normalizedPath = path;
        } else {
            // 普通路径
            std::filesystem::path fsPath(path);
            
            if (fsPath.has_extension()) {
                info.pathType = ImportPathType::NAME_WITH_EXT;
                info.extension = fsPath.extension().string().substr(1); // 去掉点
            } else {
                info.pathType = ImportPathType::NAME_ONLY;
            }
            
            // 检查是否为具体路径
            if (path.find('/') != std::string::npos || path.find('\\') != std::string::npos) {
                if (std::filesystem::is_directory(path)) {
                    info.pathType = ImportPathType::FOLDER_PATH;
                } else {
                    info.pathType = ImportPathType::SPECIFIC_PATH;
                }
            }
            
            info.normalizedPath = normalize(path);
        }
    }
    
    return info;
}

bool PathNormalizer::isSamePath(const std::string& path1, const std::string& path2) {
    return normalize(path1) == normalize(path2);
}

std::string PathNormalizer::getExtension(const std::string& path) {
    std::filesystem::path fsPath(path);
    if (fsPath.has_extension()) {
        std::string ext = fsPath.extension().string();
        return ext.empty() ? ext : ext.substr(1); // 去掉点
    }
    return "";
}

std::string PathNormalizer::getFileNameWithoutExtension(const std::string& path) {
    std::filesystem::path fsPath(path);
    return fsPath.stem().string();
}

bool PathNormalizer::isWildcardPath(const std::string& path) {
    return path.find('*') != std::string::npos;
}

// ========== AdvancedImportManager 实现 ==========

AdvancedImportManager::AdvancedImportManager(const ImportSearchConfig& config)
    : config_(config), circularDetector_(std::make_unique<CircularDependencyDetector>()),
      officialModuleRoot_("/usr/local/share/chtl/official") {
}

AdvancedImportManager::~AdvancedImportManager() = default;

void AdvancedImportManager::setSearchConfig(const ImportSearchConfig& config) {
    std::lock_guard<std::mutex> lock(mutex_);
    config_ = config;
}

const ImportSearchConfig& AdvancedImportManager::getSearchConfig() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return config_;
}

void AdvancedImportManager::addSearchPath(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    config_.searchPaths.push_back(path);
}

ImportResult AdvancedImportManager::processImport(const std::string& importType, const std::string& fromPath,
                                                 const std::string& asName, const std::string& sourceFile) {
    ImportResult result;
    
    // 解析导入类型
    ImportType type = parseImportType(importType);
    
    // 解析路径信息
    ImportPathInfo pathInfo = PathNormalizer::parsePathInfo(fromPath);
    
    // 创建导入记录
    ImportRecord record;
    record.type = type;
    record.pathInfo = pathInfo;
    record.asName = asName;
    record.sourceFile = sourceFile;
    record.hasAsClause = !asName.empty();
    
    // 根据导入类型处理
    switch (type) {
        case ImportType::HTML:
        case ImportType::STYLE:
        case ImportType::JAVASCRIPT:
            result = processOriginImport(type, pathInfo, asName, sourceFile);
            break;
            
        case ImportType::CHTL:
            result = processChtlImport(pathInfo, sourceFile);
            break;
            
        case ImportType::CJMOD:
            result = processCjmodImport(pathInfo, sourceFile);
            break;
            
        default:
            result.errorMessage = "Unsupported import type: " + importType;
            return result;
    }
    
    // 如果成功，添加导入记录
    if (result.success) {
        for (const auto& resolvedFile : result.resolvedFiles) {
            ImportRecord recordCopy = record;
            recordCopy.resolvedPath = resolvedFile;
            recordCopy.importId = generateImportId(recordCopy);
            
            if (!addImportRecord(recordCopy)) {
                result.success = false;
                result.errorMessage = "Failed to add import record for: " + resolvedFile;
                break;
            }
            
            result.imports.push_back(recordCopy);
        }
        
        // 更新依赖关系
        updateDependencies(sourceFile, result.resolvedFiles);
    }
    
    return result;
}

ImportResult AdvancedImportManager::processOriginImport(ImportType type, const ImportPathInfo& pathInfo,
                                                       const std::string& asName, const std::string& sourceFile) {
    ImportResult result;
    
    // 对于@Html/@Style/@JavaScript，如果没有as语法，则直接跳过
    if (asName.empty()) {
        result.success = true;
        result.errorMessage = "Skipped origin import without 'as' clause";
        return result;
    }
    
    // 检查路径类型
    if (pathInfo.pathType == ImportPathType::FOLDER_PATH) {
        result.errorMessage = "Folder path not allowed for origin imports";
        return result;
    }
    
    // 搜索文件
    std::vector<std::string> foundFiles = searchFiles(pathInfo, type, sourceFile);
    
    if (foundFiles.empty()) {
        result.errorMessage = "File not found: " + pathInfo.originalPath;
        return result;
    }
    
    result.success = true;
    result.resolvedFiles = foundFiles;
    
    return result;
}

ImportResult AdvancedImportManager::processChtlImport(const ImportPathInfo& pathInfo, const std::string& sourceFile) {
    ImportResult result;
    
    // 检查路径类型
    if (pathInfo.pathType == ImportPathType::FOLDER_PATH) {
        result.errorMessage = "Folder path not allowed for CHTL imports";
        return result;
    }
    
    // 搜索模块文件
    std::vector<std::string> foundFiles = searchModuleFiles(pathInfo, ImportType::CHTL, sourceFile);
    
    if (foundFiles.empty()) {
        result.errorMessage = "CHTL module not found: " + pathInfo.originalPath;
        return result;
    }
    
    result.success = true;
    result.resolvedFiles = foundFiles;
    
    return result;
}

ImportResult AdvancedImportManager::processCjmodImport(const ImportPathInfo& pathInfo, const std::string& sourceFile) {
    ImportResult result;
    
    // 检查路径类型
    if (pathInfo.pathType == ImportPathType::FOLDER_PATH) {
        result.errorMessage = "Folder path not allowed for CJmod imports";
        return result;
    }
    
    // 搜索模块文件
    std::vector<std::string> foundFiles = searchModuleFiles(pathInfo, ImportType::CJMOD, sourceFile);
    
    if (foundFiles.empty()) {
        result.errorMessage = "CJmod module not found: " + pathInfo.originalPath;
        return result;
    }
    
    result.success = true;
    result.resolvedFiles = foundFiles;
    
    return result;
}

std::vector<std::string> AdvancedImportManager::searchFiles(const ImportPathInfo& pathInfo, ImportType type,
                                                          const std::string& sourceFile) {
    std::vector<std::string> result;
    
    if (pathInfo.isWildcard) {
        return searchWildcardFiles(pathInfo, type, sourceFile);
    }
    
    std::vector<std::string> extensions = getImportTypeExtensions(type);
    std::filesystem::path sourceDir = std::filesystem::path(sourceFile).parent_path();
    
    if (pathInfo.pathType == ImportPathType::SPECIFIC_PATH) {
        // 具体路径
        std::string fullPath = resolveRelativePath(pathInfo.normalizedPath, sourceDir.string());
        if (fileExists(fullPath)) {
            result.push_back(fullPath);
        }
    } else if (pathInfo.pathType == ImportPathType::NAME_WITH_EXT) {
        // 带扩展名的文件名
        std::string fullPath = sourceDir / pathInfo.normalizedPath;
        if (fileExists(fullPath)) {
            result.push_back(fullPath);
        }
    } else {
        // 仅文件名，尝试各种扩展名
        for (const auto& ext : extensions) {
            std::string filename = pathInfo.normalizedPath + "." + ext;
            std::string fullPath = sourceDir / filename;
            if (fileExists(fullPath)) {
                result.push_back(fullPath);
                break; // 找到第一个匹配的就停止
            }
        }
    }
    
    return result;
}

std::vector<std::string> AdvancedImportManager::searchModuleFiles(const ImportPathInfo& pathInfo, ImportType type,
                                                                 const std::string& sourceFile) {
    std::vector<std::string> result;
    
    if (pathInfo.isWildcard) {
        return searchWildcardFiles(pathInfo, type, sourceFile);
    }
    
    std::vector<std::string> extensions = getImportTypeExtensions(type);
    std::filesystem::path sourceDir = std::filesystem::path(sourceFile).parent_path();
    
    // 搜索路径优先级
    std::vector<std::string> searchDirs;
    
    if (pathInfo.pathType == ImportPathType::SPECIFIC_PATH) {
        // 具体路径，直接搜索
        std::string fullPath = resolveRelativePath(pathInfo.normalizedPath, sourceDir.string());
        if (fileExists(fullPath)) {
            result.push_back(fullPath);
        }
        return result;
    }
    
    // 构建搜索目录列表
    searchDirs.push_back(config_.officialModulePath);                    // 官方模块
    searchDirs.push_back(sourceDir / config_.moduleDirectory);          // 当前目录/module
    searchDirs.push_back(sourceDir.string());                          // 当前目录
    
    // 添加额外搜索路径
    for (const auto& path : config_.searchPaths) {
        searchDirs.push_back(path);
    }
    
    std::string targetName = pathInfo.normalizedPath;
    
    // 处理子模块语法
    if (pathInfo.pathType == ImportPathType::SUBMODULE_SPECIFIC) {
        // Module.SubModule -> Module/SubModule
        targetName = pathInfo.moduleName + "/" + pathInfo.subModuleName;
    }
    
    // 在各搜索目录中查找
    for (const auto& searchDir : searchDirs) {
        if (!directoryExists(searchDir)) {
            continue;
        }
        
        if (pathInfo.pathType == ImportPathType::NAME_WITH_EXT) {
            // 带扩展名
            std::string fullPath = std::filesystem::path(searchDir) / targetName;
            if (fileExists(fullPath)) {
                result.push_back(fullPath);
                break;
            }
        } else {
            // 尝试各种扩展名 (cmod优先)
            std::vector<std::string> tryExtensions = extensions;
            if (type == ImportType::CHTL) {
                // CHTL导入时，cmod优先
                std::sort(tryExtensions.begin(), tryExtensions.end(), [](const std::string& a, const std::string& b) {
                    if (a == "cmod" && b != "cmod") return true;
                    if (a != "cmod" && b == "cmod") return false;
                    return a < b;
                });
            }
            
            for (const auto& ext : tryExtensions) {
                std::string filename = targetName + "." + ext;
                std::string fullPath = std::filesystem::path(searchDir) / filename;
                if (fileExists(fullPath)) {
                    result.push_back(fullPath);
                    return result; // 找到就返回
                }
            }
        }
    }
    
    return result;
}

std::vector<std::string> AdvancedImportManager::searchWildcardFiles(const ImportPathInfo& pathInfo, ImportType type,
                                                                   const std::string& sourceFile) {
    std::vector<std::string> result;
    
    std::string searchDir;
    if (pathInfo.normalizedPath == "." || pathInfo.normalizedPath.empty()) {
        searchDir = std::filesystem::path(sourceFile).parent_path();
    } else {
        searchDir = resolveRelativePath(pathInfo.normalizedPath, std::filesystem::path(sourceFile).parent_path().string());
    }
    
    if (!directoryExists(searchDir)) {
        return result;
    }
    
    std::vector<std::string> extensions;
    if (pathInfo.pathType == ImportPathType::WILDCARD_EXT) {
        extensions.push_back(pathInfo.extension);
    } else {
        extensions = getImportTypeExtensions(type);
    }
    
    result = listFiles(searchDir, extensions);
    
    return result;
}

bool AdvancedImportManager::isDuplicateImport(const std::string& normalizedPath, const std::string& sourceFile) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::string key = sourceFile + ":" + normalizedPath;
    return duplicateChecker_.count(key) > 0;
}

bool AdvancedImportManager::addImportRecord(const ImportRecord& record) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // 验证记录
    if (!validateImportRecord(record)) {
        return false;
    }
    
    // 检查重复导入
    std::string key = record.sourceFile + ":" + record.resolvedPath;
    if (duplicateChecker_.count(key)) {
        return true; // 重复导入，但不是错误
    }
    
    // 检查循环依赖
    if (!circularDetector_->addDependency(record.sourceFile, record.resolvedPath)) {
        return false; // 循环依赖
    }
    
    // 添加记录
    importRecords_.push_back(record);
    duplicateChecker_.insert(key);
    
    return true;
}

const std::vector<ImportRecord>& AdvancedImportManager::getImportRecords() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return importRecords_;
}

void AdvancedImportManager::clearImportRecords() {
    std::lock_guard<std::mutex> lock(mutex_);
    importRecords_.clear();
    duplicateChecker_.clear();
    circularDetector_->clear();
}

ImportType AdvancedImportManager::parseImportType(const std::string& typeString) {
    if (typeString == "@Html") return ImportType::HTML;
    if (typeString == "@Style") return ImportType::STYLE;
    if (typeString == "@JavaScript") return ImportType::JAVASCRIPT;
    if (typeString == "@Chtl") return ImportType::CHTL;
    if (typeString == "@CJmod") return ImportType::CJMOD;
    if (typeString == "[Custom]") return ImportType::CUSTOM;
    if (typeString == "[Template]") return ImportType::TEMPLATE;
    
    return ImportType::CHTL; // 默认
}

std::string AdvancedImportManager::getImportTypeString(ImportType type) {
    switch (type) {
        case ImportType::HTML: return "@Html";
        case ImportType::STYLE: return "@Style";
        case ImportType::JAVASCRIPT: return "@JavaScript";
        case ImportType::CHTL: return "@Chtl";
        case ImportType::CJMOD: return "@CJmod";
        case ImportType::CUSTOM: return "[Custom]";
        case ImportType::TEMPLATE: return "[Template]";
        default: return "@Chtl";
    }
}

std::vector<std::string> AdvancedImportManager::getImportTypeExtensions(ImportType type) {
    switch (type) {
        case ImportType::HTML:
            return {"html", "htm"};
        case ImportType::STYLE:
            return {"css", "scss", "sass"};
        case ImportType::JAVASCRIPT:
            return {"js", "mjs"};
        case ImportType::CHTL:
            return {"cmod", "chtl"};
        case ImportType::CJMOD:
            return {"cjmod"};
        case ImportType::CUSTOM:
        case ImportType::TEMPLATE:
            return {"chtl"};
        default:
            return {"chtl"};
    }
}

bool AdvancedImportManager::fileExists(const std::string& path) {
    return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
}

bool AdvancedImportManager::directoryExists(const std::string& path) {
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

std::vector<std::string> AdvancedImportManager::listFiles(const std::string& directory,
                                                         const std::vector<std::string>& extensions) {
    std::vector<std::string> result;
    
    if (!directoryExists(directory)) {
        return result;
    }
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                std::string path = entry.path().string();
                
                if (extensions.empty()) {
                    result.push_back(path);
                } else {
                    std::string ext = PathNormalizer::getExtension(path);
                    if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) {
                        result.push_back(path);
                    }
                }
            }
        }
    } catch (const std::filesystem::filesystem_error&) {
        // 忽略文件系统错误
    }
    
    return result;
}

std::string AdvancedImportManager::getImportStatistics() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::stringstream ss;
    ss << "Import Statistics:\n";
    ss << "  Total imports: " << importRecords_.size() << "\n";
    
    std::unordered_map<ImportType, int> typeCounts;
    for (const auto& record : importRecords_) {
        typeCounts[record.type]++;
    }
    
    for (const auto& [type, count] : typeCounts) {
        ss << "  " << getImportTypeString(type) << ": " << count << "\n";
    }
    
    return ss.str();
}

std::string AdvancedImportManager::getDependencyGraph() const {
    return circularDetector_->getDependencyGraph();
}

std::string AdvancedImportManager::dumpImportRecords() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::stringstream ss;
    ss << "Import Records:\n";
    
    for (size_t i = 0; i < importRecords_.size(); ++i) {
        const auto& record = importRecords_[i];
        ss << "  [" << i << "] " << getImportTypeString(record.type) 
           << " from " << record.pathInfo.originalPath;
        if (record.hasAsClause) {
            ss << " as " << record.asName;
        }
        ss << " -> " << record.resolvedPath 
           << " (in " << record.sourceFile << ")\n";
    }
    
    return ss.str();
}

// 私有方法实现

std::string AdvancedImportManager::resolveRelativePath(const std::string& path, const std::string& basePath) {
    if (std::filesystem::path(path).is_absolute()) {
        return path;
    }
    
    return std::filesystem::path(basePath) / path;
}

std::string AdvancedImportManager::generateImportId(const ImportRecord& record) {
    return record.sourceFile + ":" + getImportTypeString(record.type) + ":" + record.resolvedPath;
}

bool AdvancedImportManager::validateImportRecord(const ImportRecord& record) {
    if (record.sourceFile.empty() || record.resolvedPath.empty()) {
        return false;
    }
    
    if (record.pathInfo.originalPath.empty()) {
        return false;
    }
    
    return true;
}

void AdvancedImportManager::updateDependencies(const std::string& sourceFile, const std::vector<std::string>& targetFiles) {
    for (const auto& targetFile : targetFiles) {
        circularDetector_->addDependency(sourceFile, targetFile);
    }
}

// ===== 官方模块前缀支持 =====

std::string AdvancedImportManager::resolveModuleImportPath(const std::string& importPath, ImportType importType) {
    // 检查是否为官方模块（chtl::前缀）
    if (importPath.length() >= 6 && importPath.substr(0, 6) == "chtl::") {
        std::string moduleName = importPath.substr(6); // 去除chtl::前缀
        
        std::string moduleTypeStr;
        if (importType == ImportType::CHTL) {
            moduleTypeStr = "cmod";
        } else if (importType == ImportType::CJMOD) {
            moduleTypeStr = "cjmod";
        } else {
            return importPath; // 其他类型不支持官方前缀
        }
        
        std::string basePath = officialModuleRoot_ + "/" + moduleTypeStr + "/" + moduleName;
        
        // 优先查找打包后的模块文件
        std::string packedPath = basePath + "." + moduleTypeStr;
        if (std::filesystem::exists(packedPath)) {
            return packedPath;
        }
        
        // 查找源码目录
        if (std::filesystem::exists(basePath) && std::filesystem::is_directory(basePath)) {
            return basePath;
        }
        
        // 如果官方模块不存在，返回空字符串表示未找到
        return "";
    }
    
    // 普通模块，返回原路径
    return importPath;
}

bool AdvancedImportManager::isOfficialModule(const std::string& moduleName) {
    return moduleName.length() >= 6 && moduleName.substr(0, 6) == "chtl::";
}

void AdvancedImportManager::setOfficialModuleRoot(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    officialModuleRoot_ = path;
}

} // namespace chtl