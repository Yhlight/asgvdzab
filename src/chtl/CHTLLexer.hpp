#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "CHTLToken.hpp"
#include "CHTLContext.hpp"

namespace chtl {

/**
 * CHTL词法分析器
 * 负责将源代码转换为Token流
 */
class CHTLLexer {
public:
    explicit CHTLLexer(CHTLContext& context);
    ~CHTLLexer();
    
    /**
     * 词法分析
     * @param source 源代码
     * @return Token流
     */
    std::vector<CHTLToken> tokenize(const std::string& source);
    
private:
    CHTLContext& context_;
    std::string source_;
    size_t currentPos_;
    size_t currentLine_;
    size_t currentColumn_;
    std::vector<CHTLToken> tokens_;
    
    // 关键字映射
    static const std::unordered_map<std::string, CHTLTokenType> keywords_;
    static const std::unordered_map<std::string, CHTLTokenType> atDirectives_;
    
    // 基础字符操作
    char peek(size_t offset = 0) const;
    char consume();
    void skipWhitespace();
    
    // Token识别
    void scanToken();
    void scanIdentifierOrKeyword();
    void scanString(char quote);
    void scanNumber();
    void scanComment();
    void scanBracketBlock();  // [Template], [Custom]等
    void scanAtDirective();   // @Style, @Element等
    void scanUnquotedLiteral();
    
    // 辅助函数
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isValidIdentifierStart(char c) const;
    bool isValidIdentifierChar(char c) const;
    bool isAtEnd() const;
    
    // 判断是否需要扫描无引号字面量
    bool shouldScanUnquotedLiteral() const;
    
    // 添加Token
    void addToken(CHTLTokenType type);
    void addToken(CHTLTokenType type, const std::string& value);
    
    // 错误处理
    void reportError(const std::string& message);
    
    // 初始化关键字映射
    static std::unordered_map<std::string, CHTLTokenType> initializeKeywords();
    static std::unordered_map<std::string, CHTLTokenType> initializeAtDirectives();
};

} // namespace chtl