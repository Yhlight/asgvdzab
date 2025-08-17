#ifndef CHTL_CHTL_COMPILER_H
#define CHTL_CHTL_COMPILER_H

#include "compilers/base_compiler.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <set>

namespace chtl {

// CHTL词法单元类型
enum class TokenType {
    // 字面量
    IDENTIFIER,      // 标识符
    STRING_LITERAL,  // 字符串字面量
    NUMBER,          // 数字
    UNQUOTED_LITERAL, // 无引号字面量
    
    // 关键字
    TEXT,            // text
    STYLE,           // style  
    SCRIPT,          // script
    TEMPLATE,        // [Template]
    CUSTOM,          // [Custom]
    ORIGIN,          // [Origin]
    IMPORT,          // [Import]
    NAMESPACE,       // [Namespace]
    CONFIGURATION,   // [Configuration]
    
    // 模板类型
    AT_STYLE,        // @Style
    AT_ELEMENT,      // @Element
    AT_VAR,          // @Var
    
    // 操作符
    LBRACE,          // {
    RBRACE,          // }
    LPAREN,          // (
    RPAREN,          // )
    LBRACKET,        // [
    RBRACKET,        // ]
    SEMICOLON,       // ;
    COLON,           // :
    EQUALS,          // =
    COMMA,           // ,
    DOT,             // .
    AMPERSAND,       // &
    
    // 其他
    COMMENT,         // 注释
    WHITESPACE,      // 空白
    NEWLINE,         // 换行
    EOF_TOKEN        // 文件结束
};

// 词法单元
struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
    
    Token(TokenType t = TokenType::EOF_TOKEN, const std::string& v = "", 
          size_t l = 0, size_t c = 0)
        : type(t), value(v), line(l), column(c) {}
};

// CHTL编译器
class CHTLCompiler : public ICompiler {
public:
    CHTLCompiler();
    ~CHTLCompiler() override;
    
    // 实现ICompiler接口
    CompileResult compile(const CodeFragment& fragment,
                         const CompileOptions& options) override;
    
    std::string getName() const override { return "CHTL Compiler"; }
    
    CodeFragmentType getSupportedType() const override { 
        return CodeFragmentType::CHTL; 
    }
    
    void setDebugMode(bool debug) override { debug_mode_ = debug; }

private:
    // 词法分析
    std::vector<Token> tokenize(const std::string& source);
    Token getNextToken();
    void skipWhitespace();
    Token readIdentifier();
    Token readString();
    Token readNumber();
    Token readComment();
    
    // 语法分析和代码生成
    std::string parseAndGenerate(const std::vector<Token>& tokens);
    
    // 解析器方法
    void parseElement();
    void parseAttributes();
    void parseStyle();
    void parseScript();
    void parseText();
    void parseTemplate();
    void parseCustom();
    void parseOrigin();
    
    // 工具方法
    bool isKeyword(const std::string& str);
    bool expectToken(TokenType type);
    bool matchToken(TokenType type);
    void consumeToken(TokenType type);
    std::string escapeHtml(const std::string& str);
    
    // 样式处理
    struct StyleRule {
        std::string selector;
        std::string properties;
        bool is_inline;
    };
    
    void processLocalStyle(const std::string& style_content);
    std::string generateUniqueClassName();
    
private:
    bool debug_mode_;
    
    // 词法分析状态
    std::string source_code_;
    size_t current_pos_;
    size_t current_line_;
    size_t current_column_;
    
    // 语法分析状态
    std::vector<Token> tokens_;
    size_t token_index_;
    Token current_token_;
    
    // 生成状态
    std::string output_;
    std::string global_styles_;
    std::string global_scripts_;
    std::stack<std::string> element_stack_;
    
    // 样式管理
    std::vector<StyleRule> style_rules_;
    std::set<std::string> generated_classes_;
    int class_counter_;
    
    // 当前元素的类名
    std::string current_element_class_;
    
    // 模板存储
    std::unordered_map<std::string, std::string> style_templates_;
    std::unordered_map<std::string, std::string> element_templates_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> var_templates_;
};

} // namespace chtl

#endif // CHTL_CHTL_COMPILER_H