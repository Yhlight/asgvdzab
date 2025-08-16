#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "ast/ASTNode.h"
#include "lexer/CHTLLexer.h"
#include "parser/AttributeSystem.h"

namespace chtl {

// 模板类型枚举
enum class TemplateType {
    STYLE_TEMPLATE,        // @Style 样式组模板
    ELEMENT_TEMPLATE,      // @Element 元素模板
    VAR_TEMPLATE           // @Var 变量组模板
};

// 模板继承类型
enum class InheritanceType {
    COMBINATION,           // 组合式继承
    EXPLICIT               // 显式继承（inherit关键字）
};

// 模板信息结构
struct TemplateInfo {
    std::string name;                      // 模板名称
    TemplateType type;                     // 模板类型
    std::string content;                   // 模板内容
    std::vector<std::string> dependencies; // 依赖的模板
    InheritanceType inheritance_type;      // 继承类型
    std::string parent_template;           // 父模板名称
    size_t line;                           // 行号
    size_t column;                         // 列号
    std::unordered_map<std::string, std::string> metadata; // 元数据
    
    TemplateInfo() : type(TemplateType::STYLE_TEMPLATE), 
                     inheritance_type(InheritanceType::COMBINATION), 
                     line(0), column(0) {}
};

// 模板系统配置
struct TemplateSystemConfig {
    bool enable_style_templates;           // 启用样式组模板
    bool enable_element_templates;         // 启用元素模板
    bool enable_var_templates;             // 启用变量组模板
    bool enable_template_inheritance;      // 启用模板继承
    bool strict_template_validation;       // 严格模板验证
    bool auto_resolve_dependencies;        // 自动解析依赖
    size_t max_template_name_length;       // 最大模板名称长度
    size_t max_template_content_length;    // 最大模板内容长度
    size_t max_inheritance_depth;          // 最大继承深度
    
    TemplateSystemConfig() : 
        enable_style_templates(true), enable_element_templates(true), 
        enable_var_templates(true), enable_template_inheritance(true),
        strict_template_validation(false), auto_resolve_dependencies(true),
        max_template_name_length(100), max_template_content_length(10000),
        max_inheritance_depth(10) {}
};

// 模板系统解析器类
class TemplateSystem {
public:
    TemplateSystem(const TemplateSystemConfig& config = TemplateSystemConfig());
    ~TemplateSystem();
    
    // 模板解析
    std::shared_ptr<ASTNode> parseTemplateDeclaration(const std::vector<Token>& tokens, size_t& position);
    TemplateInfo parseStyleTemplate(const std::vector<Token>& tokens, size_t& position);
    TemplateInfo parseElementTemplate(const std::vector<Token>& tokens, size_t& position);
    TemplateInfo parseVarTemplate(const std::vector<Token>& tokens, size_t& position);
    
    // 模板使用解析
    std::shared_ptr<ASTNode> parseTemplateUsage(const std::vector<Token>& tokens, size_t& position);
    std::vector<std::string> parseTemplateParameters(const std::vector<Token>& tokens, size_t& position);
    
    // 模板继承解析
    bool parseTemplateInheritance(const std::vector<Token>& tokens, size_t& position, TemplateInfo& template_info);
    std::vector<std::string> resolveTemplateDependencies(const std::string& template_name);
    bool validateInheritanceChain(const std::string& template_name, size_t depth = 0);
    
    // 模板管理
    void addTemplate(const TemplateInfo& template_info);
    void removeTemplate(const std::string& name);
    TemplateInfo* getTemplate(const std::string& name);
    std::vector<TemplateInfo> getAllTemplates() const;
    std::vector<TemplateInfo> getTemplatesByType(TemplateType type) const;
    bool hasTemplate(const std::string& name) const;
    
    // 模板验证
    bool validateTemplate(const TemplateInfo& template_info);
    std::vector<std::string> validateTemplateList(const std::vector<TemplateInfo>& templates);
    bool validateTemplateName(const std::string& name);
    bool validateTemplateContent(const std::string& content, TemplateType type);
    
    // 模板展开
    std::string expandStyleTemplate(const std::string& template_name, const std::vector<std::string>& parameters = {});
    std::string expandElementTemplate(const std::string& template_name, const std::vector<std::string>& parameters = {});
    std::string expandVarTemplate(const std::string& template_name, const std::string& variable_name);
    
    // 配置管理
    void setConfig(const TemplateSystemConfig& config);
    TemplateSystemConfig getConfig() const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error_message);
    
    // 调试和诊断
    std::string getDebugInfo() const;
    void enableDebugMode(bool enable);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
    bool isTemplateDeclaration(const std::vector<Token>& tokens, size_t position);
    bool isTemplateUsage(const std::vector<Token>& tokens, size_t position);
    TemplateType detectTemplateType(const std::vector<Token>& tokens, size_t position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    
    // 模板验证辅助方法
    bool isValidTemplateName(const std::string& name);
    bool isValidTemplateContent(const std::string& content);
    bool hasCircularDependency(const std::string& template_name, std::unordered_set<std::string>& visited);
    
    // 模板展开辅助方法
    std::string substituteTemplateVariables(const std::string& content, const std::vector<std::string>& parameters);
    std::string resolveTemplateReferences(const std::string& content);
    std::string mergeTemplateInheritance(const TemplateInfo& template_info);
};

} // namespace chtl