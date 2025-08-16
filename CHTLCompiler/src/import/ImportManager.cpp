#include "import/ImportManager.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <chrono>

namespace chtl {

// 实现类
class ImportManager::Impl {
public:
    ImportManagerConfig config;
    std::vector<ImportInfo> imports;
    std::vector<ModuleInfo> modules;
    std::vector<std::string> errors;
    std::unordered_map<std::string, std::string> import_cache;
    std::unordered_map<std::string, std::chrono::system_clock::time_point> cache_timestamps;
    bool debug_mode;
    
    Impl(const ImportManagerConfig& cfg) : config(cfg), debug_mode(false) {}
    
    ~Impl() = default;
    
    // 内部方法实现
    bool validateImport(const ImportInfo& import_info);
    bool checkCircularDependency(const std::string& path, const std::string& current_path);
    bool checkDuplicateImport(const ImportInfo& import_info);
    std::string normalizePath(const std::string& path);
    std::string getFileExtension(const std::string& filename);
    bool isValidFileExtension(const std::string& extension, ImportType type);
    std::vector<std::string> getSearchPaths(ImportType type);
    bool isOfficialModule(const std::string& path);
    bool isLocalModule(const std::string& path);
    void updateImportCache();
    void cleanupCache();
    
    // 路径解析方法
    std::string resolveHtmlStyleJsPath(const std::string& path, ImportType type);
    std::string resolveChtlPath(const std::string& path);
    std::string resolveCJmodPath(const std::string& path);
    std::vector<std::string> resolveWildcardPath(const std::string& path, ImportType type);
    std::vector<std::string> resolveSubmodulePath(const std::string& path, ImportType type);
    
