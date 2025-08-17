#ifndef CHTL_AST_CONFIGNODES_H
#define CHTL_AST_CONFIGNODES_H

#include "AST/ASTNode.h"
#include <unordered_map>

namespace Chtl {

// [Origin] 原始嵌入块
class OriginBlockNode : public ASTNode {
public:
    enum OriginType {
        HTML,           // @Html
        STYLE,          // @Style
        JAVASCRIPT,     // @JavaScript
        CUSTOM          // 自定义类型
    };
    
    OriginBlockNode(const Token& token);
    
    OriginType getOriginType() const { return originType_; }
    void setOriginType(OriginType type) { originType_ = type; }
    
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    const std::string& getCustomTypeName() const { return customTypeName_; }
    void setCustomTypeName(const std::string& name) { customTypeName_ = name; }
    
    const std::string& getContent() const { return content_; }
    void setContent(const std::string& content) { content_ = content; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    OriginType originType_;
    std::string name_;           // 可选的名称
    std::string customTypeName_; // 自定义类型名称
    std::string content_;        // 原始内容
};

// [Configuration] 配置块
class ConfigurationBlockNode : public ASTNode {
public:
    ConfigurationBlockNode(const Token& token);
    
    // 添加配置项
    void addConfigItem(const std::string& key, ASTNodePtr value);
    const std::unordered_map<std::string, ASTNodePtr>& getConfigItems() const { return configItems_; }
    
    // 获取特定配置值
    ASTNodePtr getConfigItem(const std::string& key) const;
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::unordered_map<std::string, ASTNodePtr> configItems_;
};

// [Name] 名称组配置块
class NameBlockNode : public ASTNode {
public:
    NameBlockNode(const Token& token);
    
    // 添加名称组
    void addNameGroup(const std::string& groupName, const std::vector<std::string>& names);
    const std::unordered_map<std::string, std::vector<std::string>>& getNameGroups() const { return nameGroups_; }
    
    // 获取option-count配置
    int getOptionCount() const { return optionCount_; }
    void setOptionCount(int count) { optionCount_ = count; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::unordered_map<std::string, std::vector<std::string>> nameGroups_;
    int optionCount_ = 3;  // 默认值
};

// [OriginType] 自定义原始类型块
class OriginTypeBlockNode : public ASTNode {
public:
    OriginTypeBlockNode(const Token& token);
    
    // 添加自定义类型
    void addOriginType(const std::string& typeName, const std::string& handlingType);
    const std::unordered_map<std::string, std::string>& getOriginTypes() const { return originTypes_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::unordered_map<std::string, std::string> originTypes_;  // 类型名 -> 处理方式
};

// [Import] 导入语句
class ImportStatementNode : public ASTNode {
public:
    enum ImportType {
        HTML,           // @Html
        STYLE,          // @Style (CSS)
        JAVASCRIPT,     // @JavaScript
        CHTL,           // @Chtl
        CJMOD           // @CJmod
    };
    
    ImportStatementNode(const Token& token);
    
    ImportType getImportType() const { return importType_; }
    void setImportType(ImportType type) { importType_ = type; }
    
    const std::string& getPath() const { return path_; }
    void setPath(const std::string& path) { path_ = path; }
    
    const std::string& getAlias() const { return alias_; }
    void setAlias(const std::string& alias) { alias_ = alias; }
    
    // 针对CHTL导入的特定项
    const std::string& getTargetName() const { return targetName_; }
    void setTargetName(const std::string& name) { targetName_ = name; }
    
    const std::string& getTargetType() const { return targetType_; }
    void setTargetType(const std::string& type) { targetType_ = type; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    ImportType importType_;
    std::string path_;
    std::string alias_;      // as 别名（可选）
    std::string targetName_; // 导入的特定项名称（CHTL专用）
    std::string targetType_; // [Custom]/@Style等（CHTL专用）
};

// [Export] 导出语句
class ExportStatementNode : public ASTNode {
public:
    enum ExportType {
        ALL,            // 导出所有
        SPECIFIC        // 导出特定项
    };
    
    ExportStatementNode(const Token& token);
    
    ExportType getExportType() const { return exportType_; }
    void setExportType(ExportType type) { exportType_ = type; }
    
    // 添加要导出的项
    void addExportItem(const std::string& type, const std::string& name);
    const std::vector<std::pair<std::string, std::string>>& getExportItems() const { return exportItems_; }
    
    // 添加排除项 (except)
    void addExceptItem(const std::string& type, const std::string& name = "");
    const std::vector<std::pair<std::string, std::string>>& getExceptItems() const { return exceptItems_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    ExportType exportType_;
    std::vector<std::pair<std::string, std::string>> exportItems_;  // type -> name
    std::vector<std::pair<std::string, std::string>> exceptItems_;  // except列表
};

// [Namespace] 命名空间块
class NamespaceBlockNode : public ASTNode {
public:
    NamespaceBlockNode(const Token& token);
    
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    // 获取命名空间内的所有内容
    std::vector<ASTNodePtr> getContent() const;
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::string name_;
};

// [Info] 信息块（用于CMOD）
class InfoBlockNode : public ASTNode {
public:
    InfoBlockNode(const Token& token);
    
    void addInfo(const std::string& key, const std::string& value);
    const std::unordered_map<std::string, std::string>& getInfo() const { return info_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::unordered_map<std::string, std::string> info_;
};

} // namespace Chtl

#endif // CHTL_AST_CONFIGNODES_H