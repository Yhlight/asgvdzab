#include "parser/StyleBlockParser.h"
#include "ast/ASTNode.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>
#include <regex>
#include <random>

namespace chtl {

// 实现类
class StyleBlockParser::Impl {
public:
    StyleBlockConfig config;
    std::vector<std::shared_ptr<ASTNode>> style_blocks;
    std::vector<StyleRuleInfo> all_style_rules;
    std::unordered_set<std::string> used_selectors;
    std::unordered_map<std::string, std::string> auto_generated_selectors;
    std::vector<std::string> errors;
    bool debug_mode;
    
    // CSS属性验证规则
    std::unordered_set<std::string> valid_css_properties;
    std::unordered_set<std::string> valid_css_values;
    
    Impl(const StyleBlockConfig& cfg) : config(cfg), debug_mode(false) {
        initializeCSSValidation();
    }
    
    ~Impl() = default;
    
    // 内部方法实现
    bool isStyleBlockStart(const std::vector<Token>& tokens, size_t position);
    bool isCSSRuleStart(const std::vector<Token>& tokens, size_t position);
    bool isSelectorStart(const std::vector<Token>& tokens, size_t position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    
    // CSS验证辅助方法
    bool isValidCSSPropertyName(const std::string& property);
    bool isValidCSSValue(const std::string& value);
    bool isValidSelector(const std::string& selector);
    std::string normalizeCSSValue(const std::string& value);
    
    // 选择器生成辅助方法
    std::string sanitizeElementName(const std::string& element_name);
    std::string generateUniqueSelector(const std::string& base, const std::string& type);
    bool isSelectorAlreadyUsed(const std::string& selector);
    
    // 辅助方法
    void initializeCSSValidation();
    void logDebug(const std::string& message);
    std::string generateRandomSuffix();
};

// StyleBlockParser实现
StyleBlockParser::StyleBlockParser(const StyleBlockConfig& config) 
    : pImpl(std::make_unique<Impl>(config)) {}

StyleBlockParser::~StyleBlockParser() = default;

std::shared_ptr<ASTNode> StyleBlockParser::parseStyleBlock(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isStyleBlockStart(tokens, position)) {
        addError("不是样式块开始");
        return nullptr;
    }
    
    // 跳过style关键字
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("样式块缺少左大括号");
        return nullptr;
    }
    position++;
    
    // 解析样式规则
    auto style_rules = parseStyleRules(tokens, position);
    
    // 检查右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("样式块缺少右大括号");
        return nullptr;
    }
    position++;
    
    // 创建样式块节点（这里需要创建具体的AST节点类型）
    // 暂时返回nullptr，等待具体的AST节点类型定义
    if (pImpl->debug_mode) {
        pImpl->logDebug("样式块解析完成，包含 " + std::to_string(style_rules.size()) + " 条规则");
    }
    
    return nullptr; // TODO: 创建具体的样式块AST节点
}

std::vector<StyleRuleInfo> StyleBlockParser::parseStyleRules(const std::vector<Token>& tokens, size_t& position) {
    std::vector<StyleRuleInfo> rules;
    
    while (position < tokens.size() && tokens[position].value != "}") {
        pImpl->skipWhitespace(tokens, position);
        
        if (position >= tokens.size()) {
            addError("样式块未正确结束");
            break;
        }
        
        // 检查是否是选择器
        if (pImpl->isSelectorStart(tokens, position)) {
            std::string selector = parseSelector(tokens, position);
            
            // 跳过空白字符
            pImpl->skipWhitespace(tokens, position);
            
            // 检查左大括号
            if (position >= tokens.size() || tokens[position].value != "{") {
                addError("选择器缺少左大括号");
                break;
            }
            position++;
            
            // 解析选择器内的CSS规则
            while (position < tokens.size() && tokens[position].value != "}") {
                pImpl->skipWhitespace(tokens, position);
                
                if (position >= tokens.size()) {
                    addError("选择器块未正确结束");
                    break;
                }
                
                auto [property, value] = parseCSSRule(tokens, position);
                if (!property.empty() && !value.empty()) {
                    StyleRuleInfo rule;
                    rule.selector = selector;
                    rule.property = property;
                    rule.value = value;
                    rule.original_value = value;
                    rule.line = tokens[position - 2].line;
                    rule.column = tokens[position - 2].column;
                    rule.is_inline = false;
                    rule.is_auto_generated = false;
                    
                    rules.push_back(rule);
                    pImpl->all_style_rules.push_back(rule);
                }
                
                pImpl->skipWhitespace(tokens, position);
            }
            
            // 跳过右大括号
            if (position < tokens.size() && tokens[position].value == "}") {
                position++;
            }
        } else if (pImpl->isCSSRuleStart(tokens, position)) {
            // 内联样式规则
            auto [property, value] = parseCSSRule(tokens, position);
            if (!property.empty() && !value.empty()) {
                StyleRuleInfo rule;
                rule.selector = ""; // 内联样式没有选择器
                rule.property = property;
                rule.value = value;
                rule.original_value = value;
                rule.line = tokens[position - 2].line;
                rule.column = tokens[position - 2].column;
                rule.is_inline = true;
                rule.is_auto_generated = false;
                
                rules.push_back(rule);
                pImpl->all_style_rules.push_back(rule);
            }
        } else {
            // 跳过未知内容
            position++;
        }
        
        pImpl->skipWhitespace(tokens, position);
    }
    
    return rules;
}

