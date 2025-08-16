#include "lexer/complete_chtl_tokens.hpp"
#include <algorithm>
#include <regex>
#include <sstream>

namespace chtl {

// ===== CompleteCHTLToken 实现 =====

void CompleteCHTLToken::updateSemanticInfo() {
    semantic = SemanticInfo{}; // 重置
    
    switch (type) {
        // 字面量类型
        case CompleteCHTLTokenType::IDENTIFIER:
        case CompleteCHTLTokenType::STRING_DOUBLE:
        case CompleteCHTLTokenType::STRING_SINGLE:
        case CompleteCHTLTokenType::STRING_UNQUOTED:
            semantic.isLiteral = true;
            break;
            
        // 关键字类型
        case CompleteCHTLTokenType::KEYWORD_TEXT:
        case CompleteCHTLTokenType::KEYWORD_STYLE:
        case CompleteCHTLTokenType::KEYWORD_SCRIPT:
        case CompleteCHTLTokenType::KEYWORD_INHERIT:
        case CompleteCHTLTokenType::KEYWORD_DELETE:
        case CompleteCHTLTokenType::KEYWORD_INSERT:
        case CompleteCHTLTokenType::KEYWORD_AFTER:
        case CompleteCHTLTokenType::KEYWORD_BEFORE:
        case CompleteCHTLTokenType::KEYWORD_REPLACE:
        case CompleteCHTLTokenType::KEYWORD_AT_TOP:
        case CompleteCHTLTokenType::KEYWORD_AT_BOTTOM:
        case CompleteCHTLTokenType::KEYWORD_FROM:
        case CompleteCHTLTokenType::KEYWORD_AS:
        case CompleteCHTLTokenType::KEYWORD_EXCEPT:
            semantic.isKeyword = true;
            semantic.isConfigurable = true;
            break;
            
        // 结构关键字
        case CompleteCHTLTokenType::KEYWORD_TEMPLATE:
        case CompleteCHTLTokenType::KEYWORD_CUSTOM:
        case CompleteCHTLTokenType::KEYWORD_ORIGIN:
        case CompleteCHTLTokenType::KEYWORD_IMPORT:
        case CompleteCHTLTokenType::KEYWORD_NAMESPACE:
        case CompleteCHTLTokenType::KEYWORD_CONFIGURATION:
        case CompleteCHTLTokenType::KEYWORD_INFO:
        case CompleteCHTLTokenType::KEYWORD_EXPORT:
        case CompleteCHTLTokenType::KEYWORD_NAME:
            semantic.isKeyword = true;
            semantic.isStructuralKeyword = true;
            semantic.isConfigurable = true;
            break;
            
        // 前缀类型
        case CompleteCHTLTokenType::PREFIX_STYLE:
        case CompleteCHTLTokenType::PREFIX_ELEMENT:
        case CompleteCHTLTokenType::PREFIX_VAR:
        case CompleteCHTLTokenType::PREFIX_HTML:
        case CompleteCHTLTokenType::PREFIX_JAVASCRIPT:
        case CompleteCHTLTokenType::PREFIX_CHTL:
        case CompleteCHTLTokenType::PREFIX_CJMOD:
            semantic.isPrefixType = true;
            semantic.isConfigurable = true;
            break;
            
        // 操作符
        case CompleteCHTLTokenType::COLON:
        case CompleteCHTLTokenType::EQUALS:
        case CompleteCHTLTokenType::COLON_EQUALS:
        case CompleteCHTLTokenType::DOT:
        case CompleteCHTLTokenType::HASH:
        case CompleteCHTLTokenType::AMPERSAND:
        case CompleteCHTLTokenType::CHTL_JS_ARROW:
            semantic.isOperator = true;
            break;
            
        // 分隔符
        case CompleteCHTLTokenType::LBRACE:
        case CompleteCHTLTokenType::RBRACE:
        case CompleteCHTLTokenType::LBRACKET:
        case CompleteCHTLTokenType::RBRACKET:
        case CompleteCHTLTokenType::LPAREN:
        case CompleteCHTLTokenType::RPAREN:
        case CompleteCHTLTokenType::SEMICOLON:
        case CompleteCHTLTokenType::COMMA:
        case CompleteCHTLTokenType::SLASH:
            semantic.isSeparator = true;
            break;
            
        // 注释
        case CompleteCHTLTokenType::COMMENT_LINE:
        case CompleteCHTLTokenType::COMMENT_MULTILINE:
        case CompleteCHTLTokenType::COMMENT_GENERATOR:
            semantic.isComment = true;
            break;
            
        // 空白字符
        case CompleteCHTLTokenType::WHITESPACE:
        case CompleteCHTLTokenType::NEWLINE:
        case CompleteCHTLTokenType::TAB:
            semantic.isWhitespace = true;
            break;
            
        // HTML元素
        case CompleteCHTLTokenType::HTML_TAG:
            semantic.isHTMLElement = true;
            break;
            
        // CSS相关
        case CompleteCHTLTokenType::CSS_PROPERTY:
        case CompleteCHTLTokenType::CSS_VALUE:
        case CompleteCHTLTokenType::CSS_SELECTOR_CLASS:
        case CompleteCHTLTokenType::CSS_SELECTOR_ID:
        case CompleteCHTLTokenType::CSS_PSEUDO_CLASS:
        case CompleteCHTLTokenType::CSS_PSEUDO_ELEMENT:
            semantic.isCSSRelated = true;
            break;
            
        // CHTL JS相关
        case CompleteCHTLTokenType::CHTL_JS_SELECTOR:
        case CompleteCHTLTokenType::CHTL_JS_ARROW:
        case CompleteCHTLTokenType::CHTL_JS_LISTEN:
        case CompleteCHTLTokenType::CHTL_JS_DELEGATE:
        case CompleteCHTLTokenType::CHTL_JS_ANIMATE:
            semantic.isCHTLJSRelated = true;
            break;
            
        default:
            break;
    }
}

bool CompleteCHTLToken::isLiteral() const { return semantic.isLiteral; }
bool CompleteCHTLToken::isKeyword() const { return semantic.isKeyword; }
bool CompleteCHTLToken::isOperator() const { return semantic.isOperator; }
bool CompleteCHTLToken::isSeparator() const { return semantic.isSeparator; }
bool CompleteCHTLToken::isComment() const { return semantic.isComment; }
bool CompleteCHTLToken::isWhitespace() const { return semantic.isWhitespace; }
bool CompleteCHTLToken::isPrefixType() const { return semantic.isPrefixType; }
bool CompleteCHTLToken::isStructuralKeyword() const { return semantic.isStructuralKeyword; }
bool CompleteCHTLToken::isHTMLElement() const { return semantic.isHTMLElement; }
bool CompleteCHTLToken::isCSSRelated() const { return semantic.isCSSRelated; }
bool CompleteCHTLToken::isCHTLJSRelated() const { return semantic.isCHTLJSRelated; }
bool CompleteCHTLToken::isConfigurable() const { return semantic.isConfigurable; }

std::string CompleteCHTLToken::toString() const {
    return CompleteCHTLTokenUtils::tokenTypeToString(type) + ": \"" + value + "\"";
}

std::string CompleteCHTLToken::getContextualInfo() const {
    std::stringstream ss;
    ss << toString();
    if (!semantic.context.empty()) {
        ss << " [context: " << semantic.context << "]";
    }
    if (semantic.nestingLevel > 0) {
        ss << " [level: " << semantic.nestingLevel << "]";
    }
    return ss.str();
}

// ===== CompleteCHTLKeywordMap 实现 =====

CompleteCHTLKeywordMap::CompleteCHTLKeywordMap() {
    initializeDefaultKeywords();
    initializeConfigurableKeywords();
}

void CompleteCHTLKeywordMap::initializeDefaultKeywords() {
    // 核心关键字
    keywordMap_["text"] = CompleteCHTLTokenType::KEYWORD_TEXT;
    keywordMap_["style"] = CompleteCHTLTokenType::KEYWORD_STYLE;
    keywordMap_["script"] = CompleteCHTLTokenType::KEYWORD_SCRIPT;
    
    // 模板和自定义关键字
    keywordMap_["inherit"] = CompleteCHTLTokenType::KEYWORD_INHERIT;
    keywordMap_["delete"] = CompleteCHTLTokenType::KEYWORD_DELETE;
    keywordMap_["insert"] = CompleteCHTLTokenType::KEYWORD_INSERT;
    keywordMap_["after"] = CompleteCHTLTokenType::KEYWORD_AFTER;
    keywordMap_["before"] = CompleteCHTLTokenType::KEYWORD_BEFORE;
    keywordMap_["replace"] = CompleteCHTLTokenType::KEYWORD_REPLACE;
    keywordMap_["at top"] = CompleteCHTLTokenType::KEYWORD_AT_TOP;
    keywordMap_["at bottom"] = CompleteCHTLTokenType::KEYWORD_AT_BOTTOM;
    
    // 导入和命名空间
    keywordMap_["from"] = CompleteCHTLTokenType::KEYWORD_FROM;
    keywordMap_["as"] = CompleteCHTLTokenType::KEYWORD_AS;
    keywordMap_["except"] = CompleteCHTLTokenType::KEYWORD_EXCEPT;
    
    // 结构标记
    keywordMap_["[Template]"] = CompleteCHTLTokenType::KEYWORD_TEMPLATE;
    keywordMap_["[Custom]"] = CompleteCHTLTokenType::KEYWORD_CUSTOM;
    keywordMap_["[Origin]"] = CompleteCHTLTokenType::KEYWORD_ORIGIN;
    keywordMap_["[Import]"] = CompleteCHTLTokenType::KEYWORD_IMPORT;
    keywordMap_["[Namespace]"] = CompleteCHTLTokenType::KEYWORD_NAMESPACE;
    keywordMap_["[Configuration]"] = CompleteCHTLTokenType::KEYWORD_CONFIGURATION;
    keywordMap_["[Info]"] = CompleteCHTLTokenType::KEYWORD_INFO;
    keywordMap_["[Export]"] = CompleteCHTLTokenType::KEYWORD_EXPORT;
    keywordMap_["[Name]"] = CompleteCHTLTokenType::KEYWORD_NAME;
}

void CompleteCHTLKeywordMap::initializeConfigurableKeywords() {
    // 根据语法文档中的[Name]配置块设置可配置关键字
    configurableKeywords_["CUSTOM_STYLE"] = {"@Style", "@style", "@CSS", "@Css", "@css"};
    configurableKeywords_["CUSTOM_ELEMENT"] = {"@Element"};
    configurableKeywords_["CUSTOM_VAR"] = {"@Var"};
    configurableKeywords_["TEMPLATE_STYLE"] = {"@Style"};
    configurableKeywords_["TEMPLATE_ELEMENT"] = {"@Element"};
    configurableKeywords_["TEMPLATE_VAR"] = {"@Var"};
    configurableKeywords_["ORIGIN_HTML"] = {"@Html"};
    configurableKeywords_["ORIGIN_STYLE"] = {"@Style"};
    configurableKeywords_["ORIGIN_JAVASCRIPT"] = {"@JavaScript"};
    configurableKeywords_["IMPORT_HTML"] = {"@Html"};
    configurableKeywords_["IMPORT_STYLE"] = {"@Style"};
    configurableKeywords_["IMPORT_JAVASCRIPT"] = {"@JavaScript"};
    configurableKeywords_["IMPORT_CHTL"] = {"@Chtl"};
    configurableKeywords_["IMPORT_CJMOD"] = {"@CJmod"};
}

void CompleteCHTLKeywordMap::updateFromConfiguration(const std::unordered_map<std::string, std::string>& config) {
    // 更新单个关键字配置
    for (const auto& [key, value] : config) {
        if (key == "KEYWORD_INHERIT") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_INHERIT;
        else if (key == "KEYWORD_DELETE") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_DELETE;
        else if (key == "KEYWORD_INSERT") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_INSERT;
        else if (key == "KEYWORD_AFTER") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_AFTER;
        else if (key == "KEYWORD_BEFORE") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_BEFORE;
        else if (key == "KEYWORD_REPLACE") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_REPLACE;
        else if (key == "KEYWORD_ATTOP") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_AT_TOP;
        else if (key == "KEYWORD_ATBOTTOM") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_AT_BOTTOM;
        else if (key == "KEYWORD_FROM") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_FROM;
        else if (key == "KEYWORD_AS") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_AS;
        else if (key == "KEYWORD_EXCEPT") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_EXCEPT;
        else if (key == "KEYWORD_TEXT") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_TEXT;
        else if (key == "KEYWORD_STYLE") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_STYLE;
        else if (key == "KEYWORD_SCRIPT") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_SCRIPT;
        else if (key == "KEYWORD_CUSTOM") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_CUSTOM;
        else if (key == "KEYWORD_TEMPLATE") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_TEMPLATE;
        else if (key == "KEYWORD_ORIGIN") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_ORIGIN;
        else if (key == "KEYWORD_IMPORT") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_IMPORT;
        else if (key == "KEYWORD_NAMESPACE") keywordMap_[value] = CompleteCHTLTokenType::KEYWORD_NAMESPACE;
    }
}

void CompleteCHTLKeywordMap::updateFromNameConfiguration(const std::unordered_map<std::string, std::vector<std::string>>& nameConfig) {
    for (const auto& [key, values] : nameConfig) {
        if (values.size() > maxOptionCount_) {
            // 限制组选项数量
            std::vector<std::string> limitedValues(values.begin(), values.begin() + maxOptionCount_);
            configurableKeywords_[key] = limitedValues;
        } else {
            configurableKeywords_[key] = values;
        }
    }
}

bool CompleteCHTLKeywordMap::isKeyword(const std::string& word) const {
    return keywordMap_.find(word) != keywordMap_.end();
}

CompleteCHTLTokenType CompleteCHTLKeywordMap::getKeywordType(const std::string& word) const {
    auto it = keywordMap_.find(word);
    return (it != keywordMap_.end()) ? it->second : CompleteCHTLTokenType::UNKNOWN;
}

std::vector<std::string> CompleteCHTLKeywordMap::getAllKeywords() const {
    std::vector<std::string> keywords;
    for (const auto& [word, type] : keywordMap_) {
        keywords.push_back(word);
    }
    return keywords;
}

bool CompleteCHTLKeywordMap::isTemplateKeyword(const std::string& word) const {
    return word == "[Template]";
}

bool CompleteCHTLKeywordMap::isCustomKeyword(const std::string& word) const {
    return word == "[Custom]";
}

bool CompleteCHTLKeywordMap::isOriginKeyword(const std::string& word) const {
    return word == "[Origin]";
}

bool CompleteCHTLKeywordMap::isImportKeyword(const std::string& word) const {
    return word == "[Import]";
}

bool CompleteCHTLKeywordMap::isNamespaceKeyword(const std::string& word) const {
    return word == "[Namespace]";
}

bool CompleteCHTLKeywordMap::isConfigurationKeyword(const std::string& word) const {
    return word == "[Configuration]";
}

bool CompleteCHTLKeywordMap::isCustomStyleKeyword(const std::string& word) const {
    auto it = configurableKeywords_.find("CUSTOM_STYLE");
    if (it != configurableKeywords_.end()) {
        return std::find(it->second.begin(), it->second.end(), word) != it->second.end();
    }
    return false;
}

bool CompleteCHTLKeywordMap::isCustomElementKeyword(const std::string& word) const {
    auto it = configurableKeywords_.find("CUSTOM_ELEMENT");
    if (it != configurableKeywords_.end()) {
        return std::find(it->second.begin(), it->second.end(), word) != it->second.end();
    }
    return false;
}

bool CompleteCHTLKeywordMap::isCustomVarKeyword(const std::string& word) const {
    auto it = configurableKeywords_.find("CUSTOM_VAR");
    if (it != configurableKeywords_.end()) {
        return std::find(it->second.begin(), it->second.end(), word) != it->second.end();
    }
    return false;
}

std::vector<std::string> CompleteCHTLKeywordMap::getKeywordVariants(const std::string& baseKeyword) const {
    auto it = configurableKeywords_.find(baseKeyword);
    return (it != configurableKeywords_.end()) ? it->second : std::vector<std::string>{};
}

// ===== CompleteCHTLPrefixMap 实现 =====

CompleteCHTLPrefixMap::CompleteCHTLPrefixMap() {
    initializeDefaultPrefixes();
}

void CompleteCHTLPrefixMap::initializeDefaultPrefixes() {
    prefixMap_["@Style"] = CompleteCHTLTokenType::PREFIX_STYLE;
    prefixMap_["@Element"] = CompleteCHTLTokenType::PREFIX_ELEMENT;
    prefixMap_["@Var"] = CompleteCHTLTokenType::PREFIX_VAR;
    prefixMap_["@Html"] = CompleteCHTLTokenType::PREFIX_HTML;
    prefixMap_["@JavaScript"] = CompleteCHTLTokenType::PREFIX_JAVASCRIPT;
    prefixMap_["@Chtl"] = CompleteCHTLTokenType::PREFIX_CHTL;
    prefixMap_["@CJmod"] = CompleteCHTLTokenType::PREFIX_CJMOD;
}

bool CompleteCHTLPrefixMap::isPrefix(const std::string& prefix) const {
    return prefixMap_.find(prefix) != prefixMap_.end();
}

CompleteCHTLTokenType CompleteCHTLPrefixMap::getPrefixType(const std::string& prefix) const {
    auto it = prefixMap_.find(prefix);
    return (it != prefixMap_.end()) ? it->second : CompleteCHTLTokenType::UNKNOWN;
}

void CompleteCHTLPrefixMap::updateFromConfiguration(const std::unordered_map<std::string, std::string>& config) {
    // 根据配置更新前缀映射
    for (const auto& [key, value] : config) {
        if (key.find("PREFIX_") == 0) {
            // 处理前缀配置
            if (key == "PREFIX_STYLE") prefixMap_[value] = CompleteCHTLTokenType::PREFIX_STYLE;
            else if (key == "PREFIX_ELEMENT") prefixMap_[value] = CompleteCHTLTokenType::PREFIX_ELEMENT;
            else if (key == "PREFIX_VAR") prefixMap_[value] = CompleteCHTLTokenType::PREFIX_VAR;
            else if (key == "PREFIX_HTML") prefixMap_[value] = CompleteCHTLTokenType::PREFIX_HTML;
            else if (key == "PREFIX_JAVASCRIPT") prefixMap_[value] = CompleteCHTLTokenType::PREFIX_JAVASCRIPT;
            else if (key == "PREFIX_CHTL") prefixMap_[value] = CompleteCHTLTokenType::PREFIX_CHTL;
            else if (key == "PREFIX_CJMOD") prefixMap_[value] = CompleteCHTLTokenType::PREFIX_CJMOD;
        }
    }
}

bool CompleteCHTLPrefixMap::isStylePrefix(const std::string& prefix) const {
    return getPrefixType(prefix) == CompleteCHTLTokenType::PREFIX_STYLE;
}

bool CompleteCHTLPrefixMap::isElementPrefix(const std::string& prefix) const {
    return getPrefixType(prefix) == CompleteCHTLTokenType::PREFIX_ELEMENT;
}

bool CompleteCHTLPrefixMap::isVarPrefix(const std::string& prefix) const {
    return getPrefixType(prefix) == CompleteCHTLTokenType::PREFIX_VAR;
}

bool CompleteCHTLPrefixMap::isOriginPrefix(const std::string& prefix) const {
    return prefix == "@Html" || prefix == "@Style" || prefix == "@JavaScript";
}

bool CompleteCHTLPrefixMap::isImportPrefix(const std::string& prefix) const {
    return prefix == "@Html" || prefix == "@Style" || prefix == "@JavaScript" || 
           prefix == "@Chtl" || prefix == "@CJmod";
}

// ===== HTMLElementRecognizer 实现 =====

HTMLElementRecognizer::HTMLElementRecognizer() {
    initializeHTMLElements();
}

void HTMLElementRecognizer::initializeHTMLElements() {
    // 所有标准HTML元素
    htmlElements_ = {
        // 文档结构
        "html", "head", "body", "title", "meta", "link", "style", "script",
        
        // 块级元素
        "div", "p", "h1", "h2", "h3", "h4", "h5", "h6", "header", "footer", 
        "main", "section", "article", "aside", "nav", "blockquote", "pre",
        "address", "hr", "figure", "figcaption",
        
        // 行内元素
        "span", "a", "strong", "em", "b", "i", "u", "s", "small", "mark",
        "del", "ins", "sub", "sup", "code", "kbd", "samp", "var", "time",
        "abbr", "dfn", "q", "cite", "ruby", "rt", "rp", "bdi", "bdo",
        
        // 表单元素
        "form", "input", "textarea", "button", "select", "option", "optgroup",
        "label", "fieldset", "legend", "datalist", "output", "progress", "meter",
        
        // 表格元素
        "table", "thead", "tbody", "tfoot", "tr", "th", "td", "caption",
        "colgroup", "col",
        
        // 列表元素
        "ul", "ol", "li", "dl", "dt", "dd",
        
        // 媒体元素
        "img", "audio", "video", "source", "track", "canvas", "svg", "picture",
        
        // 嵌入元素
        "iframe", "embed", "object", "param", "area", "map",
        
        // 其他元素
        "details", "summary", "dialog", "menu", "menuitem", "template", "slot"
    };
    
    // 块级元素
    blockElements_ = {
        "div", "p", "h1", "h2", "h3", "h4", "h5", "h6", "header", "footer",
        "main", "section", "article", "aside", "nav", "blockquote", "pre",
        "address", "hr", "figure", "figcaption", "form", "table", "ul", "ol", "dl"
    };
    
    // 行内元素
    inlineElements_ = {
        "span", "a", "strong", "em", "b", "i", "u", "s", "small", "mark",
        "del", "ins", "sub", "sup", "code", "kbd", "samp", "var", "time",
        "abbr", "dfn", "q", "cite", "ruby", "rt", "rp", "bdi", "bdo"
    };
    
    // 单标签元素（void elements）
    voidElements_ = {
        "area", "base", "br", "col", "embed", "hr", "img", "input",
        "link", "meta", "param", "source", "track", "wbr"
    };
}

bool HTMLElementRecognizer::isHTMLElement(const std::string& tagName) const {
    return htmlElements_.find(tagName) != htmlElements_.end();
}

bool HTMLElementRecognizer::isBlockElement(const std::string& tagName) const {
    return blockElements_.find(tagName) != blockElements_.end();
}

bool HTMLElementRecognizer::isInlineElement(const std::string& tagName) const {
    return inlineElements_.find(tagName) != inlineElements_.end();
}

bool HTMLElementRecognizer::isVoidElement(const std::string& tagName) const {
    return voidElements_.find(tagName) != voidElements_.end();
}

std::vector<std::string> HTMLElementRecognizer::getAllHTMLElements() const {
    return std::vector<std::string>(htmlElements_.begin(), htmlElements_.end());
}

// ===== CSSPropertyRecognizer 实现 =====

CSSPropertyRecognizer::CSSPropertyRecognizer() {
    initializeCSSProperties();
}

void CSSPropertyRecognizer::initializeCSSProperties() {
    // 常用CSS属性
    cssProperties_ = {
        // 布局
        "display", "position", "top", "right", "bottom", "left", "float", "clear",
        "width", "height", "min-width", "min-height", "max-width", "max-height",
        "margin", "margin-top", "margin-right", "margin-bottom", "margin-left",
        "padding", "padding-top", "padding-right", "padding-bottom", "padding-left",
        "border", "border-width", "border-style", "border-color", "border-radius",
        "box-sizing", "overflow", "overflow-x", "overflow-y", "z-index",
        
        // 弹性布局
        "flex", "flex-direction", "flex-wrap", "flex-flow", "justify-content",
        "align-items", "align-content", "align-self", "flex-grow", "flex-shrink", "flex-basis",
        
        // 网格布局
        "grid", "grid-template-columns", "grid-template-rows", "grid-template-areas",
        "grid-column", "grid-row", "grid-area", "gap", "row-gap", "column-gap",
        
        // 文本
        "color", "font", "font-family", "font-size", "font-weight", "font-style",
        "line-height", "text-align", "text-decoration", "text-transform", "text-indent",
        "letter-spacing", "word-spacing", "white-space", "text-overflow",
        
        // 背景
        "background", "background-color", "background-image", "background-repeat",
        "background-position", "background-size", "background-attachment",
        
        // 变换和动画
        "transform", "transform-origin", "transition", "animation", "opacity",
        
        // 其他
        "cursor", "visibility", "content", "list-style", "outline", "resize"
    };
}

bool CSSPropertyRecognizer::isCSSProperty(const std::string& property) const {
    return cssProperties_.find(property) != cssProperties_.end();
}

std::vector<std::string> CSSPropertyRecognizer::getAllCSSProperties() const {
    return std::vector<std::string>(cssProperties_.begin(), cssProperties_.end());
}

// ===== CompleteCHTLTokenUtils 实现 =====

bool CompleteCHTLTokenUtils::isIdentifierStart(char ch) {
    return std::isalpha(ch) || ch == '_' || ch == '$';
}

bool CompleteCHTLTokenUtils::isIdentifierChar(char ch) {
    return std::isalnum(ch) || ch == '_' || ch == '$' || ch == '-';
}

bool CompleteCHTLTokenUtils::isWhitespace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\r';
}

