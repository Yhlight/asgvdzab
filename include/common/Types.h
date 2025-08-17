#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <variant>
#include <optional>

namespace CHTL {

// 代码片段类型枚举
enum class FragmentType {
    CHTL,           // CHTL片段
    CHTL_JS,        // CHTL JS片段  
    CSS,            // CSS片段
    JAVASCRIPT      // JS片段
};

// 编译器类型枚举
enum class CompilerType {
    CHTL_COMPILER,      // CHTL编译器(手写)
    CHTL_JS_COMPILER,   // CHTL JS编译器(手写)
    CSS_COMPILER,       // CSS编译器(ANTLR)
    JS_COMPILER         // JavaScript编译器(ANTLR)
};

// 代码片段结构
struct CodeFragment {
    FragmentType type;
    std::string content;
    size_t start_line;
    size_t end_line;
    size_t start_column;
    size_t end_column;
    
    CodeFragment(FragmentType t, const std::string& c, 
                size_t sl = 0, size_t el = 0, size_t sc = 0, size_t ec = 0)
        : type(t), content(c), start_line(sl), end_line(el), 
          start_column(sc), end_column(ec) {}
};

// 编译结果结构
struct CompileResult {
    bool success;
    std::string output;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    
    CompileResult() : success(false) {}
    CompileResult(bool s, const std::string& o) : success(s), output(o) {}
};

// 模板类型枚举
enum class TemplateType {
    STYLE,      // @Style
    ELEMENT,    // @Element  
    VAR         // @Var
};

// 自定义类型枚举
enum class CustomType {
    STYLE,      // @Style
    ELEMENT,    // @Element
    VAR         // @Var
};

// 原始嵌入类型枚举
enum class OriginType {
    HTML,           // @Html
    STYLE,          // @Style
    JAVASCRIPT,     // @JavaScript
    CUSTOM          // 自定义类型
};

// 配置项结构
struct Configuration {
    int index_initial_count = 0;
    bool disable_name_group = false;
    bool disable_custom_origin_type = false;
    bool debug_mode = false;
    
    // 自定义关键字映射
    std::map<std::string, std::vector<std::string>> name_mappings;
    std::map<std::string, std::string> origin_types;
};

// 前向声明
class Token;
class ASTNode;

// 智能指针类型别名
using TokenPtr = std::shared_ptr<Token>;
using ASTNodePtr = std::shared_ptr<ASTNode>;
using FragmentPtr = std::shared_ptr<CodeFragment>;
using CompileResultPtr = std::shared_ptr<CompileResult>;

} // namespace CHTL