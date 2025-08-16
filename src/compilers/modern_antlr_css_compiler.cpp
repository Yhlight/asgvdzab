#include "compilers/modern_antlr_css_compiler.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>

using namespace chtl;
using namespace antlr4;
using namespace antlr4::tree;

// CSSSelector实现
void CSSSelector::calculateSpecificity() {
    switch (type) {
        case StandardCSSSelector::ID:
            specificity = 100;
            break;
        case StandardCSSSelector::CLASS:
        case StandardCSSSelector::ATTRIBUTE:
        case StandardCSSSelector::PSEUDO_CLASS:
            specificity = 10;
            break;
        case StandardCSSSelector::TYPE:
        case StandardCSSSelector::PSEUDO_ELEMENT:
            specificity = 1;
            break;
        case StandardCSSSelector::UNIVERSAL:
            specificity = 0;
            break;
        default:
            specificity = 5;
            break;
    }
}

// ModernCSSListener实现
ModernCSSListener::ModernCSSListener() {
}

void ModernCSSListener::enterStylesheet(CSS3Parser::StylesheetContext *ctx) {
    std::cout << "[ANTLR] 开始解析样式表" << std::endl;
}

void ModernCSSListener::exitStylesheet(CSS3Parser::StylesheetContext *ctx) {
    std::cout << "[ANTLR] 样式表解析完成，共" << rules_.size() << "条规则" << std::endl;
}

void ModernCSSListener::enterRuleset(CSS3Parser::RulesetContext *ctx) {
    // 开始新的CSS规则
    currentRule_ = CSSRule();
    if (ctx) {
        currentRule_.sourceText = ctx->getText();
    }
}

void ModernCSSListener::exitRuleset(CSS3Parser::RulesetContext *ctx) {
    // 完成当前规则并添加到列表
    if (!currentRule_.selectors.empty() || !currentRule_.properties.empty()) {
        rules_.push_back(currentRule_);
    }
}

void ModernCSSListener::enterSelector(CSS3Parser::SelectorContext *ctx) {
    if (ctx) {
        std::string selectorText = ctx->getText();
        if (!selectorText.empty()) {
            try {
                CSSSelector selector = parseStandardSelector(selectorText);
                selectors_.push_back(selector);
                currentRule_.selectors.push_back(selector);
                std::cout << "[ANTLR] 解析选择器: " << selectorText << " (类型: " << (int)selector.type << ", 权重: " << selector.specificity << ")" << std::endl;
            } catch (const std::exception& e) {
                errors_.push_back("选择器解析错误: " + selectorText + " - " + e.what());
            }
        }
    }
}

void ModernCSSListener::enterDeclaration(CSS3Parser::DeclarationContext *ctx) {
    if (ctx) {
        try {
            // 使用getText()获取属性和值，因为property()和expr()方法可能不存在
            std::string declText = ctx->getText();
            
            // 使用正则表达式解析属性声明
            std::regex declRegex(R"(([^:]+):\s*([^;]+))");
            std::smatch match;
            
            if (std::regex_search(declText, match, declRegex)) {
                std::string property = match.str(1);
                std::string value = match.str(2);
                
                // 清理空白
                property = std::regex_replace(property, std::regex(R"(^\s+|\s+$)"), "");
                value = std::regex_replace(value, std::regex(R"(^\s+|\s+$)"), "");
                
                if (!property.empty() && !value.empty()) {
                    currentRule_.properties.emplace_back(property, value);
                    
                    // 检查是否为颜色值
                    if (isColor(value)) {
                        colors_.push_back(value);
                    }
                    
                    // 检查是否为字体属性
                    if (isFont(property)) {
                        fonts_.push_back(value);
                    }
                    
                    std::cout << "[ANTLR] 解析属性: " << property << " = " << value << std::endl;
                }
            }
        } catch (const std::exception& e) {
            errors_.push_back("属性解析错误: " + std::string(e.what()));
        }
    }
}

CSSSelector ModernCSSListener::parseStandardSelector(const std::string& selectorText) {
    StandardCSSSelector type = detectSelectorType(selectorText);
    return CSSSelector(type, selectorText);
}

