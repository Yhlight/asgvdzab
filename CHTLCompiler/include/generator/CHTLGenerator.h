#pragma once

#include "ast/ASTNode.h"
#include "ast/CHTLASTNodes.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace chtl {

// 生成器配置
struct GeneratorConfig {
    bool enable_formatting;         // 是否启用代码格式化
    bool enable_minification;       // 是否启用代码压缩
    bool preserve_comments;         // 是否保留注释
    bool enable_debug;              // 是否启用调试模式
    std::string indent_style;       // 缩进样式 (space, tab)
    size_t indent_size;             // 缩进大小
    bool add_doctype;               // 是否添加DOCTYPE
    bool add_meta_charset;          // 是否添加meta charset
    bool add_viewport;              // 是否添加viewport meta
    bool enable_css_vendor_prefixes; // 是否启用CSS厂商前缀
    bool enable_js_polyfills;       // 是否启用JavaScript polyfill
    std::string output_encoding;    // 输出编码 (UTF-8, GBK等)
    
    GeneratorConfig() : 
        enable_formatting(true), enable_minification(false), preserve_comments(true),
        enable_debug(false), indent_style("space"), indent_size(2), add_doctype(true),
        add_meta_charset(true), add_viewport(true), enable_css_vendor_prefixes(false),
        enable_js_polyfills(false), output_encoding("UTF-8") {}
};

// 生成器输出类型
enum class OutputType {
    HTML,                   // HTML输出
    CSS,                    // CSS输出
    JAVASCRIPT,            // JavaScript输出
    COMBINED               // 组合输出
};

// 生成器统计信息
struct GeneratorStats {
    size_t total_nodes_processed;
    size_t html_lines_generated;
    size_t css_lines_generated;
    size_t js_lines_generated;
    size_t template_expansions;
    size_t custom_object_handlings;
    size_t generation_time_ms;
    size_t output_size_bytes;
};

// CHTL生成器类
class CHTLGenerator {
public:
    CHTLGenerator(const GeneratorConfig& config = GeneratorConfig());
    ~CHTLGenerator();
    
    // 生成方法
    std::string generateHTML(const ASTNode* root);
    std::string generateCSS(const ASTNode* root);
    std::string generateJavaScript(const ASTNode* root);
    std::string generateCombined(const ASTNode* root);
    
    // 文件生成方法
    bool generateHTMLFile(const ASTNode* root, const std::string& file_path);
    bool generateCSSFile(const ASTNode* root, const std::string& file_path);
    bool generateJavaScriptFile(const ASTNode* root, const std::string& file_path);
    bool generateCombinedFile(const ASTNode* root, const std::string& file_path);
    
    // 配置管理
    void setConfig(const GeneratorConfig& config);
    GeneratorConfig getConfig() const;
    
    // 统计信息
    GeneratorStats getStats() const;
    void resetStats();
    
    // 调试和诊断
    std::string getDebugInfo() const;
    void enableDebugMode(bool enable);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 核心生成方法
    std::string generateFromNode(const ASTNode* node, OutputType type);
    std::string generateElement(const HTMLElementNode* element);
    std::string generateAttribute(const AttributeNode* attribute);
    std::string generateStyleBlock(const StyleBlockNode* style_block);
    std::string generateCSSRule(const CSSRuleNode* css_rule);
    std::string generateCSSSelector(const CSSSelectorNode* selector);
    std::string generateCSSProperty(const CSSPropertyNode* property);
    
    // 模板系统生成
    std::string generateTemplateDeclaration(const TemplateDeclarationNode* template_decl);
    std::string generateTemplateUsage(const TemplateUsageNode* template_usage);
    std::string expandTemplate(const std::string& template_name, const std::string& template_type);
    std::string processTemplateInheritance(const TemplateDeclarationNode* template_decl);
    
    // 自定义系统生成
    std::string generateCustomDeclaration(const CustomDeclarationNode* custom_decl);
    std::string generateCustomSpecialization(const ASTNode* specialization);
    std::string processCustomObject(const std::string& custom_name, const std::string& custom_type);
    
    // 原始嵌入生成
    std::string generateOriginDeclaration(const OriginDeclarationNode* origin_decl);
    std::string processOriginContent(const std::string& origin_type, const std::string& content);
    
    // 导入和命名空间生成
    std::string generateImportDeclaration(const ImportDeclarationNode* import_decl);
    std::string generateNamespaceDeclaration(const NamespaceDeclarationNode* namespace_decl);
    std::string processImport(const std::string& import_type, const std::string& import_path);
    std::string processNamespace(const std::string& namespace_name);
    
    // 约束系统生成
    std::string generateConstraintDeclaration(const ConstraintDeclarationNode* constraint_decl);
    std::string processConstraints(const std::vector<std::unique_ptr<ASTNode>>& constraints);
    
    // 模块信息生成
    std::string generateInfoDeclaration(const InfoDeclarationNode* info_decl);
    std::string generateExportDeclaration(const ExportDeclarationNode* export_decl);
    
    // 辅助生成方法
    std::string generateComment(const CommentNode* comment);
    std::string generateTextNode(const TextNode* text_node);
    std::string generateLiteral(const LiteralNode* literal);
    std::string generateDeleteOperation(const DeleteOperationNode* delete_op);
    
    // 格式化方法
    std::string formatHTML(const std::string& html);
    std::string formatCSS(const std::string& css);
    std::string formatJavaScript(const std::string& js);
    std::string addIndentation(const std::string& code, size_t level);
    
    // 压缩方法
    std::string minifyHTML(const std::string& html);
    std::string minifyCSS(const std::string& css);
    std::string minifyJavaScript(const std::string& js);
    
    // 验证方法
    bool validateHTMLOutput(const std::string& html) const;
    bool validateCSSOutput(const std::string& css) const;
    bool validateJavaScriptOutput(const std::string& js) const;
    
    // 工具方法
    std::string escapeHTML(const std::string& text);
    std::string escapeCSS(const std::string& text);
    std::string escapeJavaScript(const std::string& text);
    std::string normalizeWhitespace(const std::string& text);
    std::string generateUniqueId(const std::string& prefix);
    
    // 依赖管理
    void collectDependencies(const ASTNode* node);
    std::vector<std::string> getDependencies() const;
    void resolveDependencies();
    
    // 错误处理
    void addGenerationError(const std::string& error_message);
    bool hasGenerationErrors() const;
    std::vector<std::string> getGenerationErrors() const;
    void clearGenerationErrors();
};

} // namespace chtl