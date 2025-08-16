#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "ast/ASTNode.h"
#include "lexer/CHTLLexer.h"
#include "parser/AttributeSystem.h"
#include "statemachine/CHTLStateMachine.h"

namespace chtl {

// 原始嵌入类型枚举
enum class OriginType {
    HTML_ORIGIN,           // @Html 原始HTML代码
    STYLE_ORIGIN,          // @Style 原始CSS代码
    JAVASCRIPT_ORIGIN      // @JavaScript 原始JavaScript代码
};

// 原始嵌入信息结构
struct OriginInfo {
    std::string name;                      // 嵌入名称（可选）
    OriginType type;                       // 嵌入类型
    std::string content;                   // 原始代码内容
    std::string target_element;            // 目标元素（用于增强原始嵌入）
    size_t line;                           // 行号
    size_t column;                         // 列号
    std::unordered_map<std::string, std::string> metadata; // 元数据
    
    OriginInfo() : type(OriginType::HTML_ORIGIN), line(0), column(0) {}
};

// 原始嵌入系统配置
struct OriginSystemConfig {
    bool enable_html_origin;               // 启用HTML原始嵌入
    bool enable_style_origin;              // 启用CSS原始嵌入
    bool enable_javascript_origin;         // 启用JavaScript原始嵌入
    bool enable_enhanced_origin;           // 启用增强原始嵌入
    bool strict_content_validation;        // 严格内容验证
    bool auto_escape_content;              // 自动转义内容
    size_t max_origin_name_length;         // 最大嵌入名称长度
    size_t max_origin_content_length;      // 最大嵌入内容长度
    bool preserve_whitespace;              // 保留空白字符
    bool enable_debug_mode;                // 启用调试模式
    
    OriginSystemConfig() : 
        enable_html_origin(true), enable_style_origin(true), 
        enable_javascript_origin(true), enable_enhanced_origin(true),
        strict_content_validation(false), auto_escape_content(false),
        max_origin_name_length(100), max_origin_content_length(100000),
        preserve_whitespace(true), enable_debug_mode(false) {}
};

// 原始嵌入系统解析器类
class OriginSystem {
public:
    OriginSystem(const OriginSystemConfig& config = OriginSystemConfig());
    ~OriginSystem();
    
    // 原始嵌入声明解析
    std::shared_ptr<ASTNode> parseOriginDeclaration(const std::vector<Token>& tokens, size_t& position);
    OriginInfo parseHtmlOrigin(const std::vector<Token>& tokens, size_t& position);
    OriginInfo parseStyleOrigin(const std::vector<Token>& tokens, size_t& position);
    OriginInfo parseJavaScriptOrigin(const std::vector<Token>& tokens, size_t& position);
    
    // 增强原始嵌入解析
    OriginInfo parseEnhancedOrigin(const std::vector<Token>& tokens, size_t& position);
    std::string parseOriginTarget(const std::vector<Token>& tokens, size_t& position);
    
    // 原始嵌入使用解析
    std::shared_ptr<ASTNode> parseOriginUsage(const std::vector<Token>& tokens, size_t& position);
    std::string parseOriginReference(const std::vector<Token>& tokens, size_t& position);
    
    // 原始嵌入管理
    void addOrigin(const OriginInfo& origin_info);
    void removeOrigin(const std::string& name);
    OriginInfo* getOrigin(const std::string& name);
    std::vector<OriginInfo> getAllOrigins() const;
    std::vector<OriginInfo> getOriginsByType(OriginType type) const;
    bool hasOrigin(const std::string& name) const;
    
    // 原始嵌入验证
    bool validateOrigin(const OriginInfo& origin_info);
    std::vector<std::string> validateOriginList(const std::vector<OriginInfo>& origins);
    bool validateOriginName(const std::string& name);
    bool validateOriginContent(const std::string& content, OriginType type);
    
    // 原始嵌入处理
    std::string processHtmlOrigin(const std::string& origin_name, const std::string& target_element = "");
    std::string processStyleOrigin(const std::string& origin_name, const std::string& target_element = "");
    std::string processJavaScriptOrigin(const std::string& origin_name, const std::string& target_element = "");
    
    // 内容处理
    std::string escapeContent(const std::string& content, OriginType type);
    std::string unescapeContent(const std::string& content, OriginType type);
    std::string validateContent(const std::string& content, OriginType type);
    
    // 配置管理
    void setConfig(const OriginSystemConfig& config);
    OriginSystemConfig getConfig() const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error_message);
    
    // 调试和诊断
    std::string getDebugInfo() const;
    void enableDebugMode(bool enable);
    
    // 状态机管理
    void setStateMachine(std::shared_ptr<CHTLStateMachine> state_machine);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
    bool isOriginDeclaration(const std::vector<Token>& tokens, size_t position);
    bool isOriginUsage(const std::vector<Token>& tokens, size_t position);
    OriginType detectOriginType(const std::vector<Token>& tokens, size_t position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    
    // 原始嵌入验证辅助方法
    bool isValidOriginName(const std::string& name);
    bool isValidOriginContent(const std::string& content);
    bool hasValidTargetElement(const std::string& target_element);
    
    // 内容处理辅助方法
    std::string sanitizeHtmlContent(const std::string& content);
    std::string sanitizeCssContent(const std::string& content);
    std::string sanitizeJsContent(const std::string& content);
    std::string injectTargetElement(const std::string& content, const std::string& target_element, OriginType type);
    
    // 状态机管理
    std::shared_ptr<CHTLStateMachine> state_machine;
};

} // namespace chtl