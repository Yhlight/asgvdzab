#ifndef CHTL_LEXER_TOKEN_H
#define CHTL_LEXER_TOKEN_H

#include <string>
#include <ostream>

namespace Chtl {

// 严格按照CHTL语法文档定义的Token类型
enum class TokenType {
    // 字面量
    IDENTIFIER,              // 标识符（HTML元素名、属性名等）
    STRING_DOUBLE_QUOTE,     // 双引号字符串 ""
    STRING_SINGLE_QUOTE,     // 单引号字符串 ''
    UNQUOTED_LITERAL,        // 无引号字面量
    NUMBER,                  // 数字
    
    // 关键字 - 元素相关
    TEXT,                    // text
    STYLE,                   // style （局部样式块）
    SCRIPT,                  // script （局部脚本块）
    
    // 关键字 - 模板和自定义
    TEMPLATE_MARK,           // [Template]
    CUSTOM_MARK,             // [Custom]
    ORIGIN_MARK,             // [Origin]
    IMPORT_MARK,             // [Import]
    NAMESPACE_MARK,          // [Namespace]
    CONFIGURATION_MARK,      // [Configuration]
    INFO_MARK,               // [Info]
    EXPORT_MARK,             // [Export]
    
    // @ 前缀类型
    AT_STYLE,                // @Style
    AT_ELEMENT,              // @Element
    AT_VAR,                  // @Var
    AT_HTML,                 // @Html
    AT_JAVASCRIPT,           // @JavaScript
    AT_CHTL,                 // @Chtl
    AT_CJMOD,                // @CJmod
    
    // 关键字 - 操作
    INHERIT,                 // inherit
    DELETE,                  // delete
    INSERT,                  // insert
    AFTER,                   // after
    BEFORE,                  // before
    REPLACE,                 // replace
    AT_TOP,                  // at top
    AT_BOTTOM,               // at bottom
    FROM,                    // from
    AS,                      // as
    EXCEPT,                  // except
    
    // 操作符
    LBRACE,                  // {
    RBRACE,                  // }
    LPAREN,                  // (
    RPAREN,                  // )
    LBRACKET,                // [
    RBRACKET,                // ]
    SEMICOLON,               // ;
    COLON,                   // :
    EQUALS,                  // =
    COMMA,                   // ,
    DOT,                     // .
    AMPERSAND,               // &
    ARROW,                   // -> (CHTL JS专用)
    
    // 注释
    COMMENT_SINGLE,          // // 注释
    COMMENT_MULTI,           // /* */ 注释
    COMMENT_GENERATOR,       // -- 生成器识别的注释
    
    // CHTL JS 特殊语法
    DOUBLE_LBRACE,           // {{
    DOUBLE_RBRACE,           // }}
    
    // 特殊
    NEWLINE,                 // 换行符
    WHITESPACE,              // 空白符
    EOF_TOKEN,               // 文件结束
    
    // 错误
    ERROR                    // 词法错误
};

// Token结构体
struct Token {
    TokenType type;
    std::string value;
    std::string file;
    size_t line;
    size_t column;
    size_t position;         // 在源码中的位置
    
    Token() 
        : type(TokenType::EOF_TOKEN), line(1), column(1), position(0) {}
    
    Token(TokenType t, const std::string& v, const std::string& f, 
          size_t l, size_t c, size_t p)
        : type(t), value(v), file(f), line(l), column(c), position(p) {}
    
    // 判断是否是关键字
    bool isKeyword() const {
        return type >= TokenType::TEXT && type <= TokenType::EXCEPT;
    }
    
    // 判断是否是字面量
    bool isLiteral() const {
        return type >= TokenType::IDENTIFIER && type <= TokenType::NUMBER;
    }
    
    // 判断是否是操作符
    bool isOperator() const {
        return type >= TokenType::LBRACE && type <= TokenType::ARROW;
    }
    
    // 获取Token类型名称
    static std::string getTypeName(TokenType type);
    
    // 调试输出
    std::string toString() const;
};

// Token类型名称映射
inline std::string Token::getTypeName(TokenType type) {
    switch (type) {
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING_DOUBLE_QUOTE: return "STRING_DOUBLE_QUOTE";
        case TokenType::STRING_SINGLE_QUOTE: return "STRING_SINGLE_QUOTE";
        case TokenType::UNQUOTED_LITERAL: return "UNQUOTED_LITERAL";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::TEXT: return "TEXT";
        case TokenType::STYLE: return "STYLE";
        case TokenType::SCRIPT: return "SCRIPT";
        case TokenType::TEMPLATE_MARK: return "[Template]";
        case TokenType::CUSTOM_MARK: return "[Custom]";
        case TokenType::ORIGIN_MARK: return "[Origin]";
        case TokenType::IMPORT_MARK: return "[Import]";
        case TokenType::NAMESPACE_MARK: return "[Namespace]";
        case TokenType::CONFIGURATION_MARK: return "[Configuration]";
        case TokenType::INFO_MARK: return "[Info]";
        case TokenType::EXPORT_MARK: return "[Export]";
        case TokenType::AT_STYLE: return "@Style";
        case TokenType::AT_ELEMENT: return "@Element";
        case TokenType::AT_VAR: return "@Var";
        case TokenType::AT_HTML: return "@Html";
        case TokenType::AT_JAVASCRIPT: return "@JavaScript";
        case TokenType::AT_CHTL: return "@Chtl";
        case TokenType::AT_CJMOD: return "@CJmod";
        case TokenType::INHERIT: return "inherit";
        case TokenType::DELETE: return "delete";
        case TokenType::INSERT: return "insert";
        case TokenType::AFTER: return "after";
        case TokenType::BEFORE: return "before";
        case TokenType::REPLACE: return "replace";
        case TokenType::AT_TOP: return "at top";
        case TokenType::AT_BOTTOM: return "at bottom";
        case TokenType::FROM: return "from";
        case TokenType::AS: return "as";
        case TokenType::EXCEPT: return "except";
        case TokenType::LBRACE: return "{";
        case TokenType::RBRACE: return "}";
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";
        case TokenType::LBRACKET: return "[";
        case TokenType::RBRACKET: return "]";
        case TokenType::SEMICOLON: return ";";
        case TokenType::COLON: return ":";
        case TokenType::EQUALS: return "=";
        case TokenType::COMMA: return ",";
        case TokenType::DOT: return ".";
        case TokenType::AMPERSAND: return "&";
        case TokenType::ARROW: return "->";
        case TokenType::COMMENT_SINGLE: return "//";
        case TokenType::COMMENT_MULTI: return "/* */";
        case TokenType::COMMENT_GENERATOR: return "--";
        case TokenType::DOUBLE_LBRACE: return "{{";
        case TokenType::DOUBLE_RBRACE: return "}}";
        case TokenType::NEWLINE: return "\\n";
        case TokenType::WHITESPACE: return "WHITESPACE";
        case TokenType::EOF_TOKEN: return "EOF";
        case TokenType::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

inline std::string Token::toString() const {
    return getTypeName(type) + " '" + value + "' at " + 
           file + ":" + std::to_string(line) + ":" + std::to_string(column);
}

} // namespace Chtl

#endif // CHTL_LEXER_TOKEN_H