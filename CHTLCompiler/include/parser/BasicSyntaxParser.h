#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ast/ASTNode.h"
#include "lexer/CHTLLexer.h"

namespace chtl {

// 基础语法节点类型（严格按照CHTL语法文档）
enum class BasicSyntaxNodeType {
    COMMENT_SINGLE_LINE,      // // 单行注释
    COMMENT_MULTI_LINE,       // /* */ 多行注释
    COMMENT_GENERATOR,        // -- 生成器注释
    TEXT_NODE,                // text { } 文本节点
    LITERAL_UNQUOTED,         // 无修饰字面量
    LITERAL_DOUBLE_QUOTE,     // 双引号字符串 ""
    LITERAL_SINGLE_QUOTE,     // 单引号字符串 ''
    HTML_ELEMENT,             // HTML元素节点
    ATTRIBUTE_DECLARATION,    // 属性声明 属性名 : "属性值";
    CE_EQUIVALENCE            // : 与 = 等价
};

// 基础语法解析器配置
struct BasicSyntaxParserConfig {
    bool enable_ce_equivalence;    // 是否启用CE等价性
    bool enable_unquoted_literals; // 是否启用无修饰字面量
    bool enable_generator_comments; // 是否启用生成器注释
    bool strict_mode;              // 严格模式
    size_t max_text_length;        // 最大文本长度
    size_t max_comment_length;     // 最大注释长度
    
    BasicSyntaxParserConfig() : 
        enable_ce_equivalence(true), enable_unquoted_literals(true), 
        enable_generator_comments(true), strict_mode(false), 
        max_text_length(10000), max_comment_length(1000) {}
};

// 基础语法解析器类
class BasicSyntaxParser {
public:
    BasicSyntaxParser(const BasicSyntaxParserConfig& config = BasicSyntaxParserConfig());
    ~BasicSyntaxParser();
    
    // 基础语法解析
    std::shared_ptr<ASTNode> parseComment(const std::vector<Token>& tokens, size_t& position);
    std::shared_ptr<ASTNode> parseTextNode(const std::vector<Token>& tokens, size_t& position);
    std::shared_ptr<ASTNode> parseLiteral(const std::vector<Token>& tokens, size_t& position);
    std::shared_ptr<ASTNode> parseHTMLElement(const std::vector<Token>& tokens, size_t& position);
    std::shared_ptr<ASTNode> parseAttribute(const std::vector<Token>& tokens, size_t& position);
    
    // 组合解析
    std::vector<std::shared_ptr<ASTNode>> parseBasicSyntax(const std::vector<Token>& tokens);
    std::shared_ptr<ASTNode> parseDocument(const std::vector<Token>& tokens);
    
    // 验证和错误处理
    bool validateBasicSyntax(const std::vector<Token>& tokens);
    std::vector<std::string> getValidationErrors() const;
    bool hasValidationErrors() const;
    
    // 配置管理
    void setConfig(const BasicSyntaxParserConfig& config);
    BasicSyntaxParserConfig getConfig() const;
    
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
    bool isCommentToken(const Token& token);
    bool isTextNodeStart(const std::vector<Token>& tokens, size_t position);
    bool isHTMLElementStart(const std::vector<Token>& tokens, size_t position);
    bool isAttributeDeclaration(const std::vector<Token>& tokens, size_t position);
    std::string extractCommentContent(const std::vector<Token>& tokens, size_t& position);
    std::string extractTextContent(const std::vector<Token>& tokens, size_t& position);
    std::string extractLiteralContent(const std::vector<Token>& tokens, size_t& position);
    std::string extractElementName(const std::vector<Token>& tokens, size_t& position);
    std::pair<std::string, std::string> extractAttribute(const std::vector<Token>& tokens, size_t& position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    void addValidationError(const std::string& error_message);
};

} // namespace chtl