#pragma once

#include "lexer/CHTLToken.h"
#include "lexer/GlobalMap.h"
#include "lexer/LexerState.h"
#include "lexer/LexerContext.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace chtl {

// 词法分析器配置
struct LexerConfig {
    bool enable_debug;           // 是否启用调试模式
    bool strict_mode;            // 是否启用严格模式
    bool ignore_whitespace;      // 是否忽略空白字符
    bool preserve_comments;      // 是否保留注释
    size_t max_token_length;     // 最大Token长度
    size_t max_line_length;      // 最大行长度
    
    LexerConfig() : 
        enable_debug(false), strict_mode(true), ignore_whitespace(false),
        preserve_comments(true), max_token_length(1024), max_line_length(10000) {}
};

// CHTL词法分析器类
class CHTLLexer {
public:
    CHTLLexer(const LexerConfig& config = LexerConfig());
    ~CHTLLexer();
    
    // 词法分析
    std::vector<Token> tokenize(const std::string& source_code);
    std::vector<Token> tokenizeFile(const std::string& file_path);
    
    // Token流管理
    Token getNextToken();
    Token peekToken(size_t offset = 1);
    bool hasMoreTokens() const;
    void resetTokenStream();
    
    // 状态管理
    LexerState getCurrentState() const;
    void setState(LexerState new_state);
    bool isInState(LexerState state) const;
    
    // 上下文管理
    LexerContext* getContext() const;
    bool isInContext(ContextType type) const;
    std::string getCurrentContextPath() const;
    
    // 全局符号表
    GlobalMap* getGlobalMap() const;
    bool addSymbol(const SymbolInfo& symbol);
    SymbolInfo* findSymbol(const std::string& name, const std::string& namespace_path = "");
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error, size_t line = 0, size_t column = 0);
    
    // 警告处理
    bool hasWarnings() const;
    std::vector<std::string> getWarnings() const;
    void clearWarnings();
    void addWarning(const std::string& warning, size_t line = 0, size_t column = 0);
    
    // 统计信息
    struct LexerStats {
        size_t total_tokens;
        size_t total_lines;
        size_t total_characters;
        size_t error_count;
        size_t warning_count;
        size_t comment_count;
        size_t string_count;
        size_t template_count;
        size_t custom_count;
        size_t namespace_count;
        size_t import_count;
    };
    
    LexerStats getStats() const;
    
    // 配置管理
    void setConfig(const LexerConfig& config);
    LexerConfig getConfig() const;
    
    // 调试和诊断
    std::string getDebugInfo() const;
    std::string getTokenStreamInfo() const;
    void enableDebugMode(bool enable);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部词法分析方法
    Token processNextToken();
    Token processComment();
    Token processString();
    Token processUnquotedLiteral();
    Token processTemplateDeclaration();
    Token processCustomDeclaration();
    Token processOriginDeclaration();
    Token processConfigurationDeclaration();
    Token processImportDeclaration();
    Token processNamespaceDeclaration();
    Token processAttribute();
    Token processStyleBlock();
    Token processScriptBlock();
    Token processEnhancedSelector();
    Token processAnimationBlock();
    
    // 辅助方法
    bool isWhitespace(char ch) const;
    bool isNewline(char ch) const;
    bool isDigit(char ch) const;
    bool isLetter(char ch) const;
    bool isIdentifierStart(char ch) const;
    bool isIdentifierPart(char ch) const;
    bool isOperator(char ch) const;
    bool isDelimiter(char ch) const;
    
    // 关键字识别
    bool isKeyword(const std::string& word) const;
    bool isHTMLTag(const std::string& word) const;
    bool isCSSProperty(const std::string& word) const;
    bool isCSSValue(const std::string& word) const;
    bool isJavaScriptKeyword(const std::string& word) const;
    
    // 状态转换
    bool transitionState(const std::string& trigger);
    void updateContext(ContextType type, const std::string& name = "");
    
    // 错误恢复
    void recoverFromError();
    bool tryToRecover();
    void skipToNextSafePoint();
};

// CHTL JS词法分析器类
class CHTLJSLexer {
public:
    CHTLJSLexer(const LexerConfig& config = LexerConfig());
    ~CHTLJSLexer();
    
    // 词法分析
    std::vector<CHTLJSToken> tokenize(const std::string& source_code);
    std::vector<CHTLJSToken> tokenizeFile(const std::string& file_path);
    
    // Token流管理
    CHTLJSToken getNextToken();
    CHTLJSToken peekToken(size_t offset = 1);
    bool hasMoreTokens() const;
    void resetTokenStream();
    
    // 状态管理
    LexerState getCurrentState() const;
    void setState(LexerState new_state);
    bool isInState(LexerState state) const;
    
    // 上下文管理
    LexerContext* getContext() const;
    bool isInContext(ContextType type) const;
    std::string getCurrentContextPath() const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error, size_t line = 0, size_t column = 0);
    
    // 统计信息
    struct CHTLJSLexerStats {
        size_t total_tokens;
        size_t total_lines;
        size_t total_characters;
        size_t error_count;
        size_t enhanced_selector_count;
        size_t chain_access_count;
        size_t listen_count;
        size_t delegate_count;
        size_t animate_count;
    };
    
    CHTLJSLexerStats getStats() const;
    
    // 配置管理
    void setConfig(const LexerConfig& config);
    LexerConfig getConfig() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部词法分析方法
    CHTLJSToken processNextToken();
    CHTLJSToken processEnhancedSelector();
    CHTLJSToken processChainAccess();
    CHTLJSToken processListenDeclaration();
    CHTLJSToken processDelegateDeclaration();
    CHTLJSToken processAnimateDeclaration();
    CHTLJSToken processJavaScriptToken();
    
    // 辅助方法
    bool isWhitespace(char ch) const;
    bool isNewline(char ch) const;
    bool isDigit(char ch) const;
    bool isLetter(char ch) const;
    bool isIdentifierStart(char ch) const;
    bool isIdentifierPart(char ch) const;
    bool isOperator(char ch) const;
    bool isDelimiter(char ch) const;
    
    // 关键字识别
    bool isCHTLJSKeyword(const std::string& word) const;
    bool isJavaScriptKeyword(const std::string& word) const;
    
    // 状态转换
    bool transitionState(const std::string& trigger);
    void updateContext(ContextType type, const std::string& name = "");
    
    // 错误恢复
    void recoverFromError();
    bool tryToRecover();
    void skipToNextSafePoint();
};

} // namespace chtl