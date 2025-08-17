#include "compiler/javascript/js_compiler.h"
#include <sstream>
#include <algorithm>
#include <regex>

namespace chtl {

JavaScriptCompiler::JavaScriptCompiler() = default;
JavaScriptCompiler::~JavaScriptCompiler() = default;

CompilationResult JavaScriptCompiler::compile(const CodeFragment& fragment, 
                                             const CompilerOptions& options) {
    CompilationResult result;
    result.success = true;
    
    try {
        // 处理JavaScript
        std::string processed = processJavaScript(fragment.content, options);
        
        // 最小化JavaScript（如果启用）
        if (options.minimize_output) {
            processed = minifyJavaScript(processed);
        }
        
        result.output = processed;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("JavaScript compilation failed: ") + e.what());
    }
    
    return result;
}

std::string JavaScriptCompiler::processJavaScript(const std::string& input, 
                                                  const CompilerOptions& options) {
    std::string result = input;
    
    // 基本的JavaScript验证和处理
    // 这里只做最基本的处理，实际的JS编译需要更复杂的解析器
    
    // 移除单行注释（但保留URL中的//）
    result = std::regex_replace(result, 
        std::regex("(?<!:)//.*$", std::regex_constants::multiline), "");
    
    // 移除多行注释
    result = std::regex_replace(result, 
        std::regex("/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/"), "");
    
    if (!options.minimize_output) {
        // 格式化代码（简单版本）
        // 在分号后添加换行
        result = std::regex_replace(result, std::regex(";\\s*"), ";\n");
        
        // 在大括号后添加换行
        result = std::regex_replace(result, std::regex("\\{\\s*"), "{\n");
        result = std::regex_replace(result, std::regex("\\}\\s*"), "}\n");
    }
    
    return result;
}

std::string JavaScriptCompiler::minifyJavaScript(const std::string& js) {
    std::string result = js;
    
    // 简单的JavaScript最小化
    // 注意：这是一个非常基础的实现，真正的JS最小化需要完整的解析器
    
    // 移除多余的空白（保留字符串中的空白）
    std::stringstream ss(result);
    std::stringstream minified;
    
    bool in_string = false;
    bool in_single_quote = false;
    bool escaped = false;
    char prev_char = 0;
    
    char c;
    while (ss.get(c)) {
        if (!escaped) {
            if (c == '"' && !in_single_quote) {
                in_string = !in_string;
            } else if (c == '\'' && !in_string) {
                in_single_quote = !in_single_quote;
            } else if (c == '\\') {
                escaped = true;
            }
        } else {
            escaped = false;
        }
        
        // 在字符串内，保留所有字符
        if (in_string || in_single_quote) {
            minified << c;
        } else {
            // 在字符串外，压缩空白
            if (std::isspace(c)) {
                // 只在必要时保留一个空格
                if (prev_char != 0 && !std::isspace(prev_char) && 
                    prev_char != ';' && prev_char != '{' && prev_char != '}' &&
                    prev_char != '(' && prev_char != ')' && prev_char != '[' && 
                    prev_char != ']' && prev_char != ',' && prev_char != ':') {
                    
                    // 检查下一个字符
                    char next = ss.peek();
                    if (next != EOF && !std::isspace(next) &&
                        next != ';' && next != '{' && next != '}' &&
                        next != '(' && next != ')' && next != '[' && 
                        next != ']' && next != ',' && next != ':') {
                        minified << ' ';
                    }
                }
            } else {
                minified << c;
                prev_char = c;
            }
        }
    }
    
    result = minified.str();
    
    // 移除最后的分号（如果有）
    if (!result.empty() && result.back() == ';') {
        result.pop_back();
    }
    
    return result;
}

} // namespace chtl