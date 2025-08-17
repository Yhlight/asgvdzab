#include "compiler/CSSCompiler.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <regex>
#include <unordered_set>

namespace chtl {

// 私有实现类
class CSSCompiler::Impl {
public:
    std::unordered_map<std::string, std::string> options;
    CompileStats stats;
    std::vector<std::string> errors;
    
    // CSS属性验证规则
    std::unordered_set<std::string> validProperties;
    std::unordered_set<std::string> validUnits;
    
    Impl() {
        stats = CompileStats{};
        
        // 初始化有效的CSS属性
        validProperties = {
            "color", "background", "background-color", "background-image", "background-repeat",
            "background-position", "background-size", "width", "height", "min-width", "min-height",
            "max-width", "max-height", "margin", "margin-top", "margin-right", "margin-bottom", "margin-left",
            "padding", "padding-top", "padding-right", "padding-bottom", "padding-left",
            "border", "border-width", "border-style", "border-color", "border-radius",
            "font", "font-family", "font-size", "font-weight", "font-style", "line-height",
            "display", "position", "top", "left", "right", "bottom", "z-index",
            "opacity", "transform", "transition", "animation", "cursor", "overflow",
            "text-align", "text-decoration", "text-transform", "vertical-align",
            "flex", "flex-direction", "flex-wrap", "justify-content", "align-items",
            "grid", "grid-template-columns", "grid-template-rows", "grid-gap"
        };
        
        // 初始化有效的CSS单位
        validUnits = {
            "px", "em", "rem", "%", "vw", "vh", "deg", "rad", "s", "ms"
        };
    }
    
    ~Impl() = default;
};

CSSCompiler::CSSCompiler() : pImpl(std::make_unique<Impl>()) {}

CSSCompiler::~CSSCompiler() = default;

CompilationResult CSSCompiler::compile(const CodeFragment& fragment) {
    CompilationResult result;
    
    try {
        // 验证CSS语法
        if (!validateCSS(fragment.content)) {
            result.success = false;
            result.errors = pImpl->errors;
            return result;
        }
        
        // 解析CSS
        std::string parsed = parseCSS(fragment.content);
        
        // 生成CSS
        std::string generated = generateCSS(parsed);
        
        result.success = true;
        result.output = generated;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back("编译异常: " + std::string(e.what()));
    }
    
    return result;
}

void CSSCompiler::setOptions(const std::unordered_map<std::string, std::string>& options) {
    pImpl->options = options;
}

CSSCompiler::CompileStats CSSCompiler::getCompileStats() const {
    return pImpl->stats;
}

std::vector<std::string> CSSCompiler::getErrors() const {
    return pImpl->errors;
}

void CSSCompiler::clearErrors() {
    pImpl->errors.clear();
}

bool CSSCompiler::validateCSS(const std::string& css) {
    pImpl->errors.clear();
    
    // 基本的CSS语法验证
    std::istringstream stream(css);
    std::string line;
    size_t lineNumber = 0;
    
    while (std::getline(stream, line)) {
        lineNumber++;
        
        // 跳过空行和注释
        if (line.empty() || line.find("/*") == 0 || line.find("//") == 0) {
            continue;
        }
        
        // 检查选择器
        if (line.find('{') != std::string::npos) {
            size_t bracePos = line.find('{');
            std::string selector = line.substr(0, bracePos);
            
            if (!validateSelector(selector)) {
                addError("第" + std::to_string(lineNumber) + "行: 无效的选择器: " + selector);
            }
        }
        
        // 检查属性
        if (line.find(':') != std::string::npos && line.find(';') != std::string::npos) {
            size_t colonPos = line.find(':');
            size_t semicolonPos = line.find(';');
            
            if (colonPos < semicolonPos) {
                std::string property = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 1, semicolonPos - colonPos - 1);
                
                if (!validateProperty(property)) {
                    addError("第" + std::to_string(lineNumber) + "行: 无效的属性: " + property);
                }
                
                if (!validateValue(value)) {
                    addError("第" + std::to_string(lineNumber) + "行: 无效的值: " + value);
                }
            }
        }
    }
    
    return pImpl->errors.empty();
}

std::string CSSCompiler::minifyCSS(const std::string& css) {
    std::string minified = css;
    
    // 移除注释
    std::regex commentRegex(R"(/\*[\s\S]*?\*/)");
    minified = std::regex_replace(minified, commentRegex, "");
    
    // 移除多余的空白字符
    std::regex whitespaceRegex(R"(\s+)");
    minified = std::regex_replace(minified, whitespaceRegex, " ");
    
    // 移除行首行尾空白
    minified = std::regex_replace(minified, std::regex(R"(^\s+|\s+$)"), "");
    
    // 移除选择器后的空白
    minified = std::regex_replace(minified, std::regex(R"(\s*\{\s*)"), "{");
    
    // 移除属性后的空白
    minified = std::regex_replace(minified, std::regex(R"(\s*:\s*)"), ":");
    
    // 移除分号后的空白
    minified = std::regex_replace(minified, std::regex(R"(\s*;\s*)"), ";");
    
    // 移除结束大括号前的空白
    minified = std::regex_replace(minified, std::regex(R"(\s*\})"), "}");
    
    return minified;
}

