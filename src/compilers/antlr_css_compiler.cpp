#include "compilers/antlr_css_compiler.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <antlr4-runtime.h>

using namespace antlr4;
using namespace chtl;

// CSSASTVisitor实现

CSSASTVisitor::CSSASTVisitor(const CSSCompileOptions& options) 
    : options_(options) {
    result_.success = true;
}

antlrcpp::Any CSSASTVisitor::visitStylesheet(CSS3Parser::StylesheetContext *ctx) {
    if (!ctx) return nullptr;
    
    std::ostringstream css;
    
    for (auto statement : ctx->statement()) {
        if (statement->ruleset()) {
            auto rulesetResult = visitRuleset(statement->ruleset());
            if (!rulesetResult.is<std::string>()) continue;
            
            std::string rule = rulesetResult.as<std::string>();
            if (!rule.empty()) {
                css << rule;
                if (!options_.minify) css << "\n\n";
            }
        }
        // 处理其他类型的语句...
    }
    
    result_.css = css.str();
    result_.originalSize = result_.css.length();
    
    if (options_.minify) {
        result_.css = minifyCSS(result_.css);
    }
    
    result_.compiledSize = result_.css.length();
    return result_.css;
}

antlrcpp::Any CSSASTVisitor::visitRuleset(CSS3Parser::RulesetContext *ctx) {
    if (!ctx) return std::string("");
    
    std::ostringstream rule;
    
    // 处理选择器
    std::vector<std::string> selectorList;
    for (size_t i = 0; i < ctx->selector().size(); ++i) {
        std::string selector = formatSelector(ctx->selector(i));
        if (!selector.empty()) {
            selectorList.push_back(selector);
            result_.selectors.push_back(selector);
        }
    }
    
    if (selectorList.empty()) {
        return std::string("");
    }
    
    // 输出选择器
    for (size_t i = 0; i < selectorList.size(); ++i) {
        if (i > 0) rule << ", ";
        rule << selectorList[i];
    }
    
    rule << " {";
    if (!options_.minify) rule << "\n";
    
    // 处理声明
    if (ctx->block() && ctx->block()->declaration().size() > 0) {
        for (auto decl : ctx->block()->declaration()) {
            std::string declaration = formatDeclaration(decl);
            if (!declaration.empty()) {
                if (!options_.minify) rule << "  ";
                rule << declaration;
                if (!options_.minify) rule << "\n";
            }
        }
    }
    
    rule << "}";
    
    return rule.str();
}

antlrcpp::Any CSSASTVisitor::visitSelector(CSS3Parser::SelectorContext *ctx) {
    return formatSelector(ctx);
}

antlrcpp::Any CSSASTVisitor::visitDeclaration(CSS3Parser::DeclarationContext *ctx) {
    return formatDeclaration(ctx);
}

antlrcpp::Any CSSASTVisitor::visitExpr(CSS3Parser::ExprContext *ctx) {
    if (!ctx) return std::string("");
    
    std::ostringstream expr;
    std::string text = ctx->getText();
    
    // 提取颜色和字体信息
    if (options_.extractColors) {
        extractColors(text);
    }
    
    return text;
}

void CSSASTVisitor::extractColors(const std::string& value) {
    // 提取十六进制颜色
    std::regex hexColor(R"(#[0-9a-fA-F]{3,6})");
    std::sregex_iterator begin(value.begin(), value.end(), hexColor);
    std::sregex_iterator end;
    
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::string color = i->str();
        if (std::find(result_.colors.begin(), result_.colors.end(), color) == result_.colors.end()) {
            result_.colors.push_back(color);
        }
    }
    
    // 提取RGB/RGBA颜色
    std::regex rgbColor(R"(rgba?\([^)]+\))");
    begin = std::sregex_iterator(value.begin(), value.end(), rgbColor);
    
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::string color = i->str();
        if (std::find(result_.colors.begin(), result_.colors.end(), color) == result_.colors.end()) {
            result_.colors.push_back(color);
        }
    }
}

void CSSASTVisitor::extractFonts(const std::string& property, const std::string& value) {
    if (property == "font-family" || property == "font") {
        result_.fonts.push_back(value);
    }
}

