#include "compiler/chtl/chtl_lexer.h"
#include "common/utils.h"
#include <cctype>
#include <unordered_map>

namespace chtl {
namespace compiler {

// 关键字映射表
static const std::unordered_map<std::string, TokenType> KEYWORDS = {
    // 基础关键字
    {"text", TokenType::KEYWORD_TEXT},
    {"style", TokenType::KEYWORD_STYLE},
    {"script", TokenType::KEYWORD_SCRIPT},
    {"Template", TokenType::KEYWORD_TEMPLATE},
    {"Custom", TokenType::KEYWORD_CUSTOM},
    {"Import", TokenType::KEYWORD_IMPORT},
    {"Namespace", TokenType::KEYWORD_NAMESPACE},
    {"Origin", TokenType::KEYWORD_ORIGIN},
    {"Configuration", TokenType::KEYWORD_CONFIGURATION},
    
    // 模板相关
    {"inherit", TokenType::KEYWORD_INHERIT},
    {"delete", TokenType::KEYWORD_DELETE},
    {"insert", TokenType::KEYWORD_INSERT},
    {"except", TokenType::KEYWORD_EXCEPT},
    {"from", TokenType::KEYWORD_FROM},
    {"as", TokenType::KEYWORD_AS},
    
    // 自定义相关
    {"after", TokenType::KEYWORD_AFTER},
    {"before", TokenType::KEYWORD_BEFORE},
    {"replace", TokenType::KEYWORD_REPLACE},
    {"at", TokenType::KEYWORD_AT},
    {"top", TokenType::KEYWORD_TOP},
    {"bottom", TokenType::KEYWORD_BOTTOM}
};

CHTLLexer::CHTLLexer(const std::string& source, const std::string& filename)
    : source_(source), filename_(filename), position_(0), 
      line_(1), column_(1), lastTokenType_(TokenType::UNKNOWN) {
}

CHTLLexer::~CHTLLexer() = default;

Token CHTLLexer::nextToken() {
    skipWhitespaceAndComments();
    
    if (isAtEnd()) {
        return makeToken(TokenType::EOF_TOKEN);
    }
    
    markTokenStart();
    char ch = advance();
    
    // 处理双字符符号
    if (ch == '-' && peek() == '-') {
        // 语义注释 --
        advance();
        return scanSemanticComment();
    }
    
    if (ch == '-' && peek() == '>') {
        // 箭头操作符 -> (CHTL JS)
        advance();
        return makeToken(TokenType::ARROW);
    }
    
    if (ch == '{' && peek() == '{') {
        // 双左花括号 {{ (CHTL JS增强选择器)
        advance();
        return makeToken(TokenType::DOUBLE_LEFT_BRACE);
    }
    
    if (ch == '}' && peek() == '}') {
        // 双右花括号 }}
        advance();
        return makeToken(TokenType::DOUBLE_RIGHT_BRACE);
    }
    
    // 处理字符串字面量
    if (ch == '"' || ch == '\'') {
        return scanString(ch);
    }
    
    // 处理数字字面量
    if (std::isdigit(ch)) {
        return scanNumber();
    }
    
    // 处理标识符和关键字
    if (std::isalpha(ch) || ch == '_') {
        return scanIdentifier();
    }
    
    // 处理单字符符号
    switch (ch) {
        case '{': return makeToken(TokenType::LEFT_BRACE);
        case '}': return makeToken(TokenType::RIGHT_BRACE);
        case '[': return makeToken(TokenType::LEFT_BRACKET);
        case ']': return makeToken(TokenType::RIGHT_BRACKET);
        case '(': return makeToken(TokenType::LEFT_PAREN);
        case ')': return makeToken(TokenType::RIGHT_PAREN);
        case ';': return makeToken(TokenType::SEMICOLON);
        case ':': return makeToken(TokenType::COLON);
        case '=': return makeToken(TokenType::EQUALS);
        case ',': return makeToken(TokenType::COMMA);
        case '.': return makeToken(TokenType::DOT);
        case '@': return makeToken(TokenType::AT);
        case '&': return makeToken(TokenType::AMPERSAND);
        case '#': return makeToken(TokenType::HASH);
        default:
            // 处理无修饰字面量
            if (canStartUnquotedLiteral(ch)) {
                return scanUnquotedLiteral();
            }
            return makeToken(TokenType::UNKNOWN);
    }
}

bool CHTLLexer::hasMoreTokens() const {
    return position_ < source_.length();
}

void CHTLLexer::reset() {
    position_ = 0;
    line_ = 1;
    column_ = 1;
    lastTokenType_ = TokenType::UNKNOWN;
}

void CHTLLexer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        char ch = peek();
        
        // 跳过空白字符
        if (std::isspace(ch)) {
            if (ch == '\n') {
                line_++;
                column_ = 1;
            } else {
                column_++;
            }
            position_++;
            continue;
        }
        
        // 跳过单行注释 //
        if (ch == '/' && peekNext() == '/') {
            position_ += 2;
            column_ += 2;
            while (!isAtEnd() && peek() != '\n') {
                position_++;
                column_++;
            }
            continue;
        }
        
