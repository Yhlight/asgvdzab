#include "CHTLLexer.hpp"
#include <cctype>
#include <sstream>

namespace chtl {

// 静态成员初始化
const std::unordered_map<std::string, CHTLTokenType> CHTLLexer::keywords_ = CHTLLexer::initializeKeywords();
const std::unordered_map<std::string, CHTLTokenType> CHTLLexer::atDirectives_ = CHTLLexer::initializeAtDirectives();

CHTLLexer::CHTLLexer(CHTLContext& context)
    : context_(context), currentPos_(0), currentLine_(1), currentColumn_(1) {
}

CHTLLexer::~CHTLLexer() = default;

std::vector<CHTLToken> CHTLLexer::tokenize(const std::string& source) {
    source_ = source;
    currentPos_ = 0;
    currentLine_ = 1;
    currentColumn_ = 1;
    tokens_.clear();
    
    while (!isAtEnd()) {
        skipWhitespace();
        if (!isAtEnd()) {
            scanToken();
        }
    }
    
    addToken(CHTLTokenType::EOF_TOKEN);
    return tokens_;
}

char CHTLLexer::peek(size_t offset) const {
    size_t pos = currentPos_ + offset;
    return (pos < source_.length()) ? source_[pos] : '\0';
}

char CHTLLexer::consume() {
    if (isAtEnd()) return '\0';
    
    char c = source_[currentPos_++];
    if (c == '\n') {
        currentLine_++;
        currentColumn_ = 1;
    } else {
        currentColumn_++;
    }
    context_.setCurrentPosition(currentLine_, currentColumn_);
    return c;
}

void CHTLLexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\r' || c == '\t' || c == '\n') {
            consume();
        } else {
            break;
        }
    }
}

void CHTLLexer::scanToken() {
    char c = peek();
    
    switch (c) {
        case '{': 
            consume(); 
            addToken(CHTLTokenType::LEFT_BRACE);
            context_.getStateManager().enterBrace();
            
            // 检查是否刚进入style块
            if (!tokens_.empty() && tokens_.size() >= 2) {
                auto& prevToken = tokens_[tokens_.size() - 2];
                if (prevToken.type == CHTLTokenType::STYLE) {
                    context_.enterStyleBlock();
                }
            }
            break;
            
        case '}': 
            consume(); 
            addToken(CHTLTokenType::RIGHT_BRACE);
            context_.getStateManager().exitBrace();
            
            // 检查是否退出style块
            if (context_.isInStyleBlock() && context_.getStateManager().getBraceDepth() == 0) {
                context_.exitStyleBlock();
            }
            break;
            
        case '[': scanBracketBlock(); break;
        case ']': consume(); addToken(CHTLTokenType::RIGHT_BRACKET); break;
        case '(': consume(); addToken(CHTLTokenType::LEFT_PAREN); break;
        case ')': consume(); addToken(CHTLTokenType::RIGHT_PAREN); break;
        case ';': consume(); addToken(CHTLTokenType::SEMICOLON); break;
        case ',': consume(); addToken(CHTLTokenType::COMMA); break;
        case '.': consume(); addToken(CHTLTokenType::DOT); break;
        case '&': consume(); addToken(CHTLTokenType::AMPERSAND); break;
        case '@': scanAtDirective(); break;
        
        case ':':
            consume();
            addToken(CHTLTokenType::COLON);
            break;
            
        case '=':
            consume();
            addToken(CHTLTokenType::EQUALS);
            break;
            
        case '"':
        case '\'':
            scanString(c);
            break;
            
        case '/':
            if (peek(1) == '/' || peek(1) == '*') {
                scanComment();
            } else {
                reportError("未预期的字符 '/'");
                consume();
            }
            break;
            
        case '-':
            if (peek(1) == '-') {
                // HTML注释
                consume(); // -
                consume(); // -
                addToken(CHTLTokenType::COMMENT_HTML, "--");
            } else if (isDigit(peek(1))) {
                scanNumber();
            } else if (shouldScanUnquotedLiteral()) {
                scanUnquotedLiteral();
            } else {
                reportError("未预期的字符 '-'");
                consume();
            }
            break;
            
        default:
            if (isDigit(c)) {
                scanNumber();
            } else if (isValidIdentifierStart(c)) {
                scanIdentifierOrKeyword();
            } else if (shouldScanUnquotedLiteral()) {
                scanUnquotedLiteral();
            } else {
                reportError(std::string("未预期的字符 '") + c + "'");
                consume();
            }
            break;
    }
}

void CHTLLexer::scanIdentifierOrKeyword() {
    size_t startPos = currentPos_;
    size_t startLine = currentLine_;
    size_t startColumn = currentColumn_;
    
    while (!isAtEnd() && isValidIdentifierChar(peek())) {
        consume();
    }
    
    std::string value = source_.substr(startPos, currentPos_ - startPos);
    
    // 检查是否是关键字
    auto it = keywords_.find(value);
    if (it != keywords_.end()) {
        tokens_.emplace_back(it->second, value, startLine, startColumn);
    } else {
        tokens_.emplace_back(CHTLTokenType::IDENTIFIER, value, startLine, startColumn);
    }
}

