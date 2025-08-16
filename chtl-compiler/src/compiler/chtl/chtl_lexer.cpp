#include "compiler/chtl/chtl_lexer.h"
#include "common/utils.h"
#include <cctype>

namespace chtl {
namespace compiler {

// 静态关键字映射
const std::unordered_map<std::string, TokenType> CHTLLexer::keywords_ = {
    {"text", TokenType::KEYWORD_TEXT},
    {"style", TokenType::KEYWORD_STYLE},
    {"script", TokenType::KEYWORD_SCRIPT},
    {"inherit", TokenType::KEYWORD_INHERIT},
    {"delete", TokenType::KEYWORD_DELETE},
    {"insert", TokenType::KEYWORD_INSERT},
    {"except", TokenType::KEYWORD_EXCEPT},
    {"Template", TokenType::KEYWORD_TEMPLATE},
    {"Custom", TokenType::KEYWORD_CUSTOM},
    {"Import", TokenType::KEYWORD_IMPORT},
    {"Namespace", TokenType::KEYWORD_NAMESPACE},
    {"Origin", TokenType::KEYWORD_ORIGIN}
};

CHTLLexer::CHTLLexer() 
    : position_(0), line_(1), column_(1), hasCurrentToken_(false) {
}

CHTLLexer::~CHTLLexer() = default;

void CHTLLexer::init(const std::string& source, const std::string& filename) {
    source_ = source;
    filename_ = filename;
    position_ = 0;
    line_ = 1;
    column_ = 1;
    errors_.clear();
    hasCurrentToken_ = false;
}

Token CHTLLexer::nextToken() {
    skipWhitespace();
    
    if (isEOF()) {
        return makeEOFToken();
    }
    
    char ch = peek();
    
    // 处理注释
    if (ch == '/' && peek(1) == '/') {
        skipComment();
        return nextToken();
    }
    if (ch == '/' && peek(1) == '*') {
        skipComment();
        return nextToken();
    }
    
    // 处理标识符和关键字
    if (isIdentifierStart(ch)) {
        return scanIdentifier();
    }
    
    // 处理字符串字面量
    if (ch == '"' || ch == '\'') {
        return scanString();
    }
    
    // 处理数字
    if (isDigit(ch)) {
        return scanNumber();
    }
    
    // 处理符号
    return scanSymbol();
}

Token CHTLLexer::peekToken() {
    if (!hasCurrentToken_) {
        currentToken_ = nextToken();
        hasCurrentToken_ = true;
    }
    return currentToken_;
}

Token CHTLLexer::consumeToken() {
    if (hasCurrentToken_) {
        hasCurrentToken_ = false;
        return currentToken_;
    }
    return nextToken();
}

bool CHTLLexer::isEOF() const {
    return position_ >= source_.length();
}

SourceLocation CHTLLexer::getCurrentLocation() const {
    return SourceLocation(line_, column_, position_, filename_);
}

void CHTLLexer::skipWhitespace() {
    while (!isEOF() && isWhitespace(peek())) {
        advance();
    }
}

void CHTLLexer::skipComment() {
    if (peek() == '/' && peek(1) == '/') {
        // 单行注释
        advance(); // /
        advance(); // /
        while (!isEOF() && peek() != '\n') {
            advance();
        }
    } else if (peek() == '/' && peek(1) == '*') {
        // 多行注释
        advance(); // /
        advance(); // *
        while (!isEOF()) {
            if (peek() == '*' && peek(1) == '/') {
                advance(); // *
                advance(); // /
                break;
            }
            advance();
        }
    }
}

Token CHTLLexer::scanIdentifier() {
    size_t start = position_;
    size_t startColumn = column_;
    
    while (!isEOF() && isIdentifierPart(peek())) {
        advance();
    }
    
    std::string value = source_.substr(start, position_ - start);
    
    // 检查是否是关键字
    auto it = keywords_.find(value);
    TokenType type = (it != keywords_.end()) ? it->second : TokenType::IDENTIFIER;
    
    Token token = makeToken(type, value);
    token.location.column = startColumn;
    return token;
}

Token CHTLLexer::scanString() {
    size_t start = position_;
    size_t startColumn = column_;
    char quote = peek();
    advance(); // 跳过开始引号
    
    std::string value;
    while (!isEOF() && peek() != quote) {
        if (peek() == '\\') {
            advance();
            if (!isEOF()) {
                char escape = peek();
                switch (escape) {
                    case 'n': value += '\n'; break;
                    case 't': value += '\t'; break;
                    case 'r': value += '\r'; break;
                    case '\\': value += '\\'; break;
                    case '\'': value += '\''; break;
                    case '"': value += '"'; break;
                    default: value += escape; break;
                }
                advance();
            }
        } else {
            value += peek();
            advance();
        }
    }
    
    if (!isEOF()) {
        advance(); // 跳过结束引号
    } else {
        addError("Unterminated string literal");
    }
    
    Token token = makeToken(TokenType::STRING_LITERAL, value);
    token.location.column = startColumn;
    return token;
}

Token CHTLLexer::scanNumber() {
    size_t start = position_;
    size_t startColumn = column_;
    
    while (!isEOF() && isDigit(peek())) {
        advance();
    }
    
    // 处理小数部分
    if (peek() == '.' && isDigit(peek(1))) {
        advance(); // .
        while (!isEOF() && isDigit(peek())) {
            advance();
        }
    }
    
    std::string value = source_.substr(start, position_ - start);
    Token token = makeToken(TokenType::NUMBER_LITERAL, value);
    token.location.column = startColumn;
    return token;
}

Token CHTLLexer::scanSymbol() {
    size_t startColumn = column_;
    char ch = advance();
    TokenType type = TokenType::UNKNOWN;
    std::string value(1, ch);
    
    switch (ch) {
        case '{': type = TokenType::LEFT_BRACE; break;
        case '}': type = TokenType::RIGHT_BRACE; break;
        case '[': type = TokenType::LEFT_BRACKET; break;
        case ']': type = TokenType::RIGHT_BRACKET; break;
        case '(': type = TokenType::LEFT_PAREN; break;
        case ')': type = TokenType::RIGHT_PAREN; break;
        case ';': type = TokenType::SEMICOLON; break;
        case ':': type = TokenType::COLON; break;
        case '=': type = TokenType::EQUALS; break;
        case ',': type = TokenType::COMMA; break;
        case '.': type = TokenType::DOT; break;
        case '@': type = TokenType::AT; break;
        case '&': type = TokenType::AMPERSAND; break;
        default:
            type = TokenType::UNKNOWN;
            break;
    }
    
    Token token = makeToken(type, value);
    token.location.column = startColumn;
    return token;
}

bool CHTLLexer::isIdentifierStart(char ch) const {
    return std::isalpha(ch) || ch == '_';
}

bool CHTLLexer::isIdentifierPart(char ch) const {
    return std::isalnum(ch) || ch == '_';
}

bool CHTLLexer::isDigit(char ch) const {
    return std::isdigit(ch);
}

bool CHTLLexer::isWhitespace(char ch) const {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

char CHTLLexer::peek(size_t offset) const {
    size_t pos = position_ + offset;
    if (pos >= source_.length()) {
        return '\0';
    }
    return source_[pos];
}

char CHTLLexer::advance() {
    if (isEOF()) {
        return '\0';
    }
    
    char ch = source_[position_++];
    if (ch == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    return ch;
}

void CHTLLexer::addError(const std::string& message) {
    errors_.push_back(utils::formatError(filename_, line_, column_, message));
}

Token CHTLLexer::makeToken(TokenType type, const std::string& value) {
    Token token;
    token.type = type;
    token.value = value;
    token.location = getCurrentLocation();
    return token;
}

Token CHTLLexer::makeEOFToken() {
    return makeToken(TokenType::EOF_TOKEN, "");
}

} // namespace compiler
} // namespace chtl