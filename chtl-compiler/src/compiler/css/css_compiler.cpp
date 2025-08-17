#include "compiler/css/css_compiler.h"
#include <sstream>
#include <regex>

namespace chtl {

// 简单的CSS处理实现（暂时不使用ANTLR）
class CSSCompiler::Impl {
public:
    std::string compile(const std::string& input) {
        std::stringstream output;
        output << "/* Processed by CHTL CSS Compiler */\n";
        
        // 简单的CSS格式化
        std::string formatted = formatCSS(input);
        
        // 处理CHTL特性
        formatted = processCHTLFeatures(formatted);
        
        output << formatted;
        return output.str();
    }
    
private:
    std::string formatCSS(const std::string& css) {
        std::string result = css;
        
        // 规范化空白
        result = std::regex_replace(result, std::regex(R"(\s+)"), " ");
        
        // 在大括号周围添加换行
        result = std::regex_replace(result, std::regex(R"(\s*\{\s*)"), " {\n  ");
        result = std::regex_replace(result, std::regex(R"(\s*\}\s*)"), "\n}\n");
        
        // 在分号后添加换行
        result = std::regex_replace(result, std::regex(R"(;\s*)"), ";\n  ");
        
        return result;
    }
    
    std::string processCHTLFeatures(const std::string& css) {
        std::string result = css;
        
        // 处理@Style模板使用
        std::regex style_template_regex(R"(@Style\s+(\w+)\s*;)");
        result = std::regex_replace(result, style_template_regex, "/* @Style $1 - TODO: expand template */");
        
        // 处理变量使用
        std::regex var_usage_regex(R"((\w+)\((\w+)\))");
        result = std::regex_replace(result, var_usage_regex, "var(--$2)");
        
        return result;
    }
};

CSSCompiler::CSSCompiler() : impl_(std::make_unique<Impl>()) {}

CSSCompiler::~CSSCompiler() = default;

CompilationResult CSSCompiler::compile(const CodeFragment& fragment,
                                      const CompilerOptions& options) {
    CompilationResult result;
    result.success = false;

    try {
        // 处理CHTL特性
        std::string processedCSS = handleCHTLFeatures(fragment.content);
        
        // 使用ANTLR解析CSS
        result.output = impl_->compile(processedCSS);
        result.success = true;
    }
    catch (const std::exception& e) {
        result.errors.push_back(e.what());
    }

    return result;
}

std::string CSSCompiler::processCSS(const std::string& css) {
    // 基本的CSS处理逻辑
    return css;
}

std::string CSSCompiler::handleCHTLFeatures(const std::string& css) {
    std::string result = css;

    // 处理CHTL注释 (-- 注释)
    std::regex chtl_comment_regex(R"(--[^\n]*\n)");
    result = std::regex_replace(result, chtl_comment_regex, "/* $& */\n");

    // 处理其他CHTL特性...
    
    return result;
}

} // namespace chtl