std::string CSSASTVisitor::minifyCSS(const std::string& css) {
    std::string minified = css;
    
    // 简单的压缩：移除多余空白
    minified = std::regex_replace(minified, std::regex(R"(\s+)"), " ");
    minified = std::regex_replace(minified, std::regex(R"(\s*{\s*)"), "{");
    minified = std::regex_replace(minified, std::regex(R"(\s*}\s*)"), "}");
    minified = std::regex_replace(minified, std::regex(R"(\s*;\s*)"), ";");
    minified = std::regex_replace(minified, std::regex(R"(\s*:\s*)"), ":");
    minified = std::regex_replace(minified, std::regex(R"(\s*,\s*)"), ",");
    
    return minified;
}

std::string CSSASTVisitor::formatSelector(CSS3Parser::SelectorContext *ctx) {
    if (!ctx) return "";
    return ctx->getText();
}

std::string CSSASTVisitor::formatDeclaration(CSS3Parser::DeclarationContext *ctx) {
    if (!ctx) return "";
    
    std::ostringstream decl;
    
    if (ctx->property()) {
        std::string property = ctx->property()->getText();
        result_.properties.push_back(property);
        decl << property;
    }
    
    decl << ": ";
    
    if (ctx->expr()) {
        std::string value = ctx->expr()->getText();
        
        // 提取信息
        if (options_.extractColors) {
            extractColors(value);
        }
        if (options_.extractFonts && ctx->property()) {
            extractFonts(ctx->property()->getText(), value);
        }
        
        decl << value;
    }
    
    decl << ";";
    
    return decl.str();
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
    
    try {
        // 创建输入流
        ANTLRInputStream input(cssCode);
        
        // 创建词法分析器
        CSS3Lexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        
        // 创建解析器
        CSS3Parser parser(&tokens);
        
        // 禁用默认错误输出
        parser.removeErrorListeners();
        
        // 解析
        CSS3Parser::StylesheetContext* tree = parser.stylesheet();
        
        if (tree) {
            // 使用访问器遍历AST
            CSSASTVisitor visitor(options);
            visitor.visitStylesheet(tree);
            result = visitor.getResult();
        } else {
            result.success = false;
            result.errors.push_back("Failed to parse CSS: Invalid syntax");
        }
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("CSS compilation error: ") + e.what());
    }
    
    return result;
}

bool ANTLRCSSCompiler::validateSyntax(const std::string& cssCode, std::vector<std::string>& errors) {
    try {
        ANTLRInputStream input(cssCode);
        CSS3Lexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        CSS3Parser parser(&tokens);
        
        // 禁用默认错误输出
        parser.removeErrorListeners();
        
        CSS3Parser::StylesheetContext* tree = parser.stylesheet();
        return tree != nullptr;
        
    } catch (const std::exception& e) {
        errors.push_back(std::string("Syntax validation error: ") + e.what());
        return false;
    }
}

std::vector<std::string> ANTLRCSSCompiler::extractSelectors(const std::string& cssCode) {
    CSSCompileOptions options;
    options.extractColors = false;
    options.extractFonts = false;
    
    auto result = compileCSS(cssCode, options);
    return result.selectors;
}

std::vector<std::string> ANTLRCSSCompiler::extractProperties(const std::string& cssCode) {
    CSSCompileOptions options;
    options.extractColors = false;
    options.extractFonts = false;
    
    auto result = compileCSS(cssCode, options);
    return result.properties;
}

std::vector<std::string> ANTLRCSSCompiler::extractColors(const std::string& cssCode) {
    CSSCompileOptions options;
    options.extractColors = true;
    options.extractFonts = false;
    
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

std::unique_ptr<CSS3Parser> ANTLRCSSCompiler::createParser(const std::string& input, std::vector<std::string>& errors) {
    // 这个方法可以在未来扩展以提供更复杂的解析器配置
    return nullptr;
}

void ANTLRCSSCompiler::handleParseErrors(antlr4::Parser* parser, std::vector<std::string>& errors) {
    // 这个方法可以在未来扩展以提供更好的错误处理
}

// CSSUtils实现

bool CSSUtils::isValidColor(const std::string& color) {
    // 简单的颜色验证
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
    
    // 简单的特异性计算
    // ID选择器 = 100
    std::regex idSelector(R"(#[a-zA-Z][\w-]*)");
    std::sregex_iterator idIter(selector.begin(), selector.end(), idSelector);
    specificity += std::distance(idIter, std::sregex_iterator()) * 100;
    
    // 类选择器和属性选择器 = 10
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
    // 简单的属性验证
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
    // 简单的缩进添加
    std::string result = css;
    result = std::regex_replace(result, std::regex(R"(})"), "}\n");
    result = std::regex_replace(result, std::regex(R"({)"), "{\n" + indent);
    return result;
}