void CHTLLexer::scanString(char quote) {
    size_t startLine = currentLine_;
    size_t startColumn = currentColumn_;
    
    consume(); // 消费开始引号
    
    std::string value;
    while (!isAtEnd() && peek() != quote) {
        if (peek() == '\\') {
            consume();
            if (!isAtEnd()) {
                char escaped = consume();
                switch (escaped) {
                    case 'n': value += '\n'; break;
                    case 't': value += '\t'; break;
                    case 'r': value += '\r'; break;
                    case '\\': value += '\\'; break;
                    case '"': value += '"'; break;
                    case '\'': value += '\''; break;
                    default: value += escaped; break;
                }
            }
        } else {
            value += consume();
        }
    }
    
    if (isAtEnd()) {
        reportError("未结束的字符串");
    } else {
        consume(); // 消费结束引号
    }
    
    tokens_.emplace_back(CHTLTokenType::STRING_LITERAL, value, startLine, startColumn);
}

void CHTLLexer::scanNumber() {
    size_t startPos = currentPos_;
    size_t startLine = currentLine_;
    size_t startColumn = currentColumn_;
    
    // 处理负号
    if (peek() == '-') {
        consume();
    }
    
    // 整数部分
    while (!isAtEnd() && isDigit(peek())) {
        consume();
    }
    
    // 小数部分
    if (peek() == '.' && isDigit(peek(1))) {
        consume(); // .
        while (!isAtEnd() && isDigit(peek())) {
            consume();
        }
    }
    
    // 检查是否后面跟着单位（如px, %, em等）
    if (!isAtEnd() && (peek() == '%' || isAlpha(peek()))) {
        // 如果是在样式块中，将数字和单位作为一个无引号字面量处理
        if (context_.isInStyleBlock() || shouldScanUnquotedLiteral()) {
            // 回退到开始位置
            currentPos_ = startPos;
            scanUnquotedLiteral();
            return;
        }
    }
    
    std::string value = source_.substr(startPos, currentPos_ - startPos);
    tokens_.emplace_back(CHTLTokenType::NUMBER_LITERAL, value, startLine, startColumn);
}

void CHTLLexer::scanComment() {
    if (peek() == '/' && peek(1) == '/') {
        // 单行注释
        consume(); // /
        consume(); // /
        size_t startPos = currentPos_;
        while (!isAtEnd() && peek() != '\n') {
            consume();
        }
        std::string value = source_.substr(startPos, currentPos_ - startPos);
        addToken(CHTLTokenType::COMMENT_LINE, value);
    } else if (peek() == '/' && peek(1) == '*') {
        // 多行注释
        consume(); // /
        consume(); // *
        size_t startPos = currentPos_;
        while (!isAtEnd()) {
            if (peek() == '*' && peek(1) == '/') {
                std::string value = source_.substr(startPos, currentPos_ - startPos);
                consume(); // *
                consume(); // /
                addToken(CHTLTokenType::COMMENT_BLOCK, value);
                return;
            }
            consume();
        }
        reportError("未结束的注释");
    }
}

void CHTLLexer::scanBracketBlock() {
    size_t startLine = currentLine_;
    size_t startColumn = currentColumn_;
    
    consume(); // [
    
    std::string content;
    while (!isAtEnd() && peek() != ']') {
        content += consume();
    }
    
    if (isAtEnd()) {
        reportError("未结束的方括号");
        return;
    }
    
    consume(); // ]
    
    // 识别特殊的方括号块
    if (content == "Template") {
        tokens_.emplace_back(CHTLTokenType::TEMPLATE, "[Template]", startLine, startColumn);
    } else if (content == "Custom") {
        tokens_.emplace_back(CHTLTokenType::CUSTOM, "[Custom]", startLine, startColumn);
    } else if (content == "Origin") {
        tokens_.emplace_back(CHTLTokenType::ORIGIN, "[Origin]", startLine, startColumn);
    } else if (content == "Import") {
        tokens_.emplace_back(CHTLTokenType::IMPORT, "[Import]", startLine, startColumn);
    } else if (content == "Namespace") {
        tokens_.emplace_back(CHTLTokenType::NAMESPACE, "[Namespace]", startLine, startColumn);
    } else if (content == "Configuration") {
        tokens_.emplace_back(CHTLTokenType::CONFIGURATION, "[Configuration]", startLine, startColumn);
    } else if (content == "Name") {
        tokens_.emplace_back(CHTLTokenType::NAME, "[Name]", startLine, startColumn);
    } else if (content == "Info") {
        tokens_.emplace_back(CHTLTokenType::INFO, "[Info]", startLine, startColumn);
    } else if (content == "Export") {
        tokens_.emplace_back(CHTLTokenType::EXPORT, "[Export]", startLine, startColumn);
    } else {
        // 普通方括号内容，作为标识符处理
        tokens_.emplace_back(CHTLTokenType::LEFT_BRACKET, "[", startLine, startColumn);
        if (!content.empty()) {
            tokens_.emplace_back(CHTLTokenType::IDENTIFIER, content, startLine, startColumn + 1);
        }
        tokens_.emplace_back(CHTLTokenType::RIGHT_BRACKET, "]", currentLine_, currentColumn_ - 1);
    }
}

