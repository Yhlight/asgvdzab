#include "parsers/chtl_custom_parser.hpp"
#include "parsers/chtl_basic_parser.hpp"
#include <regex>
#include <algorithm>
#include <sstream>

namespace chtl {

// ===== CustomRegistry 实现 =====

CustomRegistry::CustomRegistry() = default;

CustomRegistry::~CustomRegistry() = default;

bool CustomRegistry::registerCustom(const CustomDefinition& customDef) {
    std::string key = generateCustomKey(customDef.type, customDef.name);
    
    // 检查是否已存在
    if (customs_.find(key) != customs_.end()) {
        return false; // 自定义已存在
    }
    
    customs_[key] = customDef;
    return true;
}

const CustomDefinition* CustomRegistry::findCustom(CustomType type, const std::string& name) const {
    std::string key = generateCustomKey(type, name);
    auto it = customs_.find(key);
    return (it != customs_.end()) ? &it->second : nullptr;
}

const std::unordered_map<std::string, CustomDefinition>& CustomRegistry::getAllCustoms() const {
    return customs_;
}

void CustomRegistry::clear() {
    customs_.clear();
}

bool CustomRegistry::validateCustomReference(const CustomReference& ref) const {
    return findCustom(ref.type, ref.customName) != nullptr;
}

std::string CustomRegistry::generateCustomKey(CustomType type, const std::string& name) const {
    std::string prefix;
    switch (type) {
        case CustomType::STYLE_CUSTOM:
            prefix = "style:";
            break;
        case CustomType::ELEMENT_CUSTOM:
            prefix = "element:";
            break;
        case CustomType::VAR_CUSTOM:
            prefix = "var:";
            break;
    }
    return prefix + name;
}

// ===== CustomScanner 实现 =====

CustomScanner::CustomScanner() : currentPos_(0), line_(1), column_(1) {}

CustomScanner::~CustomScanner() = default;

std::vector<Token> CustomScanner::scanCustomContent(const std::string& content, const CustomContext& context) {
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

        // 特殊关键字识别
        if (isAlpha(c)) {
            std::string word;
            size_t startPos = currentPos_;
            while (!isEOF() && (isAlphaNumeric(currentChar()) || currentChar() == '-' || currentChar() == '_')) {
                word += currentChar();
                advance();
            }
            
            // 重置位置用于重新扫描
            currentPos_ = startPos;
            
            if (word == "delete") {
                tokens.push_back(scanDeleteKeyword());
                continue;
            } else if (word == "insert") {
                tokens.push_back(scanInsertKeyword());
                continue;
            } else if (word == "after" || word == "before" || word == "replace" || 
                       word == "at" || word == "top" || word == "bottom") {
                tokens.push_back(scanPositionKeyword());
                continue;
            }
            
            // 普通标识符
            std::string identifier;
            while (!isEOF() && (isAlphaNumeric(currentChar()) || currentChar() == '-' || currentChar() == '_')) {
                identifier += currentChar();
                advance();
            }
            tokens.emplace_back(TokenType::IDENTIFIER, identifier, pos);
        } else if (c == '[') {
            // 索引访问
            tokens.push_back(scanIndexAccess());
        } else if (c == ',') {
            // 无值属性分隔符
            tokens.emplace_back(TokenType::OPERATOR, ",", pos);
            advance();
        } else if (c == '=') {
            // 赋值操作
            tokens.emplace_back(TokenType::ASSIGNMENT, "=", pos);
            advance();
        } else if (c == '{') {
            tokens.emplace_back(TokenType::LEFT_BRACE, "{", pos);
            advance();
        } else if (c == '}') {
            tokens.emplace_back(TokenType::RIGHT_BRACE, "}", pos);
            advance();
        } else if (c == ';') {
            tokens.emplace_back(TokenType::SEMICOLON, ";", pos);
            advance();
        } else if (c == ':') {
            tokens.emplace_back(TokenType::COLON, ":", pos);
            advance();
        } else if (c == '@') {
            tokens.emplace_back(TokenType::OPERATOR, "@", pos);
            advance();
        } else if (c == '"' || c == '\'') {
            // 字符串字面量
            char quote = c;
            advance();
            std::string value;
            while (!isEOF() && currentChar() != quote) {
                value += currentChar();
                advance();
            }
            if (!isEOF()) advance(); // 跳过结束引号
            tokens.emplace_back(TokenType::STRING_LITERAL, value, pos);
        } else if (isDigit(c)) {
            // 数字
            std::string number;
            while (!isEOF() && (isDigit(currentChar()) || currentChar() == '.')) {
                number += currentChar();
                advance();
            }
            tokens.emplace_back(TokenType::NUMBER, number, pos);
        } else {
            // 其他字符
            tokens.emplace_back(TokenType::OPERATOR, std::string(1, c), pos);
            advance();
        }
    }

