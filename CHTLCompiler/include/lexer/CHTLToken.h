#pragma once

#include <string>
#include <variant>

namespace chtl {

// CHTL Token类型枚举 - 严格按照语法文档
enum class TokenType {
    // 基础Token
    END_OF_FILE,        // 文件结束
    WHITESPACE,         // 空白字符
    NEWLINE,            // 换行符
    
    // 注释Token - 严格按照文档
    SINGLE_LINE_COMMENT,    // // 注释
    MULTI_LINE_COMMENT,     // /* */ 注释
    GENERATOR_COMMENT,      // -- 生成器注释
    
    // 字面量Token - 严格按照文档
    TEXT_LITERAL,          // text关键字
    STRING_LITERAL,        // 字符串字面量（双引号、单引号）
    UNQUOTED_LITERAL,      // 无修饰字面量（无引号）
    
    // 元素Token - 严格按照文档
    HTML_ELEMENT,          // HTML元素（html, head, body, div, span等）
    
    // 属性Token - 严格按照文档
    COLON,                 // : 属性分隔符
    EQUAL,                 // = 属性分隔符（与:等价）
    SEMICOLON,             // ; 属性结束符
    
    // 样式块Token - 严格按照文档
    STYLE_KEYWORD,         // style关键字
    CSS_PROPERTY,          // CSS属性
    CSS_VALUE,             // CSS值
    CSS_SELECTOR,          // CSS选择器（.class, #id）
    CONTEXT_REFERENCE,     // & 上下文引用
    
    // 模板Token - 严格按照文档
    TEMPLATE_START,        // [Template]
    STYLE_TEMPLATE,        // @Style
    ELEMENT_TEMPLATE,      // @Element
    VAR_TEMPLATE,          // @Var
    TEMPLATE_NAME,         // 模板名称
    TEMPLATE_INHERIT,      // inherit关键字
    
    // 自定义Token - 严格按照文档
    CUSTOM_START,          // [Custom]
    CUSTOM_STYLE,          // @Style（自定义）
    CUSTOM_ELEMENT,        // @Element（自定义）
    CUSTOM_VAR,            // @Var（自定义）
    DELETE_KEYWORD,        // delete关键字
    INSERT_KEYWORD,        // insert关键字
    AFTER_KEYWORD,         // after关键字
    BEFORE_KEYWORD,        // before关键字
    REPLACE_KEYWORD,       // replace关键字
    AT_TOP_KEYWORD,        // at top关键字
    AT_BOTTOM_KEYWORD,     // at bottom关键字
    FROM_KEYWORD,          // from关键字
    AS_KEYWORD,            // as关键字
    EXCEPT_KEYWORD,        // except关键字
    
    // 原始嵌入Token - 严格按照文档
    ORIGIN_START,          // [Origin]
    ORIGIN_HTML,           // @Html
    ORIGIN_STYLE,          // @Style（原始）
    ORIGIN_JAVASCRIPT,     // @JavaScript
    
    // 配置组Token - 严格按照文档
    CONFIGURATION_START,   // [Configuration]
    NAME_START,            // [Name]
    CONFIG_ASSIGNMENT,     // = 配置赋值
    CONFIG_OPTION,         // 配置选项
    
    // 导入Token - 严格按照文档
    IMPORT_START,          // [Import]
    IMPORT_HTML,           // @Html（导入）
    IMPORT_STYLE,          // @Style（导入）
    IMPORT_JAVASCRIPT,     // @JavaScript（导入）
    IMPORT_CHTL,           // @Chtl
    IMPORT_CJMOD,          // @CJmod
    
    // 命名空间Token - 严格按照文档
    NAMESPACE_START,       // [Namespace]
    NAMESPACE_NAME,        // 命名空间名称
    
    // 约束Token - 严格按照文档
    // EXCEPT_KEYWORD已在自定义Token中定义
    
    // 结构Token
    LEFT_BRACE,            // {
    RIGHT_BRACE,           // }
    LEFT_BRACKET,          // [
    RIGHT_BRACKET,         // ]
    LEFT_PAREN,            // (
    RIGHT_PAREN,           // )
    
    // 其他Token
    IDENTIFIER,            // 标识符
    UNKNOWN                // 未知Token
};

// CHTL JS Token类型枚举 - 严格按照语法文档
enum class CHTLJSTokenType {
    // 基础Token
    END_OF_FILE,        // 文件结束
    WHITESPACE,         // 空白字符
    NEWLINE,            // 换行符
    
