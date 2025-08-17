#include "../../include/compilers/JavaScriptCompiler.h"
#include "../../include/common/Utils.h"
#include <iostream>
#include <sstream>
#include <regex>

namespace CHTL {

JavaScriptCompiler::JavaScriptCompiler() : debug_mode(false) {
}

JavaScriptCompiler::JavaScriptCompiler(const Configuration& cfg) : config(cfg), debug_mode(false) {
}

void JavaScriptCompiler::setConfiguration(const Configuration& cfg) {
    config = cfg;
}

CompileResultPtr JavaScriptCompiler::compile(const std::string& js_code) {
    auto result = std::make_shared<CompileResult>();
    
    try {
        // 验证JavaScript代码
        if (!validateJS(js_code)) {
            result->success = false;
            result->errors.push_back("JavaScript代码验证失败");
            result->errors.insert(result->errors.end(), errors.begin(), errors.end());
            return result;
        }
        
        // 规范化JavaScript代码
        std::string normalized_js = normalizeJSCode(js_code);
        
        // 优化JavaScript代码
        std::string optimized_js = optimizeJS(normalized_js);
        
        // 添加到编译结果
        addGlobalJS(optimized_js);
        
        result->success = errors.empty();
        result->output = optimized_js;
        result->errors = errors;
        result->warnings = warnings;
        
    } catch (const std::exception& e) {
        result->success = false;
        result->errors.push_back("JavaScript编译异常: " + std::string(e.what()));
    }
    
    return result;
}

bool JavaScriptCompiler::validateJS(const std::string& js_code) const {
    if (js_code.empty()) {
        return true; // 空JS是有效的
    }
    
    // 简单的JavaScript验证
    // 检查括号匹配
    int brace_count = 0;
    int paren_count = 0;
    int bracket_count = 0;
    bool in_string = false;
    char string_delimiter = '\0';
    bool in_regex = false;
    
    for (size_t i = 0; i < js_code.length(); ++i) {
        char c = js_code[i];
        char prev_c = (i > 0) ? js_code[i-1] : '\0';
        
        if (!in_string && !in_regex) {
            if (c == '"' || c == '\'' || c == '`') {
                in_string = true;
                string_delimiter = c;
            } else if (c == '/' && prev_c != '\\') {
                // 可能是正则表达式开始
                if (i + 1 < js_code.length() && js_code[i+1] != '/' && js_code[i+1] != '*') {
                    in_regex = true;
                }
            } else if (c == '{') {
                brace_count++;
            } else if (c == '}') {
                brace_count--;
                if (brace_count < 0) return false;
            } else if (c == '(') {
                paren_count++;
            } else if (c == ')') {
                paren_count--;
                if (paren_count < 0) return false;
            } else if (c == '[') {
                bracket_count++;
            } else if (c == ']') {
                bracket_count--;
                if (bracket_count < 0) return false;
            }
        } else if (in_string) {
            if (c == string_delimiter && prev_c != '\\') {
                in_string = false;
                string_delimiter = '\0';
            }
        } else if (in_regex) {
            if (c == '/' && prev_c != '\\') {
                in_regex = false;
            }
        }
    }
    
    return brace_count == 0 && paren_count == 0 && bracket_count == 0 && 
           !in_string && !in_regex;
}

std::string JavaScriptCompiler::normalizeJSCode(const std::string& js_code) const {
    std::string normalized = js_code;
    
    // 基本的代码格式化
    // 在分号后添加换行
    normalized = std::regex_replace(normalized, std::regex(R"(;\s*)"), ";\n");
    
    // 在大括号后添加换行
    normalized = std::regex_replace(normalized, std::regex(R"({\s*)"), " {\n");
    normalized = std::regex_replace(normalized, std::regex(R"(\s*})"), "\n}");
    
    return Utils::trim(normalized);
}

std::string JavaScriptCompiler::optimizeJS(const std::string& js_code) const {
    std::string optimized = js_code;
    
    // 移除单行注释
    optimized = std::regex_replace(optimized, std::regex(R"(//.*$)", std::regex_constants::multiline), "");
    
    // 移除多行注释
    optimized = std::regex_replace(optimized, std::regex(R"(/\*.*?\*/)"), "");
    
    // 移除多余的空行
    optimized = std::regex_replace(optimized, std::regex(R"(\n\s*\n)"), "\n");
    
    return Utils::trim(optimized);
}

std::string JavaScriptCompiler::minifyJS(const std::string& js_code) const {
    std::string minified = js_code;
    
    // 移除所有不必要的空白字符
    minified = std::regex_replace(minified, std::regex(R"(\s+)"), " ");
    minified = std::regex_replace(minified, std::regex(R"(\s*{\s*)"), "{");
    minified = std::regex_replace(minified, std::regex(R"(\s*}\s*)"), "}");
    minified = std::regex_replace(minified, std::regex(R"(\s*;\s*)"), ";");
    minified = std::regex_replace(minified, std::regex(R"(\s*\(\s*)"), "(");
    minified = std::regex_replace(minified, std::regex(R"(\s*\)\s*)"), ")");
    minified = std::regex_replace(minified, std::regex(R"(\s*,\s*)"), ",");
    minified = std::regex_replace(minified, std::regex(R"(\s*=\s*)"), "=");
    
    return Utils::trim(minified);
}

bool JavaScriptCompiler::checkSyntax(const std::string& js_code) const {
    // 简单的语法检查
    return validateJS(js_code);
}

std::vector<std::string> JavaScriptCompiler::findSyntaxErrors(const std::string& js_code) const {
    std::vector<std::string> syntax_errors;
    
    // 简单的语法错误检查
    if (!validateJS(js_code)) {
        syntax_errors.push_back("括号不匹配或字符串未闭合");
    }
    
    // 检查是否有未定义的变量（简化检查）
    std::regex undefined_var_regex(R"(\b(\w+)\s*\(\s*\))");
    std::sregex_iterator iter(js_code.begin(), js_code.end(), undefined_var_regex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::smatch match = *iter;
        std::string func_name = match[1].str();
        
        // 检查是否为已知的内置函数
        if (func_name != "console" && func_name != "alert" && 
            func_name != "setTimeout" && func_name != "setInterval") {
            // 这里可以添加更多的检查
        }
    }
    
    return syntax_errors;
}

void JavaScriptCompiler::addJSStatement(const std::string& statement) {
    if (!statement.empty()) {
        js_statements.push_back(statement);
        
        if (debug_mode) {
            Utils::debugLog("JS_COMPILER", "添加JS语句: " + statement);
        }
    }
}

void JavaScriptCompiler::addGlobalJS(const std::string& js) {
    if (!js.empty()) {
        if (!compiled_js.empty()) {
            compiled_js += "\n";
        }
        compiled_js += js;
        
        if (debug_mode) {
            Utils::debugLog("JS_COMPILER", "添加全局JS");
        }
    }
}

std::string JavaScriptCompiler::getCompiledJS() const {
    std::stringstream js;
    
    // 添加编译的JS
    if (!compiled_js.empty()) {
        js << compiled_js << "\n";
    }
    
    // 添加JS语句
    for (const auto& statement : js_statements) {
        js << statement << "\n";
    }
    
    return js.str();
}

std::string JavaScriptCompiler::mergeJS(const std::vector<std::string>& js_parts) const {
    std::stringstream merged;
    
    for (const auto& part : js_parts) {
        if (!part.empty()) {
            merged << part << "\n";
        }
    }
    
    return merged.str();
}

std::string JavaScriptCompiler::optimizeAndMinify(const std::string& js) const {
    std::string optimized = optimizeJS(js);
    return minifyJS(optimized);
}

bool JavaScriptCompiler::validateSyntax(const std::string& js_code) const {
    return checkSyntax(js_code);
}

std::vector<std::string> JavaScriptCompiler::getSyntaxErrors(const std::string& js_code) const {
    return findSyntaxErrors(js_code);
}

void JavaScriptCompiler::addError(const std::string& message) {
    errors.push_back("JavaScript编译器错误: " + message);
    
    if (debug_mode) {
        Utils::debugLog("JS_COMPILER_ERROR", message);
    }
}

void JavaScriptCompiler::addWarning(const std::string& message) {
    warnings.push_back("JavaScript编译器警告: " + message);
    
    if (debug_mode) {
        Utils::debugLog("JS_COMPILER_WARNING", message);
    }
}

void JavaScriptCompiler::reset() {
    compiled_js.clear();
    js_statements.clear();
    errors.clear();
    warnings.clear();
}

void JavaScriptCompiler::dumpJS() const {
    std::cout << "=== JavaScript编译器状态 ===" << std::endl;
    std::cout << "编译JS长度: " << compiled_js.length() << std::endl;
    std::cout << "JS语句数量: " << js_statements.size() << std::endl;
    std::cout << "错误数量: " << errors.size() << std::endl;
    std::cout << "警告数量: " << warnings.size() << std::endl;
    
    if (debug_mode && !compiled_js.empty()) {
        std::cout << "\n编译JS内容:\n" << compiled_js << std::endl;
    }
    
    std::cout << "============================" << std::endl;
}

} // namespace CHTL