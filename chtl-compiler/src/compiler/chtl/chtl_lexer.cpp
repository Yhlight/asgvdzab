#include "compiler/chtl/chtl_lexer.h"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <iostream>

namespace chtl {

CHTLLexer::CHTLLexer(const std::string& input, const std::string& filename)
    : input_(input), filename_(filename), position_(0), line_(1), column_(1),
      has_peeked_(false) {}

Token CHTLLexer::nextToken() {
    if (has_peeked_) {
        has_peeked_ = false;
        return peeked_token_;
    }
    
    skipWhitespace();
    
    if (position_ >= input_.size()) {
        return makeToken(TokenType::EOF_TOKEN, "", position_, position_);
    }
    
    size_t start_pos = position_;
    size_t start_line = line_;
    size_t start_column = column_;
    
    char c = peek();
    
    // 注释处理
    if (c == '/' && peek(1) == '/') {
        // 单行注释
        advance(); advance();
        std::string comment = "//";
        while (peek() != '\n' && peek() != '\0') {
            comment += advance();
        }
        return makeToken(TokenType::COMMENT_SINGLE, comment, start_pos, position_);
    }
    
    if (c == '/' && peek(1) == '*') {
        // 多行注释
        advance(); advance();
        std::string comment = "/*";
        while (position_ < input_.size() - 1) {
            if (peek() == '*' && peek(1) == '/') {
                comment += advance();
                comment += advance();
                break;
            }
            comment += advance();
        }
        return makeToken(TokenType::COMMENT_MULTI, comment, start_pos, position_);
    }
    
    if (c == '-' && peek(1) == '-') {
        // 生成器识别的注释
        advance(); advance();
        std::string comment = "--";
        while (peek() != '\n' && peek() != '\0') {
            comment += advance();
        }
        return makeToken(TokenType::COMMENT_GENERATOR, comment, start_pos, position_);
    }
    
    // 特殊块处理 [...]
    if (c == '[') {
        advance();  // 消费 '['
        return scanSpecialBlock();
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

Token CHTLLexer::peekToken() {
    if (!has_peeked_) {
        peeked_token_ = nextToken();
        has_peeked_ = true;
    }
    return peeked_token_;
}

void CHTLLexer::skipWhitespace() {
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

void CHTLLexer::skipComment() {
    // 注释在nextToken中直接处理
}

Token CHTLLexer::scanIdentifierOrKeyword() {
    size_t start = position_;
    std::string value;
    
    // 处理@开头的类型标记
    if (peek() == '@') {
        value += advance();
    }
    
    while (isIdentifierChar(peek())) {
        value += advance();
    }
    
    // 检查是否是关键字
    TokenType type = TokenType::IDENTIFIER;
    if (getCHTLKeywords().count(value) > 0) {
        type = TokenType::KEYWORD;
    }
    
    return makeToken(type, value, start, position_);
}

Token CHTLLexer::scanNumber() {
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
    
    return makeToken(TokenType::NUMBER, value, start, position_);
}

Token CHTLLexer::scanString(char quote) {
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
    
    return makeToken(TokenType::STRING_LITERAL, value, start, position_);
}

Token CHTLLexer::scanUnquotedLiteral() {
    size_t start = position_;
    std::string value;
    
    // 无引号字面量：读取到分隔符为止
    while (peek() != '\0' && !isWhitespace(peek()) && 
           peek() != ';' && peek() != '}' && peek() != '{' &&
           peek() != ':' && peek() != '=' && peek() != ',') {
        value += advance();
    }
    
    return makeToken(TokenType::UNQUOTED_LITERAL, value, start, position_);
}

Token CHTLLexer::scanSpecialBlock() {
    size_t start = position_ - 1;  // 我们已经读取了'['
    std::string value = "[";
    
    // 读取内容直到 ']'
    while (peek() != ']' && peek() != '\0') {
        value += advance();
    }
    if (peek() == ']') {
        value += advance();
    }
    
    // DEBUG: 输出value的内容
    // std::cerr << "scanSpecialBlock: value='" << value << "'" << std::endl;
    
    // 检查是否是特殊块
    if (getSpecialBlocks().count(value) > 0) {
        return makeToken(TokenType::IDENTIFIER, value, start, position_);
    }
    
    // 如果不是特殊块，回退到'['之后
    position_ = start + 1;
    // 重新计算行列位置
    line_ = 1;
    column_ = 1;
    for (size_t i = 0; i < position_; ++i) {
        if (input_[i] == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
    }
    
    return makeToken(TokenType::LBRACKET, "[", start, start + 1);
}

Token CHTLLexer::scanOperator() {
    size_t start = position_;
    char c = advance();
    
    switch (c) {
        case '{': return makeToken(TokenType::LBRACE, "{", start, position_);
        case '}': return makeToken(TokenType::RBRACE, "}", start, position_);
        case '(': return makeToken(TokenType::LPAREN, "(", start, position_);
        case ')': return makeToken(TokenType::RPAREN, ")", start, position_);
        case '[': return makeToken(TokenType::LBRACKET, "[", start, position_);
        case ']': return makeToken(TokenType::RBRACKET, "]", start, position_);
        case ':': return makeToken(TokenType::COLON, ":", start, position_);
        case '=': return makeToken(TokenType::EQUALS, "=", start, position_);
        case ';': return makeToken(TokenType::SEMICOLON, ";", start, position_);
        case ',': return makeToken(TokenType::COMMA, ",", start, position_);
        case '.': return makeToken(TokenType::DOT, ".", start, position_);
        case '@': return makeToken(TokenType::AT, "@", start, position_);
        case '&': return makeToken(TokenType::AMPERSAND, "&", start, position_);
        case '#':
            // # 后面跟着标识符表示ID选择器
            if (isIdentifierStart(peek())) {
                std::string id = "#";
                while (isIdentifierChar(peek())) {
                    id += advance();
                }
                return makeToken(TokenType::IDENTIFIER, id, start, position_);
            }
            return makeToken(TokenType::IDENTIFIER, "#", start, position_);
        case '-':
            if (peek() == '>') {
                advance();
                return makeToken(TokenType::ARROW, "->", start, position_);
            }
            // 如果不是箭头，可能是减号或其他
            return makeToken(TokenType::IDENTIFIER, "-", start, position_);
        default:
            // 未知字符，作为标识符处理
            return makeToken(TokenType::IDENTIFIER, std::string(1, c), start, position_);
    }
}

bool CHTLLexer::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool CHTLLexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool CHTLLexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool CHTLLexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool CHTLLexer::isIdentifierStart(char c) const {
    return isAlpha(c) || c == '_';
}

bool CHTLLexer::isIdentifierChar(char c) const {
    return isAlphaNumeric(c) || c == '_' || c == '-';
}

char CHTLLexer::peek(size_t offset) const {
    size_t pos = position_ + offset;
    if (pos >= input_.size()) {
        return '\0';
    }
    return input_[pos];
}

char CHTLLexer::peekChar(size_t offset) const {
    return peek(offset);
}

void CHTLLexer::skipChar() {
    if (position_ < input_.size()) {
        if (input_[position_] == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        position_++;
    }
}

char CHTLLexer::advance() {
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

bool CHTLLexer::match(char expected) {
    if (peek() != expected) {
        return false;
    }
    advance();
    return true;
}

bool CHTLLexer::matchSequence(const std::string& seq) {
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

Token CHTLLexer::makeToken(TokenType type, const std::string& value,
                           size_t start_pos, size_t end_pos) {
    return Token(type, value, line_, column_, start_pos, end_pos);
}

void CHTLLexer::reportError(const std::string& message) {
    std::stringstream ss;
    ss << filename_ << ":" << line_ << ":" << column_ << ": " << message;
    errors_.push_back(ss.str());
}

} // namespace chtl