#include "namespace/enhanced_namespace_manager.hpp"
#include <algorithm>
#include <sstream>
#include <regex>

namespace chtl {

EnhancedNamespaceManager::EnhancedNamespaceManager() {
    // 初始化根命名空间
    namespaceStack_.push_back("");
}

EnhancedNamespaceManager::~EnhancedNamespaceManager() = default;

// ===== 命名空间管理 =====

bool EnhancedNamespaceManager::declareNamespace(const std::string& namespacePath, 
                                               const Position& position, 
                                               const std::string& sourceFile) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!isValidNamespaceName(getNamespaceName(namespacePath))) {
        return false;
    }
    
    // 检测命名空间冲突
    auto conflicts = detectNamespaceConflicts(namespacePath, position, sourceFile);
    if (!conflicts.empty()) {
        // 如果有冲突，尝试合并
        for (const auto& conflict : conflicts) {
            if (conflict.type != ConflictType::NAME_COLLISION) {
                return false; // 只允许名称冲突（用于合并）
            }
        }
    }
    
    // 创建或获取命名空间
    getOrCreateNamespace(namespacePath, position, sourceFile);
    
    return true;
}

bool EnhancedNamespaceManager::enterNamespace(const std::string& namespacePath) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!namespaceExists(namespacePath)) {
        return false;
    }
    
    namespaceStack_.push_back(namespacePath);
    return true;
}

bool EnhancedNamespaceManager::exitNamespace() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (namespaceStack_.size() <= 1) {
        return false; // 不能退出根命名空间
    }
    
    namespaceStack_.pop_back();
    return true;
}

std::string EnhancedNamespaceManager::getCurrentNamespacePath() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return getCurrentNamespacePathInternal();
}

bool EnhancedNamespaceManager::namespaceExists(const std::string& namespacePath) const {
    std::lock_guard<std::mutex> lock(mutex_);
    return namespaces_.find(namespacePath) != namespaces_.end();
}

const NamespaceDefinition* EnhancedNamespaceManager::getNamespaceDefinition(const std::string& namespacePath) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = namespaces_.find(namespacePath);
    return (it != namespaces_.end()) ? &it->second : nullptr;
}

// ===== 符号管理 =====

NamespaceResolutionResult EnhancedNamespaceManager::declareSymbol(const std::string& symbolName, 
                                                                 NamespaceSymbolType symbolType,
                                                                 const Position& position,
                                                                 const std::string& sourceFile,
                                                                 const std::string& content,
                                                                 bool isPublic,
                                                                 bool isExported) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    NamespaceResolutionResult result;
    
    if (!isValidSymbolName(symbolName)) {
        result.errorMessage = "Invalid symbol name: " + symbolName;
        return result;
    }
    
    std::string currentNs = getCurrentNamespacePathInternal();
    
    // 创建符号定义
    NamespaceSymbolDefinition symbolDef;
    symbolDef.name = symbolName;
    symbolDef.type = symbolType;
    symbolDef.fullName = generateFullSymbolName(symbolName, currentNs);
    symbolDef.namespacePath = currentNs;
    symbolDef.position = position;
    symbolDef.sourceFile = sourceFile;
    symbolDef.content = content;
    symbolDef.isPublic = isPublic;
    symbolDef.isExported = isExported;
    
    // 检测符号冲突
    auto conflicts = detectSymbolConflicts(symbolDef, currentNs);
    
    if (!conflicts.empty()) {
        // 检查是否可以合并
        bool canMerge = true;
        for (const auto& conflict : conflicts) {
            if (conflict.type != ConflictType::DUPLICATE_SYMBOL) {
                canMerge = false;
                break;
            }
        }
        
        if (!canMerge) {
            result.conflicts = conflicts;
            result.errorMessage = "Symbol conflicts detected for: " + symbolName;
            return result;
        }
    }
    
    // 确保命名空间存在
    auto& namespaceDef = getOrCreateNamespace(currentNs, position, sourceFile);
    
    // 添加符号
    namespaceDef.symbols[symbolName] = symbolDef;
    
    result.success = true;
    result.resolvedSymbols.push_back(symbolDef);
    result.conflicts = conflicts; // 包含已解决的冲突
    
    return result;
}