std::vector<StyleRuleInfo> StyleBlockParser::parseInlineStyles(const std::vector<Token>& tokens, size_t& position) {
    std::vector<StyleRuleInfo> inline_rules;
    
    while (position < tokens.size() && pImpl->isCSSRuleStart(tokens, position)) {
        auto [property, value] = parseCSSRule(tokens, position);
        if (!property.empty() && !value.empty()) {
            StyleRuleInfo rule;
            rule.selector = "";
            rule.property = property;
            rule.value = value;
            rule.original_value = value;
            rule.line = tokens[position - 2].line;
            rule.column = tokens[position - 2].column;
            rule.is_inline = true;
            rule.is_auto_generated = false;
            
            inline_rules.push_back(rule);
        }
        
        pImpl->skipWhitespace(tokens, position);
    }
    
    return inline_rules;
}

std::string StyleBlockParser::parseSelector(const std::vector<Token>& tokens, size_t& position) {
    std::string selector;
    
    while (position < tokens.size() && tokens[position].value != "{") {
        if (tokens[position].type == TokenType::IDENTIFIER || 
            tokens[position].type == TokenType::CSS_SELECTOR ||
            tokens[position].type == TokenType::CONTEXT_REFERENCE) {
            selector += tokens[position].value;
        } else if (tokens[position].type == TokenType::WHITESPACE) {
            selector += " ";
        }
        position++;
    }
    
    // 移除首尾空白字符
    selector.erase(0, selector.find_first_not_of(" \t\n\r"));
    selector.erase(selector.find_last_not_of(" \t\n\r") + 1);
    
    return selector;
}

bool StyleBlockParser::isClassSelector(const std::string& selector) {
    return selector.length() > 1 && selector[0] == '.';
}

bool StyleBlockParser::isIdSelector(const std::string& selector) {
    return selector.length() > 1 && selector[0] == '#';
}

bool StyleBlockParser::isPseudoSelector(const std::string& selector) {
    return selector.length() > 1 && (selector[0] == ':' || 
           (selector.length() > 2 && selector[0] == ':' && selector[1] == ':'));
}

std::string StyleBlockParser::resolveContextReference(const std::string& reference, const std::string& element_name) {
    if (reference == "&") {
        // 根据上下文推导，优先使用类名
        if (pImpl->config.auto_generate_classes) {
            return "." + pImpl->sanitizeElementName(element_name);
        } else if (pImpl->config.auto_generate_ids) {
            return "#" + pImpl->sanitizeElementName(element_name);
        }
        return element_name;
    }
    
    // 处理伪类和伪元素
    if (reference.find("&:") == 0) {
        std::string pseudo = reference.substr(1); // 移除&
        if (pImpl->config.auto_generate_classes) {
            return "." + pImpl->sanitizeElementName(element_name) + pseudo;
        }
    }
    
    if (reference.find("&::") == 0) {
        std::string pseudo = reference.substr(1); // 移除&
        if (pImpl->config.auto_generate_classes) {
            return "." + pImpl->sanitizeElementName(element_name) + pseudo;
        }
    }
    
    return reference;
}

std::string StyleBlockParser::expandContextReference(const std::string& reference, const std::string& base_selector) {
    if (reference == "&") {
        return base_selector;
    }
    
    if (reference.find("&") != std::string::npos) {
        std::string result = reference;
        size_t pos = result.find("&");
        while (pos != std::string::npos) {
            result.replace(pos, 1, base_selector);
            pos = result.find("&", pos + base_selector.length());
        }
        return result;
    }
    
    return reference;
}

