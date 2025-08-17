#ifndef CHTL_TOKENS_H
#define CHTL_TOKENS_H

#include <string>
#include <unordered_set>
#include <unordered_map>

namespace chtl {

// Token类型 - 严格按照CHTL语法文档定义
enum class TokenType {
    // 基础标记
    IDENTIFIER,           // 标识符
    KEYWORD,             // 关键字
    STRING_LITERAL,      // 字符串字面量 ("" 或 '')
    UNQUOTED_LITERAL,    // 无引号字面量
    NUMBER,              // 数字
    
    // 注释
    COMMENT_SINGLE,      // // 注释
    COMMENT_MULTI,       // /* */ 注释
    COMMENT_GENERATOR,   // -- 生成器识别的注释
    
    // 操作符和分隔符
    LBRACE,              // {
    RBRACE,              // }
    LPAREN,              // (
    RPAREN,              // )
    LBRACKET,            // [
    RBRACKET,            // ]
    COLON,               // : (CE对等式)
    EQUALS,              // = (CE对等式)
    SEMICOLON,           // ;
    COMMA,               // ,
    DOT,                 // .
    AT,                  // @
    AMPERSAND,           // &
    ARROW,               // ->
    
    // 特殊标记
    WHITESPACE,          // 空白
    NEWLINE,             // 换行
    EOF_TOKEN            // 文件结束
};

// CHTL关键字 - 严格按照语法文档定义
inline const std::unordered_set<std::string>& getCHTLKeywords() {
    static const std::unordered_set<std::string> keywords = {
        // 基础关键字
        "text",              // 文本节点
        "style",             // 局部样式块
        "script",            // 局部脚本块
        
        // 模板和自定义关键字
        "inherit",           // 显性继承
        "delete",            // 删除操作
        "insert",            // 插入操作
        "after",             // insert位置
        "before",            // insert位置  
        "replace",           // insert位置
        "at",                // at top/bottom
        "top",               // at top
        "bottom",            // at bottom
        
        // 导入相关
        "from",              // 导入来源
        "as",                // 重命名
        "except",            // 约束
        
        // CHTL JS关键字
        "vir",               // 虚对象
        
        // 特殊函数（CHTL JS）
        "listen",            // 增强监听器
        "delegate",          // 事件委托
        "animate"            // 动画
    };
    return keywords;
}

// 特殊块标记 - 严格按照语法文档定义
inline const std::unordered_set<std::string>& getSpecialBlocks() {
    static const std::unordered_set<std::string> blocks = {
        "[Template]",        // 模板定义
        "[Custom]",          // 自定义定义
        "[Origin]",          // 原始嵌入
        "[Import]",          // 导入
        "[Namespace]",       // 命名空间
        "[Configuration]",   // 配置组
        "[Info]",           // CMOD信息
        "[Export]",         // CMOD导出表
        "[Name]",           // 配置组Name块
        "[OriginType]"      // 配置组OriginType块
    };
    return blocks;
}

// 类型标记 - 严格按照语法文档定义
inline const std::unordered_set<std::string>& getTypeMarkers() {
    static const std::unordered_set<std::string> types = {
        "@Style",            // 样式组
        "@Element",          // 元素
        "@Var",              // 变量组
        "@Html",             // HTML原始嵌入
        "@JavaScript",       // JS原始嵌入
        "@Chtl",             // CHTL文件导入
        "@CJmod"             // CJmod导入
    };
    return types;
}

// HTML标签集合 - 常用HTML5标签
inline const std::unordered_set<std::string>& getHTMLTags() {
    static const std::unordered_set<std::string> tags = {
        // 文档结构
        "html", "head", "body", "meta", "link", "title",
        
        // 内容分区
        "header", "nav", "main", "section", "article", "aside", "footer",
        "h1", "h2", "h3", "h4", "h5", "h6",
        
        // 文本内容
        "div", "p", "span", "a", "em", "strong", "small", "mark",
        "del", "ins", "sub", "sup", "br", "hr",
        
        // 列表
        "ul", "ol", "li", "dl", "dt", "dd",
        
        // 表格
        "table", "thead", "tbody", "tfoot", "tr", "th", "td",
        "caption", "colgroup", "col",
        
        // 表单
        "form", "input", "textarea", "button", "select", "option",
        "optgroup", "label", "fieldset", "legend", "datalist",
        
        // 多媒体
        "img", "picture", "source", "audio", "video", "track",
        "canvas", "svg", "iframe", "embed", "object", "param",
        
        // 其他
        "details", "summary", "dialog", "template", "slot"
    };
    return tags;
}

// Token结构
struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
    size_t start_pos;    // 在源码中的起始位置
    size_t end_pos;      // 在源码中的结束位置
    
    Token() : type(TokenType::EOF_TOKEN), line(1), column(1), start_pos(0), end_pos(0) {}
    
    Token(TokenType t, const std::string& v, size_t l, size_t c, size_t s, size_t e)
        : type(t), value(v), line(l), column(c), start_pos(s), end_pos(e) {}
        
    bool isKeyword() const {
        return type == TokenType::KEYWORD;
    }
    
    bool isKeyword(const std::string& kw) const {
        return type == TokenType::KEYWORD && value == kw;
    }
    
    bool isSpecialBlock() const {
        return type == TokenType::IDENTIFIER && getSpecialBlocks().count(value) > 0;
    }
    
    bool isTypeMarker() const {
        return type == TokenType::IDENTIFIER && value.length() > 0 && value[0] == '@';
    }
    
    bool isHTMLTag() const {
        return type == TokenType::IDENTIFIER && getHTMLTags().count(value) > 0;
    }
};

} // namespace chtl

#endif // CHTL_TOKENS_H