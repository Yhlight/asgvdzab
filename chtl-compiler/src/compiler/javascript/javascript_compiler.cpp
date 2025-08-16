#include "compiler/javascript/javascript_compiler.h"
#include "common/utils.h"
#include <sstream>
#include <regex>

namespace chtl {
namespace compiler {

JavaScriptCompiler::JavaScriptCompiler() {
}

JavaScriptCompiler::~JavaScriptCompiler() = default;

CompilationResult JavaScriptCompiler::compile(const CodeSegment& segment,
                                            const CompilerOptions& options) {
    CompilationResult result;
    
    try {
        std::string js = segment.content;
        
        // 验证JavaScript
        js = validateJavaScript(js);
        
        // 处理JavaScript
        js = processJavaScript(js);
        
        // 如果启用了压缩
        if (options.minify) {
            js = minifyJavaScript(js);
        }
        
        result.output = js;
        result.success = true;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back("JavaScript compilation error: " + std::string(e.what()));
    }
    
    return result;
}

void JavaScriptCompiler::reset() {
    // JavaScript编译器无状态，无需重置
}

std::string JavaScriptCompiler::processJavaScript(const std::string& js) {
    // 这里可以添加JavaScript处理逻辑
    // 例如：ES6转ES5、添加polyfills等
    
    // 暂时直接返回原始JavaScript
    return js;
}

std::string JavaScriptCompiler::minifyJavaScript(const std::string& js) {
    std::string minified = js;
    
    // 移除单行注释（简化处理，不考虑字符串中的//）
    std::regex singleLineComment(R"(//[^\n]*)");
    minified = std::regex_replace(minified, singleLineComment, "");
    
    // 移除多行注释
    std::regex multiLineComment(R"(/\*[^*]*\*+(?:[^/*][^*]*\*+)*/)");
    minified = std::regex_replace(minified, multiLineComment, "");
    
    // 移除多余的空白
    std::regex whitespaceRegex(R"(\s+)");
    minified = std::regex_replace(minified, whitespaceRegex, " ");
    
    // 移除不必要的空格
    std::regex unnecessarySpaces(R"(\s*([{}();,:])\s*)");
    minified = std::regex_replace(minified, unnecessarySpaces, "$1");
    
    // 去除首尾空白
    minified = utils::trim(minified);
    
    return minified;
}

std::string JavaScriptCompiler::validateJavaScript(const std::string& js) {
    // 简单的JavaScript验证
    // 检查花括号、圆括号和方括号是否匹配
    int braceCount = 0;
    int parenCount = 0;
    int bracketCount = 0;
    bool inString = false;
    char stringChar = '\0';
    
    for (size_t i = 0; i < js.length(); ++i) {
        char c = js[i];
        
        // 处理字符串
        if (!inString && (c == '"' || c == '\'' || c == '`')) {
            inString = true;
            stringChar = c;
        } else if (inString && c == stringChar && (i == 0 || js[i-1] != '\\')) {
            inString = false;
            stringChar = '\0';
        } else if (!inString) {
            // 计数括号
            switch (c) {
                case '{': braceCount++; break;
                case '}': braceCount--; break;
                case '(': parenCount++; break;
                case ')': parenCount--; break;
                case '[': bracketCount++; break;
                case ']': bracketCount--; break;
            }
            
            if (braceCount < 0 || parenCount < 0 || bracketCount < 0) {
                throw std::runtime_error("Unmatched closing bracket in JavaScript");
            }
        }
    }
    
    if (braceCount != 0) {
        throw std::runtime_error("Unmatched braces {} in JavaScript");
    }
    if (parenCount != 0) {
        throw std::runtime_error("Unmatched parentheses () in JavaScript");
    }
    if (bracketCount != 0) {
        throw std::runtime_error("Unmatched brackets [] in JavaScript");
    }
    
    return js;
}

} // namespace compiler
} // namespace chtl