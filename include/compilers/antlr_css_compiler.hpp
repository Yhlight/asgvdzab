#pragma once

#include "common/types.hpp"
#include "dispatcher/compiler_dispatcher.hpp"
#include <string>
#include <vector>
#include <memory>

// 标准CSS解析器 - 不再依赖ANTLR
// 使用基于正则表达式的解析方式

namespace chtl {

/**
 * 标准CSS选择器类型
 */
enum class StandardCSSSelector {
    UNIVERSAL,          // * (通用选择器)
    TYPE,              // element (类型选择器)
    CLASS,             // .class (类选择器)
    ID,                // #id (ID选择器)
    ATTRIBUTE,         // [attr] (属性选择器)
    DESCENDANT,        // A B (后代选择器)
    CHILD,             // A > B (子选择器)
    ADJACENT_SIBLING,  // A + B (相邻兄弟选择器)
    GENERAL_SIBLING,   // A ~ B (通用兄弟选择器)
    PSEUDO_CLASS,      // :hover (伪类选择器)
    PSEUDO_ELEMENT     // ::before (伪元素选择器)
};

/**
 * CSS选择器信息
 */
struct CSSSelector {
    StandardCSSSelector type;
    std::string value;
    int specificity = 0;
    std::string originalText;
    
    CSSSelector(StandardCSSSelector t, const std::string& v) 
        : type(t), value(v), originalText(v) {
        calculateSpecificity();
    }
    
private:
    void calculateSpecificity();
};

/**
 * CSS规则信息
 */
struct CSSRule {
    std::vector<CSSSelector> selectors;
    std::vector<std::pair<std::string, std::string>> properties;
    std::string sourceText;
    int lineNumber = 0;
};

/**
 * CSS编译选项
 */
struct CSSCompileOptions {
    bool minify = false;                      // 是否压缩CSS
    bool validateSyntax = true;               // 是否验证语法
    bool extractSelectors = true;             // 是否提取选择器
    bool extractProperties = true;            // 是否提取属性
    bool extractColors = false;               // 是否提取颜色信息
    bool extractFonts = false;                // 是否提取字体信息
    bool validateStandardCompliance = true;   // 是否验证标准合规性
    std::string outputFormat = "css";         // 输出格式：css, json
};

/**
 * CSS编译结果
 */
struct CSSCompileResult {
    bool success = false;
    std::string css;                          // 编译后的CSS
    std::string json;                         // JSON格式的结构化信息
    std::vector<std::string> errors;          // 错误信息
    std::vector<std::string> warnings;        // 警告信息
    std::vector<CSSSelector> selectors;       // 提取的标准选择器
    std::vector<CSSRule> rules;               // 提取的CSS规则
    std::vector<std::string> colors;          // 提取的颜色
    std::vector<std::string> fonts;           // 提取的字体
    size_t originalSize = 0;                  // 原始大小
    size_t compiledSize = 0;                  // 编译后大小
};

/**
 * 标准CSS选择器解析器 - 基于正则表达式实现
 * 
 * 这个解析器专注于标准CSS选择器的识别和分析，
 * 提供完整的CSS3标准合规性验证功能。
 */

/**
 * 标准CSS选择器编译器
 * 
 * 专注于CSS3标准选择器的解析、验证和分析，
 * 提供完整的选择器类型识别和权重计算功能。
 */
class ANTLRCSSCompiler : public ICompiler {
public:
    ANTLRCSSCompiler();
    virtual ~ANTLRCSSCompiler() = default;

    // ICompiler接口实现
    std::string getName() const override { return "StandardCSSCompiler (CSS3 Selectors)"; }
    std::vector<CodeSegmentType> getSupportedTypes() const override;
    CompileResult compile(const CodeSegment& segment, const CompileConfig& config) override;

    // CSS特定的编译方法
    CSSCompileResult compileCSS(const std::string& cssCode, const CSSCompileOptions& options = {});
    
    // 验证CSS语法和标准合规性
    bool validateSyntax(const std::string& cssCode, std::vector<std::string>& errors);
    bool validateStandardCompliance(const std::vector<CSSSelector>& selectors, std::vector<std::string>& warnings);
    
    // 标准选择器相关方法
    std::vector<CSSSelector> extractStandardSelectors(const std::string& cssCode);
    std::vector<CSSRule> extractCSSRules(const std::string& cssCode);
    std::vector<std::string> extractProperties(const std::string& cssCode);
    std::vector<std::string> extractColors(const std::string& cssCode);
    
    // CSS工具方法
    std::string minifyCSS(const std::string& cssCode);
    std::string formatCSS(const std::string& cssCode, const std::string& indent = "  ");
    std::string generateSelectorDocumentation(const std::vector<CSSSelector>& selectors);

private:
    // 标准CSS解析（基于正则表达式）
    std::vector<CSSSelector> extractStandardSelectorsRegex(const std::string& cssCode);
    std::vector<CSSRule> extractCSSRulesRegex(const std::string& cssCode);
    StandardCSSSelector detectSelectorTypeRegex(const std::string& selector);
};

/**
 * 标准CSS工具类
 */
class StandardCSSUtils {
public:
    // 选择器相关
    static bool isValidStandardSelector(const std::string& selector);
    static StandardCSSSelector detectSelectorType(const std::string& selector);
    static int calculateStandardSpecificity(const CSSSelector& selector);
    static std::string normalizeSelectorSyntax(const std::string& selector);
    
    // CSS3标准验证
    static bool isValidCSS3Property(const std::string& property);
    static bool isValidCSS3Value(const std::string& property, const std::string& value);
    static std::vector<std::string> getStandardPseudoClasses();
    static std::vector<std::string> getStandardPseudoElements();
    
    // 颜色相关
    static bool isValidColor(const std::string& color);
    static std::string normalizeColor(const std::string& color);
    static std::vector<std::string> extractColorsFromValue(const std::string& value);
    
    // 格式化相关
    static std::string removeComments(const std::string& css);
    static std::string removeWhitespace(const std::string& css);
    static std::string addIndentation(const std::string& css, const std::string& indent);
    
    // 文档生成
    static std::string generateSelectorReference(const std::vector<CSSSelector>& selectors);
    static std::string generateCSS3ComplianceReport(const std::vector<CSSRule>& rules);
};

} // namespace chtl