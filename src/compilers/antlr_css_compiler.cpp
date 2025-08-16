#include "compilers/antlr_css_compiler.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>

using namespace chtl;
using namespace antlr4;

// CSSASTVisitor实现
CSSASTVisitor::CSSASTVisitor() {
}

void CSSASTVisitor::enterStylesheet(CSS3Parser::StylesheetContext *ctx) {
    // 处理样式表
}

void CSSASTVisitor::enterRuleset(CSS3Parser::RulesetContext *ctx) {
    // 处理规则集
}

void CSSASTVisitor::enterSelector(CSS3Parser::SelectorContext *ctx) {
    if (ctx) {
        std::string selector = ctx->getText();
        if (!selector.empty()) {
            selectors_.push_back(selector);
        }
    }
}

void CSSASTVisitor::enterDeclaration(CSS3Parser::DeclarationContext *ctx) {
    if (ctx && ctx->property() && ctx->expr()) {
        std::string property = ctx->property()->getText();
        std::string value = ctx->expr()->getText();
        
        if (!property.empty()) {
            properties_.push_back(property);
        }
        
        // 检查是否为颜色值
        if (isColor(value)) {
            colors_.push_back(value);
        }
        
        // 检查是否为字体属性
        if (isFont(property)) {
            fonts_.push_back(value);
        }
    }
}

bool CSSASTVisitor::isColor(const std::string& value) {
    // 简单的颜色检测
    if (value.empty()) return false;
    
    // 十六进制颜色
    if (value[0] == '#') {
        return value.length() == 4 || value.length() == 7;
    }
    
    // RGB/RGBA
    if (value.find("rgb") == 0) {
        return true;
    }
    
    // 命名颜色（简化版）
    static const std::vector<std::string> namedColors = {
        "red", "green", "blue", "black", "white", "yellow", "orange", "purple", "pink", "gray"
    };
    
    return std::find(namedColors.begin(), namedColors.end(), value) != namedColors.end();
}

bool CSSASTVisitor::isFont(const std::string& property) {
    return property == "font-family" || property == "font";
}

// ANTLRCSSCompiler实现
ANTLRCSSCompiler::ANTLRCSSCompiler() {
}

std::vector<CodeSegmentType> ANTLRCSSCompiler::getSupportedTypes() const {
    return {
        CodeSegmentType::CSS_GLOBAL,
        CodeSegmentType::CSS_LOCAL_STYLE,
        CodeSegmentType::CSS_ORIGIN
    };
}

CompileResult ANTLRCSSCompiler::compile(const CodeSegment& segment, const CompileConfig& config) {
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

CSSCompileResult ANTLRCSSCompiler::compileCSS(const std::string& cssCode, const CSSCompileOptions& options) {
    CSSCompileResult result;
    result.originalSize = cssCode.length();
    
    try {
        std::vector<std::string> parseErrors;
        auto tree = parseCSS(cssCode, parseErrors);
        
        if (!tree || !parseErrors.empty()) {
            result.success = false;
            result.errors = parseErrors;
            return result;
        }
        
        // 提取信息
        extractInfo(tree.get(), result, options);
        
        // 生成CSS
        result.css = generateCSS(tree.get(), options);
        result.compiledSize = result.css.length();
        result.success = true;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("CSS compilation error: ") + e.what());
    }
    
    return result;
}

std::unique_ptr<tree::ParseTree> ANTLRCSSCompiler::parseCSS(const std::string& cssCode, 
                                                             std::vector<std::string>& errors) {
    try {
        ANTLRInputStream input(cssCode);
        CSS3Lexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        CSS3Parser parser(&tokens);
        
        // 禁用错误输出到控制台
        parser.removeErrorListeners();
        lexer.removeErrorListeners();
        
        // 解析CSS
        return std::unique_ptr<tree::ParseTree>(parser.stylesheet());
        
    } catch (const std::exception& e) {
        errors.push_back(std::string("Parse error: ") + e.what());
        return nullptr;
    }
}

std::string ANTLRCSSCompiler::generateCSS(tree::ParseTree* tree, const CSSCompileOptions& options) {
    if (!tree) return "";
    
    // 简单地返回原始文本（可以在这里实现更复杂的格式化）
    std::string css = tree->getText();
    
    if (options.minify) {
        // 简单的压缩：移除多余空白
        css = CSSUtils::removeWhitespace(css);
    }
    
    return css;
}

void ANTLRCSSCompiler::extractInfo(tree::ParseTree* tree, CSSCompileResult& result, 
                                   const CSSCompileOptions& options) {
    if (!tree) return;
    
    CSSASTVisitor visitor;
    tree::ParseTreeWalker walker;
    walker.walk(&visitor, tree);
    
    // 获取提取的信息
    result.selectors = visitor.getSelectors();
    result.properties = visitor.getProperties();
    result.colors = visitor.getColors();
    result.fonts = visitor.getFonts();
}

bool ANTLRCSSCompiler::validateSyntax(const std::string& cssCode, std::vector<std::string>& errors) {
    auto tree = parseCSS(cssCode, errors);
    return tree != nullptr && errors.empty();
}

std::vector<std::string> ANTLRCSSCompiler::extractSelectors(const std::string& cssCode) {
    CSSCompileOptions options;
    auto result = compileCSS(cssCode, options);
    return result.selectors;
}

std::vector<std::string> ANTLRCSSCompiler::extractProperties(const std::string& cssCode) {
    CSSCompileOptions options;
    auto result = compileCSS(cssCode, options);
    return result.properties;
}

std::vector<std::string> ANTLRCSSCompiler::extractColors(const std::string& cssCode) {
    CSSCompileOptions options;
    options.extractColors = true;
    auto result = compileCSS(cssCode, options);
    return result.colors;
}

std::string ANTLRCSSCompiler::minifyCSS(const std::string& cssCode) {
    CSSCompileOptions options;
    options.minify = true;
    auto result = compileCSS(cssCode, options);
    return result.success ? result.css : cssCode;
}

std::string ANTLRCSSCompiler::formatCSS(const std::string& cssCode, const std::string& indent) {
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