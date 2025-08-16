#include "module/ModuleManager.h"
#include "compression/SimpleZip.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <chrono>

namespace chtl {

// 实现类
class ModuleManager::Impl {
public:
    ModuleManagerConfig config;
    std::vector<ModuleInfo> modules;
    std::vector<ModuleContent> module_contents;
    std::vector<std::string> errors;
    std::unordered_map<std::string, ModuleContent> content_cache;
    std::unordered_map<std::string, std::chrono::system_clock::time_point> cache_timestamps;
    std::unordered_set<std::string> official_modules;
    bool debug_mode;
    
    Impl(const ModuleManagerConfig& cfg) : config(cfg), debug_mode(false) {
        // 初始化官方模块前缀
        official_modules.insert("chtl");
    }
    
    ~Impl() = default;
    
    // 内部方法实现
    bool validateModulePath(const std::string& module_path);
    bool validateModuleFormat(const std::string& module_path, ModuleType type);
    std::string calculateModuleChecksum(const std::string& module_path);
    bool isModuleCorrupted(const std::string& module_path);
    void updateModuleCache();
    void cleanupCache();
    bool checkModuleCompatibility(const ModuleInfo& module_info);
    std::string getModuleTypeString(ModuleType type);
    ModuleType getModuleTypeFromString(const std::string& type_string);
    
    // 模块打包和解包方法
    bool packCmodDirectory(const std::string& source_directory, const std::string& output_path);
    bool packCJmodDirectory(const std::string& source_directory, const std::string& output_path);
    bool unpackCmodArchive(const std::string& module_path, const std::string& output_directory);
    bool unpackCJmodArchive(const std::string& module_path, const std::string& output_directory);
    
    // 模块内容解析
    bool parseModuleManifest(const std::string& manifest_content, ModuleContent& content);
    bool validateModuleContent(const ModuleContent& content);
    std::string generateModuleManifest(const ModuleContent& content);
    