std::pair<std::string, std::string> StyleBlockParser::parseCSSRule(const std::vector<Token>& tokens, size_t& position) {
    std::string property;
    std::string value;
    
    // 解析CSS属性
    if (position < tokens.size() && 
        (tokens[position].type == TokenType::IDENTIFIER || tokens[position].type == TokenType::CSS_PROPERTY)) {
        property = tokens[position].value;
        position++;
    } else {
        return {"", ""};
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查冒号
    if (position >= tokens.size() || tokens[position].value != ":") {
        addError("CSS规则缺少冒号");
        return {"", ""};
    }
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析CSS值
    while (position < tokens.size() && tokens[position].value != ";") {
        value += tokens[position].value;
        position++;
    }
    
    // 跳过分号
    if (position < tokens.size() && tokens[position].value == ";") {
        position++;
    }
    
    // 标准化值
    value = pImpl->normalizeCSSValue(value);
    
    return {property, value};
}

bool StyleBlockParser::validateCSSProperty(const std::string& property) {
    return pImpl->isValidCSSPropertyName(property);
}

bool StyleBlockParser::validateCSSValue(const std::string& value) {
    return pImpl->isValidCSSValue(value);
}

std::string StyleBlockParser::generateClassName(const std::string& element_name, const std::string& context) {
    std::string base = pImpl->sanitizeElementName(element_name);
    if (!context.empty()) {
        base += "_" + pImpl->sanitizeElementName(context);
    }
    return pImpl->generateUniqueSelector(base, "class");
}

std::string StyleBlockParser::generateIdName(const std::string& element_name, const std::string& context) {
    std::string base = pImpl->sanitizeElementName(element_name);
    if (!context.empty()) {
        base += "_" + pImpl->sanitizeElementName(context);
    }
    return pImpl->generateUniqueSelector(base, "id");
}

void StyleBlockParser::addAutoGeneratedSelector(const std::string& selector, const std::string& element_name) {
    pImpl->auto_generated_selectors[selector] = element_name;
    pImpl->used_selectors.insert(selector);
}

void StyleBlockParser::addStyleBlock(std::shared_ptr<ASTNode> style_block) {
    if (style_block) {
        pImpl->style_blocks.push_back(style_block);
    }
}

std::vector<std::shared_ptr<ASTNode>> StyleBlockParser::getStyleBlocks() const {
    return pImpl->style_blocks;
}

std::vector<StyleRuleInfo> StyleBlockParser::getAllStyleRules() const {
    return pImpl->all_style_rules;
}

void StyleBlockParser::setConfig(const StyleBlockConfig& config) {
    pImpl->config = config;
}

StyleBlockConfig StyleBlockParser::getConfig() const {
    return pImpl->config;
}

bool StyleBlockParser::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> StyleBlockParser::getErrors() const {
    return pImpl->errors;
}

void StyleBlockParser::clearErrors() {
    pImpl->errors.clear();
}

void StyleBlockParser::addError(const std::string& error_message) {
    pImpl->errors.push_back(error_message);
    
    if (pImpl->debug_mode) {
        std::cerr << "样式块解析器错误: " << error_message << std::endl;
    }
}

std::string StyleBlockParser::getDebugInfo() const {
    std::ostringstream oss;
    oss << "样式块解析器调试信息:\n";
    oss << "样式块数量: " << pImpl->style_blocks.size() << "\n";
    oss << "样式规则总数: " << pImpl->all_style_rules.size() << "\n";
    oss << "已使用选择器数量: " << pImpl->used_selectors.size() << "\n";
    oss << "自动生成选择器数量: " << pImpl->auto_generated_selectors.size() << "\n";
    oss << "错误数量: " << pImpl->errors.size() << "\n";
    oss << "内联样式: " << (pImpl->config.enable_inline_styles ? "启用" : "禁用") << "\n";
    oss << "自动化选择器: " << (pImpl->config.enable_auto_selectors ? "启用" : "禁用") << "\n";
    oss << "上下文引用: " << (pImpl->config.enable_context_reference ? "启用" : "禁用") << "\n";
    oss << "自动生成类名: " << (pImpl->config.auto_generate_classes ? "启用" : "禁用") << "\n";
    oss << "自动生成ID: " << (pImpl->config.auto_generate_ids ? "启用" : "禁用") << "\n";
    return oss.str();
}

void StyleBlockParser::enableDebugMode(bool enable) {
    pImpl->debug_mode = enable;
}

// Impl类方法实现
bool StyleBlockParser::Impl::isStyleBlockStart(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return false;
    }
    
    const Token& token = tokens[position];
    return token.type == TokenType::STYLE_KEYWORD && token.value == "style";
}

