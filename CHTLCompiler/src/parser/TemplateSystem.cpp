#include "parser/TemplateSystem.h"
#include "ast/ASTNode.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>
#include <regex>

namespace chtl {

// 实现类
class TemplateSystem::Impl {
public:
    TemplateSystemConfig config;
    std::unordered_map<std::string, TemplateInfo> templates;
    std::vector<std::string> errors;
    bool debug_mode;
    
    // 模板依赖图
    std::unordered_map<std::string, std::vector<std::string>> dependency_graph;
    
    Impl(const TemplateSystemConfig& cfg) : config(cfg), debug_mode(false) {}
    
    ~Impl() = default;
    
    // 内部方法实现
    bool isTemplateDeclaration(const std::vector<Token>& tokens, size_t position);
    bool isTemplateUsage(const std::vector<Token>& tokens, size_t position);
    TemplateType detectTemplateType(const std::vector<Token>& tokens, size_t position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    
    // 模板验证辅助方法
    bool isValidTemplateName(const std::string& name);
    bool isValidTemplateContent(const std::string& content);
    bool hasCircularDependency(const std::string& template_name, std::unordered_set<std::string>& visited);
    
    // 模板展开辅助方法
    std::string substituteTemplateVariables(const std::string& content, const std::vector<std::string>& parameters);
    std::string resolveTemplateReferences(const std::string& content);
    std::string mergeTemplateInheritance(const TemplateInfo& template_info);
    
    // 辅助方法
    void logDebug(const std::string& message);
    void addDependency(const std::string& template_name, const std::string& dependency);
    std::string extractTemplateContent(const std::vector<Token>& tokens, size_t& position);
};

// TemplateSystem实现
TemplateSystem::TemplateSystem(const TemplateSystemConfig& config) 
    : pImpl(std::make_unique<Impl>(config)) {}

TemplateSystem::~TemplateSystem() = default;

std::shared_ptr<ASTNode> TemplateSystem::parseTemplateDeclaration(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isTemplateDeclaration(tokens, position)) {
        addError("不是模板声明");
        return nullptr;
    }
    
    // 跳过[Template]
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检测模板类型
    TemplateType template_type = pImpl->detectTemplateType(tokens, position);
    
    // 根据类型解析模板
    TemplateInfo template_info;
    switch (template_type) {
        case TemplateType::STYLE_TEMPLATE:
            template_info = parseStyleTemplate(tokens, position);
            break;
        case TemplateType::ELEMENT_TEMPLATE:
            template_info = parseElementTemplate(tokens, position);
            break;
        case TemplateType::VAR_TEMPLATE:
            template_info = parseVarTemplate(tokens, position);
            break;
        default:
            addError("未知的模板类型");
            return nullptr;
    }
    
    // 验证模板
    if (!validateTemplate(template_info)) {
        return nullptr;
    }
    
    // 添加模板到系统
    addTemplate(template_info);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("模板声明解析完成: " + template_info.name + " (类型: " + std::to_string(static_cast<int>(template_info.type)) + ")");
    }
    
    // 创建模板声明节点（这里需要创建具体的AST节点类型）
    // 暂时返回nullptr，等待具体的AST节点类型定义
    return nullptr; // TODO: 创建具体的模板声明AST节点
}

TemplateInfo TemplateSystem::parseStyleTemplate(const std::vector<Token>& tokens, size_t& position) {
    TemplateInfo template_info;
    template_info.type = TemplateType::STYLE_TEMPLATE;
    
    // 跳过@Style
    if (position < tokens.size() && tokens[position].value == "@Style") {
        position++;
    } else {
        addError("样式组模板缺少@Style关键字");
        return template_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析模板名称
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        template_info.name = tokens[position].value;
        template_info.line = tokens[position].line;
        template_info.column = tokens[position].column;
        position++;
    } else {
        addError("样式组模板缺少名称");
        return template_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("样式组模板缺少左大括号");
        return template_info;
    }
    position++;
    
    // 解析模板内容
    template_info.content = pImpl->extractTemplateContent(tokens, position);
    
    // 检查右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("样式组模板缺少右大括号");
        return template_info;
    }
    position++;
    
    return template_info;
}

