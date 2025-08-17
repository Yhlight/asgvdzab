#ifndef CHTLJS_TOKENS_H
#define CHTLJS_TOKENS_H

#include <string>
#include <unordered_set>

namespace chtl {

// CHTL JS Token类型 - 严格按照CHTL语法文档定义
enum class CHTLJSTokenType {
    // 基础标记
    IDENTIFIER,           // 标识符
    KEYWORD,             // 关键字 (vir, listen, delegate, animate)
    STRING_LITERAL,      // 字符串字面量
    NUMBER,              // 数字
    
    // CHTL JS特有标记
    SELECTOR_START,      // {{
    SELECTOR_END,        // }}
    ARROW,               // ->
    
    // JavaScript标记（用于传递给JS编译器的部分）
    JS_CODE,             // 纯JavaScript代码块
    
    // 操作符和分隔符
    LPAREN,              // (
    RPAREN,              // )
    LBRACE,              // {
    RBRACE,              // }
    LBRACKET,            // [
    RBRACKET,            // ]
    COLON,               // :
    SEMICOLON,           // ;
    COMMA,               // ,
    DOT,                 // .
    EQUALS,              // =
    
    // 特殊标记
    WHITESPACE,          // 空白
    NEWLINE,             // 换行
    COMMENT,             // 注释
    EOF_TOKEN            // 文件结束
};

// CHTL JS关键字 - 严格按照语法文档定义
inline const std::unordered_set<std::string>& getCHTLJSKeywords() {
    static const std::unordered_set<std::string> keywords = {
        "vir",               // 虚对象
        "listen",            // 增强监听器
        "delegate",          // 事件委托
        "animate",           // 动画
        
        // JavaScript保留字（需要识别但不处理）
        "var", "let", "const", "function", "return", "if", "else", 
        "for", "while", "do", "switch", "case", "break", "continue",
        "try", "catch", "finally", "throw", "new", "this", "class"
    };
    return keywords;
}

// CHTL JS特殊函数 - 这些是CHTL JS提供的增强函数
inline const std::unordered_set<std::string>& getCHTLJSFunctions() {
    static const std::unordered_set<std::string> functions = {
        "listen",            // 增强监听器
        "delegate",          // 事件委托
        "animate"            // 动画
    };
    return functions;
}

// CHTL JS Token结构
struct CHTLJSToken {
    CHTLJSTokenType type;
    std::string value;
    size_t line;
    size_t column;
    size_t start_pos;
    size_t end_pos;
    
    CHTLJSToken() : type(CHTLJSTokenType::EOF_TOKEN), line(1), column(1), 
                   start_pos(0), end_pos(0) {}
    
    CHTLJSToken(CHTLJSTokenType t, const std::string& v, size_t l, size_t c, 
                size_t s, size_t e)
        : type(t), value(v), line(l), column(c), start_pos(s), end_pos(e) {}
        
    bool isKeyword() const {
        return type == CHTLJSTokenType::KEYWORD;
    }
    
    bool isKeyword(const std::string& kw) const {
        return type == CHTLJSTokenType::KEYWORD && value == kw;
    }
    
    bool isCHTLJSFunction() const {
        return type == CHTLJSTokenType::IDENTIFIER && 
               getCHTLJSFunctions().count(value) > 0;
    }
};

} // namespace chtl

#endif // CHTLJS_TOKENS_H