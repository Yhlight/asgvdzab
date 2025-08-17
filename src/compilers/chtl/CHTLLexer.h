#pragma once

#include <string>
#include <vector>
#include <unordered_set>

namespace CHTL {

/**
 * CHTL词法单元类型
 */
enum class TokenType {
    // 基本符号
    LEFT_BRACE,         // {
    RIGHT_BRACE,        // }
    LEFT_BRACKET,       // [
    RIGHT_BRACKET,      // ]
    LEFT_PAREN,         // (
    RIGHT_PAREN,        // )
    SEMICOLON,          // ;
    COLON,              // :
    EQUALS,             // =
    COMMA,              // ,
    DOT,                // .
    HASH,               // #
    AMPERSAND,          // &
    AT,                 // @
    
    // 字面量
    IDENTIFIER,         // 标识符
    STRING,             // 字符串
    NUMBER,             // 数字
    UNQUOTED_STRING,    // 无引号字符串
    
    // 关键字
    TEXT,               // text
    STYLE,              // style
    SCRIPT,             // script
    TEMPLATE,           // [Template]
    CUSTOM,             // [Custom]
    ORIGIN,             // [Origin]
    IMPORT,             // [Import]
    NAMESPACE,          // [Namespace]
    CONFIGURATION,      // [Configuration]
    INHERIT,            // inherit
    DELETE,             // delete
    INSERT,             // insert
    AFTER,              // after
    BEFORE,             // before
    REPLACE,            // replace
    AT_TOP,             // at top
    AT_BOTTOM,          // at bottom
    EXCEPT,             // except
    FROM,               // from
    AS,                 // as
    
    // 特殊
    COMMENT,            // 注释
    NEWLINE,            // 换行
    WHITESPACE,         // 空白
    END_OF_FILE,        // 文件结束
    UNKNOWN             // 未知
};

/**
 * CHTL词法单元
 */
struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
    size_t position;
    
    Token() : type(TokenType::UNKNOWN), line(0), column(0), position(0) {}
    
    Token(TokenType t, const std::string& v, size_t l, size_t c, size_t p)
        : type(t), value(v), line(l), column(c), position(p) {}
    
    std::string toString() const;
};

/**
 * CHTL词法分析器
 */
class CHTLLexer {
public:
    CHTLLexer();
    ~CHTLLexer() = default;

    /**
     * 设置源代码
     * @param source 源代码字符串
     */
    void setSource(const std::string& source);

    /**
     * 获取下一个词法单元
     * @return 词法单元
     */
    Token nextToken();

    /**
     * 预览下一个词法单元（不移动位置）
     * @return 词法单元
     */
    Token peekToken();

    /**
     * 获取当前位置
     * @return 当前位置
     */
    size_t getCurrentPosition() const { return position_; }

    /**
     * 获取当前行号
     * @return 当前行号
     */
    size_t getCurrentLine() const { return line_; }

    /**
     * 获取当前列号
     * @return 当前列号
     */
    size_t getCurrentColumn() const { return column_; }

    /**
     * 是否到达文件结尾
     * @return 是否到达结尾
     */
    bool isEOF() const { return position_ >= source_.length(); }

    /**
     * 重置词法分析器
     */
    void reset();

    /**
     * 获取错误信息
     * @return 错误信息列表
     */
    const std::vector<std::string>& getErrors() const { return errors_; }

private:
    std::string source_;
    size_t position_;
    size_t line_;
    size_t column_;
    std::vector<std::string> errors_;
    
    // 关键字集合
    std::unordered_set<std::string> keywords_;
    
    // 初始化关键字
    void initializeKeywords();
    
    // 获取当前字符
    char currentChar() const;
    
    // 获取下一个字符
    char peekChar(size_t offset = 1) const;
    
    // 前进一个字符
    void advance();
    
    // 跳过空白字符
    void skipWhitespace();
    
    // 跳过注释
    void skipComment();
    
    // 扫描标识符
    Token scanIdentifier();
    
    // 扫描字符串
    Token scanString(char quote);
    
    // 扫描无引号字符串
    Token scanUnquotedString();
    
    // 扫描数字
    Token scanNumber();
    
    // 扫描特殊块标记 [xxx]
    Token scanBlockMarker();
    
    // 判断是否为关键字
    TokenType getKeywordType(const std::string& identifier) const;
    
    // 判断字符类型
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isWhitespace(char c) const;
    
    // 创建词法单元
    Token makeToken(TokenType type, const std::string& value = "");
    
    // 记录错误
    void recordError(const std::string& message);
};

} // namespace CHTL