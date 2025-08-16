#include "parsers/chtl_namespace_parser.hpp"
#include "parsers/chtl_basic_parser.hpp"
#include <regex>
#include <algorithm>
#include <sstream>

namespace chtl {

// ===== NamespaceRegistry 实现 =====

NamespaceRegistry::NamespaceRegistry() = default;

NamespaceRegistry::~NamespaceRegistry() = default;

bool NamespaceRegistry::registerNamespace(const NamespaceDefinition& namespaceDef) {
    // 检查是否已存在
    if (pathToNamespace_.find(namespaceDef.fullPath) != pathToNamespace_.end()) {
        return false; // 重复定义
    }
    
    topLevelNamespaces_.push_back(namespaceDef);
    indexNamespace(topLevelNamespaces_.back());
    
    return true;
}

const NamespaceDefinition* NamespaceRegistry::findNamespace(const std::string& fullPath) const {
    auto it = pathToNamespace_.find(fullPath);
    return (it != pathToNamespace_.end()) ? it->second : nullptr;
}

const std::vector<NamespaceDefinition>& NamespaceRegistry::getTopLevelNamespaces() const {
    return topLevelNamespaces_;
}

bool NamespaceRegistry::isValidNamespacePath(const std::string& path) const {
    return pathToNamespace_.find(path) != pathToNamespace_.end();
}

std::string NamespaceRegistry::getFullPath(const std::string& parentPath, const std::string& childName) const {
    if (parentPath.empty()) {
        return childName;
    }
    return parentPath + "." + childName;
}

bool NamespaceRegistry::isConstraintViolated(const std::string& namespacePath, const ConstraintTarget& target) const {
    const auto* namespaceDef = findNamespace(namespacePath);
    if (!namespaceDef) {
        return false; // 命名空间不存在，不应用约束
    }
    
    // 检查全局约束
    for (const auto& constraint : namespaceDef->constraints) {
        if (isConstraintApplicable(constraint, target)) {
            return true; // 违反约束
        }
    }
    
    return false;
}

void NamespaceRegistry::clear() {
    topLevelNamespaces_.clear();
    pathToNamespace_.clear();
}

void NamespaceRegistry::indexNamespace(const NamespaceDefinition& namespaceDef, const std::string& parentPath) {
    std::string fullPath = getFullPath(parentPath, namespaceDef.name);
    pathToNamespace_[fullPath] = &namespaceDef;
    
    // 递归索引嵌套命名空间
    for (const auto& child : namespaceDef.children) {
        indexNamespace(child, fullPath);
    }
}

bool NamespaceRegistry::isConstraintApplicable(const ConstraintDefinition& constraint, const ConstraintTarget& target) const {
    for (const auto& constraintTarget : constraint.targets) {
        if (constraintTarget.type == target.type && 
            (constraintTarget.name.empty() || constraintTarget.name == target.name)) {
            return true;
        }
    }
    return false;
}

// ===== ConstraintRegistry 实现 =====

ConstraintRegistry::ConstraintRegistry() = default;

ConstraintRegistry::~ConstraintRegistry() = default;

bool ConstraintRegistry::registerConstraint(const std::string& scope, const ConstraintDefinition& constraint) {
    scopeConstraints_[scope].push_back(constraint);
    return true;
}

std::vector<ConstraintDefinition> ConstraintRegistry::getConstraints(const std::string& scope) const {
    auto it = scopeConstraints_.find(scope);
    return (it != scopeConstraints_.end()) ? it->second : std::vector<ConstraintDefinition>{};
}

bool ConstraintRegistry::hasConstraintViolation(const std::string& scope, const ConstraintTarget& target) const {
    auto constraints = getConstraints(scope);
    
    for (const auto& constraint : constraints) {
        if (isViolated(constraint, target)) {
            return true;
        }
    }
    
    return false;
}

void ConstraintRegistry::clear() {
    scopeConstraints_.clear();
}

bool ConstraintRegistry::isViolated(const ConstraintDefinition& constraint, const ConstraintTarget& target) const {
    for (const auto& constraintTarget : constraint.targets) {
        // 检查类型匹配
        if (constraintTarget.type == target.type) {
            // 如果约束目标没有指定名称，或名称匹配，则违反约束
            if (constraintTarget.name.empty() || constraintTarget.name == target.name) {
                return true;
            }
        }
        
        // 检查类型约束 (例如 @Html, [Custom], [Template])
        if (constraintTarget.type == ConstraintTargetType::HTML_TYPE && 
            (target.type == ConstraintTargetType::HTML_ELEMENT)) {
            return true;
        }
        
        if (constraintTarget.type == ConstraintTargetType::CUSTOM_TYPE && 
            (target.type == ConstraintTargetType::CUSTOM_ELEMENT || 
             target.type == ConstraintTargetType::CUSTOM_STYLE || 
             target.type == ConstraintTargetType::CUSTOM_VAR)) {
            return true;
        }
        
        if (constraintTarget.type == ConstraintTargetType::TEMPLATE_TYPE && 
            (target.type == ConstraintTargetType::TEMPLATE_ELEMENT || 
             target.type == ConstraintTargetType::TEMPLATE_STYLE || 
             target.type == ConstraintTargetType::TEMPLATE_VAR)) {
            return true;
        }
    }
    
    return false;
}

// ===== NamespaceScanner 实现 =====

NamespaceScanner::NamespaceScanner() = default;

NamespaceScanner::~NamespaceScanner() = default;

NamespaceDefinition NamespaceScanner::scanNamespaceDeclaration(const std::string& content) {
    NamespaceDefinition namespaceDef;
    
    // 解析命名空间声明的正则表达式
    std::regex namespaceRegex(R"(\[Namespace\]\s+(\w+))");
    std::smatch match;
    
    if (std::regex_search(content, match, namespaceRegex)) {
        namespaceDef.name = match[1].str();
        namespaceDef.fullPath = namespaceDef.name;
        
        // 检查是否有花括号
        namespaceDef.hasBlock = content.find('{') != std::string::npos;
    }
    
    return namespaceDef;
}

ConstraintDefinition NamespaceScanner::scanConstraintDeclaration(const std::string& content) {
    ConstraintDefinition constraint;
    
    // 解析约束声明的正则表达式
    std::regex constraintRegex(R"(except\s+(.+);)");
    std::smatch match;
    
    if (std::regex_search(content, match, constraintRegex)) {
        std::string targetsStr = match[1].str();
        constraint.targets = scanConstraintTargets(targetsStr);
        constraint.type = parseConstraintType(content);
    }
    
    return constraint;
}

std::vector<ConstraintTarget> NamespaceScanner::scanConstraintTargets(const std::string& content) {
    std::vector<ConstraintTarget> targets;
    
    // 分割约束目标
    auto targetStrings = splitTargets(content);
    
    for (const auto& targetStr : targetStrings) {
        ConstraintTarget target = parseConstraintTarget(targetStr);
        if (target.type != ConstraintTargetType::HTML_ELEMENT || !target.name.empty()) {
            targets.push_back(target);
        }
    }
    
    return targets;
}

bool NamespaceScanner::validateNamespaceSyntax(const std::string& content) {
    std::regex namespaceRegex(R"(\[Namespace\]\s+\w+(\s*\{.*\})?)", std::regex_constants::icase);
    return std::regex_search(content, namespaceRegex);
}

bool NamespaceScanner::validateConstraintSyntax(const std::string& content) {
    std::regex constraintRegex(R"(except\s+.+;)", std::regex_constants::icase);
    return std::regex_search(content, constraintRegex);
}

std::string NamespaceScanner::parseNamespaceName(const std::string& line) {
    std::regex nameRegex(R"(\[Namespace\]\s+(\w+))");
    std::smatch match;
    
    if (std::regex_search(line, match, nameRegex)) {
        return match[1].str();
    }
    
    return "";
}

ConstraintType NamespaceScanner::parseConstraintType(const std::string& line) {
    // 如果有具体的类型+名称组合 (如 @Element Box, @Style Theme)，这是精确约束
    if (line.find("@Element") != std::string::npos ||
        line.find("@Style") != std::string::npos ||
        line.find("@Var") != std::string::npos) {
        return ConstraintType::PRECISE;
    }
    
    // 如果只有类型标识符 ([Custom], [Template], @Html)，这是类型约束
    if (line.find("@Html") != std::string::npos ||
        (line.find("[Custom]") != std::string::npos && line.find("@") == std::string::npos) ||
        (line.find("[Template]") != std::string::npos && line.find("@") == std::string::npos)) {
        return ConstraintType::TYPE_BASED;
    }
    
    return ConstraintType::PRECISE;
}

ConstraintTarget NamespaceScanner::parseConstraintTarget(const std::string& targetStr) {
    ConstraintTarget target;
    std::string trimmed = trimWhitespace(targetStr);
    
    target.type = determineTargetType(trimmed);
    
    // 提取名称
    if (target.type == ConstraintTargetType::HTML_ELEMENT) {
        target.name = trimmed;
    } else if (trimmed.find("@Element") != std::string::npos ||
               trimmed.find("@Style") != std::string::npos ||
               trimmed.find("@Var") != std::string::npos) {
        // 提取自定义/模板对象名称
        std::regex nameRegex(R"(@\w+\s+(\w+))");
        std::smatch match;
        if (std::regex_search(trimmed, match, nameRegex)) {
            target.name = match[1].str();
        }
    }
    
    return target;
}

ConstraintTargetType NamespaceScanner::determineTargetType(const std::string& targetStr) {
    if (targetStr.find("[Custom]") != std::string::npos) {
        if (targetStr.find("@Element") != std::string::npos) {
            return ConstraintTargetType::CUSTOM_ELEMENT;
        } else if (targetStr.find("@Style") != std::string::npos) {
            return ConstraintTargetType::CUSTOM_STYLE;
        } else if (targetStr.find("@Var") != std::string::npos) {
            return ConstraintTargetType::CUSTOM_VAR;
        } else {
            return ConstraintTargetType::CUSTOM_TYPE;
        }
    } else if (targetStr.find("[Template]") != std::string::npos) {
        if (targetStr.find("@Element") != std::string::npos) {
            return ConstraintTargetType::TEMPLATE_ELEMENT;
        } else if (targetStr.find("@Style") != std::string::npos) {
            return ConstraintTargetType::TEMPLATE_STYLE;
        } else if (targetStr.find("@Var") != std::string::npos) {
            return ConstraintTargetType::TEMPLATE_VAR;
        } else {
            return ConstraintTargetType::TEMPLATE_TYPE;
        }
    } else if (targetStr.find("@Html") != std::string::npos) {
        return ConstraintTargetType::HTML_TYPE;
    } else {
        // 默认为HTML元素
        return ConstraintTargetType::HTML_ELEMENT;
    }
}

std::string NamespaceScanner::trimWhitespace(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> NamespaceScanner::splitTargets(const std::string& str) {
    std::vector<std::string> targets;
    std::istringstream stream(str);
    std::string target;
    
    while (std::getline(stream, target, ',')) {
        target = trimWhitespace(target);
        if (!target.empty()) {
            targets.push_back(target);
        }
    }
    
    return targets;
}

bool NamespaceScanner::isValidIdentifier(const std::string& identifier) {
    if (identifier.empty()) return false;
    
    std::regex identifierRegex(R"(^[a-zA-Z_][a-zA-Z0-9_]*$)");
    return std::regex_match(identifier, identifierRegex);
}

bool NamespaceScanner::isValidNamespaceName(const std::string& name) {
    return isValidIdentifier(name);
}

// ===== NamespaceStateMachine 实现 =====

NamespaceStateMachine::NamespaceStateMachine() = default;

NamespaceStateMachine::~NamespaceStateMachine() = default;

void NamespaceStateMachine::reset() {
    namespaceDefinitions_.clear();
    constraintDefinitions_.clear();
    currentNamespaceName_.clear();
    currentConstraintTargets_.clear();
    currentConstraint_ = ConstraintDefinition{};
}

bool NamespaceStateMachine::processNamespaceToken(const Token& token, NamespaceContext& context) {
    switch (context.state) {
        case NamespaceParseState::INITIAL:
            handleNamespaceInitialState(token, context);
            break;
        case NamespaceParseState::READING_NAMESPACE_HEADER:
            handleNamespaceHeaderState(token, context);
            break;
        case NamespaceParseState::READING_NAMESPACE_NAME:
            handleNamespaceNameState(token, context);
            break;
        case NamespaceParseState::READING_NAMESPACE_CONTENT:
            handleNamespaceContentState(token, context);
            break;
        default:
            break;
    }
    
    return true;
}

bool NamespaceStateMachine::processConstraintToken(const Token& token, NamespaceContext& context) {
    switch (context.state) {
        case NamespaceParseState::READING_CONSTRAINT:
            handleConstraintInitialState(token, context);
            break;
        case NamespaceParseState::READING_CONSTRAINT_TARGETS:
            handleConstraintTargetsState(token, context);
            break;
        default:
            break;
    }
    
    return true;
}

const std::vector<NamespaceDefinition>& NamespaceStateMachine::getNamespaceDefinitions() const {
    return namespaceDefinitions_;
}

const std::vector<ConstraintDefinition>& NamespaceStateMachine::getConstraintDefinitions() const {
    return constraintDefinitions_;
}

void NamespaceStateMachine::handleNamespaceInitialState(const Token& token, NamespaceContext& context) {
    if (token.type == TokenType::LEFT_BRACKET && token.value == "[") {
        context.state = NamespaceParseState::READING_NAMESPACE_HEADER;
    }
}

void NamespaceStateMachine::handleNamespaceHeaderState(const Token& token, NamespaceContext& context) {
    if (token.type == TokenType::IDENTIFIER && token.value == "Namespace") {
        context.state = NamespaceParseState::READING_NAMESPACE_NAME;
    }
}

void NamespaceStateMachine::handleNamespaceNameState(const Token& token, NamespaceContext& context) {
    if (token.type == TokenType::IDENTIFIER) {
        currentNamespaceName_ = token.value;
        context.currentNamespace = currentNamespaceName_;
        context.state = NamespaceParseState::READING_NAMESPACE_CONTENT;
    }
}

void NamespaceStateMachine::handleNamespaceContentState(const Token& token, NamespaceContext& context) {
    if (token.type == TokenType::LEFT_BRACE) {
        context.nestingLevel++;
        context.expectingBlock = true;
    } else if (token.type == TokenType::RIGHT_BRACE) {
        context.nestingLevel--;
        if (context.nestingLevel == 0) {
            // 命名空间结束
            NamespaceDefinition namespaceDef(currentNamespaceName_);
            namespaceDef.hasBlock = true;
            addNamespaceDefinition(namespaceDef);
            context.state = NamespaceParseState::INITIAL;
        }
    }
}

void NamespaceStateMachine::handleConstraintInitialState(const Token& token, NamespaceContext& context) {
    if (token.type == TokenType::IDENTIFIER && token.value == "except") {
        context.state = NamespaceParseState::READING_CONSTRAINT_TARGETS;
        currentConstraint_ = ConstraintDefinition();
    }
}

void NamespaceStateMachine::handleConstraintTargetsState(const Token& token, NamespaceContext& context) {
    if (token.type == TokenType::SEMICOLON) {
        // 约束结束
        currentConstraint_.targets = currentConstraintTargets_;
        addConstraintDefinition(currentConstraint_);
        currentConstraintTargets_.clear();
        context.state = NamespaceParseState::READING_NAMESPACE_CONTENT;
    } else if (token.type == TokenType::OPERATOR && token.value == ",") {
        // 约束目标分隔符
    } else if (token.type == TokenType::IDENTIFIER) {
        // 收集约束目标
        ConstraintTarget target;
        target.type = parseConstraintTargetType(token.value);
        target.name = token.value;
        currentConstraintTargets_.push_back(target);
    }
}

void NamespaceStateMachine::addNamespaceDefinition(const NamespaceDefinition& namespaceDef) {
    namespaceDefinitions_.push_back(namespaceDef);
}

void NamespaceStateMachine::addConstraintDefinition(const ConstraintDefinition& constraint) {
    constraintDefinitions_.push_back(constraint);
}

ConstraintTargetType NamespaceStateMachine::parseConstraintTargetType(const std::string& typeStr, bool hasPrefix) {
    if (typeStr == "Html" || typeStr == "@Html") {
        return ConstraintTargetType::HTML_TYPE;
    } else if (typeStr == "Custom" || typeStr == "[Custom]") {
        return ConstraintTargetType::CUSTOM_TYPE;
    } else if (typeStr == "Template" || typeStr == "[Template]") {
        return ConstraintTargetType::TEMPLATE_TYPE;
    } else {
        return ConstraintTargetType::HTML_ELEMENT;
    }
}

// ===== CHTLNamespaceParser 实现 =====

CHTLNamespaceParser::CHTLNamespaceParser() 
    : namespaceRegistry_(std::make_shared<NamespaceRegistry>())
    , constraintRegistry_(std::make_shared<ConstraintRegistry>())
    , strictMode_(true) {
}

CHTLNamespaceParser::~CHTLNamespaceParser() = default;

ParseResult CHTLNamespaceParser::parseNamespaceDeclaration(const std::string& source, const std::string& filename) {
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
        // 解析命名空间定义
        NamespaceDefinition namespaceDef = parseNamespaceDefinition(lexResult.tokens);
        
        // 注册命名空间
        if (!namespaceRegistry_->registerNamespace(namespaceDef)) {
            reportError("命名空间注册失败: " + namespaceDef.name);
        }
        
        // 创建命名空间节点
        auto namespaceNode = createNamespaceNode(namespaceDef);
        
        result.success = errors_.empty();
        result.rootNode = namespaceNode;
        result.errors = errors_;
        result.warnings = warnings_;
        
    } catch (const std::exception& e) {
        reportError("命名空间解析失败: " + std::string(e.what()));
        result.success = false;
        result.errors = errors_;
    }
    
    return result;
}

ParseResult CHTLNamespaceParser::parseConstraintDeclaration(const std::string& source, const std::string& filename) {
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
        // 解析约束定义
        ConstraintDefinition constraint = parseConstraintDefinition(lexResult.tokens);
        
        // 注册约束
        if (!constraintRegistry_->registerConstraint("global", constraint)) {
            reportError("约束注册失败");
        }
        
        // 创建约束节点
        auto constraintNode = createConstraintNode(constraint);
        
        result.success = errors_.empty();
        result.rootNode = constraintNode;
        result.errors = errors_;
        result.warnings = warnings_;
        
    } catch (const std::exception& e) {
        reportError("约束解析失败: " + std::string(e.what()));
        result.success = false;
        result.errors = errors_;
    }
    
