#ifndef CHTL_COMMON_TYPES_H
#define CHTL_COMMON_TYPES_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>

namespace chtl {

// 代码片段类型
enum class CodeSegmentType {
    CHTL,           // CHTL代码片段
    CHTL_JS,        // CHTL JS代码片段  
    CSS,            // 纯CSS代码片段
    JAVASCRIPT,     // 纯JavaScript代码片段
    HTML_RAW,       // 原始HTML代码片段
    UNKNOWN
};

// 代码片段位置信息
struct SourceLocation {
    size_t line;
    size_t column;
    size_t offset;  // 在源文件中的字节偏移
    std::string filename;
    
    SourceLocation() : line(1), column(1), offset(0) {}
    SourceLocation(size_t l, size_t c, size_t o, const std::string& f = "")
        : line(l), column(c), offset(o), filename(f) {}
};

// 代码片段
struct CodeSegment {
    CodeSegmentType type;
    std::string content;
    SourceLocation location;
    
    // 用于局部样式块和脚本块的上下文信息
    std::string parentElement;  // 父元素标签名
    std::string className;      // 自动生成的类名（如果有）
    std::string id;            // 自动生成的ID（如果有）
    
    CodeSegment(CodeSegmentType t = CodeSegmentType::UNKNOWN) : type(t) {}
};

// 编译结果
struct CompilationResult {
    bool success;
    std::string output;         // 编译后的代码
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    
    // 用于合并的额外信息
    std::string globalStyles;   // 需要添加到全局样式的内容
    std::string globalScripts;  // 需要添加到全局脚本的内容
    
    CompilationResult() : success(true) {}
};

// 编译选项
struct CompilerOptions {
    bool debugMode = false;
    bool minify = false;
    bool generateSourceMap = false;
    std::string outputPath;
    std::map<std::string, std::string> customOptions;
};

// Token类型（用于词法分析）
enum class TokenType {
    // 字面量
    IDENTIFIER,
    STRING_LITERAL,      // "string", 'string'
    NUMBER_LITERAL,
    UNQUOTED_LITERAL,    // 无修饰字面量（无引号的字符串）
    
    // 关键字 - 基础
    KEYWORD_TEXT,
    KEYWORD_STYLE,
    KEYWORD_SCRIPT,
    KEYWORD_TEMPLATE,
    KEYWORD_CUSTOM,
    KEYWORD_IMPORT,
    KEYWORD_NAMESPACE,
    KEYWORD_ORIGIN,
    KEYWORD_CONFIGURATION,
    
    // 关键字 - 模板相关
    KEYWORD_INHERIT,
    KEYWORD_DELETE,
    KEYWORD_INSERT,
    KEYWORD_EXCEPT,
    KEYWORD_FROM,
    KEYWORD_AS,
    
    // 关键字 - 自定义相关
    KEYWORD_AFTER,
    KEYWORD_BEFORE,
    KEYWORD_REPLACE,
    KEYWORD_AT,
    KEYWORD_TOP,
    KEYWORD_BOTTOM,
    
    // 符号
    LEFT_BRACE,          // {
    RIGHT_BRACE,         // }
    LEFT_BRACKET,        // [
    RIGHT_BRACKET,       // ]
    LEFT_PAREN,          // (
    RIGHT_PAREN,         // )
    SEMICOLON,           // ;
    COLON,               // :
    EQUALS,              // =
    COMMA,               // ,
    DOT,                 // .
    AT,                  // @
    AMPERSAND,           // &
    ARROW,               // -> (CHTL JS)
    DOUBLE_LEFT_BRACE,   // {{ (CHTL JS增强选择器)
    DOUBLE_RIGHT_BRACE,  // }} (CHTL JS增强选择器)
    HASH,                // #
    DOUBLE_DASH,         // -- (会被生成器识别的注释)
    
    // 注释
    SINGLE_LINE_COMMENT, // //
    MULTI_LINE_COMMENT,  // /* */
    SEMANTIC_COMMENT,    // -- (语义注释)
    
    // 特殊
    WHITESPACE,
    NEWLINE,
    EOF_TOKEN,
    UNKNOWN
};

// Token结构
struct Token {
    TokenType type;
    std::string value;
    SourceLocation location;
    
    Token(TokenType t = TokenType::UNKNOWN) : type(t) {}
};

} // namespace chtl

#endif // CHTL_COMMON_TYPES_H