    return tokens;
}

Token CustomScanner::scanDeleteKeyword() {
    Position pos = getCurrentPosition();
    std::string keyword = "delete";
    
    for (char c : keyword) {
        if (!isEOF() && currentChar() == c) {
            advance();
        } else {
            break;
        }
    }
    
    return Token(TokenType::KEYWORD, keyword, pos);
}

Token CustomScanner::scanInsertKeyword() {
    Position pos = getCurrentPosition();
    std::string keyword = "insert";
    
    for (char c : keyword) {
        if (!isEOF() && currentChar() == c) {
            advance();
        } else {
            break;
        }
    }
    
    return Token(TokenType::KEYWORD, keyword, pos);
}

Token CustomScanner::scanPositionKeyword() {
    Position pos = getCurrentPosition();
    std::string keyword;
    
    // 读取位置关键字
    while (!isEOF() && (isAlphaNumeric(currentChar()) || currentChar() == ' ')) {
        keyword += currentChar();
        advance();
    }
    
    return Token(TokenType::KEYWORD, keyword, pos);
}

Token CustomScanner::scanIndexAccess() {
    Position pos = getCurrentPosition();
    std::string access = "[";
    advance(); // 跳过 '['
    
    while (!isEOF() && currentChar() != ']') {
        access += currentChar();
        advance();
    }
    
    if (!isEOF()) {
        access += currentChar(); // 添加 ']'
        advance();
    }
    
    return Token(TokenType::LEFT_BRACKET, access, pos);
}

char CustomScanner::currentChar() const {
    if (currentPos_ >= content_.length()) return '\0';
    return content_[currentPos_];
}

char CustomScanner::peekChar(size_t offset) const {
    size_t peekPos = currentPos_ + offset;
    if (peekPos >= content_.length()) return '\0';
    return content_[peekPos];
}

