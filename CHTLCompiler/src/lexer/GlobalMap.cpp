#include "lexer/GlobalMap.h"
#include <sstream>
#include <algorithm>
#include <iostream>

namespace chtl {

class GlobalMap::Impl {
public:
    std::unordered_map<std::string, SymbolInfo> symbols;
    std::unordered_map<std::string, std::vector<std::string>> namespaces;
    std::unordered_map<std::string, std::string> imported_modules;
    std::unordered_map<std::string, std::vector<std::string>> dependencies;
    std::unordered_map<std::string, std::vector<std::string>> dependents;
    
    Impl() = default;
    ~Impl() = default;
};

GlobalMap::GlobalMap() : pImpl(std::make_unique<Impl>()) {}

GlobalMap::~GlobalMap() = default;

bool GlobalMap::addSymbol(const SymbolInfo& symbol) {
    std::string key = symbol.namespace_path.empty() ? symbol.name : 
                     symbol.namespace_path + "::" + symbol.name;
    
    if (pImpl->symbols.find(key) != pImpl->symbols.end()) {
        return false; // 符号已存在
    }
    
    pImpl->symbols[key] = symbol;
    return true;
}

bool GlobalMap::removeSymbol(const std::string& name, const std::string& namespace_path) {
    std::string key = namespace_path.empty() ? name : namespace_path + "::" + name;
    
    auto it = pImpl->symbols.find(key);
    if (it == pImpl->symbols.end()) {
        return false;
    }
    
    pImpl->symbols.erase(it);
    return true;
}

SymbolInfo* GlobalMap::findSymbol(const std::string& name, const std::string& namespace_path) {
    std::string key = namespace_path.empty() ? name : namespace_path + "::" + name;
    
    auto it = pImpl->symbols.find(key);
    if (it != pImpl->symbols.end()) {
        return &(it->second);
    }
    
    return nullptr;
}

std::vector<SymbolInfo*> GlobalMap::findSymbolsByType(SymbolType type) {
    std::vector<SymbolInfo*> result;
    
    for (auto& pair : pImpl->symbols) {
        if (pair.second.type == type) {
            result.push_back(&(pair.second));
        }
    }
    
    return result;
}

bool GlobalMap::addNamespace(const std::string& name, const std::string& parent) {
    if (pImpl->namespaces.find(name) != pImpl->namespaces.end()) {
        return false; // 命名空间已存在
    }
    
    pImpl->namespaces[name] = {};
    if (!parent.empty()) {
        pImpl->namespaces[name].push_back(parent);
    }
    
    return true;
}

bool GlobalMap::removeNamespace(const std::string& name) {
    auto it = pImpl->namespaces.find(name);
    if (it == pImpl->namespaces.end()) {
        return false;
    }
    
    pImpl->namespaces.erase(it);
    return true;
}

std::vector<std::string> GlobalMap::getNamespaces() const {
    std::vector<std::string> result;
    for (const auto& pair : pImpl->namespaces) {
        result.push_back(pair.first);
    }
    return result;
}

std::vector<std::string> GlobalMap::getNamespaceHierarchy(const std::string& name) const {
    auto it = pImpl->namespaces.find(name);
    if (it == pImpl->namespaces.end()) {
        return {};
    }
    
    return it->second;
}

bool GlobalMap::addTemplate(const SymbolInfo& template_info) {
    if (template_info.type != SymbolType::TEMPLATE_STYLE && 
        template_info.type != SymbolType::TEMPLATE_ELEMENT && 
        template_info.type != SymbolType::TEMPLATE_VAR) {
        return false;
    }
    
    return addSymbol(template_info);
}

bool GlobalMap::removeTemplate(const std::string& name, const std::string& namespace_path) {
    return removeSymbol(name, namespace_path);
}

SymbolInfo* GlobalMap::findTemplate(const std::string& name, const std::string& namespace_path) {
    return findSymbol(name, namespace_path);
}

bool GlobalMap::addCustomObject(const SymbolInfo& custom_info) {
    if (custom_info.type != SymbolType::CUSTOM_STYLE && 
        custom_info.type != SymbolType::CUSTOM_ELEMENT && 
        custom_info.type != SymbolType::CUSTOM_VAR) {
        return false;
    }
    
    return addSymbol(custom_info);
}

bool GlobalMap::removeCustomObject(const std::string& name, const std::string& namespace_path) {
    return removeSymbol(name, namespace_path);
}

SymbolInfo* GlobalMap::findCustomObject(const std::string& name, const std::string& namespace_path) {
    return findSymbol(name, namespace_path);
}

bool GlobalMap::addImportedModule(const std::string& module_name, const std::string& path) {
    if (pImpl->imported_modules.find(module_name) != pImpl->imported_modules.end()) {
        return false; // 模块已导入
    }
    
    pImpl->imported_modules[module_name] = path;
    return true;
}

bool GlobalMap::removeImportedModule(const std::string& module_name) {
    auto it = pImpl->imported_modules.find(module_name);
    if (it == pImpl->imported_modules.end()) {
        return false;
    }
    
    pImpl->imported_modules.erase(it);
    return true;
}

std::vector<std::string> GlobalMap::getImportedModules() const {
    std::vector<std::string> result;
    for (const auto& pair : pImpl->imported_modules) {
        result.push_back(pair.first);
    }
    return result;
}

bool GlobalMap::addDependency(const std::string& symbol_name, const std::string& dependency) {
    pImpl->dependencies[symbol_name].push_back(dependency);
    pImpl->dependents[dependency].push_back(symbol_name);
    return true;
}

std::vector<std::string> GlobalMap::getDependencies(const std::string& symbol_name) const {
    auto it = pImpl->dependencies.find(symbol_name);
    if (it == pImpl->dependencies.end()) {
        return {};
    }
    return it->second;
}

std::vector<std::string> GlobalMap::getDependents(const std::string& symbol_name) const {
    auto it = pImpl->dependents.find(symbol_name);
    if (it == pImpl->dependents.end()) {
        return {};
    }
    return it->second;
}

size_t GlobalMap::getSymbolCount() const {
    return pImpl->symbols.size();
}

size_t GlobalMap::getNamespaceCount() const {
    return pImpl->namespaces.size();
}

size_t GlobalMap::getTemplateCount() const {
    size_t count = 0;
    for (const auto& pair : pImpl->symbols) {
        if (pair.second.type == SymbolType::TEMPLATE_STYLE || 
            pair.second.type == SymbolType::TEMPLATE_ELEMENT || 
            pair.second.type == SymbolType::TEMPLATE_VAR) {
            count++;
        }
    }
    return count;
}

size_t GlobalMap::getCustomObjectCount() const {
    size_t count = 0;
    for (const auto& pair : pImpl->symbols) {
        if (pair.second.type == SymbolType::CUSTOM_STYLE || 
            pair.second.type == SymbolType::CUSTOM_ELEMENT || 
            pair.second.type == SymbolType::CUSTOM_VAR) {
            count++;
        }
    }
    return count;
}

void GlobalMap::clear() {
    pImpl->symbols.clear();
    pImpl->namespaces.clear();
    pImpl->imported_modules.clear();
    pImpl->dependencies.clear();
    pImpl->dependents.clear();
}

void GlobalMap::clearNamespace(const std::string& namespace_path) {
    // 移除该命名空间下的所有符号
    auto it = pImpl->symbols.begin();
    while (it != pImpl->symbols.end()) {
        if (it->second.namespace_path == namespace_path) {
            it = pImpl->symbols.erase(it);
        } else {
            ++it;
        }
    }
    
    // 移除命名空间
    pImpl->namespaces.erase(namespace_path);
}

std::string GlobalMap::serialize() const {
    std::ostringstream oss;
    oss << "GlobalMap{" << std::endl;
    oss << "  symbols: " << pImpl->symbols.size() << std::endl;
    oss << "  namespaces: " << pImpl->namespaces.size() << std::endl;
    oss << "  imported_modules: " << pImpl->imported_modules.size() << std::endl;
    oss << "}";
    return oss.str();
}

bool GlobalMap::deserialize(const std::string& data) {
    // 简化实现，实际应该解析JSON或其他格式
    clear();
    return true;
}

std::string GlobalMap::getDebugInfo() const {
    std::ostringstream oss;
    oss << "=== GlobalMap 调试信息 ===" << std::endl;
    oss << "符号总数: " << getSymbolCount() << std::endl;
    oss << "命名空间总数: " << getNamespaceCount() << std::endl;
    oss << "模板总数: " << getTemplateCount() << std::endl;
    oss << "自定义对象总数: " << getCustomObjectCount() << std::endl;
    oss << "导入模块总数: " << getImportedModules().size() << std::endl;
    
    if (!pImpl->symbols.empty()) {
        oss << std::endl << "符号列表:" << std::endl;
        for (const auto& pair : pImpl->symbols) {
            oss << "  " << pair.first << " (" << static_cast<int>(pair.second.type) << ")" << std::endl;
        }
    }
    
    return oss.str();
}

std::vector<std::string> GlobalMap::getConflicts() const {
    std::vector<std::string> conflicts;
    
    // 检查命名冲突
    std::unordered_map<std::string, std::vector<std::string>> name_conflicts;
    
    for (const auto& pair : pImpl->symbols) {
        std::string short_name = pair.second.name;
        name_conflicts[short_name].push_back(pair.first);
    }
    
    for (const auto& pair : name_conflicts) {
        if (pair.second.size() > 1) {
            std::ostringstream oss;
            oss << "命名冲突: " << pair.first << " 在以下位置定义: ";
            for (size_t i = 0; i < pair.second.size(); i++) {
                if (i > 0) oss << ", ";
                oss << pair.second[i];
            }
            conflicts.push_back(oss.str());
        }
    }
    
    return conflicts;
}

std::string GlobalMap::normalizeNamespacePath(const std::string& path) const {
    if (path.empty()) return "";
    
    std::string normalized = path;
    // 移除前导和尾随的::
    if (normalized.substr(0, 2) == "::") {
        normalized = normalized.substr(2);
    }
    if (normalized.length() >= 2 && normalized.substr(normalized.length() - 2) == "::") {
        normalized = normalized.substr(0, normalized.length() - 2);
    }
    
    return normalized;
}

bool GlobalMap::isValidSymbolName(const std::string& name) const {
    if (name.empty()) return false;
    
    // 检查是否以字母或下划线开头
    if (!std::isalpha(name[0]) && name[0] != '_') {
        return false;
    }
    
    // 检查是否只包含字母、数字和下划线
    for (char ch : name) {
        if (!std::isalnum(ch) && ch != '_') {
            return false;
        }
    }
    
    return true;
}

bool GlobalMap::isValidNamespacePath(const std::string& path) const {
    if (path.empty()) return true;
    
    std::string normalized = normalizeNamespacePath(path);
    if (normalized.empty()) return true;
    
    // 检查路径中的每个部分
    std::istringstream iss(normalized);
    std::string part;
    while (std::getline(iss, part, ':')) {
        if (!isValidSymbolName(part)) {
            return false;
        }
    }
    
    return true;
}

std::string GlobalMap::resolveNamespacePath(const std::string& name, const std::string& current_namespace) const {
    if (name.empty()) return current_namespace;
    
    if (name[0] == ':') {
        // 绝对路径
        return normalizeNamespacePath(name);
    } else {
        // 相对路径
        if (current_namespace.empty()) {
            return name;
        } else {
            return current_namespace + "::" + name;
        }
    }
}

} // namespace chtl