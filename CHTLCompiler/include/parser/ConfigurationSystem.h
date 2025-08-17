#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include "ast/ASTNode.h"
#include "lexer/CHTLLexer.h"
#include "parser/AttributeSystem.h"
#include "statemachine/CHTLStateMachine.h"

namespace chtl {

// 配置值类型
using ConfigValue = std::variant<std::string, int, bool, double>;

// 配置项类型枚举
enum class ConfigItemType {
    INDEX_INITIAL_COUNT,      // 索引的起始计数
    DISABLE_NAME_GROUP,       // 是否禁用Name配置组
    DEBUG_MODE,               // DEBUG模式
    OPTION_COUNT              // 组选项的数量限制
};

// 关键字类型枚举
enum class KeywordType {
    CUSTOM_STYLE,             // 自定义样式组关键字
    CUSTOM_ELEMENT,           // 自定义元素关键字
    CUSTOM_VAR,               // 自定义变量组关键字
    TEMPLATE_STYLE,           // 模板样式组关键字
    TEMPLATE_ELEMENT,         // 模板元素关键字
    TEMPLATE_VAR,             // 模板变量组关键字
    ORIGIN_HTML,              // 原始HTML关键字
    ORIGIN_STYLE,             // 原始CSS关键字
    ORIGIN_JAVASCRIPT,        // 原始JavaScript关键字
    IMPORT_HTML,              // 导入HTML关键字
    IMPORT_STYLE,             // 导入CSS关键字
    IMPORT_JAVASCRIPT,        // 导入JavaScript关键字
    IMPORT_CHTL,              // 导入CHTL关键字
    IMPORT_CJMOD,             // 导入CJmod关键字
    KEYWORD_INHERIT,          // 继承关键字
    KEYWORD_DELETE,           // 删除关键字
    KEYWORD_INSERT,           // 插入关键字
    KEYWORD_AFTER,            // after关键字
    KEYWORD_BEFORE,           // before关键字
    KEYWORD_REPLACE,          // replace关键字
    KEYWORD_ATTOP,            // at top关键字
    KEYWORD_ATBOTTOM,         // at bottom关键字
    KEYWORD_FROM,             // from关键字
    KEYWORD_AS,               // as关键字
    KEYWORD_EXCEPT,           // except关键字
    KEYWORD_TEXT,             // text关键字
    KEYWORD_STYLE,            // style关键字
    KEYWORD_SCRIPT,           // script关键字
    KEYWORD_CUSTOM,           // custom关键字
    KEYWORD_TEMPLATE,         // template关键字
    KEYWORD_ORIGIN,           // origin关键字
    KEYWORD_IMPORT,           // import关键字
    KEYWORD_NAMESPACE         // namespace关键字
};

// 配置项信息结构
struct ConfigItemInfo {
    std::string name;                      // 配置项名称
    ConfigItemType type;                   // 配置项类型
    ConfigValue value;                     // 配置值
    size_t line;                           // 行号
    size_t column;                         // 列号
    std::string description;               // 描述信息
    
    ConfigItemInfo() : type(ConfigItemType::DEBUG_MODE), line(0), column(0) {}
};

// 关键字配置信息结构
struct KeywordConfigInfo {
    std::string name;                      // 关键字名称
    KeywordType type;                      // 关键字类型
    std::vector<std::string> values;       // 关键字值列表（用于组选项）
    std::string single_value;              // 单个关键字值
    size_t line;                           // 行号
    size_t column;                         // 列号
    bool is_group_option;                  // 是否为组选项
    
    KeywordConfigInfo() : type(KeywordType::CUSTOM_STYLE), line(0), column(0), is_group_option(false) {}
};

// 配置组信息结构
struct ConfigurationGroupInfo {
    std::string name;                      // 配置组名称
    std::vector<ConfigItemInfo> config_items; // 配置项列表
    std::vector<KeywordConfigInfo> keyword_configs; // 关键字配置列表
    size_t line;                           // 行号
    size_t column;                         // 列号
    
    ConfigurationGroupInfo() : line(0), column(0) {}
};

// 配置组系统配置
struct ConfigurationSystemConfig {
    bool enable_configuration_groups;       // 启用配置组
    bool enable_name_groups;               // 启用名称配置组
    bool strict_validation;                // 严格验证
    bool auto_resolve_keywords;            // 自动解析关键字
    size_t max_config_name_length;         // 最大配置名称长度
    size_t max_keyword_value_length;       // 最大关键字值长度
    size_t max_group_option_count;         // 最大组选项数量
    bool enable_debug_mode;                // 启用调试模式
    
