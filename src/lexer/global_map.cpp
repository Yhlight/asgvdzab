#include "lexer/global_map.hpp"
#include <algorithm>
#include <sstream>
#include <cctype>

namespace chtl {

// SymbolDefinition 实现
std::string SymbolDefinition::getFullyQualifiedName() const {
    if (namespacePath.empty()) {
        return name;
    }
    return namespacePath + "." + name;
}

bool SymbolDefinition::isInNamespace(const std::string& ns) const {
    return namespacePath == ns || namespacePath.find(ns + ".") == 0;
}

std::string SymbolDefinition::toString() const {
    std::ostringstream oss;
    oss << "SymbolDefinition{";
    oss << "type=" << static_cast<int>(type);
    oss << ", name=\"" << name << "\"";
    oss << ", namespace=\"" << namespacePath << "\"";
    oss << ", position=" << position.line << ":" << position.column;
    oss << "}";
    return oss.str();
}

// NamespaceDefinition 实现
std::string NamespaceDefinition::getFullPath() const {
    if (parentNamespace.empty()) {
        return name;
    }
    return parentNamespace + "." + name;
}

bool NamespaceDefinition::isNested() const {
    return !parentNamespace.empty();
}

std::string NamespaceDefinition::toString() const {
    std::ostringstream oss;
    oss << "NamespaceDefinition{";
    oss << "name=\"" << name << "\"";
    oss << ", parent=\"" << parentNamespace << "\"";
    oss << ", children=" << children.size();
    oss << "}";
    return oss.str();
}

// ImportDefinition 实现
std::string ImportDefinition::getEffectiveName() const {
    return aliasName.empty() ? symbolName : aliasName;
}

std::string ImportDefinition::toString() const {
    std::ostringstream oss;
    oss << "ImportDefinition{";
    oss << "type=" << static_cast<int>(type);
    oss << ", symbol=\"" << symbolName << "\"";
    oss << ", file=\"" << filePath << "\"";
    if (!aliasName.empty()) {
        oss << ", alias=\"" << aliasName << "\"";
    }
    oss << "}";
    return oss.str();
}

// GlobalMap 实现
GlobalMap::GlobalMap() {
    // 初始化
}

bool GlobalMap::registerSymbol(const SymbolDefinition& symbol) {
    std::string qualifiedName = buildFullyQualifiedName(symbol.name, symbol.namespacePath);
    
    // 检查是否已存在
    if (symbols_.find(qualifiedName) != symbols_.end()) {
        return false; // 符号已存在
    }
    
    // 验证符号名称
    if (!isValidSymbolName(symbol.name)) {
        return false;
    }
    
    // 注册符号
    symbols_[qualifiedName] = std::make_unique<SymbolDefinition>(symbol);
    symbols_[qualifiedName]->sourceFile = currentFile_;
    
    return true;
}

const SymbolDefinition* GlobalMap::findSymbol(const std::string& name, 
                                              const std::string& currentNamespace) const {
    // 首先尝试在当前命名空间中查找
    if (!currentNamespace.empty()) {
        std::string qualifiedName = buildFullyQualifiedName(name, currentNamespace);
        auto it = symbols_.find(qualifiedName);
        if (it != symbols_.end()) {
            return it->second.get();
        }
    }
    
    // 然后尝试全局查找
    auto it = symbols_.find(name);
    if (it != symbols_.end()) {
        return it->second.get();
    }
    
    // 最后尝试模糊匹配
    return findBestMatch(name, currentNamespace);
}

std::vector<const SymbolDefinition*> GlobalMap::findSymbolsByType(SymbolType type) const {
    std::vector<const SymbolDefinition*> result;
    
    for (const auto& [name, symbol] : symbols_) {
        if (symbol->type == type) {
            result.push_back(symbol.get());
        }
    }
    
    return result;
}

bool GlobalMap::removeSymbol(const std::string& name, const std::string& namespacePath) {
    std::string qualifiedName = buildFullyQualifiedName(name, namespacePath);
    auto it = symbols_.find(qualifiedName);
    if (it != symbols_.end()) {
        symbols_.erase(it);
        return true;
    }
    return false;
}

std::vector<const SymbolDefinition*> GlobalMap::getAllSymbols() const {
    std::vector<const SymbolDefinition*> result;
    
    for (const auto& [name, symbol] : symbols_) {
        result.push_back(symbol.get());
    }
    
    return result;
}

bool GlobalMap::registerNamespace(const NamespaceDefinition& ns) {
    std::string fullPath = ns.getFullPath();
    
    // 检查是否已存在
    if (namespaces_.find(fullPath) != namespaces_.end()) {
        return false;
    }
    
    // 验证命名空间名称
    if (!isValidNamespaceName(ns.name)) {
        return false;
    }
    
    // 注册命名空间
    namespaces_[fullPath] = std::make_unique<NamespaceDefinition>(ns);
    namespaces_[fullPath]->sourceFile = currentFile_;
    
    return true;
}

const NamespaceDefinition* GlobalMap::findNamespace(const std::string& path) const {
    auto it = namespaces_.find(path);
    return (it != namespaces_.end()) ? it->second.get() : nullptr;
}

std::vector<const SymbolDefinition*> GlobalMap::getNamespaceSymbols(const std::string& namespacePath) const {
    std::vector<const SymbolDefinition*> result;
    
    for (const auto& [name, symbol] : symbols_) {
        if (symbol->isInNamespace(namespacePath)) {
            result.push_back(symbol.get());
        }
    }
    
    return result;
}

bool GlobalMap::checkNamespaceConstraints(const std::string& namespacePath, 
                                          const std::string& symbolName, 
                                          SymbolType symbolType) const {
    const NamespaceDefinition* ns = findNamespace(namespacePath);
    if (!ns) {
        return true; // 命名空间不存在，允许
    }
    
    // 检查约束
    for (const auto& constraint : ns->constraints) {
        // 这里可以实现复杂的约束检查逻辑
        // 暂时简化处理
        if (constraint == "except [Template]" && 
            (symbolType == SymbolType::TEMPLATE_STYLE || 
             symbolType == SymbolType::TEMPLATE_ELEMENT || 
             symbolType == SymbolType::TEMPLATE_VAR)) {
            return false;
        }
    }
    
    return true;
}

bool GlobalMap::registerImport(const ImportDefinition& import) {
    std::string key = import.getEffectiveName();
    
    // 检查是否已存在
    if (imports_.find(key) != imports_.end()) {
        return false;
    }
    
    // 注册导入
    imports_[key] = std::make_unique<ImportDefinition>(import);
    imports_[key]->sourceFile = currentFile_;
    
    return true;
}

const ImportDefinition* GlobalMap::findImport(const std::string& name) const {
    auto it = imports_.find(name);
    return (it != imports_.end()) ? it->second.get() : nullptr;
}

std::vector<const ImportDefinition*> GlobalMap::getAllImports() const {
    std::vector<const ImportDefinition*> result;
    
    for (const auto& [name, import] : imports_) {
        result.push_back(import.get());
    }
    
    return result;
}

bool GlobalMap::resolveImports() {
    // 这里实现导入解析逻辑
    // 暂时返回true，表示成功
    return true;
}

SymbolDefinition GlobalMap::resolveInheritance(const SymbolDefinition& symbol) const {
    SymbolDefinition resolved = symbol;
    
    // 处理继承链
    std::unordered_set<std::string> visited;
    for (const auto& parentName : symbol.inherits) {
        if (visited.count(parentName)) {
            continue; // 避免循环继承
        }
        visited.insert(parentName);
        
        const SymbolDefinition* parent = findSymbol(parentName, symbol.namespacePath);
        if (parent) {
            // 合并内容
            resolved.content = mergeInheritedContent(*parent, resolved);
        }
    }
    
    // 应用特例化
    resolved = applySpecialization(resolved);
    
    return resolved;
}

bool GlobalMap::hasCircularInheritance(const std::string& symbolName, 
                                       std::unordered_set<std::string>& visited) const {
    if (visited.count(symbolName)) {
        return true; // 发现循环
    }
    
    visited.insert(symbolName);
    
    const SymbolDefinition* symbol = findSymbol(symbolName);
    if (symbol) {
        for (const auto& parentName : symbol->inherits) {
            if (hasCircularInheritance(parentName, visited)) {
                return true;
            }
        }
    }
    
    visited.erase(symbolName);
    return false;
}

void GlobalMap::registerConfiguration(const std::unordered_map<std::string, std::string>& config) {
    for (const auto& [key, value] : config) {
        configurations_[key] = value;
    }
}

std::string GlobalMap::getConfigValue(const std::string& key, const std::string& defaultValue) const {
    auto it = configurations_.find(key);
    return (it != configurations_.end()) ? it->second : defaultValue;
}

const std::unordered_map<std::string, std::string>& GlobalMap::getAllConfigurations() const {
    return configurations_;
}

void GlobalMap::setCurrentFile(const std::string& filePath) {
    currentFile_ = filePath;
}

const std::string& GlobalMap::getCurrentFile() const {
    return currentFile_;
}

void GlobalMap::clear() {
    symbols_.clear();
    namespaces_.clear();
    imports_.clear();
    configurations_.clear();
    currentFile_.clear();
}

GlobalMap::Statistics GlobalMap::getStatistics() const {
    Statistics stats;
    
    stats.totalSymbols = symbols_.size();
    stats.totalNamespaces = namespaces_.size();
    stats.totalImports = imports_.size();
    
    // 统计各类型符号
    for (const auto& [name, symbol] : symbols_) {
        stats.symbolsByType[symbol->type]++;
    }
    
    return stats;
}

std::string GlobalMap::exportToString() const {
    std::ostringstream oss;
    
    oss << "=== GlobalMap Export ===\n";
    oss << "Symbols: " << symbols_.size() << "\n";
    for (const auto& [name, symbol] : symbols_) {
        oss << "  " << symbol->toString() << "\n";
    }
    
    oss << "Namespaces: " << namespaces_.size() << "\n";
    for (const auto& [name, ns] : namespaces_) {
        oss << "  " << ns->toString() << "\n";
    }
    
    oss << "Imports: " << imports_.size() << "\n";
    for (const auto& [name, import] : imports_) {
        oss << "  " << import->toString() << "\n";
    }
    
    oss << "Configurations: " << configurations_.size() << "\n";
    for (const auto& [key, value] : configurations_) {
        oss << "  " << key << " = " << value << "\n";
    }
    
    return oss.str();
}

std::vector<std::string> GlobalMap::validateConsistency() const {
    std::vector<std::string> errors;
    
    // 检查循环继承
    for (const auto& [name, symbol] : symbols_) {
        std::unordered_set<std::string> visited;
        if (hasCircularInheritance(symbol->name, visited)) {
            errors.push_back("Circular inheritance detected for symbol: " + symbol->name);
        }
    }
    
    // 检查符号引用
    for (const auto& [name, symbol] : symbols_) {
        for (const auto& parentName : symbol->inherits) {
            if (!findSymbol(parentName, symbol->namespacePath)) {
                errors.push_back("Unresolved inheritance reference: " + parentName + " in " + symbol->name);
            }
        }
    }
    
    return errors;
}

// 私有方法实现
std::string GlobalMap::buildFullyQualifiedName(const std::string& name, 
                                               const std::string& namespacePath) const {
    if (namespacePath.empty()) {
        return name;
    }
    return namespacePath + "." + name;
}

std::vector<std::string> GlobalMap::parseNamespacePath(const std::string& path) const {
    std::vector<std::string> parts;
    std::istringstream iss(path);
    std::string part;
    
    while (std::getline(iss, part, '.')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }
    
    return parts;
}

const SymbolDefinition* GlobalMap::findBestMatch(const std::string& name, 
                                                  const std::string& currentNamespace) const {
    // 实现模糊匹配逻辑
    for (const auto& [qualifiedName, symbol] : symbols_) {
        if (symbol->name == name) {
            return symbol.get();
        }
    }
    
    return nullptr;
}

std::string GlobalMap::mergeInheritedContent(const SymbolDefinition& base, 
                                             const SymbolDefinition& derived) const {
    // 简单的内容合并逻辑
    // 实际实现应该根据符号类型进行智能合并
    return base.content + "\n" + derived.content;
}

SymbolDefinition GlobalMap::applySpecialization(const SymbolDefinition& symbol) const {
    SymbolDefinition specialized = symbol;
    
    // 应用删除操作
    for (const auto& deleted : symbol.specialization.deletedProperties) {
        // 从内容中删除指定属性
        // 这里需要实际的解析和处理逻辑
    }
    
    // 应用插入和替换操作
    for (const auto& [position, content] : symbol.specialization.insertions) {
        // 在指定位置插入内容
    }
    
    for (const auto& [target, replacement] : symbol.specialization.replacements) {
        // 替换指定内容
    }
    
    return specialized;
}

bool GlobalMap::isValidSymbolName(const std::string& name) const {
    if (name.empty()) {
        return false;
    }
    
    // 第一个字符必须是字母或下划线
    if (!std::isalpha(name[0]) && name[0] != '_') {
        return false;
    }
    
    // 其他字符必须是字母、数字、下划线或连字符
    for (size_t i = 1; i < name.length(); ++i) {
        char ch = name[i];
        if (!std::isalnum(ch) && ch != '_' && ch != '-') {
            return false;
        }
    }
    
    return true;
}

bool GlobalMap::isValidNamespaceName(const std::string& name) const {
    return isValidSymbolName(name); // 使用相同的验证规则
}

// SymbolUtils 实现
std::pair<SymbolType, std::string> SymbolUtils::parseSymbolReference(const std::string& reference) {
    std::string trimmed = reference;
    
    // 移除前后空白
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);
    