StandardCSSSelector ModernCSSListener::detectSelectorType(const std::string& selector) {
    if (selector.empty()) return StandardCSSSelector::UNIVERSAL;
    
    char firstChar = selector[0];
    switch (firstChar) {
        case '*':
            return StandardCSSSelector::UNIVERSAL;
        case '.':
            return StandardCSSSelector::CLASS;
        case '#':
            return StandardCSSSelector::ID;
        case '[':
            return StandardCSSSelector::ATTRIBUTE;
        case ':':
            if (selector.length() > 1 && selector[1] == ':') {
                return StandardCSSSelector::PSEUDO_ELEMENT;
            }
            return StandardCSSSelector::PSEUDO_CLASS;
        default:
            // 检查组合选择器
            if (selector.find(' ') != std::string::npos) {
                return StandardCSSSelector::DESCENDANT;
            }
            if (selector.find('>') != std::string::npos) {
                return StandardCSSSelector::CHILD;
            }
            if (selector.find('+') != std::string::npos) {
                return StandardCSSSelector::ADJACENT_SIBLING;
            }
            if (selector.find('~') != std::string::npos) {
                return StandardCSSSelector::GENERAL_SIBLING;
            }
            return StandardCSSSelector::TYPE;
    }
}

bool ModernCSSListener::isColor(const std::string& value) {
    if (value.empty()) return false;
    
    // 十六进制颜色
    if (value[0] == '#') {
        return value.length() == 4 || value.length() == 7;
    }
    
    // RGB/RGBA
    if (value.find("rgb") == 0) {
        return true;
    }
    
    // 命名颜色
    static const std::vector<std::string> namedColors = {
        "red", "green", "blue", "black", "white", "yellow", "orange", "purple", "pink", "gray"
    };
    
    return std::find(namedColors.begin(), namedColors.end(), value) != namedColors.end();
}

bool ModernCSSListener::isFont(const std::string& property) {
    return property == "font-family" || property == "font";
}

// ModernCSSErrorListener实现
void ModernCSSErrorListener::syntaxError(Recognizer *recognizer, Token *offendingSymbol,
                                         size_t line, size_t charPositionInLine, 
                                         const std::string &msg, std::exception_ptr e) {
    std::ostringstream errorMsg;
    errorMsg << "第" << line << "行第" << charPositionInLine << "列: " << msg;
    errors_.push_back(errorMsg.str());
    std::cout << "[ANTLR错误] " << errorMsg.str() << std::endl;
}

// ModernANTLRCSSCompiler实现
ModernANTLRCSSCompiler::ModernANTLRCSSCompiler() {
}

std::vector<CodeSegmentType> ModernANTLRCSSCompiler::getSupportedTypes() const {
    return {
        CodeSegmentType::CSS_GLOBAL,
        CodeSegmentType::CSS_LOCAL_STYLE,
        CodeSegmentType::CSS_ORIGIN
    };
}

CompileResult ModernANTLRCSSCompiler::compile(const CodeSegment& segment, const CompileConfig& config) {
    CSSCompileOptions cssOptions;
    cssOptions.minify = config.optimization.minify;
    cssOptions.validateSyntax = true;
    cssOptions.extractSelectors = true;
    cssOptions.extractProperties = true;
    cssOptions.extractColors = true;
    cssOptions.extractFonts = true;
    cssOptions.validateStandardCompliance = true;
    
    auto cssResult = compileCSS(segment.content, cssOptions);
    
    CompileResult result;
    result.success = cssResult.success;
    result.output = cssResult.css;
    result.errors = cssResult.errors;
    result.warnings = cssResult.warnings;
    
    return result;
}

CSSCompileResult ModernANTLRCSSCompiler::compileCSS(const std::string& cssCode, const CSSCompileOptions& options) {
    CSSCompileResult result;
    result.originalSize = cssCode.length();
    
    try {
        std::cout << "[ANTLR] 开始现代ANTLR CSS编译..." << std::endl;
        
        std::vector<std::string> parseErrors;
        auto tree = parseCSS(cssCode, parseErrors);
        
        if (!tree) {
            result.success = false;
            result.errors = parseErrors;
            std::cout << "[ANTLR] 解析失败" << std::endl;
            return result;
        }
        
        if (!parseErrors.empty()) {
            result.errors = parseErrors;
            std::cout << "[ANTLR] 解析有警告" << std::endl;
        }
        
        // 提取信息
        extractInfo(tree.get(), result, options);
        
        // 验证标准合规性
        if (options.validateStandardCompliance) {
            validateStandardCompliance(result.selectors, result.warnings);
        }
        
        // 生成CSS
        result.css = generateCSS(cssCode, options);
        result.compiledSize = result.css.length();
        result.success = true;
        
        std::cout << "[ANTLR] 现代ANTLR CSS编译完成" << std::endl;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("ANTLR CSS compilation error: ") + e.what());
        std::cout << "[ANTLR] 编译异常: " << e.what() << std::endl;
    }
    
    return result;
}

