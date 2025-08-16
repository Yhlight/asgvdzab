#include "parser/OriginSystem.h"
#include "ast/ASTNode.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>
#include <regex>

namespace chtl {

// 实现类
class OriginSystem::Impl {
public:
    OriginSystemConfig config;
    std::unordered_map<std::string, OriginInfo> origins;
    std::vector<std::string> errors;
    bool debug_mode;
    
    Impl(const OriginSystemConfig& cfg) : config(cfg), debug_mode(false) {}
    
    ~Impl() = default;
    
    // 内部方法实现
    bool isOriginDeclaration(const std::vector<Token>& tokens, size_t position);
    bool isOriginUsage(const std::vector<Token>& tokens, size_t position);
    OriginType detectOriginType(const std::vector<Token>& tokens, size_t position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    
    // 原始嵌入验证辅助方法
    bool isValidOriginName(const std::string& name);
    bool isValidOriginContent(const std::string& content);
    bool hasValidTargetElement(const std::string& target_element);
    
    // 内容处理辅助方法
    std::string sanitizeHtmlContent(const std::string& content);
    std::string sanitizeCssContent(const std::string& content);
    std::string sanitizeJsContent(const std::string& content);
    std::string injectTargetElement(const std::string& content, const std::string& target_element, OriginType type);
    
    // 辅助方法
    void logDebug(const std::string& message);
    std::string extractOriginContent(const std::vector<Token>& tokens, size_t& position);
    std::string parseOriginIdentifier(const std::vector<Token>& tokens, size_t& position);
};

// OriginSystem实现
OriginSystem::OriginSystem(const OriginSystemConfig& config) 
    : pImpl(std::make_unique<Impl>(config)), state_machine(nullptr) {}

OriginSystem::~OriginSystem() = default;

std::shared_ptr<ASTNode> OriginSystem::parseOriginDeclaration(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isOriginDeclaration(tokens, position)) {
        addError("不是原始嵌入声明");
        return nullptr;
    }
    
    // 跳过[Origin]
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检测原始嵌入类型
    OriginType origin_type = pImpl->detectOriginType(tokens, position);
    
    // 根据类型解析原始嵌入
    OriginInfo origin_info;
    switch (origin_type) {
        case OriginType::HTML_ORIGIN:
            origin_info = parseHtmlOrigin(tokens, position);
            break;
        case OriginType::STYLE_ORIGIN:
            origin_info = parseStyleOrigin(tokens, position);
            break;
        case OriginType::JAVASCRIPT_ORIGIN:
            origin_info = parseJavaScriptOrigin(tokens, position);
            break;
        default:
            addError("未知的原始嵌入类型");
            return nullptr;
    }
    
    // 验证原始嵌入
    if (!validateOrigin(origin_info)) {
        return nullptr;
    }
    
    // 添加原始嵌入到系统
    addOrigin(origin_info);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("原始嵌入声明解析完成: " + origin_info.name + " (类型: " + std::to_string(static_cast<int>(origin_info.type)) + ")");
    }
    
    // 创建原始嵌入声明节点（这里需要创建具体的AST节点类型）
    // 暂时返回nullptr，等待具体的AST节点类型定义
    return nullptr; // TODO: 创建具体的原始嵌入声明AST节点
}

OriginInfo OriginSystem::parseHtmlOrigin(const std::vector<Token>& tokens, size_t& position) {
    OriginInfo origin_info;
    origin_info.type = OriginType::HTML_ORIGIN;
    
    // 跳过@Html
    if (position < tokens.size() && tokens[position].value == "@Html") {
        position++;
    } else {
        addError("HTML原始嵌入缺少@Html关键字");
        return origin_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查是否有增强原始嵌入（带名称）
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        origin_info.name = tokens[position].value;
        origin_info.line = tokens[position].line;
        origin_info.column = tokens[position].column;
        position++;
        
        // 跳过空白字符
        pImpl->skipWhitespace(tokens, position);
    }
    
    // 检查左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("HTML原始嵌入缺少左大括号");
        return origin_info;
    }
    position++;
    
    // 解析原始嵌入内容
    origin_info.content = pImpl->extractOriginContent(tokens, position);
    
    // 检查右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("HTML原始嵌入缺少右大括号");
        return origin_info;
    }
    position++;
    
    return origin_info;
}

