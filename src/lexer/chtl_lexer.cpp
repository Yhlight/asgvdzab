#include "lexer/chtl_lexer.hpp"
#include <cctype>

namespace chtl {

CHTLLexer::CHTLLexer() : currentPos_(0), line_(1), column_(1) {
    initializeKeywords();
}

CHTLLexer::~CHTLLexer() = default;

LexResult CHTLLexer::tokenize(const std::string& source, const std::string& filename) {
    source_ = source;
    filename_ = filename;
    currentPos_ = 0;
    line_ = 1;
    column_ = 1;
    tokens_.clear();
    errors_.clear();

    while (!isEOF()) {
        // 跳过空白
        if (isWhitespace(currentChar())) {
            if (currentChar() == '\n') {
                tokens_.emplace_back(TokenType::NEWLINE, "\n", getCurrentPosition());
                advance();
            } else {
                skipWhitespace();
            }
            continue;
        }

        // 注释
        if (currentChar() == '/' && peekChar() == '/') {
            skipLineComment();
            continue;
        }
        
        if (currentChar() == '/' && peekChar() == '*') {
            skipMultilineComment();
            continue;
        }

        // 生成器注释
        if (currentChar() == '-' && peekChar() == '-') {
            tokens_.emplace_back(TokenType::OPERATOR, "--", getCurrentPosition());
            advance(); // -
            advance(); // -
            continue;
        }

        // 字符串字面量
        if (currentChar() == '"' || currentChar() == '\'') {
            tokens_.push_back(readString(currentChar()));
            continue;
        }

        // 数字
        if (isDigit(currentChar())) {
            tokens_.push_back(readNumber());
            continue;
        }

        // 标识符和关键字
        if (isAlpha(currentChar()) || currentChar() == '_') {
            tokens_.push_back(readIdentifier());
            continue;
        }

        // 单字符token
        char c = currentChar();
        Position pos = getCurrentPosition();
        
        switch (c) {
            case '{':
                tokens_.emplace_back(TokenType::LEFT_BRACE, "{", pos);
                advance();
                break;
            case '}':
                tokens_.emplace_back(TokenType::RIGHT_BRACE, "}", pos);
                advance();
                break;
            case '(':
                tokens_.emplace_back(TokenType::LEFT_PAREN, "(", pos);
                advance();
                break;
            case ')':
                tokens_.emplace_back(TokenType::RIGHT_PAREN, ")", pos);
                advance();
                break;
            case '[':
                tokens_.emplace_back(TokenType::LEFT_BRACKET, "[", pos);
                advance();
                break;
            case ']':
                tokens_.emplace_back(TokenType::RIGHT_BRACKET, "]", pos);
                advance();
                break;
            case ':':
                tokens_.emplace_back(TokenType::COLON, ":", pos);
                advance();
                break;
            case ';':
                tokens_.emplace_back(TokenType::SEMICOLON, ";", pos);
                advance();
                break;
            case '=':
                tokens_.emplace_back(TokenType::ASSIGNMENT, "=", pos);
                advance();
                break;
            case '.':
                tokens_.emplace_back(TokenType::DOT, ".", pos);
                advance();
                break;
            case '#':
                tokens_.emplace_back(TokenType::HASH, "#", pos);
                advance();
                break;
            case '&':
                tokens_.emplace_back(TokenType::AMPERSAND, "&", pos);
                advance();
                break;
            default:
                tokens_.push_back(readOperator());
                break;
        }
    }

    // 添加EOF token
    tokens_.emplace_back(TokenType::EOF_TOKEN, "", getCurrentPosition());

    LexResult result;
    result.success = errors_.empty();
    result.tokens = tokens_;
    result.errors = errors_;

    return result;
}

void CHTLLexer::initializeKeywords() {
    keywords_ = {
        "text", "style", "script",
        "inherit", "delete", "insert", 
        "after", "before", "replace", "at", "top", "bottom",
        "from", "as", "except"
    };
}

char CHTLLexer::currentChar() const {
    if (currentPos_ >= source_.length()) {
        return '\0';
    }
    return source_[currentPos_];
}

char CHTLLexer::peekChar(size_t offset) const {
    size_t peekPos = currentPos_ + offset;
    if (peekPos >= source_.length()) {
        return '\0';
    }
    return source_[peekPos];
}

void CHTLLexer::advance() {
    if (currentPos_ < source_.length()) {
        if (source_[currentPos_] == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        currentPos_++;
    }
}

bool CHTLLexer::isEOF() const {
    return currentPos_ >= source_.length();
}

Position CHTLLexer::getCurrentPosition() const {
    return Position{static_cast<size_t>(line_), static_cast<size_t>(column_)};
}

void CHTLLexer::skipWhitespace() {
    Position startPos = getCurrentPosition();
    std::string whitespace;
    
    while (!isEOF() && isWhitespace(currentChar()) && currentChar() != '\n') {
        whitespace += currentChar();
        advance();
    }
    
    if (!whitespace.empty()) {
        tokens_.emplace_back(TokenType::WHITESPACE, whitespace, startPos);
    }
}

void CHTLLexer::skipLineComment() {
    Position startPos = getCurrentPosition();
    std::string comment;
    
    // 跳过 //
    advance();
    advance();
    
    // 读取注释内容直到行尾
    while (!isEOF() && currentChar() != '\n') {
        comment += currentChar();
        advance();
    }
    
    tokens_.emplace_back(TokenType::COMMENT_LINE, "//" + comment, startPos);
}

void CHTLLexer::skipMultilineComment() {
    Position startPos = getCurrentPosition();
    std::string comment = "/*";
    
    // 跳过 /*
    advance();
    advance();
    
    // 读取注释内容直到 */
    while (!isEOF()) {
        if (currentChar() == '*' && peekChar() == '/') {
            comment += "*/";
            advance(); // *
            advance(); // /
            break;
        }
        comment += currentChar();
        advance();
    }
    
    tokens_.emplace_back(TokenType::COMMENT_MULTILINE, comment, startPos);
}

Token CHTLLexer::readString(char quote) {
    Position startPos = getCurrentPosition();
    std::string value;
    value += quote;
    
    advance(); // 跳过开始引号
    
    while (!isEOF() && currentChar() != quote) {
        if (currentChar() == '\\') {
            value += currentChar();
            advance();
            if (!isEOF()) {
                value += currentChar();
                advance();
            }
        } else {
            value += currentChar();
            advance();
        }
    }
    
    if (!isEOF() && currentChar() == quote) {
        value += quote;
        advance(); // 跳过结束引号
    } else {
        reportError("未闭合的字符串字面量");
    }
    
    return Token(TokenType::STRING_LITERAL, value, startPos);
}

Token CHTLLexer::readNumber() {
    Position startPos = getCurrentPosition();
    std::string value;
    
    while (!isEOF() && (isDigit(currentChar()) || currentChar() == '.')) {
        value += currentChar();
        advance();
    }
    
    return Token(TokenType::NUMBER, value, startPos);
}

Token CHTLLexer::readIdentifier() {
    Position startPos = getCurrentPosition();
    std::string value;
    
    while (!isEOF() && (isAlphaNumeric(currentChar()) || currentChar() == '_' || currentChar() == '-')) {
        value += currentChar();
        advance();
    }
    
    TokenType type = keywords_.find(value) != keywords_.end() ? TokenType::KEYWORD : TokenType::IDENTIFIER;
    return Token(type, value, startPos);
}

Token CHTLLexer::readOperator() {
    Position startPos = getCurrentPosition();
    std::string value;
    value += currentChar();
    advance();
    
    return Token(TokenType::OPERATOR, value, startPos);
}

bool CHTLLexer::isAlpha(char c) const {
    return std::isalpha(c) || c > 127; // 支持Unicode字符
}

bool CHTLLexer::isDigit(char c) const {
    return std::isdigit(c);
}

bool CHTLLexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool CHTLLexer::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

void CHTLLexer::reportError(const std::string& message) {
    ParseError error;
    error.message = message;
    error.position = getCurrentPosition();
    error.filename = filename_;
    errors_.push_back(error);
}

} // namespace chtl