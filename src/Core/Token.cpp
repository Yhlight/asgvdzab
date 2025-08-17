#include "../../include/Core/Token.h"
#include <sstream>
#include <unordered_map>

namespace CHTL {
namespace Core {

// Token类方法实现

bool Token::isKeyword() const {
    return isKeywordToken(type_);
}

bool Token::isOperator() const {
    return isOperatorToken(type_);
}

bool Token::isDelimiter() const {
    return isDelimiterToken(type_);
}

String Token::getStringValue() const {
    if (std::holds_alternative<String>(value_)) {
        return std::get<String>(value_);
    }
    return text_;
}

int64_t Token::getIntValue() const {
    if (std::holds_alternative<int64_t>(value_)) {
        return std::get<int64_t>(value_);
    }
    if (std::holds_alternative<double>(value_)) {
        return static_cast<int64_t>(std::get<double>(value_));
    }
    return 0;
}

double Token::getDoubleValue() const {
    if (std::holds_alternative<double>(value_)) {
        return std::get<double>(value_);
    }
    if (std::holds_alternative<int64_t>(value_)) {
        return static_cast<double>(std::get<int64_t>(value_));
    }
    return 0.0;
}

bool Token::getBoolValue() const {
    if (std::holds_alternative<bool>(value_)) {
        return std::get<bool>(value_);
    }
    return false;
}

String Token::toString() const {
    std::stringstream ss;
    ss << "Token{type=" << tokenTypeToString(type_)
       << ", text=\"" << text_ << "\""
       << ", pos=" << position_.line << ":" << position_.column
       << "}";
    return ss.str();
}

bool Token::operator==(const Token& other) const {
    return type_ == other.type_ && 
           text_ == other.text_ && 
           value_ == other.value_;
}

// 全局函数实现

String tokenTypeToString(TokenType type) {
    static const std::unordered_map<TokenType, String> tokenNames = {
        {TokenType::IDENTIFIER, "IDENTIFIER"},
        {TokenType::STRING_LITERAL, "STRING_LITERAL"},
        {TokenType::NUMBER_LITERAL, "NUMBER_LITERAL"},
        {TokenType::COMMENT, "COMMENT"},
        {TokenType::LBRACE, "LBRACE"},
        {TokenType::RBRACE, "RBRACE"},
        {TokenType::LPAREN, "LPAREN"},
        {TokenType::RPAREN, "RPAREN"},
        {TokenType::LBRACKET, "LBRACKET"},
        {TokenType::RBRACKET, "RBRACKET"},
        {TokenType::SEMICOLON, "SEMICOLON"},
        {TokenType::COLON, "COLON"},
        {TokenType::COMMA, "COMMA"},
        {TokenType::DOT, "DOT"},
        {TokenType::ARROW, "ARROW"},
        {TokenType::EQUAL, "EQUAL"},
        {TokenType::TEXT, "TEXT"},
        {TokenType::STYLE, "STYLE"},
        {TokenType::SCRIPT, "SCRIPT"},
        {TokenType::TEMPLATE, "TEMPLATE"},
        {TokenType::CUSTOM, "CUSTOM"},
        {TokenType::ORIGIN, "ORIGIN"},
        {TokenType::IMPORT, "IMPORT"},
        {TokenType::NAMESPACE, "NAMESPACE"},
        {TokenType::CONFIGURATION, "CONFIGURATION"},
        {TokenType::AT_STYLE, "AT_STYLE"},
        {TokenType::AT_ELEMENT, "AT_ELEMENT"},
        {TokenType::AT_VAR, "AT_VAR"},
        {TokenType::AT_HTML, "AT_HTML"},
        {TokenType::AT_JAVASCRIPT, "AT_JAVASCRIPT"},
        {TokenType::AT_CHTL, "AT_CHTL"},
        {TokenType::AT_CJMOD, "AT_CJMOD"},
        {TokenType::INHERIT, "INHERIT"},
        {TokenType::DELETE, "DELETE"},
        {TokenType::INSERT, "INSERT"},
        {TokenType::FROM, "FROM"},
        {TokenType::AS, "AS"},
        {TokenType::EXCEPT, "EXCEPT"},
        {TokenType::AFTER, "AFTER"},
        {TokenType::BEFORE, "BEFORE"},
        {TokenType::REPLACE, "REPLACE"},
        {TokenType::AT_TOP, "AT_TOP"},
        {TokenType::AT_BOTTOM, "AT_BOTTOM"},
        {TokenType::VIR, "VIR"},
        {TokenType::LISTEN, "LISTEN"},
        {TokenType::ANIMATE, "ANIMATE"},
        {TokenType::DELEGATE, "DELEGATE"},
        {TokenType::SELECTOR_START, "SELECTOR_START"},
        {TokenType::SELECTOR_END, "SELECTOR_END"},
        {TokenType::EOF_TOKEN, "EOF_TOKEN"},
        {TokenType::ERROR, "ERROR"}
    };
    
    auto it = tokenNames.find(type);
    return it != tokenNames.end() ? it->second : "UNKNOWN";
}

bool isKeywordToken(TokenType type) {
    switch (type) {
        case TokenType::TEXT:
        case TokenType::STYLE:
        case TokenType::SCRIPT:
        case TokenType::TEMPLATE:
        case TokenType::CUSTOM:
        case TokenType::ORIGIN:
        case TokenType::IMPORT:
        case TokenType::NAMESPACE:
        case TokenType::CONFIGURATION:
        case TokenType::AT_STYLE:
        case TokenType::AT_ELEMENT:
        case TokenType::AT_VAR:
        case TokenType::AT_HTML:
        case TokenType::AT_JAVASCRIPT:
        case TokenType::AT_CHTL:
        case TokenType::AT_CJMOD:
        case TokenType::INHERIT:
        case TokenType::DELETE:
        case TokenType::INSERT:
        case TokenType::FROM:
        case TokenType::AS:
        case TokenType::EXCEPT:
        case TokenType::AFTER:
        case TokenType::BEFORE:
        case TokenType::REPLACE:
        case TokenType::AT_TOP:
        case TokenType::AT_BOTTOM:
        case TokenType::VIR:
        case TokenType::LISTEN:
        case TokenType::ANIMATE:
        case TokenType::DELEGATE:
            return true;
        default:
            return false;
    }
}

bool isOperatorToken(TokenType type) {
    switch (type) {
        case TokenType::ARROW:
        case TokenType::EQUAL:
        case TokenType::DOT:
            return true;
        default:
            return false;
    }
}

bool isDelimiterToken(TokenType type) {
    switch (type) {
        case TokenType::LBRACE:
        case TokenType::RBRACE:
        case TokenType::LPAREN:
        case TokenType::RPAREN:
        case TokenType::LBRACKET:
        case TokenType::RBRACKET:
        case TokenType::SEMICOLON:
        case TokenType::COLON:
        case TokenType::COMMA:
        case TokenType::SELECTOR_START:
        case TokenType::SELECTOR_END:
            return true;
        default:
            return false;
    }
}

TokenType getKeywordType(const String& text) {
    static const std::unordered_map<String, TokenType> keywords = {
        {"text", TokenType::TEXT},
        {"style", TokenType::STYLE},
        {"script", TokenType::SCRIPT},
        {"[Template]", TokenType::TEMPLATE},
        {"[Custom]", TokenType::CUSTOM},
        {"[Origin]", TokenType::ORIGIN},
        {"[Import]", TokenType::IMPORT},
        {"[Namespace]", TokenType::NAMESPACE},
        {"[Configuration]", TokenType::CONFIGURATION},
        {"@Style", TokenType::AT_STYLE},
        {"@Element", TokenType::AT_ELEMENT},
        {"@Var", TokenType::AT_VAR},
        {"@Html", TokenType::AT_HTML},
        {"@JavaScript", TokenType::AT_JAVASCRIPT},
        {"@Chtl", TokenType::AT_CHTL},
        {"@CJmod", TokenType::AT_CJMOD},
        {"inherit", TokenType::INHERIT},
        {"delete", TokenType::DELETE},
        {"insert", TokenType::INSERT},
        {"from", TokenType::FROM},
        {"as", TokenType::AS},
        {"except", TokenType::EXCEPT},
        {"after", TokenType::AFTER},
        {"before", TokenType::BEFORE},
        {"replace", TokenType::REPLACE},
        {"at top", TokenType::AT_TOP},
        {"at bottom", TokenType::AT_BOTTOM},
        {"vir", TokenType::VIR},
        {"listen", TokenType::LISTEN},
        {"animate", TokenType::ANIMATE},
        {"delegate", TokenType::DELEGATE}
    };
    
    auto it = keywords.find(text);
    return it != keywords.end() ? it->second : TokenType::IDENTIFIER;
}

// 便利函数实现

TokenPtr makeToken(TokenType type, const String& text, const Position& pos) {
    return std::make_shared<Token>(type, text, pos);
}

TokenPtr makeStringToken(const String& text, const String& value, const Position& pos) {
    return std::make_shared<Token>(TokenType::STRING_LITERAL, text, value, pos);
}

TokenPtr makeNumberToken(const String& text, int64_t value, const Position& pos) {
    return std::make_shared<Token>(TokenType::NUMBER_LITERAL, text, value, pos);
}

TokenPtr makeNumberToken(const String& text, double value, const Position& pos) {
    return std::make_shared<Token>(TokenType::NUMBER_LITERAL, text, value, pos);
}

TokenPtr makeErrorToken(const String& message, const Position& pos) {
    return std::make_shared<Token>(TokenType::ERROR, message, pos);
}

} // namespace Core
} // namespace CHTL