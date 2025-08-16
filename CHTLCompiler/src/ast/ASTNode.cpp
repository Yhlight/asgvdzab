#include "ast/ASTNode.h"
#include <sstream>
#include <algorithm>
#include <functional>
#include <cctype>

namespace chtl {

void ASTNode::addChild(std::unique_ptr<ASTNode> child) {
    if (child) {
        child->setParent(this);
        children.push_back(std::move(child));
    }
}

void ASTNode::removeChild(ASTNode* child) {
    auto it = std::find_if(children.begin(), children.end(),
                           [child](const std::unique_ptr<ASTNode>& ptr) {
                               return ptr.get() == child;
                           });
    if (it != children.end()) {
        children.erase(it);
    }
}

ASTNode* ASTNode::getChild(size_t index) const {
    if (index < children.size()) {
        return children[index].get();
    }
    return nullptr;
}

size_t ASTNode::getChildCount() const {
    return children.size();
}

void ASTNode::setParent(ASTNode* p) {
    parent = p;
}

ASTNode* ASTNode::getParent() const {
    return parent;
}

void ASTNode::setMetadata(const std::string& key, const std::string& value) {
    metadata[key] = value;
}

std::string ASTNode::getMetadata(const std::string& key) const {
    auto it = metadata.find(key);
    if (it != metadata.end()) {
        return it->second;
    }
    return "";
}

bool ASTNode::hasMetadata(const std::string& key) const {
    return metadata.find(key) != metadata.end();
}

void ASTNode::removeMetadata(const std::string& key) {
    metadata.erase(key);
}

std::vector<ASTNode*> ASTNode::findNodesByType(ASTNodeType type) const {
    std::vector<ASTNode*> result;
    
    if (this->type == type) {
        result.push_back(const_cast<ASTNode*>(this));
    }
    
    for (const auto& child : children) {
        auto child_results = child->findNodesByType(type);
        result.insert(result.end(), child_results.begin(), child_results.end());
    }
    
    return result;
}

ASTNode* ASTNode::findFirstNodeByType(ASTNodeType type) const {
    if (this->type == type) {
        return const_cast<ASTNode*>(this);
    }
    
    for (const auto& child : children) {
        auto result = child->findFirstNodeByType(type);
        if (result) {
            return result;
        }
    }
    
    return nullptr;
}

ASTNode* ASTNode::findNodeById(const std::string& id) const {
    if (node_id == id) {
        return const_cast<ASTNode*>(this);
    }
    
    for (const auto& child : children) {
        auto result = child->findNodeById(id);
        if (result) {
            return result;
        }
    }
    
    return nullptr;
}

void ASTNode::traverse(std::function<void(ASTNode*)> visitor) {
    visitor(this);
    for (auto& child : children) {
        child->traverse(visitor);
    }
}

void ASTNode::traverseConst(std::function<void(const ASTNode*)> visitor) const {
    visitor(this);
    for (const auto& child : children) {
        // 递归调用const版本的traverse
        child->traverseConst(visitor);
    }
}

bool ASTNode::isValid() const {
    // 基类默认验证：检查基本属性
    if (type == ASTNodeType::UNKNOWN) {
        return false;
    }
    
    // 检查子节点
    for (const auto& child : children) {
        if (!child->isValid()) {
            return false;
        }
    }
    
    return true;
}

std::vector<std::string> ASTNode::validate() const {
    std::vector<std::string> errors;
    
    // 基类验证
    if (type == ASTNodeType::UNKNOWN) {
        errors.push_back("节点类型未知");
    }
    
    // 验证子节点
    for (const auto& child : children) {
        auto child_errors = child->validate();
        errors.insert(errors.end(), child_errors.begin(), child_errors.end());
    }
    
    return errors;
}

std::string ASTNode::serialize() const {
    std::ostringstream oss;
    oss << getNodeTypeName() << "{" << std::endl;
    oss << getIndentation(1) << "type: " << static_cast<int>(type) << std::endl;
    oss << getIndentation(1) << "line: " << line << std::endl;
    oss << getIndentation(1) << "column: " << column << std::endl;
    
    if (!children.empty()) {
        oss << getIndentation(1) << "children: [" << std::endl;
        for (size_t i = 0; i < children.size(); i++) {
            oss << getIndentation(2) << "child " << i << ": " << children[i]->serialize();
            if (i < children.size() - 1) {
                oss << ",";
            }
            oss << std::endl;
        }
        oss << getIndentation(1) << "]" << std::endl;
    }
    
    oss << "}";
    return oss.str();
}

std::string ASTNode::toDebugString() const {
    std::ostringstream oss;
    oss << getNodeTypeName() << " (ID: " << node_id << ", Line: " << line 
        << ", Column: " << column << ")";
    return oss.str();
}

std::unique_ptr<ASTNode> ASTNode::clone() const {
    // 基类克隆：创建相同类型的新节点
    auto cloned = std::make_unique<ASTNode>(type, line, column, start_position, end_position);
    cloned->node_id = node_id;
    cloned->source_file = source_file;
    cloned->metadata = metadata;
    
    // 克隆子节点
    for (const auto& child : children) {
        cloned->addChild(child->clone());
    }
    
    return cloned;
}

bool ASTNode::equals(const ASTNode* other) const {
    if (!other || type != other->type) {
        return false;
    }
    
    if (line != other->line || column != other->column || 
        start_position != other->start_position || end_position != other->end_position) {
        return false;
    }
    
    if (children.size() != other->children.size()) {
        return false;
    }
    
    for (size_t i = 0; i < children.size(); i++) {
        if (!children[i]->equals(other->children[i].get())) {
            return false;
        }
    }
    
    return true;
}

size_t ASTNode::getTotalNodeCount() const {
    size_t count = 1; // 包括自己
    for (const auto& child : children) {
        count += child->getTotalNodeCount();
    }
    return count;
}

size_t ASTNode::getNodeCountByType(ASTNodeType type) const {
    size_t count = (this->type == type) ? 1 : 0;
    for (const auto& child : children) {
        count += child->getNodeCountByType(type);
    }
    return count;
}

std::string ASTNode::getNodePath() const {
    std::vector<std::string> path;
    const ASTNode* current = this;
    
    while (current) {
        path.insert(path.begin(), current->getNodeTypeName());
        current = current->getParent();
    }
    
    std::ostringstream oss;
    for (size_t i = 0; i < path.size(); i++) {
        if (i > 0) oss << " -> ";
        oss << path[i];
    }
    
    return oss.str();
}

std::vector<std::string> ASTNode::getAncestorPath() const {
    std::vector<std::string> path;
    const ASTNode* current = parent;
    
    while (current) {
        path.insert(path.begin(), current->getNodeTypeName());
        current = current->getParent();
    }
    
    return path;
}

size_t ASTNode::getDepth() const {
    size_t depth = 0;
    const ASTNode* current = parent;
    
    while (current) {
        depth++;
        current = current->getParent();
    }
    
    return depth;
}

size_t ASTNode::getMaxDepth() const {
    size_t max_depth = 0;
    
    for (const auto& child : children) {
        max_depth = std::max(max_depth, child->getMaxDepth());
    }
    
    return max_depth + 1;
}

void ASTNode::clearChildren() {
    children.clear();
}

void ASTNode::clearMetadata() {
    metadata.clear();
}

void ASTNode::generateNodeId() {
    static size_t next_id = 0;
    node_id = "node_" + std::to_string(next_id++);
}

std::string ASTNode::getNodeTypeName() const {
    return getNodeTypeName(this->type);
}

std::string ASTNode::getNodeTypeName(ASTNodeType type) const {
    switch (type) {
        case ASTNodeType::ROOT: return "ROOT";
        case ASTNodeType::COMMENT: return "COMMENT";
        case ASTNodeType::TEXT_NODE: return "TEXT_NODE";
        case ASTNodeType::LITERAL: return "LITERAL";
        case ASTNodeType::HTML_ELEMENT: return "HTML_ELEMENT";
        case ASTNodeType::ATTRIBUTE: return "ATTRIBUTE";
        case ASTNodeType::ATTRIBUTE_LIST: return "ATTRIBUTE_LIST";
        case ASTNodeType::STYLE_BLOCK: return "STYLE_BLOCK";
        case ASTNodeType::CSS_RULE: return "CSS_RULE";
        case ASTNodeType::CSS_PROPERTY: return "CSS_PROPERTY";
        case ASTNodeType::CSS_VALUE: return "CSS_VALUE";
        case ASTNodeType::CSS_SELECTOR: return "CSS_SELECTOR";
        case ASTNodeType::CONTEXT_REFERENCE: return "CONTEXT_REFERENCE";
        case ASTNodeType::TEMPLATE_DECLARATION: return "TEMPLATE_DECLARATION";
        case ASTNodeType::STYLE_TEMPLATE: return "STYLE_TEMPLATE";
        case ASTNodeType::ELEMENT_TEMPLATE: return "ELEMENT_TEMPLATE";
        case ASTNodeType::VAR_TEMPLATE: return "VAR_TEMPLATE";
        case ASTNodeType::TEMPLATE_INHERITANCE: return "TEMPLATE_INHERITANCE";
        case ASTNodeType::TEMPLATE_USAGE: return "TEMPLATE_USAGE";
        case ASTNodeType::CUSTOM_DECLARATION: return "CUSTOM_DECLARATION";
        case ASTNodeType::CUSTOM_STYLE: return "CUSTOM_STYLE";
        case ASTNodeType::CUSTOM_ELEMENT: return "CUSTOM_ELEMENT";
        case ASTNodeType::CUSTOM_VAR: return "CUSTOM_VAR";
        case ASTNodeType::CUSTOM_SPECIALIZATION: return "CUSTOM_SPECIALIZATION";
        case ASTNodeType::DELETE_OPERATION: return "DELETE_OPERATION";
        case ASTNodeType::INSERT_OPERATION: return "INSERT_OPERATION";
        case ASTNodeType::REPLACE_OPERATION: return "REPLACE_OPERATION";
        case ASTNodeType::ORIGIN_DECLARATION: return "ORIGIN_DECLARATION";
        case ASTNodeType::ORIGIN_HTML: return "ORIGIN_HTML";
        case ASTNodeType::ORIGIN_STYLE: return "ORIGIN_STYLE";
        case ASTNodeType::ORIGIN_JAVASCRIPT: return "ORIGIN_JAVASCRIPT";
        case ASTNodeType::CONFIGURATION_DECLARATION: return "CONFIGURATION_DECLARATION";
        case ASTNodeType::NAME_DECLARATION: return "NAME_DECLARATION";
        case ASTNodeType::CONFIG_OPTION: return "CONFIG_OPTION";
        case ASTNodeType::IMPORT_DECLARATION: return "IMPORT_DECLARATION";
        case ASTNodeType::IMPORT_HTML: return "IMPORT_HTML";
        case ASTNodeType::IMPORT_STYLE: return "IMPORT_STYLE";
        case ASTNodeType::IMPORT_JAVASCRIPT: return "IMPORT_JAVASCRIPT";
        case ASTNodeType::IMPORT_CHTL: return "IMPORT_CHTL";
        case ASTNodeType::IMPORT_CJMOD: return "IMPORT_CJMOD";
        case ASTNodeType::NAMESPACE_DECLARATION: return "NAMESPACE_DECLARATION";
        case ASTNodeType::NAMESPACE_BODY: return "NAMESPACE_BODY";
        case ASTNodeType::CONSTRAINT_DECLARATION: return "CONSTRAINT_DECLARATION";
        case ASTNodeType::EXCEPT_CLAUSE: return "EXCEPT_CLAUSE";
        case ASTNodeType::SCRIPT_BLOCK: return "SCRIPT_BLOCK";
        case ASTNodeType::INFO_DECLARATION: return "INFO_DECLARATION";
        case ASTNodeType::EXPORT_DECLARATION: return "EXPORT_DECLARATION";
        case ASTNodeType::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN_TYPE";
    }
}

std::string ASTNode::getIndentation(size_t depth) const {
    return std::string(depth * 2, ' ');
}

} // namespace chtl