#pragma once

#include "ast/CHTLJSASTNode.h"
#include "lexer/CHTLToken.h"
#include "lexer/GlobalMap.h"
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

namespace chtl {

// CHTL JS解析器配置
struct CHTLJSParserConfig {
    bool enable_error_recovery;     // 是否启用错误恢复
    bool strict_mode;               // 是否启用严格模式
    bool preserve_comments;         // 是否保留注释
    bool enable_debug;              // 是否启用调试模式
    size_t max_error_count;         // 最大错误数量
    size_t max_nesting_depth;       // 最大嵌套深度
    bool enable_enhanced_selectors; // 是否启用增强选择器
    bool enable_chain_access;       // 是否启用链式访问
    bool enable_listen;             // 是否启用增强监听器
    bool enable_delegate;           // 是否启用事件委托
    bool enable_animate;            // 是否启用动画
    
    CHTLJSParserConfig() : 
        enable_error_recovery(true), strict_mode(true), preserve_comments(true),
        enable_debug(false), max_error_count(100), max_nesting_depth(100),
        enable_enhanced_selectors(true), enable_chain_access(true), enable_listen(true),
        enable_delegate(true), enable_animate(true) {}
};

// CHTL JS解析器状态
enum class CHTLJSParserState {
    INITIAL,                    // 初始状态
    IN_SCRIPT_BLOCK,            // 在脚本块中
    EXPECTING_ENHANCED_SELECTOR, // 期望增强选择器
    EXPECTING_CHAIN_ACCESS,     // 期望链式访问
    EXPECTING_LISTEN_DECL,      // 期望监听器声明
    EXPECTING_DELEGATE_DECL,    // 期望事件委托声明
    EXPECTING_ANIMATE_DECL,     // 期望动画声明
    EXPECTING_EVENT_HANDLER,    // 期望事件处理器
    EXPECTING_ANIMATION_PROPERTY, // 期望动画属性
    EXPECTING_ANIMATION_VALUE,  // 期望动画值
    EXPECTING_ANIMATION_KEYFRAME, // 期望动画关键帧
    EXPECTING_FUNCTION_DECL,    // 期望函数声明
    EXPECTING_FUNCTION_BODY,    // 期望函数体
    EXPECTING_PARAMETER,        // 期望参数
    EXPECTING_EXPRESSION,       // 期望表达式
    EXPECTING_STATEMENT,        // 期望语句
    EXPECTING_BRACE,            // 期望大括号
    EXPECTING_PARENTHESIS,      // 期望小括号
    EXPECTING_SEMICOLON,        // 期望分号
    ERROR                       // 错误状态
};

// CHTL JS解析器类
class CHTLJSParser {
public:
    CHTLJSParser(const CHTLJSParserConfig& config = CHTLJSParserConfig());
    ~CHTLJSParser();
    
    // 解析方法
    std::unique_ptr<ASTNode> parse(const std::vector<CHTLJSToken>& tokens);
    std::unique_ptr<ASTNode> parseFile(const std::string& file_path);
    std::unique_ptr<ASTNode> parseString(const std::string& source_code);
    
    // 解析器状态管理
    CHTLJSParserState getCurrentState() const;
    void setState(CHTLJSParserState new_state);
    bool isInState(CHTLJSParserState state) const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<ParserError> getErrors() const;
    void clearErrors();
    void addError(ParserErrorType type, const std::string& message, 
                  size_t line, size_t column, size_t position, bool recoverable = true);
    
    // 解析统计
    struct CHTLJSParseStats {
        size_t total_tokens;
        size_t total_nodes;
        size_t enhanced_selector_count;
        size_t chain_access_count;
        size_t listen_declaration_count;
        size_t delegate_declaration_count;
        size_t animate_declaration_count;
        size_t error_count;
        size_t recovery_count;
        size_t parse_time_ms;
    };
    
    CHTLJSParseStats getStats() const;
    
    // 配置管理
    void setConfig(const CHTLJSParserConfig& config);
    CHTLJSParserConfig getConfig() const;
    
