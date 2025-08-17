#pragma once

#include "ast/ASTNode.h"
#include "ast/CHTLASTNodes.h"
#include "lexer/CHTLToken.h"
#include "lexer/GlobalMap.h"
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

namespace chtl {

// 解析器配置
struct ParserConfig {
    bool enable_error_recovery;     // 是否启用错误恢复
    bool strict_mode;               // 是否启用严格模式
    bool preserve_comments;         // 是否保留注释
    bool enable_debug;              // 是否启用调试模式
    size_t max_error_count;         // 最大错误数量
    size_t max_nesting_depth;       // 最大嵌套深度
    
    ParserConfig() : 
        enable_error_recovery(true), strict_mode(true), preserve_comments(true),
        enable_debug(false), max_error_count(100), max_nesting_depth(100) {}
};

// 解析器状态
enum class ParserState {
    INITIAL,                    // 初始状态
    EXPECTING_ELEMENT,          // 期望元素
    EXPECTING_ATTRIBUTE,        // 期望属性
    EXPECTING_ATTRIBUTE_VALUE,  // 期望属性值
    EXPECTING_STYLE_BLOCK,      // 期望样式块
    EXPECTING_TEMPLATE_DECL,    // 期望模板声明
    EXPECTING_CUSTOM_DECL,      // 期望自定义声明
    EXPECTING_ORIGIN_DECL,      // 期望原始嵌入声明
    EXPECTING_IMPORT_DECL,      // 期望导入声明
    EXPECTING_NAMESPACE_DECL,   // 期望命名空间声明
    EXPECTING_CONSTRAINT_DECL,  // 期望约束声明
    EXPECTING_SCRIPT_BLOCK,     // 期望脚本块
    EXPECTING_BRACE,            // 期望大括号
    EXPECTING_SEMICOLON,        // 期望分号
    EXPECTING_COLON_OR_EQUAL,   // 期望冒号或等号
    ERROR                       // 错误状态
};

// 解析器错误类型
enum class ParserErrorType {
    LEXICAL_ERROR,              // 词法错误
    SYNTAX_ERROR,               // 语法错误
    SEMANTIC_ERROR,             // 语义错误
    CONTEXT_ERROR,              // 上下文错误
    RECOVERY_ERROR              // 恢复错误
};

// 解析器错误信息
struct ParserError {
    ParserErrorType type;
    std::string message;
    size_t line;
    size_t column;
    size_t position;
    std::string source_file;
    bool is_recoverable;
    
    ParserError(ParserErrorType t, const std::string& msg, size_t l, size_t c, size_t p,
                const std::string& src = "", bool recoverable = true)
        : type(t), message(msg), line(l), column(c), position(p), 
          source_file(src), is_recoverable(recoverable) {}
};

// CHTL解析器类
class CHTLParser {
public:
    CHTLParser(const ParserConfig& config = ParserConfig());
    ~CHTLParser();
    
    // 解析方法
    std::unique_ptr<ASTNode> parse(const std::vector<Token>& tokens);
    std::unique_ptr<ASTNode> parseFile(const std::string& file_path);
    std::unique_ptr<ASTNode> parseString(const std::string& source_code);
    
    // 解析器状态管理
    ParserState getCurrentState() const;
    void setState(ParserState new_state);
    bool isInState(ParserState state) const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<ParserError> getErrors() const;
    void clearErrors();
    void addError(ParserErrorType type, const std::string& message, 
                  size_t line, size_t column, size_t position, bool recoverable = true);
    
    // 解析统计
    struct ParseStats {
        size_t total_tokens;
        size_t total_nodes;
        size_t error_count;
        size_t recovery_count;
        size_t parse_time_ms;
    };
    
    ParseStats getStats() const;
    
    // 配置管理
    void setConfig(const ParserConfig& config);
    ParserConfig getConfig() const;
    
    // 调试和诊断
    std::string getDebugInfo() const;
    void enableDebugMode(bool enable);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 核心解析方法
    std::unique_ptr<ASTNode> parseRoot();
    std::unique_ptr<ASTNode> parseElement();
    std::unique_ptr<ASTNode> parseAttribute();
    std::unique_ptr<ASTNode> parseStyleBlock();
    std::unique_ptr<ASTNode> parseTemplateDeclaration();
    std::unique_ptr<ASTNode> parseCustomDeclaration();
    std::unique_ptr<ASTNode> parseOriginDeclaration();
    std::unique_ptr<ASTNode> parseImportDeclaration();
    std::unique_ptr<ASTNode> parseNamespaceDeclaration();
    std::unique_ptr<ASTNode> parseConstraintDeclaration();
    std::unique_ptr<ASTNode> parseScriptBlock();
    
    // 辅助解析方法
    std::unique_ptr<ASTNode> parseComment();
    std::unique_ptr<ASTNode> parseTextNode();
    std::unique_ptr<ASTNode> parseLiteral();
    std::unique_ptr<ASTNode> parseCSSRule();
    std::unique_ptr<ASTNode> parseCSSSelector();
    std::unique_ptr<ASTNode> parseCSSProperty();
    std::unique_ptr<ASTNode> parseTemplateUsage();
    std::unique_ptr<ASTNode> parseCustomSpecialization();
    
    // 错误恢复方法
    bool tryToRecover();
    void skipToNextSafePoint();
    void skipToNextBrace();
    void skipToNextSemicolon();
    
    // 辅助方法
    bool isElementStart(const Token& token) const;
    bool isAttributeStart(const Token& token) const;
    bool isStyleBlockStart(const Token& token) const;
    bool isTemplateStart(const Token& token) const;
    bool isCustomStart(const Token& token) const;
    bool isOriginStart(const Token& token) const;
    bool isImportStart(const Token& token) const;
    bool isNamespaceStart(const Token& token) const;
    bool isConstraintStart(const Token& token) const;
    bool isScriptStart(const Token& token) const;
    
    // 上下文管理
    void pushContext(const std::string& context_name);
    void popContext();
    std::string getCurrentContext() const;
    bool isInContext(const std::string& context_name) const;
    
    // 符号表管理
    void addSymbol(const SymbolInfo& symbol);
    SymbolInfo* findSymbol(const std::string& name, const std::string& namespace_path = "");
    
    // 验证方法
    bool validateElement(const std::unique_ptr<ASTNode>& element) const;
    bool validateAttribute(const std::unique_ptr<ASTNode>& attribute) const;
    bool validateStyleBlock(const std::unique_ptr<ASTNode>& style_block) const;
    bool validateTemplateDeclaration(const std::unique_ptr<ASTNode>& template_decl) const;
    bool validateCustomDeclaration(const std::unique_ptr<ASTNode>& custom_decl) const;
};

} // namespace chtl