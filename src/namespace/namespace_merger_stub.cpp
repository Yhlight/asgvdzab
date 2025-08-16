#include "namespace/namespace_merger.hpp"
#include <sstream>
#include <algorithm>

namespace chtl {

// ========== NamespaceDefinition 实现 ==========

NamespaceDefinition::NamespaceDefinition(const std::string& name) : name_(name) {
}

NamespaceDefinition::~NamespaceDefinition() = default;

bool NamespaceDefinition::addElement(const NamespaceElement& element) {
    elements_[element.name][element.type] = element;
    return true;
}

bool NamespaceDefinition::removeElement(const std::string& elementName, NamespaceElementType type) {
    auto it = elements_.find(elementName);
    if (it != elements_.end()) {
        auto typeIt = it->second.find(type);
        if (typeIt != it->second.end()) {
            it->second.erase(typeIt);
            if (it->second.empty()) {
                elements_.erase(it);
            }
            return true;
        }
    }
    return false;
}

const NamespaceElement* NamespaceDefinition::getElement(const std::string& elementName, NamespaceElementType type) const {
    auto it = elements_.find(elementName);
    if (it != elements_.end()) {
        auto typeIt = it->second.find(type);
        if (typeIt != it->second.end()) {
            return &typeIt->second;
        }
    }
    return nullptr;
}

bool NamespaceDefinition::hasElement(const std::string& elementName, NamespaceElementType type) const {
    return getElement(elementName, type) != nullptr;
}

std::vector<NamespaceElement> NamespaceDefinition::getAllElements() const {
    std::vector<NamespaceElement> result;
    for (const auto& [name, typeMap] : elements_) {
        for (const auto& [type, element] : typeMap) {
            result.push_back(element);
        }
    }
    return result;
}

std::vector<NamespaceElement> NamespaceDefinition::getElementsByType(NamespaceElementType type) const {
    std::vector<NamespaceElement> result;
    for (const auto& [name, typeMap] : elements_) {
        auto it = typeMap.find(type);
        if (it != typeMap.end()) {
            result.push_back(it->second);
        }
    }
    return result;
}

bool NamespaceDefinition::addNestedNamespace(std::shared_ptr<NamespaceDefinition> childNamespace) {
    if (!childNamespace) return false;
    nestedNamespaces_[childNamespace->getName()] = childNamespace;
    return true;
}

std::shared_ptr<NamespaceDefinition> NamespaceDefinition::getNestedNamespace(const std::string& name) const {
    auto it = nestedNamespaces_.find(name);
    return (it != nestedNamespaces_.end()) ? it->second : nullptr;
}

std::vector<std::shared_ptr<NamespaceDefinition>> NamespaceDefinition::getNestedNamespaces() const {
    std::vector<std::shared_ptr<NamespaceDefinition>> result;
    for (const auto& [name, ns] : nestedNamespaces_) {
        result.push_back(ns);
    }
    return result;
}

std::vector<NamespaceConflict> NamespaceDefinition::checkConflicts(const NamespaceDefinition& other) const {
    std::vector<NamespaceConflict> conflicts;
    
    for (const auto& [name, typeMap] : elements_) {
        auto otherIt = other.elements_.find(name);
        if (otherIt != other.elements_.end()) {
            for (const auto& [type, element] : typeMap) {
                auto otherTypeIt = otherIt->second.find(type);
                if (otherTypeIt != otherIt->second.end()) {
                    NamespaceConflict conflict;
                    conflict.namespaceName = name_;
                    conflict.elementName = name;
                    conflict.conflictingElements = {element, otherTypeIt->second};
                    conflict.conflictType = "同名元素冲突";
                    conflict.resolutionSuggestion = "使用合并策略解决";
                    conflicts.push_back(conflict);
                }
            }
        }
    }
    
    return conflicts;
}

std::string NamespaceDefinition::getStatistics() const {
    std::stringstream ss;
    ss << "命名空间: " << name_ << "\n";
    ss << "元素数量: " << getAllElements().size() << "\n";
    ss << "嵌套命名空间: " << nestedNamespaces_.size() << "\n";
    return ss.str();
}

std::string NamespaceDefinition::generateElementKey(const std::string& name, NamespaceElementType type) const {
    return name + ":" + std::to_string(static_cast<int>(type));
}

// ========== NamespaceMerger 实现 ==========

NamespaceMerger::NamespaceMerger(const NamespaceMergeConfig& config) 
    : config_(config), mergedNamespaces_(0), resolvedConflicts_(0), failedMerges_(0) {
}

NamespaceMerger::~NamespaceMerger() = default;

void NamespaceMerger::setMergeConfig(const NamespaceMergeConfig& config) {
    config_ = config;
}

const NamespaceMergeConfig& NamespaceMerger::getMergeConfig() const {
    return config_;
}

bool NamespaceMerger::mergeNamespaces(NamespaceDefinition& target, const NamespaceDefinition& source) {
    auto sourceElements = source.getAllElements();
    
    for (const auto& element : sourceElements) {
        if (!mergeElement(target, element)) {
            failedMerges_++;
            return false;
        }
    }
    
    mergedNamespaces_++;
    return true;
}

std::shared_ptr<NamespaceDefinition> NamespaceMerger::mergeMultipleNamespaces(
    const std::vector<std::shared_ptr<NamespaceDefinition>>& namespaces) {
    if (namespaces.empty()) return nullptr;
    
    auto result = std::make_shared<NamespaceDefinition>(namespaces[0]->getName());
    
    for (const auto& ns : namespaces) {
        if (!mergeNamespaces(*result, *ns)) {
            return nullptr;
        }
    }
    
    return result;
}

std::vector<NamespaceConflict> NamespaceMerger::detectConflicts(
    const std::vector<std::shared_ptr<NamespaceDefinition>>& namespaces) {
    std::vector<NamespaceConflict> allConflicts;
    
    for (size_t i = 0; i < namespaces.size(); ++i) {
        for (size_t j = i + 1; j < namespaces.size(); ++j) {
            auto conflicts = namespaces[i]->checkConflicts(*namespaces[j]);
            allConflicts.insert(allConflicts.end(), conflicts.begin(), conflicts.end());
        }
    }
    
    lastConflicts_ = allConflicts;
    return allConflicts;
}

bool NamespaceMerger::resolveConflicts(const std::vector<NamespaceConflict>& conflicts,
                                      const std::unordered_map<std::string, std::string>& resolution) {
    // 简化实现：假设所有冲突都能解决
    resolvedConflicts_ += conflicts.size();
    return true;
}

std::string NamespaceMerger::getMergeStatistics() const {
    std::stringstream ss;
    ss << "合并统计:\n";
    ss << "  合并的命名空间: " << mergedNamespaces_ << "\n";
    ss << "  解决的冲突: " << resolvedConflicts_ << "\n";
    ss << "  失败的合并: " << failedMerges_ << "\n";
    return ss.str();
}

void NamespaceMerger::clearMergeHistory() {
    lastConflicts_.clear();
    mergedNamespaces_ = 0;
    resolvedConflicts_ = 0;
    failedMerges_ = 0;
}

bool NamespaceMerger::mergeElement(NamespaceDefinition& target, const NamespaceElement& element) {
    if (target.hasElement(element.name, element.type)) {
        auto* existing = target.getElement(element.name, element.type);
        auto strategy = getStrategyForType(element.type);
        
        if (strategy == MergeStrategy::FAIL_ON_CONFLICT) {
            return false;
        } else if (strategy == MergeStrategy::LAST_WINS) {
            target.addElement(element); // 覆盖现有元素
        }
        // FIRST_WINS情况下不做任何操作
    } else {
        target.addElement(element);
    }
    
    return true;
}

std::unique_ptr<NamespaceElement> NamespaceMerger::resolveElementConflict(
    const NamespaceElement& existing, 
    const NamespaceElement& newElement,
    MergeStrategy strategy) {
    
    switch (strategy) {
        case MergeStrategy::FIRST_WINS:
            return std::make_unique<NamespaceElement>(existing);
        case MergeStrategy::LAST_WINS:
            return std::make_unique<NamespaceElement>(newElement);
        case MergeStrategy::EXPLICIT_PRIORITY:
            return hasHigherPriority(newElement, existing) ? 
                   std::make_unique<NamespaceElement>(newElement) :
                   std::make_unique<NamespaceElement>(existing);
        default:
            return nullptr;
    }
}

MergeStrategy NamespaceMerger::getStrategyForType(NamespaceElementType type) const {
    auto it = config_.typeStrategies.find(type);
    return (it != config_.typeStrategies.end()) ? it->second : config_.defaultStrategy;
}

bool NamespaceMerger::hasHigherPriority(const NamespaceElement& elem1, const NamespaceElement& elem2) const {
    auto it1 = config_.filePriorities.find(elem1.sourceFile);
    auto it2 = config_.filePriorities.find(elem2.sourceFile);
    
    int priority1 = (it1 != config_.filePriorities.end()) ? it1->second : 0;
    int priority2 = (it2 != config_.filePriorities.end()) ? it2->second : 0;
    
    return priority1 > priority2;
}

bool NamespaceMerger::areElementsCompatible(const NamespaceElement& elem1, const NamespaceElement& elem2) const {
    return elem1.type == elem2.type && elem1.name == elem2.name;
}

// ========== NamespaceManager 实现 ==========

NamespaceManager::NamespaceManager() {
}

NamespaceManager::~NamespaceManager() = default;

bool NamespaceManager::registerNamespace(std::shared_ptr<NamespaceDefinition> namespaceDef) {
    if (!namespaceDef) return false;
    
    const std::string& name = namespaceDef->getName();
    namespacesByName_[name].push_back(namespaceDef);
    namespacesByPath_[name] = namespaceDef; // 简化：不支持完整路径
    
    updateNamespaceIndex(namespaceDef);
    return true;
}

std::shared_ptr<NamespaceDefinition> NamespaceManager::getNamespace(const std::string& namespacePath) {
    auto it = namespacesByPath_.find(namespacePath);
    return (it != namespacesByPath_.end()) ? it->second : nullptr;
}

std::shared_ptr<NamespaceDefinition> NamespaceManager::createNamespace(const std::string& namespacePath) {
    auto ns = std::make_shared<NamespaceDefinition>(namespacePath);
    registerNamespace(ns);
    return ns;
}

bool NamespaceManager::removeNamespace(const std::string& namespacePath) {
    auto it = namespacesByPath_.find(namespacePath);
    if (it != namespacesByPath_.end()) {
        namespacesByPath_.erase(it);
        
        // 从名称索引中移除
        auto nameIt = namespacesByName_.find(namespacePath);
        if (nameIt != namespacesByName_.end()) {
            auto& vec = nameIt->second;
            vec.erase(std::remove_if(vec.begin(), vec.end(),
                [&namespacePath](const std::weak_ptr<NamespaceDefinition>& weak) {
                    auto shared = weak.lock();
                    return !shared || shared->getName() == namespacePath;
                }), vec.end());
            
            if (vec.empty()) {
                namespacesByName_.erase(nameIt);
            }
        }
        
        return true;
    }
    return false;
}

bool NamespaceManager::performNamespaceMerging(const NamespaceMergeConfig& config) {
    merger_.setMergeConfig(config);
    
    for (auto& [name, namespaces] : namespacesByName_) {
        if (namespaces.size() > 1) {
            auto merged = merger_.mergeMultipleNamespaces(namespaces);
            if (merged) {
                namespacesByPath_[name] = merged;
                namespaces = {merged}; // 替换为合并后的命名空间
            } else {
                return false;
            }
        }
    }
    
    return true;
}

const NamespaceElement* NamespaceManager::findElement(const std::string& namespacePath, 
                                                     const std::string& elementName, 
                                                     NamespaceElementType type) {
    auto ns = getNamespace(namespacePath);
    return ns ? ns->getElement(elementName, type) : nullptr;
}

std::vector<std::shared_ptr<NamespaceDefinition>> NamespaceManager::getAllNamespaces() const {
    std::vector<std::shared_ptr<NamespaceDefinition>> result;
    for (const auto& [path, ns] : namespacesByPath_) {
        result.push_back(ns);
    }
    return result;
}

std::vector<std::shared_ptr<NamespaceDefinition>> NamespaceManager::getNamespacesByName(const std::string& name) const {
    auto it = namespacesByName_.find(name);
    return (it != namespacesByName_.end()) ? it->second : std::vector<std::shared_ptr<NamespaceDefinition>>{};
}

std::string NamespaceManager::exportStatistics() const {
    std::stringstream ss;
    ss << "命名空间管理器统计:\n";
    ss << "  总命名空间数: " << namespacesByPath_.size() << "\n";
    ss << "  唯一名称数: " << namespacesByName_.size() << "\n";
    return ss.str();
}

void NamespaceManager::clear() {
    namespacesByName_.clear();
    namespacesByPath_.clear();
    merger_.clearMergeHistory();
}

std::vector<std::string> NamespaceManager::parseNamespacePath(const std::string& path) const {
    std::vector<std::string> components;
    std::stringstream ss(path);
    std::string component;
    
    while (std::getline(ss, component, '.')) {
        if (!component.empty()) {
            components.push_back(component);
        }
    }
    
    return components;
}

std::string NamespaceManager::buildNamespacePath(const std::vector<std::string>& components) const {
    std::string result;
    for (size_t i = 0; i < components.size(); ++i) {
        if (i > 0) result += ".";
        result += components[i];
    }
    return result;
}

void NamespaceManager::updateNamespaceIndex(std::shared_ptr<NamespaceDefinition> namespaceDef) {
    // 简化实现，已在registerNamespace中处理
}

// ========== NamespaceUtils 实现 ==========

namespace NamespaceUtils {

std::string getElementTypeString(NamespaceElementType type) {
    switch (type) {
        case NamespaceElementType::TEMPLATE_STYLE: return "[Template] @Style";
        case NamespaceElementType::TEMPLATE_ELEMENT: return "[Template] @Element";
        case NamespaceElementType::TEMPLATE_VAR: return "[Template] @Var";
        case NamespaceElementType::CUSTOM_STYLE: return "[Custom] @Style";
        case NamespaceElementType::CUSTOM_ELEMENT: return "[Custom] @Element";
        case NamespaceElementType::CUSTOM_VAR: return "[Custom] @Var";
        case NamespaceElementType::VARIABLE: return "Variable";
        case NamespaceElementType::FUNCTION: return "Function";
        case NamespaceElementType::NESTED_NAMESPACE: return "Nested Namespace";
        default: return "Unknown";
    }
}

NamespaceElementType parseElementType(const std::string& typeString) {
    if (typeString == "[Template] @Style") return NamespaceElementType::TEMPLATE_STYLE;
    if (typeString == "[Template] @Element") return NamespaceElementType::TEMPLATE_ELEMENT;
    if (typeString == "[Template] @Var") return NamespaceElementType::TEMPLATE_VAR;
    if (typeString == "[Custom] @Style") return NamespaceElementType::CUSTOM_STYLE;
    if (typeString == "[Custom] @Element") return NamespaceElementType::CUSTOM_ELEMENT;
    if (typeString == "[Custom] @Var") return NamespaceElementType::CUSTOM_VAR;
    if (typeString == "Variable") return NamespaceElementType::VARIABLE;
    if (typeString == "Function") return NamespaceElementType::FUNCTION;
    if (typeString == "Nested Namespace") return NamespaceElementType::NESTED_NAMESPACE;
    return NamespaceElementType::VARIABLE;
}

std::string getMergeStrategyString(MergeStrategy strategy) {
    switch (strategy) {
        case MergeStrategy::FIRST_WINS: return "First Wins";
        case MergeStrategy::LAST_WINS: return "Last Wins";
        case MergeStrategy::EXPLICIT_PRIORITY: return "Explicit Priority";
        case MergeStrategy::FAIL_ON_CONFLICT: return "Fail on Conflict";
        case MergeStrategy::INTERACTIVE_RESOLVE: return "Interactive Resolve";
        default: return "Unknown";
    }
}

MergeStrategy parseMergeStrategy(const std::string& strategyString) {
    if (strategyString == "First Wins") return MergeStrategy::FIRST_WINS;
    if (strategyString == "Last Wins") return MergeStrategy::LAST_WINS;
    if (strategyString == "Explicit Priority") return MergeStrategy::EXPLICIT_PRIORITY;
    if (strategyString == "Fail on Conflict") return MergeStrategy::FAIL_ON_CONFLICT;
    if (strategyString == "Interactive Resolve") return MergeStrategy::INTERACTIVE_RESOLVE;
    return MergeStrategy::FAIL_ON_CONFLICT;
}

std::string generateConflictReport(const std::vector<NamespaceConflict>& conflicts) {
    std::stringstream ss;
    ss << "命名空间冲突报告:\n";
    ss << "冲突总数: " << conflicts.size() << "\n\n";
    
    for (size_t i = 0; i < conflicts.size(); ++i) {
        const auto& conflict = conflicts[i];
        ss << "冲突 " << (i + 1) << ":\n";
        ss << "  命名空间: " << conflict.namespaceName << "\n";
        ss << "  元素名: " << conflict.elementName << "\n";
        ss << "  冲突类型: " << conflict.conflictType << "\n";
        ss << "  建议: " << conflict.resolutionSuggestion << "\n";
        ss << "  冲突元素数量: " << conflict.conflictingElements.size() << "\n\n";
    }
    
    return ss.str();
}

bool isValidNamespaceName(const std::string& name) {
    if (name.empty()) return false;
    
    // 简化验证：检查是否包含有效字符
    for (char c : name) {
        if (!std::isalnum(c) && c != '_' && c != '-') {
            return false;
        }
    }
    
    return true;
}

bool isValidElementName(const std::string& name) {
    return isValidNamespaceName(name); // 使用相同的验证规则
}

} // namespace NamespaceUtils

} // namespace chtl