TemplateInfo TemplateSystem::parseElementTemplate(const std::vector<Token>& tokens, size_t& position) {
    TemplateInfo template_info;
    template_info.type = TemplateType::ELEMENT_TEMPLATE;
    
    // 跳过@Element
    if (position < tokens.size() && tokens[position].value == "@Element") {
        position++;
    } else {
        addError("元素模板缺少@Element关键字");
        return template_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析模板名称
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        template_info.name = tokens[position].value;
        template_info.line = tokens[position].line;
        template_info.column = tokens[position].column;
        position++;
    } else {
        addError("元素模板缺少名称");
        return template_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("元素模板缺少左大括号");
        return template_info;
    }
    position++;
    
    // 解析模板内容
    template_info.content = pImpl->extractTemplateContent(tokens, position);
    
    // 检查右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("元素模板缺少右大括号");
        return template_info;
    }
    position++;
    
    return template_info;
}

TemplateInfo TemplateSystem::parseVarTemplate(const std::vector<Token>& tokens, size_t& position) {
    TemplateInfo template_info;
    template_info.type = TemplateType::VAR_TEMPLATE;
    
    // 跳过@Var
    if (position < tokens.size() && tokens[position].value == "@Var") {
        position++;
    } else {
        addError("变量组模板缺少@Var关键字");
        return template_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析模板名称
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        template_info.name = tokens[position].value;
        template_info.line = tokens[position].line;
        template_info.column = tokens[position].column;
        position++;
    } else {
        addError("变量组模板缺少名称");
        return template_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("变量组模板缺少左大括号");
        return template_info;
    }
    position++;
    
    // 解析模板内容
    template_info.content = pImpl->extractTemplateContent(tokens, position);
    
    // 检查右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("变量组模板缺少右大括号");
        return template_info;
    }
    position++;
    
    return template_info;
}

std::shared_ptr<ASTNode> TemplateSystem::parseTemplateUsage(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isTemplateUsage(tokens, position)) {
        addError("不是模板使用");
        return nullptr;
    }
    
    // 解析模板名称
    std::string template_name;
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        template_name = tokens[position].value;
        position++;
    } else {
        addError("模板使用缺少名称");
        return nullptr;
    }
    
    // 检查模板是否存在
    if (!hasTemplate(template_name)) {
        addError("模板不存在: " + template_name);
        return nullptr;
    }
    
    // 解析模板参数（如果有）
    std::vector<std::string> parameters;
    if (position < tokens.size() && tokens[position].value == "(") {
        position++;
        parameters = parseTemplateParameters(tokens, position);
        
        if (position >= tokens.size() || tokens[position].value != ")") {
            addError("模板参数缺少右括号");
            return nullptr;
        }
        position++;
    }
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("模板使用解析完成: " + template_name + " (参数数量: " + std::to_string(parameters.size()) + ")");
    }
    
    // 创建模板使用节点（这里需要创建具体的AST节点类型）
    // 暂时返回nullptr，等待具体的AST节点类型定义
    return nullptr; // TODO: 创建具体的模板使用AST节点
}

std::vector<std::string> TemplateSystem::parseTemplateParameters(const std::vector<Token>& tokens, size_t& position) {
    std::vector<std::string> parameters;
    
    while (position < tokens.size() && tokens[position].value != ")") {
        pImpl->skipWhitespace(tokens, position);
        
        if (position >= tokens.size()) {
            break;
        }
        
        // 解析参数
        std::string parameter;
        while (position < tokens.size() && tokens[position].value != "," && tokens[position].value != ")") {
            parameter += tokens[position].value;
            position++;
        }
        
        // 移除首尾空白字符
        parameter.erase(0, parameter.find_first_not_of(" \t\n\r"));
        parameter.erase(parameter.find_last_not_of(" \t\n\r") + 1);
        
        if (!parameter.empty()) {
            parameters.push_back(parameter);
        }
        
        // 跳过逗号
        if (position < tokens.size() && tokens[position].value == ",") {
            position++;
        }
    }
    
    return parameters;
}

bool TemplateSystem::parseTemplateInheritance(const std::vector<Token>& tokens, size_t& position, TemplateInfo& template_info) {
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查是否是组合式继承
    if (position < tokens.size() && tokens[position].value == "@Style") {
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
            std::string parent_name = tokens[position].value;
            template_info.dependencies.push_back(parent_name);
            template_info.inheritance_type = InheritanceType::COMBINATION;
            pImpl->addDependency(template_info.name, parent_name);
            position++;
            return true;
        }
    }
    
    // 检查是否是显式继承
    if (position < tokens.size() && tokens[position].value == "inherit") {
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        if (position < tokens.size() && tokens[position].value == "@Style") {
            position++;
            pImpl->skipWhitespace(tokens, position);
            
            if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
                std::string parent_name = tokens[position].value;
                template_info.parent_template = parent_name;
                template_info.inheritance_type = InheritanceType::EXPLICIT;
                pImpl->addDependency(template_info.name, parent_name);
                position++;
                return true;
            }
        }
    }
    
    return false;
}

