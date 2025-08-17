#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "ast/ASTNode.h"
#include "lexer/CHTLLexer.h"
#include "parser/AttributeSystem.h"
#include "parser/TemplateSystem.h"
#include "statemachine/CHTLStateMachine.h"

namespace chtl {

// 自定义类型枚举
enum class CustomType {
    STYLE_CUSTOM,        // @Style 自定义样式组
    ELEMENT_CUSTOM,      // @Element 自定义元素
    VAR_CUSTOM           // @Var 自定义变量组
};

// 特例化操作类型
enum class SpecializationType {
    DELETE_PROPERTY,     // 删除属性
    DELETE_INHERITANCE,  // 删除继承
    INSERT_ELEMENT,      // 插入元素
    REPLACE_ELEMENT,     // 替换元素
    MODIFY_VALUE,        // 修改值
    ADD_STYLE,           // 添加样式
    INDEX_ACCESS         // 索引访问
};

// 插入位置枚举
enum class InsertPosition {
    AFTER,               // after
    BEFORE,              // before
    REPLACE,             // replace
    AT_TOP,              // at top
    AT_BOTTOM            // at bottom
};

// 特例化操作信息
struct SpecializationInfo {
    SpecializationType type;
    std::string target;              // 目标（属性名、元素名、索引等）
    std::string value;               // 新值（用于修改值）
    InsertPosition insert_pos;        // 插入位置
    std::string insert_content;       // 插入内容
    size_t line;                      // 行号
    size_t column;                    // 列号
    
    SpecializationInfo() : type(SpecializationType::DELETE_PROPERTY), 
                           insert_pos(InsertPosition::AFTER), line(0), column(0) {}
};

// 自定义信息结构
struct CustomInfo {
    std::string name;                      // 自定义名称
    CustomType type;                       // 自定义类型
    std::string base_template;             // 基础模板名称
    std::vector<SpecializationInfo> specializations; // 特例化操作列表
    std::string custom_content;            // 自定义内容
    size_t line;                           // 行号
    size_t column;                         // 列号
    std::unordered_map<std::string, std::string> metadata; // 元数据
    
    CustomInfo() : type(CustomType::STYLE_CUSTOM), line(0), column(0) {}
};

// 自定义系统配置
struct CustomSystemConfig {
    bool enable_style_customs;             // 启用自定义样式组
    bool enable_element_customs;           // 启用自定义元素
    bool enable_var_customs;               // 启用自定义变量组
    bool enable_specialization;            // 启用特例化操作
    bool strict_validation;                // 严格验证
    bool auto_resolve_base_templates;      // 自动解析基础模板
    size_t max_custom_name_length;         // 最大自定义名称长度
    size_t max_specialization_depth;       // 最大特例化深度
    bool enable_index_access;              // 启用索引访问
    bool enable_insert_operations;         // 启用插入操作
    
    CustomSystemConfig() : 
        enable_style_customs(true), enable_element_customs(true), 
        enable_var_customs(true), enable_specialization(true),
        strict_validation(false), auto_resolve_base_templates(true),
        max_custom_name_length(100), max_specialization_depth(5),
        enable_index_access(true), enable_insert_operations(true) {}
};

// 自定义系统解析器类
class CustomSystem {
public:
    CustomSystem(const CustomSystemConfig& config = CustomSystemConfig());
    ~CustomSystem();
    
    // 自定义声明解析
    std::shared_ptr<ASTNode> parseCustomDeclaration(const std::vector<Token>& tokens, size_t& position);
    CustomInfo parseStyleCustom(const std::vector<Token>& tokens, size_t& position);
    CustomInfo parseElementCustom(const std::vector<Token>& tokens, size_t& position);
    CustomInfo parseVarCustom(const std::vector<Token>& tokens, size_t& position);
    
    // 特例化操作解析
    std::vector<SpecializationInfo> parseSpecializations(const std::vector<Token>& tokens, size_t& position);
    SpecializationInfo parseDeleteProperty(const std::vector<Token>& tokens, size_t& position);
    SpecializationInfo parseDeleteInheritance(const std::vector<Token>& tokens, size_t& position);
    SpecializationInfo parseInsertElement(const std::vector<Token>& tokens, size_t& position);
    SpecializationInfo parseModifyValue(const std::vector<Token>& tokens, size_t& position);
    SpecializationInfo parseIndexAccess(const std::vector<Token>& tokens, size_t& position);
    
    // 自定义使用解析
    std::shared_ptr<ASTNode> parseCustomUsage(const std::vector<Token>& tokens, size_t& position);
    std::string parseIndexSelector(const std::vector<Token>& tokens, size_t& position);
    std::string parseInsertPosition(const std::vector<Token>& tokens, size_t& position);
    
    // 自定义管理
    void addCustom(const CustomInfo& custom_info);
    void removeCustom(const std::string& name);
    CustomInfo* getCustom(const std::string& name);
    std::vector<CustomInfo> getAllCustoms() const;
    std::vector<CustomInfo> getCustomsByType(CustomType type) const;
    bool hasCustom(const std::string& name) const;
    
    // 自定义验证
    bool validateCustom(const CustomInfo& custom_info);
    std::vector<std::string> validateCustomList(const std::vector<CustomInfo>& customs);
    bool validateCustomName(const std::string& name);
    bool validateSpecialization(const SpecializationInfo& spec);
    
    // 自定义应用
    std::string applyStyleCustom(const std::string& custom_name, const std::vector<SpecializationInfo>& specs = {});
    std::string applyElementCustom(const std::string& custom_name, const std::vector<SpecializationInfo>& specs = {});
    std::string applyVarCustom(const std::string& custom_name, const std::string& variable_name, const std::vector<SpecializationInfo>& specs = {});
    
    // 特例化应用
    std::string applyDeleteProperty(const std::string& base_content, const std::vector<std::string>& properties);
    std::string applyDeleteInheritance(const std::string& base_content, const std::vector<std::string>& inheritances);
    std::string applyInsertElement(const std::string& base_content, const SpecializationInfo& spec);
    std::string applyModifyValue(const std::string& base_content, const std::string& property, const std::string& new_value);
    std::string applyIndexAccess(const std::string& base_content, const std::string& index);
    
    // 配置管理
    void setConfig(const CustomSystemConfig& config);
    CustomSystemConfig getConfig() const;
    
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
    bool isCustomDeclaration(const std::vector<Token>& tokens, size_t position);
    bool isCustomUsage(const std::vector<Token>& tokens, size_t position);
    CustomType detectCustomType(const std::vector<Token>& tokens, size_t position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    
    // 自定义验证辅助方法
    bool isValidCustomName(const std::string& name);
    bool isValidCustomContent(const std::string& content);
    bool hasValidBaseTemplate(const std::string& base_template);
    
    // 特例化应用辅助方法
    std::string parseCSSProperties(const std::string& content);
    std::vector<std::string> extractPropertyNames(const std::string& content);
    std::string removeProperty(const std::string& content, const std::string& property);
    std::string insertElementAtPosition(const std::string& content, const std::string& element, InsertPosition pos, const std::string& target);
    
    // 状态机管理
    std::shared_ptr<CHTLStateMachine> state_machine;
};

} // namespace chtl