    return result;
}

void CHTLNamespaceParser::setNamespaceRegistry(std::shared_ptr<NamespaceRegistry> registry) {
    namespaceRegistry_ = registry;
}

std::shared_ptr<NamespaceRegistry> CHTLNamespaceParser::getNamespaceRegistry() const {
    return namespaceRegistry_;
}

void CHTLNamespaceParser::setConstraintRegistry(std::shared_ptr<ConstraintRegistry> registry) {
    constraintRegistry_ = registry;
}

std::shared_ptr<ConstraintRegistry> CHTLNamespaceParser::getConstraintRegistry() const {
    return constraintRegistry_;
}

void CHTLNamespaceParser::setStrictMode(bool strict) {
    strictMode_ = strict;
}

const std::vector<ParseError>& CHTLNamespaceParser::getErrors() const {
    return errors_;
}

const std::vector<ParseWarning>& CHTLNamespaceParser::getWarnings() const {
    return warnings_;
}

NamespaceDefinition CHTLNamespaceParser::parseNamespaceDefinition(const std::vector<Token>& tokens) {
    NamespaceDefinition namespaceDef;
    size_t index = 0;
    
    // 解析 [Namespace]
    if (index >= tokens.size() || tokens[index].type != TokenType::LEFT_BRACKET) {
        throw std::runtime_error("期望 '[Namespace]' 开始");
    }
    index++; // 跳过 '['
    
    if (index >= tokens.size() || tokens[index].value != "Namespace") {
        throw std::runtime_error("期望 'Namespace' 关键字");
    }
    index++; // 跳过 'Namespace'
    
    if (index >= tokens.size() || tokens[index].type != TokenType::RIGHT_BRACKET) {
        throw std::runtime_error("期望 ']' 结束");
    }
    index++; // 跳过 ']'
    
    // 解析命名空间名称
    namespaceDef.name = parseNamespaceName(tokens, index);
    namespaceDef.fullPath = namespaceDef.name;
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 检查是否有花括号块
    if (index < tokens.size() && tokens[index].type == TokenType::LEFT_BRACE) {
        namespaceDef.hasBlock = true;
        index++; // 跳过 '{'
        
        // 解析嵌套内容
        namespaceDef.children = parseNestedNamespaces(tokens, index);
        
        // 期望右花括号
        if (index < tokens.size() && tokens[index].type == TokenType::RIGHT_BRACE) {
            index++; // 跳过 '}'
        }
    }
    
    return namespaceDef;
}