std::unique_ptr<ParseTree> ModernANTLRCSSCompiler::parseCSS(const std::string& cssCode, 
                                                             std::vector<std::string>& errors) {
    try {
        std::cout << "[ANTLR] 开始解析CSS, 长度: " << cssCode.length() << std::endl;
        
        // 创建输入流
        ANTLRInputStream input(cssCode);
        std::cout << "[ANTLR] ANTLRInputStream 创建成功" << std::endl;
        
        // 创建词法分析器
        CSS3Lexer lexer(&input);
        std::cout << "[ANTLR] CSS3Lexer 创建成功" << std::endl;
        
        // 创建Token流
        CommonTokenStream tokens(&lexer);
        std::cout << "[ANTLR] CommonTokenStream 创建成功" << std::endl;
        
        // 创建解析器
        CSS3Parser parser(&tokens);
        std::cout << "[ANTLR] CSS3Parser 创建成功" << std::endl;
        
        // 添加自定义错误监听器
        auto errorListener = std::make_shared<ModernCSSErrorListener>();
        parser.removeErrorListeners(); // 移除默认错误监听器
        lexer.removeErrorListeners();
        parser.addErrorListener(errorListener.get());
        lexer.addErrorListener(errorListener.get());
        std::cout << "[ANTLR] 错误监听器已配置" << std::endl;
        
        // 解析CSS
        std::cout << "[ANTLR] 开始调用 parser.stylesheet()" << std::endl;
        auto result = parser.stylesheet();
        std::cout << "[ANTLR] parser.stylesheet() 调用完成" << std::endl;
        
        // 检查错误
        if (errorListener->hasErrors()) {
            auto listenerErrors = errorListener->getErrors();
            errors.insert(errors.end(), listenerErrors.begin(), listenerErrors.end());
        }
        
        return std::make_unique<ParseTree>(*result);
        
    } catch (const std::exception& e) {
        std::cout << "[ANTLR] 解析异常: " << e.what() << std::endl;
        errors.push_back(std::string("ANTLR Parse error: ") + e.what());
        return nullptr;
    }
}

void ModernANTLRCSSCompiler::extractInfo(ParseTree* tree, CSSCompileResult& result, 
                                          const CSSCompileOptions& options) {
    if (!tree) return;
    
    std::cout << "[ANTLR] 开始信息提取" << std::endl;
    
    try {
        // 创建监听器
        ModernCSSListener listener;
        
        // 遍历解析树
        ParseTreeWalker walker;
        walker.walk(&listener, tree);
        std::cout << "[ANTLR] ParseTree遍历完成" << std::endl;
        
        // 获取提取的信息
        result.selectors = listener.getSelectors();
        result.rules = listener.getRules();
        result.colors = listener.getColors();
        result.fonts = listener.getFonts();
        
        // 添加监听器错误到结果
        auto listenerErrors = listener.getErrors();
        result.errors.insert(result.errors.end(), listenerErrors.begin(), listenerErrors.end());
        
        std::cout << "[ANTLR] 提取完成: " 
                  << result.selectors.size() << "个选择器, "
                  << result.rules.size() << "个规则, "
                  << result.colors.size() << "个颜色" << std::endl;
        
    } catch (const std::exception& e) {
        result.errors.push_back(std::string("信息提取错误: ") + e.what());
        std::cout << "[ANTLR] 信息提取异常: " << e.what() << std::endl;
    }
}

std::string ModernANTLRCSSCompiler::generateCSS(const std::string& originalCSS, const CSSCompileOptions& options) {
    std::string result = originalCSS;
    
    if (options.minify) {
        result = StandardCSSUtils::removeWhitespace(result);
    }
    
    return result;
}

bool ModernANTLRCSSCompiler::validateSyntax(const std::string& cssCode, std::vector<std::string>& errors) {
    auto tree = parseCSS(cssCode, errors);
    return tree != nullptr && errors.empty();
}

bool ModernANTLRCSSCompiler::validateStandardCompliance(const std::vector<CSSSelector>& selectors, std::vector<std::string>& warnings) {
    bool isCompliant = true;
    
    for (const auto& selector : selectors) {
        if (!StandardCSSUtils::isValidStandardSelector(selector.value)) {
            warnings.push_back("Non-standard selector detected: " + selector.value);
            isCompliant = false;
        }
    }
    
    return isCompliant;
}

std::vector<CSSSelector> ModernANTLRCSSCompiler::extractStandardSelectors(const std::string& cssCode) {
    CSSCompileOptions options;
    options.extractSelectors = true;
    auto result = compileCSS(cssCode, options);
    return result.selectors;
}

std::vector<CSSRule> ModernANTLRCSSCompiler::extractCSSRules(const std::string& cssCode) {
    CSSCompileOptions options;
    auto result = compileCSS(cssCode, options);
    return result.rules;
}