bool CompleteCHTLTokenUtils::isNewline(char ch) {
    return ch == '\n';
}

bool CompleteCHTLTokenUtils::isStringDelimiter(char ch) {
    return ch == '"' || ch == '\'';
}

bool CompleteCHTLTokenUtils::isDigit(char ch) {
    return std::isdigit(ch);
}

bool CompleteCHTLTokenUtils::isAlpha(char ch) {
    return std::isalpha(ch);
}

bool CompleteCHTLTokenUtils::isAlphaNumeric(char ch) {
    return std::isalnum(ch);
}

std::string CompleteCHTLTokenUtils::unescapeString(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            switch (str[i + 1]) {
                case 'n': result += '\n'; ++i; break;
                case 't': result += '\t'; ++i; break;
                case 'r': result += '\r'; ++i; break;
                case '\\': result += '\\'; ++i; break;
                case '"': result += '"'; ++i; break;
                case '\'': result += '\''; ++i; break;
                default: result += str[i]; break;
            }
        } else {
            result += str[i];
        }
    }
    return result;
}

std::string CompleteCHTLTokenUtils::escapeString(const std::string& str) {
    std::string result;
    for (char ch : str) {
        switch (ch) {
            case '\n': result += "\\n"; break;
            case '\t': result += "\\t"; break;
            case '\r': result += "\\r"; break;
            case '\\': result += "\\\\"; break;
            case '"': result += "\\\""; break;
            case '\'': result += "\\'"; break;
            default: result += ch; break;
        }
    }
    return result;
}

