#include "parser/ConfigurationSystem.h"
#include "ast/ASTNode.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>
#include <regex>

namespace chtl {

// 实现类
class ConfigurationSystem::Impl {
public:
    ConfigurationSystemConfig config;
    std::unordered_map<std::string, ConfigurationGroupInfo> configuration_groups;
    std::vector<std::string> errors;
    bool debug_mode;
    
    Impl(const ConfigurationSystemConfig& cfg) : config(cfg), debug_mode(false) {}
    
    ~Impl() = default;
    
    // 内部方法实现
    bool isConfigurationDeclaration(const std::vector<Token>& tokens, size_t position);
    bool isNameGroupDeclaration(const std::vector<Token>& tokens, size_t position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    
    // 配置验证辅助方法
    bool isValidConfigName(const std::string& name);
    bool isValidKeywordName(const std::string& name);
    bool isValidConfigValue(const ConfigValue& value);
    bool isValidGroupOptionCount(size_t count);
    
    // 配置应用辅助方法
    void updateSystemConfig(const ConfigItemInfo& config_item);
    void updateKeywordMappings(const KeywordConfigInfo& keyword_config);
    void validateKeywordConsistency();
    
    // 辅助方法
    void logDebug(const std::string& message);
    std::string extractConfigurationContent(const std::vector<Token>& tokens, size_t& position);
    std::string parseConfigurationIdentifier(const std::vector<Token>& tokens, size_t& position);
    KeywordType detectKeywordType(const std::string& name);
};

// ConfigurationSystem实现
ConfigurationSystem::ConfigurationSystem(const ConfigurationSystemConfig& config) 
    : pImpl(std::make_unique<Impl>(config)), state_machine(nullptr) {}

ConfigurationSystem::~ConfigurationSystem() = default;

std::shared_ptr<ASTNode> ConfigurationSystem::parseConfigurationDeclaration(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isConfigurationDeclaration(tokens, position)) {
        addError("不是配置组声明");
        return nullptr;
    }
    
    // 跳过[Configuration]
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析配置组
    ConfigurationGroupInfo config_group = parseConfigurationGroup(tokens, position);
    
    // 验证配置组
    if (!validateConfigurationGroup(config_group)) {
        return nullptr;
    }
    
    // 添加配置组到系统
    addConfigurationGroup(config_group);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("配置组声明解析完成: " + config_group.name);
    }
    
    // 创建配置组声明节点（这里需要创建具体的AST节点类型）
    // 暂时返回nullptr，等待具体的AST节点类型定义
    return nullptr; // TODO: 创建具体的配置组声明AST节点
}

ConfigurationGroupInfo ConfigurationSystem::parseConfigurationGroup(const std::vector<Token>& tokens, size_t& position) {
    ConfigurationGroupInfo config_group;
    
    // 检查左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("配置组缺少左大括号");
        return config_group;
    }
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析配置项
    config_group.config_items = parseConfigItems(tokens, position);
    
    // 检查是否有名称配置组
    if (position < tokens.size() && tokens[position].value == "[Name]") {
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 检查左大括号
        if (position < tokens.size() && tokens[position].value == "{") {
            position++;
            pImpl->skipWhitespace(tokens, position);
            
            // 解析名称配置组
            config_group.keyword_configs = parseNameGroup(tokens, position);
            
            // 检查右大括号
            if (position < tokens.size() && tokens[position].value == "}") {
                position++;
            }
        }
    }
    
    // 检查右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("配置组缺少右大括号");
        return config_group;
    }
    position++;
    
    return config_group;
}

std::vector<ConfigItemInfo> ConfigurationSystem::parseConfigItems(const std::vector<Token>& tokens, size_t& position) {
    std::vector<ConfigItemInfo> config_items;
    
    while (position < tokens.size() && tokens[position].value != "}" && tokens[position].value != "[Name]") {
        // 跳过注释
        if (tokens[position].value == "//") {
            while (position < tokens.size() && tokens[position].value != "\n") {
                position++;
            }
            pImpl->skipWhitespace(tokens, position);
            continue;
        }
        
        // 解析配置项
        ConfigItemInfo config_item = parseConfigItem(tokens, position);
        if (!config_item.name.empty()) {
            config_items.push_back(config_item);
        }
        
        pImpl->skipWhitespace(tokens, position);
    }
    
    return config_items;
}

