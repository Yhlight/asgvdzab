#ifndef CHTL_LEXER_LEXER_H
#define CHTL_LEXER_LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Lexer/Token.h"

namespace Chtl {

class Lexer {
public:
    Lexer();
    ~Lexer();
    
    // 设置源代码
    void setSource(const std::string& source, const std::string& fileName = "");
    
    // 获取下一个Token
    Token nextToken();
    
    // 预览下一个Token（不消耗）
    Token peekToken();
    
    // 获取所有Token
    std::vector<Token> tokenize();
    
    // 获取当前位置
    size_t getCurrentLine() const { return currentLine_; }
    size_t getCurrentColumn() const { return currentColumn_; }
    size_t getCurrentPosition() const { return currentPos_; }
    
    // 错误处理
    bool hasError() const { return hasError_; }
    std::string getLastError() const { return lastError_; }
    
    // 重置词法分析器
    void reset();

private:
    // 内部方法
    void skipWhitespace();
    void skipComment();
    
    Token readIdentifier();
    Token readNumber();
    Token readStringDoubleQuote();
    Token readStringSingleQuote();
    Token readUnquotedLiteral();
    Token readMarkKeyword();      // 读取[Template]等标记
    Token readAtKeyword();        // 读取@Style等
    Token readOperator();
    
    // 判断字符类型
    bool isAlpha(char ch) const;
    bool isDigit(char ch) const;
    bool isAlphaNum(char ch) const;
    bool isWhitespace(char ch) const;
    bool isOperatorChar(char ch) const;
    bool isUnquotedLiteralChar(char ch) const;
    
    // 关键字识别
    TokenType identifyKeyword(const std::string& text) const;
    TokenType identifyMarkKeyword(const std::string& text) const;
    TokenType identifyAtKeyword(const std::string& text) const;
    
    // 辅助方法
    char peek(int offset = 0) const;
    char advance();
    void consume();
    bool match(const std::string& text) const;
    bool isAtEnd() const;
    
    // 创建Token
    Token makeToken(TokenType type, const std::string& value);
    Token makeError(const std::string& message);
    
    // 更新位置信息
    void updatePosition(char ch);

private:
    // 源代码
    std::string source_;
    std::string fileName_;
    
    // 当前位置
    size_t currentPos_;
    size_t currentLine_;
    size_t currentColumn_;
    
    // Token起始位置
    size_t tokenStartPos_;
    size_t tokenStartLine_;
    size_t tokenStartColumn_;
    
    // 缓存的Token（用于peek）
    Token cachedToken_;
    bool hasCachedToken_;
    
    // 错误状态
    bool hasError_;
    std::string lastError_;
    
    // 关键字映射表
    static const std::unordered_map<std::string, TokenType> keywords_;
    static const std::unordered_map<std::string, TokenType> markKeywords_;
    static const std::unordered_map<std::string, TokenType> atKeywords_;
};

} // namespace Chtl

#endif // CHTL_LEXER_LEXER_H