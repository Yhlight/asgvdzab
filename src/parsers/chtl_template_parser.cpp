#include "parsers/chtl_template_parser.hpp"
#include "parsers/chtl_basic_parser.hpp"
#include <regex>
#include <algorithm>
#include <sstream>

namespace chtl {

// ===== TemplateRegistry 实现 =====

TemplateRegistry::TemplateRegistry() = default;

TemplateRegistry::~TemplateRegistry() = default;

bool TemplateRegistry::registerTemplate(const TemplateDefinition& templateDef) {
    std::string key = generateTemplateKey(templateDef.type, templateDef.name);
    
    // 检查是否已存在
    if (templates_.find(key) != templates_.end()) {
        return false; // 模板已存在
    }
    
    templates_[key] = templateDef;
    return true;
}

const TemplateDefinition* TemplateRegistry::findTemplate(TemplateType type, const std::string& name) const {
    std::string key = generateTemplateKey(type, name);
    auto it = templates_.find(key);
    return (it != templates_.end()) ? &it->second : nullptr;
}

const std::unordered_map<std::string, TemplateDefinition>& TemplateRegistry::getAllTemplates() const {
    return templates_;
}

void TemplateRegistry::clear() {
    templates_.clear();
}

bool TemplateRegistry::validateTemplateReference(const TemplateReference& ref) const {
    return findTemplate(ref.type, ref.templateName) != nullptr;
}

std::string TemplateRegistry::generateTemplateKey(TemplateType type, const std::string& name) const {
    std::string prefix;
    switch (type) {
        case TemplateType::STYLE_TEMPLATE:
            prefix = "style:";
            break;
        case TemplateType::ELEMENT_TEMPLATE:
            prefix = "element:";
            break;
        case TemplateType::VAR_TEMPLATE:
            prefix = "var:";
            break;
    }
    return prefix + name;
}

// ===== CHTLTemplateParser 实现 =====

CHTLTemplateParser::CHTLTemplateParser() 
    : registry_(std::make_shared<TemplateRegistry>())
    , currentState_(TemplateParseState::INITIAL)
    , strictMode_(true) {
}

CHTLTemplateParser::~CHTLTemplateParser() = default;

ParseResult CHTLTemplateParser::parseTemplateDeclaration(const std::string& source, const std::string& filename) {
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
        // 解析模板定义
        TemplateDefinition templateDef = parseTemplateDefinition(lexResult.tokens);
        
        // 应用继承
        applyInheritance(templateDef);
        
        // 注册模板
        if (!registry_->registerTemplate(templateDef)) {
            reportError("模板名称重复: " + templateDef.name, templateDef.position);
        }
        
        // 创建模板声明节点
        std::string typeStr;
        switch (templateDef.type) {
            case TemplateType::STYLE_TEMPLATE: typeStr = "@Style"; break;
            case TemplateType::ELEMENT_TEMPLATE: typeStr = "@Element"; break;
            case TemplateType::VAR_TEMPLATE: typeStr = "@Var"; break;
        }
        auto templateNode = std::make_shared<TemplateDeclarationNode>(
            typeStr, templateDef.name, templateDef.position
        );
        
        result.success = errors_.empty();
        result.rootNode = templateNode;
        result.errors = errors_;
        result.warnings = warnings_;
        
    } catch (const std::exception& e) {
        reportError("模板解析失败: " + std::string(e.what()));
        result.success = false;
        result.errors = errors_;
    }
    
    return result;
}

TemplateReference CHTLTemplateParser::parseTemplateUsage(const std::string& usage, const std::string& context) {
    errors_.clear();
    warnings_.clear();
    
    // 解析模板使用语法: @Style TemplateName; 或 inherit @Style TemplateName;
    std::regex usageRegex(R"((inherit\s+)?@(Style|Element|Var)\s+(\w+)\s*;?)");
    std::smatch match;
    
    if (!std::regex_match(usage, match, usageRegex)) {
        reportError("无效的模板使用语法: " + usage);
        return TemplateReference(TemplateType::STYLE_TEMPLATE, "");
    }
    
    // 解析继承类型
    InheritanceType inheritType = match[1].matched ? 
        InheritanceType::EXPLICIT : InheritanceType::COMPOSITION;
    
    // 解析模板类型
    std::string typeStr = match[2].str();
    TemplateType type = parseTemplateType(typeStr);
    
    // 解析模板名称
    std::string templateName = match[3].str();
    
    TemplateReference ref(type, templateName);
    ref.inheritanceType = inheritType;
    
    // 验证模板引用
    if (strictMode_ && !registry_->validateTemplateReference(ref)) {
        reportError("未定义的模板: " + templateName);
    }
    
    return ref;
}