std::string CompleteCHTLTokenUtils::normalizeUnquotedString(const std::string& str) {
    // 标准化无引号字符串（CHTL特色）
    std::string result = str;
    // 移除前后空白
    result.erase(0, result.find_first_not_of(" \t\r\n"));
    result.erase(result.find_last_not_of(" \t\r\n") + 1);
    return result;
}

std::string CompleteCHTLTokenUtils::tokenTypeToString(CompleteCHTLTokenType type) {
    switch (type) {
        case CompleteCHTLTokenType::IDENTIFIER: return "IDENTIFIER";
        case CompleteCHTLTokenType::STRING_DOUBLE: return "STRING_DOUBLE";
        case CompleteCHTLTokenType::STRING_SINGLE: return "STRING_SINGLE";
        case CompleteCHTLTokenType::STRING_UNQUOTED: return "STRING_UNQUOTED";
        case CompleteCHTLTokenType::LBRACE: return "LBRACE";
        case CompleteCHTLTokenType::RBRACE: return "RBRACE";
        case CompleteCHTLTokenType::LBRACKET: return "LBRACKET";
        case CompleteCHTLTokenType::RBRACKET: return "RBRACKET";
        case CompleteCHTLTokenType::LPAREN: return "LPAREN";
        case CompleteCHTLTokenType::RPAREN: return "RPAREN";
        case CompleteCHTLTokenType::SEMICOLON: return "SEMICOLON";
        case CompleteCHTLTokenType::COLON: return "COLON";
        case CompleteCHTLTokenType::EQUALS: return "EQUALS";
        case CompleteCHTLTokenType::COLON_EQUALS: return "COLON_EQUALS";
        case CompleteCHTLTokenType::COMMA: return "COMMA";
        case CompleteCHTLTokenType::DOT: return "DOT";
        case CompleteCHTLTokenType::SLASH: return "SLASH";
        case CompleteCHTLTokenType::HASH: return "HASH";
        case CompleteCHTLTokenType::AMPERSAND: return "AMPERSAND";
        case CompleteCHTLTokenType::KEYWORD_TEXT: return "KEYWORD_TEXT";
        case CompleteCHTLTokenType::KEYWORD_STYLE: return "KEYWORD_STYLE";
        case CompleteCHTLTokenType::KEYWORD_SCRIPT: return "KEYWORD_SCRIPT";
        case CompleteCHTLTokenType::KEYWORD_INHERIT: return "KEYWORD_INHERIT";
        case CompleteCHTLTokenType::KEYWORD_DELETE: return "KEYWORD_DELETE";
        case CompleteCHTLTokenType::KEYWORD_INSERT: return "KEYWORD_INSERT";
        case CompleteCHTLTokenType::KEYWORD_AFTER: return "KEYWORD_AFTER";
        case CompleteCHTLTokenType::KEYWORD_BEFORE: return "KEYWORD_BEFORE";
        case CompleteCHTLTokenType::KEYWORD_REPLACE: return "KEYWORD_REPLACE";
        case CompleteCHTLTokenType::KEYWORD_AT_TOP: return "KEYWORD_AT_TOP";
        case CompleteCHTLTokenType::KEYWORD_AT_BOTTOM: return "KEYWORD_AT_BOTTOM";
        case CompleteCHTLTokenType::KEYWORD_FROM: return "KEYWORD_FROM";
        case CompleteCHTLTokenType::KEYWORD_AS: return "KEYWORD_AS";
        case CompleteCHTLTokenType::KEYWORD_EXCEPT: return "KEYWORD_EXCEPT";
        case CompleteCHTLTokenType::KEYWORD_TEMPLATE: return "KEYWORD_TEMPLATE";
        case CompleteCHTLTokenType::KEYWORD_CUSTOM: return "KEYWORD_CUSTOM";
        case CompleteCHTLTokenType::KEYWORD_ORIGIN: return "KEYWORD_ORIGIN";
        case CompleteCHTLTokenType::KEYWORD_IMPORT: return "KEYWORD_IMPORT";
        case CompleteCHTLTokenType::KEYWORD_NAMESPACE: return "KEYWORD_NAMESPACE";
        case CompleteCHTLTokenType::KEYWORD_CONFIGURATION: return "KEYWORD_CONFIGURATION";
        case CompleteCHTLTokenType::KEYWORD_INFO: return "KEYWORD_INFO";
        case CompleteCHTLTokenType::KEYWORD_EXPORT: return "KEYWORD_EXPORT";
        case CompleteCHTLTokenType::KEYWORD_NAME: return "KEYWORD_NAME";
        case CompleteCHTLTokenType::PREFIX_STYLE: return "PREFIX_STYLE";
        case CompleteCHTLTokenType::PREFIX_ELEMENT: return "PREFIX_ELEMENT";
        case CompleteCHTLTokenType::PREFIX_VAR: return "PREFIX_VAR";
        case CompleteCHTLTokenType::PREFIX_HTML: return "PREFIX_HTML";
        case CompleteCHTLTokenType::PREFIX_JAVASCRIPT: return "PREFIX_JAVASCRIPT";
        case CompleteCHTLTokenType::PREFIX_CHTL: return "PREFIX_CHTL";
        case CompleteCHTLTokenType::PREFIX_CJMOD: return "PREFIX_CJMOD";
        case CompleteCHTLTokenType::COMMENT_LINE: return "COMMENT_LINE";
        case CompleteCHTLTokenType::COMMENT_MULTILINE: return "COMMENT_MULTILINE";
        case CompleteCHTLTokenType::COMMENT_GENERATOR: return "COMMENT_GENERATOR";
        case CompleteCHTLTokenType::HTML_TAG: return "HTML_TAG";
        case CompleteCHTLTokenType::CSS_PROPERTY: return "CSS_PROPERTY";
        case CompleteCHTLTokenType::CSS_VALUE: return "CSS_VALUE";
        case CompleteCHTLTokenType::CSS_SELECTOR_CLASS: return "CSS_SELECTOR_CLASS";
        case CompleteCHTLTokenType::CSS_SELECTOR_ID: return "CSS_SELECTOR_ID";
        case CompleteCHTLTokenType::CSS_PSEUDO_CLASS: return "CSS_PSEUDO_CLASS";
        case CompleteCHTLTokenType::CSS_PSEUDO_ELEMENT: return "CSS_PSEUDO_ELEMENT";
        case CompleteCHTLTokenType::CHTL_JS_SELECTOR: return "CHTL_JS_SELECTOR";
        case CompleteCHTLTokenType::CHTL_JS_ARROW: return "CHTL_JS_ARROW";
        case CompleteCHTLTokenType::CHTL_JS_LISTEN: return "CHTL_JS_LISTEN";
        case CompleteCHTLTokenType::CHTL_JS_DELEGATE: return "CHTL_JS_DELEGATE";
        case CompleteCHTLTokenType::CHTL_JS_ANIMATE: return "CHTL_JS_ANIMATE";
        case CompleteCHTLTokenType::INDEX_ACCESS: return "INDEX_ACCESS";
        case CompleteCHTLTokenType::WHITESPACE: return "WHITESPACE";
        case CompleteCHTLTokenType::NEWLINE: return "NEWLINE";
        case CompleteCHTLTokenType::TAB: return "TAB";
        case CompleteCHTLTokenType::EOF_TOKEN: return "EOF_TOKEN";
        case CompleteCHTLTokenType::UNKNOWN: return "UNKNOWN";
        case CompleteCHTLTokenType::ERROR_TOKEN: return "ERROR_TOKEN";
        default: return "UNKNOWN";
    }
}