void CustomScanner::advance() {
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

bool CustomScanner::isEOF() const {
    return currentPos_ >= content_.length();
}

Position CustomScanner::getCurrentPosition() const {
    return Position{static_cast<size_t>(line_), static_cast<size_t>(column_)};
}

bool CustomScanner::isAlpha(char c) const {
    return std::isalpha(c) || c > 127;
}

bool CustomScanner::isDigit(char c) const {
    return std::isdigit(c);
}

bool CustomScanner::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool CustomScanner::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

void CustomScanner::skipWhitespace() {
    while (!isEOF() && isWhitespace(currentChar())) {
        advance();
    }
}

// ===== CustomStateMachine 实现 =====

CustomStateMachine::CustomStateMachine() 
    : currentState_(CustomParseState::INITIAL), currentPosition_(InsertPosition::AFTER), currentIndex_(-1) {}

CustomStateMachine::~CustomStateMachine() = default;

void CustomStateMachine::reset() {
    currentState_ = CustomParseState::INITIAL;
    specializations_.clear();
    valuelessProperties_.clear();
    currentOperation_.clear();
    currentTarget_.clear();
    currentPosition_ = InsertPosition::AFTER;
    currentIndex_ = -1;
}

bool CustomStateMachine::processToken(const Token& token, CustomContext& context) {
    currentLocation_ = token.position;

    switch (currentState_) {
        case CustomParseState::INITIAL:
            handleInitialState(token, context);
            break;
        case CustomParseState::READING_SPECIALIZATION:
            handleReadingSpecializationState(token, context);
            break;
        case CustomParseState::READING_VALUELESS_PROPS:
            handleReadingValuelessPropsState(token, context);
            break;
        case CustomParseState::READING_INSERT_OP:
            handleReadingInsertOpState(token, context);
            break;
        case CustomParseState::READING_DELETE_OP:
            handleReadingDeleteOpState(token, context);
            break;
        case CustomParseState::READING_INDEX_ACCESS:
            handleReadingIndexAccessState(token, context);
            break;
        default:
            break;
    }

    return true;
}

CustomParseState CustomStateMachine::getCurrentState() const {
    return currentState_;
}

const std::vector<SpecializationOperation>& CustomStateMachine::getSpecializations() const {
    return specializations_;
}

const std::vector<ValuelessProperty>& CustomStateMachine::getValuelessProperties() const {
    return valuelessProperties_;
}

void CustomStateMachine::handleInitialState(const Token& token, CustomContext& context) {
    if (token.type == TokenType::KEYWORD && token.value == "delete") {
        currentOperation_ = "delete";
        currentState_ = CustomParseState::READING_DELETE_OP;
    } else if (token.type == TokenType::KEYWORD && token.value == "insert") {
        currentOperation_ = "insert";
        currentState_ = CustomParseState::READING_INSERT_OP;
    } else if (token.type == TokenType::IDENTIFIER) {
        // 可能是无值属性或索引访问
        currentTarget_ = token.value;
        currentState_ = CustomParseState::READING_VALUELESS_PROPS;
    }
}

void CustomStateMachine::handleReadingSpecializationState(const Token& token, CustomContext& context) {
    // 处理特例化操作的一般逻辑
    if (token.type == TokenType::SEMICOLON) {
        currentState_ = CustomParseState::INITIAL;
    }
}

void CustomStateMachine::handleReadingValuelessPropsState(const Token& token, CustomContext& context) {
    if (token.type == TokenType::OPERATOR && token.value == ",") {
        // 无值属性
        addValuelessProperty(ValuelessProperty(currentTarget_, currentLocation_));
        currentTarget_.clear();
        currentState_ = CustomParseState::INITIAL;
    } else if (token.type == TokenType::LEFT_BRACKET) {
        // 索引访问
        currentState_ = CustomParseState::READING_INDEX_ACCESS;
    } else if (token.type == TokenType::SEMICOLON) {
        // 单个无值属性结束
        addValuelessProperty(ValuelessProperty(currentTarget_, currentLocation_));
        currentTarget_.clear();
        currentState_ = CustomParseState::INITIAL;
    }
}

void CustomStateMachine::handleReadingInsertOpState(const Token& token, CustomContext& context) {
    if (token.type == TokenType::KEYWORD) {
        // 插入位置关键字
        currentPosition_ = parseInsertPosition(token.value);
    } else if (token.type == TokenType::IDENTIFIER) {
        // 目标选择器
        currentTarget_ = token.value;
    } else if (token.type == TokenType::LEFT_BRACE) {
        // 创建插入操作
        SpecializationOperation op(SpecializationType::INSERT_ELEMENT, currentTarget_, currentLocation_);
        op.position = currentPosition_;
        addSpecialization(op);
        currentState_ = CustomParseState::INITIAL;
    }
}

void CustomStateMachine::handleReadingDeleteOpState(const Token& token, CustomContext& context) {
    if (token.type == TokenType::IDENTIFIER) {
        currentTarget_ = token.value;
    } else if (token.type == TokenType::OPERATOR && token.value == "@") {
        // 删除继承
        currentTarget_ = "@";
    } else if (token.type == TokenType::SEMICOLON || (token.type == TokenType::OPERATOR && token.value == ",")) {
        // 创建删除操作
        SpecializationType opType = currentTarget_.front() == '@' ? 
            SpecializationType::DELETE_INHERITANCE : 
            (context.currentType == CustomType::STYLE_CUSTOM ? 
                SpecializationType::DELETE_PROPERTY : 
                SpecializationType::DELETE_ELEMENT);
        
        SpecializationOperation op(opType, currentTarget_, currentLocation_);
        addSpecialization(op);
        
        if (token.type == TokenType::SEMICOLON) {
            currentState_ = CustomParseState::INITIAL;
        }
        currentTarget_.clear();
    }
}

void CustomStateMachine::handleReadingIndexAccessState(const Token& token, CustomContext& context) {
    // 解析索引访问 element[index]
    if (token.type == TokenType::LEFT_BRACKET) {
        // 提取索引
        std::string indexStr = token.value;
        std::regex indexRegex(R"(\[(\d+)\])");
        std::smatch match;
        
        if (std::regex_search(indexStr, match, indexRegex)) {
            currentIndex_ = std::stoi(match[1].str());
            
            SpecializationOperation op(SpecializationType::INDEX_ACCESS, currentTarget_, currentLocation_);
            op.index = currentIndex_;
            addSpecialization(op);
        }
        
        currentState_ = CustomParseState::INITIAL;
    }
}

void CustomStateMachine::addSpecialization(const SpecializationOperation& op) {
    specializations_.push_back(op);
}

void CustomStateMachine::addValuelessProperty(const ValuelessProperty& prop) {
    valuelessProperties_.push_back(prop);
}

InsertPosition CustomStateMachine::parseInsertPosition(const std::string& posStr) {
    if (posStr == "after") return InsertPosition::AFTER;
    if (posStr == "before") return InsertPosition::BEFORE;
    if (posStr == "replace") return InsertPosition::REPLACE;
    if (posStr == "at top") return InsertPosition::AT_TOP;
    if (posStr == "at bottom") return InsertPosition::AT_BOTTOM;
    return InsertPosition::AFTER; // 默认
}

bool CustomStateMachine::isValidInsertPosition(const std::string& posStr) const {
    return posStr == "after" || posStr == "before" || posStr == "replace" ||
           posStr == "at top" || posStr == "at bottom";
}

bool CustomStateMachine::isValidDeleteTarget(const std::string& target) const {
    return !target.empty();
}

// ===== CHTLCustomParser 实现 =====

CHTLCustomParser::CHTLCustomParser() 
    : customRegistry_(std::make_shared<CustomRegistry>())
    , currentState_(CustomParseState::INITIAL)
    , strictMode_(true) {
}

CHTLCustomParser::~CHTLCustomParser() = default;

ParseResult CHTLCustomParser::parseCustomDeclaration(const std::string& source, const std::string& filename) {
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
        // 解析自定义定义
        CustomDefinition customDef = parseCustomDefinition(lexResult.tokens);
        
        // 注册自定义
        if (!customRegistry_->registerCustom(customDef)) {
            reportError("自定义名称重复: " + customDef.name, customDef.position);
        }
        
        // 创建自定义声明节点
        std::string typeStr;
        switch (customDef.type) {
            case CustomType::STYLE_CUSTOM: typeStr = "@Style"; break;
            case CustomType::ELEMENT_CUSTOM: typeStr = "@Element"; break;
            case CustomType::VAR_CUSTOM: typeStr = "@Var"; break;
        }
        auto customNode = std::make_shared<CustomDeclarationNode>(
            typeStr, customDef.name, customDef.position
        );
        
        result.success = errors_.empty();
        result.rootNode = customNode;
        result.errors = errors_;
        result.warnings = warnings_;
        
    } catch (const std::exception& e) {
        reportError("自定义解析失败: " + std::string(e.what()));
        result.success = false;
        result.errors = errors_;
    }
    
    return result;
}

