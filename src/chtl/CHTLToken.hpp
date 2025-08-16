#pragma once

#include <string>
#include <ostream>

namespace chtl {

/**
 * CHTL Token类型
 * 根据CHTL语法文档定义
 */
enum class CHTLTokenType {
    // 结构符号
    LEFT_BRACE,         // {
    RIGHT_BRACE,        // }
    LEFT_BRACKET,       // [
    RIGHT_BRACKET,      // ]
    LEFT_PAREN,         // (
    RIGHT_PAREN,        // )
    SEMICOLON,          // ;
    COMMA,              // ,
    COLON,              // :
    EQUALS,             // =
    DOT,                // .
    AT,                 // @
    AMPERSAND,          // &
    
    // 注释
    COMMENT_LINE,       // //
    COMMENT_BLOCK,      // /* */
    COMMENT_HTML,       // --
    
    // 字面量
    STRING_LITERAL,     // "..." 或 '...'
    UNQUOTED_LITERAL,   // 无引号字面量
    NUMBER_LITERAL,     // 数字
    
    // 关键字 - 严格按照语法文档
    TEXT,               // text
    STYLE,              // style
    SCRIPT,             // script
    TEMPLATE,           // [Template]
    CUSTOM,             // [Custom]
    ORIGIN,             // [Origin]
    IMPORT,             // [Import]
    NAMESPACE,          // [Namespace]
    CONFIGURATION,      // [Configuration]
    NAME,               // [Name]
    INFO,               // [Info]
    EXPORT,             // [Export]
    
    // 模板/自定义修饰符
    AT_STYLE,           // @Style
    AT_ELEMENT,         // @Element
    AT_VAR,             // @Var
    AT_HTML,            // @Html
    AT_JAVASCRIPT,      // @JavaScript
    AT_CHTL,            // @Chtl
    AT_CJMOD,           // @CJmod
    
    // 操作关键字
    INHERIT,            // inherit
    DELETE,             // delete
    INSERT,             // insert
    AFTER,              // after
    BEFORE,             // before
    REPLACE,            // replace
    AT_TOP,             // at top
    AT_BOTTOM,          // at bottom
    FROM,               // from
    AS,                 // as
    EXCEPT,             // except
    
    // 标识符
    IDENTIFIER,         // 变量名、元素名等
    
    // 特殊
    EOF_TOKEN,          // 文件结束
    ERROR               // 错误Token
};

/**
 * CHTL Token
 */
struct CHTLToken {
    CHTLTokenType type;
    std::string value;
    size_t line;
    size_t column;
    
    CHTLToken(CHTLTokenType t, const std::string& v, size_t l, size_t c)
        : type(t), value(v), line(l), column(c) {}
    
    bool is(CHTLTokenType t) const { return type == t; }
    
    bool isKeyword() const {
        return type >= CHTLTokenType::TEXT && type <= CHTLTokenType::EXCEPT;
    }
    
    bool isStructural() const {
        return type >= CHTLTokenType::LEFT_BRACE && type <= CHTLTokenType::AMPERSAND;
    }
};

// 输出Token类型名称
const char* tokenTypeName(CHTLTokenType type);

// 输出操作符重载
std::ostream& operator<<(std::ostream& os, const CHTLToken& token);

} // namespace chtl