bool CompleteCHTLTokenUtils::isLiteralType(CompleteCHTLTokenType type) {
    return type == CompleteCHTLTokenType::IDENTIFIER ||
           type == CompleteCHTLTokenType::STRING_DOUBLE ||
           type == CompleteCHTLTokenType::STRING_SINGLE ||
           type == CompleteCHTLTokenType::STRING_UNQUOTED;
}

bool CompleteCHTLTokenUtils::isKeywordType(CompleteCHTLTokenType type) {
    return static_cast<int>(type) >= static_cast<int>(CompleteCHTLTokenType::KEYWORD_TEXT) &&
           static_cast<int>(type) <= static_cast<int>(CompleteCHTLTokenType::KEYWORD_NAME);
}

bool CompleteCHTLTokenUtils::isOperatorType(CompleteCHTLTokenType type) {
    return type == CompleteCHTLTokenType::COLON ||
           type == CompleteCHTLTokenType::EQUALS ||
           type == CompleteCHTLTokenType::COLON_EQUALS ||
           type == CompleteCHTLTokenType::DOT ||
           type == CompleteCHTLTokenType::HASH ||
           type == CompleteCHTLTokenType::AMPERSAND ||
           type == CompleteCHTLTokenType::CHTL_JS_ARROW;
}

bool CompleteCHTLTokenUtils::isSeparatorType(CompleteCHTLTokenType type) {
    return type == CompleteCHTLTokenType::LBRACE ||
           type == CompleteCHTLTokenType::RBRACE ||
           type == CompleteCHTLTokenType::LBRACKET ||
           type == CompleteCHTLTokenType::RBRACKET ||
           type == CompleteCHTLTokenType::LPAREN ||
           type == CompleteCHTLTokenType::RPAREN ||
           type == CompleteCHTLTokenType::SEMICOLON ||
           type == CompleteCHTLTokenType::COMMA ||
           type == CompleteCHTLTokenType::SLASH;
}