OriginInfo OriginSystem::parseStyleOrigin(const std::vector<Token>& tokens, size_t& position) {
    OriginInfo origin_info;
    origin_info.type = OriginType::STYLE_ORIGIN;
    
    // 跳过@Style
    if (position < tokens.size() && tokens[position].value == "@Style") {
        position++;
    } else {
        addError("CSS原始嵌入缺少@Style关键字");
        return origin_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查是否有增强原始嵌入（带名称）
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        origin_info.name = tokens[position].value;
        origin_info.line = tokens[position].line;
        origin_info.column = tokens[position].column;
        position++;
        
        // 跳过空白字符
        pImpl->skipWhitespace(tokens, position);
    }
    
    // 检查左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("CSS原始嵌入缺少左大括号");
        return origin_info;
    }
    position++;
    
    // 解析原始嵌入内容
    origin_info.content = pImpl->extractOriginContent(tokens, position);
    
    // 检查右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("CSS原始嵌入缺少右大括号");
        return origin_info;
    }
    position++;
    
    return origin_info;
}

OriginInfo OriginSystem::parseJavaScriptOrigin(const std::vector<Token>& tokens, size_t& position) {
    OriginInfo origin_info;
    origin_info.type = OriginType::JAVASCRIPT_ORIGIN;
    
    // 跳过@JavaScript
    if (position < tokens.size() && tokens[position].value == "@JavaScript") {
        position++;
    } else {
        addError("JavaScript原始嵌入缺少@JavaScript关键字");
        return origin_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查是否有增强原始嵌入（带名称）
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        origin_info.name = tokens[position].value;
        origin_info.line = tokens[position].line;
        origin_info.column = tokens[position].column;
        position++;
        
        // 跳过空白字符
        pImpl->skipWhitespace(tokens, position);
    }
    
    // 检查左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("JavaScript原始嵌入缺少左大括号");
        return origin_info;
    }
    position++;
    
    // 解析原始嵌入内容
    origin_info.content = pImpl->extractOriginContent(tokens, position);
    
    // 检查右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("JavaScript原始嵌入缺少右大括号");
        return origin_info;
    }
    position++;
    
    return origin_info;
}

OriginInfo OriginSystem::parseEnhancedOrigin(const std::vector<Token>& tokens, size_t& position) {
    OriginInfo origin_info;
    
    // 检测原始嵌入类型
    origin_info.type = pImpl->detectOriginType(tokens, position);
    
    // 跳过类型关键字
    switch (origin_info.type) {
        case OriginType::HTML_ORIGIN:
            if (position < tokens.size() && tokens[position].value == "@Html") {
                position++;
            }
            break;
        case OriginType::STYLE_ORIGIN:
            if (position < tokens.size() && tokens[position].value == "@Style") {
                position++;
            }
            break;
        case OriginType::JAVASCRIPT_ORIGIN:
            if (position < tokens.size() && tokens[position].value == "@JavaScript") {
                position++;
            }
            break;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析嵌入名称
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        origin_info.name = tokens[position].value;
        origin_info.line = tokens[position].line;
        origin_info.column = tokens[position].column;
        position++;
        
        // 跳过空白字符
        pImpl->skipWhitespace(tokens, position);
    }
    
    // 检查左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("增强原始嵌入缺少左大括号");
        return origin_info;
    }
    position++;
    
    // 解析原始嵌入内容
    origin_info.content = pImpl->extractOriginContent(tokens, position);
    
    // 检查右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("增强原始嵌入缺少右大括号");
        return origin_info;
    }
    position++;
    
    return origin_info;
}