    // 文件搜索方法
    std::string findFileInDirectory(const std::string& directory, const std::string& filename, ImportType type);
    std::vector<std::string> findFilesByPattern(const std::string& directory, const std::string& pattern);
    bool isDirectory(const std::string& path);
    bool isFile(const std::string& path);
    std::vector<std::string> listDirectory(const std::string& directory);
};

// ImportManager实现
ImportManager::ImportManager(const ImportManagerConfig& config) 
    : pImpl(std::make_unique<Impl>(config)) {}

ImportManager::~ImportManager() = default;

bool ImportManager::addImport(const ImportInfo& import_info) {
    if (!pImpl->validateImport(import_info)) {
        return false;
    }
    
    // 检查重复导入
    if (config.enable_duplicate_detection && pImpl->checkDuplicateImport(import_info)) {
        addError("重复导入: " + import_info.path);
        return false;
    }
    
    // 检查循环依赖
    if (config.enable_circular_detection && pImpl->checkCircularDependency(import_info.path, import_info.source_file)) {
        addError("循环依赖检测: " + import_info.path);
        return false;
    }
    
    pImpl->imports.push_back(import_info);
    
    if (pImpl->debug_mode) {
        std::cout << "添加导入: " << import_info.path << " (类型: " << static_cast<int>(import_info.type) << ")" << std::endl;
    }
    
    return true;
}

bool ImportManager::resolveImport(ImportInfo& import_info) {
    std::string resolved_path;
    
    switch (import_info.type) {
        case ImportType::HTML:
        case ImportType::STYLE:
        case ImportType::JAVASCRIPT:
            resolved_path = pImpl->resolveHtmlStyleJsPath(import_info.path, import_info.type);
            break;
        case ImportType::CHTL:
            resolved_path = pImpl->resolveChtlPath(import_info.path);
            break;
        case ImportType::CJMOD:
            resolved_path = pImpl->resolveCJmodPath(import_info.path);
            break;
        default:
            addError("未知的导入类型: " + std::to_string(static_cast<int>(import_info.type)));
            return false;
    }
    
    if (resolved_path.empty()) {
        addError("无法解析导入路径: " + import_info.path);
        return false;
    }
    
    import_info.resolved_path = resolved_path;
    import_info.is_resolved = true;
    
    if (pImpl->debug_mode) {
        std::cout << "解析导入: " << import_info.path << " -> " << resolved_path << std::endl;
    }
    
    return true;
}

bool ImportManager::resolveAllImports() {
    bool all_resolved = true;
    
    for (auto& import_info : pImpl->imports) {
        if (!import_info.is_resolved) {
            if (!resolveImport(import_info)) {
                all_resolved = false;
            }
        }
    }
    
    return all_resolved;
}

void ImportManager::clearImports() {
    pImpl->imports.clear();
    pImpl->import_cache.clear();
    pImpl->cache_timestamps.clear();
}

std::vector<ImportInfo> ImportManager::getImports() const {
    return pImpl->imports;
}

std::vector<ImportInfo> ImportManager::getImportsByType(ImportType type) const {
    std::vector<ImportInfo> result;
    for (const auto& import_info : pImpl->imports) {
        if (import_info.type == type) {
            result.push_back(import_info);
        }
    }
    return result;
}

std::vector<ImportInfo> ImportManager::getImportsByPath(const std::string& path) const {
    std::vector<ImportInfo> result;
    std::string normalized_path = pImpl->normalizePath(path);
    
    for (const auto& import_info : pImpl->imports) {
        if (pImpl->normalizePath(import_info.path) == normalized_path) {
            result.push_back(import_info);
        }
    }
    return result;
}

bool ImportManager::hasImport(const std::string& path, ImportType type) const {
    std::string normalized_path = pImpl->normalizePath(path);
    
    for (const auto& import_info : pImpl->imports) {
        if (pImpl->normalizePath(import_info.path) == normalized_path && import_info.type == type) {
            return true;
        }
    }
    return false;
}

bool ImportManager::detectCircularDependencies() {
    std::vector<std::vector<std::string>> circular_deps;
    
    for (const auto& import_info : pImpl->imports) {
        if (pImpl->checkCircularDependency(import_info.path, import_info.source_file)) {
            std::vector<std::string> cycle = {import_info.source_file, import_info.path};
            circular_deps.push_back(cycle);
        }
    }
    
    return !circular_deps.empty();
}

std::vector<std::vector<std::string>> ImportManager::getCircularDependencies() const {
    std::vector<std::vector<std::string>> circular_deps;
    
    for (const auto& import_info : pImpl->imports) {
        if (pImpl->checkCircularDependency(import_info.path, import_info.source_file)) {
            std::vector<std::string> cycle = {import_info.source_file, import_info.path};
            circular_deps.push_back(cycle);
        }
    }
    
    return circular_deps;
}

bool ImportManager::hasCircularDependencies() const {
    for (const auto& import_info : pImpl->imports) {
        if (pImpl->checkCircularDependency(import_info.path, import_info.source_file)) {
            return true;
        }
    }
    return false;
}

bool ImportManager::detectDuplicateImports() {
    std::unordered_set<std::string> seen_paths;
    std::vector<std::string> duplicates;
    
    for (const auto& import_info : pImpl->imports) {
        std::string key = import_info.path + ":" + std::to_string(static_cast<int>(import_info.type));
        if (seen_paths.find(key) != seen_paths.end()) {
            duplicates.push_back(import_info.path);
        } else {
            seen_paths.insert(key);
        }
    }
    
    return !duplicates.empty();
}

std::vector<std::string> ImportManager::getDuplicateImports() const {
    std::unordered_set<std::string> seen_paths;
    std::vector<std::string> duplicates;
    
    for (const auto& import_info : pImpl->imports) {
        std::string key = import_info.path + ":" + std::to_string(static_cast<int>(import_info.type));
        if (seen_paths.find(key) != seen_paths.end()) {
            duplicates.push_back(import_info.path);
        } else {
            seen_paths.insert(key);
        }
    }
    
    return duplicates;
}

bool ImportManager::hasDuplicateImports() const {
    std::unordered_set<std::string> seen_paths;
    
    for (const auto& import_info : pImpl->imports) {
        std::string key = import_info.path + ":" + std::to_string(static_cast<int>(import_info.type));
        if (seen_paths.find(key) != seen_paths.end()) {
            return true;
        } else {
            seen_paths.insert(key);
        }
    }
    
    return false;
}

std::string ImportManager::resolvePath(const std::string& path, ImportType type) {
    switch (type) {
        case ImportType::HTML:
        case ImportType::STYLE:
        case ImportType::JAVASCRIPT:
            return pImpl->resolveHtmlStyleJsPath(path, type);
        case ImportType::CHTL:
            return pImpl->resolveChtlPath(path);
        case ImportType::CJMOD:
            return pImpl->resolveCJmodPath(path);
        default:
            addError("未知的导入类型: " + std::to_string(static_cast<int>(type)));
            return "";
    }
}

std::vector<std::string> ImportManager::resolveWildcardPath(const std::string& path, ImportType type) {
    return pImpl->resolveWildcardPath(path, type);
}

std::vector<std::string> ImportManager::resolveSubmodulePath(const std::string& path, ImportType type) {
    return pImpl->resolveSubmodulePath(path, type);
}

std::string ImportManager::findFile(const std::string& filename, ImportType type) {
    std::vector<std::string> search_paths = pImpl->getSearchPaths(type);
    
    for (const auto& search_path : search_paths) {
        std::string found_file = pImpl->findFileInDirectory(search_path, filename, type);
        if (!found_file.empty()) {
            return found_file;
        }
    }
    
    return "";
}

std::vector<std::string> ImportManager::findFilesInDirectory(const std::string& directory, ImportType type) {
    if (!pImpl->isDirectory(directory)) {
        return {};
    }
    
    std::vector<std::string> files;
    std::vector<std::string> items = pImpl->listDirectory(directory);
    
    for (const auto& item : items) {
        std::string full_path = directory + "/" + item;
        if (pImpl->isFile(full_path)) {
            std::string extension = pImpl->getFileExtension(item);
            if (pImpl->isValidFileExtension(extension, type)) {
                files.push_back(full_path);
            }
        }
    }
    
    return files;
}

std::vector<std::string> ImportManager::findFilesByPattern(const std::string& pattern, ImportType type) {
    // 解析模式，支持通配符
    std::string directory = pattern.substr(0, pattern.find_last_of("/\\"));
    std::string filename_pattern = pattern.substr(pattern.find_last_of("/\\") + 1);
    
    if (directory.empty()) {
        directory = ".";
    }
    
    return pImpl->findFilesByPattern(directory, filename_pattern);
}

bool ImportManager::loadModule(const std::string& module_path) {
    // 检查模块是否已加载
    for (const auto& module : pImpl->modules) {
        if (module.path == module_path) {
            return true;
        }
    }
    
    // 尝试加载模块
    if (pImpl->isFile(module_path)) {
        std::string extension = pImpl->getFileExtension(module_path);
        std::string name = module_path.substr(module_path.find_last_of("/\\") + 1);
        
        ModuleInfo module_info(name, module_path, extension);
        pImpl->modules.push_back(module_info);
        
        if (pImpl->debug_mode) {
            std::cout << "加载模块: " << name << " (" << module_path << ")" << std::endl;
        }
        
        return true;
    }
    
    addError("无法加载模块: " + module_path);
    return false;
}

std::vector<ModuleInfo> ImportManager::getModules() const {
    return pImpl->modules;
}

ModuleInfo* ImportManager::getModule(const std::string& name) {
    for (auto& module : pImpl->modules) {
        if (module.name == name) {
            return &module;
        }
    }
    return nullptr;
}

void ImportManager::setConfig(const ImportManagerConfig& config) {
    pImpl->config = config;
}

ImportManagerConfig ImportManager::getConfig() const {
    return pImpl->config;
}

bool ImportManager::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> ImportManager::getErrors() const {
    return pImpl->errors;
}

void ImportManager::clearErrors() {
    pImpl->errors.clear();
}

void ImportManager::addError(const std::string& error_message) {
    pImpl->errors.push_back(error_message);
    
    if (pImpl->debug_mode) {
        std::cerr << "Import管理器错误: " << error_message << std::endl;
    }
}

std::string ImportManager::getDebugInfo() const {
    std::ostringstream oss;
    oss << "Import管理器调试信息:\n";
    oss << "导入数量: " << pImpl->imports.size() << "\n";
    oss << "模块数量: " << pImpl->modules.size() << "\n";
    oss << "错误数量: " << pImpl->errors.size() << "\n";
    oss << "缓存大小: " << pImpl->import_cache.size() << "\n";
    oss << "官方模块路径: " << pImpl->config.official_module_path << "\n";
    oss << "本地模块路径: " << pImpl->config.local_module_path << "\n";
    oss << "当前目录: " << pImpl->config.current_directory << "\n";
    return oss.str();
}

std::string ImportManager::getImportGraph() const {
    std::ostringstream oss;
    oss << "导入关系图:\n";
    
    for (const auto& import_info : pImpl->imports) {
        oss << "  " << import_info.source_file << " -> " << import_info.path;
        if (import_info.is_resolved) {
            oss << " (" << import_info.resolved_path << ")";
        }
        oss << "\n";
    }
    
    return oss.str();
}

void ImportManager::enableDebugMode(bool enable) {
    pImpl->debug_mode = enable;
}

// Impl类方法实现
bool ImportManager::Impl::validateImport(const ImportInfo& import_info) {
    if (import_info.path.empty()) {
        return false;
    }
    
    if (import_info.type == ImportType::UNKNOWN) {
        return false;
    }
    
    return true;
}

bool ImportManager::Impl::checkCircularDependency(const std::string& path, const std::string& current_path) {
    // 简单的循环依赖检测
    if (path == current_path) {
        return true;
    }
    
    // 检查路径是否包含当前文件
    if (path.find(current_path) != std::string::npos) {
        return true;
    }
    
    return false;
}

bool ImportManager::Impl::checkDuplicateImport(const ImportInfo& import_info) {
    for (const auto& existing_import : imports) {
        if (existing_import.path == import_info.path && 
            existing_import.type == import_info.type &&
            existing_import.source_file == import_info.source_file) {
            return true;
        }
    }
    return false;
}

std::string ImportManager::Impl::normalizePath(const std::string& path) {
    std::string normalized = path;
    
    // 替换反斜杠为正斜杠
    std::replace(normalized.begin(), normalized.end(), '\\', '/');
    
    // 移除多余的斜杠
    std::regex multiple_slashes("//+");
    normalized = std::regex_replace(normalized, multiple_slashes, "/");
    
    // 移除开头的斜杠
    if (!normalized.empty() && normalized[0] == '/') {
        normalized = normalized.substr(1);
    }
    
    return normalized;
}

std::string ImportManager::Impl::getFileExtension(const std::string& filename) {
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos != std::string::npos) {
        return filename.substr(dot_pos + 1);
    }
    return "";
}