bool CompleteCHTLTokenUtils::isCommentType(CompleteCHTLTokenType type) {
    return type == CompleteCHTLTokenType::COMMENT_LINE ||
           type == CompleteCHTLTokenType::COMMENT_MULTILINE ||
           type == CompleteCHTLTokenType::COMMENT_GENERATOR;
}

bool CompleteCHTLTokenUtils::isWhitespaceType(CompleteCHTLTokenType type) {
    return type == CompleteCHTLTokenType::WHITESPACE ||
           type == CompleteCHTLTokenType::NEWLINE ||
           type == CompleteCHTLTokenType::TAB;
}

bool CompleteCHTLTokenUtils::startsMultipleCharSequence(char ch) {
    return ch == '/' || ch == '*' || ch == '-' || ch == ':' || ch == '=' || 
           ch == '[' || ch == '{' || ch == '<';
}

std::string CompleteCHTLTokenUtils::getMultipleCharSequence(const std::string& source, size_t pos) {
    if (pos >= source.length()) return "";
    
    // 检查多字符序列
    if (pos + 1 < source.length()) {
        std::string twoChar = source.substr(pos, 2);
        if (twoChar == "//" || twoChar == "/*" || twoChar == "*/" || 
            twoChar == "--" || twoChar == "::" || twoChar == "->") {
            return twoChar;
        }
    }
    
    // 检查特殊关键字序列
    if (source.substr(pos, 6) == "at top" && (pos + 6 >= source.length() || isWhitespace(source[pos + 6]))) {
        return "at top";
    }
    if (source.substr(pos, 9) == "at bottom" && (pos + 9 >= source.length() || isWhitespace(source[pos + 9]))) {
        return "at bottom";
    }
    
    return std::string(1, source[pos]);
}

