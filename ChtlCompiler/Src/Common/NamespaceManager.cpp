#include "Common/NamespaceManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace Chtl {

NamespaceManager::NamespaceManager() {
}

NamespaceManager::~NamespaceManager() {
}

void NamespaceManager::registerNamespace(const std::string& name, const NamespaceContent& content) {
    namespaces_[name].push_back(content);
    nestedCache_.clear();  // 清空缓存
}

bool NamespaceManager::mergeNamespace(const std::string& name, const NamespaceContent& content) {
    // 检查冲突
    auto conflicts = checkConflicts(name, content);
    if (!conflicts.empty()) {
        // 报告冲突
        for (const auto& conflict : conflicts) {
            std::cerr << "Namespace conflict in '" << name << "': " 
                     << conflict.itemType << " '" << conflict.itemName << "' "
                     << "defined in both " << conflict.file1 << ":" << conflict.line1
                     << " and " << conflict.file2 << ":" << conflict.line2 << std::endl;
        }
        return false;
    }
    
    // 注册命名空间
    registerNamespace(name, content);
    
    // 更新合并后的命名空间
    if (namespaces_.find(name) != namespaces_.end()) {
        mergedNamespaces_[name] = mergeContents(namespaces_[name]);
    }
    
    return true;
}

std::vector<ConflictInfo> NamespaceManager::checkConflicts(const std::string& name, const NamespaceContent& content) {
    std::vector<ConflictInfo> conflicts;
    
    auto it = namespaces_.find(name);
    if (it == namespaces_.end()) {
        return conflicts;  // 没有同名命名空间，不存在冲突
    }
    
    // 检查与每个已存在的同名命名空间的冲突
    for (const auto& existing : it->second) {
        // 检查样式模板冲突
        for (const auto& [itemName, _] : content.styleTemplates) {
            if (existing.styleTemplates.find(itemName) != existing.styleTemplates.end()) {
                ConflictInfo conflict;
                conflict.itemName = itemName;
                conflict.itemType = "style template";
                conflict.namespace1 = name;
                conflict.file1 = existing.sourceFile;
                conflict.line1 = existing.startLine;
                conflict.namespace2 = name;
                conflict.file2 = content.sourceFile;
                conflict.line2 = content.startLine;
                conflicts.push_back(conflict);
            }
        }
        
        // 检查元素模板冲突
        for (const auto& [itemName, _] : content.elementTemplates) {
            if (existing.elementTemplates.find(itemName) != existing.elementTemplates.end()) {
                ConflictInfo conflict;
                conflict.itemName = itemName;
                conflict.itemType = "element template";
                conflict.namespace1 = name;
                conflict.file1 = existing.sourceFile;
                conflict.line1 = existing.startLine;
                conflict.namespace2 = name;
                conflict.file2 = content.sourceFile;
                conflict.line2 = content.startLine;
                conflicts.push_back(conflict);
            }
        }
        
        // 检查变量模板冲突
        for (const auto& [itemName, _] : content.varTemplates) {
            if (existing.varTemplates.find(itemName) != existing.varTemplates.end()) {
                ConflictInfo conflict;
                conflict.itemName = itemName;
                conflict.itemType = "variable template";
                conflict.namespace1 = name;
                conflict.file1 = existing.sourceFile;
                conflict.line1 = existing.startLine;
                conflict.namespace2 = name;
                conflict.file2 = content.sourceFile;
                conflict.line2 = content.startLine;
                conflicts.push_back(conflict);
            }
        }
        
        // 检查自定义样式冲突
        for (const auto& [itemName, _] : content.customStyles) {
            if (existing.customStyles.find(itemName) != existing.customStyles.end()) {
                ConflictInfo conflict;
                conflict.itemName = itemName;
                conflict.itemType = "custom style";
                conflict.namespace1 = name;
                conflict.file1 = existing.sourceFile;
                conflict.line1 = existing.startLine;
                conflict.namespace2 = name;
                conflict.file2 = content.sourceFile;
                conflict.line2 = content.startLine;
                conflicts.push_back(conflict);
            }
        }
        
        // 检查自定义元素冲突
        for (const auto& [itemName, _] : content.customElements) {
            if (existing.customElements.find(itemName) != existing.customElements.end()) {
                ConflictInfo conflict;
                conflict.itemName = itemName;
                conflict.itemType = "custom element";
                conflict.namespace1 = name;
                conflict.file1 = existing.sourceFile;
                conflict.line1 = existing.startLine;
                conflict.namespace2 = name;
                conflict.file2 = content.sourceFile;
                conflict.line2 = content.startLine;
                conflicts.push_back(conflict);
            }
        }
        
        // 检查自定义变量冲突
        for (const auto& [itemName, _] : content.customVars) {
            if (existing.customVars.find(itemName) != existing.customVars.end()) {
                ConflictInfo conflict;
                conflict.itemName = itemName;
                conflict.itemType = "custom variable";
                conflict.namespace1 = name;
                conflict.file1 = existing.sourceFile;
                conflict.line1 = existing.startLine;
                conflict.namespace2 = name;
                conflict.file2 = content.sourceFile;
                conflict.line2 = content.startLine;
                conflicts.push_back(conflict);
            }
        }
    }
    
    return conflicts;
}