TemplateReference CHTLTemplateParser::parseVariableTemplateUsage(const std::string& expression) {
    errors_.clear();
    warnings_.clear();
    
    // 解析变量模板使用语法: TemplateName(variableName)
    std::regex varRegex(R"((\w+)\s*\(\s*(\w+)\s*\))");
    std::smatch match;
    
    if (!std::regex_match(expression, match, varRegex)) {
        reportError("无效的变量模板使用语法: " + expression);
        return TemplateReference(TemplateType::VAR_TEMPLATE, "");
    }
    
    std::string templateName = match[1].str();
    std::string variableName = match[2].str();
    
    TemplateReference ref(TemplateType::VAR_TEMPLATE, templateName);
    ref.arguments.push_back(variableName);
    
    // 验证模板引用
    if (strictMode_ && !registry_->validateTemplateReference(ref)) {
        reportError("未定义的变量模板: " + templateName);
    }
    
    return ref;
}

CHTLASTNodePtr CHTLTemplateParser::expandTemplateReference(const TemplateReference& ref) {
    const TemplateDefinition* templateDef = registry_->findTemplate(ref.type, ref.templateName);
    if (!templateDef) {
        reportError("未找到模板定义: " + ref.templateName);
        return nullptr;
    }
    
    switch (ref.type) {
        case TemplateType::STYLE_TEMPLATE:
            return expandStyleTemplate(*templateDef, ref);
        case TemplateType::ELEMENT_TEMPLATE:
            return expandElementTemplate(*templateDef, ref);
        case TemplateType::VAR_TEMPLATE:
            return expandVariableTemplate(*templateDef, ref);
    }
    
    return nullptr;
}

void CHTLTemplateParser::setTemplateRegistry(std::shared_ptr<TemplateRegistry> registry) {
    registry_ = registry;
}

std::shared_ptr<TemplateRegistry> CHTLTemplateParser::getTemplateRegistry() const {
    return registry_;
}

void CHTLTemplateParser::setStrictMode(bool strict) {
    strictMode_ = strict;
}

const std::vector<ParseError>& CHTLTemplateParser::getErrors() const {
    return errors_;
}

const std::vector<ParseWarning>& CHTLTemplateParser::getWarnings() const {
    return warnings_;
}

// ===== 核心解析方法 =====

TemplateDefinition CHTLTemplateParser::parseTemplateDefinition(const std::vector<Token>& tokens) {
    size_t index = 0;
    
    // 解析 [Template]
    if (index >= tokens.size() || tokens[index].type != TokenType::LEFT_BRACKET) {
        throw std::runtime_error("期望 '[Template]' 开始");
    }
    index++; // 跳过 '['
    
    if (index >= tokens.size() || tokens[index].value != "Template") {
        throw std::runtime_error("期望 'Template' 关键字");
    }
    index++; // 跳过 'Template'
    
    if (index >= tokens.size() || tokens[index].type != TokenType::RIGHT_BRACKET) {
        throw std::runtime_error("期望 ']' 结束");
    }
    index++; // 跳过 ']'
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 解析模板类型 @Style, @Element, @Var
    if (index >= tokens.size() || tokens[index].type != TokenType::OPERATOR || tokens[index].value != "@") {
        throw std::runtime_error("期望模板类型标识符 '@'");
    }
    index++; // 跳过 '@'
    
    if (index >= tokens.size() || tokens[index].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("期望模板类型名称");
    }
    
    TemplateType type = parseTemplateType(tokens[index].value);
    Position pos = tokens[index].position;
    index++; // 跳过类型名称
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 解析模板名称
    std::string templateName = parseTemplateName(tokens, index);
    
    // 创建模板定义
    TemplateDefinition templateDef(type, templateName, pos);
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 解析模板内容
    templateDef.content = parseTemplateContent(tokens, index, type);
    
    return templateDef;
}

TemplateType CHTLTemplateParser::parseTemplateType(const std::string& typeStr) {
    if (typeStr == "Style") {
        return TemplateType::STYLE_TEMPLATE;
    } else if (typeStr == "Element") {
        return TemplateType::ELEMENT_TEMPLATE;
    } else if (typeStr == "Var") {
        return TemplateType::VAR_TEMPLATE;
    } else {
        throw std::runtime_error("未知的模板类型: " + typeStr);
    }
}

