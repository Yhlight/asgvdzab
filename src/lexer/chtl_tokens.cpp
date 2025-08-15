#include "lexer/chtl_tokens.hpp"
#include <cctype>
#include <algorithm>
#include <sstream>

namespace chtl {

// CHTLToken 实现
void CHTLToken::updateSemanticInfo() {
    semantic.isKeyword = CHTLTokenUtils::isKeywordType(type);
    semantic.isLiteral = CHTLTokenUtils::isLiteralType(type);
    semantic.isOperator = CHTLTokenUtils::isOperatorType(type);
    semantic.isSeparator = CHTLTokenUtils::isSeparatorType(type);
    semantic.isComment = CHTLTokenUtils::isCommentType(type);
    semantic.isWhitespace = CHTLTokenUtils::isWhitespaceType(type);
}

bool CHTLToken::isLiteral() const {
    return semantic.isLiteral;
}

bool CHTLToken::isKeyword() const {
    return semantic.isKeyword;
}

bool CHTLToken::isOperator() const {
    return semantic.isOperator;
}

bool CHTLToken::isSeparator() const {
    return semantic.isSeparator;
}

bool CHTLToken::isComment() const {
    return semantic.isComment;
}

bool CHTLToken::isWhitespace() const {
    return semantic.isWhitespace;
}

std::string CHTLToken::toString() const {
    std::ostringstream oss;
    oss << "CHTLToken{type=" << CHTLTokenUtils::tokenTypeToString(type)
        << ", value=\"" << value << "\""
        << ", pos=" << position.line << ":" << position.column << "}";
    return oss.str();
}

// CHTLKeywordMap 实现
CHTLKeywordMap::CHTLKeywordMap() {
    initializeDefaultKeywords();
}

void CHTLKeywordMap::initializeDefaultKeywords() {
    // 基础关键字
    keywordMap_["text"] = CHTLTokenType::KEYWORD_TEXT;
    keywordMap_["style"] = CHTLTokenType::KEYWORD_STYLE;
    keywordMap_["script"] = CHTLTokenType::KEYWORD_SCRIPT;
    keywordMap_["inherit"] = CHTLTokenType::KEYWORD_INHERIT;
    keywordMap_["delete"] = CHTLTokenType::KEYWORD_DELETE;
    keywordMap_["insert"] = CHTLTokenType::KEYWORD_INSERT;
    keywordMap_["after"] = CHTLTokenType::KEYWORD_AFTER;
    keywordMap_["before"] = CHTLTokenType::KEYWORD_BEFORE;
    keywordMap_["replace"] = CHTLTokenType::KEYWORD_REPLACE;
    keywordMap_["at top"] = CHTLTokenType::KEYWORD_AT_TOP;
    keywordMap_["at bottom"] = CHTLTokenType::KEYWORD_AT_BOTTOM;
    keywordMap_["from"] = CHTLTokenType::KEYWORD_FROM;
    keywordMap_["as"] = CHTLTokenType::KEYWORD_AS;
    keywordMap_["except"] = CHTLTokenType::KEYWORD_EXCEPT;
    
    // 特殊结构关键字
    keywordMap_["[Template]"] = CHTLTokenType::KEYWORD_TEMPLATE;
    keywordMap_["[Custom]"] = CHTLTokenType::KEYWORD_CUSTOM;
    keywordMap_["[Origin]"] = CHTLTokenType::KEYWORD_ORIGIN;
    keywordMap_["[Import]"] = CHTLTokenType::KEYWORD_IMPORT;
    keywordMap_["[Namespace]"] = CHTLTokenType::KEYWORD_NAMESPACE;
    keywordMap_["[Configuration]"] = CHTLTokenType::KEYWORD_CONFIGURATION;
    keywordMap_["[Info]"] = CHTLTokenType::KEYWORD_INFO;
    keywordMap_["[Export]"] = CHTLTokenType::KEYWORD_EXPORT;
    keywordMap_["[Name]"] = CHTLTokenType::KEYWORD_NAME;
    
    // 初始化可配置关键字的默认值（支持配置组修改）
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

void CHTLKeywordMap::updateFromConfiguration(const std::unordered_map<std::string, std::string>& config) {
    // 根据配置组更新关键字映射
    for (const auto& [key, value] : config) {
        if (key == "KEYWORD_TEXT") {
            keywordMap_.erase("text");
            keywordMap_[value] = CHTLTokenType::KEYWORD_TEXT;
        } else if (key == "KEYWORD_STYLE") {
            keywordMap_.erase("style");
            keywordMap_[value] = CHTLTokenType::KEYWORD_STYLE;
        } else if (key == "KEYWORD_SCRIPT") {
            keywordMap_.erase("script");
            keywordMap_[value] = CHTLTokenType::KEYWORD_SCRIPT;
        } else if (key == "KEYWORD_INHERIT") {
            keywordMap_.erase("inherit");
            keywordMap_[value] = CHTLTokenType::KEYWORD_INHERIT;
        } else if (key == "KEYWORD_DELETE") {
            keywordMap_.erase("delete");
            keywordMap_[value] = CHTLTokenType::KEYWORD_DELETE;
        } else if (key == "KEYWORD_INSERT") {
            keywordMap_.erase("insert");
            keywordMap_[value] = CHTLTokenType::KEYWORD_INSERT;
        } else if (key == "KEYWORD_AFTER") {
            keywordMap_.erase("after");
            keywordMap_[value] = CHTLTokenType::KEYWORD_AFTER;
        } else if (key == "KEYWORD_BEFORE") {
            keywordMap_.erase("before");
            keywordMap_[value] = CHTLTokenType::KEYWORD_BEFORE;
        } else if (key == "KEYWORD_REPLACE") {
            keywordMap_.erase("replace");
            keywordMap_[value] = CHTLTokenType::KEYWORD_REPLACE;
        } else if (key == "KEYWORD_ATTOP") {
            keywordMap_.erase("at top");
            keywordMap_[value] = CHTLTokenType::KEYWORD_AT_TOP;
        } else if (key == "KEYWORD_ATBOTTOM") {
            keywordMap_.erase("at bottom");
            keywordMap_[value] = CHTLTokenType::KEYWORD_AT_BOTTOM;
        } else if (key == "KEYWORD_FROM") {
            keywordMap_.erase("from");
            keywordMap_[value] = CHTLTokenType::KEYWORD_FROM;
        } else if (key == "KEYWORD_AS") {
            keywordMap_.erase("as");
            keywordMap_[value] = CHTLTokenType::KEYWORD_AS;
        } else if (key == "KEYWORD_EXCEPT") {
            keywordMap_.erase("except");
            keywordMap_[value] = CHTLTokenType::KEYWORD_EXCEPT;
        } else if (key == "KEYWORD_TEMPLATE") {
            keywordMap_.erase("[Template]");
            keywordMap_[value] = CHTLTokenType::KEYWORD_TEMPLATE;
        } else if (key == "KEYWORD_CUSTOM") {
            keywordMap_.erase("[Custom]");
            keywordMap_[value] = CHTLTokenType::KEYWORD_CUSTOM;
        } else if (key == "KEYWORD_ORIGIN") {
            keywordMap_.erase("[Origin]");
            keywordMap_[value] = CHTLTokenType::KEYWORD_ORIGIN;
        } else if (key == "KEYWORD_IMPORT") {
            keywordMap_.erase("[Import]");
            keywordMap_[value] = CHTLTokenType::KEYWORD_IMPORT;
        } else if (key == "KEYWORD_NAMESPACE") {
            keywordMap_.erase("[Namespace]");
            keywordMap_[value] = CHTLTokenType::KEYWORD_NAMESPACE;
        }
    }
}

bool CHTLKeywordMap::isKeyword(const std::string& word) const {
    return keywordMap_.find(word) != keywordMap_.end();
}

CHTLTokenType CHTLKeywordMap::getKeywordType(const std::string& word) const {
    auto it = keywordMap_.find(word);
    return (it != keywordMap_.end()) ? it->second : CHTLTokenType::UNKNOWN;
}

std::vector<std::string> CHTLKeywordMap::getAllKeywords() const {
    std::vector<std::string> keywords;
    for (const auto& [word, type] : keywordMap_) {
        keywords.push_back(word);
    }
    return keywords;
}

bool CHTLKeywordMap::isTemplateKeyword(const std::string& word) const {
    return word == "[Template]";
}

bool CHTLKeywordMap::isCustomKeyword(const std::string& word) const {
    return word == "[Custom]";
}

bool CHTLKeywordMap::isOriginKeyword(const std::string& word) const {
    return word == "[Origin]";
}

bool CHTLKeywordMap::isImportKeyword(const std::string& word) const {
    return word == "[Import]";
}

bool CHTLKeywordMap::isNamespaceKeyword(const std::string& word) const {
    return word == "[Namespace]";
}

bool CHTLKeywordMap::isConfigurationKeyword(const std::string& word) const {
    return word == "[Configuration]";
}

// CHTLPrefixMap 实现
CHTLPrefixMap::CHTLPrefixMap() {
    initializeDefaultPrefixes();
}

void CHTLPrefixMap::initializeDefaultPrefixes() {
    prefixMap_["@Style"] = CHTLTokenType::PREFIX_STYLE;
    prefixMap_["@Element"] = CHTLTokenType::PREFIX_ELEMENT;
    prefixMap_["@Var"] = CHTLTokenType::PREFIX_VAR;
    prefixMap_["@Html"] = CHTLTokenType::PREFIX_HTML;
    prefixMap_["@JavaScript"] = CHTLTokenType::PREFIX_JAVASCRIPT;
    prefixMap_["@Chtl"] = CHTLTokenType::PREFIX_CHTL;
    prefixMap_["@CJmod"] = CHTLTokenType::PREFIX_CJMOD;
    
    // 支持文档中提到的可选前缀
    prefixMap_["@style"] = CHTLTokenType::PREFIX_STYLE;
    prefixMap_["@CSS"] = CHTLTokenType::PREFIX_STYLE;
    prefixMap_["@Css"] = CHTLTokenType::PREFIX_STYLE;
    prefixMap_["@css"] = CHTLTokenType::PREFIX_STYLE;
}

bool CHTLPrefixMap::isPrefix(const std::string& prefix) const {
    return prefixMap_.find(prefix) != prefixMap_.end();
}

CHTLTokenType CHTLPrefixMap::getPrefixType(const std::string& prefix) const {
    auto it = prefixMap_.find(prefix);
    return (it != prefixMap_.end()) ? it->second : CHTLTokenType::UNKNOWN;
}

void CHTLPrefixMap::updateFromConfiguration(const std::unordered_map<std::string, std::string>& config) {
    // 根据配置更新前缀映射
    // 这里可以根据需要实现更复杂的配置逻辑
}

// CHTLTokenUtils 实现
bool CHTLTokenUtils::isIdentifierStart(char ch) {
    return std::isalpha(ch) || ch == '_';
}

bool CHTLTokenUtils::isIdentifierChar(char ch) {
    return std::isalnum(ch) || ch == '_' || ch == '-';
}

bool CHTLTokenUtils::isWhitespace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\r';
}

bool CHTLTokenUtils::isNewline(char ch) {
    return ch == '\n';
}

bool CHTLTokenUtils::isStringDelimiter(char ch) {
    return ch == '"' || ch == '\'';
}

std::string CHTLTokenUtils::unescapeString(const std::string& str) {
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

std::string CHTLTokenUtils::escapeString(const std::string& str) {
    std::string result;
    result.reserve(str.length() * 2); // 预留一些空间
    
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

std::string CHTLTokenUtils::tokenTypeToString(CHTLTokenType type) {
    switch (type) {
        case CHTLTokenType::IDENTIFIER: return "IDENTIFIER";
        case CHTLTokenType::STRING_DOUBLE: return "STRING_DOUBLE";
        case CHTLTokenType::STRING_SINGLE: return "STRING_SINGLE";
        case CHTLTokenType::STRING_UNQUOTED: return "STRING_UNQUOTED";
        case CHTLTokenType::LBRACE: return "LBRACE";
        case CHTLTokenType::RBRACE: return "RBRACE";
        case CHTLTokenType::LBRACKET: return "LBRACKET";
        case CHTLTokenType::RBRACKET: return "RBRACKET";
        case CHTLTokenType::LPAREN: return "LPAREN";
        case CHTLTokenType::RPAREN: return "RPAREN";
        case CHTLTokenType::SEMICOLON: return "SEMICOLON";
        case CHTLTokenType::COLON: return "COLON";
        case CHTLTokenType::EQUALS: return "EQUALS";
        case CHTLTokenType::COMMA: return "COMMA";
        case CHTLTokenType::DOT: return "DOT";
        case CHTLTokenType::HASH: return "HASH";
        case CHTLTokenType::AMPERSAND: return "AMPERSAND";
        case CHTLTokenType::KEYWORD_TEXT: return "KEYWORD_TEXT";
        case CHTLTokenType::KEYWORD_STYLE: return "KEYWORD_STYLE";
        case CHTLTokenType::KEYWORD_SCRIPT: return "KEYWORD_SCRIPT";
        case CHTLTokenType::KEYWORD_INHERIT: return "KEYWORD_INHERIT";
        case CHTLTokenType::KEYWORD_DELETE: return "KEYWORD_DELETE";
        case CHTLTokenType::KEYWORD_INSERT: return "KEYWORD_INSERT";
        case CHTLTokenType::KEYWORD_AFTER: return "KEYWORD_AFTER";
        case CHTLTokenType::KEYWORD_BEFORE: return "KEYWORD_BEFORE";
        case CHTLTokenType::KEYWORD_REPLACE: return "KEYWORD_REPLACE";
        case CHTLTokenType::KEYWORD_AT_TOP: return "KEYWORD_AT_TOP";
        case CHTLTokenType::KEYWORD_AT_BOTTOM: return "KEYWORD_AT_BOTTOM";
        case CHTLTokenType::KEYWORD_FROM: return "KEYWORD_FROM";
        case CHTLTokenType::KEYWORD_AS: return "KEYWORD_AS";
        case CHTLTokenType::KEYWORD_EXCEPT: return "KEYWORD_EXCEPT";
        case CHTLTokenType::KEYWORD_TEMPLATE: return "KEYWORD_TEMPLATE";
        case CHTLTokenType::KEYWORD_CUSTOM: return "KEYWORD_CUSTOM";
        case CHTLTokenType::KEYWORD_ORIGIN: return "KEYWORD_ORIGIN";
        case CHTLTokenType::KEYWORD_IMPORT: return "KEYWORD_IMPORT";
        case CHTLTokenType::KEYWORD_NAMESPACE: return "KEYWORD_NAMESPACE";
        case CHTLTokenType::KEYWORD_CONFIGURATION: return "KEYWORD_CONFIGURATION";
        case CHTLTokenType::KEYWORD_INFO: return "KEYWORD_INFO";
        case CHTLTokenType::KEYWORD_EXPORT: return "KEYWORD_EXPORT";
        case CHTLTokenType::KEYWORD_NAME: return "KEYWORD_NAME";
        case CHTLTokenType::PREFIX_STYLE: return "PREFIX_STYLE";
        case CHTLTokenType::PREFIX_ELEMENT: return "PREFIX_ELEMENT";
        case CHTLTokenType::PREFIX_VAR: return "PREFIX_VAR";
        case CHTLTokenType::PREFIX_HTML: return "PREFIX_HTML";
        case CHTLTokenType::PREFIX_JAVASCRIPT: return "PREFIX_JAVASCRIPT";
        case CHTLTokenType::PREFIX_CHTL: return "PREFIX_CHTL";
        case CHTLTokenType::PREFIX_CJMOD: return "PREFIX_CJMOD";
        case CHTLTokenType::COMMENT_LINE: return "COMMENT_LINE";
        case CHTLTokenType::COMMENT_MULTILINE: return "COMMENT_MULTILINE";
        case CHTLTokenType::COMMENT_GENERATOR: return "COMMENT_GENERATOR";
        case CHTLTokenType::WHITESPACE: return "WHITESPACE";
        case CHTLTokenType::NEWLINE: return "NEWLINE";
        case CHTLTokenType::EOF_TOKEN: return "EOF_TOKEN";
        case CHTLTokenType::UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

bool CHTLTokenUtils::isLiteralType(CHTLTokenType type) {
    return type == CHTLTokenType::IDENTIFIER ||
           type == CHTLTokenType::STRING_DOUBLE ||
           type == CHTLTokenType::STRING_SINGLE ||
           type == CHTLTokenType::STRING_UNQUOTED;
}

bool CHTLTokenUtils::isKeywordType(CHTLTokenType type) {
    return type >= CHTLTokenType::KEYWORD_TEXT && type <= CHTLTokenType::KEYWORD_NAME;
}

bool CHTLTokenUtils::isOperatorType(CHTLTokenType type) {
    return type == CHTLTokenType::COLON ||
           type == CHTLTokenType::EQUALS ||
           type == CHTLTokenType::DOT ||
           type == CHTLTokenType::HASH ||
           type == CHTLTokenType::AMPERSAND;
}

bool CHTLTokenUtils::isSeparatorType(CHTLTokenType type) {
    return type == CHTLTokenType::LBRACE ||
           type == CHTLTokenType::RBRACE ||
           type == CHTLTokenType::LBRACKET ||
           type == CHTLTokenType::RBRACKET ||
           type == CHTLTokenType::LPAREN ||
           type == CHTLTokenType::RPAREN ||
           type == CHTLTokenType::SEMICOLON ||
           type == CHTLTokenType::COMMA;
}

bool CHTLTokenUtils::isCommentType(CHTLTokenType type) {
    return type == CHTLTokenType::COMMENT_LINE ||
           type == CHTLTokenType::COMMENT_MULTILINE ||
           type == CHTLTokenType::COMMENT_GENERATOR;
}

bool CHTLTokenUtils::isWhitespaceType(CHTLTokenType type) {
    return type == CHTLTokenType::WHITESPACE ||
           type == CHTLTokenType::NEWLINE;
}

bool CHTLTokenUtils::startsMultipleCharSequence(char ch) {
    return ch == '/' || ch == '-' || ch == 'a'; // "//", "/*", "--", "at top", "at bottom"
}

std::string CHTLTokenUtils::getMultipleCharSequence(const std::string& source, size_t pos) {
    if (pos >= source.length()) return "";
    
    char ch = source[pos];
    
    // 检查注释序列
    if (ch == '/' && pos + 1 < source.length()) {
        if (source[pos + 1] == '/') return "//";
        if (source[pos + 1] == '*') return "/*";
    }
    
    // 检查生成器注释
    if (ch == '-' && pos + 1 < source.length() && source[pos + 1] == '-') {
        return "--";
    }
    
    // 检查复合关键字 "at top", "at bottom"
    if (ch == 'a' && pos + 5 < source.length()) {
        std::string potential = source.substr(pos, 6);
        if (potential == "at top") return "at top";
        if (pos + 8 < source.length()) {
            potential = source.substr(pos, 9);
            if (potential == "at bottom") return "at bottom";
        }
    }
    
    return std::string(1, ch);
}

} // namespace chtl