std::vector<std::string> TemplateSystem::resolveTemplateDependencies(const std::string& template_name) {
    auto it = pImpl->dependency_graph.find(template_name);
    if (it != pImpl->dependency_graph.end()) {
        return it->second;
    }
    return {};
}

bool TemplateSystem::validateInheritanceChain(const std::string& template_name, size_t depth) {
    if (depth > pImpl->config.max_inheritance_depth) {
        addError("模板继承深度超过限制: " + template_name);
        return false;
    }
    
    std::unordered_set<std::string> visited;
    return !pImpl->hasCircularDependency(template_name, visited);
}

void TemplateSystem::addTemplate(const TemplateInfo& template_info) {
    if (template_info.name.empty()) {
        addError("模板名称不能为空");
        return;
    }
    
    pImpl->templates[template_info.name] = template_info;
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("添加模板: " + template_info.name);
    }
}

void TemplateSystem::removeTemplate(const std::string& name) {
    pImpl->templates.erase(name);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("删除模板: " + name);
    }
}

TemplateInfo* TemplateSystem::getTemplate(const std::string& name) {
    auto it = pImpl->templates.find(name);
    if (it != pImpl->templates.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<TemplateInfo> TemplateSystem::getAllTemplates() const {
    std::vector<TemplateInfo> result;
    for (const auto& [name, info] : pImpl->templates) {
        result.push_back(info);
    }
    return result;
}

std::vector<TemplateInfo> TemplateSystem::getTemplatesByType(TemplateType type) const {
    std::vector<TemplateInfo> result;
    for (const auto& [name, info] : pImpl->templates) {
        if (info.type == type) {
            result.push_back(info);
        }
    }
    return result;
}

bool TemplateSystem::hasTemplate(const std::string& name) const {
    return pImpl->templates.find(name) != pImpl->templates.end();
}

bool TemplateSystem::validateTemplate(const TemplateInfo& template_info) {
    // 验证模板名称
    if (!validateTemplateName(template_info.name)) {
        addError("无效的模板名称: " + template_info.name);
        return false;
    }
    
    // 验证模板内容
    if (!validateTemplateContent(template_info.content, template_info.type)) {
        addError("无效的模板内容: " + template_info.name);
        return false;
    }
    
    // 验证继承链
    if (!validateInheritanceChain(template_info.name)) {
        return false;
    }
    
    return true;
}

std::vector<std::string> TemplateSystem::validateTemplateList(const std::vector<TemplateInfo>& templates) {
    std::vector<std::string> errors;
    
    for (const auto& template_info : templates) {
        if (!validateTemplate(template_info)) {
            auto template_errors = getErrors();
            errors.insert(errors.end(), template_errors.begin(), template_errors.end());
        }
    }
    
    return errors;
}

bool TemplateSystem::validateTemplateName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 检查长度限制
    if (name.length() > pImpl->config.max_template_name_length) {
        return false;
    }
    
    // 检查是否只包含字母、数字、下划线，且不能以数字开头
    if (std::isdigit(name[0])) {
        return false;
    }
    
    for (char c : name) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool TemplateSystem::validateTemplateContent(const std::string& content, TemplateType type) {
    if (content.empty()) {
        return false;
    }
    
    // 检查长度限制
    if (content.length() > pImpl->config.max_template_content_length) {
        return false;
    }
    
    // 根据模板类型进行特定验证
    switch (type) {
        case TemplateType::STYLE_TEMPLATE:
            // 样式组模板应该包含CSS属性
            return content.find(":") != std::string::npos;
        case TemplateType::ELEMENT_TEMPLATE:
            // 元素模板应该包含HTML元素
            return content.find("<") != std::string::npos || content.find("{") != std::string::npos;
        case TemplateType::VAR_TEMPLATE:
            // 变量组模板应该包含变量定义
            return content.find(":") != std::string::npos;
        default:
            return true;
    }
}

std::string TemplateSystem::expandStyleTemplate(const std::string& template_name, const std::vector<std::string>& parameters) {
    auto* template_info = getTemplate(template_name);
    if (!template_info || template_info->type != TemplateType::STYLE_TEMPLATE) {
        addError("样式组模板不存在: " + template_name);
        return "";
    }
    
    std::string expanded_content = template_info->content;
    
    // 合并继承的模板
    if (!template_info->dependencies.empty()) {
        expanded_content = pImpl->mergeTemplateInheritance(*template_info);
    }
    
    // 替换模板变量
    if (!parameters.empty()) {
        expanded_content = pImpl->substituteTemplateVariables(expanded_content, parameters);
    }
    
    return expanded_content;
}

std::string TemplateSystem::expandElementTemplate(const std::string& template_name, const std::vector<std::string>& parameters) {
    auto* template_info = getTemplate(template_name);
    if (!template_info || template_info->type != TemplateType::ELEMENT_TEMPLATE) {
        addError("元素模板不存在: " + template_name);
        return "";
    }
    
    std::string expanded_content = template_info->content;
    
    // 合并继承的模板
    if (!template_info->dependencies.empty()) {
        expanded_content = pImpl->mergeTemplateInheritance(*template_info);
    }
    
    // 替换模板变量
    if (!parameters.empty()) {
        expanded_content = pImpl->substituteTemplateVariables(expanded_content, parameters);
    }
    
    return expanded_content;
}

std::string TemplateSystem::expandVarTemplate(const std::string& template_name, const std::string& variable_name) {
    auto* template_info = getTemplate(template_name);
    if (!template_info || template_info->type != TemplateType::VAR_TEMPLATE) {
        addError("变量组模板不存在: " + template_name);
        return "";
    }
    
    // 在变量组模板中查找指定的变量
    std::istringstream iss(template_info->content);
    std::string line;
    
    while (std::getline(iss, line)) {
        size_t colon_pos = line.find(":");
        if (colon_pos != std::string::npos) {
            std::string var_name = line.substr(0, colon_pos);
            // 移除首尾空白字符
            var_name.erase(0, var_name.find_first_not_of(" \t\n\r"));
            var_name.erase(var_name.find_last_not_of(" \t\n\r") + 1);
            
            if (var_name == variable_name) {
                std::string var_value = line.substr(colon_pos + 1);
                // 移除首尾空白字符和引号
                var_value.erase(0, var_value.find_first_not_of(" \t\n\r\"'"));
                var_value.erase(var_value.find_last_not_of(" \t\n\r\"'") + 1);
                return var_value;
            }
        }
    }
    
    addError("变量不存在: " + variable_name + " 在模板 " + template_name + " 中");
    return "";
}

void TemplateSystem::setConfig(const TemplateSystemConfig& config) {
    pImpl->config = config;
}

TemplateSystemConfig TemplateSystem::getConfig() const {
    return pImpl->config;
}

bool TemplateSystem::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> TemplateSystem::getErrors() const {
    return pImpl->errors;
}

void TemplateSystem::clearErrors() {
    pImpl->errors.clear();
}

void TemplateSystem::addError(const std::string& error_message) {
    pImpl->errors.push_back(error_message);
    
    if (pImpl->debug_mode) {
        std::cerr << "模板系统错误: " << error_message << std::endl;
    }
}

std::string TemplateSystem::getDebugInfo() const {
    std::ostringstream oss;
    oss << "模板系统调试信息:\n";
    oss << "模板总数: " << pImpl->templates.size() << "\n";
    oss << "样式组模板: " << getTemplatesByType(TemplateType::STYLE_TEMPLATE).size() << "\n";
    oss << "元素模板: " << getTemplatesByType(TemplateType::ELEMENT_TEMPLATE).size() << "\n";
    oss << "变量组模板: " << getTemplatesByType(TemplateType::VAR_TEMPLATE).size() << "\n";
    oss << "错误数量: " << pImpl->errors.size() << "\n";
    oss << "样式组模板: " << (pImpl->config.enable_style_templates ? "启用" : "禁用") << "\n";
    oss << "元素模板: " << (pImpl->config.enable_element_templates ? "启用" : "禁用") << "\n";
    oss << "变量组模板: " << (pImpl->config.enable_var_templates ? "启用" : "禁用") << "\n";
    oss << "模板继承: " << (pImpl->config.enable_template_inheritance ? "启用" : "禁用") << "\n";
    oss << "自动解析依赖: " << (pImpl->config.auto_resolve_dependencies ? "启用" : "禁用") << "\n";
    return oss.str();
}

void TemplateSystem::enableDebugMode(bool enable) {
    pImpl->debug_mode = enable;
}

// Impl类方法实现
bool TemplateSystem::Impl::isTemplateDeclaration(const std::vector<Token>& tokens, size_t position) {
    if (position + 1 >= tokens.size()) {
        return false;
    }
    
    const Token& token1 = tokens[position];
    const Token& token2 = tokens[position + 1];
    
    return token1.value == "[Template]" && 
           (token2.value == "@Style" || token2.value == "@Element" || token2.value == "@Var");
}

bool TemplateSystem::Impl::isTemplateUsage(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return false;
    }
    
    const Token& token = tokens[position];
    return token.type == TokenType::IDENTIFIER && 
           (templates.find(token.value) != templates.end() || 
            token.value.find("@Style") == 0 || 
            token.value.find("@Element") == 0 || 
            token.value.find("@Var") == 0);
}

TemplateType TemplateSystem::Impl::detectTemplateType(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return TemplateType::STYLE_TEMPLATE;
    }
    
    const Token& token = tokens[position];
    if (token.value == "@Style") {
        return TemplateType::STYLE_TEMPLATE;
    } else if (token.value == "@Element") {
        return TemplateType::ELEMENT_TEMPLATE;
    } else if (token.value == "@Var") {
        return TemplateType::VAR_TEMPLATE;
    }
    
    return TemplateType::STYLE_TEMPLATE;
}

