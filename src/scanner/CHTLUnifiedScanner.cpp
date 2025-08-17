#include "../../include/scanner/CHTLUnifiedScanner.h"
#include "../../include/common/Utils.h"
#include <iostream>
#include <cctype>

namespace CHTL {

CHTLUnifiedScanner::CHTLUnifiedScanner()
    : current_position(0), current_line(1), current_column(1),
      in_string(false), in_comment(false), string_delimiter('\0'),
      brace_level(0), paren_level(0), bracket_level(0) {
}

CHTLUnifiedScanner::CHTLUnifiedScanner(const std::string& source)
    : source_code(source), current_position(0), current_line(1), current_column(1),
      in_string(false), in_comment(false), string_delimiter('\0'),
      brace_level(0), paren_level(0), bracket_level(0) {
}

void CHTLUnifiedScanner::setSource(const std::string& source) {
    source_code = source;
    reset();
}

void CHTLUnifiedScanner::setConfiguration(const Configuration& cfg) {
    config = cfg;
}

char CHTLUnifiedScanner::peek(size_t offset) const {
    size_t pos = current_position + offset;
    return (pos < source_code.length()) ? source_code[pos] : '\0';
}

char CHTLUnifiedScanner::advance() {
    if (current_position >= source_code.length()) {
        return '\0';
    }
    
    char c = source_code[current_position++];
    
    if (c == '\n') {
        current_line++;
        current_column = 1;
    } else {
        current_column++;
    }
    
    updateNestingLevel(c);
    return c;
}

void CHTLUnifiedScanner::updateNestingLevel(char c) {
    if (!in_string && !in_comment) {
        switch (c) {
            case '{': brace_level++; break;
            case '}': brace_level--; break;
            case '(': paren_level++; break;
            case ')': paren_level--; break;
            case '[': bracket_level++; break;
            case ']': bracket_level--; break;
        }
    }
}

bool CHTLUnifiedScanner::isAtTopLevel() const {
    return brace_level == 0 && paren_level == 0 && bracket_level == 0;
}

void CHTLUnifiedScanner::skipWhitespace() {
    while (!isAtEnd() && Utils::isWhitespace(peek())) {
        advance();
    }
}

void CHTLUnifiedScanner::skipComment() {
    char current = peek();
    
    if (current == '/' && peek(1) == '/') {
        // 单行注释
        advance(); // '/'
        advance(); // '/'
        while (!isAtEnd() && peek() != '\n') {
            advance();
        }
    } else if (current == '/' && peek(1) == '*') {
        // 多行注释
        advance(); // '/'
        advance(); // '*'
        while (!isAtEnd()) {
            if (peek() == '*' && peek(1) == '/') {
                advance(); // '*'
                advance(); // '/'
                break;
            }
            advance();
        }
    } else if (current == '-' && peek(1) == '-') {
        // 生成器注释
        advance(); // '-'
        advance(); // '-'
        while (!isAtEnd() && peek() != '\n') {
            advance();
        }
    }
}

TokenPtr CHTLUnifiedScanner::scanString() {
    size_t start_pos = current_position;
    size_t start_line = current_line;
    size_t start_col = current_column;
    
    char quote = advance(); // 跳过开始引号
    std::string value;
    
    while (!isAtEnd() && peek() != quote) {
        char c = advance();
        if (c == '\\' && !isAtEnd()) {
            // 处理转义字符
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
            value += c;
        }
    }
    
    if (!isAtEnd()) {
        advance(); // 跳过结束引号
    }
    
    return std::make_shared<Token>(TokenType::STRING_LITERAL, value, start_line, start_col, start_pos);
}

TokenPtr CHTLUnifiedScanner::scanNumber() {
    size_t start_pos = current_position;
    size_t start_line = current_line;
    size_t start_col = current_column;
    
    std::string value;
    
    while (!isAtEnd() && (Utils::isDigit(peek()) || peek() == '.')) {
        value += advance();
    }
    
    return std::make_shared<Token>(TokenType::NUMBER_LITERAL, value, start_line, start_col, start_pos);
}

TokenPtr CHTLUnifiedScanner::scanIdentifier() {
    size_t start_pos = current_position;
    size_t start_line = current_line;
    size_t start_col = current_column;
    
    std::string value;
    
    while (!isAtEnd() && Utils::isValidIdentifierChar(peek())) {
        value += advance();
    }
    
    // 检查是否为关键字
    TokenType type = getKeywordType(value);
    if (type == TokenType::IDENTIFIER) {
        // 检查是否为类型标识符
        if (value == "@Style") type = TokenType::AT_STYLE;
        else if (value == "@Element") type = TokenType::AT_ELEMENT;
        else if (value == "@Var") type = TokenType::AT_VAR;
        else if (value == "@Html") type = TokenType::AT_HTML;
        else if (value == "@JavaScript") type = TokenType::AT_JAVASCRIPT;
        else if (value == "@Chtl") type = TokenType::AT_CHTL;
        else if (value == "@CJmod") type = TokenType::AT_CJMOD;
    }
    
    return std::make_shared<Token>(type, value, start_line, start_col, start_pos);
}

TokenPtr CHTLUnifiedScanner::scanOperator() {
    size_t start_pos = current_position;
    size_t start_line = current_line;
    size_t start_col = current_column;
    
    char c = peek();
    TokenType type = TokenType::INVALID;
    std::string value;
    
    switch (c) {
        case ':': type = TokenType::COLON; value = ":"; advance(); break;
        case ';': type = TokenType::SEMICOLON; value = ";"; advance(); break;
        case '=': type = TokenType::EQUALS; value = "="; advance(); break;
        case ',': type = TokenType::COMMA; value = ","; advance(); break;
        case '.': type = TokenType::DOT; value = "."; advance(); break;
        case '#': type = TokenType::HASH; value = "#"; advance(); break;
        case '&': type = TokenType::AMPERSAND; value = "&"; advance(); break;
        case '{': type = TokenType::LEFT_BRACE; value = "{"; advance(); break;
        case '}': type = TokenType::RIGHT_BRACE; value = "}"; advance(); break;
        case '(': type = TokenType::LEFT_PAREN; value = "("; advance(); break;
        case ')': type = TokenType::RIGHT_PAREN; value = ")"; advance(); break;
        case '[': type = TokenType::LEFT_BRACKET; value = "["; advance(); break;
        case ']': type = TokenType::RIGHT_BRACKET; value = "]"; advance(); break;
        case '-':
            if (peek(1) == '>') {
                type = TokenType::ARROW;
                value = "->";
                advance(); // '-'
                advance(); // '>'
            }
            break;
    }
    
    return std::make_shared<Token>(type, value, start_line, start_col, start_pos);
}

TokenPtr CHTLUnifiedScanner::scanUnquotedLiteral() {
    size_t start_pos = current_position;
    size_t start_line = current_line;
    size_t start_col = current_column;
    
    std::string value;
    
    // 扫描无引号字面量，直到遇到分隔符
    while (!isAtEnd()) {
        char c = peek();
        if (Utils::isWhitespace(c) || c == ';' || c == '}' || c == '{' || c == ',' || c == ')') {
            break;
        }
        value += advance();
    }
    
    return std::make_shared<Token>(TokenType::UNQUOTED_LITERAL, value, start_line, start_col, start_pos);
}

TokenType CHTLUnifiedScanner::getKeywordType(const std::string& identifier) const {
    // 基础关键字
    if (identifier == "text") return TokenType::KEYWORD_TEXT;
    if (identifier == "style") return TokenType::KEYWORD_STYLE;
    if (identifier == "script") return TokenType::KEYWORD_SCRIPT;
    if (identifier == "inherit") return TokenType::KEYWORD_INHERIT;
    if (identifier == "delete") return TokenType::KEYWORD_DELETE;
    if (identifier == "insert") return TokenType::KEYWORD_INSERT;
    if (identifier == "after") return TokenType::KEYWORD_AFTER;
    if (identifier == "before") return TokenType::KEYWORD_BEFORE;
    if (identifier == "replace") return TokenType::KEYWORD_REPLACE;
    if (identifier == "from") return TokenType::KEYWORD_FROM;
    if (identifier == "as") return TokenType::KEYWORD_AS;
    if (identifier == "except") return TokenType::KEYWORD_EXCEPT;
    if (identifier == "vir") return TokenType::KEYWORD_VIR;
    
    // 复合关键字
    if (identifier == "at") {
        // 需要检查下一个token是否为top或bottom
        return TokenType::IDENTIFIER; // 暂时返回标识符，后续在语法分析阶段处理
    }
    
    return TokenType::IDENTIFIER;
}

bool CHTLUnifiedScanner::isKeyword(const std::string& identifier) const {
    return getKeywordType(identifier) != TokenType::IDENTIFIER;
}

std::vector<FragmentPtr> CHTLUnifiedScanner::scanAndSplit() {
    std::vector<FragmentPtr> fragments;
    reset();
    
    while (!isAtEnd()) {
        skipWhitespace();
        if (isAtEnd()) break;
        
        // 检查是否为注释
        if ((peek() == '/' && peek(1) == '/') || 
            (peek() == '/' && peek(1) == '*') ||
            (peek() == '-' && peek(1) == '-')) {
            skipComment();
            continue;
        }
        
        // 检查代码块类型
        if (isStyleBlock()) {
            size_t start_pos = current_position;
            size_t start_line = current_line;
            std::string content = extractStyleContent();
            fragments.push_back(std::make_shared<CodeFragment>(
                FragmentType::CSS, content, start_line, current_line, 0, 0));
        } else if (isScriptBlock()) {
            size_t start_pos = current_position;
            size_t start_line = current_line;
            std::string content = extractScriptContent();
            
            // 判断是CHTL JS还是纯JS
            FragmentType type = content.find("{{") != std::string::npos || 
                               content.find("->") != std::string::npos ||
                               content.find("vir ") != std::string::npos ?
                               FragmentType::CHTL_JS : FragmentType::JAVASCRIPT;
            
            fragments.push_back(std::make_shared<CodeFragment>(
                type, content, start_line, current_line, 0, 0));
        } else if (isOriginBlock()) {
            size_t start_pos = current_position;
            size_t start_line = current_line;
            std::string content = extractOriginContent();
            
            // 根据Origin类型决定片段类型
            FragmentType type = FragmentType::CHTL; // 默认
            // 这里需要解析[Origin] @Type来确定具体类型
            
            fragments.push_back(std::make_shared<CodeFragment>(
                type, content, start_line, current_line, 0, 0));
        } else {
            // 默认为CHTL片段
            size_t start_pos = current_position;
            size_t start_line = current_line;
            std::string content = extractBlockContent();
            fragments.push_back(std::make_shared<CodeFragment>(
                FragmentType::CHTL, content, start_line, current_line, 0, 0));
        }
    }
    
    return fragments;
}

bool CHTLUnifiedScanner::isStyleBlock() const {
    // 检查是否为style块
    size_t saved_pos = current_position;
    size_t saved_line = current_line;
    size_t saved_col = current_column;
    
    // 创建临时扫描器状态
    CHTLUnifiedScanner temp = *this;
    temp.skipWhitespace();
    
    // 检查是否为style关键字
    std::string identifier;
    while (!temp.isAtEnd() && Utils::isValidIdentifierChar(temp.peek())) {
        identifier += temp.advance();
    }
    
    return identifier == "style";
}

bool CHTLUnifiedScanner::isScriptBlock() const {
    // 检查是否为script块
    size_t saved_pos = current_position;
    size_t saved_line = current_line;
    size_t saved_col = current_column;
    
    CHTLUnifiedScanner temp = *this;
    temp.skipWhitespace();
    
    std::string identifier;
    while (!temp.isAtEnd() && Utils::isValidIdentifierChar(temp.peek())) {
        identifier += temp.advance();
    }
    
    return identifier == "script";
}

bool CHTLUnifiedScanner::isOriginBlock() const {
    // 检查是否为[Origin]块
    size_t saved_pos = current_position;
    CHTLUnifiedScanner temp = *this;
    temp.skipWhitespace();
    
    return temp.peek() == '[' && 
           temp.source_code.substr(temp.current_position, 8) == "[Origin]";
}

bool CHTLUnifiedScanner::isTemplateBlock() const {
    size_t saved_pos = current_position;
    CHTLUnifiedScanner temp = *this;
    temp.skipWhitespace();
    
    return temp.peek() == '[' && 
           temp.source_code.substr(temp.current_position, 10) == "[Template]";
}

bool CHTLUnifiedScanner::isCustomBlock() const {
    size_t saved_pos = current_position;
    CHTLUnifiedScanner temp = *this;
    temp.skipWhitespace();
    
    return temp.peek() == '[' && 
           temp.source_code.substr(temp.current_position, 8) == "[Custom]";
}

std::string CHTLUnifiedScanner::extractStyleContent() {
    std::string content;
    int brace_count = 0;
    bool found_opening_brace = false;
    
    // 跳过style关键字
    while (!isAtEnd() && Utils::isValidIdentifierChar(peek())) {
        advance();
    }
    
    skipWhitespace();
    
    // 寻找开始大括号
    while (!isAtEnd() && peek() != '{') {
        advance();
    }
    
    if (!isAtEnd() && peek() == '{') {
        advance(); // 跳过 '{'
        brace_count = 1;
        found_opening_brace = true;
    }
    
    // 提取内容直到匹配的结束大括号
    while (!isAtEnd() && brace_count > 0) {
        char c = peek();
        if (c == '{') {
            brace_count++;
        } else if (c == '}') {
            brace_count--;
        }
        
        if (brace_count > 0) {
            content += advance();
        } else {
            advance(); // 跳过最后的 '}'
        }
    }
    
    return content;
}

std::string CHTLUnifiedScanner::extractScriptContent() {
    std::string content;
    int brace_count = 0;
    
    // 跳过script关键字
    while (!isAtEnd() && Utils::isValidIdentifierChar(peek())) {
        advance();
    }
    
    skipWhitespace();
    
    // 寻找开始大括号
    while (!isAtEnd() && peek() != '{') {
        advance();
    }
    
    if (!isAtEnd() && peek() == '{') {
        advance(); // 跳过 '{'
        brace_count = 1;
    }
    
    // 提取内容直到匹配的结束大括号
    while (!isAtEnd() && brace_count > 0) {
        char c = peek();
        if (c == '{') {
            brace_count++;
        } else if (c == '}') {
            brace_count--;
        }
        
        if (brace_count > 0) {
            content += advance();
        } else {
            advance(); // 跳过最后的 '}'
        }
    }
    
    return content;
}

std::string CHTLUnifiedScanner::extractOriginContent() {
    std::string content;
    
    // 跳过[Origin]标识符
    while (!isAtEnd() && peek() != '{') {
        advance();
    }
    
    if (!isAtEnd() && peek() == '{') {
        advance(); // 跳过 '{'
        int brace_count = 1;
        
        while (!isAtEnd() && brace_count > 0) {
            char c = peek();
            if (c == '{') {
                brace_count++;
            } else if (c == '}') {
                brace_count--;
            }
            
            if (brace_count > 0) {
                content += advance();
            } else {
                advance(); // 跳过最后的 '}'
            }
        }
    }
    
    return content;
}

std::string CHTLUnifiedScanner::extractBlockContent() {
    std::string content;
    size_t start_pos = current_position;
    
    // 提取直到下一个顶级块或文件结束
    while (!isAtEnd()) {
        // 不要跳过空白字符，保持原始格式
        if (isAtEnd()) break;
        
        // 检查是否到达下一个顶级块
        if (isAtTopLevel() && (isStyleBlock() || isScriptBlock() || 
                              isOriginBlock() || isTemplateBlock() || isCustomBlock())) {
            break;
        }
        
        content += advance();
    }
    
    return content;
}

std::vector<TokenPtr> CHTLUnifiedScanner::tokenize() {
    std::vector<TokenPtr> tokens;
    reset();
    
    while (!isAtEnd()) {
        TokenPtr token = nextToken();
        if (token && token->type != TokenType::WHITESPACE && 
            token->type != TokenType::COMMENT_LINE && 
            token->type != TokenType::COMMENT_BLOCK) {
            tokens.push_back(token);
        }
    }
    
    // 添加EOF token
    tokens.push_back(std::make_shared<Token>(TokenType::EOF_TOKEN, "", 
                                           current_line, current_column, current_position));
    
    return tokens;
}

TokenPtr CHTLUnifiedScanner::nextToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return std::make_shared<Token>(TokenType::EOF_TOKEN, "", 
                                     current_line, current_column, current_position);
    }
    
    char c = peek();
    
    // 处理注释
    if (c == '/' && peek(1) == '/') {
        size_t start_pos = current_position;
        size_t start_line = current_line;
        size_t start_col = current_column;
        skipComment();
        return std::make_shared<Token>(TokenType::COMMENT_LINE, "", start_line, start_col, start_pos);
    }
    
    if (c == '/' && peek(1) == '*') {
        size_t start_pos = current_position;
        size_t start_line = current_line;
        size_t start_col = current_column;
        skipComment();
        return std::make_shared<Token>(TokenType::COMMENT_BLOCK, "", start_line, start_col, start_pos);
    }
    
    if (c == '-' && peek(1) == '-') {
        size_t start_pos = current_position;
        size_t start_line = current_line;
        size_t start_col = current_column;
        skipComment();
        return std::make_shared<Token>(TokenType::COMMENT_GENERATOR, "", start_line, start_col, start_pos);
    }
    
    // 处理字符串
    if (c == '"' || c == '\'') {
        return scanString();
    }
    
    // 处理数字
    if (Utils::isDigit(c)) {
        return scanNumber();
    }
    
    // 处理标识符
    if (Utils::isValidIdentifierStart(c) || c == '@') {
        return scanIdentifier();
    }
    
    // 处理块标识符
    if (c == '[') {
        size_t start_pos = current_position;
        size_t start_line = current_line;
        size_t start_col = current_column;
        
        std::string block_name;
        advance(); // '['
        
        while (!isAtEnd() && peek() != ']') {
            block_name += advance();
        }
        
        if (!isAtEnd()) {
            advance(); // ']'
        }
        
        TokenType type = TokenType::INVALID;
        if (block_name == "Template") type = TokenType::TEMPLATE;
        else if (block_name == "Custom") type = TokenType::CUSTOM;
        else if (block_name == "Origin") type = TokenType::ORIGIN;
        else if (block_name == "Import") type = TokenType::IMPORT;
        else if (block_name == "Namespace") type = TokenType::NAMESPACE;
        else if (block_name == "Configuration") type = TokenType::CONFIGURATION;
        else if (block_name == "Info") type = TokenType::INFO;
        else if (block_name == "Export") type = TokenType::EXPORT;
        
        return std::make_shared<Token>(type, "[" + block_name + "]", start_line, start_col, start_pos);
    }
    
    // 处理操作符
    return scanOperator();
}

