#include "compilers/css_compiler.hpp"
#include <sstream>
#include <algorithm>
#include <regex>
#include <cctype>

namespace chtl {

CSSCompiler::CSSCompiler() 
    : parserInitialized_(false), debugMode_(false) {
    
    // 初始化支持的CSS特性
    supportedCSSFeatures_ = {
        "flexbox", "grid", "css-variables", "css-nesting", 
        "css-custom-properties", "css-logical-properties",
        "css-contain", "css-subgrid", "css-container-queries"
    };
}

CompileResult CSSCompiler::compile(const CodeSegment& segment, const CompileConfig& config) {
    CompileResult result;
    result.success = false;
    result.compilerName = getName();
    result.compilerVersion = getVersion();
    
    // 检查是否支持该类型
    if (!supportsType(segment.type)) {
        result.errors.push_back("CSS编译器不支持代码片段类型: " + toString(segment.type));
        return result;
    }
    
    try {
        // 验证CSS语法
        if (!validateCSSSyntax(segment.content)) {
            result.errors.insert(result.errors.end(), errors_.begin(), errors_.end());
            return result;
        }
        
        // 处理CSS变量
        std::string processedCSS = processCSSVariables(segment.content);
        
        // 处理CSS嵌套
        processedCSS = processCSSNesting(processedCSS);
        
        // 优化CSS代码
        processedCSS = optimizeCSS(processedCSS, config);
        
        // 根据配置进行压缩或格式化
        if (config.minify) {
            processedCSS = minifyCSS(processedCSS);
        } else if (config.format) {
            processedCSS = formatCSS(processedCSS);
        }
        
        // 添加CSS前缀（如果需要）
        if (config.addPrefixes) {
            processedCSS = addCSSPrefixes(processedCSS);
        }
        
        // 检查兼容性
        auto compatibilityIssues = checkCSSCompatibility(processedCSS);
        result.warnings.insert(result.warnings.end(), compatibilityIssues.begin(), compatibilityIssues.end());
        
        // 生成统计信息
        auto stats = generateCSSStatistics(processedCSS);
        result.metadata["css_rules"] = std::to_string(stats["rules"]);
        result.metadata["css_selectors"] = std::to_string(stats["selectors"]);
        result.metadata["css_properties"] = std::to_string(stats["properties"]);
        result.metadata["css_size"] = std::to_string(processedCSS.length());
        
        // 设置输出类型
        result.metadata["type"] = "css";
        result.output = processedCSS;
        result.success = true;
        
    } catch (const std::exception& e) {
        result.errors.push_back("CSS编译过程中发生异常: " + std::string(e.what()));
    }
    
    return result;
}

std::string CSSCompiler::getName() const {
    return "CHTL CSS Compiler";
}

std::string CSSCompiler::getVersion() const {
    return "1.0.0";
}

bool CSSCompiler::supportsType(CodeSegmentType type) const {
    return type == CodeSegmentType::CSS_GLOBAL || 
           type == CodeSegmentType::CSS_LOCAL_STYLE;
}

std::vector<CodeSegmentType> CSSCompiler::getSupportedTypes() const {
    return {CodeSegmentType::CSS_GLOBAL, CodeSegmentType::CSS_LOCAL_STYLE};
}

std::vector<std::string> CSSCompiler::parseCSSRules(const std::string& cssCode) {
    std::vector<std::string> rules;
    std::istringstream stream(cssCode);
    std::string line;
    std::string currentRule;
    int braceCount = 0;
    
    while (std::getline(stream, line)) {
        currentRule += line + "\n";
        
        // 计算大括号数量
        for (char c : line) {
            if (c == '{') braceCount++;
            if (c == '}') braceCount--;
        }
        
        // 当大括号平衡时，一个规则完成
        if (braceCount == 0 && !currentRule.empty()) {
            rules.push_back(currentRule);
            currentRule.clear();
        }
    }
    
    return rules;
}

bool CSSCompiler::validateCSSSyntax(const std::string& cssCode) {
    errors_.clear();
    warnings_.clear();
    
    // 基本语法检查
    int braceCount = 0;
    int parenCount = 0;
    bool inString = false;
    char stringChar = 0;
    
    for (size_t i = 0; i < cssCode.length(); i++) {
        char c = cssCode[i];
        char prev = (i > 0) ? cssCode[i-1] : 0;
        
        // 处理字符串
        if ((c == '"' || c == '\'') && prev != '\\') {
            if (!inString) {
                inString = true;
                stringChar = c;
            } else if (c == stringChar) {
                inString = false;
                stringChar = 0;
            }
        }
        
        if (inString) continue;
        
        // 检查大括号平衡
        if (c == '{') braceCount++;
        if (c == '}') braceCount--;
        
        // 检查小括号平衡
        if (c == '(') parenCount++;
        if (c == ')') parenCount--;
        
        // 检查大括号是否小于0
        if (braceCount < 0) {
            errors_.push_back("CSS语法错误：意外的大括号 '}' 在第 " + std::to_string(i + 1) + " 个字符");
            return false;
        }
        
        // 检查小括号是否小于0
        if (parenCount < 0) {
            errors_.push_back("CSS语法错误：意外的小括号 ')' 在第 " + std::to_string(i + 1) + " 个字符");
            return false;
        }
    }
    
    // 检查大括号是否平衡
    if (braceCount != 0) {
        errors_.push_back("CSS语法错误：大括号不平衡，缺少 " + std::to_string(braceCount) + " 个 '}'");
        return false;
    }
    
    // 检查小括号是否平衡
    if (parenCount != 0) {
        errors_.push_back("CSS语法错误：小括号不平衡，缺少 " + std::to_string(parenCount) + " 个 ')'");
        return false;
    }
    
    // 检查选择器语法
    std::regex selectorPattern(R"([a-zA-Z_][a-zA-Z0-9_-]*(\.[a-zA-Z_][a-zA-Z0-9_-]*)*(#[a-zA-Z_][a-zA-Z0-9_-]*)*(\[[^\]]*\])*(\:[a-zA-Z_][a-zA-Z0-9_-]*)*(\:\:[a-zA-Z_][a-zA-Z0-9_-]*)*(\*)?)");
    
    auto rules = parseCSSRules(cssCode);
    for (const auto& rule : rules) {
        size_t bracePos = rule.find('{');
        if (bracePos != std::string::npos) {
            std::string selector = rule.substr(0, bracePos);
            // 简单的选择器验证
            if (!selector.empty() && selector.find('{') == std::string::npos) {
                // 选择器看起来有效
            }
        }
    }
    
    return errors_.empty();
}

std::string CSSCompiler::optimizeCSS(const std::string& cssCode, const CompileConfig& config) {
    std::string optimized = cssCode;
    
    // 移除注释（如果配置要求）
    if (config.removeComments) {
        std::regex commentPattern(R"(\/\*[^*]*\*+(?:[^/*][^*]*\*+)*\/)");
        optimized = std::regex_replace(optimized, commentPattern, "");
    }
    
    // 合并重复的选择器
    // 这里可以实现更复杂的优化逻辑
    
    // 移除不必要的空白
    if (config.minify) {
        std::regex whitespacePattern(R"(\s+)");
        optimized = std::regex_replace(optimized, whitespacePattern, " ");
    }
    
    return optimized;
}

std::string CSSCompiler::processCSSVariables(const std::string& cssCode) {
    std::string processed = cssCode;
    
    // 处理CSS自定义属性
    std::regex varPattern(R"(var\(--([^)]+)\))");
    processed = std::regex_replace(processed, varPattern, "var(--$1)");
    
    // 这里可以添加更多的变量处理逻辑
    
    return processed;
}

std::string CSSCompiler::processCSSNesting(const std::string& cssCode) {
    std::string processed = cssCode;
    
    // 简单的嵌套处理（将嵌套选择器展开）
    // 这里可以实现更复杂的嵌套处理逻辑
    
    return processed;
}

std::string CSSCompiler::generateCSSSelector(const std::string& elementName, 
                                           const std::unordered_map<std::string, std::string>& attributes) {
    std::string selector = elementName;
    
    // 添加类选择器
    auto classIt = attributes.find("class");
    if (classIt != attributes.end()) {
        std::istringstream classStream(classIt->second);
        std::string className;
        while (std::getline(classStream, className, ' ')) {
            if (!className.empty()) {
                selector += "." + className;
            }
        }
    }
    
    // 添加ID选择器
    auto idIt = attributes.find("id");
    if (idIt != attributes.end()) {
        selector += "#" + idIt->second;
    }
    
    return selector;
}

std::string CSSCompiler::minifyCSS(const std::string& cssCode) {
    std::string minified = cssCode;
    
    // 移除注释
    std::regex commentPattern(R"(\/\*[^*]*\*+(?:[^/*][^*]*\*+)*\/)");
    minified = std::regex_replace(minified, commentPattern, "");
    
    // 移除不必要的空白
    std::regex whitespacePattern(R"(\s+)");
    minified = std::regex_replace(minified, whitespacePattern, " ");
    
    // 移除行尾空白
    std::regex trailingWhitespacePattern(R"(\s+$)");
    minified = std::regex_replace(minified, trailingWhitespacePattern, "");
    
    // 移除选择器和属性之间的空白
    std::regex selectorWhitespacePattern(R"(\s*\{\s*)");
    minified = std::regex_replace(minified, selectorWhitespacePattern, "{");
    
    // 移除属性值周围的空白
    std::regex propertyWhitespacePattern(R"(\s*:\s*)");
    minified = std::regex_replace(minified, propertyWhitespacePattern, ":");
    
    // 移除分号后的空白
    std::regex semicolonWhitespacePattern(R"(\s*;\s*)");
    minified = std::regex_replace(minified, semicolonWhitespacePattern, ";");
    
    // 移除大括号前的空白
    std::regex braceWhitespacePattern(R"(\s*\}\s*)");
    minified = std::regex_replace(minified, braceWhitespacePattern, "}");
    
    return minified;
}

std::string CSSCompiler::formatCSS(const std::string& cssCode) {
    std::string formatted = cssCode;
    
    // 基本的格式化
    std::regex rulePattern(R"(([^{]+)\{([^}]+)\})");
    formatted = std::regex_replace(formatted, rulePattern, "$1 {\n  $2\n}");
    
    // 格式化属性
    std::regex propertyPattern(R"(([^:]+):([^;]+);)");
    formatted = std::regex_replace(formatted, propertyPattern, "  $1: $2;\n");
    
    return formatted;
}

std::string CSSCompiler::addCSSPrefixes(const std::string& cssCode) {
    std::string prefixed = cssCode;
    
    // 添加常见的前缀
    std::unordered_map<std::string, std::vector<std::string>> prefixMap = {
        {"display: flex", {"display: -webkit-flex", "display: -ms-flexbox"}},
        {"display: grid", {"display: -ms-grid"}},
        {"transform", {"-webkit-transform", "-ms-transform"}},
        {"transition", {"-webkit-transition", "-ms-transition"}},
        {"animation", {"-webkit-animation", "-ms-animation"}}
    };
    
    for (const auto& [property, prefixes] : prefixMap) {
        for (const auto& prefix : prefixes) {
            // 这里可以实现更复杂的前缀添加逻辑
        }
    }
    
    return prefixed;
}

std::vector<std::string> CSSCompiler::checkCSSCompatibility(const std::string& cssCode) {
    std::vector<std::string> issues;
    
    // 检查CSS Grid支持
    if (cssCode.find("display: grid") != std::string::npos) {
        issues.push_back("CSS Grid 在某些旧版浏览器中可能不被支持");
    }
    
    // 检查CSS Flexbox支持
    if (cssCode.find("display: flex") != std::string::npos) {
        issues.push_back("CSS Flexbox 在某些旧版浏览器中可能不被支持");
    }
    
    // 检查CSS变量支持
    if (cssCode.find("var(--") != std::string::npos) {
        issues.push_back("CSS 自定义属性在某些旧版浏览器中可能不被支持");
    }
    
    return issues;
}

std::unordered_map<std::string, size_t> CSSCompiler::generateCSSStatistics(const std::string& cssCode) {
    std::unordered_map<std::string, size_t> stats;
    
    // 统计规则数量
    auto rules = parseCSSRules(cssCode);
    stats["rules"] = rules.size();
    
    // 统计选择器数量
    std::regex selectorPattern(R"([^{]+)");
    std::sregex_iterator selectorBegin(cssCode.begin(), cssCode.end(), selectorPattern);
    std::sregex_iterator selectorEnd;
    stats["selectors"] = std::distance(selectorBegin, selectorEnd);
    
    // 统计属性数量
    std::regex propertyPattern(R"([^:]+:[^;]+;)");
    std::sregex_iterator propertyBegin(cssCode.begin(), cssCode.end(), propertyPattern);
    std::sregex_iterator propertyEnd;
    stats["properties"] = std::distance(propertyBegin, propertyEnd);
    
    // 统计文件大小
    stats["size"] = cssCode.length();
    
    return stats;
}

} // namespace chtl