    SymbolType type = SymbolType::TEMPLATE_STYLE; // 默认类型
    std::string name;
    
    // 检查是否有类型前缀
    if (trimmed.find("[Template]") == 0) {
        trimmed = trimmed.substr(10); // 移除 "[Template]"
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        
        if (trimmed.find("@Style") == 0) {
            type = SymbolType::TEMPLATE_STYLE;
            name = trimmed.substr(6);
        } else if (trimmed.find("@Element") == 0) {
            type = SymbolType::TEMPLATE_ELEMENT;
            name = trimmed.substr(8);
        } else if (trimmed.find("@Var") == 0) {
            type = SymbolType::TEMPLATE_VAR;
            name = trimmed.substr(4);
        }
    } else if (trimmed.find("[Custom]") == 0) {
        trimmed = trimmed.substr(8); // 移除 "[Custom]"
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        
        if (trimmed.find("@Style") == 0) {
            type = SymbolType::CUSTOM_STYLE;
            name = trimmed.substr(6);
        } else if (trimmed.find("@Element") == 0) {
            type = SymbolType::CUSTOM_ELEMENT;
            name = trimmed.substr(8);
        } else if (trimmed.find("@Var") == 0) {
            type = SymbolType::CUSTOM_VAR;
            name = trimmed.substr(4);
        }
    } else if (trimmed.find("[Origin]") == 0) {
        trimmed = trimmed.substr(8); // 移除 "[Origin]"
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        
        if (trimmed.find("@Html") == 0) {
            type = SymbolType::ORIGIN_HTML;
            name = trimmed.substr(5);
        } else if (trimmed.find("@Style") == 0) {
            type = SymbolType::ORIGIN_STYLE;
            name = trimmed.substr(6);
        } else if (trimmed.find("@JavaScript") == 0) {
            type = SymbolType::ORIGIN_JAVASCRIPT;
            name = trimmed.substr(11);
        }
    } else if (trimmed.find("[Import]") == 0) {
        type = SymbolType::IMPORT;
        name = trimmed; // 保留完整内容用于导入解析
    } else if (trimmed.find("[Namespace]") == 0) {
        type = SymbolType::NAMESPACE;
        name = trimmed.substr(11);
    } else if (trimmed.find("[Configuration]") == 0) {
        type = SymbolType::CONFIGURATION;
        name = trimmed.substr(15);
    } else if (trimmed.find("@Style") == 0) {
        type = SymbolType::TEMPLATE_STYLE; // 默认为模板
        name = trimmed.substr(6);
    } else if (trimmed.find("@Element") == 0) {
        type = SymbolType::TEMPLATE_ELEMENT;
        name = trimmed.substr(8);
    } else if (trimmed.find("@Var") == 0) {
        type = SymbolType::TEMPLATE_VAR;
        name = trimmed.substr(4);
    } else {
        // 只有名称，类型需要从上下文推断
        name = trimmed;
    }
    
