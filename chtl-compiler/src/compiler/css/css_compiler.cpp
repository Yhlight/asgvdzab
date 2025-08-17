#include "compiler/css/css_compiler.h"
#include <sstream>
#include <algorithm>
#include <regex>

namespace chtl {

// CSSCompiler 实现
CSSCompiler::CSSCompiler() = default;
CSSCompiler::~CSSCompiler() = default;

CompilationResult CSSCompiler::compile(const CodeFragment& fragment, 
                                      const CompilerOptions& options) {
    CompilationResult result;
    result.success = true;
    
    try {
        // 处理CSS
        std::string processed = processCSS(fragment.content, options);
        
        // 优化CSS（如果启用）
        if (options.minimize_output) {
            OptimizationOptions opts;
            opts.minify = true;
            processed = optimizeCSS(processed, opts);
        }
        
        result.output = processed;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("CSS compilation failed: ") + e.what());
    }
    
    return result;
}

std::string CSSCompiler::processCSS(const std::string& input, const CompilerOptions& options) {
    // 简单的CSS处理：规范化格式
    std::string result = input;
    
    // 移除注释
    result = std::regex_replace(result, std::regex("/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/"), "");
    
    // 规范化空白（除非要最小化）
    if (!options.minimize_output) {
        // 在选择器后添加空格
        result = std::regex_replace(result, std::regex("\\{"), " {\n");
        
        // 在属性后添加空格
        result = std::regex_replace(result, std::regex(":\\s*"), ": ");
        
        // 在声明后添加换行
        result = std::regex_replace(result, std::regex(";\\s*"), ";\n");
        
        // 格式化闭合大括号
        result = std::regex_replace(result, std::regex("\\}"), "}\n");
        
        // 缩进属性
        std::stringstream ss(result);
        std::stringstream formatted;
        std::string line;
        int indent_level = 0;
        
        while (std::getline(ss, line)) {
            // 去除前后空白
            line.erase(0, line.find_first_not_of(" \t\n\r"));
            line.erase(line.find_last_not_of(" \t\n\r") + 1);
            
            if (line.empty()) continue;
            
            // 调整缩进
            if (line.find('}') != std::string::npos) {
                indent_level--;
            }
            
            // 添加缩进
            for (int i = 0; i < indent_level * 2; ++i) {
                formatted << " ";
            }
            formatted << line << "\n";
            
            if (line.find('{') != std::string::npos) {
                indent_level++;
            }
        }
        
        result = formatted.str();
    }
    
    return result;
}

std::string CSSCompiler::optimizeCSS(const std::string& css, const OptimizationOptions& opts) {
    std::string result = css;
    
    if (opts.minify) {
        // 移除所有不必要的空白
        result = std::regex_replace(result, std::regex("\\s+"), " ");
        result = std::regex_replace(result, std::regex("\\s*:\\s*"), ":");
        result = std::regex_replace(result, std::regex("\\s*;\\s*"), ";");
        result = std::regex_replace(result, std::regex("\\s*\\{\\s*"), "{");
        result = std::regex_replace(result, std::regex("\\s*\\}\\s*"), "}");
        result = std::regex_replace(result, std::regex("\\s*,\\s*"), ",");
        result = std::regex_replace(result, std::regex(";\\}"), "}");
        
        // 移除最后一个声明的分号
        result = std::regex_replace(result, std::regex(";\\s*\\}"), "}");
        
        // 移除开头和结尾的空白
        result.erase(0, result.find_first_not_of(" \t\n\r"));
        result.erase(result.find_last_not_of(" \t\n\r") + 1);
    }
    
    return result;
}

} // namespace chtl