bool CompleteCHTLTokenUtils::isCEEquivalent(char ch) {
    return ch == ':' || ch == '=';
}

CompleteCHTLTokenType CompleteCHTLTokenUtils::getCETokenType() {
    return CompleteCHTLTokenType::COLON_EQUALS;
}

bool CompleteCHTLTokenUtils::isValidInContext(CompleteCHTLTokenType tokenType, const std::string& context) {
    // 根据上下文验证token是否有效
    if (context == "style") {
        return tokenType == CompleteCHTLTokenType::CSS_PROPERTY ||
               tokenType == CompleteCHTLTokenType::CSS_VALUE ||
               tokenType == CompleteCHTLTokenType::CSS_SELECTOR_CLASS ||
               tokenType == CompleteCHTLTokenType::CSS_SELECTOR_ID ||
               tokenType == CompleteCHTLTokenType::AMPERSAND ||
               isLiteralType(tokenType) || isSeparatorType(tokenType);
    }
    
    if (context == "script") {
        return tokenType == CompleteCHTLTokenType::CHTL_JS_SELECTOR ||
               tokenType == CompleteCHTLTokenType::CHTL_JS_ARROW ||
               tokenType == CompleteCHTLTokenType::CHTL_JS_LISTEN ||
               tokenType == CompleteCHTLTokenType::CHTL_JS_DELEGATE ||
               tokenType == CompleteCHTLTokenType::CHTL_JS_ANIMATE ||
               isLiteralType(tokenType) || isSeparatorType(tokenType);
    }
    
    return true; // 默认情况下允许所有token
}