        // 跳过多行注释 /* */
        if (ch == '/' && peekNext() == '*') {
            position_ += 2;
            column_ += 2;
            while (!isAtEnd()) {
                if (peek() == '*' && peekNext() == '/') {
                    position_ += 2;
                    column_ += 2;
                    break;
                }
                if (peek() == '\n') {
                    line_++;
                    column_ = 1;
                } else {
                    column_++;
                }
                position_++;
            }
            continue;
        }
        
        break;
    }
}

Token CHTLLexer::scanString(char delimiter) {
    std::string value;
    
    while (!isAtEnd() && peek() != delimiter) {
        if (peek() == '\\' && peekNext() == delimiter) {
            // 转义字符
            advance(); // 跳过反斜杠
            value += advance();
        } else if (peek() == '\n') {
            // 字符串中的换行
            line_++;
            column_ = 1;
            value += advance();
        } else {
            value += advance();
        }
    }
    
    if (isAtEnd()) {
        return makeErrorToken("Unterminated string");
    }
    
    advance(); // 跳过结束引号
    return makeToken(TokenType::STRING_LITERAL, value);
}

Token CHTLLexer::scanNumber() {
    while (!isAtEnd() && std::isdigit(peek())) {
        advance();
    }
    
    // 处理小数点
    if (peek() == '.' && std::isdigit(peekNext())) {
        advance(); // 消耗小数点
        while (!isAtEnd() && std::isdigit(peek())) {
            advance();
        }
    }
    
    return makeToken(TokenType::NUMBER_LITERAL);
}

Token CHTLLexer::scanIdentifier() {
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
        advance();
    }
    
    std::string value = source_.substr(tokenStartPos_, position_ - tokenStartPos_);
    
    // 检查是否是关键字
    auto it = KEYWORDS.find(value);
    if (it != KEYWORDS.end()) {
        return makeToken(it->second, value);
    }
    
    return makeToken(TokenType::IDENTIFIER, value);
}

Token CHTLLexer::scanUnquotedLiteral() {
    // 无修饰字面量可以包含除了某些特殊字符外的任何字符
    while (!isAtEnd()) {
        char ch = peek();
        // 遇到这些字符时结束无修饰字面量
        if (ch == '{' || ch == '}' || ch == ';' || ch == ':' || ch == '=' ||
            ch == ',' || ch == '\n' || ch == '[' || ch == ']' || 
            ch == '(' || ch == ')' || std::isspace(ch)) {
            break;
        }
        advance();
    }
    
    std::string value = source_.substr(tokenStartPos_, position_ - tokenStartPos_);
    return makeToken(TokenType::UNQUOTED_LITERAL, value);
}

Token CHTLLexer::scanSemanticComment() {
    // 跳过 -- 后的空格
    while (!isAtEnd() && peek() == ' ') {
        advance();
    }
    
    // 读取到行尾
    std::string comment;
    while (!isAtEnd() && peek() != '\n') {
        comment += advance();
    }
    
    return makeToken(TokenType::SEMANTIC_COMMENT, comment);
}

bool CHTLLexer::canStartUnquotedLiteral(char ch) const {
    // 根据上下文判断是否可以开始无修饰字面量
    // 在属性值或text内容位置时允许
    if (lastTokenType_ == TokenType::COLON || 
        lastTokenType_ == TokenType::EQUALS ||
        lastTokenType_ == TokenType::LEFT_BRACE) {
        // 不是特殊字符且不是空白
        return ch != '{' && ch != '}' && ch != ';' && ch != ':' && 
               ch != '=' && ch != ',' && ch != '[' && ch != ']' && 
               ch != '(' && ch != ')' && !std::isspace(ch);
    }
    return false;
}

char CHTLLexer::advance() {
    column_++;
    return source_[position_++];
}

char CHTLLexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[position_];
}

char CHTLLexer::peekNext() const {
    if (position_ + 1 >= source_.length()) return '\0';
    return source_[position_ + 1];
}

bool CHTLLexer::isAtEnd() const {
    return position_ >= source_.length();
}

void CHTLLexer::markTokenStart() {
    tokenStartPos_ = position_;
    tokenStartLine_ = line_;
    tokenStartColumn_ = column_;
}

Token CHTLLexer::makeToken(TokenType type, const std::string& value) {
    Token token(type);
    
    if (value.empty() && tokenStartPos_ < position_) {
        token.value = source_.substr(tokenStartPos_, position_ - tokenStartPos_);
    } else {
        token.value = value;
    }
    
    token.location.filename = filename_;
    token.location.line = tokenStartLine_;
    token.location.column = tokenStartColumn_;
    token.location.offset = tokenStartPos_;
    
    lastTokenType_ = type;
    return token;
}

Token CHTLLexer::makeErrorToken(const std::string& message) {
    Token token(TokenType::UNKNOWN);
    token.value = message;
    token.location.filename = filename_;
    token.location.line = tokenStartLine_;
    token.location.column = tokenStartColumn_;
    token.location.offset = tokenStartPos_;
    
    return token;
}

} // namespace compiler
} // namespace chtl