ConfigItemInfo ConfigurationSystem::parseConfigItem(const std::vector<Token>& tokens, size_t& position) {
    ConfigItemInfo config_item;
    
    // 解析配置项名称
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        config_item.name = tokens[position].value;
        config_item.line = tokens[position].line;
        config_item.column = tokens[position].column;
        position++;
    } else {
        addError("配置项缺少名称");
        return config_item;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查等号
    if (position >= tokens.size() || tokens[position].value != "=") {
        addError("配置项缺少等号");
        return config_item;
    }
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析配置值
    config_item.value = parseConfigValue(tokens, position);
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查分号
    if (position < tokens.size() && tokens[position].value == ";") {
        position++;
    }
    
    return config_item;
}

std::vector<KeywordConfigInfo> ConfigurationSystem::parseNameGroup(const std::vector<Token>& tokens, size_t& position) {
    std::vector<KeywordConfigInfo> keyword_configs;
    
    while (position < tokens.size() && tokens[position].value != "}") {
        // 跳过注释
        if (tokens[position].value == "//") {
            while (position < tokens.size() && tokens[position].value != "\n") {
                position++;
            }
            pImpl->skipWhitespace(tokens, position);
            continue;
        }
        
        // 解析关键字配置
        KeywordConfigInfo keyword_config = parseKeywordConfig(tokens, position);
        if (!keyword_config.name.empty()) {
            keyword_configs.push_back(keyword_config);
        }
        
        pImpl->skipWhitespace(tokens, position);
    }
    
    return keyword_configs;
}

KeywordConfigInfo ConfigurationSystem::parseKeywordConfig(const std::vector<Token>& tokens, size_t& position) {
    KeywordConfigInfo keyword_config;
    
    // 解析关键字名称
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        keyword_config.name = tokens[position].value;
        keyword_config.line = tokens[position].line;
        keyword_config.column = tokens[position].column;
        position++;
    } else {
        addError("关键字配置缺少名称");
        return keyword_config;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查等号
    if (position >= tokens.size() || tokens[position].value != "=") {
        addError("关键字配置缺少等号");
        return keyword_config;
    }
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查是否为组选项
    if (position < tokens.size() && tokens[position].value == "[") {
        position++;
        keyword_config.is_group_option = true;
        keyword_config.values = parseGroupOptions(tokens, position);
        
        // 检查右方括号
        if (position < tokens.size() && tokens[position].value == "]") {
            position++;
        }
    } else {
        // 单个值
        keyword_config.is_group_option = false;
        keyword_config.single_value = parseStringValue(tokens, position);
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查分号
    if (position < tokens.size() && tokens[position].value == ";") {
        position++;
    }
    
    return keyword_config;
}

std::vector<std::string> ConfigurationSystem::parseGroupOptions(const std::vector<Token>& tokens, size_t& position) {
    std::vector<std::string> options;
    
    while (position < tokens.size() && tokens[position].value != "]") {
        // 跳过空白字符
        pImpl->skipWhitespace(tokens, position);
        
        // 解析选项值
        std::string option = parseStringValue(tokens, position);
        if (!option.empty()) {
            options.push_back(option);
        }
        
        // 跳过空白字符
        pImpl->skipWhitespace(tokens, position);
        
        // 检查逗号
        if (position < tokens.size() && tokens[position].value == ",") {
            position++;
        }
    }
    
    return options;
}

ConfigValue ConfigurationSystem::parseConfigValue(const std::vector<Token>& tokens, size_t& position) {
    if (position >= tokens.size()) {
        return std::string("");
    }
    
    const Token& token = tokens[position];
    
    // 尝试解析为整数
    try {
        int int_value = std::stoi(token.value);
        return int_value;
    } catch (...) {
        // 不是整数
    }
    
    // 尝试解析为布尔值
    if (token.value == "true") {
        return true;
    } else if (token.value == "false") {
        return false;
    }
    
    // 尝试解析为浮点数
    try {
        double double_value = std::stod(token.value);
        return double_value;
    } catch (...) {
        // 不是浮点数
    }
    
    // 默认为字符串
    return token.value;
}

std::string ConfigurationSystem::parseStringValue(const std::vector<Token>& tokens, size_t& position) {
    if (position >= tokens.size()) {
        return "";
    }
    
    const Token& token = tokens[position];
    
    // 如果是字符串字面量，去掉引号
    if (token.type == TokenType::STRING_LITERAL) {
        std::string value = token.value;
        if (value.length() >= 2 && value[0] == '"' && value[value.length() - 1] == '"') {
            value = value.substr(1, value.length() - 2);
        }
        position++;
        return value;
    }
    
    // 否则直接返回标识符值
    position++;
    return token.value;
}

int ConfigurationSystem::parseIntegerValue(const std::vector<Token>& tokens, size_t& position) {
    if (position >= tokens.size()) {
        return 0;
    }
    
    const Token& token = tokens[position];
    try {
        int value = std::stoi(token.value);
        position++;
        return value;
    } catch (...) {
        addError("无法解析整数值: " + token.value);
        return 0;
    }
}

bool ConfigurationSystem::parseBooleanValue(const std::vector<Token>& tokens, size_t& position) {
    if (position >= tokens.size()) {
        return false;
    }
    
    const Token& token = tokens[position];
    if (token.value == "true") {
        position++;
        return true;
    } else if (token.value == "false") {
        position++;
        return false;
    } else {
        addError("无法解析布尔值: " + token.value);
        return false;
    }
}

double ConfigurationSystem::parseFloatValue(const std::vector<Token>& tokens, size_t& position) {
    if (position >= tokens.size()) {
        return 0.0;
    }
    
    const Token& token = tokens[position];
    try {
        double value = std::stod(token.value);
        position++;
        return value;
    } catch (...) {
        addError("无法解析浮点数值: " + token.value);
        return 0.0;
    }
}

void ConfigurationSystem::addConfigurationGroup(const ConfigurationGroupInfo& config_group) {
    std::string group_name = config_group.name;
    if (group_name.empty()) {
        group_name = "default";
    }
    
    pImpl->configuration_groups[group_name] = config_group;
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("添加配置组: " + group_name);
    }
}