const NamespaceSymbolDefinition* EnhancedNamespaceManager::findSymbol(const std::string& symbolName, 
                                                                     const std::string& namespacePath) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::string targetNs = namespacePath.empty() ? getCurrentNamespacePathInternal() : namespacePath;
    
    auto nsIt = namespaces_.find(targetNs);
    if (nsIt == namespaces_.end()) {
        return nullptr;
    }
    
    auto symbolIt = nsIt->second.symbols.find(symbolName);
    return (symbolIt != nsIt->second.symbols.end()) ? &symbolIt->second : nullptr;
}

const NamespaceSymbolDefinition* EnhancedNamespaceManager::findSymbolByFullName(const std::string& fullName) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // 解析完整名称
    size_t lastDot = fullName.find_last_of('.');
    if (lastDot == std::string::npos) {
        // 没有命名空间，在当前命名空间查找
        return findSymbol(fullName, getCurrentNamespacePathInternal());
    }
    
    std::string namespacePath = fullName.substr(0, lastDot);
    std::string symbolName = fullName.substr(lastDot + 1);
    
    return findSymbol(symbolName, namespacePath);
}

std::vector<const NamespaceSymbolDefinition*> EnhancedNamespaceManager::getCurrentNamespaceSymbols(bool includePrivate) const {
    return getNamespaceSymbols(getCurrentNamespacePathInternal(), includePrivate);
}

std::vector<const NamespaceSymbolDefinition*> EnhancedNamespaceManager::getNamespaceSymbols(const std::string& namespacePath, 
                                                                                           bool includePrivate) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::vector<const NamespaceSymbolDefinition*> result;
    
    auto nsIt = namespaces_.find(namespacePath);
    if (nsIt == namespaces_.end()) {
        return result;
    }
    
    for (const auto& [name, symbol] : nsIt->second.symbols) {
        if (includePrivate || symbol.isPublic) {
            result.push_back(&symbol);
        }
    }
    
    return result;
}

// ===== 命名空间合并 =====

NamespaceResolutionResult EnhancedNamespaceManager::mergeNamespace(const std::string& namespacePath, 
                                                                  const std::string& sourceFile) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    NamespaceResolutionResult result;
    
    auto nsIt = namespaces_.find(namespacePath);
    if (nsIt == namespaces_.end()) {
        result.errorMessage = "Namespace not found: " + namespacePath;
        return result;
    }
    
    auto& namespaceDef = nsIt->second;
    
    // 检查是否已经从该文件合并过
    if (std::find(namespaceDef.mergedFiles.begin(), namespaceDef.mergedFiles.end(), sourceFile) != namespaceDef.mergedFiles.end()) {
        result.success = true; // 已经合并过，不是错误
        return result;
    }
    
    // 标记为合并的命名空间
    namespaceDef.isMerged = true;
    namespaceDef.mergedFiles.push_back(sourceFile);
    
    result.success = true;
    return result;
}

std::vector<ConflictInfo> EnhancedNamespaceManager::checkMergeConflicts(const std::string& namespacePath1, 
                                                                       const std::string& namespacePath2) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::vector<ConflictInfo> conflicts;
    
    auto ns1It = namespaces_.find(namespacePath1);
    auto ns2It = namespaces_.find(namespacePath2);
    
    if (ns1It == namespaces_.end() || ns2It == namespaces_.end()) {
        return conflicts;
    }
    
    const auto& ns1 = ns1It->second;
    const auto& ns2 = ns2It->second;
    
    // 检查符号冲突
    for (const auto& [name, symbol1] : ns1.symbols) {
        auto symbol2It = ns2.symbols.find(name);
        if (symbol2It != ns2.symbols.end()) {
            const auto& symbol2 = symbol2It->second;
            
            if (!canMergeSymbols(symbol1, symbol2)) {
                ConflictInfo conflict = createConflictInfo(ConflictType::DUPLICATE_SYMBOL, 
                                                         "Symbol exists in both namespaces", 
                                                         symbol1, symbol2);
                conflicts.push_back(conflict);
            }
        }
    }
    
    return conflicts;
}

// ===== 冲突检测 =====

