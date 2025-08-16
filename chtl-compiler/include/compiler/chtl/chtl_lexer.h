#ifndef CHTL_COMPILER_CHTL_LEXER_H
#define CHTL_COMPILER_CHTL_LEXER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "common/types.h"

namespace chtl {
namespace compiler {

class CHTLLexer {
public:
    CHTLLexer();
    ~CHTLLexer();
    
    // 初始化词法分析器
    void init(const std::string& source, const std::string& filename = "<input>");
    
    // 获取下一个token
    Token nextToken();
    
    // 预览下一个token（不消耗）
    Token peekToken();
    
    // 消耗并返回下一个token
    Token consumeToken();
    
    // 检查是否到达文件末尾
    bool isEOF() const;
    
    // 获取当前位置
    SourceLocation getCurrentLocation() const;
    
    // 获取错误信息
    const std::vector<std::string>& getErrors() const { return errors_; }

private:
    std::string source_;
    std::string filename_;
    size_t position_;
    size_t line_;
    size_t column_;
    std::vector<std::string> errors_;
    Token currentToken_;
    bool hasCurrentToken_;
    
    // 关键字映射
    static const std::unordered_map<std::string, TokenType> keywords_;
    
    // 词法分析辅助方法
    void skipWhitespace();
    void skipComment();
    Token scanIdentifier();
    Token scanString();
    Token scanNumber();
    Token scanSymbol();
    
    // 字符判断方法
    bool isIdentifierStart(char ch) const;
    bool isIdentifierPart(char ch) const;
    bool isDigit(char ch) const;
    bool isWhitespace(char ch) const;
    
    // 获取下一个字符
    char peek(size_t offset = 0) const;
    char advance();
    
    // 错误处理
    void addError(const std::string& message);
    
    // 创建token
    Token makeToken(TokenType type, const std::string& value = "");
    Token makeEOFToken();
};

} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_CHTL_LEXER_H