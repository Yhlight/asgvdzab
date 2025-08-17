#ifndef CHTLJS_LEXER_H
#define CHTLJS_LEXER_H

#include <string>
#include <vector>
#include <memory>
#include "compiler/chtljs/chtljs_tokens.h"

namespace chtl {

// CHTL JS词法分析器
class CHTLJSLexer {
public:
    CHTLJSLexer(const std::string& input, const std::string& filename = "");
    ~CHTLJSLexer() = default;
    
    // 获取下一个Token
    CHTLJSToken nextToken();
    
    // 查看下一个Token但不消费
    CHTLJSToken peekToken();
    
    // 获取当前位置
    size_t getCurrentLine() const { return line_; }
    size_t getCurrentColumn() const { return column_; }
    size_t getCurrentPosition() const { return position_; }
    
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
    CHTLJSToken peeked_token_;
    
    // 错误列表
    std::vector<std::string> errors_;
    
    // 辅助方法
    void skipWhitespace();
    void skipComment();
    CHTLJSToken scanEnhancedSelector();
    CHTLJSToken scanArrowOperator();
    CHTLJSToken scanIdentifierOrKeyword();
    CHTLJSToken scanNumber();
    CHTLJSToken scanString(char quote);
    CHTLJSToken scanOperator();
    
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
    CHTLJSToken makeToken(CHTLJSTokenType type, const std::string& value, 
                         size_t start_pos, size_t end_pos);
    
    // 错误报告
    void reportError(const std::string& message);
    
    // 检查是否是CHTL JS特殊语法
    bool isEnhancedSelector();
    bool isSpecialFunction(const std::string& identifier);
};

} // namespace chtl

#endif // CHTLJS_LEXER_H