CustomReference CHTLCustomParser::parseCustomUsage(const std::string& usage, const std::string& context) {
    errors_.clear();
    warnings_.clear();
    
    // 解析自定义使用语法: @Style CustomName { 特例化操作 }
    std::regex usageRegex(R"(@(Style|Element|Var)\s+(\w+)\s*\{)");
    std::smatch match;
    
    if (!std::regex_search(usage, match, usageRegex)) {
        reportError("无效的自定义使用语法: " + usage);
        return CustomReference(CustomType::STYLE_CUSTOM, "");
    }
    
    // 解析自定义类型
    std::string typeStr = match[1].str();
    CustomType type = parseCustomType(typeStr);
    
    // 解析自定义名称
    std::string customName = match[2].str();
    
    CustomReference ref(type, customName);
    
    // 验证自定义引用
    if (strictMode_ && !customRegistry_->validateCustomReference(ref)) {
        reportError("未定义的自定义: " + customName);
    }
    
    return ref;
}

CHTLASTNodePtr CHTLCustomParser::expandCustomReference(const CustomReference& ref) {
    const CustomDefinition* customDef = customRegistry_->findCustom(ref.type, ref.customName);
    if (!customDef) {
        reportError("未找到自定义定义: " + ref.customName);
        return nullptr;
    }
    
    switch (ref.type) {
        case CustomType::STYLE_CUSTOM:
            return expandStyleCustom(*customDef, ref);
        case CustomType::ELEMENT_CUSTOM:
            return expandElementCustom(*customDef, ref);
        case CustomType::VAR_CUSTOM:
            return expandVariableCustom(*customDef, ref);
    }
    
    return nullptr;
}

