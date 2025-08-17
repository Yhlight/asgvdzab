#pragma once

#include "Types.h"
#include <string>

namespace CHTL {

// Token类型枚举
enum class TokenType {
    // 基本标识符和字面量
    IDENTIFIER,         // 标识符
    STRING_LITERAL,     // 字符串字面量
    NUMBER_LITERAL,     // 数字字面量
    UNQUOTED_LITERAL,   // 无引号字面量
    
    // 操作符
    COLON,              // :
    SEMICOLON,          // ;
    EQUALS,             // =
    COMMA,              // ,
    DOT,                // .
    HASH,               // #
    AMPERSAND,          // &
    ARROW,              // ->
    
    // 括号
    LEFT_BRACE,         // {
    RIGHT_BRACE,        // }
    LEFT_PAREN,         // (
    RIGHT_PAREN,        // )
    LEFT_BRACKET,       // [
    RIGHT_BRACKET,      // ]
    
    // 关键字
    KEYWORD_TEXT,       // text
    KEYWORD_STYLE,      // style
    KEYWORD_SCRIPT,     // script
    KEYWORD_INHERIT,    // inherit
    KEYWORD_DELETE,     // delete
    KEYWORD_INSERT,     // insert
    KEYWORD_AFTER,      // after
    KEYWORD_BEFORE,     // before
    KEYWORD_REPLACE,    // replace
    KEYWORD_AT_TOP,     // at top
    KEYWORD_AT_BOTTOM,  // at bottom
    KEYWORD_FROM,       // from
    KEYWORD_AS,         // as
    KEYWORD_EXCEPT,     // except
    KEYWORD_VIR,        // vir
    
    // 块类型标识符
    TEMPLATE,           // [Template]
    CUSTOM,             // [Custom]
    ORIGIN,             // [Origin]
    IMPORT,             // [Import]
    NAMESPACE,          // [Namespace]
    CONFIGURATION,      // [Configuration]
    INFO,               // [Info]
    EXPORT,             // [Export]
    
    // 类型标识符
    AT_STYLE,           // @Style
    AT_ELEMENT,         // @Element
    AT_VAR,             // @Var
    AT_HTML,            // @Html
    AT_JAVASCRIPT,      // @JavaScript
    AT_CHTL,            // @Chtl
    AT_CJMOD,           // @CJmod
    
    // 特殊
    COMMENT_LINE,       // //
    COMMENT_BLOCK,      // /* */
    COMMENT_GENERATOR,  // --
    EOF_TOKEN,          // 文件结束
    NEWLINE,            // 换行
    WHITESPACE,         // 空白字符
    
    // 错误
    INVALID             // 无效token
};

class Token {
public:
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
    size_t position;
    
    Token(TokenType t, const std::string& v, size_t l, size_t c, size_t p)
        : type(t), value(v), line(l), column(c), position(p) {}
    
    // 获取token类型的字符串表示
    std::string getTypeString() const;
    
    // 判断是否为关键字
    bool isKeyword() const;
    
    // 判断是否为操作符
    bool isOperator() const;
    
    // 判断是否为字面量
    bool isLiteral() const;
    
    // 判断是否为块标识符
    bool isBlockIdentifier() const;
    
    // 判断是否为类型标识符
    bool isTypeIdentifier() const;
};

} // namespace CHTL