bool ImportManager::Impl::isValidFileExtension(const std::string& extension, ImportType type) {
    switch (type) {
        case ImportType::HTML:
            return extension == "html" || extension == "htm";
        case ImportType::STYLE:
            return extension == "css";
        case ImportType::JAVASCRIPT:
            return extension == "js";
        case ImportType::CHTL:
            return extension == "chtl" || extension == "cmod";
        case ImportType::CJMOD:
            return extension == "cjmod";
        default:
            return false;
    }
}

std::vector<std::string> ImportManager::Impl::getSearchPaths(ImportType type) {
    std::vector<std::string> paths;
    
    switch (type) {
        case ImportType::HTML:
        case ImportType::STYLE:
        case ImportType::JAVASCRIPT:
            // 对于HTML/Style/JS，只在当前目录搜索
            paths.push_back(config.current_directory);
            break;
            
        case ImportType::CHTL:
        case ImportType::CJMOD:
            // 对于CHTL/CJMOD，按优先级搜索
            paths.push_back(config.official_module_path);
            paths.push_back(config.current_directory + "/" + config.local_module_path);
            paths.push_back(config.current_directory);
            break;
            
        default:
            paths.push_back(config.current_directory);
            break;
    }
    
    return paths;
}

bool ImportManager::Impl::isOfficialModule(const std::string& path) {
    return path.find(config.official_module_path) != std::string::npos;
}