std::vector<ConflictInfo> EnhancedNamespaceManager::detectSymbolConflicts(const NamespaceSymbolDefinition& symbolDef,
                                                                         const std::string& namespacePath) const {
    std::vector<ConflictInfo> conflicts;
    
    auto nsIt = namespaces_.find(namespacePath);
    if (nsIt == namespaces_.end()) {
        return conflicts;
    }
    
    auto existingIt = nsIt->second.symbols.find(symbolDef.name);
    if (existingIt != nsIt->second.symbols.end()) {
        const auto& existing = existingIt->second;
        
        if (!canMergeSymbols(symbolDef, existing)) {
            ConflictType conflictType = ConflictType::DUPLICATE_SYMBOL;
            
            if (symbolDef.type != existing.type) {
                conflictType = ConflictType::TYPE_MISMATCH;
            } else if (symbolDef.isPublic != existing.isPublic) {
                conflictType = ConflictType::VISIBILITY_CONFLICT;
            } else if (symbolDef.isExported != existing.isExported) {
                conflictType = ConflictType::EXPORT_CONFLICT;
            }
            
            ConflictInfo conflict = createConflictInfo(conflictType, 
                                                     "Symbol already exists", 
                                                     symbolDef, existing);
            conflicts.push_back(conflict);
        }
    }
    
    return conflicts;
}

std::vector<ConflictInfo> EnhancedNamespaceManager::detectNamespaceConflicts(const std::string& namespacePath,
                                                                            const Position& position,
                                                                            const std::string& sourceFile) const {
    std::vector<ConflictInfo> conflicts;
    
    auto nsIt = namespaces_.find(namespacePath);
    if (nsIt != namespaces_.end()) {
        const auto& existing = nsIt->second;
        
        // 同名命名空间冲突（通常可以合并）
        ConflictInfo conflict;
        conflict.type = ConflictType::NAME_COLLISION;
        conflict.description = "Namespace with same name already exists";
        conflict.symbolName = namespacePath;
        conflict.namespacePath = namespacePath;
        conflict.position1 = existing.position;
        conflict.position2 = position;
        conflict.sourceFile1 = existing.sourceFile;
        conflict.sourceFile2 = sourceFile;
        conflict.suggestion = "Namespaces can be merged if no symbol conflicts exist";
        
        conflicts.push_back(conflict);
    }
    
    return conflicts;
}

bool EnhancedNamespaceManager::resolveConflicts(const std::vector<ConflictInfo>& conflicts, const std::string& strategy) {
    if (strategy == "merge") {
        // 合并策略：只允许可以合并的冲突
        for (const auto& conflict : conflicts) {
            if (conflict.type != ConflictType::NAME_COLLISION && 
                conflict.type != ConflictType::DUPLICATE_SYMBOL) {
                return false;
            }
        }
        return true;
    }
    
    return false;
}

// ===== 命名空间查询 =====

std::vector<std::string> EnhancedNamespaceManager::getAllNamespaces() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::vector<std::string> result;
    for (const auto& [path, _] : namespaces_) {
        result.push_back(path);
    }
    
    std::sort(result.begin(), result.end());
    return result;
}

std::vector<std::string> EnhancedNamespaceManager::getChildNamespaces(const std::string& parentPath) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::vector<std::string> result;
    
    auto nsIt = namespaces_.find(parentPath);
    if (nsIt != namespaces_.end()) {
        for (const auto& child : nsIt->second.childNamespaces) {
            result.push_back(child);
        }
    }
    
    std::sort(result.begin(), result.end());
    return result;
}

std::string EnhancedNamespaceManager::getNamespaceHierarchy() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::stringstream ss;
    ss << "Namespace Hierarchy:\n";
    
    // 构建层次结构
    buildHierarchyRecursive("", 0, ss);
    
    return ss.str();
}

