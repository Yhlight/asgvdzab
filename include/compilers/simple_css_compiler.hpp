#pragma once

#include "common/types.hpp"
#include "dispatcher/compiler_dispatcher.hpp"
#include <string>
#include <vector>
#include <memory>
#include <regex>

namespace chtl {

/**
 * CSS编译选项
 */
struct CSSCompileOptions {
    bool minify = false;           // 是否压缩CSS
    bool validateSyntax = true;    // 是否验证语法
    bool extractColors = false;    // 是否提取颜色信息
    bool extractFonts = false;     // 是否提取字体信息
    std::string outputFormat = "css"; // 输出格式：css, json
};

/**
 * CSS编译结果
 */
struct CSSCompileResult {
    bool success = false;
    std::string css;                    // 编译后的CSS
    std::string json;                   // JSON格式的结构化信息
    std::vector<std::string> errors;    // 错误信息
    std::vector<std::string> warnings;  // 警告信息
    std::vector<std::string> selectors; // 提取的选择器
    std::vector<std::string> properties; // 提取的属性
    std::vector<std::string> colors;    // 提取的颜色
    std::vector<std::string> fonts;     // 提取的字体
    size_t originalSize = 0;            // 原始大小
    size_t compiledSize = 0;            // 编译后大小
};

/**
 * CSS规则结构
 */
struct CSSRule {
    std::vector<std::string> selectors;
    std::vector<std::pair<std::string, std::string>> properties;
};

/**
 * 简化的CSS编译器
 */
class SimpleCSSCompiler : public ICompiler {
public:
    SimpleCSSCompiler();
    virtual ~SimpleCSSCompiler() = default;

    // ICompiler接口实现
    std::string getName() const override { return "SimpleCSSCompiler"; }
    std::vector<CodeSegmentType> getSupportedTypes() const override;
    CompileResult compile(const CodeSegment& segment, const CompileConfig& config) override;

    // CSS特定的编译方法
    CSSCompileResult compileCSS(const std::string& cssCode, const CSSCompileOptions& options = {});
    
    // 验证CSS语法
    bool validateSyntax(const std::string& cssCode, std::vector<std::string>& errors);
    
    // CSS工具方法
    std::vector<std::string> extractSelectors(const std::string& cssCode);
    std::vector<std::string> extractProperties(const std::string& cssCode);
    std::vector<std::string> extractColors(const std::string& cssCode);
    std::string minifyCSS(const std::string& cssCode);
    std::string formatCSS(const std::string& cssCode, const std::string& indent = "  ");

private:
    // 解析CSS规则
    std::vector<CSSRule> parseCSS(const std::string& cssCode);
    
    // 解析单个规则
    CSSRule parseRule(const std::string& ruleText);
    
    // 解析选择器
    std::vector<std::string> parseSelectors(const std::string& selectorText);
    
    // 解析属性
    std::vector<std::pair<std::string, std::string>> parseProperties(const std::string& propertyText);
    
    // 生成CSS
    std::string generateCSS(const std::vector<CSSRule>& rules, const CSSCompileOptions& options);
};

/**
 * CSS工具类
 */
class CSSUtils {
public:
    // 颜色相关
    static bool isValidColor(const std::string& color);
    static std::string normalizeColor(const std::string& color);
    static std::vector<std::string> extractColorsFromValue(const std::string& value);
    
    // 选择器相关
    static std::string normalizeSelector(const std::string& selector);
    static int calculateSelectorSpecificity(const std::string& selector);
    
    // 属性相关
    static bool isValidProperty(const std::string& property);
    static std::string normalizeProperty(const std::string& property);
    
    // 格式化相关
    static std::string removeComments(const std::string& css);
    static std::string removeWhitespace(const std::string& css);
    static std::string addIndentation(const std::string& css, const std::string& indent);
};

} // namespace chtl