#include "compilers/antlr_css_compiler.hpp"
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
            specificity = 5; // 复合选择器的默认权重
            break;
    }
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

CSSCompileResult ANTLRCSSCompiler::compileCSS(const std::string& cssCode, const CSSCompileOptions& options) {
    CSSCompileResult result;
    result.originalSize = cssCode.length();
    
    try {
        std::cout << "开始ANTLR CSS编译..." << std::endl;
        
        // 使用ANTLR解析CSS
        std::vector<std::string> parseErrors;
        auto parseTree = parseCSS(cssCode, parseErrors);
        
        if (!parseErrors.empty()) {
            result.errors.insert(result.errors.end(), parseErrors.begin(), parseErrors.end());
        }
        
        if (parseTree) {
            // 从解析树中提取信息
            extractInfoFromParseTree(parseTree.get(), result, options);
            std::cout << "ANTLR解析成功，提取的选择器数量: " << result.selectors.size() << std::endl;
            std::cout << "提取的CSS规则数量: " << result.rules.size() << std::endl;
        } else {
            // 如果ANTLR解析失败，回退到正则表达式解析
            std::cout << "ANTLR解析失败，回退到正则表达式解析..." << std::endl;
            
            if (options.extractSelectors) {
                result.selectors = extractStandardSelectorsRegex(cssCode);
            }
            result.rules = extractCSSRulesRegex(cssCode);
            
            if (options.extractColors) {
                result.colors = StandardCSSUtils::extractColorsFromValue(cssCode);
            }
        }
        
        // 验证标准合规性
        if (options.validateStandardCompliance) {
            validateStandardCompliance(result.selectors, result.warnings);
        }
        
        // 生成CSS
        result.css = cssCode;
        if (options.minify) {
            result.css = StandardCSSUtils::removeWhitespace(result.css);
        }
        
        result.compiledSize = result.css.length();
        result.success = true;
        
        std::cout << "标准CSS编译完成" << std::endl;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("CSS compilation error: ") + e.what());
    }
    
    return result;
}

std::vector<CSSSelector> ANTLRCSSCompiler::extractStandardSelectorsRegex(const std::string& cssCode) {
    std::vector<CSSSelector> selectors;
    
    // 使用正则表达式提取标准CSS选择器
    std::regex ruleRegex(R"(([^{]+)\s*\{[^}]*\})");
    std::sregex_iterator iter(cssCode.begin(), cssCode.end(), ruleRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::string selectorGroup = iter->str(1);
        
        // 分割多个选择器（逗号分隔）
        std::regex commaRegex(R"(\s*,\s*)");
        std::sregex_token_iterator tokIter(selectorGroup.begin(), selectorGroup.end(), commaRegex, -1);
        std::sregex_token_iterator tokEnd;
        
        for (; tokIter != tokEnd; ++tokIter) {
            std::string selectorText = tokIter->str();
            selectorText = std::regex_replace(selectorText, std::regex(R"(^\s+|\s+$)"), ""); // trim
            
            if (!selectorText.empty()) {
                StandardCSSSelector type = detectSelectorTypeRegex(selectorText);
                selectors.emplace_back(type, selectorText);
            }
        }
    }
    
    return selectors;
}

std::vector<CSSRule> ANTLRCSSCompiler::extractCSSRulesRegex(const std::string& cssCode) {
    std::vector<CSSRule> rules;
    
    std::regex ruleRegex(R"(([^{]+)\s*\{([^}]*)\})");
    std::sregex_iterator iter(cssCode.begin(), cssCode.end(), ruleRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        CSSRule rule;
        
        // 解析选择器
        std::string selectorsText = iter->str(1);
        auto selectorObjs = extractStandardSelectorsRegex(selectorsText + " {}"); // 添加临时规则体
        if (!selectorObjs.empty()) {
            rule.selectors = selectorObjs;
        }
        
        // 解析属性
        std::string propertiesText = iter->str(2);
        std::regex propRegex(R"(([^:]+)\s*:\s*([^;]+);?)");
        std::sregex_iterator propIter(propertiesText.begin(), propertiesText.end(), propRegex);
        std::sregex_iterator propEnd;
        
        for (; propIter != propEnd; ++propIter) {
            std::string property = propIter->str(1);
            std::string value = propIter->str(2);
            
            // Trim whitespace
            property = std::regex_replace(property, std::regex(R"(^\s+|\s+$)"), "");
            value = std::regex_replace(value, std::regex(R"(^\s+|\s+$)"), "");
            
            if (!property.empty() && !value.empty()) {
                rule.properties.emplace_back(property, value);
            }
        }
        
        rule.sourceText = iter->str();
        rules.push_back(rule);
    }
    
    return rules;
}

