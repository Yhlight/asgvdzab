#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "ast/ASTNode.h"
#include "ast/AttributeNode.h"
#include "lexer/CHTLLexer.h"

namespace chtl {

// 属性值类型枚举
enum class AttributeValueType {
    STRING_LITERAL,      // 字符串字面量
    UNQUOTED_LITERAL,    // 无修饰字面量
    NUMERIC_LITERAL,     // 数值字面量
    BOOLEAN_LITERAL,     // 布尔字面量
    EXPRESSION,          // 表达式
    TEMPLATE_REFERENCE,  // 模板引用
    FUNCTION_CALL        // 函数调用
};

// 属性验证规则
enum class AttributeValidationRule {
    REQUIRED,            // 必需属性
    OPTIONAL,            // 可选属性
    UNIQUE,              // 唯一属性
    PATTERN_MATCH,       // 模式匹配
    VALUE_RANGE,         // 值范围
    ENUM_VALUES,         // 枚举值
    CUSTOM_VALIDATOR     // 自定义验证器
};

// 属性信息结构
struct AttributeInfo {
    std::string name;                    // 属性名
    std::string value;                   // 属性值
    AttributeValueType value_type;       // 值类型
    std::string original_value;          // 原始值
    size_t line;                         // 行号
    size_t column;                       // 列号
    std::vector<AttributeValidationRule> validation_rules;  // 验证规则
    std::unordered_map<std::string, std::string> metadata; // 元数据
    
    AttributeInfo() : value_type(AttributeValueType::STRING_LITERAL), line(0), column(0) {}
};

// 属性系统配置
struct AttributeSystemConfig {
    bool enable_ce_equivalence;          // 启用CE等价性
    bool enable_unquoted_literals;       // 启用无修饰字面量
    bool strict_validation;              // 严格验证模式
    bool allow_duplicate_attributes;     // 允许重复属性
    bool enable_attribute_inheritance;   // 启用属性继承
    size_t max_attribute_name_length;    // 最大属性名长度
    size_t max_attribute_value_length;   // 最大属性值长度
    
    AttributeSystemConfig() : 
        enable_ce_equivalence(true), enable_unquoted_literals(true), 
        strict_validation(false), allow_duplicate_attributes(false),
        enable_attribute_inheritance(true), max_attribute_name_length(100),
        max_attribute_value_length(1000) {}
};

// 属性系统管理器类
class AttributeSystem {
public:
    AttributeSystem(const AttributeSystemConfig& config = AttributeSystemConfig());
    ~AttributeSystem();
    
    // 属性解析
    std::shared_ptr<AttributeNode> parseAttribute(const std::vector<Token>& tokens, size_t& position);
    std::vector<std::shared_ptr<AttributeNode>> parseAttributeList(const std::vector<Token>& tokens, size_t& position);
    
    // 属性值处理
    std::string normalizeAttributeValue(const std::string& value, AttributeValueType type);
    AttributeValueType detectAttributeValueType(const std::string& value);
    std::string escapeAttributeValue(const std::string& value);
    std::string unescapeAttributeValue(const std::string& value);
    
    // 属性验证
    bool validateAttribute(const AttributeInfo& attr_info);
    std::vector<std::string> validateAttributeList(const std::vector<AttributeInfo>& attrs);
    bool validateAttributeName(const std::string& name);
    bool validateAttributeValue(const std::string& value, AttributeValueType type);
    
    // 属性管理
    void addAttribute(std::shared_ptr<AttributeNode> attr);
    void removeAttribute(const std::string& name);
    std::shared_ptr<AttributeNode> getAttribute(const std::string& name) const;
    std::vector<std::shared_ptr<AttributeNode>> getAttributes() const;
    bool hasAttribute(const std::string& name) const;
    
    // 属性继承和覆盖
    void inheritAttributes(const std::vector<std::shared_ptr<AttributeNode>>& parent_attrs);
    void overrideAttribute(const std::string& name, const std::string& new_value);
    std::vector<std::shared_ptr<AttributeNode>> getInheritedAttributes() const;
    
    // 属性转换
    std::string convertToHTML(const std::vector<std::shared_ptr<AttributeNode>>& attrs);
    std::string convertToCSS(const std::vector<std::shared_ptr<AttributeNode>>& attrs);
    std::string convertToJavaScript(const std::vector<std::shared_ptr<AttributeNode>>& attrs);
    
    // 配置管理
    void setConfig(const AttributeSystemConfig& config);
    AttributeSystemConfig getConfig() const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error_message);
    
    // 调试和诊断
    std::string getDebugInfo() const;
    void enableDebugMode(bool enable);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
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
};

} // namespace chtl