void TemplateSystem::Impl::skipWhitespace(const std::vector<Token>& tokens, size_t& position) {
    while (position < tokens.size() && 
           (tokens[position].value == " " || tokens[position].value == "\t" || 
            tokens[position].value == "\n" || tokens[position].value == "\r")) {
        position++;
    }
}

bool TemplateSystem::Impl::isEndOfTokens(const std::vector<Token>& tokens, size_t position) {
    return position >= tokens.size();
}

bool TemplateSystem::Impl::isValidTemplateName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 检查是否只包含字母、数字、下划线
    for (char c : name) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool TemplateSystem::Impl::isValidTemplateContent(const std::string& content) {
    return !content.empty();
}

bool TemplateSystem::Impl::hasCircularDependency(const std::string& template_name, std::unordered_set<std::string>& visited) {
    if (visited.find(template_name) != visited.end()) {
        return true;
    }
    
    visited.insert(template_name);
    
    auto it = dependency_graph.find(template_name);
    if (it != dependency_graph.end()) {
        for (const auto& dependency : it->second) {
            if (hasCircularDependency(dependency, visited)) {
                return true;
            }
        }
    }
    
    visited.erase(template_name);
    return false;
}

std::string TemplateSystem::Impl::substituteTemplateVariables(const std::string& content, const std::vector<std::string>& parameters) {
    std::string result = content;
    
    // 简单的变量替换，这里可以根据需要实现更复杂的逻辑
    for (size_t i = 0; i < parameters.size(); i++) {
        std::string placeholder = "{" + std::to_string(i) + "}";
        size_t pos = result.find(placeholder);
        while (pos != std::string::npos) {
            result.replace(pos, placeholder.length(), parameters[i]);
            pos = result.find(placeholder, pos + parameters[i].length());
        }
    }
    
    return result;
}