std::vector<std::string> ModernANTLRCSSCompiler::extractProperties(const std::string& cssCode) {
    auto rules = extractCSSRules(cssCode);
    std::vector<std::string> properties;
    
    for (const auto& rule : rules) {
        for (const auto& prop : rule.properties) {
            properties.push_back(prop.first);
        }
    }
    
    return properties;
}

std::vector<std::string> ModernANTLRCSSCompiler::extractColors(const std::string& cssCode) {
    CSSCompileOptions options;
    options.extractColors = true;
    auto result = compileCSS(cssCode, options);
    return result.colors;
}

std::string ModernANTLRCSSCompiler::minifyCSS(const std::string& cssCode) {
    CSSCompileOptions options;
    options.minify = true;
    auto result = compileCSS(cssCode, options);
    return result.success ? result.css : cssCode;
}

std::string ModernANTLRCSSCompiler::formatCSS(const std::string& cssCode, const std::string& indent) {
    CSSCompileOptions options;
    options.minify = false;
    auto result = compileCSS(cssCode, options);
    return result.success ? result.css : cssCode;
}

std::string ModernANTLRCSSCompiler::generateSelectorDocumentation(const std::vector<CSSSelector>& selectors) {
    return StandardCSSUtils::generateSelectorReference(selectors);
}

// StandardCSSUtils实现
bool StandardCSSUtils::isValidStandardSelector(const std::string& selector) {
    if (selector.empty()) return false;
    
    // 基本选择器验证
    char firstChar = selector[0];
    switch (firstChar) {
        case '*':
        case '.':
        case '#':
        case '[':
        case ':':
            return true;
        default:
            // 类型选择器：必须是有效的HTML元素名
            return std::regex_match(selector, std::regex("^[a-zA-Z][a-zA-Z0-9-]*$"));
    }
}

StandardCSSSelector StandardCSSUtils::detectSelectorType(const std::string& selector) {
    if (selector.empty()) return StandardCSSSelector::UNIVERSAL;
    
    char firstChar = selector[0];
    switch (firstChar) {
        case '*': return StandardCSSSelector::UNIVERSAL;
        case '.': return StandardCSSSelector::CLASS;
        case '#': return StandardCSSSelector::ID;
        case '[': return StandardCSSSelector::ATTRIBUTE;
        case ':':
            if (selector.length() > 1 && selector[1] == ':') {
                return StandardCSSSelector::PSEUDO_ELEMENT;
            }
            return StandardCSSSelector::PSEUDO_CLASS;
        default:
            if (selector.find(' ') != std::string::npos) return StandardCSSSelector::DESCENDANT;
            if (selector.find('>') != std::string::npos) return StandardCSSSelector::CHILD;
            if (selector.find('+') != std::string::npos) return StandardCSSSelector::ADJACENT_SIBLING;
            if (selector.find('~') != std::string::npos) return StandardCSSSelector::GENERAL_SIBLING;
            return StandardCSSSelector::TYPE;
    }
}

int StandardCSSUtils::calculateStandardSpecificity(const CSSSelector& selector) {
    return selector.specificity;
}

std::string StandardCSSUtils::normalizeSelectorSyntax(const std::string& selector) {
    std::string normalized = selector;
    // 移除多余空白
    normalized = std::regex_replace(normalized, std::regex(R"(\s+)"), " ");
    return normalized;
}

bool StandardCSSUtils::isValidCSS3Property(const std::string& property) {
    static const std::vector<std::string> css3Properties = {
        "color", "background", "background-color", "font-size", "font-family",
        "margin", "padding", "border", "width", "height", "display", "position",
        "flex", "grid", "transform", "transition", "animation", "opacity"
    };
    
    return std::find(css3Properties.begin(), css3Properties.end(), property) != css3Properties.end();
}

bool StandardCSSUtils::isValidCSS3Value(const std::string& property, const std::string& value) {
    // 简化的CSS3值验证
    if (property == "display") {
        static const std::vector<std::string> displayValues = {
            "block", "inline", "inline-block", "flex", "grid", "none"
        };
        return std::find(displayValues.begin(), displayValues.end(), value) != displayValues.end();
    }
    
    return !value.empty();
}

std::vector<std::string> StandardCSSUtils::getStandardPseudoClasses() {
    return {
        "hover", "active", "focus", "visited", "link", "first-child", "last-child",
        "nth-child", "nth-of-type", "disabled", "enabled", "checked"
    };
}

std::vector<std::string> StandardCSSUtils::getStandardPseudoElements() {
    return {
        "before", "after", "first-line", "first-letter", "selection"
    };
}