    // 清理名称
    name.erase(0, name.find_first_not_of(" \t"));
    name.erase(name.find_last_not_of(" \t") + 1);
    
    return std::make_pair(type, name);
}

std::string SymbolUtils::formatSymbolReference(SymbolType type, const std::string& name) {
    std::string prefix;
    
    switch (type) {
        case SymbolType::TEMPLATE_STYLE:
            prefix = "[Template] @Style ";
            break;
        case SymbolType::TEMPLATE_ELEMENT:
            prefix = "[Template] @Element ";
            break;
        case SymbolType::TEMPLATE_VAR:
            prefix = "[Template] @Var ";
            break;
        case SymbolType::CUSTOM_STYLE:
            prefix = "[Custom] @Style ";
            break;
        case SymbolType::CUSTOM_ELEMENT:
            prefix = "[Custom] @Element ";
            break;
        case SymbolType::CUSTOM_VAR:
            prefix = "[Custom] @Var ";
            break;
        default:
            prefix = "";
            break;
    }
    
    return prefix + name;
}

bool SymbolUtils::areTypesCompatible(SymbolType sourceType, SymbolType targetType) {
    // 模板和自定义的相同类型是兼容的
    if ((sourceType == SymbolType::TEMPLATE_STYLE && targetType == SymbolType::CUSTOM_STYLE) ||
        (sourceType == SymbolType::CUSTOM_STYLE && targetType == SymbolType::TEMPLATE_STYLE) ||
        (sourceType == SymbolType::TEMPLATE_ELEMENT && targetType == SymbolType::CUSTOM_ELEMENT) ||
        (sourceType == SymbolType::CUSTOM_ELEMENT && targetType == SymbolType::TEMPLATE_ELEMENT) ||
        (sourceType == SymbolType::TEMPLATE_VAR && targetType == SymbolType::CUSTOM_VAR) ||
        (sourceType == SymbolType::CUSTOM_VAR && targetType == SymbolType::TEMPLATE_VAR)) {
        return true;
    }
    
    return sourceType == targetType;
}

