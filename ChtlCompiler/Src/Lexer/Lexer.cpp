#include "Lexer/Lexer.h"
#include <cctype>
#include <algorithm>

namespace Chtl {

// 静态关键字映射表初始化
const std::unordered_map<std::string, TokenType> Lexer::keywords_ = {
    {"text", TokenType::TEXT},
    {"style", TokenType::STYLE},
    {"script", TokenType::SCRIPT},
    {"vir", TokenType::VIR},
    {"inherit", TokenType::INHERIT},
    {"delete", TokenType::DELETE},
    {"insert", TokenType::INSERT},
    {"after", TokenType::AFTER},
    {"before", TokenType::BEFORE},
    {"replace", TokenType::REPLACE},
    {"at", TokenType::IDENTIFIER},  // "at top"和"at bottom"需要特殊处理
    {"top", TokenType::IDENTIFIER},
    {"bottom", TokenType::IDENTIFIER},
    {"from", TokenType::FROM},
    {"as", TokenType::AS},
    {"except", TokenType::EXCEPT}
};

const std::unordered_map<std::string, TokenType> Lexer::markKeywords_ = {
    {"[Template]", TokenType::TEMPLATE_MARK},
    {"[Custom]", TokenType::CUSTOM_MARK},
    {"[Origin]", TokenType::ORIGIN_MARK},
    {"[Import]", TokenType::IMPORT_MARK},
    {"[Namespace]", TokenType::NAMESPACE_MARK},
    {"[Configuration]", TokenType::CONFIGURATION_MARK},
    {"[Info]", TokenType::INFO_MARK},
    {"[Name]", TokenType::INFO_MARK},  // [Name] 也是信息标记
    {"[Export]", TokenType::EXPORT_MARK}
};

const std::unordered_map<std::string, TokenType> Lexer::atKeywords_ = {
    {"@Style", TokenType::AT_STYLE},
    {"@Element", TokenType::AT_ELEMENT},
    {"@Var", TokenType::AT_VAR},
    {"@Html", TokenType::AT_HTML},
    {"@JavaScript", TokenType::AT_JAVASCRIPT},
    {"@Chtl", TokenType::AT_CHTL},
    {"@CJmod", TokenType::AT_CJMOD}
};

Lexer::Lexer() 
    : currentPos_(0),
      currentLine_(1),
      currentColumn_(1),
      tokenStartPos_(0),
      tokenStartLine_(1),
      tokenStartColumn_(1),
      hasCachedToken_(false),
      hasError_(false) {
}

Lexer::~Lexer() {
}

void Lexer::setSource(const std::string& source, const std::string& fileName) {
    source_ = source;
    fileName_ = fileName;
    reset();
}

void Lexer::reset() {
    currentPos_ = 0;
    currentLine_ = 1;
    currentColumn_ = 1;
    tokenStartPos_ = 0;
    tokenStartLine_ = 1;
    tokenStartColumn_ = 1;
    hasCachedToken_ = false;
    hasError_ = false;
    lastError_.clear();
}

Token Lexer::nextToken() {
    if (hasCachedToken_) {
        hasCachedToken_ = false;
        return cachedToken_;
    }
    
    skipWhitespace();
    
    // 记录Token起始位置
    tokenStartPos_ = currentPos_;
    tokenStartLine_ = currentLine_;
    tokenStartColumn_ = currentColumn_;
    
    if (isAtEnd()) {
        return makeToken(TokenType::EOF_TOKEN, "");
    }
    
    char ch = peek();
    
    // 处理注释
    if (ch == '/') {
        if (peek(1) == '/') {
            advance(); advance();
            skipComment();
            return makeToken(TokenType::COMMENT_SINGLE, "");
        } else if (peek(1) == '*') {
            advance(); advance();
            skipComment();
            return makeToken(TokenType::COMMENT_MULTI, "");
        }
    }
    
    // 处理生成器注释
    if (ch == '-' && peek(1) == '-') {
        advance(); advance();
        skipComment();
        return makeToken(TokenType::COMMENT_GENERATOR, "");
    }
    
    // 处理字符串
    if (ch == '"') {
        return readStringDoubleQuote();
    }
    if (ch == '\'') {
        return readStringSingleQuote();
    }
    
    // 处理数字
    if (isDigit(ch)) {
        return readNumber();
    }
    
    // 处理标记关键字 [Template] 等
    if (ch == '[') {
        return readMarkKeyword();
    }
    
    // 处理@关键字
    if (ch == '@') {
        return readAtKeyword();
    }
    
    // 处理标识符和关键字
    if (isAlpha(ch) || ch == '_') {
        return readIdentifier();
    }
    
    // 处理{{和}}（CHTL JS）
    if (ch == '{' && peek(1) == '{') {
        advance(); advance();
        return makeToken(TokenType::DOUBLE_LBRACE, "{{");
    }
    if (ch == '}' && peek(1) == '}') {
        advance(); advance();
        return makeToken(TokenType::DOUBLE_RBRACE, "}}");
    }
    
    // 处理->（CHTL JS）
    if (ch == '-' && peek(1) == '>') {
        advance(); advance();
        return makeToken(TokenType::ARROW, "->");
    }
    
    // 处理单字符操作符
    return readOperator();
}

Token Lexer::peekToken() {
    if (!hasCachedToken_) {
        cachedToken_ = nextToken();
        hasCachedToken_ = true;
    }
    return cachedToken_;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token token;
    
    do {
        token = nextToken();
        // 跳过空白和注释Token
        if (token.type != TokenType::WHITESPACE &&
            token.type != TokenType::COMMENT_SINGLE &&
            token.type != TokenType::COMMENT_MULTI) {
            tokens.push_back(token);
        }
    } while (token.type != TokenType::EOF_TOKEN && token.type != TokenType::ERROR);
    
    return tokens;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && isWhitespace(peek())) {
        advance();
    }
}