void CHTLCustomParser::setCustomRegistry(std::shared_ptr<CustomRegistry> registry) {
    customRegistry_ = registry;
}

std::shared_ptr<CustomRegistry> CHTLCustomParser::getCustomRegistry() const {
    return customRegistry_;
}

void CHTLCustomParser::setTemplateRegistry(std::shared_ptr<TemplateRegistry> templateRegistry) {
    templateRegistry_ = templateRegistry;
}

void CHTLCustomParser::setStrictMode(bool strict) {
    strictMode_ = strict;
}

const std::vector<ParseError>& CHTLCustomParser::getErrors() const {
    return errors_;
}

const std::vector<ParseWarning>& CHTLCustomParser::getWarnings() const {
    return warnings_;
}

// ===== 核心解析方法 =====

CustomDefinition CHTLCustomParser::parseCustomDefinition(const std::vector<Token>& tokens) {
    size_t index = 0;
    
    // 解析 [Custom]
    if (index >= tokens.size() || tokens[index].type != TokenType::LEFT_BRACKET) {
        throw std::runtime_error("期望 '[Custom]' 开始");
    }
    index++; // 跳过 '['
    
    if (index >= tokens.size() || tokens[index].value != "Custom") {
        throw std::runtime_error("期望 'Custom' 关键字");
    }
    index++; // 跳过 'Custom'
    
    if (index >= tokens.size() || tokens[index].type != TokenType::RIGHT_BRACKET) {
        throw std::runtime_error("期望 ']' 结束");
    }
    index++; // 跳过 ']'
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 解析自定义类型 @Style, @Element, @Var
    if (index >= tokens.size() || tokens[index].type != TokenType::OPERATOR || tokens[index].value != "@") {
        throw std::runtime_error("期望自定义类型标识符 '@'");
    }
    index++; // 跳过 '@'
    
    if (index >= tokens.size() || tokens[index].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("期望自定义类型名称");
    }
    
    CustomType type = parseCustomType(tokens[index].value);
    Position pos = tokens[index].position;
    index++; // 跳过类型名称
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 解析自定义名称
    std::string customName = parseCustomName(tokens, index);
    
    // 创建自定义定义
    CustomDefinition customDef(type, customName, pos);
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 解析自定义内容
    customDef.content = parseCustomContent(tokens, index, type);
    
    return customDef;
}

CustomType CHTLCustomParser::parseCustomType(const std::string& typeStr) {
    if (typeStr == "Style") {
        return CustomType::STYLE_CUSTOM;
    } else if (typeStr == "Element") {
        return CustomType::ELEMENT_CUSTOM;
    } else if (typeStr == "Var") {
        return CustomType::VAR_CUSTOM;
    } else {
        throw std::runtime_error("未知的自定义类型: " + typeStr);
    }
}

