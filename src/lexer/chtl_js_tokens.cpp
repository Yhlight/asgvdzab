#include "lexer/chtl_js_tokens.hpp"
#include <cctype>
#include <algorithm>
#include <sstream>
#include <regex>

namespace chtl {

// CHTLJSToken 实现
void CHTLJSToken::updateSemanticInfo() {
    semantic.isKeyword = CHTLJSTokenUtils::isKeywordType(type);
    semantic.isLiteral = CHTLJSTokenUtils::isLiteralType(type);
    semantic.isOperator = CHTLJSTokenUtils::isOperatorType(type);
    semantic.isSeparator = CHTLJSTokenUtils::isSeparatorType(type);
    semantic.isComment = CHTLJSTokenUtils::isCommentType(type);
    semantic.isWhitespace = CHTLJSTokenUtils::isWhitespaceType(type);
    semantic.isCHTLJSSpecific = CHTLJSTokenUtils::isCHTLJSSpecificType(type);
    semantic.isEnhancedSelector = (type == CHTLJSTokenType::ENHANCED_SELECTOR_START || 
                                   type == CHTLJSTokenType::ENHANCED_SELECTOR_END);
}

bool CHTLJSToken::isCHTLJSSpecific() const {
    return semantic.isCHTLJSSpecific;
}

bool CHTLJSToken::isEnhancedSelector() const {
    return semantic.isEnhancedSelector;
}

bool CHTLJSToken::isLiteral() const {
    return semantic.isLiteral;
}

bool CHTLJSToken::isKeyword() const {
    return semantic.isKeyword;
}

bool CHTLJSToken::isOperator() const {
    return semantic.isOperator;
}

bool CHTLJSToken::isSeparator() const {
    return semantic.isSeparator;
}

bool CHTLJSToken::isComment() const {
    return semantic.isComment;
}

bool CHTLJSToken::isWhitespace() const {
    return semantic.isWhitespace;
}

std::string CHTLJSToken::toString() const {
    std::ostringstream oss;
    oss << "CHTLJSToken{type=" << CHTLJSTokenUtils::tokenTypeToString(type)
        << ", value=\"" << value << "\""
        << ", pos=" << position.line << ":" << position.column;
    if (semantic.isCHTLJSSpecific) {
        oss << ", CHTL_SPECIFIC";
    }
    oss << "}";
    return oss.str();
}

// EnhancedSelector 实现
EnhancedSelector::EnhancedSelector(const std::string& sel, Position pos) 
    : selector(sel), position(pos) {
    parse();
}

void EnhancedSelector::parse() {
    std::string trimmed = selector;
    // 移除前后空白
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);
    
    if (trimmed.empty()) {
        type = EnhancedSelectorType::AUTO_DETECT;
        return;
    }
    
    // 初始化类型
    type = EnhancedSelectorType::AUTO_DETECT;
    
    // 检查索引访问 element[0]
    size_t bracketPos = trimmed.find('[');
    if (bracketPos != std::string::npos) {
        size_t closeBracket = trimmed.find(']', bracketPos);
        if (closeBracket != std::string::npos) {
            std::string indexStr = trimmed.substr(bracketPos + 1, closeBracket - bracketPos - 1);
            parsed.index = std::stoi(indexStr);
            parsed.hasIndex = true;
            trimmed = trimmed.substr(0, bracketPos); // 移除索引部分
            type = EnhancedSelectorType::INDEXED;
        }
    }
    
