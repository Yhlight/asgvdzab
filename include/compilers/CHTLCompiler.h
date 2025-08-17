#pragma once

#include "../common/Types.h"
#include "../common/Token.h"
#include "../common/AST.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>

namespace CHTL {

/**
 * CHTL编译器 - 手写实现
 * 负责编译CHTL代码片段，特别是局部style相关的内容
 * 处理：元素节点、文本节点、属性、局部样式块、模板、自定义、导入等
 */
class CHTLCompiler {
private:
    Configuration config;
    std::vector<TokenPtr> tokens;
    size_t current_token_index;
    
    // 解析状态
    ASTNodePtr current_ast;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    
    // 模板和自定义存储
    std::map<std::string, ASTNodePtr> style_templates;
    std::map<std::string, ASTNodePtr> element_templates;
    std::map<std::string, ASTNodePtr> var_templates;
    std::map<std::string, ASTNodePtr> custom_styles;
    std::map<std::string, ASTNodePtr> custom_elements;
    std::map<std::string, ASTNodePtr> custom_vars;
    
    // 命名空间管理
    std::string current_namespace;
    std::map<std::string, std::map<std::string, ASTNodePtr>> namespaced_templates;
    
    // 自动生成的类名和ID管理
    std::map<std::string, std::string> auto_generated_classes;
    std::map<std::string, std::string> auto_generated_ids;
    int class_counter;
    int id_counter;
    
    // Token操作方法
    TokenPtr currentToken() const;
    TokenPtr peekToken(size_t offset = 1) const;
    TokenPtr advance();
    bool match(TokenType type);
    bool check(TokenType type) const;
    bool isAtEnd() const;
    
    // 解析方法
    ASTNodePtr parseProgram();
    ASTNodePtr parseElement();
    ASTNodePtr parseTextNode();
    ASTNodePtr parseStyleBlock();
    ASTNodePtr parseScriptBlock();
    ASTNodePtr parseAttribute();
    
    // 模板解析
    ASTNodePtr parseTemplate();
    ASTNodePtr parseTemplateStyle();
    ASTNodePtr parseTemplateElement();
    ASTNodePtr parseTemplateVar();
    
    // 自定义解析
    ASTNodePtr parseCustom();
    ASTNodePtr parseCustomStyle();
    ASTNodePtr parseCustomElement();
    ASTNodePtr parseCustomVar();
    
    // 原始嵌入解析
    ASTNodePtr parseOrigin();
    
    // 导入解析
    ASTNodePtr parseImport();
    
    // 命名空间解析
    ASTNodePtr parseNamespace();
    
    // 配置解析
    ASTNodePtr parseConfiguration();
    
    // CSS相关解析
    ASTNodePtr parseCSSRule();
    ASTNodePtr parseCSSSelector();
    ASTNodePtr parseCSSProperty();
    
    // 特殊操作解析
    ASTNodePtr parseInherit();
    ASTNodePtr parseDelete();
    ASTNodePtr parseInsert();
    
    // 工具方法
    std::string generateClassName();
    std::string generateId();
    std::string resolveTemplate(const std::string& template_name, TemplateType type);
    std::string resolveCustom(const std::string& custom_name, CustomType type);
    std::string resolveVariable(const std::string& var_group, const std::string& var_name);
    
    // 代码生成
    std::string generateHTML(ASTNodePtr node);
    std::string generateCSS(ASTNodePtr node);
    std::string generateInlineStyle(ASTNodePtr style_node);
    std::string generateGlobalCSS();
    void collectCSSFromNode(ASTNodePtr node, std::stringstream& css);
    
    // 错误处理
    void addError(const std::string& message);
    void addWarning(const std::string& message);
    std::string formatError(const std::string& message, TokenPtr token = nullptr);
    
public:
    CHTLCompiler();
    explicit CHTLCompiler(const Configuration& cfg);
    
    // 设置配置
    void setConfiguration(const Configuration& cfg);
    
    // 主要编译方法
    CompileResultPtr compile(const std::string& chtl_code);
    CompileResultPtr compile(const std::vector<TokenPtr>& token_stream);
    
    // 解析方法
    ASTNodePtr parse(const std::vector<TokenPtr>& token_stream);
    
    // 代码生成方法
    std::string generateCode(ASTNodePtr ast);
    
    // 模板管理
    void registerTemplate(const std::string& name, TemplateType type, ASTNodePtr node);
    ASTNodePtr getTemplate(const std::string& name, TemplateType type) const;
    
    // 自定义管理
    void registerCustom(const std::string& name, CustomType type, ASTNodePtr node);
    ASTNodePtr getCustom(const std::string& name, CustomType type) const;
    
    // 命名空间管理
    void enterNamespace(const std::string& name);
    void exitNamespace();
    std::string getCurrentNamespace() const { return current_namespace; }
    
    // 错误和警告
    std::vector<std::string> getErrors() const { return errors; }
    std::vector<std::string> getWarnings() const { return warnings; }
    void clearErrors() { errors.clear(); }
    void clearWarnings() { warnings.clear(); }
    
    // 重置编译器状态
    void reset();
    
    // 调试方法
    void dumpAST(ASTNodePtr node = nullptr) const;
    void setDebugMode(bool enabled);
};

} // namespace CHTL