std::vector<CompleteCHTLTokenType> CompleteCHTLTokenUtils::getValidTokensInContext(const std::string& context) {
    std::vector<CompleteCHTLTokenType> validTokens;
    
    if (context == "style") {
        validTokens = {
            CompleteCHTLTokenType::CSS_PROPERTY,
            CompleteCHTLTokenType::CSS_VALUE,
            CompleteCHTLTokenType::CSS_SELECTOR_CLASS,
            CompleteCHTLTokenType::CSS_SELECTOR_ID,
            CompleteCHTLTokenType::CSS_PSEUDO_CLASS,
            CompleteCHTLTokenType::CSS_PSEUDO_ELEMENT,
            CompleteCHTLTokenType::AMPERSAND
        };
    } else if (context == "script") {
        validTokens = {
            CompleteCHTLTokenType::CHTL_JS_SELECTOR,
            CompleteCHTLTokenType::CHTL_JS_ARROW,
            CompleteCHTLTokenType::CHTL_JS_LISTEN,
            CompleteCHTLTokenType::CHTL_JS_DELEGATE,
            CompleteCHTLTokenType::CHTL_JS_ANIMATE
        };
    }
    
    return validTokens;
}

bool CompleteCHTLTokenUtils::isIndexAccess(const std::string& str) {
    std::regex indexRegex(R"(\[\d+\])");
    return std::regex_match(str, indexRegex);
}

int CompleteCHTLTokenUtils::parseIndexAccess(const std::string& str) {
    std::regex indexRegex(R"(\[(\d+)\])");
    std::smatch match;
    if (std::regex_match(str, match, indexRegex)) {
        return std::stoi(match[1].str());
    }
    return -1;
}

bool CompleteCHTLTokenUtils::isCHTLJSSelector(const std::string& str) {
    return str.length() >= 4 && str.substr(0, 2) == "{{" && str.substr(str.length() - 2) == "}}";
}

std::string CompleteCHTLTokenUtils::parseCHTLJSSelector(const std::string& str) {
    if (isCHTLJSSelector(str)) {
        return str.substr(2, str.length() - 4);
    }
    return "";
}

bool CompleteCHTLTokenUtils::isConfigurationValue(const std::string& str) {
    // 检查是否为配置值（数字、布尔值、字符串等）
    return str == "true" || str == "false" || 
           std::regex_match(str, std::regex(R"(\d+)")) ||
           std::regex_match(str, std::regex(R"(".*")"));
}

std::pair<std::string, std::string> CompleteCHTLTokenUtils::parseConfigurationAssignment(const std::string& str) {
    std::regex assignRegex(R"((\w+)\s*=\s*(.+))");
    std::smatch match;
    if (std::regex_match(str, match, assignRegex)) {
        return {match[1].str(), match[2].str()};
    }
    return {"", ""};
}

// ===== CompleteCHTLTokenValidator 实现 =====

bool CompleteCHTLTokenValidator::validateTokenSequence(const std::vector<CompleteCHTLToken>& tokens) {
    if (tokens.empty()) return true;
    
    for (size_t i = 0; i < tokens.size() - 1; ++i) {
        if (!isValidTokenTransition(tokens[i].type, tokens[i + 1].type)) {
            return false;
        }
    }
    return true;
}

bool CompleteCHTLTokenValidator::validateTemplateStructure(const std::vector<CompleteCHTLToken>& tokens) {
    // 验证 [Template] @Type Name { ... } 结构
    if (tokens.size() < 4) return false;
    
    return tokens[0].type == CompleteCHTLTokenType::KEYWORD_TEMPLATE &&
           (tokens[1].type == CompleteCHTLTokenType::PREFIX_STYLE ||
            tokens[1].type == CompleteCHTLTokenType::PREFIX_ELEMENT ||
            tokens[1].type == CompleteCHTLTokenType::PREFIX_VAR) &&
           tokens[2].type == CompleteCHTLTokenType::IDENTIFIER &&
           tokens[3].type == CompleteCHTLTokenType::LBRACE;
}

