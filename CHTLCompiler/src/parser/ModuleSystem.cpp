#include "parser/ModuleSystem.h"
#include "statemachine/CHTLStateMachine.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <filesystem>

namespace chtl {

class ModuleSystem::Impl {
public:
    ModuleSystemConfig config;
    std::unordered_map<std::string, ModuleSystemInfo> modules;
    std::vector<std::string> errors;
    CHTLStateMachine* state_machine;
    
    Impl(const ModuleSystemConfig& cfg) : config(cfg), state_machine(nullptr) {}
    
    // 私有辅助方法
    bool isValidModuleName(const std::string& name);
    bool isValidModuleFile(const std::string& file_path);
    bool isValidModuleSize(size_t size);
    bool hasValidModuleStructure(const ModuleSystemInfo& module_info);
    ModuleType detectModuleType(const std::string& file_path);
    std::string normalizeModulePath(const std::string& file_path);
    std::string extractModuleExtension(const std::string& file_path);
    bool moduleFileExists(const std::string& file_path);
};

ModuleSystem::ModuleSystem(const ModuleSystemConfig& config)
    : pImpl(std::make_unique<Impl>(config)) {}

ModuleSystem::~ModuleSystem() = default;

bool ModuleSystem::loadModule(const std::string& module_name) {
    if (module_name.empty()) {
        addError("Module name cannot be empty");
        return false;
    }
    
    if (isModuleLoaded(module_name)) {
        addError("Module '" + module_name + "' is already loaded");
        return false;
    }
    
    // 创建模块信息
    ModuleSystemInfo module_info;
    module_info.name = module_name;
    module_info.file_path = pImpl->normalizeModulePath(module_name);
    module_info.type = pImpl->detectModuleType(module_info.file_path);
    module_info.status = ModuleStatus::LOADING;
    
    // 检查文件是否存在
    if (!pImpl->moduleFileExists(module_info.file_path)) {
        addError("Module file not found: " + module_info.file_path);
        return false;
    }
    
    // 加载模块内容
    if (!loadModuleContent(module_info)) {
        addError("Failed to load module content: " + module_name);
        return false;
    }
    
    // 验证模块
    if (!validateModule(module_info)) {
        addError("Module validation failed: " + module_name);
        return false;
    }
    
    // 添加到模块列表
    module_info.status = ModuleStatus::LOADED;
    addModule(module_info);
    
    return true;
}

bool ModuleSystem::unloadModule(const std::string& module_name) {
    if (module_name.empty()) {
        addError("Module name cannot be empty");
        return false;
    }
    
    if (!isModuleLoaded(module_name)) {
        addError("Module '" + module_name + "' is not loaded");
        return false;
    }
    
    auto* module_info = getModule(module_name);
    if (module_info) {
        module_info->status = ModuleStatus::UNLOADING;
        // 清理模块内容
        module_info->content = ModuleContent{};
        module_info->status = ModuleStatus::UNLOADED;
    }
    
    removeModule(module_name);
    return true;
}

bool ModuleSystem::reloadModule(const std::string& module_name) {
    if (module_name.empty()) {
        addError("Module name cannot be empty");
        return false;
    }
    
    if (!isModuleLoaded(module_name)) {
        return loadModule(module_name);
    }
    
    auto* module_info = getModule(module_name);
    if (!module_info) {
        addError("Module not found: " + module_name);
        return false;
    }
    
    // 卸载模块
    if (!unloadModule(module_name)) {
        return false;
    }
    
    // 重新加载模块
    return loadModule(module_name);
}

bool ModuleSystem::isModuleLoaded(const std::string& module_name) const {
    return pImpl->modules.find(module_name) != pImpl->modules.end() &&
           pImpl->modules.at(module_name).status == ModuleStatus::LOADED;
}

void ModuleSystem::addModule(const ModuleSystemInfo& module_info) {
    pImpl->modules[module_info.name] = module_info;
}

void ModuleSystem::removeModule(const std::string& module_name) {
    pImpl->modules.erase(module_name);
}

ModuleSystemInfo* ModuleSystem::getModule(const std::string& name) {
    auto it = pImpl->modules.find(name);
    return it != pImpl->modules.end() ? &it->second : nullptr;
}

std::vector<ModuleSystemInfo> ModuleSystem::getAllModules() const {
    std::vector<ModuleSystemInfo> result;
    result.reserve(pImpl->modules.size());
    for (const auto& pair : pImpl->modules) {
        result.push_back(pair.second);
    }
    return result;
}

std::vector<ModuleSystemInfo> ModuleSystem::getModulesByType(ModuleType type) const {
    std::vector<ModuleSystemInfo> result;
    for (const auto& pair : pImpl->modules) {
        if (pair.second.type == type) {
            result.push_back(pair.second);
        }
    }
    return result;
}

bool ModuleSystem::hasModule(const std::string& module_name) const {
    return pImpl->modules.find(module_name) != pImpl->modules.end();
}

bool ModuleSystem::validateModule(const ModuleSystemInfo& module_info) {
    if (!pImpl->hasValidModuleStructure(module_info)) {
        return false;
    }
    
    if (!validateModuleFile(module_info.file_path)) {
        addError("Invalid module file: " + module_info.file_path);
        return false;
    }
    
    if (!validateModuleContent(module_info.content)) {
        addError("Invalid module content: " + module_info.name);
        return false;
    }
    
    return true;
}

std::vector<std::string> ModuleSystem::validateModuleList(const std::vector<ModuleSystemInfo>& modules) {
    std::vector<std::string> errors;
    for (const auto& module : modules) {
        if (!validateModule(module)) {
            errors.push_back("Module validation failed: " + module.name);
        }
    }
    return errors;
}

bool ModuleSystem::validateModuleFile(const std::string& file_path) const {
    return pImpl->isValidModuleFile(file_path);
}

bool ModuleSystem::validateModuleContent(const ModuleContent& content) const {
    return pImpl->isValidModuleSize(content.source_code.size()) &&
           !content.source_code.empty();
}

bool ModuleSystem::processModule(const std::string& module_name) {
    if (module_name.empty()) {
        addError("Module name cannot be empty");
        return false;
    }
    
    auto* module_info = getModule(module_name);
    if (!module_info) {
        addError("Module not found: " + module_name);
        return false;
    }
    
    return processModuleByType(module_name, module_info->type);
}

bool ModuleSystem::processModuleByType(const std::string& module_name, ModuleType type) {
    auto* module_info = getModule(module_name);
    if (!module_info) {
        addError("Module not found: " + module_name);
        return false;
    }
    
    switch (type) {
        case ModuleType::CMOD:
            // 处理CMOD模块
            break;
        case ModuleType::CJMOD:
            // 处理CJMOD模块
            break;
        case ModuleType::HTML:
            // 处理HTML模块
            break;
        case ModuleType::CSS:
            // 处理CSS模块
            break;
        case ModuleType::JAVASCRIPT:
            // 处理JavaScript模块
            break;
        default:
            addError("Unknown module type for: " + module_name);
            return false;
    }
    
    return true;
}

std::string ModuleSystem::compileModule(const std::string& module_name) {
    if (module_name.empty()) {
        addError("Module name cannot be empty");
        return "";
    }
    
    auto* module_info = getModule(module_name);
    if (!module_info) {
        addError("Module not found: " + module_name);
        return "";
    }
    
    // 这里应该实现实际的编译逻辑
    // 目前返回源代码作为占位符
    return module_info->content.source_code;
}

bool ModuleSystem::checkCircularDependencies(const std::string& module_name) {
    // 实现循环依赖检测
    return false; // 暂时返回false
}

std::vector<std::string> ModuleSystem::getDependencyChain(const std::string& module_name) {
    std::vector<std::string> chain;
    // 实现依赖链获取
    return chain;
}

bool ModuleSystem::resolveDependencies(const std::string& module_name) {
    // 实现依赖解析
    return true;
}

bool ModuleSystem::loadDependencies(const std::string& module_name) {
    // 实现依赖加载
    return true;
}

bool ModuleSystem::packModule(const std::string& module_name, const std::string& output_path) {
    if (module_name.empty() || output_path.empty()) {
        addError("Module name and output path cannot be empty");
        return false;
    }
    
    auto* module_info = getModule(module_name);
    if (!module_info) {
        addError("Module not found: " + module_name);
        return false;
    }
    
    // 实现模块打包逻辑
    return true;
}

bool ModuleSystem::unpackModule(const std::string& archive_path, const std::string& output_path) {
    if (archive_path.empty() || output_path.empty()) {
        addError("Archive path and output path cannot be empty");
        return false;
    }
    
    // 实现模块解包逻辑
    return true;
}

bool ModuleSystem::createModuleArchive(const std::vector<std::string>& module_names, const std::string& output_path) {
    if (module_names.empty() || output_path.empty()) {
        addError("Module names and output path cannot be empty");
        return false;
    }
    
    // 实现模块归档创建逻辑
    return true;
}

void ModuleSystem::setConfig(const ModuleSystemConfig& config) {
    pImpl->config = config;
}

ModuleSystemConfig ModuleSystem::getConfig() const {
    return pImpl->config;
}

bool ModuleSystem::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> ModuleSystem::getErrors() const {
    return pImpl->errors;
}

void ModuleSystem::clearErrors() {
    pImpl->errors.clear();
}

void ModuleSystem::addError(const std::string& error) {
    pImpl->errors.push_back(error);
}

std::string ModuleSystem::getDebugInfo() const {
    std::ostringstream oss;
    oss << "=== ModuleSystem Debug Info ===\n";
    oss << "Total modules: " << pImpl->modules.size() << "\n";
    oss << "Debug mode: " << (pImpl->config.enable_debug_mode ? "enabled" : "disabled") << "\n";
    oss << "Auto reload: " << (pImpl->config.enable_auto_reload ? "enabled" : "disabled") << "\n";
    oss << "Dependency checking: " << (pImpl->config.enable_dependency_checking ? "enabled" : "disabled") << "\n";
    oss << "Circular dependency detection: " << (pImpl->config.enable_circular_dependency_detection ? "enabled" : "disabled") << "\n";
    oss << "Module search path: " << pImpl->config.module_search_path << "\n";
    oss << "Max module size: " << pImpl->config.max_module_size << " bytes\n";
    oss << "Max dependencies: " << pImpl->config.max_dependencies << "\n";
    
    if (!pImpl->modules.empty()) {
        oss << "\nLoaded modules:\n";
        for (const auto& pair : pImpl->modules) {
            const auto& module = pair.second;
            oss << "  - " << module.name << " (" << module.file_path << ") - " 
                << static_cast<int>(module.status) << "\n";
        }
    }
    
    if (!pImpl->errors.empty()) {
        oss << "\nErrors:\n";
        for (const auto& error : pImpl->errors) {
            oss << "  - " << error << "\n";
        }
    }
    
    return oss.str();
}

void ModuleSystem::enableDebugMode(bool enable) {
    pImpl->config.enable_debug_mode = enable;
}

void ModuleSystem::setStateMachine(CHTLStateMachine* state_machine) {
    pImpl->state_machine = state_machine;
}

bool ModuleSystem::loadModuleContent(ModuleSystemInfo& module_info) {
    std::ifstream file(module_info.file_path);
    if (!file.is_open()) {
        addError("Cannot open module file: " + module_info.file_path);
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    module_info.content.source_code = buffer.str();
    
    // 获取文件大小
    file.seekg(0, std::ios::end);
    module_info.file_size = file.tellg();
    file.close();
    
    return true;
}

// Impl 私有方法实现
bool ModuleSystem::Impl::isValidModuleName(const std::string& name) {
    return !name.empty() && name.length() <= 255 &&
           std::all_of(name.begin(), name.end(), [](char c) {
               return std::isalnum(c) || c == '_' || c == '-';
           });
}

bool ModuleSystem::Impl::isValidModuleFile(const std::string& file_path) {
    if (file_path.empty()) return false;
    
    std::filesystem::path path(file_path);
    if (!std::filesystem::exists(path)) return false;
    
    std::string extension = path.extension().string();
    return std::find(config.allowed_extensions.begin(), 
                     config.allowed_extensions.end(), 
                     extension) != config.allowed_extensions.end();
}

bool ModuleSystem::Impl::isValidModuleSize(size_t size) {
    return size <= config.max_module_size;
}

bool ModuleSystem::Impl::hasValidModuleStructure(const ModuleSystemInfo& module_info) {
    if (!isValidModuleName(module_info.name)) {
        return false;
    }
    
    if (!isValidModuleFile(module_info.file_path)) {
        return false;
    }
    
    if (!isValidModuleSize(module_info.file_size)) {
        return false;
    }
    
    return true;
}

ModuleType ModuleSystem::Impl::detectModuleType(const std::string& file_path) {
    std::filesystem::path path(file_path);
    std::string extension = path.extension().string();
    
    if (extension == ".cmod") return ModuleType::CMOD;
    if (extension == ".cjmod") return ModuleType::CJMOD;
    if (extension == ".chtl") return ModuleType::CMOD; // CHTL文件作为CMOD处理
    if (extension == ".html" || extension == ".htm") return ModuleType::HTML;
    if (extension == ".css") return ModuleType::CSS;
    if (extension == ".js") return ModuleType::JAVASCRIPT;
    
    return ModuleType::CMOD; // 默认类型
}

std::string ModuleSystem::Impl::normalizeModulePath(const std::string& file_path) {
    std::filesystem::path path(file_path);
    return std::filesystem::absolute(path).string();
}

std::string ModuleSystem::Impl::extractModuleExtension(const std::string& file_path) {
    std::filesystem::path path(file_path);
    return path.extension().string();
}

bool ModuleSystem::Impl::moduleFileExists(const std::string& file_path) {
    return std::filesystem::exists(file_path);
}

} // namespace chtl