void ConfigurationSystem::removeConfigurationGroup(const std::string& name) {
    pImpl->configuration_groups.erase(name);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("删除配置组: " + name);
    }
}

ConfigurationGroupInfo* ConfigurationSystem::getConfigurationGroup(const std::string& name) {
    auto it = pImpl->configuration_groups.find(name);
    if (it != pImpl->configuration_groups.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<ConfigurationGroupInfo> ConfigurationSystem::getAllConfigurationGroups() const {
    std::vector<ConfigurationGroupInfo> result;
    for (const auto& [name, info] : pImpl->configuration_groups) {
        result.push_back(info);
    }
    return result;
}

bool ConfigurationSystem::hasConfigurationGroup(const std::string& name) const {
    return pImpl->configuration_groups.find(name) != pImpl->configuration_groups.end();
}

void ConfigurationSystem::addConfigItem(const std::string& group_name, const ConfigItemInfo& config_item) {
    auto* group = getConfigurationGroup(group_name);
    if (group) {
        group->config_items.push_back(config_item);
        
        if (pImpl->debug_mode) {
            pImpl->logDebug("添加配置项: " + config_item.name + " 到组: " + group_name);
        }
    }
}

void ConfigurationSystem::removeConfigItem(const std::string& group_name, const std::string& item_name) {
    auto* group = getConfigurationGroup(group_name);
    if (group) {
        group->config_items.erase(
            std::remove_if(group->config_items.begin(), group->config_items.end(),
                [&item_name](const ConfigItemInfo& item) { return item.name == item_name; }),
            group->config_items.end()
        );
        
        if (pImpl->debug_mode) {
            pImpl->logDebug("删除配置项: " + item_name + " 从组: " + group_name);
        }
    }
}

ConfigItemInfo* ConfigurationSystem::getConfigItem(const std::string& group_name, const std::string& item_name) {
    auto* group = getConfigurationGroup(group_name);
    if (group) {
        for (auto& item : group->config_items) {
            if (item.name == item_name) {
                return &item;
            }
        }
    }
    return nullptr;
}

std::vector<ConfigItemInfo> ConfigurationSystem::getConfigItems(const std::string& group_name) const {
    auto it = pImpl->configuration_groups.find(group_name);
    if (it != pImpl->configuration_groups.end()) {
        return it->second.config_items;
    }
    return {};
}

void ConfigurationSystem::addKeywordConfig(const std::string& group_name, const KeywordConfigInfo& keyword_config) {
    auto* group = getConfigurationGroup(group_name);
    if (group) {
        group->keyword_configs.push_back(keyword_config);
        
        if (pImpl->debug_mode) {
            pImpl->logDebug("添加关键字配置: " + keyword_config.name + " 到组: " + group_name);
        }
    }
}

void ConfigurationSystem::removeKeywordConfig(const std::string& group_name, const std::string& keyword_name) {
    auto* group = getConfigurationGroup(group_name);
    if (group) {
        group->keyword_configs.erase(
            std::remove_if(group->keyword_configs.begin(), group->keyword_configs.end(),
                [&keyword_name](const KeywordConfigInfo& config) { return config.name == keyword_name; }),
            group->keyword_configs.end()
        );
        
        if (pImpl->debug_mode) {
            pImpl->logDebug("删除关键字配置: " + keyword_name + " 从组: " + group_name);
        }
    }
}

KeywordConfigInfo* ConfigurationSystem::getKeywordConfig(const std::string& group_name, const std::string& keyword_name) {
    auto* group = getConfigurationGroup(group_name);
    if (group) {
        for (auto& config : group->keyword_configs) {
            if (config.name == keyword_name) {
                return &config;
            }
        }
    }
    return nullptr;
}

std::vector<KeywordConfigInfo> ConfigurationSystem::getKeywordConfigs(const std::string& group_name) const {
    auto it = pImpl->configuration_groups.find(group_name);
    if (it != pImpl->configuration_groups.end()) {
        return it->second.keyword_configs;
    }
    return {};
}

bool ConfigurationSystem::validateConfigurationGroup(const ConfigurationGroupInfo& config_group) {
    // 验证配置项
    for (const auto& config_item : config_group.config_items) {
        if (!validateConfigItem(config_item)) {
            return false;
        }
    }
    
    // 验证关键字配置
    for (const auto& keyword_config : config_group.keyword_configs) {
        if (!validateKeywordConfig(keyword_config)) {
            return false;
        }
    }
    
    return true;
}

bool ConfigurationSystem::validateConfigItem(const ConfigItemInfo& config_item) {
    // 验证配置项名称
    if (!pImpl->isValidConfigName(config_item.name)) {
        addError("无效的配置项名称: " + config_item.name);
        return false;
    }
    
    // 验证配置值
    if (!pImpl->isValidConfigValue(config_item.value)) {
        addError("无效的配置值: " + config_item.name);
        return false;
    }
    
    return true;
}

bool ConfigurationSystem::validateKeywordConfig(const KeywordConfigInfo& keyword_config) {
    // 验证关键字名称
    if (!pImpl->isValidKeywordName(keyword_config.name)) {
        addError("无效的关键字名称: " + keyword_config.name);
        return false;
    }
    
    // 验证组选项数量
    if (keyword_config.is_group_option) {
        if (!pImpl->isValidGroupOptionCount(keyword_config.values.size())) {
            addError("无效的组选项数量: " + keyword_config.name);
            return false;
        }
    }
    
    return true;
}

std::vector<std::string> ConfigurationSystem::validateConfigurationList(const std::vector<ConfigurationGroupInfo>& config_groups) {
    std::vector<std::string> errors;
    
    for (const auto& config_group : config_groups) {
        if (!validateConfigurationGroup(config_group)) {
            auto group_errors = getErrors();
            errors.insert(errors.end(), group_errors.begin(), group_errors.end());
        }
    }
    
    return errors;
}

void ConfigurationSystem::applyConfiguration(const std::string& group_name) {
    auto* group = getConfigurationGroup(group_name);
    if (!group) {
        addError("配置组不存在: " + group_name);
        return;
    }
    
    // 应用所有配置项
    for (const auto& config_item : group->config_items) {
        applyConfigItem(group_name, config_item.name);
    }
    
    // 应用所有关键字配置
    for (const auto& keyword_config : group->keyword_configs) {
        applyKeywordConfig(group_name, keyword_config.name);
    }
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("应用配置组: " + group_name);
    }
}

void ConfigurationSystem::applyConfigItem(const std::string& group_name, const std::string& item_name) {
    auto* config_item = getConfigItem(group_name, item_name);
    if (!config_item) {
        addError("配置项不存在: " + item_name + " 在组: " + group_name);
        return;
    }
    
    pImpl->updateSystemConfig(*config_item);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("应用配置项: " + item_name + " 从组: " + group_name);
    }
}