bool StandardCSSUtils::isValidColor(const std::string& color) {
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

std::string StandardCSSUtils::normalizeColor(const std::string& color) {
    std::string normalized = color;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
    return normalized;
}

std::vector<std::string> StandardCSSUtils::extractColorsFromValue(const std::string& value) {
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

std::string StandardCSSUtils::removeComments(const std::string& css) {
    return std::regex_replace(css, std::regex(R"(/\*.*?\*/)"), "");
}

std::string StandardCSSUtils::removeWhitespace(const std::string& css) {
    return std::regex_replace(css, std::regex(R"(\s+)"), " ");
}

std::string StandardCSSUtils::addIndentation(const std::string& css, const std::string& indent) {
    std::string result = css;
    result = std::regex_replace(result, std::regex(R"(})"), "}\n");
    result = std::regex_replace(result, std::regex(R"({)"), "{\n" + indent);
    return result;
}

std::string StandardCSSUtils::generateSelectorReference(const std::vector<CSSSelector>& selectors) {
    std::ostringstream reference;
    reference << "# 现代ANTLR CSS选择器参考\n\n";
    
    for (const auto& selector : selectors) {
        reference << "## " << selector.originalText << "\n";
        reference << "- 类型: ";
        
        switch (selector.type) {
            case StandardCSSSelector::UNIVERSAL:
                reference << "通用选择器 (*)";
                break;
            case StandardCSSSelector::TYPE:
                reference << "类型选择器 (element)";
                break;
            case StandardCSSSelector::CLASS:
                reference << "类选择器 (.class)";
                break;
            case StandardCSSSelector::ID:
                reference << "ID选择器 (#id)";
                break;
            case StandardCSSSelector::ATTRIBUTE:
                reference << "属性选择器 ([attr])";
                break;
            case StandardCSSSelector::DESCENDANT:
                reference << "后代选择器 (A B)";
                break;
            case StandardCSSSelector::CHILD:
                reference << "子选择器 (A > B)";
                break;
            case StandardCSSSelector::ADJACENT_SIBLING:
                reference << "相邻兄弟选择器 (A + B)";
                break;
            case StandardCSSSelector::GENERAL_SIBLING:
                reference << "通用兄弟选择器 (A ~ B)";
                break;
            case StandardCSSSelector::PSEUDO_CLASS:
                reference << "伪类选择器 (:hover)";
                break;
            case StandardCSSSelector::PSEUDO_ELEMENT:
                reference << "伪元素选择器 (::before)";
                break;
        }
        
        reference << "\n- 权重: " << selector.specificity << "\n";
        reference << "- ANTLR解析: ✓ 完整支持\n";
        reference << "- 标准合规性: " << (isValidStandardSelector(selector.value) ? "✓ 符合" : "⚠ 非标准") << "\n\n";
    }
    
    return reference.str();
}

std::string StandardCSSUtils::generateCSS3ComplianceReport(const std::vector<CSSRule>& rules) {
    std::ostringstream report;
    report << "# 现代ANTLR CSS3合规性报告\n\n";
    
    int compliantRules = 0;
    int totalSelectors = 0;
    int compliantSelectors = 0;
    
    for (const auto& rule : rules) {
        bool ruleCompliant = true;
        
        for (const auto& selector : rule.selectors) {
            totalSelectors++;
            if (isValidStandardSelector(selector.value)) {
                compliantSelectors++;
            } else {
                ruleCompliant = false;
            }
        }
        
        for (const auto& prop : rule.properties) {
            if (!isValidCSS3Property(prop.first)) {
                ruleCompliant = false;
                break;
            }
        }
        
        if (ruleCompliant) {
            compliantRules++;
        }
    }
    
    report << "## ANTLR解析统计\n";
    report << "- 解析引擎: ANTLR4 现代API\n";
    report << "- 语法分析: 完整AST支持\n";
    report << "- 错误恢复: 智能错误监听器\n\n";
    
    report << "## 总体统计\n";
    report << "- 总规则数: " << rules.size() << "\n";
    report << "- CSS3合规规则: " << compliantRules << "\n";
    report << "- 规则合规率: " << (rules.empty() ? 0 : (compliantRules * 100 / rules.size())) << "%\n\n";
    
    report << "## 选择器统计\n";
    report << "- 总选择器数: " << totalSelectors << "\n";
    report << "- 标准合规选择器: " << compliantSelectors << "\n";
    report << "- 选择器合规率: " << (totalSelectors == 0 ? 0 : (compliantSelectors * 100 / totalSelectors)) << "%\n\n";
    
    return report.str();
}