std::string CHTLTemplateParser::parseTemplateName(const std::vector<Token>& tokens, size_t& index) {
    if (index >= tokens.size() || tokens[index].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("期望模板名称");
    }
    
    std::string name = tokens[index].value;
    index++;
    
    if (!isValidTemplateName(name)) {
        reportWarning("模板名称不符合命名规范: " + name);
    }
    
    return name;
}

CHTLASTNodePtr CHTLTemplateParser::parseTemplateContent(const std::vector<Token>& tokens, size_t& index, TemplateType type) {
    // 期望 '{'
    if (index >= tokens.size() || tokens[index].type != TokenType::LEFT_BRACE) {
        throw std::runtime_error("期望 '{' 开始模板内容");
    }
    index++; // 跳过 '{'
    
    // 收集模板内容直到 '}'
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
        throw std::runtime_error("未闭合的模板内容块");
    }
    
    index++; // 跳过最后的 '}'
    
    // 根据模板类型解析内容
    if (type == TemplateType::VAR_TEMPLATE) {
        // 变量模板需要特殊解析
        return parseVariableTemplateContent(content);
    } else {
        // 样式和元素模板使用基础解析器
        CHTLBasicParser basicParser;
        auto parseResult = basicParser.parse(content, "template_content");
        
        if (!parseResult.success) {
            throw std::runtime_error("模板内容解析失败");
        }
        
        return parseResult.rootNode;
    }
}

CHTLASTNodePtr CHTLTemplateParser::parseVariableTemplateContent(const std::string& content) {
    // 解析变量定义：variableName: "value";
    auto containerNode = std::make_shared<ProgramNode>();
    
    std::istringstream stream(content);
    std::string line;
    
    while (std::getline(stream, line)) {
        // 跳过空行和注释
        line.erase(0, line.find_first_not_of(" \t"));
        if (line.empty() || line.substr(0, 2) == "//") {
            continue;
        }
        
        // 解析变量定义：name: "value";
        std::regex varRegex(R"(\s*(\w+)\s*:\s*\"([^\"]*)\"\s*;?)");
        std::smatch match;
        
        if (std::regex_match(line, match, varRegex)) {
            std::string varName = match[1].str();
            std::string varValue = match[2].str();
            
            // 创建变量节点 (使用字符串字面量节点表示)
            auto varNode = std::make_shared<StringLiteralNode>(
                varName + ":" + varValue, LiteralType::DOUBLE_QUOTED
            );
            containerNode->addChild(varNode);
        }
    }
    
    return containerNode;
}

// ===== 模板展开方法 =====

CHTLASTNodePtr CHTLTemplateParser::expandStyleTemplate(const TemplateDefinition& templateDef, const TemplateReference& ref) {
    if (!templateDef.content) {
        reportError("样式模板内容为空: " + templateDef.name);
        return nullptr;
    }
    
    // 克隆模板内容
    auto expandedContent = templateDef.content->clone();
    
    return expandedContent;
}

CHTLASTNodePtr CHTLTemplateParser::expandElementTemplate(const TemplateDefinition& templateDef, const TemplateReference& ref) {
    if (!templateDef.content) {
        reportError("元素模板内容为空: " + templateDef.name);
        return nullptr;
    }
    
    // 克隆模板内容
    auto expandedContent = templateDef.content->clone();
    
    return expandedContent;
}

CHTLASTNodePtr CHTLTemplateParser::expandVariableTemplate(const TemplateDefinition& templateDef, const TemplateReference& ref) {
    if (ref.arguments.empty()) {
        reportError("变量模板缺少参数: " + templateDef.name);
        return nullptr;
    }
    
    std::string variableName = ref.arguments[0];
    std::string variableValue = resolveVariableValue(variableName, templateDef.variables);
    
    if (variableValue.empty()) {
        reportError("未找到变量: " + variableName + " 在模板 " + templateDef.name + " 中");
        return nullptr;
    }
    
    // 创建变量值节点
    auto valueNode = std::make_shared<StringLiteralNode>(variableValue, LiteralType::UNQUOTED);
    
    return valueNode;
}

// ===== 继承处理方法 =====