void ConfigurationSystem::applyKeywordConfig(const std::string& group_name, const std::string& keyword_name) {
    auto* keyword_config = getKeywordConfig(group_name, keyword_name);
    if (!keyword_config) {
        addError("关键字配置不存在: " + keyword_name + " 在组: " + group_name);
        return;
    }
    
    pImpl->updateKeywordMappings(*keyword_config);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("应用关键字配置: " + keyword_name + " 从组: " + group_name);
    }
}

ConfigValue ConfigurationSystem::getConfigValue(const std::string& group_name, const std::string& item_name) {
    auto* config_item = getConfigItem(group_name, item_name);
    if (config_item) {
        return config_item->value;
    }
    return std::string("");
}

std::vector<std::string> ConfigurationSystem::getKeywordValues(const std::string& group_name, const std::string& keyword_name) {
    auto* keyword_config = getKeywordConfig(group_name, keyword_name);
    if (keyword_config && keyword_config->is_group_option) {
        return keyword_config->values;
    }
    return {};
}

std::string ConfigurationSystem::getKeywordValue(const std::string& group_name, const std::string& keyword_name) {
    auto* keyword_config = getKeywordConfig(group_name, keyword_name);
    if (keyword_config && !keyword_config->is_group_option) {
        return keyword_config->single_value;
    }
    return "";
}

