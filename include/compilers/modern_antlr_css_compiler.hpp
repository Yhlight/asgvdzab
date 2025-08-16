#pragma once

#include "common/types.hpp"
#include "dispatcher/compiler_dispatcher.hpp"
#include <string>
#include <vector>
#include <memory>

// 现代ANTLR4 API
#include "antlr4-runtime.h"
#include "CSS3Lexer.h"
#include "CSS3Parser.h"
#include "CSS3BaseListener.h"

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
    bool minify = false;
    bool validateSyntax = true;
    bool extractSelectors = true;
    bool extractProperties = true;
    bool extractColors = false;
    bool extractFonts = false;
    bool validateStandardCompliance = true;
    std::string outputFormat = "css";
};

/**
 * CSS编译结果
 */
struct CSSCompileResult {
    bool success = false;
    std::string css;
    std::string json;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    std::vector<CSSSelector> selectors;
    std::vector<CSSRule> rules;
    std::vector<std::string> colors;
    std::vector<std::string> fonts;
    size_t originalSize = 0;
    size_t compiledSize = 0;
};

/**
 * 现代ANTLR4 CSS监听器
 * 使用BaseListener而不是deprecated的Visitor模式
 */
class ModernCSSListener : public CSS3BaseListener {
public:
    ModernCSSListener();
    
    // 使用现代ANTLR4监听器模式
    void enterStylesheet(CSS3Parser::StylesheetContext *ctx) override;
    void exitStylesheet(CSS3Parser::StylesheetContext *ctx) override;
    
    void enterRuleset(CSS3Parser::RulesetContext *ctx) override;
    void exitRuleset(CSS3Parser::RulesetContext *ctx) override;
    
    void enterSelector(CSS3Parser::SelectorContext *ctx) override;
    void enterDeclaration(CSS3Parser::DeclarationContext *ctx) override;
    
    // 获取提取的信息
    const std::vector<CSSSelector>& getSelectors() const { return selectors_; }
    const std::vector<CSSRule>& getRules() const { return rules_; }
    const std::vector<std::string>& getColors() const { return colors_; }
    const std::vector<std::string>& getFonts() const { return fonts_; }
    
    // 错误处理
    const std::vector<std::string>& getErrors() const { return errors_; }
    
private:
    std::vector<CSSSelector> selectors_;
    std::vector<CSSRule> rules_;
    std::vector<std::string> colors_;
    std::vector<std::string> fonts_;
    std::vector<std::string> errors_;
    CSSRule currentRule_;
    
    // 辅助方法
    CSSSelector parseStandardSelector(const std::string& selectorText);
    StandardCSSSelector detectSelectorType(const std::string& selector);
    bool isColor(const std::string& value);
    bool isFont(const std::string& property);
};

/**
 * 现代ANTLR错误监听器
 */
class ModernCSSErrorListener : public antlr4::BaseErrorListener {
public:
    void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol,
                     size_t line, size_t charPositionInLine, const std::string &msg,
                     std::exception_ptr e) override;
    
    const std::vector<std::string>& getErrors() const { return errors_; }
    bool hasErrors() const { return !errors_.empty(); }
    
private:
    std::vector<std::string> errors_;
};

/**
 * 现代ANTLR CSS编译器
 * 使用最新的ANTLR4 API和最佳实践
 */
class ModernANTLRCSSCompiler : public ICompiler {
public:
    ModernANTLRCSSCompiler();
    virtual ~ModernANTLRCSSCompiler() = default;

    // ICompiler接口实现
    std::string getName() const override { return "ModernANTLRCSSCompiler (ANTLR 4.x)"; }
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
    // 现代ANTLR4解析
    std::unique_ptr<antlr4::tree::ParseTree> parseCSS(const std::string& cssCode, 
                                                       std::vector<std::string>& errors);
    
    // 信息提取（使用监听器模式）
    void extractInfo(antlr4::tree::ParseTree* tree, CSSCompileResult& result, 
                     const CSSCompileOptions& options);
    
    // 代码生成
    std::string generateCSS(const std::string& originalCSS, const CSSCompileOptions& options);
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