#ifndef CHTL_COMPILER_COMMON_H
#define CHTL_COMPILER_COMMON_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <optional>
#include <variant>

namespace chtl {

// 代码片段类型
enum class CodeFragmentType {
    CHTL,       // CHTL标记语言片段
    CHTL_JS,    // CHTL JS片段（局部script块）
    CSS,        // CSS片段（全局style块）
    JAVASCRIPT, // 纯JavaScript片段
    HTML_RAW    // 原始HTML片段
};

// 代码片段结构
struct CodeFragment {
    CodeFragmentType type;
    std::string content;
    size_t start_line;
    size_t end_line;
    std::string source_file;
    
    // 额外的上下文信息
    std::unordered_map<std::string, std::string> metadata;
};

// 编译结果结构
struct CompilationResult {
    bool success;
    std::string output;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    
    // 用于合并的额外信息
    std::string global_styles;      // 全局样式
    std::string global_scripts;     // 全局脚本
    std::string body_content;       // 主体内容
};

// 编译选项
struct CompilerOptions {
    bool debug_mode = false;
    bool minimize_output = false;
    bool generate_source_map = false;
    std::string output_format = "html"; // html, xml等
    std::unordered_map<std::string, std::string> custom_options;
};

// 位置信息
struct SourceLocation {
    size_t line;
    size_t column;
    std::string file;
};

// 错误信息
struct CompilerError {
    enum Severity {
        ERROR,
        WARNING,
        INFO
    };
    
    Severity severity;
    std::string message;
    SourceLocation location;
    std::string suggestion; // 修复建议
};

} // namespace chtl

#endif // CHTL_COMPILER_COMMON_H