bool CHTLUnifiedScanner::isAtEnd() const {
    return current_position >= source_code.length();
}

void CHTLUnifiedScanner::reset() {
    current_position = 0;
    current_line = 1;
    current_column = 1;
    in_string = false;
    in_comment = false;
    string_delimiter = '\0';
    brace_level = 0;
    paren_level = 0;
    bracket_level = 0;
}

std::string CHTLUnifiedScanner::formatError(const std::string& message) const {
    return Utils::formatError(message, current_line, current_column);
}

void CHTLUnifiedScanner::dumpTokens(const std::vector<TokenPtr>& tokens) const {
    std::cout << "=== Token Dump ===" << std::endl;
    for (const auto& token : tokens) {
        std::cout << token->getTypeString() << " : \"" << token->value 
                  << "\" [" << token->line << ":" << token->column << "]" << std::endl;
    }
    std::cout << "=================" << std::endl;
}

void CHTLUnifiedScanner::dumpFragments(const std::vector<FragmentPtr>& fragments) const {
    std::cout << "=== Fragment Dump ===" << std::endl;
    for (size_t i = 0; i < fragments.size(); ++i) {
        const auto& fragment = fragments[i];
        std::string type_str;
        switch (fragment->type) {
            case FragmentType::CHTL: type_str = "CHTL"; break;
            case FragmentType::CHTL_JS: type_str = "CHTL_JS"; break;
            case FragmentType::CSS: type_str = "CSS"; break;
            case FragmentType::JAVASCRIPT: type_str = "JAVASCRIPT"; break;
        }
        
        std::cout << "Fragment " << i << " [" << type_str << "] "
                  << "Lines " << fragment->start_line << "-" << fragment->end_line << ":" << std::endl;
        std::cout << fragment->content << std::endl;
        std::cout << "---" << std::endl;
    }
    std::cout << "==================" << std::endl;
}

} // namespace CHTL