    // 调试和诊断
    std::string getDebugInfo() const;
    void enableDebugMode(bool enable);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 核心解析方法
    std::unique_ptr<ASTNode> parseScriptBlock();
    std::unique_ptr<ASTNode> parseEnhancedSelector();
    std::unique_ptr<ASTNode> parseChainAccess();
    std::unique_ptr<ASTNode> parseListenDeclaration();
    std::unique_ptr<ASTNode> parseDelegateDeclaration();
    std::unique_ptr<ASTNode> parseAnimateDeclaration();
    std::unique_ptr<ASTNode> parseAnimationKeyframe();
    
    // JavaScript语法解析方法
    std::unique_ptr<ASTNode> parseFunctionDeclaration();
    std::unique_ptr<ASTNode> parseFunctionParameter();
    std::unique_ptr<ASTNode> parseFunctionBody();
    std::unique_ptr<ASTNode> parseVariableDeclaration();
    std::unique_ptr<ASTNode> parseExpressionStatement();
    std::unique_ptr<ASTNode> parseIfStatement();
    std::unique_ptr<ASTNode> parseForStatement();
    std::unique_ptr<ASTNode> parseWhileStatement();
    std::unique_ptr<ASTNode> parseReturnStatement();
    std::unique_ptr<ASTNode> parseFunctionCall();
    std::unique_ptr<ASTNode> parseArgumentList();
    std::unique_ptr<ASTNode> parseObjectLiteral();
    std::unique_ptr<ASTNode> parsePropertyAssignment();
    std::unique_ptr<ASTNode> parseArrayLiteral();
    std::unique_ptr<ASTNode> parseBinaryExpression();
    std::unique_ptr<ASTNode> parseUnaryExpression();
    std::unique_ptr<ASTNode> parseIdentifier();
    std::unique_ptr<ASTNode> parseLiteralValue();
    
    // 辅助解析方法
    std::unique_ptr<ASTNode> parseComment();
    std::unique_ptr<ASTNode> parseCSSSelector();
    std::unique_ptr<ASTNode> parseArrayAccess();
    std::unique_ptr<ASTNode> parseEventType();
    std::unique_ptr<ASTNode> parseAnimationProperty();
    std::unique_ptr<ASTNode> parseAnimationValue();
    std::unique_ptr<ASTNode> parseAnimationTime();
    
    // 错误恢复方法
    bool tryToRecover();
    void skipToNextSafePoint();
    void skipToNextBrace();
    void skipToNextParenthesis();
    void skipToNextSemicolon();
    
    // 辅助方法
    bool isEnhancedSelectorStart(const CHTLJSToken& token) const;
    bool isChainAccessStart(const CHTLJSToken& token) const;
    bool isListenStart(const CHTLJSToken& token) const;
    bool isDelegateStart(const CHTLJSToken& token) const;
    bool isAnimateStart(const CHTLJSToken& token) const;
    bool isFunctionStart(const CHTLJSToken& token) const;
    bool isVariableStart(const CHTLJSToken& token) const;
    bool isControlFlowStart(const CHTLJSToken& token) const;
    bool isExpressionStart(const CHTLJSToken& token) const;
    
    // 上下文管理
    void pushContext(const std::string& context_name);
    void popContext();
    std::string getCurrentContext() const;
    bool isInContext(const std::string& context_name) const;
    
    // 符号表管理
    void addSymbol(const SymbolInfo& symbol);
    SymbolInfo* findSymbol(const std::string& name, const std::string& namespace_path = "");
    
    // 验证方法
    bool validateEnhancedSelector(const std::unique_ptr<ASTNode>& selector) const;
    bool validateChainAccess(const std::unique_ptr<ASTNode>& chain_access) const;
    bool validateListenDeclaration(const std::unique_ptr<ASTNode>& listen_decl) const;
    bool validateDelegateDeclaration(const std::unique_ptr<ASTNode>& delegate_decl) const;
    bool validateAnimateDeclaration(const std::unique_ptr<ASTNode>& animate_decl) const;
    bool validateAnimationKeyframe(const std::unique_ptr<ASTNode>& keyframe) const;
    
    // 特殊语法处理
    bool isValidCSSSelector(const std::string& selector) const;
    bool isValidAnimationProperty(const std::string& property) const;
    bool isValidAnimationValue(const std::string& value) const;
    bool isValidEventType(const std::string& event_type) const;
    bool isValidMethodName(const std::string& method_name) const;
};

} // namespace chtl