const NamespaceContent* NamespaceManager::getNamespace(const std::string& name) const {
    auto it = mergedNamespaces_.find(name);
    if (it != mergedNamespaces_.end()) {
        return &it->second;
    }
    return nullptr;
}

const NamespaceContent* NamespaceManager::getNestedNamespace(const std::string& path) const {
    // 检查缓存
    auto cacheIt = nestedCache_.find(path);
    if (cacheIt != nestedCache_.end()) {
        return &cacheIt->second;
    }
    
    // 解析路径
    auto parts = parseNamespacePath(path);
    if (parts.empty()) {
        return nullptr;
    }
    
    // TODO: 实现嵌套命名空间查找
    // 暂时只支持一级命名空间
    if (parts.size() == 1) {
        return getNamespace(parts[0]);
    }
    
    return nullptr;
}

std::vector<std::string> NamespaceManager::parseNamespacePath(const std::string& path) const {
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string part;
    
    while (std::getline(ss, part, '.')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }
    
    return parts;
}

NamespaceContent NamespaceManager::createFromASTNode(NamespaceBlockNode* node, const std::string& sourceFile) {
    NamespaceContent content;
    content.sourceFile = sourceFile;
    content.startLine = node->getLine();
    content.endLine = node->getLine();  // TODO: 计算实际结束行
    
    // TODO: 从AST节点提取内容
    // 这需要遍历命名空间节点的子节点，提取模板和自定义定义
    
    return content;
}

void NamespaceManager::addToGlobalMap(const std::string& namespaceName, GlobalMap& globalMap) {
    const NamespaceContent* content = getNamespace(namespaceName);
    if (!content) {
        return;
    }
    
    // 添加模板到全局符号表
    for (const auto& [name, def] : content->styleTemplates) {
        globalMap.addStyleGroup(def);
    }
    for (const auto& [name, def] : content->elementTemplates) {
        globalMap.addElementTemplate(def);
    }
    for (const auto& [name, group] : content->varTemplates) {
        globalMap.addVarGroup(group);
    }
    
    // 添加自定义到全局符号表
    for (const auto& [name, def] : content->customStyles) {
        globalMap.addCustomStyle(def);
    }
    for (const auto& [name, def] : content->customElements) {
        globalMap.addCustomElement(def);
    }
    for (const auto& [name, group] : content->customVars) {
        globalMap.addVarGroup(group);
    }
}

bool NamespaceManager::itemExists(const NamespaceContent& content, const std::string& itemType, const std::string& itemName) const {
    if (itemType == "style template") {
        return content.styleTemplates.find(itemName) != content.styleTemplates.end();
    } else if (itemType == "element template") {
        return content.elementTemplates.find(itemName) != content.elementTemplates.end();
    } else if (itemType == "var template") {
        return content.varTemplates.find(itemName) != content.varTemplates.end();
    } else if (itemType == "custom style") {
        return content.customStyles.find(itemName) != content.customStyles.end();
    } else if (itemType == "custom element") {
        return content.customElements.find(itemName) != content.customElements.end();
    } else if (itemType == "custom var") {
        return content.customVars.find(itemName) != content.customVars.end();
    }
    return false;
}

std::vector<std::string> NamespaceManager::getAllNamespaces() const {
    std::vector<std::string> names;
    for (const auto& [name, _] : mergedNamespaces_) {
        names.push_back(name);
    }
    return names;
}

void NamespaceManager::clear() {
    namespaces_.clear();
    mergedNamespaces_.clear();
    nestedCache_.clear();
}