std::string CSSCompiler::formatCSS(const std::string& css) {
    std::string formatted = css;
    std::string result;
    
    // 基本的格式化
    std::istringstream stream(formatted);
    std::string line;
    int indentLevel = 0;
    
    while (std::getline(stream, line)) {
        // 跳过空行和注释
        if (line.empty() || line.find("/*") == 0 || line.find("//") == 0) {
            continue;
        }
        
        // 减少缩进级别
        if (line.find('}') != std::string::npos) {
            indentLevel = std::max(0, indentLevel - 1);
        }
        
        // 添加缩进
        if (!line.empty()) {
            result += std::string(indentLevel * 4, ' ') + line + "\n";
        }
        
        // 增加缩进级别
        if (line.find('{') != std::string::npos) {
            indentLevel++;
        }
    }
    
    return result;
}

std::string CSSCompiler::parseCSS(const std::string& source) {
    // 简化实现，直接返回源内容
    // 在实际实现中，这里应该使用ANTLR4进行解析
    return source;
}

std::string CSSCompiler::generateCSS(const std::string& parsed) {
    // 处理各种CSS结构
    std::string result = parsed;
    
    result = processSelectors(result);
    result = processProperties(result);
    result = processMediaQueries(result);
    result = processKeyframes(result);
    result = processImports(result);
    
    return result;
}

std::string CSSCompiler::processSelectors(const std::string& css) {
    // 处理选择器
    std::string result = css;
    
    // 统计选择器数量
    std::regex selectorRegex(R"([^{}]+{)");
    auto begin = std::sregex_iterator(result.begin(), result.end(), selectorRegex);
    auto end = std::sregex_iterator();
    
    pImpl->stats.selectors = std::distance(begin, end);
    
    return result;
}

std::string CSSCompiler::processProperties(const std::string& css) {
    // 处理属性
    std::string result = css;
    
    // 统计属性数量
    std::regex propertyRegex(R"([a-zA-Z-]+\s*:\s*[^;]+;)");
    auto begin = std::sregex_iterator(result.begin(), result.end(), propertyRegex);
    auto end = std::sregex_iterator();
    
    pImpl->stats.properties = std::distance(begin, end);
    
    return result;
}

std::string CSSCompiler::processMediaQueries(const std::string& css) {
    // 处理媒体查询
    std::string result = css;
    
    // 统计媒体查询数量
    std::regex mediaRegex(R"(@media[^{]+{)");
    auto begin = std::sregex_iterator(result.begin(), result.end(), mediaRegex);
    auto end = std::sregex_iterator();
    
    pImpl->stats.mediaQueries = std::distance(begin, end);
    
    return result;
}

std::string CSSCompiler::processKeyframes(const std::string& css) {
    // 处理关键帧动画
    std::string result = css;
    
    // 统计关键帧数量
    std::regex keyframeRegex(R"(@keyframes[^{]+{)");
    auto begin = std::sregex_iterator(result.begin(), result.end(), keyframeRegex);
    auto end = std::sregex_iterator();
    
    pImpl->stats.keyframes = std::distance(begin, end);
    
    return result;
}

std::string CSSCompiler::processImports(const std::string& css) {
    // 处理导入
    std::string result = css;
    
    // 统计导入数量
    std::regex importRegex(R"(@import[^;]+;)");
    auto begin = std::sregex_iterator(result.begin(), result.end(), importRegex);
    auto end = std::sregex_iterator();
    
    pImpl->stats.imports = std::distance(begin, end);
    
    return result;
}

bool CSSCompiler::validateSelector(const std::string& selector) {
    // 基本的选择器验证
    std::string trimmed = selector;
    
    // 移除空白字符
    trimmed.erase(std::remove_if(trimmed.begin(), trimmed.end(), ::isspace), trimmed.end());
    
    if (trimmed.empty()) {
        return false;
    }
    
    // 检查基本的选择器模式
    std::regex validSelectorPattern(R"(^[.#]?[a-zA-Z][a-zA-Z0-9_-]*(\.[a-zA-Z][a-zA-Z0-9_-]*)*(#[a-zA-Z][a-zA-Z0-9_-]*)?(\s*[>+\s~]\s*[.#]?[a-zA-Z][a-zA-Z0-9_-]*(\.[a-zA-Z][a-zA-Z0-9_-]*)*(#[a-zA-Z][a-zA-Z0-9_-]*)?)*$)");
    
    return std::regex_match(trimmed, validSelectorPattern);
}

bool CSSCompiler::validateProperty(const std::string& property) {
    // 验证CSS属性
    std::string trimmed = property;
    
    // 移除空白字符
    trimmed.erase(std::remove_if(trimmed.begin(), trimmed.end(), ::isspace), trimmed.end());
    
    if (trimmed.empty()) {
        return false;
    }
    
    // 检查是否是有效的CSS属性
    return pImpl->validProperties.find(trimmed) != pImpl->validProperties.end();
}

bool CSSCompiler::validateValue(const std::string& value) {
    // 验证CSS值
    std::string trimmed = value;
    
    // 移除空白字符
    trimmed.erase(std::remove_if(trimmed.begin(), trimmed.end(), ::isspace), trimmed.end());
    
    if (trimmed.empty()) {
        return false;
    }
    
    // 基本的CSS值验证
    // 这里可以添加更复杂的验证逻辑
    
    return true;
}

void CSSCompiler::addError(const std::string& error) {
    pImpl->errors.push_back(error);
}

} // namespace chtl