    // 文件操作
    std::vector<std::string> scanDirectory(const std::string& directory);
    bool copyFile(const std::string& source, const std::string& destination);
    bool createDirectory(const std::string& path);
    bool isDirectory(const std::string& path);
    bool isFile(const std::string& path);
    size_t getFileSize(const std::string& file_path);
};

// ModuleManager实现
ModuleManager::ModuleManager(const ModuleManagerConfig& config) 
    : pImpl(std::make_unique<Impl>(config)) {}

ModuleManager::~ModuleManager() = default;

bool ModuleManager::loadModule(const std::string& module_path) {
    if (!pImpl->validateModulePath(module_path)) {
        addError("无效的模块路径: " + module_path);
        return false;
    }
    
    // 检查模块是否已加载
    for (const auto& module : pImpl->modules) {
        if (module.path == module_path) {
            if (module.is_loaded) {
                if (pImpl->debug_mode) {
                    std::cout << "模块已加载: " << module.name << std::endl;
                }
                return true;
            }
        }
    }
    
    // 验证模块格式
    ModuleType module_type = pImpl->getModuleTypeFromString(module_path);
    if (module_type == ModuleType::UNKNOWN) {
        addError("无法确定模块类型: " + module_path);
        return false;
    }
    
    if (!pImpl->validateModuleFormat(module_path, module_type)) {
        addError("模块格式验证失败: " + module_path);
        return false;
    }
    
    // 检查模块是否损坏
    if (pImpl->isModuleCorrupted(module_path)) {
        addError("模块文件已损坏: " + module_path);
        return false;
    }
    
    // 创建模块信息
    std::string module_name = std::filesystem::path(module_path).stem().string();
    ModuleInfo module_info(module_name, module_path, module_type);
    module_info.status = ModuleStatus::LOADING;
    module_info.file_size = pImpl->getFileSize(module_path);
    module_info.checksum = pImpl->calculateModuleChecksum(module_path);
    module_info.is_official = pImpl->official_modules.find(module_name) != pImpl->official_modules.end();
    
    // 加载模块内容
    if (module_type == ModuleType::CMOD) {
        if (!pImpl->unpackCmodArchive(module_path, pImpl->config.temp_directory + "/" + module_name)) {
            addError("无法解包Cmod模块: " + module_path);
            return false;
        }
    } else if (module_type == ModuleType::CJMOD) {
        if (!pImpl->unpackCJmodArchive(module_path, pImpl->config.temp_directory + "/" + module_name)) {
            addError("无法解包CJmod模块: " + module_path);
            return false;
        }
    }
    
    // 解析模块清单
    std::string manifest_path = pImpl->config.temp_directory + "/" + module_name + "/manifest.json";
    if (std::filesystem::exists(manifest_path)) {
        std::ifstream manifest_file(manifest_path);
        if (manifest_file.is_open()) {
            std::string manifest_content((std::istreambuf_iterator<char>(manifest_file)),
                                       std::istreambuf_iterator<char>());
            manifest_file.close();
            
            ModuleContent content;
            if (pImpl->parseModuleManifest(manifest_content, content)) {
                pImpl->module_contents.push_back(content);
                module_info.version = content.version;
                module_info.description = content.description;
                module_info.author = content.author;
                module_info.license = content.license;
                module_info.dependencies = content.dependencies;
                module_info.exports = content.exports;
                module_info.imports = content.imports;
            }
        }
    }
    
    // 完成加载
    module_info.status = ModuleStatus::LOADED;
    module_info.is_loaded = true;
    module_info.loaded_time = std::chrono::system_clock::now();
    
    pImpl->modules.push_back(module_info);
    
    if (pImpl->debug_mode) {
        std::cout << "模块加载成功: " << module_name << " (" << module_path << ")" << std::endl;
    }
    
    return true;
}

bool ModuleManager::unloadModule(const std::string& module_name) {
    for (auto it = pImpl->modules.begin(); it != pImpl->modules.end(); ++it) {
        if (it->name == module_name) {
            if (it->is_loaded) {
                it->status = ModuleStatus::UNLOADED;
                it->is_loaded = false;
                
                // 清理临时文件
                std::string temp_dir = pImpl->config.temp_directory + "/" + module_name;
                if (std::filesystem::exists(temp_dir)) {
                    std::filesystem::remove_all(temp_dir);
                }
                
                if (pImpl->debug_mode) {
                    std::cout << "模块卸载成功: " << module_name << std::endl;
                }
                return true;
            }
        }
    }
    
    addError("模块未找到或未加载: " + module_name);
    return false;
}

bool ModuleManager::reloadModule(const std::string& module_name) {
    for (auto& module : pImpl->modules) {
        if (module.name == module_name) {
            std::string module_path = module.path;
            if (unloadModule(module_name)) {
                return loadModule(module_path);
            }
        }
    }
    
    addError("模块未找到: " + module_name);
    return false;
}

void ModuleManager::unloadAllModules() {
    for (auto& module : pImpl->modules) {
        if (module.is_loaded) {
            module.status = ModuleStatus::UNLOADED;
            module.is_loaded = false;
            
            // 清理临时文件
            std::string temp_dir = pImpl->config.temp_directory + "/" + module.name;
            if (std::filesystem::exists(temp_dir)) {
                std::filesystem::remove_all(temp_dir);
            }
        }
    }
    
    if (pImpl->debug_mode) {
        std::cout << "所有模块已卸载" << std::endl;
    }
}

std::vector<ModuleInfo> ModuleManager::getModules() const {
    return pImpl->modules;
}

std::vector<ModuleInfo> ModuleManager::getModulesByType(ModuleType type) const {
    std::vector<ModuleInfo> result;
    for (const auto& module : pImpl->modules) {
        if (module.type == type) {
            result.push_back(module);
        }
    }
    return result;
}

std::vector<ModuleInfo> ModuleManager::getOfficialModules() const {
    std::vector<ModuleInfo> result;
    for (const auto& module : pImpl->modules) {
        if (module.is_official) {
            result.push_back(module);
        }
    }
    return result;
}

std::vector<ModuleInfo> ModuleManager::getLocalModules() const {
    std::vector<ModuleInfo> result;
    for (const auto& module : pImpl->modules) {
        if (!module.is_official) {
            result.push_back(module);
        }
    }
    return result;
}

ModuleInfo* ModuleManager::getModule(const std::string& name) {
    for (auto& module : pImpl->modules) {
        if (module.name == name) {
            return &module;
        }
    }
    return nullptr;
}

bool ModuleManager::hasModule(const std::string& name) const {
    for (const auto& module : pImpl->modules) {
        if (module.name == name) {
            return true;
        }
    }
    return false;
}

bool ModuleManager::packModule(const std::string& source_directory, const std::string& output_path, ModuleType type) {
    if (!std::filesystem::exists(source_directory)) {
        addError("源目录不存在: " + source_directory);
        return false;
    }
    
    if (!std::filesystem::is_directory(source_directory)) {
        addError("源路径不是目录: " + source_directory);
        return false;
    }
    
    switch (type) {
        case ModuleType::CMOD:
            return pImpl->packCmodDirectory(source_directory, output_path);
        case ModuleType::CJMOD:
            return pImpl->packCJmodDirectory(source_directory, output_path);
        default:
            addError("不支持的模块类型: " + std::to_string(static_cast<int>(type)));
            return false;
    }
}

bool ModuleManager::packCmodModule(const std::string& source_directory, const std::string& output_path) {
    return pImpl->packCmodDirectory(source_directory, output_path);
}

bool ModuleManager::packCJmodModule(const std::string& source_directory, const std::string& output_path) {
    return pImpl->packCJmodDirectory(source_directory, output_path);
}

bool ModuleManager::unpackModule(const std::string& module_path, const std::string& output_directory) {
    if (!std::filesystem::exists(module_path)) {
        addError("模块文件不存在: " + module_path);
        return false;
    }
    
    ModuleType module_type = pImpl->getModuleTypeFromString(module_path);
    if (module_type == ModuleType::UNKNOWN) {
        addError("无法确定模块类型: " + module_path);
        return false;
    }
    
    switch (module_type) {
        case ModuleType::CMOD:
            return pImpl->unpackCmodArchive(module_path, output_directory);
        case ModuleType::CJMOD:
            return pImpl->unpackCJmodArchive(module_path, output_directory);
        default:
            addError("不支持的模块类型: " + std::to_string(static_cast<int>(module_type)));
            return false;
    }
}

bool ModuleManager::unpackCmodModule(const std::string& module_path, const std::string& output_directory) {
    return pImpl->unpackCmodArchive(module_path, output_directory);
}

bool ModuleManager::unpackCJmodModule(const std::string& module_path, const std::string& output_directory) {
    return pImpl->unpackCJmodArchive(module_path, output_directory);
}

bool ModuleManager::validateModule(const std::string& module_path) {
    if (!pImpl->validateModulePath(module_path)) {
        return false;
    }
    
    ModuleType module_type = pImpl->getModuleTypeFromString(module_path);
    if (module_type == ModuleType::UNKNOWN) {
        return false;
    }
    
    if (!pImpl->validateModuleFormat(module_path, module_type)) {
        return false;
    }
    
    if (pImpl->isModuleCorrupted(module_path)) {
        return false;
    }
    
    return true;
}

bool ModuleManager::validateModuleContent(const ModuleContent& content) {
    return pImpl->validateModuleContent(content);
}

std::vector<std::string> ModuleManager::getValidationErrors() const {
    return pImpl->errors;
}

ModuleContent* ModuleManager::getModuleContent(const std::string& module_name) {
    for (auto& content : pImpl->module_contents) {
        if (content.module_name == module_name) {
            return &content;
        }
    }
    return nullptr;
}

std::string ModuleManager::getModuleFile(const std::string& module_name, const std::string& file_path) {
    ModuleContent* content = getModuleContent(module_name);
    if (content && content->files.find(file_path) != content->files.end()) {
        return content->files[file_path];
    }
    return "";
}

std::vector<std::string> ModuleManager::getModuleFiles(const std::string& module_name) {
    std::vector<std::string> result;
    ModuleContent* content = getModuleContent(module_name);
    if (content) {
        for (const auto& file : content->files) {
            result.push_back(file.first);
        }
    }
    return result;
}

bool ModuleManager::registerOfficialModule(const std::string& module_path) {
    if (!std::filesystem::exists(module_path)) {
        addError("官方模块文件不存在: " + module_path);
        return false;
    }
    
    std::string module_name = std::filesystem::path(module_path).stem().string();
    pImpl->official_modules.insert(module_name);
    
    if (pImpl->debug_mode) {
        std::cout << "官方模块注册成功: " << module_name << std::endl;
    }
    
    return true;
}

bool ModuleManager::unregisterOfficialModule(const std::string& module_name) {
    auto it = pImpl->official_modules.find(module_name);
    if (it != pImpl->official_modules.end()) {
        pImpl->official_modules.erase(it);
        
        if (pImpl->debug_mode) {
            std::cout << "官方模块注销成功: " << module_name << std::endl;
        }
        return true;
    }
    
    addError("官方模块未找到: " + module_name);
    return false;
}

std::vector<std::string> ModuleManager::getOfficialModuleNames() const {
    std::vector<std::string> result(pImpl->official_modules.begin(), pImpl->official_modules.end());
    return result;
}

bool ModuleManager::isOfficialModule(const std::string& module_name) const {
    return pImpl->official_modules.find(module_name) != pImpl->official_modules.end();
}

bool ModuleManager::resolveDependencies(const std::string& module_name) {
    ModuleInfo* module = getModule(module_name);
    if (!module) {
        addError("模块未找到: " + module_name);
        return false;
    }
    
    for (const auto& dependency : module->dependencies) {
        if (!hasModule(dependency)) {
            // 尝试加载依赖模块
            std::string dependency_path = pImpl->config.official_module_path + "/" + dependency + ".cmod";
            if (!std::filesystem::exists(dependency_path)) {
                dependency_path = pImpl->config.local_module_path + "/" + dependency + ".cmod";
            }
            if (!std::filesystem::exists(dependency_path)) {
                dependency_path = dependency + ".cmod";
            }
            
            if (std::filesystem::exists(dependency_path)) {
                if (!loadModule(dependency_path)) {
                    addError("无法加载依赖模块: " + dependency);
                    return false;
                }
            } else {
                addError("依赖模块未找到: " + dependency);
                return false;
            }
        }
    }
    
    return true;
}

std::vector<std::string> ModuleManager::getModuleDependencies(const std::string& module_name) {
    ModuleInfo* module = getModule(module_name);
    if (module) {
        return module->dependencies;
    }
    return {};
}

bool ModuleManager::hasCircularDependencies() const {
    // 简单的循环依赖检测
    for (const auto& module : pImpl->modules) {
        for (const auto& dependency : module.dependencies) {
            if (dependency == module.name) {
                return true;
            }
        }
    }
    return false;
}

std::vector<std::vector<std::string>> ModuleManager::getCircularDependencies() const {
    std::vector<std::vector<std::string>> result;
    
    // 简单的循环依赖检测
    for (const auto& module : pImpl->modules) {
        for (const auto& dependency : module.dependencies) {
            if (dependency == module.name) {
                std::vector<std::string> cycle = {module.name, dependency};
                result.push_back(cycle);
            }
        }
    }
    
    return result;
}

void ModuleManager::setConfig(const ModuleManagerConfig& config) {
    pImpl->config = config;
}

ModuleManagerConfig ModuleManager::getConfig() const {
    return pImpl->config;
}

bool ModuleManager::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> ModuleManager::getErrors() const {
    return pImpl->errors;
}

void ModuleManager::clearErrors() {
    pImpl->errors.clear();
}

void ModuleManager::addError(const std::string& error_message) {
    pImpl->errors.push_back(error_message);
    
    if (pImpl->debug_mode) {
        std::cerr << "模块管理器错误: " << error_message << std::endl;
    }
}

std::string ModuleManager::getDebugInfo() const {
    std::ostringstream oss;
    oss << "模块管理器调试信息:\n";
    oss << "模块数量: " << pImpl->modules.size() << "\n";
    oss << "官方模块数量: " << pImpl->official_modules.size() << "\n";
    oss << "错误数量: " << pImpl->errors.size() << "\n";
    oss << "官方模块路径: " << pImpl->config.official_module_path << "\n";
    oss << "本地模块路径: " << pImpl->config.local_module_path << "\n";
    oss << "临时目录: " << pImpl->config.temp_directory << "\n";
    return oss.str();
}

std::string ModuleManager::getModuleGraph() const {
    std::ostringstream oss;
    oss << "模块依赖关系图:\n";
    
    for (const auto& module : pImpl->modules) {
        oss << "  " << module.name << " (" << (module.is_official ? "官方" : "本地") << ")\n";
        for (const auto& dependency : module.dependencies) {
            oss << "    -> " << dependency << "\n";
        }
    }
    
    return oss.str();
}

std::string ModuleManager::getModuleReport() const {
    std::ostringstream oss;
    oss << "模块状态报告:\n";
    
    for (const auto& module : pImpl->modules) {
        oss << "  " << module.name << ":\n";
        oss << "    状态: " << static_cast<int>(module.status) << "\n";
        oss << "    类型: " << static_cast<int>(module.type) << "\n";
        oss << "    版本: " << module.version << "\n";
        oss << "    大小: " << module.file_size << " 字节\n";
        oss << "    已加载: " << (module.is_loaded ? "是" : "否") << "\n";
        oss << "    官方模块: " << (module.is_official ? "是" : "否") << "\n";
    }
    
    return oss.str();
}

void ModuleManager::enableDebugMode(bool enable) {
    pImpl->debug_mode = enable;
}

// Impl类方法实现
bool ModuleManager::Impl::validateModulePath(const std::string& module_path) {
    return std::filesystem::exists(module_path) && std::filesystem::is_regular_file(module_path);
}

bool ModuleManager::Impl::validateModuleFormat(const std::string& module_path, ModuleType type) {
    // 检查文件扩展名
    std::string extension = std::filesystem::path(module_path).extension().string();
    
    switch (type) {
        case ModuleType::CMOD:
            return extension == ".cmod";
        case ModuleType::CJMOD:
            return extension == ".cjmod";
        default:
            return false;
    }
}

std::string ModuleManager::Impl::calculateModuleChecksum(const std::string& module_path) {
    // 简单的校验和计算（实际应用中应使用更安全的哈希算法）
    std::ifstream file(module_path, std::ios::binary);
    if (!file.is_open()) {
        return "";
    }
    
    uint32_t checksum = 0;
    char byte;
    while (file.get(byte)) {
        checksum += static_cast<uint8_t>(byte);
    }
    
    std::ostringstream oss;
    oss << std::hex << checksum;
    return oss.str();
}

bool ModuleManager::Impl::isModuleCorrupted(const std::string& module_path) {
    // 简单的损坏检测
    std::ifstream file(module_path, std::ios::binary);
    if (!file.is_open()) {
        return true;
    }
    
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    
    if (file_size < 100) { // 最小文件大小检查
        return true;
    }
    
    return false;
}

void ModuleManager::Impl::updateModuleCache() {
    auto now = std::chrono::system_clock::now();
    
    for (auto it = cache_timestamps.begin(); it != cache_timestamps.end();) {
        if (now - it->second > config.cache_timeout) {
            content_cache.erase(it->first);
            it = cache_timestamps.erase(it);
        } else {
            ++it;
        }
    }
}

void ModuleManager::Impl::cleanupCache() {
    content_cache.clear();
    cache_timestamps.clear();
}

bool ModuleManager::Impl::checkModuleCompatibility(const ModuleInfo& module_info) {
    // 简单的兼容性检查
    return module_info.is_valid && module_info.status != ModuleStatus::CORRUPTED;
}

std::string ModuleManager::Impl::getModuleTypeString(ModuleType type) {
    switch (type) {
        case ModuleType::CMOD:
            return "cmod";
        case ModuleType::CJMOD:
            return "cjmod";
        default:
            return "unknown";
    }
}

ModuleType ModuleManager::Impl::getModuleTypeFromString(const std::string& module_path) {
    std::string extension = std::filesystem::path(module_path).extension().string();
    
    if (extension == ".cmod") {
        return ModuleType::CMOD;
    } else if (extension == ".cjmod") {
        return ModuleType::CJMOD;
    } else {
        return ModuleType::UNKNOWN;
    }
}

// 模块打包和解包方法实现
bool ModuleManager::Impl::packCmodDirectory(const std::string& source_directory, const std::string& output_path) {
    try {
        // 创建ZIP文件
        SimpleZip zip;
        if (!zip.createZip(output_path)) {
            return false;
        }
        
        // 扫描目录
        std::vector<std::string> files = scanDirectory(source_directory);
        
        // 添加文件到ZIP
        for (const auto& file : files) {
            std::string relative_path = std::filesystem::relative(file, source_directory).string();
            if (!zip.addFile(file, relative_path)) {
                return false;
            }
        }
        
        // 关闭ZIP文件
        if (!zip.closeZip()) {
            return false;
        }
        
        return true;
    } catch (...) {
        return false;
    }
}

bool ModuleManager::Impl::packCJmodDirectory(const std::string& source_directory, const std::string& output_path) {
    try {
        // 创建ZIP文件
        SimpleZip zip;
        if (!zip.createZip(output_path)) {
            return false;
        }
        
        // 扫描目录
        std::vector<std::string> files = scanDirectory(source_directory);
        
        // 添加文件到ZIP
        for (const auto& file : files) {
            std::string relative_path = std::filesystem::relative(file, source_directory).string();
            if (!zip.addFile(file, relative_path)) {
                return false;
            }
        }
        
        // 关闭ZIP文件
        if (!zip.closeZip()) {
            return false;
        }
        
        return true;
    } catch (...) {
        return false;
    }
}

bool ModuleManager::Impl::unpackCmodArchive(const std::string& module_path, const std::string& output_directory) {
    try {
        // 创建输出目录
        if (!createDirectory(output_directory)) {
            return false;
        }
        
        // 打开ZIP文件
        SimpleZip zip;
        if (!zip.openZip(module_path)) {
            return false;
        }
        
        // 解压所有文件
        if (!zip.extractAll(output_directory)) {
            return false;
        }
        
        // 关闭ZIP文件
        zip.closeZip();
        
        return true;
    } catch (...) {
        return false;
    }
}

bool ModuleManager::Impl::unpackCJmodArchive(const std::string& module_path, const std::string& output_directory) {
    try {
        // 创建输出目录
        if (!createDirectory(output_directory)) {
            return false;
        }
        
        // 打开ZIP文件
        SimpleZip zip;
        if (!zip.openZip(module_path)) {
            return false;
        }
        
        // 解压所有文件
        if (!zip.extractAll(output_directory)) {
            return false;
        }
        
        // 关闭ZIP文件
        zip.closeZip();
        
        return true;
    } catch (...) {
        return false;
    }
}

// 模块内容解析
bool ModuleManager::Impl::parseModuleManifest(const std::string& manifest_content, ModuleContent& content) {
    // 简单的JSON解析（实际应用中应使用JSON库）
    // 这里只是占位符实现
    content.module_name = "unknown";
    content.module_type = "unknown";
    content.version = "1.0.0";
    content.description = "Module description";
    content.author = "Unknown";
    content.license = "MIT";
    
    return true;
}

bool ModuleManager::Impl::validateModuleContent(const ModuleContent& content) {
    return !content.module_name.empty() && !content.module_type.empty();
}

std::string ModuleManager::Impl::generateModuleManifest(const ModuleContent& content) {
    // 简单的JSON生成（实际应用中应使用JSON库）
    std::ostringstream oss;
    oss << "{\n";
    oss << "  \"name\": \"" << content.module_name << "\",\n";
    oss << "  \"type\": \"" << content.module_type << "\",\n";
    oss << "  \"version\": \"" << content.version << "\",\n";
    oss << "  \"description\": \"" << content.description << "\",\n";
    oss << "  \"author\": \"" << content.author << "\",\n";
    oss << "  \"license\": \"" << content.license << "\"\n";
    oss << "}\n";
    return oss.str();
}

// 文件操作
std::vector<std::string> ModuleManager::Impl::scanDirectory(const std::string& directory) {
    std::vector<std::string> result;
    
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                result.push_back(entry.path().string());
            }
        }
    } catch (...) {
        // 忽略错误
    }
    
    return result;
}

bool ModuleManager::Impl::copyFile(const std::string& source, const std::string& destination) {
    try {
        std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing);
        return true;
    } catch (...) {
        return false;
    }
}

bool ModuleManager::Impl::createDirectory(const std::string& path) {
    try {
        return std::filesystem::create_directories(path);
    } catch (...) {
        return false;
    }
}

bool ModuleManager::Impl::isDirectory(const std::string& path) {
    try {
        return std::filesystem::is_directory(path);
    } catch (...) {
        return false;
    }
}

bool ModuleManager::Impl::isFile(const std::string& path) {
    try {
        return std::filesystem::is_regular_file(path);
    } catch (...) {
        return false;
    }
}

size_t ModuleManager::Impl::getFileSize(const std::string& file_path) {
    try {
        return std::filesystem::file_size(file_path);
    } catch (...) {
        return 0;
    }
}

} // namespace chtl