ConstraintDefinition CHTLNamespaceParser::parseConstraintDefinition(const std::vector<Token>& tokens) {
    ConstraintDefinition constraint;
    size_t index = 0;
    
    // 查找 except 关键字
    while (index < tokens.size() && 
           !((tokens[index].type == TokenType::IDENTIFIER || tokens[index].type == TokenType::KEYWORD) && tokens[index].value == "except")) {
        index++;
    }
    
    if (index >= tokens.size()) {
        throw std::runtime_error("期望 'except' 关键字");
    }
    index++; // 跳过 'except'
    
    // 解析约束类型和目标
    constraint.type = parseConstraintType(tokens, index);
    constraint.targets = parseConstraintTargets(tokens, index);
    
    return constraint;
}

std::string CHTLNamespaceParser::parseNamespaceName(const std::vector<Token>& tokens, size_t& index) {
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    if (index >= tokens.size() || tokens[index].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("期望命名空间名称");
    }
    
    std::string name = tokens[index].value;
    index++;
    
    if (!isValidNamespaceName(name)) {
        reportWarning("命名空间名称格式可能不正确: " + name);
    }
    
    return name;
}

std::vector<NamespaceDefinition> CHTLNamespaceParser::parseNestedNamespaces(const std::vector<Token>& tokens, size_t& index) {
    std::vector<NamespaceDefinition> nestedNamespaces;
    
    int braceCount = 1;
    while (index < tokens.size() && braceCount > 0) {
        const auto& token = tokens[index];
        
        if (token.type == TokenType::LEFT_BRACE) {
            braceCount++;
        } else if (token.type == TokenType::RIGHT_BRACE) {
            braceCount--;
            if (braceCount == 0) {
                break; // 命名空间块结束
            }
        }
        
        // 检查嵌套命名空间
        if (token.type == TokenType::LEFT_BRACKET && 
            index + 1 < tokens.size() && 
            tokens[index + 1].value == "Namespace") {
            // 解析嵌套命名空间
            try {
                size_t startIndex = index;
                NamespaceDefinition nested = parseNamespaceDefinition(std::vector<Token>(tokens.begin() + index, tokens.end()));
                nestedNamespaces.push_back(nested);
                
                // 跳过已解析的tokens
                while (index < tokens.size() && index - startIndex < 10) { // 简单的跳过逻辑
                    index++;
                }
            } catch (const std::exception&) {
                index++; // 跳过有问题的token
            }
        } else {
            index++;
        }
    }
    
    return nestedNamespaces;
}