    // 解析基础选择器部分（不覆盖INDEXED类型）
    if (trimmed[0] == '.') {
        // 类选择器
        if (type != EnhancedSelectorType::INDEXED) {
            type = EnhancedSelectorType::CLASS_NAME;
        }
        parsed.className = trimmed.substr(1);
        
        // 检查后代选择器
        size_t spacePos = parsed.className.find(' ');
        if (spacePos != std::string::npos) {
            if (type != EnhancedSelectorType::INDEXED) {
                type = EnhancedSelectorType::DESCENDANT;
            }
            std::string rest = parsed.className.substr(spacePos + 1);
            parsed.className = parsed.className.substr(0, spacePos);
            
            // 解析后代路径
            std::istringstream iss(rest);
            std::string token;
            while (iss >> token) {
                parsed.descendantPath.push_back(token);
            }
        }
    } else if (trimmed[0] == '#') {
        // ID选择器
        if (type != EnhancedSelectorType::INDEXED) {
            type = EnhancedSelectorType::ID_NAME;
        }
        parsed.idName = trimmed.substr(1);
        
        // 检查后代选择器
        size_t spacePos = parsed.idName.find(' ');
        if (spacePos != std::string::npos) {
            if (type != EnhancedSelectorType::INDEXED) {
                type = EnhancedSelectorType::DESCENDANT;
            }
            std::string rest = parsed.idName.substr(spacePos + 1);
            parsed.idName = parsed.idName.substr(0, spacePos);
            
            // 解析后代路径
            std::istringstream iss(rest);
            std::string token;
            while (iss >> token) {
                parsed.descendantPath.push_back(token);
            }
        }
    } else {
        // 检查是否包含空格（后代选择器）
        size_t spacePos = trimmed.find(' ');
        if (spacePos != std::string::npos) {
            if (type != EnhancedSelectorType::INDEXED) {
                type = EnhancedSelectorType::DESCENDANT;
            }
            parsed.tagName = trimmed.substr(0, spacePos);
            
            std::string rest = trimmed.substr(spacePos + 1);
            std::istringstream iss(rest);
            std::string token;
            while (iss >> token) {
                parsed.descendantPath.push_back(token);
            }
        } else {
            // 标签名或自动检测
            if (type != EnhancedSelectorType::INDEXED) {
                type = EnhancedSelectorType::AUTO_DETECT;
            }
            parsed.tagName = trimmed;
        }
    }
}

std::string EnhancedSelector::toCSSSelector() const {
    std::string result;
    
    switch (type) {
        case EnhancedSelectorType::TAG_NAME:
            result = parsed.tagName;
            break;
        case EnhancedSelectorType::CLASS_NAME:
            result = "." + parsed.className;
            break;
        case EnhancedSelectorType::ID_NAME:
            result = "#" + parsed.idName;
            break;
        case EnhancedSelectorType::DESCENDANT:
            if (!parsed.tagName.empty()) {
                result = parsed.tagName;
            } else if (!parsed.className.empty()) {
                result = "." + parsed.className;
            } else if (!parsed.idName.empty()) {
                result = "#" + parsed.idName;
            }
            for (const auto& desc : parsed.descendantPath) {
                result += " " + desc;
            }
            break;
        case EnhancedSelectorType::INDEXED:
            // 对于索引选择器，返回基础选择器（不递归调用避免无限循环）
            if (!parsed.tagName.empty()) {
                result = parsed.tagName;
            } else if (!parsed.className.empty()) {
                result = "." + parsed.className;
            } else if (!parsed.idName.empty()) {
                result = "#" + parsed.idName;
            }
            break;
        case EnhancedSelectorType::AUTO_DETECT:
            result = parsed.tagName; // 作为标签名处理
            break;
    }
    
    return result;
}

std::string EnhancedSelector::toDOMQuery() const {
    std::string cssSelector = toCSSSelector();
    
    if (parsed.hasIndex) {
        return "document.querySelectorAll('" + cssSelector + "')[" + 
               std::to_string(parsed.index) + "]";
    } else {
        if (type == EnhancedSelectorType::ID_NAME) {
            return "document.getElementById('" + parsed.idName + "')";
        } else {
            return "document.querySelector('" + cssSelector + "')";
        }
    }
}

