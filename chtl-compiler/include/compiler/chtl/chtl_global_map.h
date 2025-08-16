#ifndef CHTL_COMPILER_CHTL_GLOBAL_MAP_H
#define CHTL_COMPILER_CHTL_GLOBAL_MAP_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "common/types.h"

namespace chtl {
namespace compiler {

// 前向声明
namespace ast {
    class ASTNode;
}

// 样式组模板
struct StyleTemplate {
    std::string name;
    std::vector<std::pair<std::string, std::string>> properties; // 属性名-值对
    SourceLocation location;
};

// 元素模板
struct ElementTemplate {
    std::string name;
    std::vector<std::shared_ptr<ast::ASTNode>> elements; // 模板内的元素
    SourceLocation location;
};

// 变量组模板
struct VarTemplate {
    std::string name;
    std::unordered_map<std::string, std::string> variables; // 变量名-值映射
    SourceLocation location;
};

// 自定义定义
struct CustomDefinition {
    std::string name;
    std::string baseElement;  // 基础元素
    std::vector<std::shared_ptr<ast::ASTNode>> elements; // 自定义内容
    
    // 特化操作
    struct Operation {
        enum Type { DELETE, INSERT_AFTER, INSERT_BEFORE, REPLACE, AT, TOP, BOTTOM };
        Type type;
        std::string target;      // 目标选择器或索引
        std::vector<std::shared_ptr<ast::ASTNode>> content; // 要插入的内容（对于insert操作）
    };
    std::vector<Operation> operations;
    
    SourceLocation location;
};

// 配置组
struct ConfigurationGroup {
    std::string name;
    std::unordered_map<std::string, std::string> settings;
    SourceLocation location;
};

// 导入信息
struct ImportInfo {
    std::string path;
    std::string alias;  // 可选的别名
    std::vector<std::string> exceptions; // except列表
    bool importAll;     // 是否导入所有（没有except）
    SourceLocation location;
};

// 命名空间信息
struct NamespaceInfo {
    std::string name;
    std::string alias;  // 可选的别名
    std::vector<std::shared_ptr<ast::ASTNode>> content;
    SourceLocation location;
};

// CHTL全局映射表
class CHTLGlobalMap {
public:
    CHTLGlobalMap();
    ~CHTLGlobalMap();
    
    // 样式组模板管理
    void addStyleTemplate(const std::string& name, const StyleTemplate& tmpl);
    StyleTemplate* getStyleTemplate(const std::string& name);
    bool hasStyleTemplate(const std::string& name) const;
    
    // 元素模板管理
    void addElementTemplate(const std::string& name, const ElementTemplate& tmpl);
    ElementTemplate* getElementTemplate(const std::string& name);
    bool hasElementTemplate(const std::string& name) const;
    
    // 变量组模板管理
    void addVarTemplate(const std::string& name, const VarTemplate& tmpl);
    VarTemplate* getVarTemplate(const std::string& name);
    bool hasVarTemplate(const std::string& name) const;
    std::string resolveVarTemplateValue(const std::string& templateName, const std::string& varName);
    
    // 自定义管理
    void addCustomDefinition(const std::string& name, const CustomDefinition& custom);
    CustomDefinition* getCustomDefinition(const std::string& name);
    bool hasCustomDefinition(const std::string& name) const;
    
    // 配置组管理
    void addConfigurationGroup(const std::string& name, const ConfigurationGroup& config);
    ConfigurationGroup* getConfigurationGroup(const std::string& name);
    bool hasConfigurationGroup(const std::string& name) const;
    std::string getConfigValue(const std::string& groupName, const std::string& key);
    
    // 导入管理
    void addImport(const ImportInfo& import);
    const std::vector<ImportInfo>& getImports() const { return imports_; }
    
    // 命名空间管理
    void addNamespace(const std::string& name, const NamespaceInfo& ns);
    NamespaceInfo* getNamespace(const std::string& name);
    bool hasNamespace(const std::string& name) const;
    
    // 全局样式和脚本收集
    void addGlobalStyle(const std::string& style);
    void addGlobalScript(const std::string& script);
    const std::vector<std::string>& getGlobalStyles() const { return globalStyles_; }
    const std::vector<std::string>& getGlobalScripts() const { return globalScripts_; }
    
    // 清空所有数据
    void clear();
    
private:
    // 模板存储
    std::unordered_map<std::string, StyleTemplate> styleTemplates_;
    std::unordered_map<std::string, ElementTemplate> elementTemplates_;
    std::unordered_map<std::string, VarTemplate> varTemplates_;
    
    // 自定义存储
    std::unordered_map<std::string, CustomDefinition> customDefinitions_;
    
    // 配置组存储
    std::unordered_map<std::string, ConfigurationGroup> configurations_;
    
    // 导入列表
    std::vector<ImportInfo> imports_;
    
    // 命名空间存储
    std::unordered_map<std::string, NamespaceInfo> namespaces_;
    
    // 全局样式和脚本
    std::vector<std::string> globalStyles_;
    std::vector<std::string> globalScripts_;
};

} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_CHTL_GLOBAL_MAP_H