void CHTLLexer::scanAtDirective() {
    size_t startLine = currentLine_;
    size_t startColumn = currentColumn_;
    
    consume(); // @
    
    if (!isAlpha(peek())) {
        addToken(CHTLTokenType::AT);
        return;
    }
    
    size_t startPos = currentPos_;
    while (!isAtEnd() && isAlphaNumeric(peek())) {
        consume();
    }
    
    std::string directive = source_.substr(startPos, currentPos_ - startPos);
    
    auto it = atDirectives_.find(directive);
    if (it != atDirectives_.end()) {
        tokens_.emplace_back(it->second, "@" + directive, startLine, startColumn);
    } else {
        // 未知的@指令，作为AT + IDENTIFIER处理
        tokens_.emplace_back(CHTLTokenType::AT, "@", startLine, startColumn);
        tokens_.emplace_back(CHTLTokenType::IDENTIFIER, directive, startLine, startColumn + 1);
    }
}

void CHTLLexer::scanUnquotedLiteral() {
    size_t startPos = currentPos_;
    size_t startLine = currentLine_;
    size_t startColumn = currentColumn_;
    
    // 无引号字面量可以包含字母、数字、连字符、下划线、百分号等
    while (!isAtEnd()) {
        char c = peek();
        if (c == ';' || c == '{' || c == '}' || c == ',' || 
            c == '(' || c == ')' || c == '[' || c == ']' ||
            (std::isspace(c) && c != '\n' && !context_.isInStyleBlock())) {
            break;
        }
        if (c == ':' && !context_.isInStyleBlock()) {
            // 在非样式块中，冒号是分隔符
            break;
        }
        consume();
    }
    
    std::string value = source_.substr(startPos, currentPos_ - startPos);
    tokens_.emplace_back(CHTLTokenType::UNQUOTED_LITERAL, value, startLine, startColumn);
}

bool CHTLLexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool CHTLLexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool CHTLLexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool CHTLLexer::isValidIdentifierStart(char c) const {
    return isAlpha(c) || c == '_';
}

bool CHTLLexer::isValidIdentifierChar(char c) const {
    return isAlphaNumeric(c) || c == '_' || c == '-';
}

bool CHTLLexer::isAtEnd() const {
    return currentPos_ >= source_.length();
}

bool CHTLLexer::shouldScanUnquotedLiteral() const {
    // 在属性值位置、样式值位置等可以使用无引号字面量
    auto state = context_.getStateManager().getCurrentParserState();
    return state == ParserState::IN_ATTRIBUTE || 
           state == ParserState::IN_STYLE ||
           context_.isInStyleBlock();
}

void CHTLLexer::addToken(CHTLTokenType type) {
    tokens_.emplace_back(type, "", currentLine_, currentColumn_);
}

void CHTLLexer::addToken(CHTLTokenType type, const std::string& value) {
    tokens_.emplace_back(type, value, currentLine_, currentColumn_);
}

void CHTLLexer::reportError(const std::string& message) {
    context_.addError(message, currentLine_, currentColumn_);
}

std::unordered_map<std::string, CHTLTokenType> CHTLLexer::initializeKeywords() {
    return {
        {"text", CHTLTokenType::TEXT},
        {"style", CHTLTokenType::STYLE},
        {"script", CHTLTokenType::SCRIPT},
        {"inherit", CHTLTokenType::INHERIT},
        {"delete", CHTLTokenType::DELETE},
        {"insert", CHTLTokenType::INSERT},
        {"after", CHTLTokenType::AFTER},
        {"before", CHTLTokenType::BEFORE},
        {"replace", CHTLTokenType::REPLACE},
        {"at", CHTLTokenType::AT_TOP},  // "at top" 或 "at bottom"
        {"top", CHTLTokenType::AT_TOP},
        {"bottom", CHTLTokenType::AT_BOTTOM},
        {"from", CHTLTokenType::FROM},
        {"as", CHTLTokenType::AS},
        {"except", CHTLTokenType::EXCEPT}
    };
}

std::unordered_map<std::string, CHTLTokenType> CHTLLexer::initializeAtDirectives() {
    return {
        {"Style", CHTLTokenType::AT_STYLE},
        {"Element", CHTLTokenType::AT_ELEMENT},
        {"Var", CHTLTokenType::AT_VAR},
        {"Html", CHTLTokenType::AT_HTML},
        {"JavaScript", CHTLTokenType::AT_JAVASCRIPT},
        {"Chtl", CHTLTokenType::AT_CHTL},
        {"CJmod", CHTLTokenType::AT_CJMOD}
    };
}

} // namespace chtl