std::vector<const NamespaceSymbolDefinition*> EnhancedNamespaceManager::searchSymbols(const std::string& pattern,
                                                                                     NamespaceSymbolType symbolType,
                                                                                     const std::string& namespacePath) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::vector<const NamespaceSymbolDefinition*> result;
    std::regex regex(pattern, std::regex_constants::icase);
    
    auto searchInNamespace = [&](const NamespaceDefinition& ns) {
        for (const auto& [name, symbol] : ns.symbols) {
            if ((symbolType == NamespaceSymbolType::TEMPLATE || symbol.type == symbolType) &&
                std::regex_search(symbol.name, regex)) {
                result.push_back(&symbol);
            }
        }
    };
    
    if (namespacePath.empty()) {
        // 搜索所有命名空间
        for (const auto& [path, ns] : namespaces_) {
            searchInNamespace(ns);
        }
    } else {
        // 搜索指定命名空间
        auto nsIt = namespaces_.find(namespacePath);
        if (nsIt != namespaces_.end()) {
            searchInNamespace(nsIt->second);
        }
    }
    
    return result;
}

// ===== 导入和导出 =====

NamespaceResolutionResult EnhancedNamespaceManager::importNamespace(const std::string& namespacePath,
                                                                   const std::string& alias,
                                                                   const std::vector<std::string>& selectiveSymbols) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    NamespaceResolutionResult result;
    
    if (!namespaceExists(namespacePath)) {
        result.errorMessage = "Namespace not found: " + namespacePath;
        return result;
    }
    
    // 记录导入别名
    if (!alias.empty()) {
        importAliases_[alias] = namespacePath;
    }
    
    // 获取要导入的符号
    auto symbols = getNamespaceSymbols(namespacePath, false); // 只导入公开符号
    
    for (const auto* symbol : symbols) {
        if (selectiveSymbols.empty() || 
            std::find(selectiveSymbols.begin(), selectiveSymbols.end(), symbol->name) != selectiveSymbols.end()) {
            result.resolvedSymbols.push_back(*symbol);
        }
    }
    
    result.success = true;
    return result;
}

bool EnhancedNamespaceManager::exportSymbol(const std::string& symbolName, const std::string& namespacePath) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::string targetNs = namespacePath.empty() ? getCurrentNamespacePathInternal() : namespacePath;
    
    auto nsIt = namespaces_.find(targetNs);
    if (nsIt == namespaces_.end()) {
        return false;
    }
    
    auto symbolIt = nsIt->second.symbols.find(symbolName);
    if (symbolIt == nsIt->second.symbols.end()) {
        return false;
    }
    
    symbolIt->second.isExported = true;
    return true;
}

std::vector<const NamespaceSymbolDefinition*> EnhancedNamespaceManager::getExportedSymbols(const std::string& namespacePath) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::vector<const NamespaceSymbolDefinition*> result;
    
    auto nsIt = namespaces_.find(namespacePath);
    if (nsIt == namespaces_.end()) {
        return result;
    }
    
    for (const auto& [name, symbol] : nsIt->second.symbols) {
        if (symbol.isExported) {
            result.push_back(&symbol);
        }
    }
    
    return result;
}

// ===== 实用工具 =====

std::vector<std::string> EnhancedNamespaceManager::parseNamespacePath(const std::string& fullPath) {
    std::vector<std::string> components;
    
    if (fullPath.empty()) {
        return components;
    }
    
    std::stringstream ss(fullPath);
    std::string component;
    
    while (std::getline(ss, component, '.')) {
        if (!component.empty()) {
            components.push_back(component);
        }
    }
    
    return components;
}

std::string EnhancedNamespaceManager::buildNamespacePath(const std::vector<std::string>& components) {
    if (components.empty()) {
        return "";
    }
    
    std::stringstream ss;
    for (size_t i = 0; i < components.size(); ++i) {
        if (i > 0) ss << ".";
        ss << components[i];
    }
    
    return ss.str();
}

std::string EnhancedNamespaceManager::getParentNamespacePath(const std::string& namespacePath) {
    if (namespacePath.empty()) {
        return "";
    }
    
    size_t lastDot = namespacePath.find_last_of('.');
    return (lastDot == std::string::npos) ? "" : namespacePath.substr(0, lastDot);
}

std::string EnhancedNamespaceManager::getNamespaceName(const std::string& namespacePath) {
    if (namespacePath.empty()) {
        return "";
    }
    
    size_t lastDot = namespacePath.find_last_of('.');
    return (lastDot == std::string::npos) ? namespacePath : namespacePath.substr(lastDot + 1);
}