void Lexer::skipComment() {
    // 单行注释：跳到行尾
    if (source_[tokenStartPos_] == '/' && source_[tokenStartPos_ + 1] == '/') {
        while (!isAtEnd() && peek() != '\n') {
            advance();
        }
    }
    // 多行注释：跳到*/
    else if (source_[tokenStartPos_] == '/' && source_[tokenStartPos_ + 1] == '*') {
        while (!isAtEnd()) {
            if (peek() == '*' && peek(1) == '/') {
                advance(); advance();
                break;
            }
            advance();
        }
    }
    // 生成器注释：跳到行尾
    else if (source_[tokenStartPos_] == '-' && source_[tokenStartPos_ + 1] == '-') {
        while (!isAtEnd() && peek() != '\n') {
            advance();
        }
    }
}

Token Lexer::readIdentifier() {
    std::string value;
    
    // 读取标识符
    while (!isAtEnd() && (isAlphaNum(peek()) || peek() == '_' || peek() == '-')) {
        value += advance();
    }
    
    // 检查是否是关键字
    TokenType type = identifyKeyword(value);
    
    // 特殊处理 "at top" 和 "at bottom"
    if (value == "at") {
        skipWhitespace();
        size_t savedPos = currentPos_;
        std::string next;
        
        while (!isAtEnd() && isAlpha(peek())) {
            next += advance();
        }
        
        if (next == "top") {
            return makeToken(TokenType::AT_TOP, "at top");
        } else if (next == "bottom") {
            return makeToken(TokenType::AT_BOTTOM, "at bottom");
        } else {
            // 回退
            currentPos_ = savedPos;
            return makeToken(TokenType::IDENTIFIER, value);
        }
    }
    
    return makeToken(type, value);
}

Token Lexer::readNumber() {
    std::string value;
    
    // 读取整数部分
    while (!isAtEnd() && isDigit(peek())) {
        value += advance();
    }
    
    // 读取小数部分
    if (!isAtEnd() && peek() == '.' && isDigit(peek(1))) {
        value += advance(); // 添加.
        while (!isAtEnd() && isDigit(peek())) {
            value += advance();
        }
    }
    
    return makeToken(TokenType::NUMBER, value);
}

Token Lexer::readStringDoubleQuote() {
    std::string value;
    advance(); // 跳过开始的"
    
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\\' && peek(1) == '"') {
            advance(); // 跳过\
            value += advance(); // 添加"
        } else if (peek() == '\n') {
            return makeError("Unterminated string literal");
        } else {
            value += advance();
        }
    }
    
    if (isAtEnd()) {
        return makeError("Unterminated string literal");
    }
    
    advance(); // 跳过结束的"
    return makeToken(TokenType::STRING_DOUBLE_QUOTE, value);
}

Token Lexer::readStringSingleQuote() {
    std::string value;
    advance(); // 跳过开始的'
    
    while (!isAtEnd() && peek() != '\'') {
        if (peek() == '\\' && peek(1) == '\'') {
            advance(); // 跳过\
            value += advance(); // 添加'
        } else if (peek() == '\n') {
            return makeError("Unterminated string literal");
        } else {
            value += advance();
        }
    }
    
    if (isAtEnd()) {
        return makeError("Unterminated string literal");
    }
    
    advance(); // 跳过结束的'
    return makeToken(TokenType::STRING_SINGLE_QUOTE, value);
}