bool ImportManager::Impl::isLocalModule(const std::string& path) {
    return path.find(config.local_module_path) != std::string::npos;
}

void ImportManager::Impl::updateImportCache() {
    auto now = std::chrono::system_clock::now();
    
    for (auto it = cache_timestamps.begin(); it != cache_timestamps.end();) {
        if (now - it->second > config.cache_timeout) {
            import_cache.erase(it->first);
            it = cache_timestamps.erase(it);
        } else {
            ++it;
        }
    }
}

void ImportManager::Impl::cleanupCache() {
    import_cache.clear();
    cache_timestamps.clear();
}

// 路径解析方法实现
std::string ImportManager::Impl::resolveHtmlStyleJsPath(const std::string& path, ImportType type) {
    std::string normalized_path = normalizePath(path);
    
    // 检查是否是文件夹（报错）
    if (isDirectory(normalized_path)) {
        return "";
    }
    
    // 如果路径是文件名（不带后缀名），在当前目录搜索相关文件
    if (getFileExtension(normalized_path).empty()) {
        std::vector<std::string> search_paths = getSearchPaths(type);
        for (const auto& search_path : search_paths) {
            std::string found_file = findFileInDirectory(search_path, normalized_path, type);
            if (!found_file.empty()) {
                return found_file;
            }
        }
    } else {
        // 如果路径是具体文件名（带后缀名），直接在当前目录搜索
        std::vector<std::string> search_paths = getSearchPaths(type);
        for (const auto& search_path : search_paths) {
            std::string full_path = search_path + "/" + normalized_path;
            if (isFile(full_path)) {
                return full_path;
            }
        }
    }
    
    return "";
}