std::string EnhancedSelector::toString() const {
    std::ostringstream oss;
    oss << "EnhancedSelector{";
    oss << "type=" << static_cast<int>(type);
    oss << ", selector=\"" << selector << "\"";
    if (parsed.hasIndex) {
        oss << ", index=" << parsed.index;
    }
    oss << "}";
    return oss.str();
}

// CHTLJSKeywordMap 实现
CHTLJSKeywordMap::CHTLJSKeywordMap() {
    initializeDefaultKeywords();
}

void CHTLJSKeywordMap::initializeDefaultKeywords() {
    // CHTL JS特有关键字
    keywordMap_["listen"] = CHTLJSTokenType::KEYWORD_LISTEN;
    keywordMap_["delegate"] = CHTLJSTokenType::KEYWORD_DELEGATE;
    keywordMap_["animate"] = CHTLJSTokenType::KEYWORD_ANIMATE;
    
    // 标准JavaScript关键字
    keywordMap_["function"] = CHTLJSTokenType::KEYWORD_FUNCTION;
    keywordMap_["var"] = CHTLJSTokenType::KEYWORD_VAR;
    keywordMap_["let"] = CHTLJSTokenType::KEYWORD_LET;
    keywordMap_["const"] = CHTLJSTokenType::KEYWORD_CONST;
    keywordMap_["if"] = CHTLJSTokenType::KEYWORD_IF;
    keywordMap_["else"] = CHTLJSTokenType::KEYWORD_ELSE;
    keywordMap_["for"] = CHTLJSTokenType::KEYWORD_FOR;
    keywordMap_["while"] = CHTLJSTokenType::KEYWORD_WHILE;
    keywordMap_["return"] = CHTLJSTokenType::KEYWORD_RETURN;
    keywordMap_["true"] = CHTLJSTokenType::KEYWORD_TRUE;
    keywordMap_["false"] = CHTLJSTokenType::KEYWORD_FALSE;
    keywordMap_["null"] = CHTLJSTokenType::KEYWORD_NULL;
    keywordMap_["undefined"] = CHTLJSTokenType::KEYWORD_UNDEFINED;
    
    // 动画相关关键字
    keywordMap_["duration"] = CHTLJSTokenType::KEYWORD_DURATION;
    keywordMap_["easing"] = CHTLJSTokenType::KEYWORD_EASING;
    keywordMap_["begin"] = CHTLJSTokenType::KEYWORD_BEGIN;
    keywordMap_["when"] = CHTLJSTokenType::KEYWORD_WHEN;
    keywordMap_["end"] = CHTLJSTokenType::KEYWORD_END;
    keywordMap_["at"] = CHTLJSTokenType::KEYWORD_AT;
    keywordMap_["loop"] = CHTLJSTokenType::KEYWORD_LOOP;
    keywordMap_["direction"] = CHTLJSTokenType::KEYWORD_DIRECTION;
    keywordMap_["delay"] = CHTLJSTokenType::KEYWORD_DELAY;
    keywordMap_["callback"] = CHTLJSTokenType::KEYWORD_CALLBACK;
    
    // 事件委托关键字
    keywordMap_["target"] = CHTLJSTokenType::KEYWORD_TARGET;
}

bool CHTLJSKeywordMap::isKeyword(const std::string& word) const {
    return keywordMap_.find(word) != keywordMap_.end();
}

CHTLJSTokenType CHTLJSKeywordMap::getKeywordType(const std::string& word) const {
    auto it = keywordMap_.find(word);
    return (it != keywordMap_.end()) ? it->second : CHTLJSTokenType::UNKNOWN;
}

std::vector<std::string> CHTLJSKeywordMap::getAllKeywords() const {
    std::vector<std::string> keywords;
    for (const auto& [word, type] : keywordMap_) {
        keywords.push_back(word);
    }
    return keywords;
}

bool CHTLJSKeywordMap::isCHTLJSKeyword(const std::string& word) const {
    return word == "listen" || word == "delegate" || word == "animate";
}

