#include "parser/AttributeSystem.h"
#include "ast/AttributeNode.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>
#include <regex>
#include <cmath>

namespace chtl {

// 实现类
class AttributeSystem::Impl {
public:
    AttributeSystemConfig config;
    std::vector<std::shared_ptr<AttributeNode>> attributes;
    std::vector<std::shared_ptr<AttributeNode>> inherited_attributes;
    std::vector<std::string> errors;
    std::vector<std::string> validation_errors;
    bool debug_mode;
    
    // 预定义的属性验证规则
    std::unordered_map<std::string, std::vector<AttributeValidationRule>> element_attribute_rules;
    std::unordered_map<std::string, std::vector<std::string>> attribute_enum_values;
    std::unordered_map<std::string, std::pair<double, double>> attribute_value_ranges;
    std::unordered_map<std::string, std::string> attribute_patterns;
    
    Impl(const AttributeSystemConfig& cfg) : config(cfg), debug_mode(false) {
        initializeAttributeRules();
    }
    
    ~Impl() = default;
    
    // 内部方法实现
    bool isAttributeDeclaration(const std::vector<Token>& tokens, size_t position);
    std::pair<std::string, std::string> extractAttribute(const std::vector<Token>& tokens, size_t& position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    void addValidationError(const std::string& error_message);
    
    // 属性验证辅助方法
    bool validateAttributePattern(const std::string& name, const std::string& pattern);
    bool validateAttributeValueRange(const std::string& value, double min_val, double max_val);
    bool validateAttributeEnum(const std::string& value, const std::vector<std::string>& allowed_values);
    bool validateCustomAttribute(const AttributeInfo& attr_info);
    
    // 辅助方法
    void initializeAttributeRules();
    bool isValidAttributeName(const std::string& name);
    bool isValidAttributeValue(const std::string& value);
    std::string normalizeAttributeValue(const std::string& value);
    void logDebug(const std::string& message);
};

// AttributeSystem实现
AttributeSystem::AttributeSystem(const AttributeSystemConfig& config) 
    : pImpl(std::make_unique<Impl>(config)) {}

AttributeSystem::~AttributeSystem() = default;

std::shared_ptr<AttributeNode> AttributeSystem::parseAttribute(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isAttributeDeclaration(tokens, position)) {
        addError("不是属性声明");
        return nullptr;
    }
    
    auto [attr_name, attr_value] = pImpl->extractAttribute(tokens, position);
    
    if (attr_name.empty()) {
        addError("属性名不能为空");
        return nullptr;
    }
    
    if (!pImpl->isValidAttributeName(attr_name)) {
        addError("无效的属性名: " + attr_name);
        return nullptr;
    }
    
    // 检查是否有分号
    if (position >= tokens.size() || tokens[position].value != ";") {
        addError("属性声明缺少分号");
        return nullptr;
    }
    position++;
    
    // 检测属性值类型
    AttributeValueType value_type = detectAttributeValueType(attr_value);
    
    // 创建属性节点
    auto attr_node = std::make_shared<AttributeNode>(attr_name, attr_value);
    attr_node->line = tokens[position - 2].line;  // 属性名所在行
    attr_node->column = tokens[position - 2].column;  // 属性名所在列
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("解析属性: " + attr_name + " = " + attr_value + " (类型: " + std::to_string(static_cast<int>(value_type)) + ")");
    }
    
    return attr_node;
}

std::vector<std::shared_ptr<AttributeNode>> AttributeSystem::parseAttributeList(const std::vector<Token>& tokens, size_t& position) {
    std::vector<std::shared_ptr<AttributeNode>> result;
    
    while (position < tokens.size() && pImpl->isAttributeDeclaration(tokens, position)) {
        auto attr_node = parseAttribute(tokens, position);
        if (attr_node) {
            result.push_back(attr_node);
        }
        
        pImpl->skipWhitespace(tokens, position);
    }
    
    return result;
}