std::string ImportManager::Impl::resolveChtlPath(const std::string& path) {
    std::string normalized_path = normalizePath(path);
    
    // 检查通配符导入
    if (normalized_path.find("*") != std::string::npos) {
        return ""; // 通配符导入需要特殊处理
    }
    
    // 检查子模块导入
    if (normalized_path.find(".") != std::string::npos) {
        return ""; // 子模块导入需要特殊处理
    }
    
    // 如果写的只是名称（不带后缀名）
    if (getFileExtension(normalized_path).empty()) {
        std::vector<std::string> search_paths = getSearchPaths(ImportType::CHTL);
        for (const auto& search_path : search_paths) {
            // 优先搜索cmod文件
            std::string cmod_file = findFileInDirectory(search_path, normalized_path + ".cmod", ImportType::CHTL);
            if (!cmod_file.empty()) {
                return cmod_file;
            }
            
            // 然后搜索chtl文件
            std::string chtl_file = findFileInDirectory(search_path, normalized_path + ".chtl", ImportType::CHTL);
            if (!chtl_file.empty()) {
                return chtl_file;
            }
        }
    } else {
        // 如果写了具体的名称（带后缀名）
        std::vector<std::string> search_paths = getSearchPaths(ImportType::CHTL);
        for (const auto& search_path : search_paths) {
            std::string full_path = search_path + "/" + normalized_path;
            if (isFile(full_path)) {
                return full_path;
            }
        }
    }
    
    return "";
}

std::string ImportManager::Impl::resolveCJmodPath(const std::string& path) {
    std::string normalized_path = normalizePath(path);
    
    // 如果写的只是名称（不带后缀名）
    if (getFileExtension(normalized_path).empty()) {
        std::vector<std::string> search_paths = getSearchPaths(ImportType::CJMOD);
        for (const auto& search_path : search_paths) {
            std::string cjmod_file = findFileInDirectory(search_path, normalized_path + ".cjmod", ImportType::CJMOD);
            if (!cjmod_file.empty()) {
                return cjmod_file;
            }
        }
    } else {
        // 如果写了具体的名称（带后缀名）
        std::vector<std::string> search_paths = getSearchPaths(ImportType::CJMOD);
        for (const auto& search_path : search_paths) {
            std::string full_path = search_path + "/" + normalized_path;
            if (isFile(full_path)) {
                return full_path;
            }
        }
    }
    
    return "";
}

std::vector<std::string> ImportManager::Impl::resolveWildcardPath(const std::string& path, ImportType type) {
    std::vector<std::string> result;
    std::string normalized_path = normalizePath(path);
    
    // 移除通配符部分
    size_t wildcard_pos = normalized_path.find("*");
    if (wildcard_pos == std::string::npos) {
        return result;
    }
    
    std::string directory = normalized_path.substr(0, wildcard_pos);
    std::string pattern = normalized_path.substr(wildcard_pos);
    
    // 搜索匹配的文件
    if (isDirectory(directory)) {
        result = findFilesByPattern(directory, pattern);
    }
    
    return result;
}