bool CompleteCHTLTokenValidator::validateCustomStructure(const std::vector<CompleteCHTLToken>& tokens) {
    // 验证 [Custom] @Type Name { ... } 结构
    if (tokens.size() < 4) return false;
    
    return tokens[0].type == CompleteCHTLTokenType::KEYWORD_CUSTOM &&
           (tokens[1].type == CompleteCHTLTokenType::PREFIX_STYLE ||
            tokens[1].type == CompleteCHTLTokenType::PREFIX_ELEMENT ||
            tokens[1].type == CompleteCHTLTokenType::PREFIX_VAR) &&
           tokens[2].type == CompleteCHTLTokenType::IDENTIFIER &&
           tokens[3].type == CompleteCHTLTokenType::LBRACE;
}

bool CompleteCHTLTokenValidator::validateImportStructure(const std::vector<CompleteCHTLToken>& tokens) {
    // 验证 [Import] @Type from path [as alias] 结构
    if (tokens.size() < 4) return false;
    
    return tokens[0].type == CompleteCHTLTokenType::KEYWORD_IMPORT &&
           (tokens[1].type == CompleteCHTLTokenType::PREFIX_HTML ||
            tokens[1].type == CompleteCHTLTokenType::PREFIX_STYLE ||
            tokens[1].type == CompleteCHTLTokenType::PREFIX_JAVASCRIPT ||
            tokens[1].type == CompleteCHTLTokenType::PREFIX_CHTL ||
            tokens[1].type == CompleteCHTLTokenType::PREFIX_CJMOD) &&
           tokens[2].type == CompleteCHTLTokenType::KEYWORD_FROM;
}

bool CompleteCHTLTokenValidator::validateNamespaceStructure(const std::vector<CompleteCHTLToken>& tokens) {
    // 验证 [Namespace] name 结构
    if (tokens.size() < 2) return false;
    
    return tokens[0].type == CompleteCHTLTokenType::KEYWORD_NAMESPACE &&
           tokens[1].type == CompleteCHTLTokenType::IDENTIFIER;
}

bool CompleteCHTLTokenValidator::validateElementStructure(const std::vector<CompleteCHTLToken>& tokens) {
    // 验证HTML元素结构
    if (tokens.empty()) return false;
    
    return tokens[0].type == CompleteCHTLTokenType::HTML_TAG ||
           tokens[0].type == CompleteCHTLTokenType::IDENTIFIER;
}

bool CompleteCHTLTokenValidator::validateStyleBlockStructure(const std::vector<CompleteCHTLToken>& tokens) {
    // 验证 style { ... } 结构
    if (tokens.size() < 3) return false;
    
    return tokens[0].type == CompleteCHTLTokenType::KEYWORD_STYLE &&
           tokens[1].type == CompleteCHTLTokenType::LBRACE;
}

bool CompleteCHTLTokenValidator::validateScriptBlockStructure(const std::vector<CompleteCHTLToken>& tokens) {
    // 验证 script { ... } 结构
    if (tokens.size() < 3) return false;
    
    return tokens[0].type == CompleteCHTLTokenType::KEYWORD_SCRIPT &&
           tokens[1].type == CompleteCHTLTokenType::LBRACE;
}

bool CompleteCHTLTokenValidator::validateExceptConstraints(const std::vector<CompleteCHTLToken>& tokens) {
    // 验证 except 约束语法
    if (tokens.empty()) return false;
    
    return tokens[0].type == CompleteCHTLTokenType::KEYWORD_EXCEPT;
}

bool CompleteCHTLTokenValidator::validateConfigurationBlock(const std::vector<CompleteCHTLToken>& tokens) {
    // 验证 [Configuration] { ... } 结构
    if (tokens.size() < 3) return false;
    
    return tokens[0].type == CompleteCHTLTokenType::KEYWORD_CONFIGURATION &&
           tokens[1].type == CompleteCHTLTokenType::LBRACE;
}

bool CompleteCHTLTokenValidator::validateNameBlock(const std::vector<CompleteCHTLToken>& tokens) {
    // 验证 [Name] { ... } 结构
    if (tokens.size() < 3) return false;
    
    return tokens[0].type == CompleteCHTLTokenType::KEYWORD_NAME &&
           tokens[1].type == CompleteCHTLTokenType::LBRACE;
}

bool CompleteCHTLTokenValidator::isValidTokenTransition(CompleteCHTLTokenType from, CompleteCHTLTokenType to) {
    // 简化的token转换验证
    // 在实际实现中，这里应该包含完整的语法规则
    
    // 跳过空白字符的验证
    if (CompleteCHTLTokenUtils::isWhitespaceType(to)) {
        return true;
    }
    
    // 基本的语法规则检查
    if (from == CompleteCHTLTokenType::LBRACE) {
        return true; // 左大括号后可以跟任何内容
    }
    
    if (to == CompleteCHTLTokenType::RBRACE) {
        return true; // 右大括号可以结束任何块
    }
    
    return true; // 简化实现，实际应该更严格
}

bool CompleteCHTLTokenValidator::isValidInStructure(CompleteCHTLTokenType tokenType, const std::string& structure) {
    if (structure == "template") {
        return tokenType == CompleteCHTLTokenType::PREFIX_STYLE ||
               tokenType == CompleteCHTLTokenType::PREFIX_ELEMENT ||
               tokenType == CompleteCHTLTokenType::PREFIX_VAR ||
               CompleteCHTLTokenUtils::isLiteralType(tokenType);
    }
    
    if (structure == "custom") {
        return tokenType == CompleteCHTLTokenType::PREFIX_STYLE ||
               tokenType == CompleteCHTLTokenType::PREFIX_ELEMENT ||
               tokenType == CompleteCHTLTokenType::PREFIX_VAR ||
               tokenType == CompleteCHTLTokenType::KEYWORD_DELETE ||
               tokenType == CompleteCHTLTokenType::KEYWORD_INSERT ||
               CompleteCHTLTokenUtils::isLiteralType(tokenType);
    }
    
    return true;
}

} // namespace chtl