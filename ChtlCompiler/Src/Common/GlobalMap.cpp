#include "Common/GlobalMap.h"
#include <iostream>
#include <sstream>

namespace Chtl {

GlobalMap::GlobalMap() {
    // 初始化默认配置
    config_.indexInitialCount = 0;
    config_.disableNameGroup = false;
    config_.disableCustomOriginType = false;
    config_.debugMode = false;
    config_.optionCount = 3;
}

GlobalMap::~GlobalMap() {
}

bool GlobalMap::addStyleGroup(const StyleGroup& style) {
    std::string fullName = buildFullName(style.name, style.namespace_);
    if (styleGroups_.find(fullName) != styleGroups_.end()) {
        return false; // 已存在
    }
    styleGroups_[fullName] = style;
    return true;
}

std::optional<StyleGroup> GlobalMap::getStyleGroup(const std::string& name, 
                                                   const std::string& namespace_) const {
    std::string fullName = resolveFullName(name, namespace_);
    auto it = styleGroups_.find(fullName);
    if (it != styleGroups_.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool GlobalMap::addElementGroup(const ElementGroup& element) {
    std::string fullName = buildFullName(element.name, element.namespace_);
    if (elementGroups_.find(fullName) != elementGroups_.end()) {
        return false; // 已存在
    }
    elementGroups_[fullName] = element;
    return true;
}

std::optional<ElementGroup> GlobalMap::getElementGroup(const std::string& name,
                                                       const std::string& namespace_) const {
    std::string fullName = resolveFullName(name, namespace_);
    auto it = elementGroups_.find(fullName);
    if (it != elementGroups_.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool GlobalMap::addVarGroup(const VarGroup& var) {
    std::string fullName = buildFullName(var.name, var.namespace_);
    if (varGroups_.find(fullName) != varGroups_.end()) {
        return false; // 已存在
    }
    varGroups_[fullName] = var;
    return true;
}

std::optional<VarGroup> GlobalMap::getVarGroup(const std::string& name,
                                               const std::string& namespace_) const {
    std::string fullName = resolveFullName(name, namespace_);
    auto it = varGroups_.find(fullName);
    if (it != varGroups_.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool GlobalMap::addOriginBlock(const OriginBlock& origin) {
    std::string key = origin.name.empty() ? 
                     origin.type + "_" + std::to_string(originBlocks_.size()) : 
                     origin.name;
    
    if (originBlocks_.find(key) != originBlocks_.end()) {
        return false; // 已存在
    }
    originBlocks_[key] = origin;
    return true;
}

std::optional<OriginBlock> GlobalMap::getOriginBlock(const std::string& name,
                                                    const std::string& type) const {
    if (!name.empty()) {
        auto it = originBlocks_.find(name);
        if (it != originBlocks_.end()) {
            return it->second;
        }
    }
    
    // 如果指定了类型，按类型查找
    if (!type.empty()) {
        for (const auto& [key, block] : originBlocks_) {
            if (block.type == type) {
                return block;
            }
        }
    }
    
    return std::nullopt;
}

void GlobalMap::addImport(const ImportRecord& import) {
    imports_.push_back(import);
}

void GlobalMap::enterNamespace(const std::string& name) {
    namespaceStack_.push_back(name);
}

void GlobalMap::exitNamespace() {
    if (!namespaceStack_.empty()) {
        namespaceStack_.pop_back();
    }
}

std::string GlobalMap::getCurrentNamespace() const {
    if (namespaceStack_.empty()) {
        return "";
    }
    
    std::stringstream ss;
    for (size_t i = 0; i < namespaceStack_.size(); ++i) {
        if (i > 0) ss << ".";
        ss << namespaceStack_[i];
    }
    return ss.str();
}

std::string GlobalMap::resolveFullName(const std::string& name, 
                                      const std::string& namespace_) const {
    // 如果name已经包含命名空间（有.），直接返回
    if (name.find('.') != std::string::npos) {
        return name;
    }
    
    // 如果提供了命名空间，使用提供的
    if (!namespace_.empty()) {
        return buildFullName(name, namespace_);
    }
    
    // 否则使用当前命名空间
    return buildFullName(name, getCurrentNamespace());
}

void GlobalMap::clear() {
    styleGroups_.clear();
    elementGroups_.clear();
    varGroups_.clear();
    originBlocks_.clear();
    imports_.clear();
    namespaceStack_.clear();
}

void GlobalMap::dump() const {
    std::cout << "=== GlobalMap Dump ===\n";
    
    std::cout << "\nStyle Groups (" << styleGroups_.size() << "):\n";
    for (const auto& [name, style] : styleGroups_) {
        std::cout << "  " << name << " (template: " << style.isTemplate << ")\n";
    }
    
    std::cout << "\nElement Groups (" << elementGroups_.size() << "):\n";
    for (const auto& [name, element] : elementGroups_) {
        std::cout << "  " << name << " (template: " << element.isTemplate << ")\n";
    }
    
    std::cout << "\nVar Groups (" << varGroups_.size() << "):\n";
    for (const auto& [name, var] : varGroups_) {
        std::cout << "  " << name << " (template: " << var.isTemplate << ")\n";
    }
    
    std::cout << "\nOrigin Blocks (" << originBlocks_.size() << "):\n";
    for (const auto& [name, origin] : originBlocks_) {
        std::cout << "  " << name << " (type: " << origin.type << ")\n";
    }
    
    std::cout << "\nImports (" << imports_.size() << "):\n";
    for (const auto& import : imports_) {
        std::cout << "  " << import.type << " from " << import.path;
        if (!import.alias.empty()) {
            std::cout << " as " << import.alias;
        }
        std::cout << "\n";
    }
    
    std::cout << "\nCurrent Namespace: " << getCurrentNamespace() << "\n";
    std::cout << "=====================\n";
}

std::string GlobalMap::buildFullName(const std::string& name, 
                                    const std::string& namespace_) const {
    if (namespace_.empty()) {
        return name;
    }
    return namespace_ + "." + name;
}

} // namespace Chtl