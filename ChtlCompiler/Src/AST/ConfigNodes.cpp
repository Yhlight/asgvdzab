#include "AST/ConfigNodes.h"

namespace Chtl {

// OriginBlockNode
OriginBlockNode::OriginBlockNode(const Token& token)
    : ASTNode(ASTNodeType::ORIGIN_BLOCK, token) {
}

void OriginBlockNode::accept(ASTVisitor* visitor) {
    visitor->visitOriginBlock(this);
}

// ConfigurationBlockNode
ConfigurationBlockNode::ConfigurationBlockNode(const Token& token)
    : ASTNode(ASTNodeType::CONFIGURATION_BLOCK, token) {
}

void ConfigurationBlockNode::addConfigItem(const std::string& key, ASTNodePtr value) {
    configItems_[key] = value;
}

ASTNodePtr ConfigurationBlockNode::getConfigItem(const std::string& key) const {
    auto it = configItems_.find(key);
    if (it != configItems_.end()) {
        return it->second;
    }
    return nullptr;
}

void ConfigurationBlockNode::accept(ASTVisitor* visitor) {
    visitor->visitConfigurationBlock(this);
}

// NameBlockNode
NameBlockNode::NameBlockNode(const Token& token)
    : ASTNode(ASTNodeType::NAME_BLOCK, token),
      optionCount_(3) {
}

void NameBlockNode::addNameGroup(const std::string& groupName, const std::vector<std::string>& names) {
    nameGroups_[groupName] = names;
}

void NameBlockNode::accept(ASTVisitor* visitor) {
    visitor->visitNameBlock(this);
}

// OriginTypeBlockNode
OriginTypeBlockNode::OriginTypeBlockNode(const Token& token)
    : ASTNode(ASTNodeType::ORIGIN_TYPE_BLOCK, token) {
}

void OriginTypeBlockNode::addOriginType(const std::string& typeName, const std::string& handlingType) {
    originTypes_[typeName] = handlingType;
}

void OriginTypeBlockNode::accept(ASTVisitor* visitor) {
    visitor->visitOriginTypeBlock(this);
}

// ImportStatementNode
ImportStatementNode::ImportStatementNode(const Token& token)
    : ASTNode(ASTNodeType::IMPORT_STATEMENT, token) {
}

void ImportStatementNode::accept(ASTVisitor* visitor) {
    visitor->visitImportStatement(this);
}

// ExportStatementNode
ExportStatementNode::ExportStatementNode(const Token& token)
    : ASTNode(ASTNodeType::EXPORT_STATEMENT, token) {
}

void ExportStatementNode::addExportItem(const std::string& type, const std::string& name) {
    exportItems_.push_back({type, name});
}

void ExportStatementNode::addExceptItem(const std::string& type, const std::string& name) {
    exceptItems_.push_back({type, name});
}

void ExportStatementNode::accept(ASTVisitor* visitor) {
    visitor->visitExportStatement(this);
}

// NamespaceBlockNode
NamespaceBlockNode::NamespaceBlockNode(const Token& token)
    : ASTNode(ASTNodeType::NAMESPACE_BLOCK, token) {
}

std::vector<ASTNodePtr> NamespaceBlockNode::getContent() const {
    return children_;
}

void NamespaceBlockNode::accept(ASTVisitor* visitor) {
    visitor->visitNamespaceBlock(this);
}

// InfoBlockNode
InfoBlockNode::InfoBlockNode(const Token& token)
    : ASTNode(ASTNodeType::INFO_BLOCK, token) {
}

void InfoBlockNode::addInfo(const std::string& key, const std::string& value) {
    info_[key] = value;
}

void InfoBlockNode::accept(ASTVisitor* visitor) {
    visitor->visitInfoBlock(this);
}

} // namespace Chtl