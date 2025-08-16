#include "compilers/simple_css_compiler.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace chtl;

SimpleCSSCompiler::SimpleCSSCompiler() {
}

std::vector<CodeSegmentType> SimpleCSSCompiler::getSupportedTypes() const {
    return {
        CodeSegmentType::CSS_GLOBAL,
        CodeSegmentType::CSS_LOCAL_STYLE,
        CodeSegmentType::CSS_ORIGIN
    };
}

CompileResult SimpleCSSCompiler::compile(const CodeSegment& segment, const CompileConfig& config) {
    CSSCompileOptions cssOptions;
    cssOptions.minify = config.optimization.minify;
    cssOptions.validateSyntax = true;
    cssOptions.extractColors = true;
    cssOptions.extractFonts = true;
    
    auto cssResult = compileCSS(segment.content, cssOptions);
    
    CompileResult result;
    result.success = cssResult.success;
    result.output = cssResult.css;
    result.errors = cssResult.errors;
    result.warnings = cssResult.warnings;
    
    return result;
}

CSSCompileResult SimpleCSSCompiler::compileCSS(const std::string& cssCode, const CSSCompileOptions& options) {
    CSSCompileResult result;
    result.originalSize = cssCode.length();
    
    try {
        // 移除注释
        std::string cleanCode = CSSUtils::removeComments(cssCode);
        
        // 解析CSS规则
        auto rules = parseCSS(cleanCode);
        
        // 提取信息
        for (const auto& rule : rules) {
            // 提取选择器
            for (const auto& selector : rule.selectors) {
                result.selectors.push_back(selector);
            }
            
            // 提取属性和颜色
            for (const auto& prop : rule.properties) {
                result.properties.push_back(prop.first);
                
                if (options.extractColors) {
                    auto colors = CSSUtils::extractColorsFromValue(prop.second);
                    result.colors.insert(result.colors.end(), colors.begin(), colors.end());
                }
                
                if (options.extractFonts && 
                    (prop.first == "font-family" || prop.first == "font")) {
                    result.fonts.push_back(prop.second);
                }
            }
        }
        
        // 生成CSS
        result.css = generateCSS(rules, options);
        result.compiledSize = result.css.length();
        result.success = true;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("CSS compilation error: ") + e.what());
    }
    
    return result;
}

std::vector<CSSRule> SimpleCSSCompiler::parseCSS(const std::string& cssCode) {
    std::vector<CSSRule> rules;
    
    // 简单的规则解析：查找 { } 对
    size_t pos = 0;
    while (pos < cssCode.length()) {
        // 查找规则开始
        size_t selectorStart = pos;
        size_t braceStart = cssCode.find('{', pos);
        
        if (braceStart == std::string::npos) {
            break;
        }
        
        // 查找规则结束
        size_t braceEnd = cssCode.find('}', braceStart);
        if (braceEnd == std::string::npos) {
            break;
        }
        
        // 提取规则文本
        std::string selectorText = cssCode.substr(selectorStart, braceStart - selectorStart);
        std::string propertyText = cssCode.substr(braceStart + 1, braceEnd - braceStart - 1);
        
        // 解析规则
        CSSRule rule;
        rule.selectors = parseSelectors(selectorText);
        rule.properties = parseProperties(propertyText);
        
        if (!rule.selectors.empty()) {
            rules.push_back(rule);
        }
        
        pos = braceEnd + 1;
    }
    
    return rules;
}

std::vector<std::string> SimpleCSSCompiler::parseSelectors(const std::string& selectorText) {
    std::vector<std::string> selectors;
    
    // 按逗号分割选择器
    std::stringstream ss(selectorText);
    std::string selector;
    
    while (std::getline(ss, selector, ',')) {
        // 去除空白
        selector.erase(0, selector.find_first_not_of(" \t\r\n"));
        selector.erase(selector.find_last_not_of(" \t\r\n") + 1);
        
        if (!selector.empty()) {
            selectors.push_back(selector);
        }
    }
    
    return selectors;
}

std::vector<std::pair<std::string, std::string>> SimpleCSSCompiler::parseProperties(const std::string& propertyText) {
    std::vector<std::pair<std::string, std::string>> properties;
    
    // 按分号分割属性
    std::stringstream ss(propertyText);
    std::string property;
    
    while (std::getline(ss, property, ';')) {
        // 去除空白
        property.erase(0, property.find_first_not_of(" \t\r\n"));
        property.erase(property.find_last_not_of(" \t\r\n") + 1);
        
        if (property.empty()) continue;
        
        // 查找冒号分割属性名和值
        size_t colonPos = property.find(':');
        if (colonPos != std::string::npos) {
            std::string name = property.substr(0, colonPos);
            std::string value = property.substr(colonPos + 1);
            
            // 去除空白
            name.erase(0, name.find_first_not_of(" \t\r\n"));
            name.erase(name.find_last_not_of(" \t\r\n") + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);
            
            if (!name.empty() && !value.empty()) {
                properties.push_back(std::make_pair(name, value));
            }
        }
    }
    
    return properties;
}

std::string SimpleCSSCompiler::generateCSS(const std::vector<CSSRule>& rules, const CSSCompileOptions& options) {
    std::ostringstream css;
    
    for (size_t i = 0; i < rules.size(); ++i) {
        const auto& rule = rules[i];
        
        // 输出选择器
        for (size_t j = 0; j < rule.selectors.size(); ++j) {
            if (j > 0) css << ", ";
            css << rule.selectors[j];
        }
        
        css << " {";
        if (!options.minify) css << "\n";
        
        // 输出属性
        for (const auto& prop : rule.properties) {
            if (!options.minify) css << "  ";
            css << prop.first << ": " << prop.second << ";";
            if (!options.minify) css << "\n";
        }
        
        css << "}";
        if (!options.minify && i < rules.size() - 1) css << "\n\n";
    }
    
    return css.str();
}