std::string CHTLCustomParser::parseCustomName(const std::vector<Token>& tokens, size_t& index) {
    if (index >= tokens.size() || tokens[index].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("期望自定义名称");
    }
    
    std::string name = tokens[index].value;
    index++;
    
    if (!isValidCustomName(name)) {
        reportWarning("自定义名称不符合命名规范: " + name);
    }
    
    return name;
}

CHTLASTNodePtr CHTLCustomParser::parseCustomContent(const std::vector<Token>& tokens, size_t& index, CustomType type) {
    // 期望 '{'
    if (index >= tokens.size() || tokens[index].type != TokenType::LEFT_BRACE) {
        throw std::runtime_error("期望 '{' 开始自定义内容");
    }
    index++; // 跳过 '{'
    
    // 收集自定义内容直到 '}'
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
        // 不在值后面自动添加空格，让内容保持原样
        index++;
    }
    
    if (braceCount > 0) {
        throw std::runtime_error("未闭合的自定义内容块");
    }
    
    index++; // 跳过最后的 '}'
    
    // 创建一个简单的容器节点
    auto containerNode = std::make_shared<ProgramNode>();
    
    return containerNode;
}

// ===== 自定义展开方法 =====

CHTLASTNodePtr CHTLCustomParser::expandStyleCustom(const CustomDefinition& customDef, const CustomReference& ref) {
    if (!customDef.content) {
        reportError("样式自定义内容为空: " + customDef.name);
        return nullptr;
    }
    
    // 克隆自定义内容
    auto expandedContent = customDef.content->clone();
    
    // 应用特例化操作
    applySpecializations(expandedContent, ref.specializations);
    
    return expandedContent;
}

CHTLASTNodePtr CHTLCustomParser::expandElementCustom(const CustomDefinition& customDef, const CustomReference& ref) {
    if (!customDef.content) {
        reportError("元素自定义内容为空: " + customDef.name);
        return nullptr;
    }
    
    // 克隆自定义内容
    auto expandedContent = customDef.content->clone();
    
    // 应用特例化操作
    applySpecializations(expandedContent, ref.specializations);
    
    return expandedContent;
}

CHTLASTNodePtr CHTLCustomParser::expandVariableCustom(const CustomDefinition& customDef, const CustomReference& ref) {
    if (!customDef.content) {
        reportError("变量自定义内容为空: " + customDef.name);
        return nullptr;
    }
    
    // 克隆自定义内容
    auto expandedContent = customDef.content->clone();
    
    // 应用变量覆盖
    for (const auto& override : ref.variableOverrides) {
        // 在这里处理变量覆盖逻辑
        // 这需要遍历AST并替换变量值
    }
    
    return expandedContent;
}

// ===== 特例化操作处理方法 =====

void CHTLCustomParser::applySpecializations(CHTLASTNodePtr& target, const std::vector<SpecializationOperation>& specializations) {
    for (const auto& op : specializations) {
        switch (op.type) {
            case SpecializationType::DELETE_PROPERTY:
            case SpecializationType::DELETE_ELEMENT:
            case SpecializationType::DELETE_INHERITANCE:
                applyDeleteOperation(target, op);
                break;
            case SpecializationType::INSERT_ELEMENT:
                applyInsertOperation(target, op);
                break;
            case SpecializationType::INDEX_ACCESS:
                applyIndexAccessOperation(target, op);
                break;
            default:
                break;
        }
    }
}

void CHTLCustomParser::applyDeleteOperation(CHTLASTNodePtr& target, const SpecializationOperation& op) {
    // 实现删除操作 - 这里简化处理
    if (target && !target->children.empty()) {
        // 简单的删除逻辑 - 根据目标名称删除子节点
        auto it = std::remove_if(target->children.begin(), target->children.end(),
            [&op](const CHTLASTNodePtr& child) {
                // 简化的匹配逻辑
                return false; // 实际应该检查节点名称或类型
            });
        target->children.erase(it, target->children.end());
    }
}