std::string AttributeSystem::normalizeAttributeValue(const std::string& value, AttributeValueType type) {
    switch (type) {
        case AttributeValueType::STRING_LITERAL:
            return unescapeAttributeValue(value);
        case AttributeValueType::UNQUOTED_LITERAL:
            return pImpl->normalizeAttributeValue(value);
        case AttributeValueType::NUMERIC_LITERAL:
            return value;  // 数值不需要转义
        case AttributeValueType::BOOLEAN_LITERAL:
            return value;  // 布尔值不需要转义
        case AttributeValueType::EXPRESSION:
            return value;  // 表达式保持原样
        case AttributeValueType::TEMPLATE_REFERENCE:
            return value;  // 模板引用保持原样
        case AttributeValueType::FUNCTION_CALL:
            return value;  // 函数调用保持原样
        default:
            return value;
    }
}

AttributeValueType AttributeSystem::detectAttributeValueType(const std::string& value) {
    if (value.empty()) {
        return AttributeValueType::STRING_LITERAL;
    }
    
    // 检查是否是布尔值
    if (value == "true" || value == "false" || value == "on" || value == "off") {
        return AttributeValueType::BOOLEAN_LITERAL;
    }
    
    // 检查是否是数值
    if (std::regex_match(value, std::regex("^-?\\d+(\\.\\d+)?$"))) {
        return AttributeValueType::NUMERIC_LITERAL;
    }
    
    // 检查是否是模板引用
    if (value.find("@") == 0 || value.find("ThemeColor(") == 0) {
        return AttributeValueType::TEMPLATE_REFERENCE;
    }
    
    // 检查是否是函数调用
    if (value.find("(") != std::string::npos && value.find(")") != std::string::npos) {
        return AttributeValueType::FUNCTION_CALL;
    }
    
    // 检查是否是表达式
    if (value.find("{{") != std::string::npos || value.find("}}") != std::string::npos) {
        return AttributeValueType::EXPRESSION;
    }
    
    // 检查是否包含引号
    if (value.find("\"") != std::string::npos || value.find("'") != std::string::npos) {
        return AttributeValueType::STRING_LITERAL;
    }
    
    // 默认为无修饰字面量
    return AttributeValueType::UNQUOTED_LITERAL;
}

std::string AttributeSystem::escapeAttributeValue(const std::string& value) {
    std::string result;
    result.reserve(value.length());
    
    for (char c : value) {
        switch (c) {
            case '"': result += "&quot;"; break;
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            default: result += c; break;
        }
    }
    
    return result;
}

std::string AttributeSystem::unescapeAttributeValue(const std::string& value) {
    std::string result = value;
    
    // 替换HTML实体
    std::unordered_map<std::string, char> entities = {
        {"&quot;", '"'}, {"&amp;", '&'}, {"&lt;", '<'}, {"&gt;", '>'}
    };
    
    for (const auto& [entity, char_val] : entities) {
        size_t pos = 0;
        while ((pos = result.find(entity, pos)) != std::string::npos) {
            result.replace(pos, entity.length(), 1, char_val);
            pos += 1;
        }
    }
    
    return result;
}

bool AttributeSystem::validateAttribute(const AttributeInfo& attr_info) {
    // 验证属性名
    if (!validateAttributeName(attr_info.name)) {
        addError("无效的属性名: " + attr_info.name);
        return false;
    }
    
    // 验证属性值
    if (!validateAttributeValue(attr_info.value, attr_info.value_type)) {
        addError("无效的属性值: " + attr_info.value);
        return false;
    }
    
    // 验证长度限制
    if (attr_info.name.length() > pImpl->config.max_attribute_name_length) {
        addError("属性名过长: " + attr_info.name);
        return false;
    }
    
    if (attr_info.value.length() > pImpl->config.max_attribute_value_length) {
        addError("属性值过长: " + attr_info.value);
        return false;
    }
    
    // 应用验证规则
    for (const auto& rule : attr_info.validation_rules) {
        switch (rule) {
            case AttributeValidationRule::REQUIRED:
                if (attr_info.value.empty()) {
                    addError("必需属性不能为空: " + attr_info.name);
                    return false;
                }
                break;
            case AttributeValidationRule::UNIQUE:
                if (hasAttribute(attr_info.name)) {
                    addError("重复的属性: " + attr_info.name);
                    return false;
                }
                break;
            case AttributeValidationRule::PATTERN_MATCH:
                if (!pImpl->validateAttributePattern(attr_info.name, attr_info.name)) {
                    addError("属性名不符合模式: " + attr_info.name);
                    return false;
                }
                break;
            case AttributeValidationRule::VALUE_RANGE:
                // 这里需要具体的范围值，暂时跳过
                break;
            case AttributeValidationRule::ENUM_VALUES:
                // 这里需要具体的枚举值，暂时跳过
                break;
            case AttributeValidationRule::CUSTOM_VALIDATOR:
                if (!pImpl->validateCustomAttribute(attr_info)) {
                    addError("自定义验证失败: " + attr_info.name);
                    return false;
                }
                break;
            default:
                break;
        }
    }
    
    return true;
}

