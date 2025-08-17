#pragma once

#include "common/fragment_types.h"
#include <string>
#include <vector>
#include <variant>

namespace chtl {

/**
 * Token类型枚举
 */
enum class TokenType {
    // 基础类型
    IDENTIFIER,         // 标识符
    STRING_LITERAL,     // 字符串字面量
    NUMBER_LITERAL,     // 数字字面量
    BOOLEAN_LITERAL,    // 布尔字面量
    
    // 分隔符
    LBRACE,            // {
    RBRACE,            // }
    LBRACKET,          // [
    RBRACKET,          // ]
    LPAREN,            // (
    RPAREN,            // )
    SEMICOLON,         // ;
    COLON,             // :
    COMMA,             // ,
    DOT,               // .
    ARROW,             // ->
    
    // 操作符
    ASSIGN,            // =
    PLUS,              // +
    MINUS,             // -
    MULTIPLY,          // *
    DIVIDE,            // /
    
    // CHTL关键字
    TEXT,              // text
    STYLE,             // style
    SCRIPT,            // script
    TEMPLATE,          // [Template]
    CUSTOM,            // [Custom]
    ORIGIN,            // [Origin]
    CONFIGURATION,     // [Configuration]
    NAMESPACE,         // [Namespace]
    IMPORT,            // [Import]
    
    // 模板关键字
    AT_STYLE,          // @Style
    AT_ELEMENT,        // @Element
    AT_VAR,            // @Var
    AT_HTML,           // @Html
    AT_JAVASCRIPT,     // @JavaScript
    AT_CHTL,           // @Chtl
    AT_CJMOD,          // @CJmod
    
    // 控制关键字
    INHERIT,           // inherit
    DELETE,            // delete
    INSERT,            // insert
    AFTER,             // after
    BEFORE,            // before
    REPLACE,           // replace
    AT_TOP,            // at top
    AT_BOTTOM,         // at bottom
    FROM,              // from
    AS,                // as
    EXCEPT,            // except
    
    // CHTL JS关键字
    VIR,               // vir (虚对象)
    LISTEN,            // listen
    DELEGATE,          // delegate
    ANIMATE,           // animate
    
    // 特殊Token
    ENHANCED_SELECTOR, // {{selector}}
    CSS_SELECTOR,      // .class, #id等
    AMPERSAND,         // &
    
    // 注释
    LINE_COMMENT,      // //
    BLOCK_COMMENT,     // /* */
    GENERATOR_COMMENT, // --
    
    // 特殊
    NEWLINE,           // \n
    WHITESPACE,        // 空白字符
    EOF_TOKEN,         // 文件结束
    UNKNOWN            // 未知Token
};

/**
 * Token值类型
 */
using TokenValue = std::variant<std::string, int, double, bool>;

/**
 * Token类
 */
class Token {
public:
    Token(TokenType type, const std::string& text = "", 
          const SourceLocation& location = SourceLocation{})
        : type_(type), text_(text), location_(location) {}
    
    Token(TokenType type, const TokenValue& value, const std::string& text,
          const SourceLocation& location = SourceLocation{})
        : type_(type), value_(value), text_(text), location_(location) {}
    
    // 基础访问器
    TokenType getType() const { return type_; }
    const std::string& getText() const { return text_; }
    const SourceLocation& getLocation() const { return location_; }
    const TokenValue& getValue() const { return value_; }
    
    // 类型检查
    bool isKeyword() const;
    bool isOperator() const;
    bool isLiteral() const;
    bool isDelimiter() const;
    bool isComment() const;
    bool isCHTLKeyword() const;
    bool isCHTLJSKeyword() const;
    bool isTemplateKeyword() const;
    
    // 值获取
    std::string getStringValue() const;
    int getIntValue() const;
    double getDoubleValue() const;
    bool getBoolValue() const;
    
    // 设置器
    void setValue(const TokenValue& value) { value_ = value; }
    void setText(const std::string& text) { text_ = text; }
    void setLocation(const SourceLocation& location) { location_ = location; }
    
    // 工具函数
    std::string toString() const;
    bool equals(const Token& other) const;

private:
    TokenType type_;
    TokenValue value_;
    std::string text_;
    SourceLocation location_;
};

/**
 * Token流类
 */
class TokenStream {
public:
    TokenStream() : current_index_(0) {}
    explicit TokenStream(std::vector<Token> tokens) 
        : tokens_(std::move(tokens)), current_index_(0) {}
    
    // 基础操作
    void addToken(const Token& token) { tokens_.push_back(token); }
    void addToken(Token&& token) { tokens_.push_back(std::move(token)); }
    
    // 流操作
    const Token& current() const;
    const Token& peek(size_t offset = 1) const;
    const Token& previous() const;
    
    Token& advance();
    bool hasNext() const { return current_index_ < tokens_.size(); }
    bool hasPrevious() const { return current_index_ > 0; }
    
    // 位置操作
    void reset() { current_index_ = 0; }
    void seek(size_t index) { current_index_ = std::min(index, tokens_.size()); }
    size_t getCurrentIndex() const { return current_index_; }
    size_t size() const { return tokens_.size(); }
    
    // 查找操作
    bool match(TokenType type);
    bool match(const std::vector<TokenType>& types);
    bool check(TokenType type) const;
    bool isAtEnd() const { return current_index_ >= tokens_.size(); }
    
    // 消费操作
    Token consume(TokenType type, const std::string& error_message = "");
    void skipWhitespace();
    void skipComments();
    void skipWhitespaceAndComments();
    
    // 容器操作
    const std::vector<Token>& getTokens() const { return tokens_; }
    std::vector<Token>& getTokens() { return tokens_; }
    
    void clear() { 
        tokens_.clear(); 
        current_index_ = 0; 
    }
    
    // 调试功能
    std::string toString() const;
    void printTokens() const;

private:
    std::vector<Token> tokens_;
    size_t current_index_;
    
    static const Token EOF_TOKEN_;
};

/**
 * Token工具类
 */
class TokenUtils {
public:
    static std::string tokenTypeToString(TokenType type);
    static TokenType stringToTokenType(const std::string& str);
    
    static bool isKeyword(const std::string& text);
    static bool isCHTLKeyword(const std::string& text);
    static bool isCHTLJSKeyword(const std::string& text);
    
    static TokenType getKeywordType(const std::string& text);
    static bool isValidIdentifier(const std::string& text);
    static bool isValidNumber(const std::string& text);
    
    // 特殊Token识别
    static bool isEnhancedSelector(const std::string& text);
    static bool isCSSSelector(const std::string& text);
    static std::string extractSelectorContent(const std::string& text);
};

} // namespace chtl