std::string TemplateSystem::Impl::resolveTemplateReferences(const std::string& content) {
    // 解析模板引用，这里可以根据需要实现更复杂的逻辑
    return content;
}

std::string TemplateSystem::Impl::mergeTemplateInheritance(const TemplateInfo& template_info) {
    std::string merged_content = template_info.content;
    
    // 合并依赖的模板内容
    for (const auto& dependency : template_info.dependencies) {
        auto it = templates.find(dependency);
        if (it != templates.end()) {
            merged_content = it->second.content + "\n" + merged_content;
        }
    }
    
    return merged_content;
}

void TemplateSystem::Impl::logDebug(const std::string& message) {
    if (debug_mode) {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}

void TemplateSystem::Impl::addDependency(const std::string& template_name, const std::string& dependency) {
    dependency_graph[template_name].push_back(dependency);
}

std::string TemplateSystem::Impl::extractTemplateContent(const std::vector<Token>& tokens, size_t& position) {
    std::string content;
    int brace_count = 1; // 已经跳过了左大括号
    
    while (position < tokens.size() && brace_count > 0) {
        if (tokens[position].value == "{") {
            brace_count++;
        } else if (tokens[position].value == "}") {
            brace_count--;
        }
        
        if (brace_count > 0) {
            content += tokens[position].value;
        }
        
        position++;
    }
    
    // 移除最后一个右大括号（如果存在）
    if (!content.empty() && content.back() == '}') {
        content.pop_back();
    }
    
    return content;
}

} // namespace chtl