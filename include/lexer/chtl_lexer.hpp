#pragma once

#include "common/types.hpp"
#include <string>
#include <vector>
#include <unordered_set>

namespace chtl {

/**
 * Token类型枚举
 */
enum class TokenType {
    // 标识符和字面量
    IDENTIFIER,                 // 标识符 (变量名、函数名、HTML标签等)
    STRING_LITERAL,             // 字符串字面量 "text", 'text'
    NUMBER,                     // 数字字面量
    
    // 关键字
    KEYWORD,                    // 保留关键字
    
    // 操作符
    COLON,                      // :
    SEMICOLON,                  // ;
    ASSIGNMENT,                 // =
    DOT,                        // .
    HASH,                       // #
    AMPERSAND,                  // &
    OPERATOR,                   // 其他操作符
    
    // 括号
    LEFT_PAREN,                 // (
    RIGHT_PAREN,                // )
    LEFT_BRACE,                 // {
    RIGHT_BRACE,                // }
    LEFT_BRACKET,               // [
    RIGHT_BRACKET,              // ]
    
    // 空白和控制
    WHITESPACE,                 // 空白字符
    NEWLINE,                    // 换行符
    
    // 注释
    COMMENT_LINE,               // 单行注释 //
    COMMENT_MULTILINE,          // 多行注释 /* */
    
    // 特殊
    EOF_TOKEN,                  // 文件结束
    UNKNOWN                     // 未知token
};

/**
 * Token结构
 */
struct Token {
    TokenType type;
    std::string value;
    Position position;
    
    Token(TokenType t = TokenType::UNKNOWN, const std::string& val = "", Position pos = Position{})
        : type(t), value(val), position(pos) {}
};

/**
 * 词法分析结果
 */
struct LexResult {
    bool success = false;
    std::vector<Token> tokens;
    std::vector<ParseError> errors;
};

/**
 * 简单的CHTL词法分析器
 */
class CHTLLexer {
public:
    CHTLLexer();
    ~CHTLLexer();

    /**
     * 对源代码进行词法分析
     * @param source 源代码
     * @param filename 文件名
     * @return 词法分析结果
     */
    LexResult tokenize(const std::string& source, const std::string& filename = "");

private:
    std::string source_;
    std::string filename_;
    size_t currentPos_;
    int line_;
    int column_;
    std::vector<Token> tokens_;
    std::vector<ParseError> errors_;
    
    std::unordered_set<std::string> keywords_;

    // 词法分析内部方法
    void initializeKeywords();
    char currentChar() const;
    char peekChar(size_t offset = 1) const;
    void advance();
    bool isEOF() const;
    Position getCurrentPosition() const;
    
    // Token识别方法
    void skipWhitespace();
    void skipLineComment();
    void skipMultilineComment();
    Token readString(char quote);
    Token readNumber();
    Token readIdentifier();
    Token readOperator();
    
    // 字符分类方法
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isWhitespace(char c) const;
    
    // 错误处理
    void reportError(const std::string& message);
};

} // namespace chtl