    ConfigurationSystemConfig() : 
        enable_configuration_groups(true), enable_name_groups(true),
        strict_validation(false), auto_resolve_keywords(true),
        max_config_name_length(100), max_keyword_value_length(200),
        max_group_option_count(10), enable_debug_mode(false) {}
};

// 配置组系统解析器类
class ConfigurationSystem {
public:
    ConfigurationSystem(const ConfigurationSystemConfig& config = ConfigurationSystemConfig());
    ~ConfigurationSystem();
    
    // 配置组声明解析
    std::shared_ptr<ASTNode> parseConfigurationDeclaration(const std::vector<Token>& tokens, size_t& position);
    ConfigurationGroupInfo parseConfigurationGroup(const std::vector<Token>& tokens, size_t& position);
    std::vector<ConfigItemInfo> parseConfigItems(const std::vector<Token>& tokens, size_t& position);
    ConfigItemInfo parseConfigItem(const std::vector<Token>& tokens, size_t& position);
    
    // 名称配置组解析
    std::vector<KeywordConfigInfo> parseNameGroup(const std::vector<Token>& tokens, size_t& position);
    KeywordConfigInfo parseKeywordConfig(const std::vector<Token>& tokens, size_t& position);
    std::vector<std::string> parseGroupOptions(const std::vector<Token>& tokens, size_t& position);
    
    // 配置值解析
    ConfigValue parseConfigValue(const std::vector<Token>& tokens, size_t& position);
    std::string parseStringValue(const std::vector<Token>& tokens, size_t& position);
    int parseIntegerValue(const std::vector<Token>& tokens, size_t& position);
    bool parseBooleanValue(const std::vector<Token>& tokens, size_t& position);
    double parseFloatValue(const std::vector<Token>& tokens, size_t& position);
    
    // 配置组管理
    void addConfigurationGroup(const ConfigurationGroupInfo& config_group);
    void removeConfigurationGroup(const std::string& name);
    ConfigurationGroupInfo* getConfigurationGroup(const std::string& name);
    std::vector<ConfigurationGroupInfo> getAllConfigurationGroups() const;
    bool hasConfigurationGroup(const std::string& name) const;
    
    // 配置项管理
    void addConfigItem(const std::string& group_name, const ConfigItemInfo& config_item);
    void removeConfigItem(const std::string& group_name, const std::string& item_name);
    ConfigItemInfo* getConfigItem(const std::string& group_name, const std::string& item_name);
    std::vector<ConfigItemInfo> getConfigItems(const std::string& group_name) const;
    
    // 关键字配置管理
    void addKeywordConfig(const std::string& group_name, const KeywordConfigInfo& keyword_config);
    void removeKeywordConfig(const std::string& group_name, const std::string& keyword_name);
    KeywordConfigInfo* getKeywordConfig(const std::string& group_name, const std::string& keyword_name);
    std::vector<KeywordConfigInfo> getKeywordConfigs(const std::string& group_name) const;
    
    // 配置验证
    bool validateConfigurationGroup(const ConfigurationGroupInfo& config_group);
    bool validateConfigItem(const ConfigItemInfo& config_item);
    bool validateKeywordConfig(const KeywordConfigInfo& keyword_config);
    std::vector<std::string> validateConfigurationList(const std::vector<ConfigurationGroupInfo>& config_groups);
    
    // 配置应用
    void applyConfiguration(const std::string& group_name);
    void applyConfigItem(const std::string& group_name, const std::string& item_name);
    void applyKeywordConfig(const std::string& group_name, const std::string& keyword_name);
    
    // 配置查询
    ConfigValue getConfigValue(const std::string& group_name, const std::string& item_name);
    std::vector<std::string> getKeywordValues(const std::string& group_name, const std::string& keyword_name);
    std::string getKeywordValue(const std::string& group_name, const std::string& keyword_name);
    
    // 配置管理
    void setConfig(const ConfigurationSystemConfig& config);
    ConfigurationSystemConfig getConfig() const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error_message);
    
    // 调试和诊断
    std::string getDebugInfo() const;
    void enableDebugMode(bool enable);
    
    // 状态机管理
    void setStateMachine(std::shared_ptr<CHTLStateMachine> state_machine);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
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
    
    // 状态机管理
    std::shared_ptr<CHTLStateMachine> state_machine;
};

} // namespace chtl