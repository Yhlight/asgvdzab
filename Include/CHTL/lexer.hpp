#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "ast_nodes.hpp"

namespace chtl {
namespace ast {

/**
 * Token类型枚举
 */
enum class TokenType {
    // 文件结束
    END_OF_FILE,
    
    // 字面量
    IDENTIFIER,         // 标识符
    STRING_LITERAL,     // 字符串字面量 "..."
    SINGLE_STRING,      // 单引号字符串 '...'
    UNQUOTED_LITERAL,   // 无引号字面量
    NUMBER_LITERAL,     // 数字字面量
    
    // 关键字
    TEXT,               // text
    STYLE,              // style
    TEMPLATE,           // Template (在[Template]中)
    CUSTOM,             // Custom (在[Custom]中)
    NAMESPACE,          // Namespace (在[Namespace]中)
    IMPORT,             // Import (在[Import]中)
    CONFIGURATION,      // Configuration (在[Configuration]中)
    ORIGIN,             // Origin (在[Origin]中)
    FROM,               // from
    AS,                 // as
    INHERIT,            // inherit
    EXCEPT,             // except
    DELETE,             // delete
    ADD,                // add
    
    // 符号和操作符
    LEFT_BRACE,         // {
    RIGHT_BRACE,        // }
    LEFT_BRACKET,       // [
    RIGHT_BRACKET,      // ]
    LEFT_PAREN,         // (
    RIGHT_PAREN,        // )
    SEMICOLON,          // ;
    COLON,              // :
    EQUAL,              // =
    DOT,                // .
    COMMA,              // ,
    AMPERSAND,          // &
    HASH,               // #
    AT,                 // @
    ARROW,              // ->
    DOUBLE_COLON,       // ::
    
    // 注释
    SINGLE_LINE_COMMENT,  // //
    MULTI_LINE_COMMENT,   // /* */
    GENERATOR_COMMENT,    // --
    
    // 特殊标记
    NEWLINE,            // \n
    WHITESPACE,         // 空白字符
    
    // CSS选择器相关
    CLASS_SELECTOR,     // .class
    ID_SELECTOR,        // #id
    PSEUDO_CLASS,       // :hover, :focus等
    PSEUDO_ELEMENT,     // ::before, ::after等
    
    // 错误标记
    INVALID,            // 无效token
    UNKNOWN             // 未知字符
};

/**
 * Token结构
 */
struct Token {
    TokenType type;
    std::string value;
    SourceLocation location;
    
    Token(TokenType t, const std::string& v, const SourceLocation& loc)
        : type(t), value(v), location(loc) {}
    
    Token() : type(TokenType::INVALID) {}
    
    bool isValid() const { return type != TokenType::INVALID && type != TokenType::UNKNOWN; }
    std::string toString() const;
};

/**
 * CHTL词法分析器
 */
class Lexer {
public:
    explicit Lexer(const std::string& source, const std::string& filename = "");
    
    /**
     * 获取下一个token
     */
    Token nextToken();
    
    /**
     * 查看下一个token但不消费
     */
    Token peekToken();
    
    /**
     * 获取当前位置
     */
    SourceLocation getCurrentLocation() const;
    
    /**
     * 检查是否到达文件末尾
     */
    bool isAtEnd() const;
    
    /**
     * 获取所有tokens（用于调试）
     */
    std::vector<Token> tokenizeAll();
    
    /**
     * 设置是否跳过空白字符和注释
     */
    void setSkipWhitespace(bool skip) { skipWhitespace_ = skip; }
    void setSkipComments(bool skip) { skipComments_ = skip; }
    
    /**
     * 获取错误信息
     */
    const std::vector<std::string>& getErrors() const { return errors_; }

private:
    std::string source_;
    std::string filename_;
    size_t position_;
    size_t line_;
    size_t column_;
    bool skipWhitespace_;
    bool skipComments_;
    std::vector<std::string> errors_;
    
    // 关键字映射
    static std::unordered_map<std::string, TokenType> keywords_;
    
    /**
     * 当前字符相关操作
     */
    char currentChar() const;
    char peekChar(size_t offset = 1) const;
    void advance();
    bool match(char expected);
    bool match(const std::string& expected);
    
    /**
     * 跳过操作
     */
    void skipWhitespace();
    void skipSingleLineComment();
    void skipMultiLineComment();
    
    /**
     * Token生成方法
     */
    Token makeToken(TokenType type, const std::string& value = "");
    Token makeStringLiteral();
    Token makeSingleStringLiteral();
    Token makeUnquotedLiteral();
    Token makeNumberLiteral();
    Token makeIdentifierOrKeyword();
    Token makeGeneratorComment();
    Token makeCssSelector();
    Token makePseudoSelector();
    
    /**
     * 字符判断方法
     */
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isHexDigit(char c) const;
    bool isWhitespace(char c) const;
    bool isNewline(char c) const;
    bool isValidUnquotedChar(char c) const;
    bool isValidIdentifierStart(char c) const;
    bool isValidIdentifierChar(char c) const;
    
    /**
     * 错误处理
     */
    void addError(const std::string& message);
    Token makeErrorToken(const std::string& message);
    
    /**
     * 位置更新
     */
    void updatePosition(char c);
    SourceLocation makeLocation() const;
};

/**
 * Token工具函数
 */
namespace TokenUtils {
    /**
     * 获取token类型的字符串表示
     */
    std::string getTokenTypeName(TokenType type);
    
    /**
     * 检查token是否是关键字
     */
    bool isKeyword(TokenType type);
    
    /**
     * 检查token是否是字面量
     */
    bool isLiteral(TokenType type);
    
    /**
     * 检查token是否是操作符
     */
    bool isOperator(TokenType type);
    
    /**
     * 检查token是否是分隔符
     */
    bool isDelimiter(TokenType type);
    
    /**
     * 检查token是否是注释
     */
    bool isComment(TokenType type);
    
    /**
     * 获取CHTL关键字列表
     */
    std::vector<std::string> getCHTLKeywords();
    
    /**
     * 检查字符串是否是有效的HTML标签
     */
    bool isValidHtmlTag(const std::string& tag);
    
    /**
     * 检查字符串是否是有效的CSS属性
     */
    bool isValidCssProperty(const std::string& property);
}

} // namespace ast
} // namespace chtl