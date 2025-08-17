#pragma once

#include "Common/FragmentTypes.h"
#include <string>
#include <vector>
#include <variant>

namespace Chtl {

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
        : Type_(type), Text_(text), Location_(location) {}
    
    Token(TokenType type, const TokenValue& value, const std::string& text,
          const SourceLocation& location = SourceLocation{})
        : Type_(type), Value_(value), Text_(text), Location_(location) {}
    
    // 基础访问器
    TokenType GetType() const { return Type_; }
    const std::string& GetText() const { return Text_; }
    const SourceLocation& GetLocation() const { return Location_; }
    const TokenValue& GetValue() const { return Value_; }
    
    // 类型检查
    bool IsKeyword() const;
    bool IsOperator() const;
    bool IsLiteral() const;
    bool IsDelimiter() const;
    bool IsComment() const;
    bool IsChtlKeyword() const;
    bool IsChtlJsKeyword() const;
    bool IsTemplateKeyword() const;
    
    // 值获取
    std::string GetStringValue() const;
    int GetIntValue() const;
    double GetDoubleValue() const;
    bool GetBoolValue() const;
    
    // 设置器
    void SetValue(const TokenValue& value) { Value_ = value; }
    void SetText(const std::string& text) { Text_ = text; }
    void SetLocation(const SourceLocation& location) { Location_ = location; }
    
    // 工具函数
    std::string ToString() const;
    bool Equals(const Token& other) const;

private:
    TokenType Type_;
    TokenValue Value_;
    std::string Text_;
    SourceLocation Location_;
};

/**
 * Token流类
 */
class TokenStream {
public:
    TokenStream() : CurrentIndex_(0) {}
    explicit TokenStream(std::vector<Token> tokens) 
        : Tokens_(std::move(tokens)), CurrentIndex_(0) {}
    
    // 基础操作
    void AddToken(const Token& token) { Tokens_.push_back(token); }
    void AddToken(Token&& token) { Tokens_.push_back(std::move(token)); }
    
    // 流操作
    const Token& Current() const;
    const Token& Peek(size_t offset = 1) const;
    const Token& Previous() const;
    
    Token& Advance();
    bool HasNext() const { return CurrentIndex_ < Tokens_.size(); }
    bool HasPrevious() const { return CurrentIndex_ > 0; }
    
    // 位置操作
    void Reset() { CurrentIndex_ = 0; }
    void Seek(size_t index) { CurrentIndex_ = std::min(index, Tokens_.size()); }
    size_t GetCurrentIndex() const { return CurrentIndex_; }
    size_t Size() const { return Tokens_.size(); }
    
    // 查找操作
    bool Match(TokenType type);
    bool Match(const std::vector<TokenType>& types);
    bool Check(TokenType type) const;
    bool IsAtEnd() const { return CurrentIndex_ >= Tokens_.size(); }
    
    // 消费操作
    Token Consume(TokenType type, const std::string& errorMessage = "");
    void SkipWhitespace();
    void SkipComments();
    void SkipWhitespaceAndComments();
    
    // 容器操作
    const std::vector<Token>& GetTokens() const { return Tokens_; }
    std::vector<Token>& GetTokens() { return Tokens_; }
    
    void Clear() { 
        Tokens_.clear(); 
        CurrentIndex_ = 0; 
    }
    
    // 调试功能
    std::string ToString() const;
    void PrintTokens() const;

private:
    std::vector<Token> Tokens_;
    size_t CurrentIndex_;
    
    static const Token EofToken_;
};

/**
 * Token工具类
 */
class TokenUtils {
public:
    static std::string TokenTypeToString(TokenType type);
    static TokenType StringToTokenType(const std::string& str);
    
    static bool IsKeyword(const std::string& text);
    static bool IsChtlKeyword(const std::string& text);
    static bool IsChtlJsKeyword(const std::string& text);
    
    static TokenType GetKeywordType(const std::string& text);
    static bool IsValidIdentifier(const std::string& text);
    static bool IsValidNumber(const std::string& text);
    
    // 特殊Token识别
    static bool IsEnhancedSelector(const std::string& text);
    static bool IsCssSelector(const std::string& text);
    static std::string ExtractSelectorContent(const std::string& text);
};

} // namespace Chtl