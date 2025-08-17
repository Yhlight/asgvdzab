#include "CHTL/Lexer.hpp"
#include <cctype>
#include <unordered_set>

namespace CHTL {

// 初始化关键字映射
std::unordered_map<std::string, TokenType> Lexer::keywords_ = {
    {"text", TokenType::TEXT},
    {"style", TokenType::STYLE},
    {"Template", TokenType::TEMPLATE},
    {"Custom", TokenType::CUSTOM},
    {"Namespace", TokenType::NAMESPACE},
    {"Import", TokenType::IMPORT},
    {"Configuration", TokenType::CONFIGURATION},
    {"Origin", TokenType::ORIGIN},
    {"from", TokenType::FROM},
    {"as", TokenType::AS},
    {"inherit", TokenType::INHERIT},
    {"except", TokenType::EXCEPT},
    {"delete", TokenType::DELETE},
    {"add", TokenType::ADD}
};

// ========== Token 实现 ==========

std::string Token::toString() const {
    return TokenUtils::getTokenTypeName(type) + "(\"" + value + "\")";
}

// ========== Lexer 实现 ==========

Lexer::Lexer(const std::string& source, const std::string& filename)
    : source_(source), filename_(filename), position_(0), line_(1), column_(1),
      skipWhitespace_(true), skipComments_(false) {
}

Token Lexer::nextToken() {
    while (!isAtEnd()) {
        // 跳过空白字符
        if (skipWhitespace_ && isWhitespace(currentChar())) {
            skipWhitespace();
            continue;
        }
        
        char c = currentChar();
        
        // 处理换行
        if (isNewline(c)) {
            Token token = makeToken(TokenType::NEWLINE, std::string(1, c));
            advance();
            return token;
        }
        
                 // 处理注释
        if (c == '/') {
            if (peekChar() == '/') {
                if (skipComments_) {
                    skipSingleLineComment();
                    continue;
                } else {
                    std::string content = "//";
                    advance();
                    advance();
                    while (!isAtEnd() && !isNewline(currentChar())) {
                        content += currentChar();
                        advance();
                    }
                    return makeToken(TokenType::SINGLE_LINE_COMMENT, content);
                }
            } else if (peekChar() == '*') {
                if (skipComments_) {
                    skipMultiLineComment();
                    continue;
                } else {
                    std::string content = "/*";
                    advance();
                    advance();
                    while (!isAtEnd()) {
                        if (currentChar() == '*' && peekChar() == '/') {
                            content += "*/";
                            advance();
                            advance();
                            break;
                        }
                        content += currentChar();
                        advance();
                    }
                    return makeToken(TokenType::MULTI_LINE_COMMENT, content);
                }
            }
        }
        
        // 处理生成器注释 --
        if (c == '-' && peekChar() == '-') {
            return makeGeneratorComment();
        }
        
        // 处理字符串字面量
        if (c == '"') {
            return makeStringLiteral();
        }
        
        if (c == '\'') {
            return makeSingleStringLiteral();
        }
        
        // 处理数字
        if (isDigit(c)) {
            return makeNumberLiteral();
        }
        
        // 处理标识符和关键字
        if (isValidIdentifierStart(c)) {
            return makeIdentifierOrKeyword();
        }
        
        // 处理CSS选择器
        if (c == '.' && isAlpha(peekChar())) {
            return makeCssSelector();
        }
        
        if (c == '#' && isAlphaNumeric(peekChar())) {
            return makeCssSelector();
        }
        
        // 处理伪选择器
        if (c == ':') {
            if (peekChar() == ':') {
                return makePseudoSelector();
            } else if (isAlpha(peekChar())) {
                return makePseudoSelector();
            }
        }
        
        // 处理单字符token
        switch (c) {
            case '{': advance(); return makeToken(TokenType::LEFT_BRACE, "{");
            case '}': advance(); return makeToken(TokenType::RIGHT_BRACE, "}");
            case '[': advance(); return makeToken(TokenType::LEFT_BRACKET, "[");
            case ']': advance(); return makeToken(TokenType::RIGHT_BRACKET, "]");
            case '(': advance(); return makeToken(TokenType::LEFT_PAREN, "(");
            case ')': advance(); return makeToken(TokenType::RIGHT_PAREN, ")");
            case ';': advance(); return makeToken(TokenType::SEMICOLON, ";");
            case ',': advance(); return makeToken(TokenType::COMMA, ",");
            case '&': advance(); return makeToken(TokenType::AMPERSAND, "&");
            case '#': advance(); return makeToken(TokenType::HASH, "#");
            case '@': advance(); return makeToken(TokenType::AT, "@");
            case '.': advance(); return makeToken(TokenType::DOT, ".");
            case '=': advance(); return makeToken(TokenType::EQUAL, "=");
            case ':': 
                advance(); 
                if (currentChar() == ':') {
                    advance();
                    return makeToken(TokenType::DOUBLE_COLON, "::");
                }
                return makeToken(TokenType::COLON, ":");
            case '-':
                advance();
                if (currentChar() == '>') {
                    advance();
                    return makeToken(TokenType::ARROW, "->");
                }
                // 这里应该回退，但为简化实现，我们将其作为无引号字面量处理
                return makeUnquotedLiteral();
        }
        
        // 处理无引号字面量
        if (isValidUnquotedChar(c)) {
            return makeUnquotedLiteral();
        }
        
        // 未知字符
        std::string errorMsg = "未知字符: ";
        errorMsg += c;
        addError(errorMsg);
        advance();
        return makeErrorToken(errorMsg);
    }
    
    return makeToken(TokenType::END_OF_FILE);
}

Token Lexer::peekToken() {
    size_t savedPos = position_;
    size_t savedLine = line_;
    size_t savedColumn = column_;
    
    Token token = nextToken();
    
    position_ = savedPos;
    line_ = savedLine;
    column_ = savedColumn;
    
    return token;
}

SourceLocation Lexer::getCurrentLocation() const {
    return makeLocation();
}

bool Lexer::isAtEnd() const {
    return position_ >= source_.length();
}

std::vector<Token> Lexer::tokenizeAll() {
    std::vector<Token> tokens;
    
    while (!isAtEnd()) {
        Token token = nextToken();
        tokens.push_back(token);
        
        if (token.type == TokenType::END_OF_FILE) {
            break;
        }
    }
    
    return tokens;
}

// ========== 私有方法实现 ==========

char Lexer::currentChar() const {
    if (isAtEnd()) return '\0';
    return source_[position_];
}

char Lexer::peekChar(size_t offset) const {
    size_t pos = position_ + offset;
    if (pos >= source_.length()) return '\0';
    return source_[pos];
}

void Lexer::advance() {
    if (!isAtEnd()) {
        updatePosition(currentChar());
        position_++;
    }
}

bool Lexer::match(char expected) {
    if (isAtEnd() || currentChar() != expected) {
        return false;
    }
    advance();
    return true;
}

bool Lexer::match(const std::string& expected) {
    for (size_t i = 0; i < expected.length(); ++i) {
        if (peekChar(i) != expected[i]) {
            return false;
        }
    }
    
    for (size_t i = 0; i < expected.length(); ++i) {
        advance();
    }
    return true;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && isWhitespace(currentChar()) && !isNewline(currentChar())) {
        advance();
    }
}