bool EnhancedNamespaceManager::isValidNamespaceName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 检查命名空间名称的有效性
    std::regex nameRegex(R"([a-zA-Z_][a-zA-Z0-9_]*)");
    return std::regex_match(name, nameRegex);
}

bool EnhancedNamespaceManager::isValidSymbolName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 检查符号名称的有效性
    std::regex nameRegex(R"([a-zA-Z_][a-zA-Z0-9_]*)");
    return std::regex_match(name, nameRegex);
}

// ===== 调试和统计 =====

std::string EnhancedNamespaceManager::getNamespaceStatistics() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::stringstream ss;
    ss << "Namespace Statistics:\n";
    ss << "  Total namespaces: " << namespaces_.size() << "\n";
    
    size_t totalSymbols = 0;
    size_t mergedNamespaces = 0;
    std::unordered_map<NamespaceSymbolType, size_t> symbolTypeCounts;
    
    for (const auto& [path, ns] : namespaces_) {
        totalSymbols += ns.symbols.size();
        if (ns.isMerged) {
            mergedNamespaces++;
        }
        
        for (const auto& [name, symbol] : ns.symbols) {
            symbolTypeCounts[symbol.type]++;
        }
    }
    
    ss << "  Total symbols: " << totalSymbols << "\n";
    ss << "  Merged namespaces: " << mergedNamespaces << "\n";
    
    for (const auto& [type, count] : symbolTypeCounts) {
        ss << "  " << static_cast<int>(type) << " symbols: " << count << "\n";
    }
    
    return ss.str();
}

std::string EnhancedNamespaceManager::dumpNamespaces() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::stringstream ss;
    ss << "Namespace Dump:\n";
    
    for (const auto& [path, ns] : namespaces_) {
        ss << "  [" << (path.empty() ? "<root>" : path) << "] ";
        ss << "(" << ns.symbols.size() << " symbols";
        if (ns.isMerged) {
            ss << ", merged from " << ns.mergedFiles.size() << " files";
        }
        ss << ")\n";
        
        ss << "    Defined in: " << ns.sourceFile << "\n";
        if (!ns.childNamespaces.empty()) {
            ss << "    Children: ";
            bool first = true;
            for (const auto& child : ns.childNamespaces) {
                if (!first) ss << ", ";
                ss << child;
                first = false;
            }
            ss << "\n";
        }
    }
    
    return ss.str();
}

std::string EnhancedNamespaceManager::dumpSymbolTable(const std::string& namespacePath) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::stringstream ss;
    ss << "Symbol Table Dump:\n";
    
    auto dumpNamespace = [&](const std::string& nsPath, const NamespaceDefinition& ns) {
        ss << "  Namespace: " << (nsPath.empty() ? "<root>" : nsPath) << "\n";
        
        for (const auto& [name, symbol] : ns.symbols) {
            ss << "    " << name << " (" << static_cast<int>(symbol.type) << ")";
            if (!symbol.isPublic) ss << " [private]";
            if (symbol.isExported) ss << " [exported]";
            ss << " -> " << symbol.sourceFile << ":" << symbol.position.line << "\n";
        }
    };
    
    if (namespacePath.empty()) {
        // 转储所有命名空间
        for (const auto& [path, ns] : namespaces_) {
            dumpNamespace(path, ns);
        }
    } else {
        // 转储指定命名空间
        auto nsIt = namespaces_.find(namespacePath);
        if (nsIt != namespaces_.end()) {
            dumpNamespace(namespacePath, nsIt->second);
        }
    }
    
    return ss.str();
}

void EnhancedNamespaceManager::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    namespaces_.clear();
    namespaceStack_.clear();
    importAliases_.clear();
    namespaceStack_.push_back(""); // 重新添加根命名空间
}

// ===== 私有方法实现 =====

std::string EnhancedNamespaceManager::getCurrentNamespacePathInternal() const {
    return namespaceStack_.empty() ? "" : namespaceStack_.back();
}