std::string OriginSystem::parseOriginTarget(const std::vector<Token>& tokens, size_t& position) {
    std::string target;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析目标元素
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        target = tokens[position].value;
        position++;
    }
    
    return target;
}

std::shared_ptr<ASTNode> OriginSystem::parseOriginUsage(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isOriginUsage(tokens, position)) {
        addError("不是原始嵌入使用");
        return nullptr;
    }
    
    // 解析原始嵌入引用
    std::string origin_ref = parseOriginReference(tokens, position);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("原始嵌入使用解析完成: " + origin_ref);
    }
    
    // 创建原始嵌入使用节点（这里需要创建具体的AST节点类型）
    // 暂时返回nullptr，等待具体的AST节点类型定义
    return nullptr; // TODO: 创建具体的原始嵌入使用AST节点
}

std::string OriginSystem::parseOriginReference(const std::vector<Token>& tokens, size_t& position) {
    std::string reference;
    
    // 检查是否是增强原始嵌入引用
    if (position < tokens.size() && tokens[position].value == "[Origin]") {
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 检测类型
        OriginType type = pImpl->detectOriginType(tokens, position);
        
        // 跳过类型关键字
        switch (type) {
            case OriginType::HTML_ORIGIN:
                if (position < tokens.size() && tokens[position].value == "@Html") {
                    position++;
                }
                break;
            case OriginType::STYLE_ORIGIN:
                if (position < tokens.size() && tokens[position].value == "@Style") {
                    position++;
                }
                break;
            case OriginType::JAVASCRIPT_ORIGIN:
                if (position < tokens.size() && tokens[position].value == "@JavaScript") {
                    position++;
                }
                break;
        }
        
        pImpl->skipWhitespace(tokens, position);
        
        // 解析名称
        if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
            reference = tokens[position].value;
            position++;
        }
        
        // 检查分号
        if (position < tokens.size() && tokens[position].value == ";") {
            position++;
        }
    }
    
    return reference;
}

void OriginSystem::addOrigin(const OriginInfo& origin_info) {
    if (origin_info.name.empty()) {
        addError("原始嵌入名称不能为空");
        return;
    }
    
    pImpl->origins[origin_info.name] = origin_info;
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("添加原始嵌入: " + origin_info.name);
    }
}

void OriginSystem::removeOrigin(const std::string& name) {
    pImpl->origins.erase(name);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("删除原始嵌入: " + name);
    }
}