bool StyleBlockParser::Impl::isCSSRuleStart(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return false;
    }
    
    const Token& token = tokens[position];
    return token.type == TokenType::IDENTIFIER || token.type == TokenType::CSS_PROPERTY;
}

bool StyleBlockParser::Impl::isSelectorStart(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return false;
    }
    
    const Token& token = tokens[position];
    return token.type == TokenType::CSS_SELECTOR || 
           (token.type == TokenType::IDENTIFIER && 
            (token.value[0] == '.' || token.value[0] == '#' || token.value[0] == ':'));
}

void StyleBlockParser::Impl::skipWhitespace(const std::vector<Token>& tokens, size_t& position) {
    while (position < tokens.size() && 
           (tokens[position].value == " " || tokens[position].value == "\t" || 
            tokens[position].value == "\n" || tokens[position].value == "\r")) {
        position++;
    }
}

bool StyleBlockParser::Impl::isEndOfTokens(const std::vector<Token>& tokens, size_t position) {
    return position >= tokens.size();
}

bool StyleBlockParser::Impl::isValidCSSPropertyName(const std::string& property) {
    if (property.empty()) {
        return false;
    }
    
    // 检查是否只包含字母、数字、连字符
    for (char c : property) {
        if (!std::isalnum(c) && c != '-') {
            return false;
        }
    }
    
    // 检查是否在预定义的有效属性列表中
    return valid_css_properties.find(property) != valid_css_properties.end();
}

bool StyleBlockParser::Impl::isValidCSSValue(const std::string& value) {
    return !value.empty() && value.length() <= config.max_css_value_length;
}

bool StyleBlockParser::Impl::isValidSelector(const std::string& selector) {
    if (selector.empty()) {
        return false;
    }
    
    // 基本选择器格式验证
    std::regex selector_pattern("^[.#]?[a-zA-Z][a-zA-Z0-9_-]*([:][a-zA-Z][a-zA-Z0-9_-]*)?$");
    return std::regex_match(selector, selector_pattern);
}

std::string StyleBlockParser::Impl::normalizeCSSValue(const std::string& value) {
    std::string result = value;
    
    // 移除首尾空白字符
    result.erase(0, result.find_first_not_of(" \t\n\r"));
    result.erase(result.find_last_not_of(" \t\n\r") + 1);
    
    return result;
}

std::string StyleBlockParser::Impl::sanitizeElementName(const std::string& element_name) {
    std::string result = element_name;
    
    // 转换为小写
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    
    // 移除无效字符
    result.erase(std::remove_if(result.begin(), result.end(), 
                [](char c) { return !std::isalnum(c) && c != '_' && c != '-'; }), result.end());
    
    return result;
}

std::string StyleBlockParser::Impl::generateUniqueSelector(const std::string& base, const std::string& type) {
    std::string selector = base;
    std::string prefix = (type == "class") ? "." : "#";
    
    // 如果选择器已被使用，添加随机后缀
    if (isSelectorAlreadyUsed(prefix + selector)) {
        selector += "_" + generateRandomSuffix();
    }
    
    return prefix + selector;
}

bool StyleBlockParser::Impl::isSelectorAlreadyUsed(const std::string& selector) {
    return used_selectors.find(selector) != used_selectors.end();
}

void StyleBlockParser::Impl::initializeCSSValidation() {
    // 初始化有效的CSS属性列表
    valid_css_properties = {
        "color", "background-color", "width", "height", "margin", "padding",
        "border", "font-size", "font-family", "text-align", "display",
        "position", "top", "left", "right", "bottom", "z-index",
        "opacity", "visibility", "overflow", "float", "clear"
    };
}

std::string StyleBlockParser::Impl::generateRandomSuffix() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1000, 9999);
    
    return std::to_string(dis(gen));
}

void StyleBlockParser::Impl::logDebug(const std::string& message) {
    if (debug_mode) {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}

} // namespace chtl