std::vector<std::string> ImportManager::Impl::resolveSubmodulePath(const std::string& path, ImportType type) {
    std::vector<std::string> result;
    std::string normalized_path = normalizePath(path);
    
    // 分割模块路径
    std::vector<std::string> parts = splitNamespacePath(normalized_path);
    if (parts.size() < 2) {
        return result;
    }
    
    std::string module_name = parts[0];
    std::string submodule_name = parts[1];
    
    // 搜索模块
    std::vector<std::string> search_paths = getSearchPaths(type);
    for (const auto& search_path : search_paths) {
        std::string module_path = search_path + "/" + module_name;
        if (isDirectory(module_path)) {
            std::vector<std::string> submodules = listDirectory(module_path);
            for (const auto& submodule : submodules) {
                if (submodule == submodule_name || submodule.find(submodule_name) != std::string::npos) {
                    result.push_back(module_path + "/" + submodule);
                }
            }
        }
    }
    
    return result;
}

std::string ImportManager::Impl::findFileInDirectory(const std::string& directory, const std::string& filename, ImportType type) {
    if (!isDirectory(directory)) {
        return "";
    }
    
    std::vector<std::string> items = listDirectory(directory);
    
    for (const auto& item : items) {
        if (item == filename) {
            std::string full_path = directory + "/" + item;
            if (isFile(full_path)) {
                return full_path;
            }
        }
    }
    
    // 如果没有找到完全匹配的文件，尝试添加扩展名
    std::string extension = getFileExtension(filename);
    if (extension.empty()) {
        std::vector<std::string> extensions;
        switch (type) {
            case ImportType::HTML:
                extensions = {"html", "htm"};
                break;
            case ImportType::STYLE:
                extensions = {"css"};
                break;
            case ImportType::JAVASCRIPT:
                extensions = {"js"};
                break;
            case ImportType::CHTL:
                extensions = {"chtl", "cmod"};
                break;
            case ImportType::CJMOD:
                extensions = {"cjmod"};
                break;
            default:
                break;
        }
        
        for (const auto& ext : extensions) {
            std::string filename_with_ext = filename + "." + ext;
            for (const auto& item : items) {
                if (item == filename_with_ext) {
                    std::string full_path = directory + "/" + item;
                    if (isFile(full_path)) {
                        return full_path;
                    }
                }
            }
        }
    }
    
    return "";
}

std::vector<std::string> ImportManager::Impl::findFilesByPattern(const std::string& directory, const std::string& pattern) {
    std::vector<std::string> result;
    
    if (!isDirectory(directory)) {
        return result;
    }
    
    std::vector<std::string> items = listDirectory(directory);
    
    // 简单的通配符匹配
    for (const auto& item : items) {
        if (pattern == "*" || pattern == "*.*") {
            // 匹配所有文件
            std::string full_path = directory + "/" + item;
            if (isFile(full_path)) {
                result.push_back(full_path);
            }
        } else if (pattern.find("*.") == 0) {
            // 匹配特定扩展名
            std::string ext = pattern.substr(2);
            std::string item_ext = getFileExtension(item);
            if (item_ext == ext) {
                std::string full_path = directory + "/" + item;
                if (isFile(full_path)) {
                    result.push_back(full_path);
                }
            }
        }
    }
    
    return result;
}

bool ImportManager::Impl::isDirectory(const std::string& path) {
    try {
        return std::filesystem::is_directory(path);
    } catch (...) {
        return false;
    }
}

bool ImportManager::Impl::isFile(const std::string& path) {
    try {
        return std::filesystem::is_regular_file(path);
    } catch (...) {
        return false;
    }
}

std::vector<std::string> ImportManager::Impl::listDirectory(const std::string& directory) {
    std::vector<std::string> result;
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            result.push_back(entry.path().filename().string());
        }
    } catch (...) {
        // 忽略错误
    }
    
    return result;
}

std::vector<std::string> ImportManager::Impl::splitNamespacePath(const std::string& path) {
    std::vector<std::string> result;
    std::string current;
    
    for (char c : path) {
        if (c == '.' || c == '/') {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    
    if (!current.empty()) {
        result.push_back(current);
    }
    
    return result;
}

} // namespace chtl