void Lexer::skipSingleLineComment() {
    advance(); // 跳过第一个 /
    advance(); // 跳过第二个 /
    
    while (!isAtEnd() && !isNewline(currentChar())) {
        advance();
    }
}

void Lexer::skipMultiLineComment() {
    advance(); // 跳过 /
    advance(); // 跳过 *
    
    while (!isAtEnd()) {
        if (currentChar() == '*' && peekChar() == '/') {
            advance(); // 跳过 *
            advance(); // 跳过 /
            break;
        }
        advance();
    }
}

Token Lexer::makeToken(TokenType type, const std::string& value) {
    return Token(type, value, makeLocation());
}

Token Lexer::makeStringLiteral() {
    SourceLocation startLoc = makeLocation();
    std::string value;
    
    advance(); // 跳过开始的引号
    
    while (!isAtEnd() && currentChar() != '"') {
        if (currentChar() == '\\') {
            advance(); // 跳过反斜杠
            if (!isAtEnd()) {
                char escaped = currentChar();
                switch (escaped) {
                    case 'n': value += '\n'; break;
                    case 't': value += '\t'; break;
                    case 'r': value += '\r'; break;
                    case '\\': value += '\\'; break;
                    case '"': value += '"'; break;
                    default: value += escaped; break;
                }
                advance();
            }
        } else {
            value += currentChar();
            advance();
        }
    }
    
    if (isAtEnd()) {
        addError("未终止的字符串字面量");
        return makeErrorToken("未终止的字符串");
    }
    
    advance(); // 跳过结束的引号
    return Token(TokenType::STRING_LITERAL, value, startLoc);
}