    // 注释Token
    SINGLE_LINE_COMMENT,    // // 注释
    MULTI_LINE_COMMENT,     // /* */ 注释
    
    // 局部script Token - 严格按照文档
    SCRIPT_KEYWORD,         // script关键字
    
    // 增强选择器Token - 严格按照文档
    ENHANCED_SELECTOR_START,    // {{
    ENHANCED_SELECTOR_END,      // }}
    CSS_SELECTOR,               // CSS选择器内容
    ARRAY_ACCESS,               // [0] 数组访问
    
    // 链式访问Token - 严格按照文档
    CHAIN_ACCESS,               // -> 链式访问（推荐）
    DOT_ACCESS,                 // . 链式访问（等价）
    
    // 增强监听器Token - 严格按照文档
    LISTEN_KEYWORD,             // listen关键字
    
    // 事件委托Token - 严格按照文档
    DELEGATE_KEYWORD,           // delegate关键字
    TARGET_KEYWORD,             // target关键字
    
    // 动画Token - 严格按照文档
    ANIMATE_KEYWORD,            // animate关键字
    DURATION_KEYWORD,           // duration关键字
    EASING_KEYWORD,             // easing关键字
    BEGIN_KEYWORD,              // begin关键字
    WHEN_KEYWORD,               // when关键字
    AT_KEYWORD,                 // at关键字
    END_KEYWORD,                // end关键字
    LOOP_KEYWORD,               // loop关键字
    DIRECTION_KEYWORD,          // direction关键字
    DELAY_KEYWORD,              // delay关键字
    CALLBACK_KEYWORD,           // callback关键字
    
    // JavaScript语法Token（基础支持）
    FUNCTION_KEYWORD,           // function关键字
    VAR_KEYWORD,                // var关键字
    LET_KEYWORD,                // let关键字
    CONST_KEYWORD,              // const关键字
    IF_KEYWORD,                 // if关键字
    ELSE_KEYWORD,               // else关键字
    FOR_KEYWORD,                // for关键字
    WHILE_KEYWORD,              // while关键字
    RETURN_KEYWORD,             // return关键字
    CONSOLE_KEYWORD,            // console关键字
    DOCUMENT_KEYWORD,           // document关键字
    WINDOW_KEYWORD,             // window关键字
    
    // 结构Token
    LEFT_BRACE,            // {
    RIGHT_BRACE,           // }
    LEFT_BRACKET,          // [
    RIGHT_BRACKET,         // ]
    LEFT_PAREN,            // (
    RIGHT_PAREN,           // )
    SEMICOLON,             // ;
    COMMA,                 // ,
    COLON,                 // :
    EQUAL,                 // =
    PLUS,                  // +
    MINUS,                 // -
    ASTERISK,              // *
    SLASH,                 // /
    PERCENT,               // %
    EXCLAMATION,           // !
    QUESTION,              // ?
    LESS_THAN,             // <
    GREATER_THAN,          // >
    LESS_EQUAL,            // <=
    GREATER_EQUAL,         // >=
    EQUAL_EQUAL,           // ==
    NOT_EQUAL,             // !=
    AND_AND,               // &&
    OR_OR,                 // ||
    ARROW,                 // =>
    
    // 其他Token
    IDENTIFIER,            // 标识符
    NUMBER,                // 数字
    STRING,                // 字符串
    UNKNOWN                // 未知Token
};

// Token基类
class Token {
public:
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
    size_t position;
    
    Token(TokenType t, const std::string& v, size_t l, size_t c, size_t p)
        : type(t), value(v), line(l), column(c), position(p) {}
    
    virtual ~Token() = default;
    
    std::string toString() const;
};

// CHTL JS Token类
class CHTLJSToken {
public:
    CHTLJSTokenType type;
    std::string value;
    size_t line;
    size_t column;
    size_t position;
    
    CHTLJSToken(CHTLJSTokenType t, const std::string& v, size_t l, size_t c, size_t p)
        : type(t), value(v), line(l), column(c), position(p) {}
    
    virtual ~CHTLJSToken() = default;
    
    std::string toString() const;
};

// Token值类型（用于存储不同类型的值）
using TokenValue = std::variant<
    std::string,      // 字符串值
    int,              // 整数值
    double,           // 浮点值
    bool,             // 布尔值
    std::nullptr_t    // 空值
>;

} // namespace chtl