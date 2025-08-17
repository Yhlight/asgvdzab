#ifndef CHTL_LEXER_H
#define CHTL_LEXER_H

#include <string>
#include <vector>
#include <memory>
#include "compiler/chtl/chtl_tokens.h"

namespace chtl {

// CHTL词法分析器
class CHTLLexer {
public:
    CHTLLexer(const std::string& input, const std::string& filename = "");
    ~CHTLLexer() = default;
    
    // 获取下一个Token
    Token nextToken();
    
    // 查看下一个Token但不消费
    Token peekToken();
    
    // 获取当前位置
    size_t getCurrentLine() const { return line_; }
    size_t getCurrentColumn() const { return column_; }
    size_t getCurrentPosition() const { return position_; }
    size_t getPosition() const { return position_; }
    
    // 原始文本处理（用于[Origin]块）
    char peekChar(size_t offset = 0) const;
    void skipChar();
    bool isAtEnd() const { return position_ >= input_.size(); }
    
    // 错误处理
    bool hasErrors() const { return !errors_.empty(); }
    const std::vector<std::string>& getErrors() const { return errors_; }
    
private:
    // 输入和位置
    std::string input_;
    std::string filename_;
    size_t position_;
    size_t line_;
    size_t column_;
    
    // 缓存的下一个token
    bool has_peeked_;
    Token peeked_token_;
    
    // 错误列表
    std::vector<std::string> errors_;
    
    // 辅助方法
    void skipWhitespace();
    void skipComment();
    Token scanIdentifierOrKeyword();
    Token scanNumber();
    Token scanString(char quote);
    Token scanUnquotedLiteral();
    Token scanSpecialBlock();
    Token scanOperator();
    
    // 字符判断
    bool isWhitespace(char c) const;
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isIdentifierStart(char c) const;
    bool isIdentifierChar(char c) const;
    
    // 查看字符
    char peek(size_t offset = 0) const;
    char advance();
    bool match(char expected);
    bool matchSequence(const std::string& seq);
    
    // 创建Token
    Token makeToken(TokenType type, const std::string& value, 
                    size_t start_pos, size_t end_pos);
    
    // 错误报告
    void reportError(const std::string& message);
};

} // namespace chtl

#endif // CHTL_LEXER_H