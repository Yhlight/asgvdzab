#include "compiler/chtljs/chtljs_lexer.h"
#include <sstream>
#include <cctype>
#include <algorithm>

namespace chtl {

CHTLJSLexer::CHTLJSLexer(const std::string& input, const std::string& filename)
    : input_(input), filename_(filename), position_(0), line_(1), column_(1),
      has_peeked_(false) {}

CHTLJSToken CHTLJSLexer::nextToken() {
    if (has_peeked_) {
        has_peeked_ = false;
        return peeked_token_;
    }
    
    skipWhitespace();
    
    if (position_ >= input_.size()) {
        return makeToken(CHTLJSTokenType::EOF_TOKEN, "", position_, position_);
    }
    
    size_t start_pos = position_;
    
    // 检查增强选择器 {{
    if (isEnhancedSelector()) {
        return scanEnhancedSelector();
    }
    
    char c = peek();
    
    // 注释处理
    if (c == '/' && peek(1) == '/') {
        // 单行注释
        advance(); advance();
        std::string comment;
        while (peek() != '\n' && peek() != '\0') {
            comment += advance();
        }
        return makeToken(CHTLJSTokenType::COMMENT, comment, start_pos, position_);
    }
    
    if (c == '/' && peek(1) == '*') {
        // 多行注释
        advance(); advance();
        std::string comment;
        while (position_ < input_.size() - 1) {
            if (peek() == '*' && peek(1) == '/') {
                advance(); advance();
                break;
            }
            comment += advance();
        }
        return makeToken(CHTLJSTokenType::COMMENT, comment, start_pos, position_);
    }
    
    // 箭头操作符 ->
    if (c == '-' && peek(1) == '>') {
        advance(); advance();
        return makeToken(CHTLJSTokenType::ARROW, "->", start_pos, position_);
    }
    
    // 字符串字面量
    if (c == '"' || c == '\'') {
        return scanString(c);
    }
    
    // 数字
    if (isDigit(c)) {
        return scanNumber();
    }
    
    // 标识符或关键字
    if (isIdentifierStart(c)) {
        return scanIdentifierOrKeyword();
    }
    
    // 操作符和分隔符
    return scanOperator();
}

CHTLJSToken CHTLJSLexer::peekToken() {
    if (!has_peeked_) {
        peeked_token_ = nextToken();
        has_peeked_ = true;
    }
    return peeked_token_;
}

void CHTLJSLexer::skipWhitespace() {
    while (position_ < input_.size() && isWhitespace(peek())) {
        if (peek() == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        position_++;
    }
}

void CHTLJSLexer::skipComment() {
    // 注释在nextToken中直接处理
}

CHTLJSToken CHTLJSLexer::scanEnhancedSelector() {
    size_t start = position_;
    
    // 跳过 {{
    advance(); advance();
    
    std::string selector;
    int brace_count = 2;
    
    while (position_ < input_.size() && brace_count > 0) {
        char c = peek();
        
        if (c == '{') {
            brace_count++;
        } else if (c == '}') {
            brace_count--;
            if (brace_count == 0) {
                advance(); // 跳过第一个 }
                if (peek() == '}') {
                    advance(); // 跳过第二个 }
                }
                break;
            }
        }
        
        selector += advance();
    }
    
    return makeToken(CHTLJSTokenType::ENHANCED_SELECTOR, selector, start, position_);
}

CHTLJSToken CHTLJSLexer::scanIdentifierOrKeyword() {
    size_t start = position_;
    std::string value;
    
    while (isIdentifierChar(peek())) {
        value += advance();
    }
    
    // 检查是否是特殊函数
    if (isSpecialFunction(value)) {
        if (value == "listen") {
            return makeToken(CHTLJSTokenType::LISTEN, value, start, position_);
        } else if (value == "delegate") {
            return makeToken(CHTLJSTokenType::DELEGATE, value, start, position_);
        } else if (value == "animate") {
            return makeToken(CHTLJSTokenType::ANIMATE, value, start, position_);
        } else if (value == "vir") {
            return makeToken(CHTLJSTokenType::VIR, value, start, position_);
        }
    }
    
    // 检查是否是关键字
    if (getCHTLJSKeywords().count(value) > 0) {
        return makeToken(CHTLJSTokenType::KEYWORD, value, start, position_);
    }
    
    return makeToken(CHTLJSTokenType::IDENTIFIER, value, start, position_);
}

CHTLJSToken CHTLJSLexer::scanNumber() {
    size_t start = position_;
    std::string value;
    
    while (isDigit(peek())) {
        value += advance();
    }
    
    // 小数部分
    if (peek() == '.' && isDigit(peek(1))) {
        value += advance(); // .
        while (isDigit(peek())) {
            value += advance();
        }
    }
    
    return makeToken(CHTLJSTokenType::NUMBER, value, start, position_);
}

CHTLJSToken CHTLJSLexer::scanString(char quote) {
    size_t start = position_;
    std::string value;
    
    advance(); // 跳过开始引号
    
    while (peek() != quote && peek() != '\0') {
        if (peek() == '\\' && peek(1) != '\0') {
            // 转义字符
            advance();
            char escaped = advance();
            switch (escaped) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '\\': value += '\\'; break;
                case '"': value += '"'; break;
                case '\'': value += '\''; break;
                default: value += escaped; break;
            }
        } else {
            value += advance();
        }
    }
    
    if (peek() == quote) {
        advance(); // 跳过结束引号
    } else {
        reportError("Unterminated string literal");
    }
    
    return makeToken(CHTLJSTokenType::STRING_LITERAL, value, start, position_);
}

CHTLJSToken CHTLJSLexer::scanOperator() {
    size_t start = position_;
    char c = advance();
    
    switch (c) {
        case '{': return makeToken(CHTLJSTokenType::LBRACE, "{", start, position_);
        case '}': return makeToken(CHTLJSTokenType::RBRACE, "}", start, position_);
        case '(': return makeToken(CHTLJSTokenType::LPAREN, "(", start, position_);
        case ')': return makeToken(CHTLJSTokenType::RPAREN, ")", start, position_);
        case '[': return makeToken(CHTLJSTokenType::LBRACKET, "[", start, position_);
        case ']': return makeToken(CHTLJSTokenType::RBRACKET, "]", start, position_);
        case ':': return makeToken(CHTLJSTokenType::COLON, ":", start, position_);
        case ';': return makeToken(CHTLJSTokenType::SEMICOLON, ";", start, position_);
        case ',': return makeToken(CHTLJSTokenType::COMMA, ",", start, position_);
        case '.': return makeToken(CHTLJSTokenType::DOT, ".", start, position_);
        case '=': return makeToken(CHTLJSTokenType::EQUALS, "=", start, position_);
        case '+': return makeToken(CHTLJSTokenType::PLUS, "+", start, position_);
        case '-': return makeToken(CHTLJSTokenType::MINUS, "-", start, position_);
        case '*': return makeToken(CHTLJSTokenType::STAR, "*", start, position_);
        case '/': return makeToken(CHTLJSTokenType::SLASH, "/", start, position_);
        default:
            // 未知字符
            return makeToken(CHTLJSTokenType::UNKNOWN, std::string(1, c), start, position_);
    }
}

bool CHTLJSLexer::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool CHTLJSLexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool CHTLJSLexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool CHTLJSLexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool CHTLJSLexer::isIdentifierStart(char c) const {
    return isAlpha(c) || c == '_' || c == '$';
}

bool CHTLJSLexer::isIdentifierChar(char c) const {
    return isAlphaNumeric(c) || c == '_' || c == '$';
}

char CHTLJSLexer::peek(size_t offset) const {
    size_t pos = position_ + offset;
    if (pos >= input_.size()) {
        return '\0';
    }
    return input_[pos];
}

char CHTLJSLexer::advance() {
    if (position_ >= input_.size()) {
        return '\0';
    }
    
    char c = input_[position_++];
    if (c == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    
    return c;
}

bool CHTLJSLexer::match(char expected) {
    if (peek() != expected) {
        return false;
    }
    advance();
    return true;
}

bool CHTLJSLexer::matchSequence(const std::string& seq) {
    for (size_t i = 0; i < seq.length(); ++i) {
        if (peek(i) != seq[i]) {
            return false;
        }
    }
    
    for (size_t i = 0; i < seq.length(); ++i) {
        advance();
    }
    
    return true;
}

CHTLJSToken CHTLJSLexer::makeToken(CHTLJSTokenType type, const std::string& value,
                                   size_t start_pos, size_t end_pos) {
    return CHTLJSToken(type, value, line_, column_, start_pos, end_pos);
}

void CHTLJSLexer::reportError(const std::string& message) {
    std::stringstream ss;
    ss << filename_ << ":" << line_ << ":" << column_ << ": " << message;
    errors_.push_back(ss.str());
}

bool CHTLJSLexer::isEnhancedSelector() {
    return peek() == '{' && peek(1) == '{';
}

bool CHTLJSLexer::isSpecialFunction(const std::string& identifier) {
    return identifier == "listen" || 
           identifier == "delegate" || 
           identifier == "animate" || 
           identifier == "vir";
}

} // namespace chtl