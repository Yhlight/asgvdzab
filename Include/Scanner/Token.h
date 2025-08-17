#pragma once

#include "Common/FragmentTypes.h"
#include <string>
#include <vector>
#include <variant>
#include <unordered_set>

namespace Chtl {

/**
 * Token类型枚举 - 严格按照CHTL语法文档定义
 */
enum class TokenType {
    // === 基础字面量类型 ===
    IDENTIFIER,                 // 标识符
    UNQUOTED_STRING,           // 无修饰字面量(没有引号的字符串)
    DOUBLE_QUOTED_STRING,      // 双引号字符串 ""
    SINGLE_QUOTED_STRING,      // 单引号字符串 ''
    
    // === 分隔符和操作符 ===
    LBRACE,                    // {
    RBRACE,                    // }
    LBRACKET,                  // [
    RBRACKET,                  // ]
    LPAREN,                    // (
    RPAREN,                    // )
    SEMICOLON,                 // ;
    COLON,                     // : (CE对等式)
    ASSIGN,                    // = (CE对等式，与:完全等价)
    COMMA,                     // ,
    DOT,                       // .
    
    // === CHTL核心关键字 ===
    TEXT,                      // text (文本节点)
    STYLE,                     // style (局部样式块)
    SCRIPT,                    // script (局部脚本块)
    
    // === HTML元素标签 ===
    HTML_ELEMENT,              // 所有HTML元素标签
    
    // === 注释类型 ===
    LINE_COMMENT,              // // 注释
    BLOCK_COMMENT,             // /* */ 多行注释
    GENERATOR_COMMENT,         // -- 生成器识别的注释
    
    // === 模板系统Token ===
    TEMPLATE_BRACKET,          // [Template]
    CUSTOM_BRACKET,            // [Custom]
    ORIGIN_BRACKET,            // [Origin]
    CONFIGURATION_BRACKET,     // [Configuration]
    NAMESPACE_BRACKET,         // [Namespace]
    IMPORT_BRACKET,            // [Import]
    INFO_BRACKET,              // [Info] (CMOD专用)
    EXPORT_BRACKET,            // [Export] (CMOD专用)
    
    AT_STYLE,                  // @Style
    AT_ELEMENT,                // @Element
    AT_VAR,                    // @Var
    AT_HTML,                   // @Html
    AT_JAVASCRIPT,             // @JavaScript
    AT_CHTL,                   // @Chtl
    AT_CJMOD,                  // @CJmod
    
    // === 控制关键字 ===
    INHERIT,                   // inherit
    DELETE,                    // delete
    INSERT,                    // insert
    AFTER,                     // after
    BEFORE,                    // before
    REPLACE,                   // replace
    AT_TOP,                    // at top
    AT_BOTTOM,                 // at bottom
    FROM,                      // from
    AS,                        // as
    EXCEPT,                    // except
    
    // === CHTL JS特有Token ===
    ENHANCED_SELECTOR,         // {{CSS选择器}}
    ARROW_OPERATOR,            // -> (CHTL JS箭头操作符)
    VIR,                       // vir (虚对象)
    LISTEN,                    // listen (增强监听器)
    DELEGATE,                  // delegate (事件委托)
    ANIMATE,                   // animate (动画)
    
    // === 特殊Token ===
    AMPERSAND,                 // & (上下文推导)
    NEWLINE,                   // \n
    WHITESPACE,                // 空白字符
    EOF_TOKEN,                 // 文件结束
    UNKNOWN                    // 未知Token
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
    bool IsHtmlElement() const;
    bool IsEnhancedSelector() const;
    
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
 * CHTL关键字表 - 严格按照语法文档定义
 */
class ChtlKeywords {
public:
    // 获取所有HTML元素标签
    static const std::unordered_set<std::string>& GetHtmlElements();
    
    // 获取CHTL核心关键字
    static const std::unordered_set<std::string>& GetChtlCoreKeywords();
    
    // 获取模板系统关键字
    static const std::unordered_set<std::string>& GetTemplateKeywords();
    
    // 获取控制关键字
    static const std::unordered_set<std::string>& GetControlKeywords();
    
    // 获取CHTL JS关键字
    static const std::unordered_set<std::string>& GetChtlJsKeywords();
    
    // 检查函数
    static bool IsHtmlElement(const std::string& text);
    static bool IsChtlCoreKeyword(const std::string& text);
    static bool IsTemplateKeyword(const std::string& text);
    static bool IsControlKeyword(const std::string& text);
    static bool IsChtlJsKeyword(const std::string& text);
    static bool IsEnhancedSelector(const std::string& text);
    
    // 获取Token类型
    static TokenType GetKeywordType(const std::string& text);

private:
    static void InitializeKeywords();
    static std::unordered_set<std::string> HtmlElements_;
    static std::unordered_set<std::string> ChtlCoreKeywords_;
    static std::unordered_set<std::string> TemplateKeywords_;
    static std::unordered_set<std::string> ControlKeywords_;
    static std::unordered_set<std::string> ChtlJsKeywords_;
    static bool Initialized_;
};

/**
 * Token工具类
 */
class TokenUtils {
public:
    static std::string TokenTypeToString(TokenType type);
    static TokenType StringToTokenType(const std::string& str);
    
    static bool IsValidIdentifier(const std::string& text);
    static bool IsValidUnquotedString(const std::string& text);
    
    // 增强选择器解析
    static bool IsEnhancedSelector(const std::string& text);
    static std::string ExtractSelectorContent(const std::string& text);
    static bool ValidateEnhancedSelector(const std::string& selector);
    
    // 字面量处理
    static std::string ProcessUnquotedString(const std::string& text);
    static std::string ProcessQuotedString(const std::string& text, char quote);
    
    // CE对等式处理
    static bool IsColonEqualOperator(TokenType type);
};

} // namespace Chtl