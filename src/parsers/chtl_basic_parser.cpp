#include "parsers/chtl_basic_parser.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>

namespace chtl {

CHTLBasicParser::CHTLBasicParser() 
    : currentTokenIndex_(0), strictMode_(true) {
    initializeHtmlElements();
}

CHTLBasicParser::~CHTLBasicParser() = default;

ParseResult CHTLBasicParser::parse(const std::string& source, const std::string& filename) {
    currentFile_ = filename;
    errors_.clear();
    warnings_.clear();
    currentTokenIndex_ = 0;

    // 词法分析
    auto lexResult = lexer_.tokenize(source, filename);
    if (!lexResult.success) {
        ParseResult result;
        result.success = false;
        result.errors = lexResult.errors;
        return result;
    }

    tokens_ = lexResult.tokens;

    // 语法分析
    auto rootNode = parseProgram();

    // 构建结果
    ParseResult result;
    result.success = errors_.empty();
    result.rootNode = rootNode;
    result.errors.insert(result.errors.end(), errors_.begin(), errors_.end());
    result.warnings.insert(result.warnings.end(), warnings_.begin(), warnings_.end());

    return result;
}

void CHTLBasicParser::setStrictMode(bool strict) {
    strictMode_ = strict;
}

const std::vector<ParseError>& CHTLBasicParser::getErrors() const {
    return errors_;
}

const std::vector<ParseWarning>& CHTLBasicParser::getWarnings() const {
    return warnings_;
}

// ===== 核心解析方法 =====

CHTLASTNodePtr CHTLBasicParser::parseProgram() {
    auto program = std::make_shared<CHTLASTNode>(CHTLASTNodeType::PROGRAM, getCurrentPosition());
    
    auto statements = parseTopLevelStatements();
    for (auto& stmt : statements) {
        if (stmt) {
            program->addChild(stmt);
        }
    }

    return program;
}

std::vector<CHTLASTNodePtr> CHTLBasicParser::parseTopLevelStatements() {
    std::vector<CHTLASTNodePtr> statements;

    while (!isEOF()) {
        // 只跳过空白，保留注释作为语句
        while (!isEOF()) {
            const auto& token = getCurrentToken();
            if (token.type == TokenType::WHITESPACE || token.type == TokenType::NEWLINE) {
                getNextToken();
            } else {
                break;
            }
        }
        
        if (isEOF()) break;

        auto stmt = parseStatement();
        if (stmt) {
            statements.push_back(stmt);
        } else {
            // 错误恢复
            synchronize();
        }
    }

    return statements;
}

CHTLASTNodePtr CHTLBasicParser::parseStatement() {
    // 只跳过空白，保留注释
    while (!isEOF()) {
        const auto& token = getCurrentToken();
        if (token.type == TokenType::WHITESPACE || token.type == TokenType::NEWLINE) {
            getNextToken();
        } else {
            break;
        }
    }
    
    if (isEOF()) return nullptr;

    const auto& token = getCurrentToken();

    // 注释
    if (token.type == TokenType::COMMENT_LINE || 
        token.type == TokenType::COMMENT_MULTILINE ||
        (token.type == TokenType::OPERATOR && token.value == "--")) {
        return parseComment();
    }

    // 文本节点
    if ((token.type == TokenType::IDENTIFIER || token.type == TokenType::KEYWORD) && token.value == "text") {
        return parseTextNode();
    }

    // 属性 (在元素内部) - 优先于元素节点
    if ((token.type == TokenType::IDENTIFIER && peekToken().type == TokenType::COLON) || 
        (token.type == TokenType::IDENTIFIER && peekToken().type == TokenType::ASSIGNMENT)) {
        return parseAttribute();
    }

    // 元素节点 (HTML标签) - 包括未知标签
    if (token.type == TokenType::IDENTIFIER) {
        return parseElementNode();
    }

    // 未知语句
    reportError("未识别的语句: " + token.value, token.position);
    getNextToken(); // 跳过当前token
    return nullptr;
}

// ===== 注释解析 =====

CHTLASTNodePtr CHTLBasicParser::parseComment() {
    const auto& token = getCurrentToken();

    if (token.type == TokenType::COMMENT_LINE) {
        return parseLineComment();
    } else if (token.type == TokenType::COMMENT_MULTILINE) {
        return parseMultilineComment();
    } else if (token.type == TokenType::OPERATOR && token.value == "--") {
        return parseGeneratorComment();
    }

    reportError("未知的注释类型", token.position);
    return nullptr;
}

CHTLASTNodePtr CHTLBasicParser::parseLineComment() {
    const auto& token = getCurrentToken();
    
    auto comment = std::make_shared<CommentNode>(
        token.value, 
        CHTLASTNodeType::COMMENT_LINE, 
        token.position
    );
    
    getNextToken();
    return comment;
}

CHTLASTNodePtr CHTLBasicParser::parseMultilineComment() {
    const auto& token = getCurrentToken();
    
    auto comment = std::make_shared<CommentNode>(
        token.value, 
        CHTLASTNodeType::COMMENT_MULTILINE, 
        token.position
    );
    
    getNextToken();
    return comment;
}

CHTLASTNodePtr CHTLBasicParser::parseGeneratorComment() {
    Position startPos = getCurrentPosition();
    
    if (!expect("--")) {
        return nullptr;
    }

    // 读取注释内容直到行尾
    std::string content;
    while (!isEOF() && getCurrentToken().type != TokenType::NEWLINE) {
        content += getCurrentToken().value;
        getNextToken();
    }

    auto comment = std::make_shared<CommentNode>(
        content, 
        CHTLASTNodeType::COMMENT_GENERATOR, 
        startPos
    );

    return comment;
}

// ===== 文本节点解析 =====

CHTLASTNodePtr CHTLBasicParser::parseTextNode() {
    Position startPos = getCurrentPosition();

    if (!expect("text")) {
        return nullptr;
    }

    skipWhitespaceAndComments();

    if (!expect(TokenType::LEFT_BRACE)) {
        reportError("期望 '{' 在 text 关键字后", getCurrentPosition());
        return nullptr;
    }

    auto contentPair = parseTextContent();
    std::string content = contentPair.first;
    LiteralType literalType = contentPair.second;

    if (!expect(TokenType::RIGHT_BRACE)) {
        reportError("期望 '}' 在文本内容后", getCurrentPosition());
        return nullptr;
    }

    return std::make_shared<TextNode>(content, literalType, startPos);
}

std::pair<std::string, LiteralType> CHTLBasicParser::parseTextContent() {
    skipWhitespaceAndComments();

    if (isEOF()) {
        reportError("期望文本内容", getCurrentPosition());
        return {"", LiteralType::UNQUOTED};
    }

    const auto& token = getCurrentToken();

    if (token.type == TokenType::STRING_LITERAL) {
        if (token.value.front() == '"' && token.value.back() == '"') {
            getNextToken();
            return {token.value.substr(1, token.value.length() - 2), LiteralType::DOUBLE_QUOTED};
        } else if (token.value.front() == '\'' && token.value.back() == '\'') {
            getNextToken();
            return {token.value.substr(1, token.value.length() - 2), LiteralType::SINGLE_QUOTED};
        }
    }

    // 无修饰字面量 - 读取直到遇到 }
    std::string content;
    while (!isEOF() && getCurrentToken().type != TokenType::RIGHT_BRACE) {
        if (getCurrentToken().type == TokenType::WHITESPACE || 
            getCurrentToken().type == TokenType::NEWLINE) {
            if (!content.empty()) content += " ";
        } else {
            content += getCurrentToken().value;
        }
        getNextToken();
    }

    return {content, LiteralType::UNQUOTED};
}

// ===== 字面量解析 =====

CHTLASTNodePtr CHTLBasicParser::parseLiteral() {
    const auto& token = getCurrentToken();
    Position pos = token.position;

    if (token.type == TokenType::STRING_LITERAL) {
        if (token.value.front() == '"') {
            std::string content = parseDoubleQuotedString();
            return std::make_shared<StringLiteralNode>(content, LiteralType::DOUBLE_QUOTED, pos);
        } else if (token.value.front() == '\'') {
            std::string content = parseSingleQuotedString();
            return std::make_shared<StringLiteralNode>(content, LiteralType::SINGLE_QUOTED, pos);
        }
    }

    // 无修饰字面量
    std::string content = parseUnquotedLiteral();
    return std::make_shared<StringLiteralNode>(content, LiteralType::UNQUOTED, pos);
}

std::string CHTLBasicParser::parseDoubleQuotedString() {
    const auto& token = getCurrentToken();
    if (token.type != TokenType::STRING_LITERAL || token.value.front() != '"') {
        reportError("期望双引号字符串", token.position);
        return "";
    }

    getNextToken();
    return token.value.substr(1, token.value.length() - 2);
}

std::string CHTLBasicParser::parseSingleQuotedString() {
    const auto& token = getCurrentToken();
    if (token.type != TokenType::STRING_LITERAL || token.value.front() != '\'') {
        reportError("期望单引号字符串", token.position);
        return "";
    }

    getNextToken();
    return token.value.substr(1, token.value.length() - 2);
}

std::string CHTLBasicParser::parseUnquotedLiteral() {
    const auto& token = getCurrentToken();
    if (token.type != TokenType::IDENTIFIER && token.type != TokenType::NUMBER) {
        reportError("期望无修饰字面量", token.position);
        return "";
    }

    getNextToken();
    return token.value;
}

// ===== 元素节点解析 =====

CHTLASTNodePtr CHTLBasicParser::parseElementNode() {
    Position startPos = getCurrentPosition();
    const auto& token = getCurrentToken();

    if (token.type != TokenType::IDENTIFIER) {
        reportError("期望元素标签名", token.position);
        return nullptr;
    }

    std::string tagName = token.value;
    
    if (strictMode_ && !isValidHtmlElement(tagName)) {
        reportWarning("未知的HTML元素: " + tagName, token.position);
    }

    getNextToken();

    if (!expect(TokenType::LEFT_BRACE)) {
        reportError("期望 '{' 在元素标签后", getCurrentPosition());
        return nullptr;
    }

    auto element = std::make_shared<ElementNode>(tagName, startPos);
    auto bodyNodes = parseElementBody();
    
    for (auto& node : bodyNodes) {
        if (node) {
            element->addChild(node);
        }
    }

    if (!expect(TokenType::RIGHT_BRACE)) {
        reportError("期望 '}' 在元素体后", getCurrentPosition());
        return nullptr;
    }

    return element;
}

std::vector<CHTLASTNodePtr> CHTLBasicParser::parseElementBody() {
    std::vector<CHTLASTNodePtr> nodes;

    while (!isEOF() && getCurrentToken().type != TokenType::RIGHT_BRACE) {
        skipWhitespaceAndComments();
        
        if (isEOF() || getCurrentToken().type == TokenType::RIGHT_BRACE) {
            break;
        }

        auto node = parseStatement();
        if (node) {
            nodes.push_back(node);
        }
    }

    return nodes;
}

bool CHTLBasicParser::isValidHtmlElement(const std::string& tagName) const {
    return htmlElements_.find(tagName) != htmlElements_.end();
}

// ===== 属性解析 =====

CHTLASTNodePtr CHTLBasicParser::parseAttribute() {
    Position startPos = getCurrentPosition();
    const auto& token = getCurrentToken();

    if (token.type != TokenType::IDENTIFIER) {
        reportError("期望属性名", token.position);
        return nullptr;
    }

    std::string attributeName = token.value;
    getNextToken();

    // CE对等式: ':' 和 '=' 等价
    if (!isCEEquivalent()) {
        reportError("期望 ':' 或 '=' 在属性名后", getCurrentPosition());
        return nullptr;
    }
    getNextToken();

    auto valuePair = parseAttributeValue();
    std::string value = valuePair.first;
    LiteralType literalType = valuePair.second;

    if (!expect(TokenType::SEMICOLON)) {
        reportError("期望 ';' 在属性值后", getCurrentPosition());
        return nullptr;
    }

    return std::make_shared<AttributeNode>(attributeName, value, literalType, startPos);
}

std::pair<std::string, LiteralType> CHTLBasicParser::parseAttributeValue() {
    skipWhitespaceAndComments();

    if (isEOF()) {
        reportError("期望属性值", getCurrentPosition());
        return {"", LiteralType::UNQUOTED};
    }

    const auto& token = getCurrentToken();

    if (token.type == TokenType::STRING_LITERAL) {
        if (token.value.front() == '"' && token.value.back() == '"') {
            getNextToken();
            return {token.value.substr(1, token.value.length() - 2), LiteralType::DOUBLE_QUOTED};
        } else if (token.value.front() == '\'' && token.value.back() == '\'') {
            getNextToken();
            return {token.value.substr(1, token.value.length() - 2), LiteralType::SINGLE_QUOTED};
        }
    }

    // 无修饰字面量
    if (token.type == TokenType::IDENTIFIER || token.type == TokenType::NUMBER) {
        getNextToken();
        return {token.value, LiteralType::UNQUOTED};
    }

    reportError("无效的属性值", token.position);
    return {"", LiteralType::UNQUOTED};
}

bool CHTLBasicParser::isCEEquivalent() const {
    const auto& token = getCurrentToken();
    return token.type == TokenType::COLON || token.type == TokenType::ASSIGNMENT;
}

// ===== 工具方法 =====

const Token& CHTLBasicParser::getCurrentToken() const {
    if (currentTokenIndex_ >= tokens_.size()) {
        static Token eofToken{TokenType::EOF_TOKEN, "", Position{}};
        return eofToken;
    }
    return tokens_[currentTokenIndex_];
}

const Token& CHTLBasicParser::getNextToken() {
    if (currentTokenIndex_ < tokens_.size()) {
        currentTokenIndex_++;
    }
    return getCurrentToken();
}

bool CHTLBasicParser::isCurrentToken(TokenType type) const {
    return getCurrentToken().type == type;
}

bool CHTLBasicParser::isCurrentToken(const std::string& value) const {
    return getCurrentToken().value == value;
}

bool CHTLBasicParser::consume(TokenType type) {
    if (isCurrentToken(type)) {
        getNextToken();
        return true;
    }
    return false;
}

bool CHTLBasicParser::consume(const std::string& value) {
    if (isCurrentToken(value)) {
        getNextToken();
        return true;
    }
    return false;
}

bool CHTLBasicParser::expect(TokenType type) {
    if (isCurrentToken(type)) {
        getNextToken();
        return true;
    }
    
    reportError("期望 " + std::to_string(static_cast<int>(type)), getCurrentPosition());
    return false;
}

bool CHTLBasicParser::expect(const std::string& value) {
    if (isCurrentToken(value)) {
        getNextToken();
        return true;
    }
    
    reportError("期望 '" + value + "'", getCurrentPosition());
    return false;
}

void CHTLBasicParser::skipWhitespaceAndComments() {
    while (!isEOF()) {
        const auto& token = getCurrentToken();
        if (token.type == TokenType::WHITESPACE || 
            token.type == TokenType::NEWLINE ||
            token.type == TokenType::COMMENT_LINE ||
            token.type == TokenType::COMMENT_MULTILINE) {
            getNextToken();
        } else {
            break;
        }
    }
}

const Token& CHTLBasicParser::peekToken(size_t offset) const {
    size_t peekIndex = currentTokenIndex_ + offset;
    if (peekIndex >= tokens_.size()) {
        static Token eofToken{TokenType::EOF_TOKEN, "", Position{}};
        return eofToken;
    }
    return tokens_[peekIndex];
}

bool CHTLBasicParser::isEOF() const {
    return currentTokenIndex_ >= tokens_.size() || getCurrentToken().type == TokenType::EOF_TOKEN;
}

void CHTLBasicParser::reportError(const std::string& message, const Position& position) {
    ParseError error;
    error.message = message;
    error.position = position.line > 0 ? position : getCurrentPosition();
    error.filename = currentFile_;
    errors_.push_back(error);
}

void CHTLBasicParser::reportWarning(const std::string& message, const Position& position) {
    ParseWarning warning;
    warning.message = message;
    warning.position = position.line > 0 ? position : getCurrentPosition();
    warning.filename = currentFile_;
    warnings_.push_back(warning);
}

void CHTLBasicParser::initializeHtmlElements() {
    // 严格按照HTML5标准定义的所有元素
    htmlElements_ = {
        // 根元素
        "html",
        
        // 文档元数据
        "head", "title", "base", "link", "meta", "style",
        
        // 分区根元素
        "body",
        
        // 内容分区
        "address", "article", "aside", "footer", "header", "h1", "h2", "h3", "h4", "h5", "h6",
        "main", "nav", "section",
        
        // 文本内容
        "blockquote", "dd", "div", "dl", "dt", "figcaption", "figure", "hr", "li", "ol", "p",
        "pre", "ul",
        
        // 内联文本语义
        "a", "abbr", "b", "bdi", "bdo", "br", "cite", "code", "data", "dfn", "em", "i", "kbd",
        "mark", "q", "rp", "rt", "ruby", "s", "samp", "small", "span", "strong", "sub", "sup",
        "time", "u", "var", "wbr",
        
        // 图片和多媒体
        "area", "audio", "img", "map", "track", "video",
        
        // 内嵌内容
        "embed", "iframe", "object", "param", "picture", "portal", "source",
        
        // SVG和MathML
        "svg", "math",
        
        // 脚本
        "canvas", "noscript", "script",
        
        // 编辑标识
        "del", "ins",
        
        // 表格内容
        "caption", "col", "colgroup", "table", "tbody", "td", "tfoot", "th", "thead", "tr",
        
        // 表单
        "button", "datalist", "fieldset", "form", "input", "label", "legend", "meter", "optgroup",
        "option", "output", "progress", "select", "textarea",
        
        // 交互元素
        "details", "dialog", "menu", "summary",
        
        // Web组件
        "slot", "template"
    };
}

Position CHTLBasicParser::getCurrentPosition() const {
    if (!isEOF()) {
        return getCurrentToken().position;
    }
    return Position{};
}

void CHTLBasicParser::synchronize() {
    // 简单的错误恢复 - 跳过直到下一个语句开始
    while (!isEOF()) {
        if (getCurrentToken().type == TokenType::SEMICOLON ||
            getCurrentToken().type == TokenType::RIGHT_BRACE ||
            getCurrentToken().type == TokenType::NEWLINE) {
            getNextToken();
            return;
        }
        
        const auto& nextToken = peekToken();
        if (nextToken.type == TokenType::IDENTIFIER &&
            (isValidHtmlElement(nextToken.value) || nextToken.value == "text")) {
            return;
        }
        
        getNextToken();
    }
}

} // namespace chtl