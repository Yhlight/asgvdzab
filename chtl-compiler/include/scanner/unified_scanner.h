#ifndef CHTL_UNIFIED_SCANNER_H
#define CHTL_UNIFIED_SCANNER_H

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_set>
#include "utils/common.h"

namespace chtl {

// 扫描器状态
enum class ScannerState {
    NORMAL,           // 普通CHTL代码
    IN_STYLE_BLOCK,   // 在style块中
    IN_SCRIPT_BLOCK,  // 在script块中
    IN_TEXT_BLOCK,    // 在text块中
    IN_COMMENT,       // 在注释中
    IN_STRING,        // 在字符串中
    IN_ORIGIN_BLOCK,  // 在[Origin]块中
    IN_TEMPLATE,      // 在模板定义中
    IN_CUSTOM         // 在自定义定义中
};

// 词法单元类型
enum class TokenType {
    // 基础标记
    IDENTIFIER,       // 标识符
    KEYWORD,          // 关键字
    STRING_LITERAL,   // 字符串字面量
    NUMBER,           // 数字
    
    // 操作符
    COLON,            // :
    SEMICOLON,        // ;
    EQUALS,           // =
    LBRACE,           // {
    RBRACE,           // }
    LPAREN,           // (
    RPAREN,           // )
    LBRACKET,         // [
    RBRACKET,         // ]
    COMMA,            // ,
    DOT,              // .
    ARROW,            // ->
    AMPERSAND,        // &
    
    // 特殊标记
    COMMENT,          // 注释
    WHITESPACE,       // 空白
    NEWLINE,          // 换行
    EOF_TOKEN,        // 文件结束
    
    // CHTL特定
    STYLE_BLOCK,      // style块
    SCRIPT_BLOCK,     // script块
    TEXT_BLOCK,       // text块
    ORIGIN_MARKER,    // [Origin]标记
    TEMPLATE_MARKER,  // [Template]标记
    CUSTOM_MARKER,    // [Custom]标记
    IMPORT_MARKER,    // [Import]标记
    
    // 增强选择器
    SELECTOR_START,   // {{
    SELECTOR_END      // }}
};

// 词法单元
struct Token {
    TokenType type;
    std::string value;
    SourceLocation location;
};

// 扫描器配置
struct ScannerConfig {
    bool preserve_comments = false;      // 是否保留注释
    bool preserve_whitespace = false;    // 是否保留空白
    bool enable_error_recovery = true;   // 是否启用错误恢复
};

// CHTLUnifiedScanner - 精准代码切割器
class CHTLUnifiedScanner {
public:
    explicit CHTLUnifiedScanner(const ScannerConfig& config = {});
    ~CHTLUnifiedScanner();
    
    // 扫描输入并生成代码片段
    std::vector<CodeFragment> scan(const std::string& input, const std::string& filename = "");
    
    // 获取扫描过程中的错误
    const std::vector<CompilerError>& getErrors() const { return errors_; }
    
    // 清除错误
    void clearErrors() { errors_.clear(); }
    
    // 设置错误处理回调
    void setErrorHandler(std::function<void(const CompilerError&)> handler) {
        error_handler_ = handler;
    }

private:
    // 内部扫描状态
    struct ScanContext {
        std::string input;
        size_t position;
        size_t line;
        size_t column;
        std::string filename;
        ScannerState state;
        std::vector<ScannerState> state_stack;
        
        // 当前正在构建的片段
        CodeFragmentType current_fragment_type;
        std::string current_content;
        size_t fragment_start_line;
        
        // 特殊块的嵌套深度
        int brace_depth;
        int style_depth;
        int script_depth;
        
        // 字符串处理
        char string_delimiter; // ' 或 "
        bool in_template_string;
    };
    
    // 扫描方法
    void scanCHTL(ScanContext& ctx, std::vector<CodeFragment>& fragments);
    void scanStyleBlock(ScanContext& ctx, std::vector<CodeFragment>& fragments);
    void scanScriptBlock(ScanContext& ctx, std::vector<CodeFragment>& fragments);
    void scanTextBlock(ScanContext& ctx, std::vector<CodeFragment>& fragments);
    void scanOriginBlock(ScanContext& ctx, std::vector<CodeFragment>& fragments);
    
    // 词法分析辅助方法
    Token nextToken(ScanContext& ctx);
    void skipWhitespace(ScanContext& ctx);
    void skipComment(ScanContext& ctx);
    std::string scanIdentifier(ScanContext& ctx);
    std::string scanString(ScanContext& ctx);
    
    // 片段处理
    void finishFragment(ScanContext& ctx, std::vector<CodeFragment>& fragments);
    void startNewFragment(ScanContext& ctx, CodeFragmentType type);
    
    // 工具方法
    bool isKeyword(const std::string& word) const;
    bool matchSequence(const ScanContext& ctx, const std::string& seq) const;
    char peek(const ScanContext& ctx, size_t offset = 0) const;
    void advance(ScanContext& ctx, size_t count = 1);
    void newLine(ScanContext& ctx);
    
    // 错误处理
    void reportError(const ScanContext& ctx, const std::string& message);
    void reportWarning(const ScanContext& ctx, const std::string& message);
    
private:
    ScannerConfig config_;
    std::vector<CompilerError> errors_;
    std::function<void(const CompilerError&)> error_handler_;
    
    // CHTL关键字集合
    static const std::unordered_set<std::string> keywords_;
    static const std::unordered_set<std::string> html_elements_;
};

} // namespace chtl

#endif // CHTL_UNIFIED_SCANNER_H