std::string SymbolUtils::symbolTypeToString(SymbolType type) {
    switch (type) {
        case SymbolType::TEMPLATE_STYLE: return "TEMPLATE_STYLE";
        case SymbolType::TEMPLATE_ELEMENT: return "TEMPLATE_ELEMENT";
        case SymbolType::TEMPLATE_VAR: return "TEMPLATE_VAR";
        case SymbolType::CUSTOM_STYLE: return "CUSTOM_STYLE";
        case SymbolType::CUSTOM_ELEMENT: return "CUSTOM_ELEMENT";
        case SymbolType::CUSTOM_VAR: return "CUSTOM_VAR";
        case SymbolType::ORIGIN_HTML: return "ORIGIN_HTML";
        case SymbolType::ORIGIN_STYLE: return "ORIGIN_STYLE";
        case SymbolType::ORIGIN_JAVASCRIPT: return "ORIGIN_JAVASCRIPT";
        case SymbolType::NAMESPACE: return "NAMESPACE";
        case SymbolType::CONFIGURATION: return "CONFIGURATION";
        case SymbolType::IMPORT: return "IMPORT";
        default: return "UNKNOWN";
    }
}

SymbolType SymbolUtils::symbolTypeFromString(const std::string& str) {
    if (str == "TEMPLATE_STYLE") return SymbolType::TEMPLATE_STYLE;
    if (str == "TEMPLATE_ELEMENT") return SymbolType::TEMPLATE_ELEMENT;
    if (str == "TEMPLATE_VAR") return SymbolType::TEMPLATE_VAR;
    if (str == "CUSTOM_STYLE") return SymbolType::CUSTOM_STYLE;
    if (str == "CUSTOM_ELEMENT") return SymbolType::CUSTOM_ELEMENT;
    if (str == "CUSTOM_VAR") return SymbolType::CUSTOM_VAR;
    if (str == "ORIGIN_HTML") return SymbolType::ORIGIN_HTML;
    if (str == "ORIGIN_STYLE") return SymbolType::ORIGIN_STYLE;
    if (str == "ORIGIN_JAVASCRIPT") return SymbolType::ORIGIN_JAVASCRIPT;
    if (str == "NAMESPACE") return SymbolType::NAMESPACE;
    if (str == "CONFIGURATION") return SymbolType::CONFIGURATION;
    if (str == "IMPORT") return SymbolType::IMPORT;
    
    return SymbolType::TEMPLATE_STYLE; // 默认
}

bool SymbolUtils::isValidSymbolName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 第一个字符必须是字母或下划线
    if (!std::isalpha(name[0]) && name[0] != '_') {
        return false;
    }
    
    // 其他字符必须是字母、数字、下划线或连字符
    for (size_t i = 1; i < name.length(); ++i) {
        char ch = name[i];
        if (!std::isalnum(ch) && ch != '_' && ch != '-') {
            return false;
        }
    }
    
    return true;
}

std::pair<std::string, std::string> SymbolUtils::splitQualifiedName(const std::string& qualifiedName) {
    size_t dotPos = qualifiedName.find_last_of('.');
    if (dotPos == std::string::npos) {
        return std::make_pair("", qualifiedName);
    }
    
    return std::make_pair(qualifiedName.substr(0, dotPos), 
                         qualifiedName.substr(dotPos + 1));
}

} // namespace chtl