std::vector<std::string> AttributeSystem::validateAttributeList(const std::vector<AttributeInfo>& attrs) {
    std::vector<std::string> errors;
    
    for (const auto& attr : attrs) {
        if (!validateAttribute(attr)) {
            auto attr_errors = getErrors();
            errors.insert(errors.end(), attr_errors.begin(), attr_errors.end());
        }
    }
    
    return errors;
}

bool AttributeSystem::validateAttributeName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 属性名必须以字母开头
    if (!std::isalpha(name[0])) {
        return false;
    }
    
    // 检查是否只包含字母、数字、连字符、下划线
    for (char c : name) {
        if (!std::isalnum(c) && c != '-' && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool AttributeSystem::validateAttributeValue(const std::string& value, AttributeValueType type) {
    switch (type) {
        case AttributeValueType::STRING_LITERAL:
            return !value.empty();
        case AttributeValueType::UNQUOTED_LITERAL:
            return !value.empty() && value.find_first_of(" \t\n\r{}[]()") == std::string::npos;
        case AttributeValueType::NUMERIC_LITERAL:
            return std::regex_match(value, std::regex("^-?\\d+(\\.\\d+)?$"));
        case AttributeValueType::BOOLEAN_LITERAL:
            return value == "true" || value == "false" || value == "on" || value == "off";
        case AttributeValueType::EXPRESSION:
            return !value.empty();
        case AttributeValueType::TEMPLATE_REFERENCE:
            return !value.empty();
        case AttributeValueType::FUNCTION_CALL:
            return !value.empty() && value.find("(") != std::string::npos && value.find(")") != std::string::npos;
        default:
            return !value.empty();
    }
}

void AttributeSystem::addAttribute(std::shared_ptr<AttributeNode> attr) {
    if (attr) {
        // 检查重复属性
        if (!pImpl->config.allow_duplicate_attributes && hasAttribute(attr->getName())) {
            addError("重复的属性: " + attr->getName());
            return;
        }
        
        pImpl->attributes.push_back(attr);
        
        if (pImpl->debug_mode) {
            pImpl->logDebug("添加属性: " + attr->getName() + " = " + attr->getValue());
        }
    }
}

void AttributeSystem::removeAttribute(const std::string& name) {
    pImpl->attributes.erase(
        std::remove_if(pImpl->attributes.begin(), pImpl->attributes.end(),
            [&name](const std::shared_ptr<AttributeNode>& attr) {
                return attr->getName() == name;
            }),
        pImpl->attributes.end()
    );
}

std::shared_ptr<AttributeNode> AttributeSystem::getAttribute(const std::string& name) const {
    for (const auto& attr : pImpl->attributes) {
        if (attr->getName() == name) {
            return attr;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<AttributeNode>> AttributeSystem::getAttributes() const {
    return pImpl->attributes;
}

bool AttributeSystem::hasAttribute(const std::string& name) const {
    return getAttribute(name) != nullptr;
}

void AttributeSystem::inheritAttributes(const std::vector<std::shared_ptr<AttributeNode>>& parent_attrs) {
    if (!pImpl->config.enable_attribute_inheritance) {
        return;
    }
    
    pImpl->inherited_attributes.clear();
    for (const auto& parent_attr : parent_attrs) {
        if (parent_attr) {
            pImpl->inherited_attributes.push_back(parent_attr);
        }
    }
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("继承属性数量: " + std::to_string(pImpl->inherited_attributes.size()));
    }
}

void AttributeSystem::overrideAttribute(const std::string& name, const std::string& new_value) {
    // 先检查当前属性
    auto current_attr = getAttribute(name);
    if (current_attr) {
        current_attr->setValue(new_value);
        if (pImpl->debug_mode) {
            pImpl->logDebug("覆盖属性: " + name + " = " + new_value);
        }
        return;
    }
    
    // 检查继承的属性
    for (auto& inherited_attr : pImpl->inherited_attributes) {
        if (inherited_attr->getName() == name) {
            inherited_attr->setValue(new_value);
            if (pImpl->debug_mode) {
                pImpl->logDebug("覆盖继承属性: " + name + " = " + new_value);
            }
            return;
        }
    }
    
    // 如果都不存在，创建新属性
    auto new_attr = std::make_shared<AttributeNode>(name, new_value);
    addAttribute(new_attr);
}

std::vector<std::shared_ptr<AttributeNode>> AttributeSystem::getInheritedAttributes() const {
    return pImpl->inherited_attributes;
}

std::string AttributeSystem::convertToHTML(const std::vector<std::shared_ptr<AttributeNode>>& attrs) {
    std::ostringstream oss;
    
    for (const auto& attr : attrs) {
        if (attr) {
            oss << " " << attr->getName() << "=\"" << escapeAttributeValue(attr->getValue()) << "\"";
        }
    }
    
    return oss.str();
}

std::string AttributeSystem::convertToCSS(const std::vector<std::shared_ptr<AttributeNode>>& attrs) {
    std::ostringstream oss;
    
    for (const auto& attr : attrs) {
        if (attr) {
            oss << attr->getName() << ": " << attr->getValue() << ";\n";
        }
    }
    
    return oss.str();
}

std::string AttributeSystem::convertToJavaScript(const std::vector<std::shared_ptr<AttributeNode>>& attrs) {
    std::ostringstream oss;
    
    for (const auto& attr : attrs) {
        if (attr) {
            oss << attr->getName() << ": \"" << escapeAttributeValue(attr->getValue()) << "\",\n";
        }
    }
    
    return oss.str();
}

void AttributeSystem::setConfig(const AttributeSystemConfig& config) {
    pImpl->config = config;
}

AttributeSystemConfig AttributeSystem::getConfig() const {
    return pImpl->config;
}

bool AttributeSystem::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> AttributeSystem::getErrors() const {
    return pImpl->errors;
}

void AttributeSystem::clearErrors() {
    pImpl->errors.clear();
}

void AttributeSystem::addError(const std::string& error_message) {
    pImpl->errors.push_back(error_message);
    
    if (pImpl->debug_mode) {
        std::cerr << "属性系统错误: " << error_message << std::endl;
    }
}

std::string AttributeSystem::getDebugInfo() const {
    std::ostringstream oss;
    oss << "属性系统调试信息:\n";
    oss << "属性数量: " << pImpl->attributes.size() << "\n";
    oss << "继承属性数量: " << pImpl->inherited_attributes.size() << "\n";
    oss << "错误数量: " << pImpl->errors.size() << "\n";
    oss << "验证错误数量: " << pImpl->validation_errors.size() << "\n";
    oss << "CE等价性: " << (pImpl->config.enable_ce_equivalence ? "启用" : "禁用") << "\n";
    oss << "无修饰字面量: " << (pImpl->config.enable_unquoted_literals ? "启用" : "禁用") << "\n";
    oss << "严格验证: " << (pImpl->config.strict_validation ? "启用" : "禁用") << "\n";
    oss << "允许重复属性: " << (pImpl->config.allow_duplicate_attributes ? "启用" : "禁用") << "\n";
    oss << "属性继承: " << (pImpl->config.enable_attribute_inheritance ? "启用" : "禁用") << "\n";
    return oss.str();
}

void AttributeSystem::enableDebugMode(bool enable) {
    pImpl->debug_mode = enable;
}

// Impl类方法实现
bool AttributeSystem::Impl::isAttributeDeclaration(const std::vector<Token>& tokens, size_t position) {
    if (position + 2 >= tokens.size()) {
        return false;
    }
    
    const Token& attr_name = tokens[position];
    const Token& separator = tokens[position + 1];
    const Token& attr_value = tokens[position + 2];
    
    // 检查格式：属性名 : "属性值"
    return isValidAttributeName(attr_name.value) && 
           (separator.value == ":" || (config.enable_ce_equivalence && separator.value == "=")) &&
           (attr_value.value == "\"" || attr_value.value == "'" || 
            (config.enable_unquoted_literals && !attr_value.value.empty()));
}

std::pair<std::string, std::string> AttributeSystem::Impl::extractAttribute(const std::vector<Token>& tokens, size_t& position) {
    std::string attr_name = tokens[position].value;
    position++;
    
    // 跳过分隔符（: 或 =）
    position++;
    
    std::string attr_value;
    if (position < tokens.size()) {
        if (tokens[position].value == "\"" || tokens[position].value == "'") {
            // 引号字符串
            char quote_char = tokens[position].value[0];
            position++;
            
            while (position < tokens.size() && tokens[position].value != std::string(1, quote_char)) {
                attr_value += tokens[position].value;
                position++;
            }
            
            if (position < tokens.size()) {
                position++; // 跳过结束引号
            }
        } else {
            // 无修饰字面量
            attr_value = tokens[position].value;
            position++;
        }
    }
    
    return {attr_name, attr_value};
}

void AttributeSystem::Impl::skipWhitespace(const std::vector<Token>& tokens, size_t& position) {
    while (position < tokens.size() && 
           (tokens[position].value == " " || tokens[position].value == "\t" || 
            tokens[position].value == "\n" || tokens[position].value == "\r")) {
        position++;
    }
}

bool AttributeSystem::Impl::isEndOfTokens(const std::vector<Token>& tokens, size_t position) {
    return position >= tokens.size();
}

void AttributeSystem::Impl::addValidationError(const std::string& error_message) {
    validation_errors.push_back(error_message);
}

bool AttributeSystem::Impl::validateAttributePattern(const std::string& name, const std::string& pattern) {
    try {
        std::regex re(pattern);
        return std::regex_match(name, re);
    } catch (const std::regex_error&) {
        return false;
    }
}

bool AttributeSystem::Impl::validateAttributeValueRange(const std::string& value, double min_val, double max_val) {
    try {
        double num_val = std::stod(value);
        return num_val >= min_val && num_val <= max_val;
    } catch (const std::invalid_argument&) {
        return false;
    }
}

bool AttributeSystem::Impl::validateAttributeEnum(const std::string& value, const std::vector<std::string>& allowed_values) {
    return std::find(allowed_values.begin(), allowed_values.end(), value) != allowed_values.end();
}

bool AttributeSystem::Impl::validateCustomAttribute(const AttributeInfo& attr_info) {
    // 这里可以实现自定义验证逻辑
    // 暂时返回true
    return true;
}

void AttributeSystem::Impl::initializeAttributeRules() {
    // 初始化HTML元素的属性验证规则
    element_attribute_rules["div"] = {AttributeValidationRule::OPTIONAL, AttributeValidationRule::UNIQUE};
    element_attribute_rules["span"] = {AttributeValidationRule::OPTIONAL, AttributeValidationRule::UNIQUE};
    element_attribute_rules["p"] = {AttributeValidationRule::OPTIONAL, AttributeValidationRule::UNIQUE};
    
    // 初始化属性枚举值
    attribute_enum_values["type"] = {"text", "password", "email", "number", "submit", "button"};
    attribute_enum_values["method"] = {"get", "post"};
    attribute_enum_values["target"] = {"_blank", "_self", "_parent", "_top"};
    
    // 初始化属性值范围
    attribute_value_ranges["width"] = {0.0, 10000.0};
    attribute_value_ranges["height"] = {0.0, 10000.0};
    attribute_value_ranges["opacity"] = {0.0, 1.0};
    
    // 初始化属性模式
    attribute_patterns["id"] = "^[a-zA-Z][a-zA-Z0-9_-]*$";
    attribute_patterns["class"] = "^[a-zA-Z][a-zA-Z0-9_-]*(\\s+[a-zA-Z][a-zA-Z0-9_-]*)*$";
}

bool AttributeSystem::Impl::isValidAttributeName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 属性名必须以字母开头
    if (!std::isalpha(name[0])) {
        return false;
    }
    
    // 检查是否只包含字母、数字、连字符、下划线
    for (char c : name) {
        if (!std::isalnum(c) && c != '-' && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool AttributeSystem::Impl::isValidAttributeValue(const std::string& value) {
    return !value.empty();
}

std::string AttributeSystem::Impl::normalizeAttributeValue(const std::string& value) {
    // 移除首尾空白字符
    std::string result = value;
    result.erase(0, result.find_first_not_of(" \t\n\r"));
    result.erase(result.find_last_not_of(" \t\n\r") + 1);
    return result;
}

void AttributeSystem::Impl::logDebug(const std::string& message) {
    if (debug_mode) {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}

} // namespace chtl