Token Lexer::readUnquotedLiteral() {
    std::string value;
    
    // 读取无引号字面量（用于属性值等）
    while (!isAtEnd() && isUnquotedLiteralChar(peek())) {
        value += advance();
    }
    
    return makeToken(TokenType::UNQUOTED_LITERAL, value);
}

Token Lexer::readMarkKeyword() {
    std::string value;
    
    // 读取[...]格式的关键字
    while (!isAtEnd() && peek() != ']') {
        value += advance();
    }
    
    if (!isAtEnd()) {
        value += advance(); // 添加]
    }
    
    TokenType type = identifyMarkKeyword(value);
    return makeToken(type, value);
}

Token Lexer::readAtKeyword() {
    std::string value;
    value += advance(); // 添加@
    
    // 读取@后面的标识符
    while (!isAtEnd() && (isAlphaNum(peek()) || peek() == '_')) {
        value += advance();
    }
    
    TokenType type = identifyAtKeyword(value);
    return makeToken(type, value);
}

Token Lexer::readOperator() {
    char ch = advance();
    
    switch (ch) {
        case '{': return makeToken(TokenType::LBRACE, "{");
        case '}': return makeToken(TokenType::RBRACE, "}");
        case '(': return makeToken(TokenType::LPAREN, "(");
        case ')': return makeToken(TokenType::RPAREN, ")");
        case '[': return makeToken(TokenType::LBRACKET, "[");
        case ']': return makeToken(TokenType::RBRACKET, "]");
        case ';': return makeToken(TokenType::SEMICOLON, ";");
        case ':': return makeToken(TokenType::COLON, ":");
        case '=': return makeToken(TokenType::EQUALS, "=");
        case ',': return makeToken(TokenType::COMMA, ",");
        case '.': return makeToken(TokenType::DOT, ".");
        case '&': return makeToken(TokenType::AMPERSAND, "&");

        default:
            // 如果不是操作符，可能是无引号字面量的一部分
            currentPos_--; // 回退
            currentColumn_--;
            return readUnquotedLiteral();
    }
}

bool Lexer::isAlpha(char ch) const {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool Lexer::isDigit(char ch) const {
    return ch >= '0' && ch <= '9';
}

bool Lexer::isAlphaNum(char ch) const {
    return isAlpha(ch) || isDigit(ch);
}

bool Lexer::isWhitespace(char ch) const {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

bool Lexer::isOperatorChar(char ch) const {
    return ch == '{' || ch == '}' || ch == '(' || ch == ')' ||
           ch == '[' || ch == ']' || ch == ';' || ch == ':' ||
           ch == '=' || ch == ',' || ch == '.' || ch == '&';
}

bool Lexer::isUnquotedLiteralChar(char ch) const {
    // 无引号字面量可以包含除了特殊字符外的任何字符
    return !isWhitespace(ch) && !isOperatorChar(ch) && ch != '"' && ch != '\'';
}

TokenType Lexer::identifyKeyword(const std::string& text) const {
    auto it = keywords_.find(text);
    return (it != keywords_.end()) ? it->second : TokenType::IDENTIFIER;
}

TokenType Lexer::identifyMarkKeyword(const std::string& text) const {
    auto it = markKeywords_.find(text);
    return (it != markKeywords_.end()) ? it->second : TokenType::IDENTIFIER;
}

TokenType Lexer::identifyAtKeyword(const std::string& text) const {
    auto it = atKeywords_.find(text);
    return (it != atKeywords_.end()) ? it->second : TokenType::IDENTIFIER;
}

char Lexer::peek(int offset) const {
    size_t pos = currentPos_ + offset;
    return (pos < source_.size()) ? source_[pos] : '\0';
}

char Lexer::advance() {
    if (isAtEnd()) return '\0';
    
    char ch = source_[currentPos_++];
    updatePosition(ch);
    return ch;
}

void Lexer::consume() {
    advance();
}

bool Lexer::match(const std::string& text) const {
    if (currentPos_ + text.size() > source_.size()) {
        return false;
    }
    
    return source_.substr(currentPos_, text.size()) == text;
}

bool Lexer::isAtEnd() const {
    return currentPos_ >= source_.size();
}

Token Lexer::makeToken(TokenType type, const std::string& value) {
    return Token(type, value, fileName_, tokenStartLine_, tokenStartColumn_, tokenStartPos_);
}

Token Lexer::makeError(const std::string& message) {
    hasError_ = true;
    lastError_ = message;
    return Token(TokenType::ERROR, message, fileName_, tokenStartLine_, tokenStartColumn_, tokenStartPos_);
}

void Lexer::updatePosition(char ch) {
    if (ch == '\n') {
        currentLine_++;
        currentColumn_ = 1;
    } else {
        currentColumn_++;
    }
}

} // namespace Chtl