bool CHTLJSKeywordMap::isJavaScriptKeyword(const std::string& word) const {
    static const std::vector<std::string> jsKeywords = {
        "function", "var", "let", "const", "if", "else", "for", "while",
        "return", "true", "false", "null", "undefined"
    };
    return std::find(jsKeywords.begin(), jsKeywords.end(), word) != jsKeywords.end();
}

bool CHTLJSKeywordMap::isAnimateKeyword(const std::string& word) const {
    static const std::vector<std::string> animateKeywords = {
        "duration", "easing", "begin", "when", "end", "at", 
        "loop", "direction", "delay", "callback"
    };
    return std::find(animateKeywords.begin(), animateKeywords.end(), word) != animateKeywords.end();
}

bool CHTLJSKeywordMap::isDelegateKeyword(const std::string& word) const {
    return word == "target";
}

// CHTLJSSyntaxDetector 实现
bool CHTLJSSyntaxDetector::hasEnhancedSelector(const std::string& content) {
    return content.find("{{") != std::string::npos && content.find("}}") != std::string::npos;
}

bool CHTLJSSyntaxDetector::hasArrowOperator(const std::string& content) {
    return content.find("->") != std::string::npos;
}

bool CHTLJSSyntaxDetector::hasListenKeyword(const std::string& content) {
    size_t pos = content.find("listen");
    if (pos != std::string::npos) {
        // 检查前面是否为单词边界
        if (pos == 0 || !std::isalnum(content[pos - 1])) {
            // 检查后面是否有 (
            size_t afterPos = pos + 6; // "listen".length()
            while (afterPos < content.length() && std::isspace(content[afterPos])) {
                afterPos++;
            }
            if (afterPos < content.length() && content[afterPos] == '(') {
                return true;
            }
        }
    }
    return false;
}

bool CHTLJSSyntaxDetector::hasDelegateKeyword(const std::string& content) {
    size_t pos = content.find("delegate");
    if (pos != std::string::npos) {
        // 检查前面是否为单词边界
        if (pos == 0 || !std::isalnum(content[pos - 1])) {
            // 检查后面是否有 (
            size_t afterPos = pos + 8; // "delegate".length()
            while (afterPos < content.length() && std::isspace(content[afterPos])) {
                afterPos++;
            }
            if (afterPos < content.length() && content[afterPos] == '(') {
                return true;
            }
        }
    }
    return false;
}

bool CHTLJSSyntaxDetector::hasAnimateKeyword(const std::string& content) {
    size_t pos = content.find("animate");
    if (pos != std::string::npos) {
        // 检查前面是否为单词边界
        if (pos == 0 || !std::isalnum(content[pos - 1])) {
            // 检查后面是否有 (
            size_t afterPos = pos + 7; // "animate".length()
            while (afterPos < content.length() && std::isspace(content[afterPos])) {
                afterPos++;
            }
            if (afterPos < content.length() && content[afterPos] == '(') {
                return true;
            }
        }
    }
    return false;
}

bool CHTLJSSyntaxDetector::hasAnyCHTLJSFeature(const std::string& content) {
    return hasEnhancedSelector(content) || hasArrowOperator(content) || 
           hasListenKeyword(content) || hasDelegateKeyword(content) || 
           hasAnimateKeyword(content);
}

std::vector<EnhancedSelector> CHTLJSSyntaxDetector::extractEnhancedSelectors(const std::string& content) {
    std::vector<EnhancedSelector> selectors;
    
    size_t pos = 0;
    while ((pos = content.find("{{", pos)) != std::string::npos) {
        size_t end = content.find("}}", pos + 2);
        if (end != std::string::npos) {
            std::string selectorContent = content.substr(pos + 2, end - pos - 2);
            Position position{1, static_cast<size_t>(pos + 1), pos}; // 简化的位置计算
            
            EnhancedSelector selector(selectorContent, position);
            selectors.push_back(selector);
            
            pos = end + 2;
        } else {
            break;
        }
    }
    
    return selectors;
}