bool SimpleCSSCompiler::validateSyntax(const std::string& cssCode, std::vector<std::string>& errors) {
    try {
        parseCSS(CSSUtils::removeComments(cssCode));
        return true;
    } catch (const std::exception& e) {
        errors.push_back(std::string("Syntax validation error: ") + e.what());
        return false;
    }
}

std::vector<std::string> SimpleCSSCompiler::extractSelectors(const std::string& cssCode) {
    CSSCompileOptions options;
    auto result = compileCSS(cssCode, options);
    return result.selectors;
}

std::vector<std::string> SimpleCSSCompiler::extractProperties(const std::string& cssCode) {
    CSSCompileOptions options;
    auto result = compileCSS(cssCode, options);
    return result.properties;
}

std::vector<std::string> SimpleCSSCompiler::extractColors(const std::string& cssCode) {
    CSSCompileOptions options;
    options.extractColors = true;
    auto result = compileCSS(cssCode, options);
    return result.colors;
}

std::string SimpleCSSCompiler::minifyCSS(const std::string& cssCode) {
    CSSCompileOptions options;
    options.minify = true;
    auto result = compileCSS(cssCode, options);
    return result.success ? result.css : cssCode;
}

std::string SimpleCSSCompiler::formatCSS(const std::string& cssCode, const std::string& indent) {
    CSSCompileOptions options;
    options.minify = false;
    auto result = compileCSS(cssCode, options);
    return result.success ? result.css : cssCode;
}

// CSSUtils实现

bool CSSUtils::isValidColor(const std::string& color) {
    if (color.empty()) return false;
    
    // 十六进制颜色
    if (color[0] == '#') {
        return color.length() == 4 || color.length() == 7;
    }
    
    // RGB/RGBA
    if (color.find("rgb") == 0) {
        return true;
    }
    
    // 命名颜色
    static const std::vector<std::string> namedColors = {
        "red", "green", "blue", "black", "white", "yellow", "orange", "purple", "pink", "gray"
    };
    
    return std::find(namedColors.begin(), namedColors.end(), color) != namedColors.end();
}

std::string CSSUtils::normalizeColor(const std::string& color) {
    std::string normalized = color;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
    return normalized;
}

std::vector<std::string> CSSUtils::extractColorsFromValue(const std::string& value) {
    std::vector<std::string> colors;
    
    // 提取十六进制颜色
    std::regex hexColor(R"(#[0-9a-fA-F]{3,6})");
    std::sregex_iterator begin(value.begin(), value.end(), hexColor);
    std::sregex_iterator end;
    
    for (std::sregex_iterator i = begin; i != end; ++i) {
        colors.push_back(i->str());
    }
    
    // 提取RGB/RGBA颜色
    std::regex rgbColor(R"(rgba?\([^)]+\))");
    begin = std::sregex_iterator(value.begin(), value.end(), rgbColor);
    
    for (std::sregex_iterator i = begin; i != end; ++i) {
        colors.push_back(i->str());
    }
    
    return colors;
}

std::string CSSUtils::normalizeSelector(const std::string& selector) {
    std::string normalized = selector;
    // 移除多余空白
    normalized = std::regex_replace(normalized, std::regex(R"(\s+)"), " ");
    return normalized;
}

int CSSUtils::calculateSelectorSpecificity(const std::string& selector) {
    int specificity = 0;
    
    // ID选择器 = 100
    std::regex idSelector(R"(#[a-zA-Z][\w-]*)");
    std::sregex_iterator idIter(selector.begin(), selector.end(), idSelector);
    specificity += std::distance(idIter, std::sregex_iterator()) * 100;
    
    // 类选择器 = 10
    std::regex classSelector(R"(\.[a-zA-Z][\w-]*)");
    std::sregex_iterator classIter(selector.begin(), selector.end(), classSelector);
    specificity += std::distance(classIter, std::sregex_iterator()) * 10;
    
    // 元素选择器 = 1
    std::regex elementSelector(R"([a-zA-Z][\w-]*)");
    std::sregex_iterator elemIter(selector.begin(), selector.end(), elementSelector);
    specificity += std::distance(elemIter, std::sregex_iterator()) * 1;
    
    return specificity;
}

bool CSSUtils::isValidProperty(const std::string& property) {
    static const std::vector<std::string> validProperties = {
        "color", "background", "background-color", "font-size", "font-family",
        "margin", "padding", "border", "width", "height", "display", "position"
    };
    
    return std::find(validProperties.begin(), validProperties.end(), property) != validProperties.end();
}

std::string CSSUtils::normalizeProperty(const std::string& property) {
    std::string normalized = property;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
    return normalized;
}

std::string CSSUtils::removeComments(const std::string& css) {
    return std::regex_replace(css, std::regex(R"(/\*.*?\*/)"), "");
}

std::string CSSUtils::removeWhitespace(const std::string& css) {
    return std::regex_replace(css, std::regex(R"(\s+)"), " ");
}

std::string CSSUtils::addIndentation(const std::string& css, const std::string& indent) {
    std::string result = css;
    result = std::regex_replace(result, std::regex(R"(})"), "}\n");
    result = std::regex_replace(result, std::regex(R"({)"), "{\n" + indent);
    return result;
}