void ConfigurationSystem::setConfig(const ConfigurationSystemConfig& config) {
    pImpl->config = config;
}

ConfigurationSystemConfig ConfigurationSystem::getConfig() const {
    return pImpl->config;
}

bool ConfigurationSystem::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> ConfigurationSystem::getErrors() const {
    return pImpl->errors;
}

void ConfigurationSystem::clearErrors() {
    pImpl->errors.clear();
}

void ConfigurationSystem::addError(const std::string& error_message) {
    pImpl->errors.push_back(error_message);
    
    if (pImpl->debug_mode) {
        std::cerr << "配置组系统错误: " << error_message << std::endl;
    }
}

std::string ConfigurationSystem::getDebugInfo() const {
    std::ostringstream oss;
    oss << "配置组系统调试信息:\n";
    oss << "配置组总数: " << pImpl->configuration_groups.size() << "\n";
    oss << "配置组: " << (pImpl->config.enable_configuration_groups ? "启用" : "禁用") << "\n";
    oss << "名称配置组: " << (pImpl->config.enable_name_groups ? "启用" : "禁用") << "\n";
    oss << "严格验证: " << (pImpl->config.strict_validation ? "启用" : "禁用") << "\n";
    oss << "自动解析关键字: " << (pImpl->config.auto_resolve_keywords ? "启用" : "禁用") << "\n";
    oss << "错误数量: " << pImpl->errors.size() << "\n";
    
    for (const auto& [name, group] : pImpl->configuration_groups) {
        oss << "  配置组: " << name << " (配置项: " << group.config_items.size() 
            << ", 关键字配置: " << group.keyword_configs.size() << ")\n";
    }
    
    return oss.str();
}

void ConfigurationSystem::enableDebugMode(bool enable) {
    pImpl->debug_mode = enable;
}

void ConfigurationSystem::setStateMachine(std::shared_ptr<CHTLStateMachine> sm) {
    state_machine = sm;
}

// Impl类方法实现
bool ConfigurationSystem::Impl::isConfigurationDeclaration(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return false;
    }
    
    const Token& token = tokens[position];
    return token.value == "[Configuration]";
}

bool ConfigurationSystem::Impl::isNameGroupDeclaration(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return false;
    }
    
    const Token& token = tokens[position];
    return token.value == "[Name]";
}

void ConfigurationSystem::Impl::skipWhitespace(const std::vector<Token>& tokens, size_t& position) {
    while (position < tokens.size() && 
           (tokens[position].value == " " || tokens[position].value == "\t" || 
            tokens[position].value == "\n" || tokens[position].value == "\r")) {
        position++;
    }
}

bool ConfigurationSystem::Impl::isEndOfTokens(const std::vector<Token>& tokens, size_t position) {
    return position >= tokens.size();
}

