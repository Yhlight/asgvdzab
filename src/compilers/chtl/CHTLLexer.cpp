#include "CHTLLexer.h"
#include <cctype>
#include <sstream>

namespace CHTL {

std::string Token::toString() const {
    std::ostringstream oss;
    oss << "Token(";
    
    // 输出类型名称
    switch (type) {
        case TokenType::LEFT_BRACE: oss << "LEFT_BRACE"; break;
        case TokenType::RIGHT_BRACE: oss << "RIGHT_BRACE"; break;
        case TokenType::LEFT_BRACKET: oss << "LEFT_BRACKET"; break;
        case TokenType::RIGHT_BRACKET: oss << "RIGHT_BRACKET"; break;
        case TokenType::LEFT_PAREN: oss << "LEFT_PAREN"; break;
        case TokenType::RIGHT_PAREN: oss << "RIGHT_PAREN"; break;
        case TokenType::SEMICOLON: oss << "SEMICOLON"; break;
        case TokenType::COLON: oss << "COLON"; break;
        case TokenType::EQUALS: oss << "EQUALS"; break;
        case TokenType::COMMA: oss << "COMMA"; break;
        case TokenType::DOT: oss << "DOT"; break;
        case TokenType::HASH: oss << "HASH"; break;
        case TokenType::AMPERSAND: oss << "AMPERSAND"; break;
        case TokenType::AT: oss << "AT"; break;
        case TokenType::IDENTIFIER: oss << "IDENTIFIER"; break;
        case TokenType::STRING: oss << "STRING"; break;
        case TokenType::NUMBER: oss << "NUMBER"; break;
        case TokenType::UNQUOTED_STRING: oss << "UNQUOTED_STRING"; break;
        case TokenType::TEXT: oss << "TEXT"; break;
        case TokenType::STYLE: oss << "STYLE"; break;
        case TokenType::SCRIPT: oss << "SCRIPT"; break;
        case TokenType::TEMPLATE: oss << "TEMPLATE"; break;
        case TokenType::CUSTOM: oss << "CUSTOM"; break;
        case TokenType::ORIGIN: oss << "ORIGIN"; break;
        case TokenType::IMPORT: oss << "IMPORT"; break;
        case TokenType::NAMESPACE: oss << "NAMESPACE"; break;
        case TokenType::CONFIGURATION: oss << "CONFIGURATION"; break;
        case TokenType::INHERIT: oss << "INHERIT"; break;
        case TokenType::DELETE: oss << "DELETE"; break;
        case TokenType::INSERT: oss << "INSERT"; break;
        case TokenType::AFTER: oss << "AFTER"; break;
        case TokenType::BEFORE: oss << "BEFORE"; break;
        case TokenType::REPLACE: oss << "REPLACE"; break;
        case TokenType::AT_TOP: oss << "AT_TOP"; break;
        case TokenType::AT_BOTTOM: oss << "AT_BOTTOM"; break;
        case TokenType::EXCEPT: oss << "EXCEPT"; break;
        case TokenType::FROM: oss << "FROM"; break;
        case TokenType::AS: oss << "AS"; break;
        case TokenType::COMMENT: oss << "COMMENT"; break;
        case TokenType::NEWLINE: oss << "NEWLINE"; break;
        case TokenType::WHITESPACE: oss << "WHITESPACE"; break;
        case TokenType::END_OF_FILE: oss << "EOF"; break;
        default: oss << "UNKNOWN"; break;
    }
    
    oss << ", \"" << value << "\", " << line << ":" << column << ")";
    return oss.str();
}

CHTLLexer::CHTLLexer() : position_(0), line_(1), column_(1) {
    initializeKeywords();
}

void CHTLLexer::initializeKeywords() {
    keywords_ = {
        "text", "style", "script", "inherit", "delete", "insert",
        "after", "before", "replace", "except", "from", "as"
    };
}

void CHTLLexer::setSource(const std::string& source) {
    source_ = source;
    reset();
}

void CHTLLexer::reset() {
    position_ = 0;
    line_ = 1;
    column_ = 1;
    errors_.clear();
}

Token CHTLLexer::nextToken() {
    // 跳过空白字符和注释
    skipWhitespace();
    
    if (isEOF()) {
        return makeToken(TokenType::END_OF_FILE);
    }
    
    char ch = currentChar();
    
    // 单字符词法单元
    switch (ch) {
        case '{': advance(); return makeToken(TokenType::LEFT_BRACE, "{");
        case '}': advance(); return makeToken(TokenType::RIGHT_BRACE, "}");
        case '[': return scanBlockMarker();
        case ']': advance(); return makeToken(TokenType::RIGHT_BRACKET, "]");
        case '(': advance(); return makeToken(TokenType::LEFT_PAREN, "(");
        case ')': advance(); return makeToken(TokenType::RIGHT_PAREN, ")");
        case ';': advance(); return makeToken(TokenType::SEMICOLON, ";");
        case ':': advance(); return makeToken(TokenType::COLON, ":");
        case '=': advance(); return makeToken(TokenType::EQUALS, "=");
        case ',': advance(); return makeToken(TokenType::COMMA, ",");
        case '.': advance(); return makeToken(TokenType::DOT, ".");
        case '#': advance(); return makeToken(TokenType::HASH, "#");
        case '&': advance(); return makeToken(TokenType::AMPERSAND, "&");
        case '@': advance(); return makeToken(TokenType::AT, "@");
        case '"': return scanString('"');
        case '\'': return scanString('\'');
    }
    
    // 处理注释
    if (ch == '/' && peekChar() == '/') {
        skipComment();
        return nextToken();
    }
    
    if (ch == '/' && peekChar() == '*') {
        skipComment();
        return nextToken();
    }
    
    if (ch == '-' && peekChar() == '-') {
        skipComment();
        return nextToken();
    }
    
    // 数字
    if (isDigit(ch)) {
        return scanNumber();
    }
    
    // 标识符或关键字
    if (isAlpha(ch) || ch == '_') {
        return scanIdentifier();
    }
    
    // 无引号字符串（在某些上下文中）
    if (ch != '\n' && ch != '\r' && !isWhitespace(ch)) {
        return scanUnquotedString();
    }
    
    // 未知字符
    recordError("未知字符: " + std::string(1, ch));
    advance();
    return makeToken(TokenType::UNKNOWN, std::string(1, ch));
}

Token CHTLLexer::peekToken() {
    size_t savedPos = position_;
    size_t savedLine = line_;
    size_t savedCol = column_;
    
    Token token = nextToken();
    
    position_ = savedPos;
    line_ = savedLine;
    column_ = savedCol;
    
    return token;
}

char CHTLLexer::currentChar() const {
    if (isEOF()) return '\0';
    return source_[position_];
}

char CHTLLexer::peekChar(size_t offset) const {
    size_t pos = position_ + offset;
    if (pos >= source_.length()) return '\0';
    return source_[pos];
}

void CHTLLexer::advance() {
    if (isEOF()) return;
    
    if (currentChar() == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    
    position_++;
}

void CHTLLexer::skipWhitespace() {
    while (!isEOF() && isWhitespace(currentChar())) {
        advance();
    }
}

void CHTLLexer::skipComment() {
    char ch = currentChar();
    
    if (ch == '/' && peekChar() == '/') {
        // 单行注释
        while (!isEOF() && currentChar() != '\n') {
            advance();
        }
    } else if (ch == '/' && peekChar() == '*') {
        // 多行注释
        advance(); // 跳过 '/'
        advance(); // 跳过 '*'
        
        while (!isEOF()) {
            if (currentChar() == '*' && peekChar() == '/') {
                advance(); // 跳过 '*'
                advance(); // 跳过 '/'
                break;
            }
            advance();
        }
    } else if (ch == '-' && peekChar() == '-') {
        // 生成器注释
        while (!isEOF() && currentChar() != '\n') {
            advance();
        }
    }
}

Token CHTLLexer::scanIdentifier() {
    size_t startPos = position_;
    size_t startCol = column_;
    std::string value;
    
    while (!isEOF() && (isAlphaNumeric(currentChar()) || currentChar() == '_' || currentChar() == '-')) {
        value += currentChar();
        advance();
    }
    
    TokenType type = getKeywordType(value);
    if (type == TokenType::UNKNOWN) {
        type = TokenType::IDENTIFIER;
    }
    
    return Token(type, value, line_, startCol, startPos);
}

Token CHTLLexer::scanString(char quote) {
    size_t startPos = position_;
    size_t startCol = column_;
    std::string value;
    
    advance(); // 跳过开始引号
    
    while (!isEOF() && currentChar() != quote) {
        if (currentChar() == '\\' && !isEOF()) {
            advance(); // 跳过反斜杠
            if (!isEOF()) {
                char escaped = currentChar();
                switch (escaped) {
                    case 'n': value += '\n'; break;
                    case 't': value += '\t'; break;
                    case 'r': value += '\r'; break;
                    case '\\': value += '\\'; break;
                    case '"': value += '"'; break;
                    case '\'': value += '\''; break;
                    default: value += escaped; break;
                }
                advance();
            }
        } else {
            value += currentChar();
            advance();
        }
    }
    
    if (isEOF()) {
        recordError("未终止的字符串");
    } else {
        advance(); // 跳过结束引号
    }
    
    return Token(TokenType::STRING, value, line_, startCol, startPos);
}

Token CHTLLexer::scanUnquotedString() {
    size_t startPos = position_;
    size_t startCol = column_;
    std::string value;
    
    while (!isEOF() && currentChar() != '\n' && currentChar() != '\r' && 
           currentChar() != ';' && currentChar() != '{' && currentChar() != '}' &&
           currentChar() != ',' && !isWhitespace(currentChar())) {
        value += currentChar();
        advance();
    }
    
    return Token(TokenType::UNQUOTED_STRING, value, line_, startCol, startPos);
}

Token CHTLLexer::scanNumber() {
    size_t startPos = position_;
    size_t startCol = column_;
    std::string value;
    
    while (!isEOF() && (isDigit(currentChar()) || currentChar() == '.')) {
        value += currentChar();
        advance();
    }
    
    // 处理单位 (px, em, %, 等)
    while (!isEOF() && isAlpha(currentChar())) {
        value += currentChar();
        advance();
    }
    
    return Token(TokenType::NUMBER, value, line_, startCol, startPos);
}

Token CHTLLexer::scanBlockMarker() {
    size_t startPos = position_;
    size_t startCol = column_;
    std::string value;
    
    advance(); // 跳过 '['
    
    while (!isEOF() && currentChar() != ']') {
        value += currentChar();
        advance();
    }
    
    if (isEOF()) {
        recordError("未终止的块标记");
        return Token(TokenType::UNKNOWN, "[" + value, line_, startCol, startPos);
    }
    
    advance(); // 跳过 ']'
    
    // 判断块标记类型
    TokenType type = TokenType::UNKNOWN;
    if (value == "Template") {
        type = TokenType::TEMPLATE;
    } else if (value == "Custom") {
        type = TokenType::CUSTOM;
    } else if (value == "Origin") {
        type = TokenType::ORIGIN;
    } else if (value == "Import") {
        type = TokenType::IMPORT;
    } else if (value == "Namespace") {
        type = TokenType::NAMESPACE;
    } else if (value == "Configuration") {
        type = TokenType::CONFIGURATION;
    } else {
        type = TokenType::LEFT_BRACKET; // 当作普通左括号处理
        // 回退位置，重新解析
        position_ = startPos + 1;
        column_ = startCol + 1;
        return Token(TokenType::LEFT_BRACKET, "[", line_, startCol, startPos);
    }
    
    return Token(type, value, line_, startCol, startPos);
}

TokenType CHTLLexer::getKeywordType(const std::string& identifier) const {
    if (keywords_.find(identifier) != keywords_.end()) {
        if (identifier == "text") return TokenType::TEXT;
        if (identifier == "style") return TokenType::STYLE;
        if (identifier == "script") return TokenType::SCRIPT;
        if (identifier == "inherit") return TokenType::INHERIT;
        if (identifier == "delete") return TokenType::DELETE;
        if (identifier == "insert") return TokenType::INSERT;
        if (identifier == "after") return TokenType::AFTER;
        if (identifier == "before") return TokenType::BEFORE;
        if (identifier == "replace") return TokenType::REPLACE;
        if (identifier == "except") return TokenType::EXCEPT;
        if (identifier == "from") return TokenType::FROM;
        if (identifier == "as") return TokenType::AS;
    }
    
    // 处理复合关键字 - 简化处理，避免修改const成员函数中的状态
    if (identifier == "at") {
        // 这里需要更复杂的处理，暂时简化
        return TokenType::UNKNOWN;
    }
    
    return TokenType::UNKNOWN;
}

bool CHTLLexer::isAlpha(char c) const {
    return std::isalpha(c) || c == '_';
}

bool CHTLLexer::isDigit(char c) const {
    return std::isdigit(c);
}

bool CHTLLexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool CHTLLexer::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

Token CHTLLexer::makeToken(TokenType type, const std::string& value) {
    return Token(type, value, line_, column_, position_);
}

void CHTLLexer::recordError(const std::string& message) {
    std::ostringstream oss;
    oss << "词法错误 [" << line_ << ":" << column_ << "]: " << message;
    errors_.push_back(oss.str());
}

} // namespace CHTL