void NamespaceManager::dump() const {
    std::cout << "=== Namespace Manager ===" << std::endl;
    for (const auto& [name, contents] : namespaces_) {
        std::cout << "Namespace: " << name << " (" << contents.size() << " definitions)" << std::endl;
        for (const auto& content : contents) {
            std::cout << "  From: " << content.sourceFile << ":" << content.startLine << std::endl;
            std::cout << "  Style Templates: " << content.styleTemplates.size() << std::endl;
            std::cout << "  Element Templates: " << content.elementTemplates.size() << std::endl;
            std::cout << "  Var Templates: " << content.varTemplates.size() << std::endl;
            std::cout << "  Custom Styles: " << content.customStyles.size() << std::endl;
            std::cout << "  Custom Elements: " << content.customElements.size() << std::endl;
            std::cout << "  Custom Vars: " << content.customVars.size() << std::endl;
        }
    }
}

// 私有辅助方法实现

bool NamespaceManager::checkItemConflict(const NamespaceContent& content1, const NamespaceContent& content2,
                                        const std::string& itemType, const std::string& itemName,
                                        std::vector<ConflictInfo>& conflicts) {
    if (itemExists(content1, itemType, itemName) && itemExists(content2, itemType, itemName)) {
        ConflictInfo conflict;
        conflict.itemName = itemName;
        conflict.itemType = itemType;
        conflict.file1 = content1.sourceFile;
        conflict.line1 = content1.startLine;
        conflict.file2 = content2.sourceFile;
        conflict.line2 = content2.startLine;
        conflicts.push_back(conflict);
        return true;
    }
    return false;
}

void NamespaceManager::mergeStyleTemplates(std::unordered_map<std::string, StyleGroup>& target,
                                          const std::unordered_map<std::string, StyleGroup>& source) {
    for (const auto& [name, def] : source) {
        target[name] = def;  // 简单覆盖，因为已经检查过冲突
    }
}

void NamespaceManager::mergeElementTemplates(std::unordered_map<std::string, ElementTemplate>& target,
                                            const std::unordered_map<std::string, ElementTemplate>& source) {
    for (const auto& [name, def] : source) {
        target[name] = def;  // 简单覆盖，因为已经检查过冲突
    }
}

void NamespaceManager::mergeCustomStyles(std::unordered_map<std::string, CustomStyle>& target,
                                        const std::unordered_map<std::string, CustomStyle>& source) {
    for (const auto& [name, def] : source) {
        target[name] = def;  // 简单覆盖，因为已经检查过冲突
    }
}

void NamespaceManager::mergeCustomElements(std::unordered_map<std::string, CustomElement>& target,
                                          const std::unordered_map<std::string, CustomElement>& source) {
    for (const auto& [name, def] : source) {
        target[name] = def;  // 简单覆盖，因为已经检查过冲突
    }
}

void NamespaceManager::mergeVars(std::unordered_map<std::string, VarGroup>& target,
                                const std::unordered_map<std::string, VarGroup>& source) {
    for (const auto& [name, group] : source) {
        target[name] = group;  // 简单覆盖，因为已经检查过冲突
    }
}

NamespaceContent NamespaceManager::mergeContents(const std::vector<NamespaceContent>& contents) {
    NamespaceContent merged;
    
    if (!contents.empty()) {
        merged.sourceFile = contents[0].sourceFile;
        merged.startLine = contents[0].startLine;
        merged.endLine = contents[0].endLine;
    }
    
    for (const auto& content : contents) {
        mergeStyleTemplates(merged.styleTemplates, content.styleTemplates);
        mergeElementTemplates(merged.elementTemplates, content.elementTemplates);
        mergeVars(merged.varTemplates, content.varTemplates);
        mergeCustomStyles(merged.customStyles, content.customStyles);
        mergeCustomElements(merged.customElements, content.customElements);
        mergeVars(merged.customVars, content.customVars);
        
        // 合并导出项
        merged.exportedItems.insert(content.exportedItems.begin(), content.exportedItems.end());
        
        // 更新行号范围
        merged.startLine = std::min(merged.startLine, content.startLine);
        merged.endLine = std::max(merged.endLine, content.endLine);
    }
    
    return merged;
}

void NamespaceManager::buildNestedNamespace(const std::vector<std::string>& path, 
                                           NamespaceContent& parent,
                                           size_t depth) {
    // TODO: 实现嵌套命名空间构建
    // 这需要递归地构建嵌套的命名空间结构
}

} // namespace Chtl