bool ConfigurationSystem::Impl::isValidConfigName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 检查长度限制
    if (name.length() > config.max_config_name_length) {
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

bool ConfigurationSystem::Impl::isValidKeywordName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 检查长度限制
    if (name.length() > config.max_keyword_value_length) {
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

bool ConfigurationSystem::Impl::isValidConfigValue(const ConfigValue& value) {
    return !std::holds_alternative<std::string>(value) || !std::get<std::string>(value).empty();
}

bool ConfigurationSystem::Impl::isValidGroupOptionCount(size_t count) {
    return count <= config.max_group_option_count;
}

void ConfigurationSystem::Impl::updateSystemConfig(const ConfigItemInfo& config_item) {
    // 这里实现具体的系统配置更新逻辑
    if (debug_mode) {
        logDebug("更新系统配置: " + config_item.name);
    }
}

void ConfigurationSystem::Impl::updateKeywordMappings(const KeywordConfigInfo& keyword_config) {
    // 这里实现具体的关键字映射更新逻辑
    if (debug_mode) {
        logDebug("更新关键字映射: " + keyword_config.name);
    }
}

void ConfigurationSystem::Impl::validateKeywordConsistency() {
    // 这里实现关键字一致性验证逻辑
    if (debug_mode) {
        logDebug("验证关键字一致性");
    }
}

std::string ConfigurationSystem::Impl::extractConfigurationContent(const std::vector<Token>& tokens, size_t& position) {
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
    
    return content;
}

std::string ConfigurationSystem::Impl::parseConfigurationIdentifier(const std::vector<Token>& tokens, size_t& position) {
    std::string identifier;
    
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        identifier = tokens[position].value;
        position++;
    }
    
    return identifier;
}

KeywordType ConfigurationSystem::Impl::detectKeywordType(const std::string& name) {
    // 根据名称检测关键字类型
    if (name == "CUSTOM_STYLE") return KeywordType::CUSTOM_STYLE;
    if (name == "CUSTOM_ELEMENT") return KeywordType::CUSTOM_ELEMENT;
    if (name == "CUSTOM_VAR") return KeywordType::CUSTOM_VAR;
    if (name == "TEMPLATE_STYLE") return KeywordType::TEMPLATE_STYLE;
    if (name == "TEMPLATE_ELEMENT") return KeywordType::TEMPLATE_ELEMENT;
    if (name == "TEMPLATE_VAR") return KeywordType::TEMPLATE_VAR;
    if (name == "ORIGIN_HTML") return KeywordType::ORIGIN_HTML;
    if (name == "ORIGIN_STYLE") return KeywordType::ORIGIN_STYLE;
    if (name == "ORIGIN_JAVASCRIPT") return KeywordType::ORIGIN_JAVASCRIPT;
    if (name == "IMPORT_HTML") return KeywordType::IMPORT_HTML;
    if (name == "IMPORT_STYLE") return KeywordType::IMPORT_STYLE;
    if (name == "IMPORT_JAVASCRIPT") return KeywordType::IMPORT_JAVASCRIPT;
    if (name == "IMPORT_CHTL") return KeywordType::IMPORT_CHTL;
    if (name == "IMPORT_CJMOD") return KeywordType::IMPORT_CJMOD;
    if (name == "KEYWORD_INHERIT") return KeywordType::KEYWORD_INHERIT;
    if (name == "KEYWORD_DELETE") return KeywordType::KEYWORD_DELETE;
    if (name == "KEYWORD_INSERT") return KeywordType::KEYWORD_INSERT;
    if (name == "KEYWORD_AFTER") return KeywordType::KEYWORD_AFTER;
    if (name == "KEYWORD_BEFORE") return KeywordType::KEYWORD_BEFORE;
    if (name == "KEYWORD_REPLACE") return KeywordType::KEYWORD_REPLACE;
    if (name == "KEYWORD_ATTOP") return KeywordType::KEYWORD_ATTOP;
    if (name == "KEYWORD_ATBOTTOM") return KeywordType::KEYWORD_ATBOTTOM;
    if (name == "KEYWORD_FROM") return KeywordType::KEYWORD_FROM;
    if (name == "KEYWORD_AS") return KeywordType::KEYWORD_AS;
    if (name == "KEYWORD_EXCEPT") return KeywordType::KEYWORD_EXCEPT;
    if (name == "KEYWORD_TEXT") return KeywordType::KEYWORD_TEXT;
    if (name == "KEYWORD_STYLE") return KeywordType::KEYWORD_STYLE;
    if (name == "KEYWORD_SCRIPT") return KeywordType::KEYWORD_SCRIPT;
    if (name == "KEYWORD_CUSTOM") return KeywordType::KEYWORD_CUSTOM;
    if (name == "KEYWORD_TEMPLATE") return KeywordType::KEYWORD_TEMPLATE;
    if (name == "KEYWORD_ORIGIN") return KeywordType::KEYWORD_ORIGIN;
    if (name == "KEYWORD_IMPORT") return KeywordType::KEYWORD_IMPORT;
    if (name == "KEYWORD_NAMESPACE") return KeywordType::KEYWORD_NAMESPACE;
    
    return KeywordType::CUSTOM_STYLE; // 默认类型
}

void ConfigurationSystem::Impl::logDebug(const std::string& message) {
    if (debug_mode) {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}

} // namespace chtl