#include "compiler/css/css_compiler.h"
#include "common/utils.h"
#include <sstream>
#include <regex>

namespace chtl {
namespace compiler {

CSSCompiler::CSSCompiler() {
}

CSSCompiler::~CSSCompiler() = default;

CompilationResult CSSCompiler::compile(const CodeSegment& segment,
                                     const CompilerOptions& options) {
    CompilationResult result;
    
    try {
        std::string css = segment.content;
        
        // 验证CSS
        css = validateCSS(css);
        
        // 处理CSS
        css = processCSS(css);
        
        // 如果启用了压缩
        if (options.minify) {
            css = minifyCSS(css);
        }
        
        result.output = css;
        result.success = true;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back("CSS compilation error: " + std::string(e.what()));
    }
    
    return result;
}

void CSSCompiler::reset() {
    // CSS编译器无状态，无需重置
}

std::string CSSCompiler::processCSS(const std::string& css) {
    // 这里可以添加CSS处理逻辑
    // 例如：添加浏览器前缀、处理变量等
    
    // 暂时直接返回原始CSS
    return css;
}

std::string CSSCompiler::minifyCSS(const std::string& css) {
    std::string minified = css;
    
    // 移除注释
    std::regex commentRegex(R"(/\*[^*]*\*+(?:[^/*][^*]*\*+)*/)");
    minified = std::regex_replace(minified, commentRegex, "");
    
    // 移除多余的空白
    std::regex whitespaceRegex(R"(\s+)");
    minified = std::regex_replace(minified, whitespaceRegex, " ");
    
    // 移除不必要的空格
    std::regex unnecessarySpaces1(R"(\s*([{}:;,])\s*)");
    minified = std::regex_replace(minified, unnecessarySpaces1, "$1");
    
    // 移除最后的分号
    std::regex lastSemicolon(R"(;})");
    minified = std::regex_replace(minified, lastSemicolon, "}");
    
    // 去除首尾空白
    minified = utils::trim(minified);
    
    return minified;
}

std::string CSSCompiler::validateCSS(const std::string& css) {
    // 简单的CSS验证
    // 检查花括号是否匹配
    int braceCount = 0;
    for (char c : css) {
        if (c == '{') braceCount++;
        else if (c == '}') braceCount--;
        if (braceCount < 0) {
            throw std::runtime_error("Unmatched closing brace in CSS");
        }
    }
    
    if (braceCount != 0) {
        throw std::runtime_error("Unmatched braces in CSS");
    }
    
    return css;
}

} // namespace compiler
} // namespace chtl