NamespaceDefinition& EnhancedNamespaceManager::getOrCreateNamespace(const std::string& namespacePath, 
                                                                   const Position& position, 
                                                                   const std::string& sourceFile) {
    auto it = namespaces_.find(namespacePath);
    if (it != namespaces_.end()) {
        // 命名空间已存在，标记为合并
        it->second.isMerged = true;
        if (std::find(it->second.mergedFiles.begin(), it->second.mergedFiles.end(), sourceFile) == it->second.mergedFiles.end()) {
            it->second.mergedFiles.push_back(sourceFile);
        }
        return it->second;
    }
    
    // 创建新命名空间
    NamespaceDefinition newNs;
    newNs.name = getNamespaceName(namespacePath);
    newNs.fullPath = namespacePath;
    newNs.parentPath = getParentNamespacePath(namespacePath);
    newNs.position = position;
    newNs.sourceFile = sourceFile;
    
    // 确保父命名空间存在
    if (!newNs.parentPath.empty()) {
        createParentNamespaces(namespacePath, position, sourceFile);
    }
    
    auto result = namespaces_.emplace(namespacePath, std::move(newNs));
    
    // 更新父命名空间的子列表
    if (!newNs.parentPath.empty()) {
        auto parentIt = namespaces_.find(newNs.parentPath);
        if (parentIt != namespaces_.end()) {
            parentIt->second.childNamespaces.insert(namespacePath);
        }
    }
    
    return result.first->second;
}

void EnhancedNamespaceManager::createParentNamespaces(const std::string& namespacePath, 
                                                     const Position& position, 
                                                     const std::string& sourceFile) {
    auto components = parseNamespacePath(namespacePath);
    
    for (size_t i = 1; i < components.size(); ++i) {
        std::string parentPath = buildNamespacePath(std::vector<std::string>(components.begin(), components.begin() + i));
        
        if (namespaces_.find(parentPath) == namespaces_.end()) {
            getOrCreateNamespace(parentPath, position, sourceFile);
        }
    }
}

ConflictInfo EnhancedNamespaceManager::createConflictInfo(ConflictType type, const std::string& description,
                                                         const NamespaceSymbolDefinition& symbol1,
                                                         const NamespaceSymbolDefinition& symbol2) const {
    ConflictInfo conflict;
    conflict.type = type;
    conflict.description = description;
    conflict.symbolName = symbol1.name;
    conflict.namespacePath = symbol1.namespacePath;
    conflict.position1 = symbol1.position;
    conflict.position2 = symbol2.position;
    conflict.sourceFile1 = symbol1.sourceFile;
    conflict.sourceFile2 = symbol2.sourceFile;
    
    switch (type) {
        case ConflictType::TYPE_MISMATCH:
            conflict.suggestion = "Rename one of the symbols or ensure type consistency";
            break;
        case ConflictType::VISIBILITY_CONFLICT:
            conflict.suggestion = "Make visibility consistent";
            break;
        case ConflictType::EXPORT_CONFLICT:
            conflict.suggestion = "Resolve export ambiguity";
            break;
        default:
            conflict.suggestion = "Consider renaming or using qualified names";
            break;
    }
    
    return conflict;
}

bool EnhancedNamespaceManager::canMergeSymbols(const NamespaceSymbolDefinition& symbol1,
                                              const NamespaceSymbolDefinition& symbol2) const {
    // 只有相同类型、相同可见性和相同导出状态的符号才能合并
    return symbol1.type == symbol2.type && 
           symbol1.isPublic == symbol2.isPublic && 
           symbol1.isExported == symbol2.isExported &&
           symbol1.content == symbol2.content; // 内容也必须相同
}

std::string EnhancedNamespaceManager::generateFullSymbolName(const std::string& symbolName, const std::string& namespacePath) const {
    if (namespacePath.empty()) {
        return symbolName;
    }
    return namespacePath + "." + symbolName;
}

void EnhancedNamespaceManager::buildHierarchyRecursive(const std::string& namespacePath, int depth, std::stringstream& ss) const {
    // 输出当前命名空间
    for (int i = 0; i < depth; ++i) {
        ss << "  ";
    }
    
    if (namespacePath.empty()) {
        ss << "<root>\n";
    } else {
        ss << getNamespaceName(namespacePath) << "\n";
    }
    
    // 输出子命名空间
    auto nsIt = namespaces_.find(namespacePath);
    if (nsIt != namespaces_.end()) {
        for (const auto& child : nsIt->second.childNamespaces) {
            buildHierarchyRecursive(child, depth + 1, ss);
        }
    }
}

} // namespace chtl