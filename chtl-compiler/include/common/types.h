#ifndef CHTL_COMMON_TYPES_H
#define CHTL_COMMON_TYPES_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

// 代码片段类型
enum class CodeFragmentType {
    CHTL,       // CHTL代码片段
    CHTL_JS,    // CHTL JS代码片段
    CSS,        // CSS代码片段
    JAVASCRIPT, // 纯JavaScript代码片段
    HTML_TEXT   // HTML文本内容
};

// 代码片段结构
struct CodeFragment {
    CodeFragmentType type;
    std::string content;
    std::string context;      // 上下文信息（如所属元素）
    size_t line_start;        // 起始行号
    size_t line_end;          // 结束行号
    std::string source_file;  // 源文件路径
    
    CodeFragment() : line_start(0), line_end(0) {}
    
    CodeFragment(CodeFragmentType t, const std::string& c, 
                 size_t ls = 0, size_t le = 0)
        : type(t), content(c), line_start(ls), line_end(le) {}
};

// 编译结果结构
struct CompileResult {
    bool success;
    std::string output;     // 编译后的代码
    std::string error_msg;  // 错误信息
    std::vector<std::string> warnings;  // 警告信息
    
    CompileResult() : success(true) {}
};

// 编译选项
struct CompileOptions {
    bool debug_mode = false;
    bool minify = false;
    bool generate_source_map = false;
    std::string output_dir = "./output";
    std::unordered_map<std::string, std::string> custom_options;
};

// 位置信息
struct SourceLocation {
    size_t line;
    size_t column;
    std::string file;
    
    SourceLocation() : line(0), column(0) {}
    SourceLocation(size_t l, size_t c, const std::string& f = "")
        : line(l), column(c), file(f) {}
};

} // namespace chtl

#endif // CHTL_COMMON_TYPES_H