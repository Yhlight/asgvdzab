#include "parsers/chtl_origin_parser.hpp"
#include "parsers/chtl_basic_parser.hpp"
#include <regex>
#include <algorithm>
#include <sstream>

namespace chtl {

// ===== OriginRegistry 实现 =====

OriginRegistry::OriginRegistry() = default;

OriginRegistry::~OriginRegistry() = default;

bool OriginRegistry::registerOriginBlock(const OriginBlock& block) {
    if (block.mode != OriginMode::ENHANCED_DECLARE) {
        return true; // 内联嵌入不需要注册
    }
    
    std::string key = generateOriginKey(block.type, block.name);
    
    // 检查是否已存在
    if (originBlocks_.find(key) != originBlocks_.end()) {
        return false; // 原始嵌入块已存在
    }
    
    originBlocks_[key] = block;
    return true;
}

const OriginBlock* OriginRegistry::findOriginBlock(OriginType type, const std::string& name) const {
    std::string key = generateOriginKey(type, name);
    auto it = originBlocks_.find(key);
    return (it != originBlocks_.end()) ? &it->second : nullptr;
}

const std::unordered_map<std::string, OriginBlock>& OriginRegistry::getAllOriginBlocks() const {
    return originBlocks_;
}

void OriginRegistry::clear() {
    originBlocks_.clear();
}

bool OriginRegistry::validateOriginReference(const OriginReference& ref) const {
    return findOriginBlock(ref.type, ref.name) != nullptr;
}

std::string OriginRegistry::generateOriginKey(OriginType type, const std::string& name) const {
    std::string prefix;
    switch (type) {
        case OriginType::HTML_ORIGIN:
            prefix = "html:";
            break;
        case OriginType::STYLE_ORIGIN:
            prefix = "style:";
            break;
        case OriginType::JAVASCRIPT_ORIGIN:
            prefix = "js:";
            break;
    }
    return prefix + name;
}

// ===== OriginScanner 实现 =====

OriginScanner::OriginScanner() : currentPos_(0), line_(1), column_(1) {}

OriginScanner::~OriginScanner() = default;

std::string OriginScanner::scanOriginContent(const std::string& content, const OriginContext& context) {
    content_ = content;
    context_ = context;
    currentPos_ = 0;
    line_ = 1;
    column_ = 1;

    std::string result;

    switch (context.currentType) {
        case OriginType::HTML_ORIGIN:
            result = scanHtmlContent();
            break;
        case OriginType::STYLE_ORIGIN:
            result = scanCssContent();
            break;
        case OriginType::JAVASCRIPT_ORIGIN:
            result = scanJavaScriptContent();
            break;
    }

    // 保持格式化
    if (context.currentMode == OriginMode::INLINE_EMBED || 
        context.currentMode == OriginMode::ENHANCED_DECLARE) {
        result = preserveWhitespace(result);
        result = preserveLineBreaks(result);
        result = preserveIndentation(result);
    }

    return result;
}

bool OriginScanner::validateOriginContent(const std::string& content, OriginType type) {
    switch (type) {
        case OriginType::HTML_ORIGIN:
            return isValidHtmlContent(content);
        case OriginType::STYLE_ORIGIN:
            return isValidCssContent(content);
        case OriginType::JAVASCRIPT_ORIGIN:
            return isValidJavaScriptContent(content);
    }
    return false;
}

std::string OriginScanner::scanHtmlContent() {
    std::string result;
    
    // 原始HTML扫描 - 保持完整内容
    while (!isEOF()) {
        result += currentChar();
        advance();
    }
    
    return result;
}

std::string OriginScanner::scanCssContent() {
    std::string result;
    
    // 原始CSS扫描 - 保持完整内容
    while (!isEOF()) {
        result += currentChar();
        advance();
    }
    
    return result;
}

std::string OriginScanner::scanJavaScriptContent() {
    std::string result;
    
    // 原始JavaScript扫描 - 保持完整内容
    while (!isEOF()) {
        result += currentChar();
        advance();
    }
    
    return result;
}

std::string OriginScanner::preserveWhitespace(const std::string& content) {
    // 保持空白字符
    return content;
}

std::string OriginScanner::preserveLineBreaks(const std::string& content) {
    // 保持换行符
    return content;
}

std::string OriginScanner::preserveIndentation(const std::string& content) {
    // 保持缩进
    return content;
}

bool OriginScanner::isValidHtmlContent(const std::string& content) const {
    // 简单的HTML验证 - 检查基本标签平衡
    int openTags = 0;
    bool inTag = false;
    
    for (size_t i = 0; i < content.length(); ++i) {
        char c = content[i];
        if (c == '<') {
            inTag = true;
            if (i + 1 < content.length() && content[i + 1] != '/') {
                openTags++;
            } else if (i + 1 < content.length() && content[i + 1] == '/') {
                openTags--;
            }
        } else if (c == '>') {
            inTag = false;
        }
    }
    
    return openTags >= 0; // 允许未闭合的标签，这在原始嵌入中是正常的
}

bool OriginScanner::isValidCssContent(const std::string& content) const {
    // 简单的CSS验证 - 检查大括号平衡
    int braceCount = 0;
    
    for (char c : content) {
        if (c == '{') {
            braceCount++;
        } else if (c == '}') {
            braceCount--;
        }
    }
    
    return braceCount >= 0; // 允许未闭合的规则，这在原始嵌入中是正常的
}

bool OriginScanner::isValidJavaScriptContent(const std::string& content) const {
    // 简单的JavaScript验证 - 检查基本语法
    int braceCount = 0;
    int parenCount = 0;
    int bracketCount = 0;
    
    for (char c : content) {
        switch (c) {
            case '{': braceCount++; break;
            case '}': braceCount--; break;
            case '(': parenCount++; break;
            case ')': parenCount--; break;
            case '[': bracketCount++; break;
            case ']': bracketCount--; break;
        }
    }
    
    return braceCount >= 0 && parenCount >= 0 && bracketCount >= 0;
}

char OriginScanner::currentChar() const {
    if (currentPos_ >= content_.length()) return '\0';
    return content_[currentPos_];
}

char OriginScanner::peekChar(size_t offset) const {
    size_t peekPos = currentPos_ + offset;
    if (peekPos >= content_.length()) return '\0';
    return content_[peekPos];
}

void OriginScanner::advance() {
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

bool OriginScanner::isEOF() const {
    return currentPos_ >= content_.length();
}

Position OriginScanner::getCurrentPosition() const {
    return Position{static_cast<size_t>(line_), static_cast<size_t>(column_)};
}

bool OriginScanner::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

void OriginScanner::skipWhitespace() {
    while (!isEOF() && isWhitespace(currentChar())) {
        advance();
    }
}

// ===== OriginStateMachine 实现 =====

OriginStateMachine::OriginStateMachine() 
    : currentState_(OriginParseState::INITIAL)
    , currentType_(OriginType::HTML_ORIGIN)
    , currentMode_(OriginMode::INLINE_EMBED) {}

OriginStateMachine::~OriginStateMachine() = default;

void OriginStateMachine::reset() {
    currentState_ = OriginParseState::INITIAL;
    originBlocks_.clear();
    originReferences_.clear();
    currentType_ = OriginType::HTML_ORIGIN;
    currentMode_ = OriginMode::INLINE_EMBED;
    currentName_.clear();
    currentContent_.clear();
}

bool OriginStateMachine::processToken(const Token& token, OriginContext& context) {
    currentPosition_ = token.position;

    switch (currentState_) {
        case OriginParseState::INITIAL:
            handleInitialState(token, context);
            break;
        case OriginParseState::READING_ORIGIN_HEADER:
            handleReadingOriginHeaderState(token, context);
            break;
        case OriginParseState::READING_ORIGIN_TYPE:
            handleReadingOriginTypeState(token, context);
            break;
        case OriginParseState::READING_ORIGIN_NAME:
            handleReadingOriginNameState(token, context);
            break;
        case OriginParseState::READING_ORIGIN_CONTENT:
            handleReadingOriginContentState(token, context);
            break;
        case OriginParseState::READING_USAGE:
            handleReadingUsageState(token, context);
            break;
    }

    return true;
}

OriginParseState OriginStateMachine::getCurrentState() const {
    return currentState_;
}

const std::vector<OriginBlock>& OriginStateMachine::getOriginBlocks() const {
    return originBlocks_;
}

const std::vector<OriginReference>& OriginStateMachine::getOriginReferences() const {
    return originReferences_;
}

void OriginStateMachine::handleInitialState(const Token& token, OriginContext& context) {
    if (token.type == TokenType::LEFT_BRACKET && token.value == "[") {
        currentState_ = OriginParseState::READING_ORIGIN_HEADER;
    }
}

void OriginStateMachine::handleReadingOriginHeaderState(const Token& token, OriginContext& context) {
    if (token.type == TokenType::IDENTIFIER && token.value == "Origin") {
        currentState_ = OriginParseState::READING_ORIGIN_TYPE;
    }
}

void OriginStateMachine::handleReadingOriginTypeState(const Token& token, OriginContext& context) {
    if (token.type == TokenType::OPERATOR && token.value == "@") {
        // 等待类型标识符
    } else if (token.type == TokenType::IDENTIFIER) {
        currentType_ = parseOriginType(token.value);
        context.currentType = currentType_;
        currentState_ = OriginParseState::READING_ORIGIN_NAME;
    }
}

void OriginStateMachine::handleReadingOriginNameState(const Token& token, OriginContext& context) {
    if (token.type == TokenType::IDENTIFIER) {
        // 有名称，这是增强模式
        currentName_ = token.value;
        currentMode_ = OriginMode::ENHANCED_DECLARE;
        context.currentMode = currentMode_;
        context.currentName = currentName_;
        // 继续等待左大括号或分号
    } else if (token.type == TokenType::LEFT_BRACE) {
        // 左大括号，开始读取内容
        if (currentName_.empty()) {
            // 无名称，这是内联模式
            currentMode_ = OriginMode::INLINE_EMBED;
            context.currentMode = currentMode_;
        }
        currentState_ = OriginParseState::READING_ORIGIN_CONTENT;
    } else if (token.type == TokenType::SEMICOLON) {
        // 这是增强使用模式
        currentMode_ = OriginMode::ENHANCED_USAGE;
        OriginReference ref(currentType_, currentName_, currentPosition_);
        addOriginReference(ref);
        currentState_ = OriginParseState::INITIAL;
    }
}

void OriginStateMachine::handleReadingOriginContentState(const Token& token, OriginContext& context) {
    if (token.type == TokenType::RIGHT_BRACE) {
        // 内容结束
        OriginBlock block(currentType_, currentMode_, currentName_, currentPosition_);
        block.content = currentContent_;
        addOriginBlock(block);
        
        currentContent_.clear();
        currentName_.clear();
        currentState_ = OriginParseState::INITIAL;
    } else {
        // 收集内容
        currentContent_ += token.value;
    }
}

void OriginStateMachine::handleReadingUsageState(const Token& token, OriginContext& context) {
    // 处理使用模式的逻辑
    if (token.type == TokenType::SEMICOLON) {
        currentState_ = OriginParseState::INITIAL;
    }
}

void OriginStateMachine::addOriginBlock(const OriginBlock& block) {
    originBlocks_.push_back(block);
}

void OriginStateMachine::addOriginReference(const OriginReference& ref) {
    originReferences_.push_back(ref);
}

OriginType OriginStateMachine::parseOriginType(const std::string& typeStr) {
    if (typeStr == "Html") {
        return OriginType::HTML_ORIGIN;
    } else if (typeStr == "Style") {
        return OriginType::STYLE_ORIGIN;
    } else if (typeStr == "JavaScript") {
        return OriginType::JAVASCRIPT_ORIGIN;
    }
    return OriginType::HTML_ORIGIN; // 默认
}

bool OriginStateMachine::isValidOriginType(const std::string& typeStr) const {
    return typeStr == "Html" || typeStr == "Style" || typeStr == "JavaScript";
}

bool OriginStateMachine::isValidOriginName(const std::string& name) const {
    if (name.empty()) return false;
    
    // 验证名称格式：只允许字母、数字和下划线，且以字母开头
    if (!std::isalpha(name[0])) {
        return false;
    }
    
    for (char c : name) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    
    return true;
}

// ===== CHTLOriginParser 实现 =====

CHTLOriginParser::CHTLOriginParser() 
    : originRegistry_(std::make_shared<OriginRegistry>())
    , currentState_(OriginParseState::INITIAL)
    , strictMode_(true)
    , preserveFormatting_(true) {
}

CHTLOriginParser::~CHTLOriginParser() = default;

ParseResult CHTLOriginParser::parseOriginDeclaration(const std::string& source, const std::string& filename) {
    errors_.clear();
    warnings_.clear();
    
    ParseResult result;
    result.success = false;
    
    // 词法分析
    auto lexResult = lexer_.tokenize(source, filename);
    if (!lexResult.success) {
        errors_.insert(errors_.end(), lexResult.errors.begin(), lexResult.errors.end());
        result.errors = errors_;
        return result;
    }
    
    try {
        // 解析原始嵌入块
        OriginBlock originBlock = parseOriginBlock(lexResult.tokens);
        
        // 注册原始嵌入块（如果是增强模式）
        if (originBlock.mode == OriginMode::ENHANCED_DECLARE) {
            if (!originRegistry_->registerOriginBlock(originBlock)) {
                reportError("原始嵌入名称重复: " + originBlock.name, originBlock.position);
            }
        }
        
        // 创建原始嵌入节点
        auto originNode = createOriginNode(originBlock);
        
        result.success = errors_.empty();
        result.rootNode = originNode;
        result.errors = errors_;
        result.warnings = warnings_;
        
    } catch (const std::exception& e) {
        reportError("原始嵌入解析失败: " + std::string(e.what()));
        result.success = false;
        result.errors = errors_;
    }
    
    return result;
}

OriginReference CHTLOriginParser::parseOriginUsage(const std::string& usage, const std::string& context) {
    errors_.clear();
    warnings_.clear();
    
    // 解析原始嵌入使用语法: [Origin] @Type name;
    std::regex usageRegex(R"(\[Origin\]\s*@(Html|Style|JavaScript)\s+(\w+)\s*;?)");
    std::smatch match;
    
    if (!std::regex_search(usage, match, usageRegex)) {
        reportError("无效的原始嵌入使用语法: " + usage);
        return OriginReference(OriginType::HTML_ORIGIN, "");
    }
    
    // 解析原始嵌入类型
    std::string typeStr = match[1].str();
    OriginType type = parseOriginType(typeStr);
    
    // 解析原始嵌入名称
    std::string originName = match[2].str();
    
    OriginReference ref(type, originName);
    
    // 验证原始嵌入引用
    if (strictMode_ && !originRegistry_->validateOriginReference(ref)) {
        reportError("未定义的原始嵌入: " + originName);
    }
    
    return ref;
}

CHTLASTNodePtr CHTLOriginParser::expandOriginReference(const OriginReference& ref) {
    const OriginBlock* originBlock = originRegistry_->findOriginBlock(ref.type, ref.name);
    if (!originBlock) {
        reportError("未找到原始嵌入定义: " + ref.name);
        return nullptr;
    }
    
    return createOriginNode(*originBlock);
}

void CHTLOriginParser::setOriginRegistry(std::shared_ptr<OriginRegistry> registry) {
    originRegistry_ = registry;
}

std::shared_ptr<OriginRegistry> CHTLOriginParser::getOriginRegistry() const {
    return originRegistry_;
}

void CHTLOriginParser::setStrictMode(bool strict) {
    strictMode_ = strict;
}

void CHTLOriginParser::setPreserveFormatting(bool preserve) {
    preserveFormatting_ = preserve;
}

const std::vector<ParseError>& CHTLOriginParser::getErrors() const {
    return errors_;
}

const std::vector<ParseWarning>& CHTLOriginParser::getWarnings() const {
    return warnings_;
}

// ===== 核心解析方法 =====

OriginBlock CHTLOriginParser::parseOriginBlock(const std::vector<Token>& tokens) {
    size_t index = 0;
    
    // 解析 [Origin]
    if (index >= tokens.size() || tokens[index].type != TokenType::LEFT_BRACKET) {
        throw std::runtime_error("期望 '[Origin]' 开始");
    }
    index++; // 跳过 '['
    
    if (index >= tokens.size() || tokens[index].value != "Origin") {
        throw std::runtime_error("期望 'Origin' 关键字");
    }
    index++; // 跳过 'Origin'
    
    if (index >= tokens.size() || tokens[index].type != TokenType::RIGHT_BRACKET) {
        throw std::runtime_error("期望 ']' 结束");
    }
    index++; // 跳过 ']'
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 解析原始嵌入类型 @Html, @Style, @JavaScript
    if (index >= tokens.size() || tokens[index].type != TokenType::OPERATOR || tokens[index].value != "@") {
        throw std::runtime_error("期望原始嵌入类型标识符 '@'");
    }
    index++; // 跳过 '@'
    
    if (index >= tokens.size() || tokens[index].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("期望原始嵌入类型名称");
    }
    
    OriginType type = parseOriginType(tokens[index].value);
    Position pos = tokens[index].position;
    index++; // 跳过类型名称
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 确定模式并解析名称（可选）
    OriginMode mode = determineOriginMode(tokens, index);
    std::string originName;
    
    if (mode == OriginMode::ENHANCED_DECLARE || mode == OriginMode::ENHANCED_USAGE) {
        originName = parseOriginName(tokens, index);
    }
    
    // 创建原始嵌入块
    OriginBlock originBlock(type, mode, originName, pos);
    
    // 如果不是使用模式，解析内容
    if (mode != OriginMode::ENHANCED_USAGE) {
        // 跳过空白
        while (index < tokens.size() && 
               (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
            index++;
        }
        
        // 解析原始嵌入内容
        originBlock.content = parseOriginContent(tokens, index, type);
    }
    
    return originBlock;
}

OriginType CHTLOriginParser::parseOriginType(const std::string& typeStr) {
    if (typeStr == "Html") {
        return OriginType::HTML_ORIGIN;
    } else if (typeStr == "Style") {
        return OriginType::STYLE_ORIGIN;
    } else if (typeStr == "JavaScript") {
        return OriginType::JAVASCRIPT_ORIGIN;
    } else {
        throw std::runtime_error("未知的原始嵌入类型: " + typeStr);
    }
}

std::string CHTLOriginParser::parseOriginName(const std::vector<Token>& tokens, size_t& index) {
    if (index >= tokens.size() || tokens[index].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("期望原始嵌入名称");
    }
    
    std::string name = tokens[index].value;
    index++;
    
    if (!isValidOriginName(name)) {
        reportWarning("原始嵌入名称不符合命名规范: " + name);
    }
    
    return name;
}

std::string CHTLOriginParser::parseOriginContent(const std::vector<Token>& tokens, size_t& index, OriginType type) {
    // 期望 '{'
    if (index >= tokens.size() || tokens[index].type != TokenType::LEFT_BRACE) {
        throw std::runtime_error("期望 '{' 开始原始嵌入内容");
    }
    index++; // 跳过 '{'
    
    // 收集原始嵌入内容直到 '}'
    std::string content;
    int braceCount = 1;
    
    while (index < tokens.size() && braceCount > 0) {
        const auto& token = tokens[index];
        
        if (token.type == TokenType::LEFT_BRACE) {
            braceCount++;
        } else if (token.type == TokenType::RIGHT_BRACE) {
            braceCount--;
            if (braceCount == 0) {
                break; // 不包含最后的 }
            }
        }
        
        content += token.value;
        // 保持原始格式，不添加额外空格
        index++;
    }
    
    if (braceCount > 0) {
        throw std::runtime_error("未闭合的原始嵌入内容块");
    }
    
    index++; // 跳过最后的 '}'
    
    // 根据类型处理内容
    switch (type) {
        case OriginType::HTML_ORIGIN:
            content = processHtmlOrigin(content);
            break;
        case OriginType::STYLE_ORIGIN:
            content = processCssOrigin(content);
            break;
        case OriginType::JAVASCRIPT_ORIGIN:
            content = processJavaScriptOrigin(content);
            break;
    }
    
    return content;
}

OriginMode CHTLOriginParser::determineOriginMode(const std::vector<Token>& tokens, size_t index) {
    // 检查下一个token来确定模式
    if (index < tokens.size()) {
        if (tokens[index].type == TokenType::IDENTIFIER) {
            // 有名称，检查后面是否有分号
            size_t nextIndex = index + 1;
            while (nextIndex < tokens.size() && 
                   (tokens[nextIndex].type == TokenType::WHITESPACE || tokens[nextIndex].type == TokenType::NEWLINE)) {
                nextIndex++;
            }
            
            if (nextIndex < tokens.size() && tokens[nextIndex].type == TokenType::SEMICOLON) {
                return OriginMode::ENHANCED_USAGE;
            } else {
                return OriginMode::ENHANCED_DECLARE;
            }
        } else if (tokens[index].type == TokenType::LEFT_BRACE) {
            return OriginMode::INLINE_EMBED;
        }
    }
    
    return OriginMode::INLINE_EMBED; // 默认
}

// ===== 原始嵌入处理方法 =====

CHTLASTNodePtr CHTLOriginParser::createOriginNode(const OriginBlock& block) {
    std::string typeStr = getOriginTypeString(block.type);
    std::string modeStr = getOriginModeString(block.mode);
    
    auto originNode = std::make_shared<OriginEmbedNode>(typeStr, block.content, block.position);
    originNode->name = block.name;
    
    return originNode;
}

std::string CHTLOriginParser::processHtmlOrigin(const std::string& content) {
    // HTML原始嵌入 - 保持原样
    std::string processed = content;
    
    if (preserveFormatting_) {
        // 保持HTML格式化
        processed = OriginUtils::formatOriginContent(processed, OriginType::HTML_ORIGIN);
    }
    
    return processed;
}

std::string CHTLOriginParser::processCssOrigin(const std::string& content) {
    // CSS原始嵌入 - 保持原样
    std::string processed = content;
    
    if (preserveFormatting_) {
        // 保持CSS格式化
        processed = OriginUtils::formatOriginContent(processed, OriginType::STYLE_ORIGIN);
    }
    
    return processed;
}

std::string CHTLOriginParser::processJavaScriptOrigin(const std::string& content) {
    // JavaScript原始嵌入 - 保持原样
    std::string processed = content;
    
    if (preserveFormatting_) {
        // 保持JavaScript格式化
        processed = OriginUtils::formatOriginContent(processed, OriginType::JAVASCRIPT_ORIGIN);
    }
    
    return processed;
}

bool CHTLOriginParser::validateOriginContent(const std::string& content, OriginType type) {
    return scanner_.validateOriginContent(content, type);
}

void CHTLOriginParser::sanitizeOriginContent(std::string& content, OriginType type) {
    // 清理原始内容（如果需要）
    content = OriginUtils::cleanOriginContent(content, type);
}

// ===== 工具方法 =====

void CHTLOriginParser::reportError(const std::string& message, const Position& position) {
    errors_.emplace_back(message, position);
}

void CHTLOriginParser::reportWarning(const std::string& message, const Position& position) {
    warnings_.emplace_back(message, position);
}

bool CHTLOriginParser::isValidOriginName(const std::string& name) const {
    // 原始嵌入名称验证：只允许字母、数字和下划线，且以字母开头
    if (name.empty() || !std::isalpha(name[0])) {
        return false;
    }
    
    for (char c : name) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    
    return true;
}

std::string CHTLOriginParser::getOriginTypeString(OriginType type) const {
    switch (type) {
        case OriginType::HTML_ORIGIN: return "@Html";
        case OriginType::STYLE_ORIGIN: return "@Style";
        case OriginType::JAVASCRIPT_ORIGIN: return "@JavaScript";
    }
    return "@Html";
}

std::string CHTLOriginParser::getOriginModeString(OriginMode mode) const {
    switch (mode) {
        case OriginMode::INLINE_EMBED: return "inline";
        case OriginMode::ENHANCED_DECLARE: return "enhanced_declare";
        case OriginMode::ENHANCED_USAGE: return "enhanced_usage";
    }
    return "inline";
}

// ===== OriginUtils 实现 =====

bool OriginUtils::isOriginDeclaration(const std::string& source) {
    std::regex originRegex(R"(\[Origin\]\s*@(Html|Style|JavaScript)(\s+\w+)?\s*\{)");
    return std::regex_search(source, originRegex);
}

bool OriginUtils::isOriginUsage(const std::string& source) {
    std::regex usageRegex(R"(\[Origin\]\s*@(Html|Style|JavaScript)\s+\w+\s*;)");
    return std::regex_search(source, usageRegex);
}

std::string OriginUtils::extractOriginTypeString(const std::string& source) {
    std::regex typeRegex(R"(@(Html|Style|JavaScript))");
    std::smatch match;
    if (std::regex_search(source, match, typeRegex)) {
        return match[1].str();
    }
    return "";
}

std::string OriginUtils::extractOriginName(const std::string& source) {
    std::regex nameRegex(R"(@(?:Html|Style|JavaScript)\s+(\w+))");
    std::smatch match;
    if (std::regex_search(source, match, nameRegex)) {
        return match[1].str();
    }
    return "";
}

bool OriginUtils::validateOriginSyntax(const std::string& source) {
    return isOriginDeclaration(source) || isOriginUsage(source);
}

OriginType OriginUtils::detectOriginType(const std::string& content) {
    // 简单的内容类型检测
    if (content.find("<html>") != std::string::npos || 
        content.find("<div>") != std::string::npos ||
        content.find("<span>") != std::string::npos) {
        return OriginType::HTML_ORIGIN;
    }
    
    if (content.find("function") != std::string::npos ||
        content.find("var") != std::string::npos ||
        content.find("let") != std::string::npos ||
        content.find("const") != std::string::npos) {
        return OriginType::JAVASCRIPT_ORIGIN;
    }
    
    if (content.find("{") != std::string::npos && content.find(":") != std::string::npos) {
        return OriginType::STYLE_ORIGIN;
    }
    
    return OriginType::HTML_ORIGIN; // 默认
}

std::string OriginUtils::cleanOriginContent(const std::string& content, OriginType type) {
    // 清理不必要的空白和注释（如果需要）
    std::string cleaned = content;
    
    // 保持原始内容的完整性，只做最小的清理
    
    return cleaned;
}

std::string OriginUtils::formatOriginContent(const std::string& content, OriginType type) {
    // 格式化原始内容（保持可读性）
    return content; // 原始嵌入应该保持原样
}

bool OriginUtils::isValidHtml(const std::string& content) {
    // 简单的HTML验证
    return true; // 原始嵌入允许任何HTML内容
}

bool OriginUtils::isValidCss(const std::string& content) {
    // 简单的CSS验证
    return true; // 原始嵌入允许任何CSS内容
}

bool OriginUtils::isValidJavaScript(const std::string& content) {
    // 简单的JavaScript验证
    return true; // 原始嵌入允许任何JavaScript内容
}

} // namespace chtl