OriginInfo* OriginSystem::getOrigin(const std::string& name) {
    auto it = pImpl->origins.find(name);
    if (it != pImpl->origins.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<OriginInfo> OriginSystem::getAllOrigins() const {
    std::vector<OriginInfo> result;
    for (const auto& [name, info] : pImpl->origins) {
        result.push_back(info);
    }
    return result;
}

std::vector<OriginInfo> OriginSystem::getOriginsByType(OriginType type) const {
    std::vector<OriginInfo> result;
    for (const auto& [name, info] : pImpl->origins) {
        if (info.type == type) {
            result.push_back(info);
        }
    }
    return result;
}

bool OriginSystem::hasOrigin(const std::string& name) const {
    return pImpl->origins.find(name) != pImpl->origins.end();
}

bool OriginSystem::validateOrigin(const OriginInfo& origin_info) {
    // 验证原始嵌入名称
    if (!validateOriginName(origin_info.name)) {
        addError("无效的原始嵌入名称: " + origin_info.name);
        return false;
    }
    
    // 验证原始嵌入内容
    if (!validateOriginContent(origin_info.content, origin_info.type)) {
        addError("无效的原始嵌入内容: " + origin_info.name);
        return false;
    }
    
    return true;
}

std::vector<std::string> OriginSystem::validateOriginList(const std::vector<OriginInfo>& origins) {
    std::vector<std::string> errors;
    
    for (const auto& origin_info : origins) {
        if (!validateOrigin(origin_info)) {
            auto origin_errors = getErrors();
            errors.insert(errors.end(), origin_errors.begin(), origin_errors.end());
        }
    }
    
    return errors;
}

bool OriginSystem::validateOriginName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 检查长度限制
    if (name.length() > pImpl->config.max_origin_name_length) {
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

bool OriginSystem::validateOriginContent(const std::string& content, OriginType type) {
    if (content.empty()) {
        return false;
    }
    
    // 检查长度限制
    if (content.length() > pImpl->config.max_origin_content_length) {
        return false;
    }
    
    // 根据类型进行特定验证
    switch (type) {
        case OriginType::HTML_ORIGIN:
            // HTML内容应该包含HTML标签
            return content.find("<") != std::string::npos || content.find(">") != std::string::npos;
        case OriginType::STYLE_ORIGIN:
            // CSS内容应该包含CSS属性
            return content.find(":") != std::string::npos || content.find(";") != std::string::npos;
        case OriginType::JAVASCRIPT_ORIGIN:
            // JavaScript内容应该包含JavaScript语法
            return content.find(";") != std::string::npos || content.find("(") != std::string::npos || content.find(")") != std::string::npos;
        default:
            return true;
    }
}

std::string OriginSystem::processHtmlOrigin(const std::string& origin_name, const std::string& target_element) {
    auto* origin_info = getOrigin(origin_name);
    if (!origin_info || origin_info->type != OriginType::HTML_ORIGIN) {
        addError("HTML原始嵌入不存在: " + origin_name);
        return "";
    }
    
    std::string content = origin_info->content;
    
    // 应用内容处理
    if (pImpl->config.auto_escape_content) {
        content = escapeContent(content, OriginType::HTML_ORIGIN);
    }
    
    // 注入目标元素（如果有）
    if (!target_element.empty()) {
        content = pImpl->injectTargetElement(content, target_element, OriginType::HTML_ORIGIN);
    }
    
    return content;
}

std::string OriginSystem::processStyleOrigin(const std::string& origin_name, const std::string& target_element) {
    auto* origin_info = getOrigin(origin_name);
    if (!origin_info || origin_info->type != OriginType::STYLE_ORIGIN) {
        addError("CSS原始嵌入不存在: " + origin_name);
        return "";
    }
    
    std::string content = origin_info->content;
    
    // 应用内容处理
    if (pImpl->config.auto_escape_content) {
        content = escapeContent(content, OriginType::STYLE_ORIGIN);
    }
    
    // 注入目标元素（如果有）
    if (!target_element.empty()) {
        content = pImpl->injectTargetElement(content, target_element, OriginType::STYLE_ORIGIN);
    }
    
    return content;
}

std::string OriginSystem::processJavaScriptOrigin(const std::string& origin_name, const std::string& target_element) {
    auto* origin_info = getOrigin(origin_name);
    if (!origin_info || origin_info->type != OriginType::JAVASCRIPT_ORIGIN) {
        addError("JavaScript原始嵌入不存在: " + origin_name);
        return "";
    }
    
    std::string content = origin_info->content;
    
    // 应用内容处理
    if (pImpl->config.auto_escape_content) {
        content = escapeContent(content, OriginType::JAVASCRIPT_ORIGIN);
    }
    
    // 注入目标元素（如果有）
    if (!target_element.empty()) {
        content = pImpl->injectTargetElement(content, target_element, OriginType::JAVASCRIPT_ORIGIN);
    }
    
    return content;
}

std::string OriginSystem::escapeContent(const std::string& content, OriginType type) {
    std::string result = content;
    
    switch (type) {
        case OriginType::HTML_ORIGIN:
            // HTML内容转义
            result = pImpl->sanitizeHtmlContent(content);
            break;
        case OriginType::STYLE_ORIGIN:
            // CSS内容转义
            result = pImpl->sanitizeCssContent(content);
            break;
        case OriginType::JAVASCRIPT_ORIGIN:
            // JavaScript内容转义
            result = pImpl->sanitizeJsContent(content);
            break;
    }
    
    return result;
}

std::string OriginSystem::unescapeContent(const std::string& content, OriginType type) {
    // 简单的反转义，这里可以根据需要实现更复杂的逻辑
    std::string result = content;
    
    // 替换常见的转义字符
    std::regex amp_regex("&amp;");
    std::regex lt_regex("&lt;");
    std::regex gt_regex("&gt;");
    std::regex quot_regex("&quot;");
    
    result = std::regex_replace(result, amp_regex, "&");
    result = std::regex_replace(result, lt_regex, "<");
    result = std::regex_replace(result, gt_regex, ">");
    result = std::regex_replace(result, quot_regex, "\"");
    
    return result;
}

std::string OriginSystem::validateContent(const std::string& content, OriginType type) {
    // 内容验证，这里可以根据需要实现更复杂的逻辑
    std::string result = content;
    
    switch (type) {
        case OriginType::HTML_ORIGIN:
            result = pImpl->sanitizeHtmlContent(content);
            break;
        case OriginType::STYLE_ORIGIN:
            result = pImpl->sanitizeCssContent(content);
            break;
        case OriginType::JAVASCRIPT_ORIGIN:
            result = pImpl->sanitizeJsContent(content);
            break;
    }
    
    return result;
}

void OriginSystem::setConfig(const OriginSystemConfig& config) {
    pImpl->config = config;
}

OriginSystemConfig OriginSystem::getConfig() const {
    return pImpl->config;
}

bool OriginSystem::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> OriginSystem::getErrors() const {
    return pImpl->errors;
}

void OriginSystem::clearErrors() {
    pImpl->errors.clear();
}

void OriginSystem::addError(const std::string& error_message) {
    pImpl->errors.push_back(error_message);
    
    if (pImpl->debug_mode) {
        std::cerr << "原始嵌入系统错误: " << error_message << std::endl;
    }
}

std::string OriginSystem::getDebugInfo() const {
    std::ostringstream oss;
    oss << "原始嵌入系统调试信息:\n";
    oss << "原始嵌入总数: " << pImpl->origins.size() << "\n";
    oss << "HTML原始嵌入: " << getOriginsByType(OriginType::HTML_ORIGIN).size() << "\n";
    oss << "CSS原始嵌入: " << getOriginsByType(OriginType::STYLE_ORIGIN).size() << "\n";
    oss << "JavaScript原始嵌入: " << getOriginsByType(OriginType::JAVASCRIPT_ORIGIN).size() << "\n";
    oss << "错误数量: " << pImpl->errors.size() << "\n";
    oss << "HTML原始嵌入: " << (pImpl->config.enable_html_origin ? "启用" : "禁用") << "\n";
    oss << "CSS原始嵌入: " << (pImpl->config.enable_style_origin ? "启用" : "禁用") << "\n";
    oss << "JavaScript原始嵌入: " << (pImpl->config.enable_javascript_origin ? "启用" : "禁用") << "\n";
    oss << "增强原始嵌入: " << (pImpl->config.enable_enhanced_origin ? "启用" : "禁用") << "\n";
    oss << "严格内容验证: " << (pImpl->config.strict_content_validation ? "启用" : "禁用") << "\n";
    oss << "自动转义内容: " << (pImpl->config.auto_escape_content ? "启用" : "禁用") << "\n";
    oss << "保留空白字符: " << (pImpl->config.preserve_whitespace ? "启用" : "禁用") << "\n";
    return oss.str();
}

void OriginSystem::enableDebugMode(bool enable) {
    pImpl->debug_mode = enable;
}

void OriginSystem::setStateMachine(std::shared_ptr<CHTLStateMachine> sm) {
    state_machine = sm;
}

// Impl类方法实现
bool OriginSystem::Impl::isOriginDeclaration(const std::vector<Token>& tokens, size_t position) {
    if (position + 1 >= tokens.size()) {
        return false;
    }
    
    const Token& token1 = tokens[position];
    const Token& token2 = tokens[position + 1];
    
    return token1.value == "[Origin]" && 
           (token2.value == "@Html" || token2.value == "@Style" || token2.value == "@JavaScript");
}

bool OriginSystem::Impl::isOriginUsage(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return false;
    }
    
    const Token& token = tokens[position];
    return token.value == "[Origin]" || 
           (token.type == TokenType::IDENTIFIER && origins.find(token.value) != origins.end());
}

OriginType OriginSystem::Impl::detectOriginType(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return OriginType::HTML_ORIGIN;
    }
    
    const Token& token = tokens[position];
    if (token.value == "@Html") {
        return OriginType::HTML_ORIGIN;
    } else if (token.value == "@Style") {
        return OriginType::STYLE_ORIGIN;
    } else if (token.value == "@JavaScript") {
        return OriginType::JAVASCRIPT_ORIGIN;
    }
    
    return OriginType::HTML_ORIGIN;
}

void OriginSystem::Impl::skipWhitespace(const std::vector<Token>& tokens, size_t& position) {
    while (position < tokens.size() && 
           (tokens[position].value == " " || tokens[position].value == "\t" || 
            tokens[position].value == "\n" || tokens[position].value == "\r")) {
        position++;
    }
}

bool OriginSystem::Impl::isEndOfTokens(const std::vector<Token>& tokens, size_t position) {
    return position >= tokens.size();
}

bool OriginSystem::Impl::isValidOriginName(const std::string& name) {
    if (name.empty()) {
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

bool OriginSystem::Impl::isValidOriginContent(const std::string& content) {
    return !content.empty();
}

bool OriginSystem::Impl::hasValidTargetElement(const std::string& target_element) {
    // 这里需要检查目标元素是否有效
    // 暂时返回true
    return true;
}

std::string OriginSystem::Impl::sanitizeHtmlContent(const std::string& content) {
    std::string result = content;
    
    // 基本的HTML内容清理
    // 这里可以根据需要实现更复杂的清理逻辑
    
    return result;
}

std::string OriginSystem::Impl::sanitizeCssContent(const std::string& content) {
    std::string result = content;
    
    // 基本的CSS内容清理
    // 这里可以根据需要实现更复杂的清理逻辑
    
    return result;
}

std::string OriginSystem::Impl::sanitizeJsContent(const std::string& content) {
    std::string result = content;
    
    // 基本的JavaScript内容清理
    // 这里可以根据需要实现更复杂的清理逻辑
    
    return result;
}

std::string OriginSystem::Impl::injectTargetElement(const std::string& content, const std::string& target_element, OriginType type) {
    std::string result = content;
    
    switch (type) {
        case OriginType::HTML_ORIGIN:
            // 在HTML内容中注入目标元素
            if (result.find("<body>") != std::string::npos) {
                std::regex body_regex("<body>");
                result = std::regex_replace(result, body_regex, "<body>" + target_element);
            } else {
                result = target_element + "\n" + result;
            }
            break;
        case OriginType::STYLE_ORIGIN:
            // 在CSS内容中注入目标元素选择器
            if (!result.empty()) {
                result = target_element + " {\n" + result + "\n}";
            }
            break;
        case OriginType::JAVASCRIPT_ORIGIN:
            // 在JavaScript内容中注入目标元素引用
            if (!result.empty()) {
                result = "const targetElement = document.querySelector('" + target_element + "');\n" + result;
            }
            break;
    }
    
    return result;
}

std::string OriginSystem::Impl::extractOriginContent(const std::vector<Token>& tokens, size_t& position) {
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

std::string OriginSystem::Impl::parseOriginIdentifier(const std::vector<Token>& tokens, size_t& position) {
    std::string identifier;
    
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        identifier = tokens[position].value;
        position++;
    }
    
    return identifier;
}

void OriginSystem::Impl::logDebug(const std::string& message) {
    if (debug_mode) {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}

} // namespace chtl