void CHTLTemplateParser::applyInheritance(TemplateDefinition& templateDef) {
    for (const auto& inheritRef : templateDef.inheritance) {
        const TemplateDefinition* sourceTemplate = registry_->findTemplate(inheritRef.type, inheritRef.templateName);
        if (!sourceTemplate) {
            reportError("未找到被继承的模板: " + inheritRef.templateName);
            continue;
        }
        
        // 根据类型进行继承合并
        switch (inheritRef.type) {
            case TemplateType::STYLE_TEMPLATE:
                mergeStyleTemplate(templateDef, *sourceTemplate);
                break;
            case TemplateType::ELEMENT_TEMPLATE:
                mergeElementTemplate(templateDef, *sourceTemplate);
                break;
            case TemplateType::VAR_TEMPLATE:
                mergeVariableTemplate(templateDef, *sourceTemplate);
                break;
        }
    }
}

void CHTLTemplateParser::mergeStyleTemplate(TemplateDefinition& target, const TemplateDefinition& source) {
    // 简单的内容合并 - 在实际实现中需要更复杂的CSS属性合并逻辑
    if (source.content && target.content) {
        // 将源模板的子节点添加到目标模板
        for (const auto& child : source.content->children) {
            target.content->addChild(child->clone());
        }
    } else if (source.content && !target.content) {
        target.content = source.content->clone();
    }
}

void CHTLTemplateParser::mergeElementTemplate(TemplateDefinition& target, const TemplateDefinition& source) {
    // 元素模板合并
    if (source.content && target.content) {
        for (const auto& child : source.content->children) {
            target.content->addChild(child->clone());
        }
    } else if (source.content && !target.content) {
        target.content = source.content->clone();
    }
}

void CHTLTemplateParser::mergeVariableTemplate(TemplateDefinition& target, const TemplateDefinition& source) {
    // 变量模板合并 - 将源模板的变量添加到目标模板
    for (const auto& sourceVar : source.variables) {
        // 检查是否已存在同名变量
        bool exists = false;
        for (const auto& targetVar : target.variables) {
            if (targetVar.name == sourceVar.name) {
                exists = true;
                break;
            }
        }
        
        // 如果不存在，则添加
        if (!exists) {
            target.variables.push_back(sourceVar);
        }
    }
}

// ===== 工具方法 =====

void CHTLTemplateParser::reportError(const std::string& message, const Position& position) {
    errors_.emplace_back(message, position);
}

void CHTLTemplateParser::reportWarning(const std::string& message, const Position& position) {
    warnings_.emplace_back(message, position);
}

bool CHTLTemplateParser::isValidTemplateName(const std::string& name) const {
    // 模板名称验证：只允许字母、数字和下划线，且以字母开头
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

bool CHTLTemplateParser::isValidVariableName(const std::string& name) const {
    return isValidTemplateName(name); // 使用相同的验证规则
}

std::string CHTLTemplateParser::resolveVariableValue(const std::string& varName, const std::vector<TemplateVariable>& variables) const {
    for (const auto& var : variables) {
        if (var.name == varName) {
            return var.value;
        }
    }
    return ""; // 未找到变量
}

// ===== TemplateUtils 实现 =====

bool TemplateUtils::isTemplateDeclaration(const std::string& source) {
    std::regex templateRegex(R"(\[Template\]\s*@(Style|Element|Var)\s+\w+\s*\{)");
    return std::regex_search(source, templateRegex);
}

bool TemplateUtils::isTemplateUsage(const std::string& source) {
    std::regex usageRegex(R"((inherit\s+)?@(Style|Element|Var)\s+\w+\s*;?)");
    return std::regex_search(source, usageRegex);
}

bool TemplateUtils::isVariableTemplateUsage(const std::string& source) {
    std::regex varRegex(R"(\w+\s*\(\s*\w+\s*\))");
    return std::regex_search(source, varRegex);
}

std::string TemplateUtils::extractTemplateTypeString(const std::string& source) {
    std::regex typeRegex(R"(@(Style|Element|Var))");
    std::smatch match;
    if (std::regex_search(source, match, typeRegex)) {
        return match[1].str();
    }
    return "";
}

std::string TemplateUtils::extractTemplateName(const std::string& source) {
    std::regex nameRegex(R"(@(?:Style|Element|Var)\s+(\w+))");
    std::smatch match;
    if (std::regex_search(source, match, nameRegex)) {
        return match[1].str();
    }
    return "";
}

bool TemplateUtils::validateTemplateSyntax(const std::string& source) {
    return isTemplateDeclaration(source) || isTemplateUsage(source) || isVariableTemplateUsage(source);
}

} // namespace chtl