void CHTLCustomParser::applyInsertOperation(CHTLASTNodePtr& target, const SpecializationOperation& op) {
    // 实现插入操作 - 这里简化处理
    if (target && op.content) {
        switch (op.position) {
            case InsertPosition::AT_TOP:
                target->children.insert(target->children.begin(), op.content);
                break;
            case InsertPosition::AT_BOTTOM:
                target->children.push_back(op.content);
                break;
            default:
                target->children.push_back(op.content);
                break;
        }
    }
}

void CHTLCustomParser::applyIndexAccessOperation(CHTLASTNodePtr& target, const SpecializationOperation& op) {
    // 实现索引访问操作 - 这里简化处理
    if (target && op.index >= 0 && op.index < static_cast<int>(target->children.size())) {
        // 可以修改特定索引的子节点
        auto& targetChild = target->children[op.index];
        if (op.content) {
            targetChild = op.content;
        }
    }
}

// ===== 工具方法 =====

void CHTLCustomParser::reportError(const std::string& message, const Position& position) {
    errors_.emplace_back(message, position);
}

void CHTLCustomParser::reportWarning(const std::string& message, const Position& position) {
    warnings_.emplace_back(message, position);
}

bool CHTLCustomParser::isValidCustomName(const std::string& name) const {
    // 自定义名称验证：只允许字母、数字和下划线，且以字母开头
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

bool CHTLCustomParser::isCompatibleWithTemplate(const CustomDefinition& customDef, const TemplateDefinition& templateDef) const {
    // 检查自定义和模板的兼容性
    return (customDef.type == CustomType::STYLE_CUSTOM && templateDef.type == TemplateType::STYLE_TEMPLATE) ||
           (customDef.type == CustomType::ELEMENT_CUSTOM && templateDef.type == TemplateType::ELEMENT_TEMPLATE) ||
           (customDef.type == CustomType::VAR_CUSTOM && templateDef.type == TemplateType::VAR_TEMPLATE);
}

// ===== CustomUtils 实现 =====

bool CustomUtils::isCustomDeclaration(const std::string& source) {
    std::regex customRegex(R"(\[Custom\]\s*@(Style|Element|Var)\s+\w+\s*\{)");
    return std::regex_search(source, customRegex);
}

bool CustomUtils::isCustomUsage(const std::string& source) {
    std::regex usageRegex(R"(@(Style|Element|Var)\s+\w+\s*\{)");
    return std::regex_search(source, usageRegex);
}

bool CustomUtils::isSpecializationOperation(const std::string& source) {
    std::regex specRegex(R"((delete|insert)\s+)");
    return std::regex_search(source, specRegex);
}

std::string CustomUtils::extractCustomTypeString(const std::string& source) {
    std::regex typeRegex(R"(@(Style|Element|Var))");
    std::smatch match;
    if (std::regex_search(source, match, typeRegex)) {
        return match[1].str();
    }
    return "";
}

std::string CustomUtils::extractCustomName(const std::string& source) {
    std::regex nameRegex(R"(@(?:Style|Element|Var)\s+(\w+))");
    std::smatch match;
    if (std::regex_search(source, match, nameRegex)) {
        return match[1].str();
    }
    return "";
}

bool CustomUtils::validateCustomSyntax(const std::string& source) {
    return isCustomDeclaration(source) || isCustomUsage(source) || isSpecializationOperation(source);
}

std::pair<std::string, int> CustomUtils::parseIndexAccess(const std::string& expression) {
    std::regex indexRegex(R"((\w+)\[(\d+)\])");
    std::smatch match;
    if (std::regex_match(expression, match, indexRegex)) {
        return {match[1].str(), std::stoi(match[2].str())};
    }
    return {"", -1};
}

bool CustomUtils::isValidInsertPosition(const std::string& position) {
    return position == "after" || position == "before" || position == "replace" ||
           position == "at top" || position == "at bottom";
}

} // namespace chtl