bool CHTLJSSyntaxDetector::isValidSelectorContent(const std::string& content) {
    // 简单验证：不能为空，不能包含特殊字符
    if (content.empty()) return false;
    
    for (char ch : content) {
        if (!CHTLJSTokenUtils::isValidSelectorChar(ch)) {
            return false;
        }
    }
    
    return true;
}

// CHTLJSTokenUtils 实现
bool CHTLJSTokenUtils::isIdentifierStart(char ch) {
    return std::isalpha(ch) || ch == '_' || ch == '$';
}

bool CHTLJSTokenUtils::isIdentifierChar(char ch) {
    return std::isalnum(ch) || ch == '_' || ch == '$';
}

bool CHTLJSTokenUtils::isWhitespace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\r';
}

bool CHTLJSTokenUtils::isNewline(char ch) {
    return ch == '\n';
}

bool CHTLJSTokenUtils::isStringDelimiter(char ch) {
    return ch == '"' || ch == '\'';
}

bool CHTLJSTokenUtils::isDigit(char ch) {
    return std::isdigit(ch);
}

std::string CHTLJSTokenUtils::unescapeString(const std::string& str) {
    std::string result;
    result.reserve(str.length());
    
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

std::string CHTLJSTokenUtils::escapeString(const std::string& str) {
    std::string result;
    result.reserve(str.length() * 2);
    
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

std::string CHTLJSTokenUtils::tokenTypeToString(CHTLJSTokenType type) {
    switch (type) {
        case CHTLJSTokenType::IDENTIFIER: return "IDENTIFIER";
        case CHTLJSTokenType::STRING_DOUBLE: return "STRING_DOUBLE";
        case CHTLJSTokenType::STRING_SINGLE: return "STRING_SINGLE";
        case CHTLJSTokenType::NUMBER: return "NUMBER";
        case CHTLJSTokenType::LBRACE: return "LBRACE";
        case CHTLJSTokenType::RBRACE: return "RBRACE";
        case CHTLJSTokenType::LBRACKET: return "LBRACKET";
        case CHTLJSTokenType::RBRACKET: return "RBRACKET";
        case CHTLJSTokenType::LPAREN: return "LPAREN";
        case CHTLJSTokenType::RPAREN: return "RPAREN";
        case CHTLJSTokenType::SEMICOLON: return "SEMICOLON";
        case CHTLJSTokenType::COLON: return "COLON";
        case CHTLJSTokenType::COMMA: return "COMMA";
        case CHTLJSTokenType::DOT: return "DOT";
        case CHTLJSTokenType::ARROW_OP: return "ARROW_OP";
        case CHTLJSTokenType::ENHANCED_SELECTOR_START: return "ENHANCED_SELECTOR_START";
        case CHTLJSTokenType::ENHANCED_SELECTOR_END: return "ENHANCED_SELECTOR_END";
        case CHTLJSTokenType::KEYWORD_LISTEN: return "KEYWORD_LISTEN";
        case CHTLJSTokenType::KEYWORD_DELEGATE: return "KEYWORD_DELEGATE";
        case CHTLJSTokenType::KEYWORD_ANIMATE: return "KEYWORD_ANIMATE";
        case CHTLJSTokenType::KEYWORD_FUNCTION: return "KEYWORD_FUNCTION";
        case CHTLJSTokenType::KEYWORD_VAR: return "KEYWORD_VAR";
        case CHTLJSTokenType::KEYWORD_LET: return "KEYWORD_LET";
        case CHTLJSTokenType::KEYWORD_CONST: return "KEYWORD_CONST";
        case CHTLJSTokenType::KEYWORD_IF: return "KEYWORD_IF";
        case CHTLJSTokenType::KEYWORD_ELSE: return "KEYWORD_ELSE";
        case CHTLJSTokenType::KEYWORD_FOR: return "KEYWORD_FOR";
        case CHTLJSTokenType::KEYWORD_WHILE: return "KEYWORD_WHILE";
        case CHTLJSTokenType::KEYWORD_RETURN: return "KEYWORD_RETURN";
        case CHTLJSTokenType::KEYWORD_TRUE: return "KEYWORD_TRUE";
        case CHTLJSTokenType::KEYWORD_FALSE: return "KEYWORD_FALSE";
        case CHTLJSTokenType::KEYWORD_NULL: return "KEYWORD_NULL";
        case CHTLJSTokenType::KEYWORD_UNDEFINED: return "KEYWORD_UNDEFINED";
        case CHTLJSTokenType::KEYWORD_DURATION: return "KEYWORD_DURATION";
        case CHTLJSTokenType::KEYWORD_EASING: return "KEYWORD_EASING";
        case CHTLJSTokenType::KEYWORD_BEGIN: return "KEYWORD_BEGIN";
        case CHTLJSTokenType::KEYWORD_WHEN: return "KEYWORD_WHEN";
        case CHTLJSTokenType::KEYWORD_END: return "KEYWORD_END";
        case CHTLJSTokenType::KEYWORD_AT: return "KEYWORD_AT";
        case CHTLJSTokenType::KEYWORD_LOOP: return "KEYWORD_LOOP";
        case CHTLJSTokenType::KEYWORD_DIRECTION: return "KEYWORD_DIRECTION";
        case CHTLJSTokenType::KEYWORD_DELAY: return "KEYWORD_DELAY";
        case CHTLJSTokenType::KEYWORD_CALLBACK: return "KEYWORD_CALLBACK";
        case CHTLJSTokenType::KEYWORD_TARGET: return "KEYWORD_TARGET";
        case CHTLJSTokenType::COMMENT_LINE: return "COMMENT_LINE";
        case CHTLJSTokenType::COMMENT_MULTILINE: return "COMMENT_MULTILINE";
        case CHTLJSTokenType::WHITESPACE: return "WHITESPACE";
        case CHTLJSTokenType::NEWLINE: return "NEWLINE";
        case CHTLJSTokenType::EOF_TOKEN: return "EOF_TOKEN";
        case CHTLJSTokenType::UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

bool CHTLJSTokenUtils::isLiteralType(CHTLJSTokenType type) {
    return type == CHTLJSTokenType::IDENTIFIER ||
           type == CHTLJSTokenType::STRING_DOUBLE ||
           type == CHTLJSTokenType::STRING_SINGLE ||
           type == CHTLJSTokenType::NUMBER;
}

bool CHTLJSTokenUtils::isKeywordType(CHTLJSTokenType type) {
    return type >= CHTLJSTokenType::KEYWORD_LISTEN && type <= CHTLJSTokenType::KEYWORD_TARGET;
}

bool CHTLJSTokenUtils::isOperatorType(CHTLJSTokenType type) {
    return type == CHTLJSTokenType::COLON ||
           type == CHTLJSTokenType::DOT ||
           type == CHTLJSTokenType::ARROW_OP;
}

bool CHTLJSTokenUtils::isSeparatorType(CHTLJSTokenType type) {
    return type == CHTLJSTokenType::LBRACE ||
           type == CHTLJSTokenType::RBRACE ||
           type == CHTLJSTokenType::LBRACKET ||
           type == CHTLJSTokenType::RBRACKET ||
           type == CHTLJSTokenType::LPAREN ||
           type == CHTLJSTokenType::RPAREN ||
           type == CHTLJSTokenType::SEMICOLON ||
           type == CHTLJSTokenType::COMMA;
}

bool CHTLJSTokenUtils::isCommentType(CHTLJSTokenType type) {
    return type == CHTLJSTokenType::COMMENT_LINE ||
           type == CHTLJSTokenType::COMMENT_MULTILINE;
}

bool CHTLJSTokenUtils::isWhitespaceType(CHTLJSTokenType type) {
    return type == CHTLJSTokenType::WHITESPACE ||
           type == CHTLJSTokenType::NEWLINE;
}

bool CHTLJSTokenUtils::isCHTLJSSpecificType(CHTLJSTokenType type) {
    return type == CHTLJSTokenType::ARROW_OP ||
           type == CHTLJSTokenType::ENHANCED_SELECTOR_START ||
           type == CHTLJSTokenType::ENHANCED_SELECTOR_END ||
           type == CHTLJSTokenType::KEYWORD_LISTEN ||
           type == CHTLJSTokenType::KEYWORD_DELEGATE ||
           type == CHTLJSTokenType::KEYWORD_ANIMATE;
}

bool CHTLJSTokenUtils::startsMultipleCharSequence(char ch) {
    return ch == '/' || ch == '-' || ch == '{'; // "//", "/*", "->", "{{"
}

std::string CHTLJSTokenUtils::getMultipleCharSequence(const std::string& source, size_t pos) {
    if (pos >= source.length()) return "";
    
    char ch = source[pos];
    
    // 检查注释序列
    if (ch == '/' && pos + 1 < source.length()) {
        if (source[pos + 1] == '/') return "//";
        if (source[pos + 1] == '*') return "/*";
    }
    
    // 检查箭头操作符
    if (ch == '-' && pos + 1 < source.length() && source[pos + 1] == '>') {
        return "->";
    }
    
    // 检查增强选择器开始
    if (ch == '{' && pos + 1 < source.length() && source[pos + 1] == '{') {
        return "{{";
    }
    
    return std::string(1, ch);
}

bool CHTLJSTokenUtils::isValidSelectorChar(char ch) {
    return std::isalnum(ch) || ch == '.' || ch == '#' || ch == '-' || ch == '_' || 
           ch == '[' || ch == ']' || ch == ' ';
}

std::string CHTLJSTokenUtils::extractSelectorContent(const std::string& source, size_t start) {
    if (start + 1 >= source.length() || source.substr(start, 2) != "{{") {
        return "";
    }
    
    size_t end = source.find("}}", start + 2);
    if (end == std::string::npos) {
        return "";
    }
    
    return source.substr(start + 2, end - start - 2);
}

bool CHTLJSTokenUtils::isValidNumber(const std::string& str) {
    if (str.empty()) return false;
    
    size_t pos = 0;
    
    // 检查符号
    if (str[pos] == '+' || str[pos] == '-') {
        pos++;
    }
    
    if (pos >= str.length()) return false;
    
    bool hasDigits = false;
    bool hasDot = false;
    
    while (pos < str.length()) {
        if (std::isdigit(str[pos])) {
            hasDigits = true;
        } else if (str[pos] == '.' && !hasDot) {
            hasDot = true;
        } else {
            return false;
        }
        pos++;
    }
    
    return hasDigits;
}

double CHTLJSTokenUtils::parseNumber(const std::string& str) {
    return std::stod(str);
}

// AnimationConfigParser 和 DelegateConfigParser 的实现可以在后续需要时添加
// 这里先提供基本的骨架
AnimationConfigParser::AnimationConfig AnimationConfigParser::parse(const std::string& configStr) {
    AnimationConfig config;
    // TODO: 实现动画配置解析
    return config;
}

std::string AnimationConfigParser::generateAnimationCode(const AnimationConfig& config) {
    // TODO: 实现动画代码生成
    return "";
}

DelegateConfigParser::DelegateConfig DelegateConfigParser::parse(const std::string& configStr) {
    DelegateConfig config;
    // TODO: 实现委托配置解析
    return config;
}

std::string DelegateConfigParser::generateDelegateCode(const DelegateConfig& config) {
    // TODO: 实现委托代码生成
    return "";
}

} // namespace chtl