Token Lexer::makeSingleStringLiteral() {
    SourceLocation startLoc = makeLocation();
    std::string value;
    
    advance(); // 跳过开始的引号
    
    while (!isAtEnd() && currentChar() != '\'') {
        if (currentChar() == '\\') {
            advance(); // 跳过反斜杠
            if (!isAtEnd()) {
                char escaped = currentChar();
                switch (escaped) {
                    case 'n': value += '\n'; break;
                    case 't': value += '\t'; break;
                    case 'r': value += '\r'; break;
                    case '\\': value += '\\'; break;
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
    
    if (isAtEnd()) {
        addError("未终止的单引号字符串");
        return makeErrorToken("未终止的单引号字符串");
    }
    
    advance(); // 跳过结束的引号
    return Token(TokenType::SINGLE_STRING, value, startLoc);
}

Token Lexer::makeUnquotedLiteral() {
    SourceLocation startLoc = makeLocation();
    std::string value;
    
    while (!isAtEnd() && isValidUnquotedChar(currentChar())) {
        value += currentChar();
        advance();
    }
    
    return Token(TokenType::UNQUOTED_LITERAL, value, startLoc);
}

Token Lexer::makeNumberLiteral() {
    SourceLocation startLoc = makeLocation();
    std::string value;
    
    while (!isAtEnd() && (isDigit(currentChar()) || currentChar() == '.')) {
        value += currentChar();
        advance();
    }
    
    return Token(TokenType::NUMBER_LITERAL, value, startLoc);
}

Token Lexer::makeIdentifierOrKeyword() {
    SourceLocation startLoc = makeLocation();
    std::string value;
    
    while (!isAtEnd() && isValidIdentifierChar(currentChar())) {
        value += currentChar();
        advance();
    }
    
    // 检查是否是关键字
    auto it = keywords_.find(value);
    if (it != keywords_.end()) {
        return Token(it->second, value, startLoc);
    }
    
    return Token(TokenType::IDENTIFIER, value, startLoc);
}

Token Lexer::makeGeneratorComment() {
    SourceLocation startLoc = makeLocation();
    std::string value = "--";
    
    advance(); // 跳过第一个 -
    advance(); // 跳过第二个 -
    
    // 读取注释内容直到行尾
    while (!isAtEnd() && !isNewline(currentChar())) {
        value += currentChar();
        advance();
    }
    
    return Token(TokenType::GENERATOR_COMMENT, value, startLoc);
}

Token Lexer::makeCssSelector() {
    SourceLocation startLoc = makeLocation();
    std::string value;
    
    char first = currentChar();
    value += first;
    advance();
    
    // 读取选择器名称
    while (!isAtEnd() && (isAlphaNumeric(currentChar()) || currentChar() == '-' || currentChar() == '_')) {
        value += currentChar();
        advance();
    }
    
    TokenType type = (first == '.') ? TokenType::CLASS_SELECTOR : TokenType::ID_SELECTOR;
    return Token(type, value, startLoc);
}

Token Lexer::makePseudoSelector() {
    SourceLocation startLoc = makeLocation();
    std::string value;
    
    if (currentChar() == ':' && peekChar() == ':') {
        // 伪元素 ::
        value += "::";
        advance();
        advance();
    } else {
        // 伪类 :
        value += ":";
        advance();
    }
    
    // 读取伪选择器名称
    while (!isAtEnd() && (isAlphaNumeric(currentChar()) || currentChar() == '-')) {
        value += currentChar();
        advance();
    }
    
    TokenType type = (value.length() >= 2 && value.substr(0, 2) == "::") ? TokenType::PSEUDO_ELEMENT : TokenType::PSEUDO_CLASS;
    return Token(type, value, startLoc);
}

// ========== 字符判断方法 ==========

bool Lexer::isAlpha(char c) const {
    return std::isalpha(c) || c == '_';
}

bool Lexer::isDigit(char c) const {
    return std::isdigit(c);
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool Lexer::isHexDigit(char c) const {
    return std::isxdigit(c);
}

bool Lexer::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\r';
}

bool Lexer::isNewline(char c) const {
    return c == '\n';
}

bool Lexer::isValidUnquotedChar(char c) const {
    // CHTL允许的无引号字符
    return isAlphaNumeric(c) || c == '-' || c == '_' || c == '/' || c == '%' || c == '.' || c == '#';
}

bool Lexer::isValidIdentifierStart(char c) const {
    return isAlpha(c);
}

bool Lexer::isValidIdentifierChar(char c) const {
    return isAlphaNumeric(c) || c == '-' || c == '_';
}

// ========== 错误处理 ==========

void Lexer::addError(const std::string& message) {
    errors_.push_back(message + " at line " + std::to_string(line_) + 
                     ", column " + std::to_string(column_));
}

Token Lexer::makeErrorToken(const std::string& message) {
    return Token(TokenType::INVALID, message, makeLocation());
}

// ========== 位置更新 ==========

void Lexer::updatePosition(char c) {
    if (c == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
}

SourceLocation Lexer::makeLocation() const {
    return SourceLocation(filename_, line_, column_, position_);
}

// ========== TokenUtils 实现 ==========

namespace TokenUtils {

std::string getTokenTypeName(TokenType type) {
    switch (type) {
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING_LITERAL: return "STRING";
        case TokenType::SINGLE_STRING: return "SINGLE_STRING";
        case TokenType::UNQUOTED_LITERAL: return "UNQUOTED";
        case TokenType::NUMBER_LITERAL: return "NUMBER";
        case TokenType::TEXT: return "TEXT";
        case TokenType::STYLE: return "STYLE";
        case TokenType::TEMPLATE: return "TEMPLATE";
        case TokenType::CUSTOM: return "CUSTOM";
        case TokenType::NAMESPACE: return "NAMESPACE";
        case TokenType::IMPORT: return "IMPORT";
        case TokenType::CONFIGURATION: return "CONFIGURATION";
        case TokenType::ORIGIN: return "ORIGIN";
        case TokenType::FROM: return "FROM";
        case TokenType::AS: return "AS";
        case TokenType::INHERIT: return "INHERIT";
        case TokenType::EXCEPT: return "EXCEPT";
        case TokenType::DELETE: return "DELETE";
        case TokenType::ADD: return "ADD";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COLON: return "COLON";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::DOT: return "DOT";
        case TokenType::COMMA: return "COMMA";
        case TokenType::AMPERSAND: return "AMPERSAND";
        case TokenType::HASH: return "HASH";
        case TokenType::AT: return "AT";
        case TokenType::ARROW: return "ARROW";
        case TokenType::DOUBLE_COLON: return "DOUBLE_COLON";
        case TokenType::SINGLE_LINE_COMMENT: return "SINGLE_COMMENT";
        case TokenType::MULTI_LINE_COMMENT: return "MULTI_COMMENT";
        case TokenType::GENERATOR_COMMENT: return "GENERATOR_COMMENT";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::WHITESPACE: return "WHITESPACE";
        case TokenType::CLASS_SELECTOR: return "CLASS_SELECTOR";
        case TokenType::ID_SELECTOR: return "ID_SELECTOR";
        case TokenType::PSEUDO_CLASS: return "PSEUDO_CLASS";
        case TokenType::PSEUDO_ELEMENT: return "PSEUDO_ELEMENT";
        case TokenType::INVALID: return "INVALID";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN_TOKEN";
    }
}

bool isKeyword(TokenType type) {
    return type >= TokenType::TEXT && type <= TokenType::ADD;
}

bool isLiteral(TokenType type) {
    return type == TokenType::STRING_LITERAL || type == TokenType::SINGLE_STRING ||
           type == TokenType::UNQUOTED_LITERAL || type == TokenType::NUMBER_LITERAL;
}

bool isOperator(TokenType type) {
    return type == TokenType::COLON || type == TokenType::EQUAL || 
           type == TokenType::ARROW || type == TokenType::DOUBLE_COLON ||
           type == TokenType::DOT || type == TokenType::AMPERSAND;
}

bool isDelimiter(TokenType type) {
    return type == TokenType::LEFT_BRACE || type == TokenType::RIGHT_BRACE ||
           type == TokenType::LEFT_BRACKET || type == TokenType::RIGHT_BRACKET ||
           type == TokenType::LEFT_PAREN || type == TokenType::RIGHT_PAREN ||
           type == TokenType::SEMICOLON || type == TokenType::COMMA;
}

bool isComment(TokenType type) {
    return type == TokenType::SINGLE_LINE_COMMENT || type == TokenType::MULTI_LINE_COMMENT ||
           type == TokenType::GENERATOR_COMMENT;
}

std::vector<std::string> getCHTLKeywords() {
    return {
        "text", "style", "Template", "Custom", "Namespace", "Import",
        "Configuration", "Origin", "from", "as", "inherit", "except", "delete", "add"
    };
}

bool isValidHtmlTag(const std::string& tag) {
    static std::unordered_set<std::string> htmlTags = {
        "html", "head", "title", "body", "div", "span", "p", "a", "img", "br", "hr",
        "h1", "h2", "h3", "h4", "h5", "h6", "ul", "ol", "li", "table", "tr", "td", "th",
        "form", "input", "button", "textarea", "select", "option", "label",
        "header", "footer", "nav", "main", "section", "article", "aside"
    };
    return htmlTags.find(tag) != htmlTags.end();
}

bool isValidCssProperty(const std::string& property) {
    static std::unordered_set<std::string> cssProperties = {
        "color", "background", "background-color", "font-size", "font-family", "font-weight",
        "width", "height", "margin", "padding", "border", "display", "position",
        "top", "left", "right", "bottom", "text-align", "line-height", "opacity"
    };
    return cssProperties.find(property) != cssProperties.end();
}

} // namespace TokenUtils

} // namespace ast
} // namespace chtl