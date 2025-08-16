#ifndef CHTL_COMPILER_CHTL_LEXER_H
#define CHTL_COMPILER_CHTL_LEXER_H

#include <string>
#include <vector>
#include "common/types.h"

namespace chtl {
namespace compiler {

// CHTL词法分析器
class CHTLLexer {
public:
    CHTLLexer(const std::string& source, const std::string& filename = "<input>");
    ~CHTLLexer();
    
    // 获取下一个token
    Token nextToken();
    
    // 是否还有更多token
    bool hasMoreTokens() const;
    
    // 重置词法分析器
    void reset();
    
private:
    std::string source_;
    std::string filename_;
    size_t position_;
    size_t line_;
    size_t column_;
    
    // 用于记录token起始位置
    size_t tokenStartPos_;
    size_t tokenStartLine_;
    size_t tokenStartColumn_;
    
    // 上一个token类型（用于上下文相关的词法分析）
    TokenType lastTokenType_;
    
    // 跳过空白字符和注释
    void skipWhitespaceAndComments();
    
    // 扫描各种token类型
    Token scanString(char delimiter);
    Token scanNumber();
    Token scanIdentifier();
    Token scanUnquotedLiteral();
    Token scanSemanticComment();
    
    // 判断是否可以开始无修饰字面量
    bool canStartUnquotedLiteral(char ch) const;
    
    // 辅助方法
    char advance();
    char peek() const;
    char peekNext() const;
    bool isAtEnd() const;
    void markTokenStart();
    
    // 创建token
    Token makeToken(TokenType type, const std::string& value = "");
    Token makeErrorToken(const std::string& message);
};

} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_CHTL_LEXER_H