ConstraintType CHTLNamespaceParser::parseConstraintType(const std::vector<Token>& tokens, size_t& index) {
    // 简单的约束类型判断，基于接下来的token
    if (index < tokens.size()) {
        const auto& token = tokens[index];
        if (token.type == TokenType::LEFT_BRACKET || token.value == "@Html") {
            return ConstraintType::TYPE_BASED;
        }
    }
    
    return ConstraintType::PRECISE;
}

std::vector<ConstraintTarget> CHTLNamespaceParser::parseConstraintTargets(const std::vector<Token>& tokens, size_t& index) {
    std::vector<ConstraintTarget> targets;
    
    while (index < tokens.size() && tokens[index].type != TokenType::SEMICOLON) {
        // 跳过空白
        while (index < tokens.size() && 
               (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
            index++;
        }
        
        if (index >= tokens.size() || tokens[index].type == TokenType::SEMICOLON) {
            break;
        }
        
        if (tokens[index].type == TokenType::OPERATOR && tokens[index].value == ",") {
            index++; // 跳过逗号
            continue;
        }
        
        // 解析一个完整的约束目标
        try {
            ConstraintTarget target = parseConstraintTarget(tokens, index);
            targets.push_back(target);
        } catch (const std::exception&) {
            index++; // 跳过有问题的token
        }
    }
    
    return targets;
}

ConstraintTarget CHTLNamespaceParser::parseConstraintTarget(const std::vector<Token>& tokens, size_t& index) {
    ConstraintTarget target;
    
    if (index >= tokens.size()) {
        throw std::runtime_error("期望约束目标");
    }
    
    // 检查是否有前缀 ([Custom], [Template])
    if (tokens[index].type == TokenType::LEFT_BRACKET) {
        index++; // 跳过 '['
        if (index < tokens.size() && tokens[index].type == TokenType::IDENTIFIER) {
            std::string prefix = tokens[index].value;
            index++; // 跳过前缀
            
            if (index < tokens.size() && tokens[index].type == TokenType::RIGHT_BRACKET) {
                index++; // 跳过 ']'
                
                // 跳过空白
                while (index < tokens.size() && 
                       (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
                    index++;
                }
                
                // 解析类型
                if (index < tokens.size() && tokens[index].type == TokenType::OPERATOR && tokens[index].value == "@") {
                    index++; // 跳过 '@'
                    if (index < tokens.size() && tokens[index].type == TokenType::IDENTIFIER) {
                        std::string type = tokens[index].value;
                        index++; // 跳过类型
                        
                        // 确定约束目标类型
                        if (prefix == "Custom") {
                            if (type == "Element") {
                                target.type = ConstraintTargetType::CUSTOM_ELEMENT;
                            } else if (type == "Style") {
                                target.type = ConstraintTargetType::CUSTOM_STYLE;
                            } else if (type == "Var") {
                                target.type = ConstraintTargetType::CUSTOM_VAR;
                            }
                        } else if (prefix == "Template") {
                            if (type == "Element") {
                                target.type = ConstraintTargetType::TEMPLATE_ELEMENT;
                            } else if (type == "Style") {
                                target.type = ConstraintTargetType::TEMPLATE_STYLE;
                            } else if (type == "Var") {
                                target.type = ConstraintTargetType::TEMPLATE_VAR;
                            }
                        }
                        
                        // 跳过空白
                        while (index < tokens.size() && 
                               (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
                            index++;
                        }
                        
                        // 解析名称（如果有）
                        if (index < tokens.size() && tokens[index].type == TokenType::IDENTIFIER) {
                            target.name = tokens[index].value;
                            index++;
                        }
                    }
                } else {
                    // 仅类型约束 ([Custom], [Template])
                    if (prefix == "Custom") {
                        target.type = ConstraintTargetType::CUSTOM_TYPE;
                    } else if (prefix == "Template") {
                        target.type = ConstraintTargetType::TEMPLATE_TYPE;
                    }
                }
            }
        }
    } else if (tokens[index].type == TokenType::OPERATOR && tokens[index].value == "@") {
        // @Html 类型约束
        index++; // 跳过 '@'
        if (index < tokens.size() && tokens[index].type == TokenType::IDENTIFIER && tokens[index].value == "Html") {
            target.type = ConstraintTargetType::HTML_TYPE;
            index++;
        }
    } else if (tokens[index].type == TokenType::IDENTIFIER) {
        // HTML元素约束
        target.type = ConstraintTargetType::HTML_ELEMENT;
        target.name = tokens[index].value;
        index++;
    }
    
    return target;
}

CHTLASTNodePtr CHTLNamespaceParser::createNamespaceNode(const NamespaceDefinition& namespaceDef) {
    auto namespaceNode = std::make_shared<NamespaceDeclarationNode>(namespaceDef.name, namespaceDef.position);
    
    // 添加嵌套命名空间作为子节点
    for (const auto& child : namespaceDef.children) {
        auto childNode = createNamespaceNode(child);
        namespaceNode->addChild(childNode);
    }
    
    return namespaceNode;
}

CHTLASTNodePtr CHTLNamespaceParser::createConstraintNode(const ConstraintDefinition& constraint) {
    // 创建约束节点，这里简化为注释节点
    auto constraintNode = std::make_shared<CommentNode>("except constraint", CHTLASTNodeType::COMMENT_GENERATOR, constraint.position);
    
    return constraintNode;
}

void CHTLNamespaceParser::reportError(const std::string& message, const Position& position) {
    errors_.emplace_back(message, position);
}

void CHTLNamespaceParser::reportWarning(const std::string& message, const Position& position) {
    warnings_.emplace_back(message, position);
}

bool CHTLNamespaceParser::isValidNamespaceName(const std::string& name) const {
    if (name.empty()) return false;
    
    std::regex nameRegex(R"(^[a-zA-Z_][a-zA-Z0-9_]*$)");
    return std::regex_match(name, nameRegex);
}

bool CHTLNamespaceParser::isValidConstraintTarget(const std::string& target) const {
    if (target.empty()) return false;
    
    // 基本的约束目标验证
    return true;
}

std::string CHTLNamespaceParser::getConstraintTypeString(ConstraintType type) const {
    switch (type) {
        case ConstraintType::PRECISE: return "precise";
        case ConstraintType::TYPE_BASED: return "type_based";
        case ConstraintType::GLOBAL: return "global";
    }
    return "precise";
}

std::string CHTLNamespaceParser::getConstraintTargetTypeString(ConstraintTargetType type) const {
    switch (type) {
        case ConstraintTargetType::HTML_ELEMENT: return "html_element";
        case ConstraintTargetType::CUSTOM_ELEMENT: return "custom_element";
        case ConstraintTargetType::CUSTOM_STYLE: return "custom_style";
        case ConstraintTargetType::CUSTOM_VAR: return "custom_var";
        case ConstraintTargetType::TEMPLATE_ELEMENT: return "template_element";
        case ConstraintTargetType::TEMPLATE_STYLE: return "template_style";
        case ConstraintTargetType::TEMPLATE_VAR: return "template_var";
        case ConstraintTargetType::HTML_TYPE: return "html_type";
        case ConstraintTargetType::CUSTOM_TYPE: return "custom_type";
        case ConstraintTargetType::TEMPLATE_TYPE: return "template_type";
    }
    return "html_element";
}

// ===== NamespaceUtils 实现 =====

bool NamespaceUtils::isNamespaceDeclaration(const std::string& source) {
    std::regex namespaceRegex(R"(\[Namespace\]\s+\w+)", std::regex_constants::icase);
    return std::regex_search(source, namespaceRegex);
}

bool NamespaceUtils::isConstraintDeclaration(const std::string& source) {
    std::regex constraintRegex(R"(except\s+.+;)", std::regex_constants::icase);
    return std::regex_search(source, constraintRegex);
}

std::string NamespaceUtils::extractNamespaceName(const std::string& source) {
    std::regex nameRegex(R"(\[Namespace\]\s+(\w+))");
    std::smatch match;
    if (std::regex_search(source, match, nameRegex)) {
        return match[1].str();
    }
    return "";
}

std::vector<std::string> NamespaceUtils::extractConstraintTargets(const std::string& source) {
    std::vector<std::string> targets;
    
    std::regex constraintRegex(R"(except\s+(.+);)");
    std::smatch match;
    if (std::regex_search(source, match, constraintRegex)) {
        std::string targetsStr = match[1].str();
        
        std::istringstream stream(targetsStr);
        std::string target;
        while (std::getline(stream, target, ',')) {
            target.erase(0, target.find_first_not_of(" \t"));
            target.erase(target.find_last_not_of(" \t") + 1);
            if (!target.empty()) {
                targets.push_back(target);
            }
        }
    }
    
    return targets;
}

bool NamespaceUtils::validateNamespaceSyntax(const std::string& source) {
    return isNamespaceDeclaration(source);
}

bool NamespaceUtils::validateConstraintSyntax(const std::string& source) {
    return isConstraintDeclaration(source);
}

std::string NamespaceUtils::normalizeNamespacePath(const std::string& path) {
    // 简单的路径标准化
    std::string normalized = path;
    
    // 移除多余的点
    normalized = std::regex_replace(normalized, std::regex(R"(\.+)"), ".");
    
    // 移除开头和结尾的点
    if (!normalized.empty() && normalized.front() == '.') {
        normalized = normalized.substr(1);
    }
    if (!normalized.empty() && normalized.back() == '.') {
        normalized = normalized.substr(0, normalized.length() - 1);
    }
    
    return normalized;
}

std::vector<std::string> NamespaceUtils::splitNamespacePath(const std::string& path) {
    std::vector<std::string> parts;
    std::istringstream stream(path);
    std::string part;
    
    while (std::getline(stream, part, '.')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }
    
    return parts;
}

std::string NamespaceUtils::joinNamespacePath(const std::vector<std::string>& pathParts) {
    std::string result;
    for (size_t i = 0; i < pathParts.size(); ++i) {
        if (i > 0) result += ".";
        result += pathParts[i];
    }
    return result;
}

bool NamespaceUtils::isValidNamespaceName(const std::string& name) {
    if (name.empty()) return false;
    
    std::regex nameRegex(R"(^[a-zA-Z_][a-zA-Z0-9_]*$)");
    return std::regex_match(name, nameRegex);
}

bool NamespaceUtils::isValidConstraintTarget(const std::string& target) {
    if (target.empty()) return false;
    
    // 基本的约束目标验证
    std::regex targetRegex(R"(^[\w\[\]@\s]+$)");
    return std::regex_match(target, targetRegex);
}

ConstraintTargetType NamespaceUtils::parseConstraintTargetType(const std::string& target) {
    if (target.find("[Custom]") != std::string::npos) {
        if (target.find("@Element") != std::string::npos) {
            return ConstraintTargetType::CUSTOM_ELEMENT;
        } else if (target.find("@Style") != std::string::npos) {
            return ConstraintTargetType::CUSTOM_STYLE;
        } else if (target.find("@Var") != std::string::npos) {
            return ConstraintTargetType::CUSTOM_VAR;
        } else {
            return ConstraintTargetType::CUSTOM_TYPE;
        }
    } else if (target.find("[Template]") != std::string::npos) {
        if (target.find("@Element") != std::string::npos) {
            return ConstraintTargetType::TEMPLATE_ELEMENT;
        } else if (target.find("@Style") != std::string::npos) {
            return ConstraintTargetType::TEMPLATE_STYLE;
        } else if (target.find("@Var") != std::string::npos) {
            return ConstraintTargetType::TEMPLATE_VAR;
        } else {
            return ConstraintTargetType::TEMPLATE_TYPE;
        }
    } else if (target.find("@Html") != std::string::npos) {
        return ConstraintTargetType::HTML_TYPE;
    } else {
        return ConstraintTargetType::HTML_ELEMENT;
    }
}

std::string NamespaceUtils::formatConstraintTarget(const ConstraintTarget& target) {
    switch (target.type) {
        case ConstraintTargetType::HTML_ELEMENT:
            return target.name;
        case ConstraintTargetType::CUSTOM_ELEMENT:
            return "[Custom] @Element " + target.name;
        case ConstraintTargetType::CUSTOM_STYLE:
            return "[Custom] @Style " + target.name;
        case ConstraintTargetType::CUSTOM_VAR:
            return "[Custom] @Var " + target.name;
        case ConstraintTargetType::TEMPLATE_ELEMENT:
            return "[Template] @Element " + target.name;
        case ConstraintTargetType::TEMPLATE_STYLE:
            return "[Template] @Style " + target.name;
        case ConstraintTargetType::TEMPLATE_VAR:
            return "[Template] @Var " + target.name;
        case ConstraintTargetType::HTML_TYPE:
            return "@Html";
        case ConstraintTargetType::CUSTOM_TYPE:
            return "[Custom]";
        case ConstraintTargetType::TEMPLATE_TYPE:
            return "[Template]";
    }
    return target.name;
}

} // namespace chtl