StandardCSSSelector ANTLRCSSCompiler::detectSelectorTypeRegex(const std::string& selector) {
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

bool ANTLRCSSCompiler::validateSyntax(const std::string& cssCode, std::vector<std::string>& errors) {
    // 基本的CSS语法验证
    try {
        auto rules = extractCSSRulesRegex(cssCode);
        return !rules.empty() || cssCode.empty();
    } catch (const std::exception& e) {
        errors.push_back(e.what());
        return false;
    }
}

bool ANTLRCSSCompiler::validateStandardCompliance(const std::vector<CSSSelector>& selectors, std::vector<std::string>& warnings) {
    bool isCompliant = true;
    
    for (const auto& selector : selectors) {
        if (!StandardCSSUtils::isValidStandardSelector(selector.value)) {
            warnings.push_back("Non-standard selector detected: " + selector.value);
            isCompliant = false;
        }
    }
    
    return isCompliant;
}

std::vector<CSSSelector> ANTLRCSSCompiler::extractStandardSelectors(const std::string& cssCode) {
    return extractStandardSelectorsRegex(cssCode);
}

std::vector<CSSRule> ANTLRCSSCompiler::extractCSSRules(const std::string& cssCode) {
    return extractCSSRulesRegex(cssCode);
}

std::vector<std::string> ANTLRCSSCompiler::extractProperties(const std::string& cssCode) {
    auto rules = extractCSSRules(cssCode);
    std::vector<std::string> properties;
    
    for (const auto& rule : rules) {
        for (const auto& prop : rule.properties) {
            properties.push_back(prop.first);
        }
    }
    
    return properties;
}

std::vector<std::string> ANTLRCSSCompiler::extractColors(const std::string& cssCode) {
    return StandardCSSUtils::extractColorsFromValue(cssCode);
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

std::string ANTLRCSSCompiler::generateSelectorDocumentation(const std::vector<CSSSelector>& selectors) {
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
    reference << "# CSS 标准选择器参考\n\n";
    
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
        reference << "- 标准合规性: " << (isValidStandardSelector(selector.value) ? "✓ 符合" : "⚠ 非标准") << "\n\n";
    }
    
    return reference.str();
}

std::string StandardCSSUtils::generateCSS3ComplianceReport(const std::vector<CSSRule>& rules) {
    std::ostringstream report;
    report << "# CSS3 标准合规性报告\n\n";
    
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

// ========== ANTLR监听器实现 ==========

void CSSListener::enterStylesheet(CSS3Parser::StylesheetContext *ctx) {
    // 重置状态
    selectors_.clear();
    rules_.clear();
    colors_.clear();
    fonts_.clear();
    errors_.clear();
}

void CSSListener::exitStylesheet(CSS3Parser::StylesheetContext *ctx) {
    // 样式表解析完成
}

void CSSListener::enterRuleset(CSS3Parser::RulesetContext *ctx) {
    // 开始新的CSS规则
    currentRule_ = std::make_unique<CSSRule>();
}

void CSSListener::exitRuleset(CSS3Parser::RulesetContext *ctx) {
    // 完成当前CSS规则
    if (currentRule_ && !currentRule_->selectors.empty()) {
        rules_.push_back(*currentRule_);
        currentRule_.reset();
    }
}

void CSSListener::enterSelector(CSS3Parser::SelectorContext *ctx) {
    if (ctx) {
        std::string selectorText = ctx->getText();
        
        // 分析选择器类型
        StandardCSSSelector selectorType = StandardCSSSelector::TYPE;
        if (selectorText.find('#') != std::string::npos) {
            selectorType = StandardCSSSelector::ID;
        } else if (selectorText.find('.') != std::string::npos) {
            selectorType = StandardCSSSelector::CLASS;
        } else if (selectorText.find('[') != std::string::npos) {
            selectorType = StandardCSSSelector::ATTRIBUTE;
        } else if (selectorText.find(':') != std::string::npos) {
            if (selectorText.find("::") != std::string::npos) {
                selectorType = StandardCSSSelector::PSEUDO_ELEMENT;
            } else {
                selectorType = StandardCSSSelector::PSEUDO_CLASS;
            }
        }
        
        // 创建选择器对象
        CSSSelector selector(selectorType, selectorText);
        selector.originalText = selectorText;
        selectors_.push_back(selector);
        
        // 更新当前规则
        if (currentRule_) {
            currentRule_->selectors.push_back(selector);
        }
    }
}

void CSSListener::enterDeclaration(CSS3Parser::DeclarationContext *ctx) {
    if (ctx) {
        std::string declarationText = ctx->getText();
        
        // 提取属性和值
        size_t colonPos = declarationText.find(':');
        if (colonPos != std::string::npos && colonPos + 1 < declarationText.length()) {
            std::string property = declarationText.substr(0, colonPos);
            std::string value = declarationText.substr(colonPos + 1);
            
            // 清理空白字符
            property.erase(std::remove_if(property.begin(), property.end(), ::isspace), property.end());
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            if (currentRule_) {
                currentRule_->properties.emplace_back(property, value);
            }
            
            // 提取颜色值
            std::regex colorRegex(R"(#[0-9a-fA-F]{3,6}|rgb\([^)]+\)|rgba\([^)]+\)|hsl\([^)]+\)|hsla\([^)]+\))");
            std::smatch match;
            if (std::regex_search(value, match, colorRegex)) {
                colors_.push_back(match.str());
            }
            
            // 提取字体信息
            if (property == "font-family" || property == "font") {
                fonts_.push_back(value);
            }
        }
    }
}

void CSSErrorListener::syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol,
                                  size_t line, size_t charPositionInLine,
                                  const std::string &msg, std::exception_ptr e) {
    std::ostringstream error;
    error << "语法错误 第" << line << "行第" << charPositionInLine << "列: " << msg;
    errors_.push_back(error.str());
}

// ========== ANTLR解析方法实现 ==========

std::unique_ptr<ParseTree> ANTLRCSSCompiler::parseCSS(const std::string& cssCode, std::vector<std::string>& errors) {
    try {
        std::string inputCopy = cssCode;  // 确保字符串的生命周期
        ANTLRInputStream input(inputCopy);
        CSS3Lexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        CSS3Parser parser(&tokens);
        
        // 添加错误监听器
        CSSErrorListener errorListener;
        parser.removeErrorListeners();
        lexer.removeErrorListeners();
        parser.addErrorListener(&errorListener);
        lexer.addErrorListener(&errorListener);
        
        auto result = parser.stylesheet();
        
        if (errorListener.hasErrors()) {
            auto listenerErrors = errorListener.getErrors();
            errors.insert(errors.end(), listenerErrors.begin(), listenerErrors.end());
        }
        
        return std::unique_ptr<ParseTree>(result);
    } catch (const std::exception& e) {
        errors.push_back(std::string("ANTLR解析错误: ") + e.what());
        return nullptr;
    }
}

void ANTLRCSSCompiler::extractInfoFromParseTree(ParseTree* tree, CSSCompileResult& result, const CSSCompileOptions& options) {
    if (!tree) return;
    
    try {
        CSSListener listener;
        ParseTreeWalker walker;
        walker.walk(&listener, tree);
        
        result.selectors = listener.getSelectors();
        result.rules = listener.getRules();
        result.colors = listener.getColors();
        result.fonts = listener.getFonts();
        
        auto listenerErrors = listener.getErrors();
        result.errors.insert(result.errors.end(), listenerErrors.begin(), listenerErrors.end());
    } catch (const std::exception& e) {
        result.errors.push_back(std::string("信息提取错误: ") + e.what());
    }
}