#include "../../include/compilers/CSSCompiler.h"
#include "../../include/common/Utils.h"
#include <iostream>
#include <sstream>
#include <regex>

namespace CHTL {

CSSCompiler::CSSCompiler() : debug_mode(false) {
}

CSSCompiler::CSSCompiler(const Configuration& cfg) : config(cfg), debug_mode(false) {
}

void CSSCompiler::setConfiguration(const Configuration& cfg) {
    config = cfg;
}

CompileResultPtr CSSCompiler::compile(const std::string& css_code) {
    auto result = std::make_shared<CompileResult>();
    
    try {
        // 验证CSS代码
        if (!validateCSS(css_code)) {
            result->success = false;
            result->errors.push_back("CSS代码验证失败");
            result->errors.insert(result->errors.end(), errors.begin(), errors.end());
            return result;
        }
        
        // 规范化CSS代码
        std::string normalized_css = normalizeCSSCode(css_code);
        
        // 优化CSS代码
        std::string optimized_css = optimizeCSS(normalized_css);
        
        // 添加到全局CSS
        addGlobalCSS(optimized_css);
        
        result->success = errors.empty();
        result->output = optimized_css;
        result->errors = errors;
        result->warnings = warnings;
        
    } catch (const std::exception& e) {
        result->success = false;
        result->errors.push_back("CSS编译异常: " + std::string(e.what()));
    }
    
    return result;
}

bool CSSCompiler::validateCSS(const std::string& css_code) const {
    if (css_code.empty()) {
        return true; // 空CSS是有效的
    }
    
    // 简单的CSS验证
    // 检查括号匹配
    int brace_count = 0;
    bool in_string = false;
    char string_delimiter = '\0';
    
    for (char c : css_code) {
        if (!in_string) {
            if (c == '"' || c == '\'') {
                in_string = true;
                string_delimiter = c;
            } else if (c == '{') {
                brace_count++;
            } else if (c == '}') {
                brace_count--;
                if (brace_count < 0) {
                    return false; // 括号不匹配
                }
            }
        } else {
            if (c == string_delimiter) {
                in_string = false;
                string_delimiter = '\0';
            }
        }
    }
    
    return brace_count == 0 && !in_string;
}

std::string CSSCompiler::normalizeCSSCode(const std::string& css_code) const {
    std::string normalized = css_code;
    
    // 移除多余的空白字符
    normalized = std::regex_replace(normalized, std::regex(R"(\s+)"), " ");
    
    // 规范化选择器和属性之间的空格
    normalized = std::regex_replace(normalized, std::regex(R"(\s*{\s*)"), " {\n  ");
    normalized = std::regex_replace(normalized, std::regex(R"(\s*}\s*)"), "\n}\n");
    normalized = std::regex_replace(normalized, std::regex(R"(\s*;\s*)"), ";\n  ");
    normalized = std::regex_replace(normalized, std::regex(R"(\s*:\s*)"), ": ");
    
    return Utils::trim(normalized);
}

std::string CSSCompiler::optimizeCSS(const std::string& css_code) const {
    std::string optimized = css_code;
    
    // 移除注释
    optimized = std::regex_replace(optimized, std::regex(R"(/\*.*?\*/)"), "");
    
    // 合并相同的选择器
    // 这里简化实现，实际需要更复杂的逻辑
    
    return optimized;
}

std::string CSSCompiler::minifyCSS(const std::string& css_code) const {
    std::string minified = css_code;
    
    // 移除所有不必要的空白字符
    minified = std::regex_replace(minified, std::regex(R"(\s+)"), " ");
    minified = std::regex_replace(minified, std::regex(R"(\s*{\s*)"), "{");
    minified = std::regex_replace(minified, std::regex(R"(\s*}\s*)"), "}");
    minified = std::regex_replace(minified, std::regex(R"(\s*;\s*)"), ";");
    minified = std::regex_replace(minified, std::regex(R"(\s*:\s*)"), ":");
    minified = std::regex_replace(minified, std::regex(R"(\s*,\s*)"), ",");
    
    return Utils::trim(minified);
}

void CSSCompiler::addCSSRule(const std::string& rule) {
    if (!rule.empty()) {
        css_rules.push_back(rule);
        
        if (debug_mode) {
            Utils::debugLog("CSS_COMPILER", "添加CSS规则: " + rule);
        }
    }
}

void CSSCompiler::addGlobalCSS(const std::string& css) {
    if (!css.empty()) {
        if (!global_css.empty()) {
            global_css += "\n";
        }
        global_css += css;
        
        if (debug_mode) {
            Utils::debugLog("CSS_COMPILER", "添加全局CSS");
        }
    }
}

std::string CSSCompiler::getCompiledCSS() const {
    std::stringstream css;
    
    // 添加全局CSS
    if (!global_css.empty()) {
        css << global_css << "\n";
    }
    
    // 添加CSS规则
    for (const auto& rule : css_rules) {
        css << rule << "\n";
    }
    
    return css.str();
}

std::string CSSCompiler::mergeCSS(const std::vector<std::string>& css_parts) const {
    std::stringstream merged;
    
    for (const auto& part : css_parts) {
        if (!part.empty()) {
            merged << part << "\n";
        }
    }
    
    return merged.str();
}

std::string CSSCompiler::optimizeAndMinify(const std::string& css) const {
    std::string optimized = optimizeCSS(css);
    return minifyCSS(optimized);
}

void CSSCompiler::addError(const std::string& message) {
    errors.push_back("CSS编译器错误: " + message);
    
    if (debug_mode) {
        Utils::debugLog("CSS_COMPILER_ERROR", message);
    }
}

void CSSCompiler::addWarning(const std::string& message) {
    warnings.push_back("CSS编译器警告: " + message);
    
    if (debug_mode) {
        Utils::debugLog("CSS_COMPILER_WARNING", message);
    }
}

void CSSCompiler::reset() {
    global_css.clear();
    css_rules.clear();
    errors.clear();
    warnings.clear();
}

void CSSCompiler::dumpCSS() const {
    std::cout << "=== CSS编译器状态 ===" << std::endl;
    std::cout << "全局CSS长度: " << global_css.length() << std::endl;
    std::cout << "CSS规则数量: " << css_rules.size() << std::endl;
    std::cout << "错误数量: " << errors.size() << std::endl;
    std::cout << "警告数量: " << warnings.size() << std::endl;
    
    if (debug_mode && !global_css.empty()) {
        std::cout << "\n全局CSS内容:\n" << global_css << std::endl;
    }
    
    std::cout << "====================" << std::endl;
}

} // namespace CHTL