#include "parsers/chtl_style_parser.hpp"
#include <cctype>
#include <sstream>
#include <algorithm>

namespace chtl {

// ===== LocalStyleScanner 实现 =====

LocalStyleScanner::LocalStyleScanner() : currentPos_(0), line_(1), column_(1) {}

LocalStyleScanner::~LocalStyleScanner() = default;

std::vector<Token> LocalStyleScanner::scanStyleContent(const std::string& content, const StyleContext& context) {
    content_ = content;
    context_ = context;
    currentPos_ = 0;
    line_ = 1;
    column_ = 1;

    std::vector<Token> tokens;

    while (!isEOF()) {
        skipWhitespace();
        if (isEOF()) break;

        char c = currentChar();
        Position pos = getCurrentPosition();

        // 选择器相关
        if (c == '.') {
            // 类选择器
            advance();
            std::string className;
            while (!isEOF() && (isAlphaNumeric(currentChar()) || currentChar() == '-' || currentChar() == '_')) {
                className += currentChar();
                advance();
            }
            tokens.emplace_back(TokenType::DOT, ".", pos);
            if (!className.empty()) {
                tokens.emplace_back(TokenType::IDENTIFIER, className, getCurrentPosition());
            }
        } else if (c == '#') {
            // ID选择器
            advance();
            std::string idName;
            while (!isEOF() && (isAlphaNumeric(currentChar()) || currentChar() == '-' || currentChar() == '_')) {
                idName += currentChar();
                advance();
            }
            tokens.emplace_back(TokenType::HASH, "#", pos);
            if (!idName.empty()) {
                tokens.emplace_back(TokenType::IDENTIFIER, idName, getCurrentPosition());
            }
        } else if (c == '&') {
            // 上下文引用
            tokens.emplace_back(TokenType::AMPERSAND, "&", pos);
            advance();
        } else if (c == ':') {
            // 冒号或伪类
            advance();
            if (!isEOF() && currentChar() == ':') {
                // 双冒号 - 伪元素
                advance();
                tokens.emplace_back(TokenType::OPERATOR, "::", pos);
            } else {
                // 单冒号 - 伪类或属性分隔符
                tokens.emplace_back(TokenType::COLON, ":", pos);
            }
        } else if (c == '{') {
            tokens.emplace_back(TokenType::LEFT_BRACE, "{", pos);
            advance();
        } else if (c == '}') {
            tokens.emplace_back(TokenType::RIGHT_BRACE, "}", pos);
            advance();
        } else if (c == ';') {
            tokens.emplace_back(TokenType::SEMICOLON, ";", pos);
            advance();
        } else if (isAlpha(c)) {
            // 标识符
            std::string identifier;
            while (!isEOF() && (isAlphaNumeric(currentChar()) || currentChar() == '-' || currentChar() == '_')) {
                identifier += currentChar();
                advance();
            }
            tokens.emplace_back(TokenType::IDENTIFIER, identifier, pos);
        } else if (isDigit(c) || c == '-') {
            // 数字或CSS值
            std::string value;
            if (c == '-') {
                value += c;
                advance();
            }
            while (!isEOF() && (isDigit(currentChar()) || currentChar() == '.' || isAlpha(currentChar()) || currentChar() == '%')) {
                value += currentChar();
                advance();
            }
            tokens.emplace_back(TokenType::NUMBER, value, pos);
        } else {
            // 其他字符
            tokens.emplace_back(TokenType::OPERATOR, std::string(1, c), pos);
            advance();
        }
    }

    return tokens;
}

char LocalStyleScanner::currentChar() const {
    if (currentPos_ >= content_.length()) return '\0';
    return content_[currentPos_];
}

char LocalStyleScanner::peekChar(size_t offset) const {
    size_t peekPos = currentPos_ + offset;
    if (peekPos >= content_.length()) return '\0';
    return content_[peekPos];
}

void LocalStyleScanner::advance() {
    if (currentPos_ < content_.length()) {
        if (content_[currentPos_] == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        currentPos_++;
    }
}

bool LocalStyleScanner::isEOF() const {
    return currentPos_ >= content_.length();
}

Position LocalStyleScanner::getCurrentPosition() const {
    return Position{static_cast<size_t>(line_), static_cast<size_t>(column_)};
}

bool LocalStyleScanner::isAlpha(char c) const {
    return std::isalpha(c) || c > 127;
}

bool LocalStyleScanner::isDigit(char c) const {
    return std::isdigit(c);
}

bool LocalStyleScanner::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool LocalStyleScanner::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

void LocalStyleScanner::skipWhitespace() {
    while (!isEOF() && isWhitespace(currentChar())) {
        advance();
    }
}

// ===== LocalStyleStateMachine 实现 =====

LocalStyleStateMachine::LocalStyleStateMachine() : currentState_(StyleParseState::INITIAL) {}

LocalStyleStateMachine::~LocalStyleStateMachine() = default;

void LocalStyleStateMachine::reset() {
    currentState_ = StyleParseState::INITIAL;
    inlineProperties_.clear();
    globalEntries_.clear();
    currentSelector_.clear();
    currentProperty_.clear();
    currentValue_.clear();
}

bool LocalStyleStateMachine::processToken(const Token& token, StyleContext& context) {
    currentPosition_ = token.position;

    switch (currentState_) {
        case StyleParseState::INITIAL:
            handleInitialState(token, context);
            break;
        case StyleParseState::READING_SELECTOR:
            handleReadingSelectorState(token, context);
            break;
        case StyleParseState::READING_PROPERTIES:
            handleReadingPropertiesState(token, context);
            break;
        case StyleParseState::READING_VALUE:
            handleReadingValueState(token, context);
            break;
        case StyleParseState::EXPECTING_SEMICOLON:
            handleExpectingSemicolonState(token, context);
            break;
        case StyleParseState::NESTED_BLOCK:
            handleNestedBlockState(token, context);
            break;
    }

    return true;
}

StyleParseState LocalStyleStateMachine::getCurrentState() const {
    return currentState_;
}

const std::vector<CSSProperty>& LocalStyleStateMachine::getInlineProperties() const {
    return inlineProperties_;
}

const std::vector<GlobalStyleEntry>& LocalStyleStateMachine::getGlobalEntries() const {
    return globalEntries_;
}

void LocalStyleStateMachine::handleInitialState(const Token& token, StyleContext& context) {
    if (token.type == TokenType::DOT || token.type == TokenType::HASH || token.type == TokenType::AMPERSAND) {
        // 选择器开始
        currentSelector_ = token.value;
        currentState_ = StyleParseState::READING_SELECTOR;
    } else if (token.type == TokenType::IDENTIFIER) {
        // 可能是CSS属性名
        currentProperty_ = token.value;
        currentState_ = StyleParseState::READING_PROPERTIES;
    } else if (token.type == TokenType::WHITESPACE || token.type == TokenType::NEWLINE) {
        // 跳过空白
    }
}

void LocalStyleStateMachine::handleReadingSelectorState(const Token& token, StyleContext& context) {
    if (token.type == TokenType::IDENTIFIER) {
        // 选择器名称
        currentSelector_ += token.value;
        
        // 处理自动生成类名/ID
        if (currentSelector_.front() == '.') {
            std::string className = currentSelector_.substr(1);
            generateAutoClass(className, context);
        } else if (currentSelector_.front() == '#') {
            std::string idName = currentSelector_.substr(1);
            generateAutoId(idName, context);
        }
    } else if (token.type == TokenType::COLON) {
        // 伪类
        currentSelector_ += ":";
    } else if (token.type == TokenType::OPERATOR && token.value == "::") {
        // 伪元素
        currentSelector_ += "::";
    } else if (token.type == TokenType::LEFT_BRACE) {
        // 进入嵌套块
        currentState_ = StyleParseState::NESTED_BLOCK;
    } else if (token.type == TokenType::WHITESPACE || token.type == TokenType::NEWLINE) {
        // 跳过空白
    }
}

void LocalStyleStateMachine::handleReadingPropertiesState(const Token& token, StyleContext& context) {
    if (token.type == TokenType::COLON) {
        // 属性名结束，开始读取值
        currentState_ = StyleParseState::READING_VALUE;
    } else if (token.type == TokenType::WHITESPACE || token.type == TokenType::NEWLINE) {
        // 跳过空白
    }
}

void LocalStyleStateMachine::handleReadingValueState(const Token& token, StyleContext& context) {
    if (token.type == TokenType::SEMICOLON) {
        // 属性结束
        addInlineProperty(currentProperty_, currentValue_, currentPosition_);
        currentProperty_.clear();
        currentValue_.clear();
        currentState_ = StyleParseState::INITIAL;
    } else if (token.type == TokenType::IDENTIFIER || token.type == TokenType::NUMBER || token.type == TokenType::OPERATOR) {
        // 值内容
        if (!currentValue_.empty()) currentValue_ += " ";
        currentValue_ += token.value;
    } else if (token.type == TokenType::WHITESPACE || token.type == TokenType::NEWLINE) {
        // 跳过空白
    }
}

void LocalStyleStateMachine::handleExpectingSemicolonState(const Token& token, StyleContext& context) {
    if (token.type == TokenType::SEMICOLON) {
        currentState_ = StyleParseState::INITIAL;
    } else if (token.type == TokenType::WHITESPACE || token.type == TokenType::NEWLINE) {
        // 跳过空白
    }
}

void LocalStyleStateMachine::handleNestedBlockState(const Token& token, StyleContext& context) {
    static std::vector<CSSProperty> nestedProperties;
    static bool readingValue = false;
    
    if (token.type == TokenType::RIGHT_BRACE) {
        // 块结束
        std::string resolvedSelector = resolveContextReference(currentSelector_, context);
        addGlobalEntry(resolvedSelector, nestedProperties, currentPosition_);
        nestedProperties.clear();
        currentSelector_.clear();
        readingValue = false;
        currentState_ = StyleParseState::INITIAL;
    } else if (token.type == TokenType::IDENTIFIER && !readingValue) {
        // CSS属性名
        currentProperty_ = token.value;
    } else if (token.type == TokenType::COLON) {
        // 开始读取值
        currentValue_.clear();
        readingValue = true;
    } else if (token.type == TokenType::SEMICOLON) {
        // 属性结束
        if (!currentProperty_.empty()) {
            nestedProperties.emplace_back(currentProperty_, currentValue_, currentPosition_);
        }
        currentProperty_.clear();
        currentValue_.clear();
        readingValue = false;
    } else if (readingValue && (token.type == TokenType::IDENTIFIER || token.type == TokenType::NUMBER || token.type == TokenType::OPERATOR)) {
        // 值内容
        if (!currentValue_.empty()) currentValue_ += " ";
        currentValue_ += token.value;
    }
}

void LocalStyleStateMachine::addInlineProperty(const std::string& name, const std::string& value, const Position& pos) {
    inlineProperties_.emplace_back(name, value, pos);
}

void LocalStyleStateMachine::addGlobalEntry(const std::string& selector, const std::vector<CSSProperty>& properties, const Position& pos) {
    GlobalStyleEntry entry(selector, pos);
    entry.properties = properties;
    globalEntries_.push_back(entry);
}

std::string LocalStyleStateMachine::resolveContextReference(const std::string& selector, const StyleContext& context) {
    std::string resolved = selector;
    
    // 替换 & 为当前元素的类名或ID
    size_t pos = resolved.find('&');
    while (pos != std::string::npos) {
        std::string replacement;
        
        // 优先使用类名
        if (!context.elementClass.empty()) {
            replacement = "." + context.elementClass;
        } else if (!context.elementId.empty()) {
            replacement = "#" + context.elementId;
        } else if (!context.autoGeneratedClasses.empty()) {
            replacement = "." + *context.autoGeneratedClasses.begin();
        } else if (!context.autoGeneratedIds.empty()) {
            replacement = "#" + *context.autoGeneratedIds.begin();
        } else {
            // 如果没有类名或ID，使用元素标签
            replacement = context.elementTag;
        }
        
        resolved.replace(pos, 1, replacement);
        pos = resolved.find('&', pos + replacement.length());
    }
    
    return resolved;
}

void LocalStyleStateMachine::generateAutoClass(const std::string& className, StyleContext& context) {
    context.autoGeneratedClasses.insert(className);
}

void LocalStyleStateMachine::generateAutoId(const std::string& idName, StyleContext& context) {
    context.autoGeneratedIds.insert(idName);
}

// ===== CHTLStyleParser 实现 =====

CHTLStyleParser::CHTLStyleParser() : strictMode_(true) {}

CHTLStyleParser::~CHTLStyleParser() = default;

CHTLASTNodePtr CHTLStyleParser::parseLocalStyleBlock(const std::string& styleContent, const std::string& elementTag) {
    errors_.clear();
    warnings_.clear();
    
    // 设置上下文
    currentContext_ = StyleContext{};
    currentContext_.elementTag = elementTag;
    
    // 扫描样式内容
    auto tokens = scanner_.scanStyleContent(styleContent, currentContext_);
    
    // 重置状态机
    stateMachine_.reset();
    
    // 解析样式块
    parseStyleBlock(tokens);
    
    // 创建样式块节点
    auto styleBlock = std::make_shared<StyleBlockNode>(Position{});
    
    // 处理内联样式
    for (const auto& property : stateMachine_.getInlineProperties()) {
        processInlineStyle(property, styleBlock);
    }
    
    // 处理全局样式
    for (const auto& entry : stateMachine_.getGlobalEntries()) {
        processGlobalStyle(entry);
    }
    
    return styleBlock;
}

void CHTLStyleParser::setStrictMode(bool strict) {
    strictMode_ = strict;
}

const std::vector<ParseError>& CHTLStyleParser::getErrors() const {
    return errors_;
}

const std::vector<ParseWarning>& CHTLStyleParser::getWarnings() const {
    return warnings_;
}

const std::vector<GlobalStyleEntry>& CHTLStyleParser::getGlobalStyleEntries() const {
    return globalStyleEntries_;
}

void CHTLStyleParser::clearGlobalStyleEntries() {
    globalStyleEntries_.clear();
}

void CHTLStyleParser::parseStyleBlock(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        if (token.type != TokenType::WHITESPACE && token.type != TokenType::NEWLINE) {
            stateMachine_.processToken(token, currentContext_);
        }
    }
}

void CHTLStyleParser::processInlineStyle(const CSSProperty& property, CHTLASTNodePtr styleBlock) {
    if (strictMode_ && !isValidCSSProperty(property.name)) {
        reportWarning("未知的CSS属性: " + property.name, property.position);
    }
    
    // 创建内联样式节点
    auto inlineStyle = std::make_shared<InlineStyleNode>(property.name, property.value, property.position);
    styleBlock->addChild(inlineStyle);
}

void CHTLStyleParser::processGlobalStyle(const GlobalStyleEntry& entry) {
    // 添加到全局样式条目
    globalStyleEntries_.push_back(entry);
    
    if (strictMode_) {
        // 验证选择器
        if (entry.selector.empty()) {
            reportWarning("空的CSS选择器", entry.position);
        }
        
        // 验证属性
        for (const auto& property : entry.properties) {
            if (!isValidCSSProperty(property.name)) {
                reportWarning("未知的CSS属性: " + property.name, property.position);
            }
        }
    }
}

void CHTLStyleParser::reportError(const std::string& message, const Position& position) {
    errors_.emplace_back(message, position);
}

void CHTLStyleParser::reportWarning(const std::string& message, const Position& position) {
    warnings_.emplace_back(message, position);
}

std::string CHTLStyleParser::generateUniqueClassName() {
    static int counter = 0;
    return "chtl-auto-class-" + std::to_string(++counter);
}

std::string CHTLStyleParser::generateUniqueIdName() {
    static int counter = 0;
    return "chtl-auto-id-" + std::to_string(++counter);
}

bool CHTLStyleParser::isValidCSSProperty(const std::string& property) const {
    // 常见CSS属性验证 (简化版)
    static const std::unordered_set<std::string> validProperties = {
        "width", "height", "margin", "padding", "border", "background", "color", "font",
        "font-size", "font-family", "font-weight", "line-height", "text-align", "display",
        "position", "top", "right", "bottom", "left", "z-index", "opacity", "visibility",
        "overflow", "float", "clear", "content", "cursor", "outline", "box-sizing",
        "border-radius", "box-shadow", "text-shadow", "transform", "transition", "animation"
    };
    
    return validProperties.find(property) != validProperties.end() ||
           property.find("-webkit-") == 0 || property.find("-moz-") == 0 ||
           property.find("-ms-") == 0 || property.find("-o-") == 0;
}

bool CHTLStyleParser::isValidCSSValue(const std::string& value) const {
    // 简单的CSS值验证
    return !value.empty();
}

} // namespace chtl