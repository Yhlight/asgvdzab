#ifndef CHTL_STATE_H
#define CHTL_STATE_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <stack>
#include "utils/common.h"
#include "compiler/chtl/chtl_ast.h"

namespace chtl {

// 模板定义
struct TemplateDefinition {
    TemplateType type;
    std::string name;
    std::unique_ptr<ASTNode> content;
    std::vector<std::string> inherited_templates; // 继承的模板
    std::string namespace_path;                  // 命名空间路径
};

// 自定义定义
struct CustomDefinition {
    CustomType type;
    std::string name;
    std::unique_ptr<ASTNode> content;
    std::vector<std::string> inherited_customs;   // 继承的自定义
    std::string namespace_path;                  // 命名空间路径
    bool has_empty_values = false;               // 是否有无值属性
};

// 原始嵌入定义
struct OriginDefinition {
    std::string type;                            // @Html, @Style, @JavaScript或自定义
    std::string name;                            // 可选的名称
    std::string content;                         // 原始内容
    std::string namespace_path;                  // 命名空间路径
};

// 变量定义（用于变量组）
struct VariableDefinition {
    std::string name;
    std::string value;
    bool is_placeholder = false;                 // 是否是占位符（无值）
};

// 命名空间定义
struct NamespaceDefinition {
    std::string name;
    std::string parent_path;                     // 父命名空间路径
    std::vector<std::string> children;           // 子命名空间
    std::vector<std::string> except_rules;       // 约束规则
};

// 配置选项
struct ConfigurationOptions {
    // 基础配置
    int index_initial_count = 0;
    bool disable_name_group = false;
    bool disable_custom_origin_type = false;
    bool debug_mode = false;
    
    // Name配置组
    std::unordered_map<std::string, std::vector<std::string>> name_mappings;
    int option_count = 3;
    
    // OriginType配置组
    std::unordered_map<std::string, std::string> origin_types;
};

// 全局映射表 - 管理所有定义
class GlobalMap {
public:
    // 模板管理
    void addTemplate(const std::string& name, std::unique_ptr<TemplateDefinition> def);
    TemplateDefinition* getTemplate(const std::string& name, TemplateType type) const;
    std::vector<std::string> getTemplatesInNamespace(const std::string& ns_path, TemplateType type) const;
    
    // 自定义管理
    void addCustom(const std::string& name, std::unique_ptr<CustomDefinition> def);
    CustomDefinition* getCustom(const std::string& name, CustomType type) const;
    std::vector<std::string> getCustomsInNamespace(const std::string& ns_path, CustomType type) const;
    
    // 原始嵌入管理
    void addOrigin(const std::string& key, std::unique_ptr<OriginDefinition> def);
    OriginDefinition* getOrigin(const std::string& key) const;
    
    // 命名空间管理
    void enterNamespace(const std::string& name);
    void exitNamespace();
    std::string getCurrentNamespace() const;
    std::string resolveNamespace(const std::string& name) const;
    
    // 配置管理
    ConfigurationOptions& getConfig() { return config_; }
    const ConfigurationOptions& getConfig() const { return config_; }
    
private:
    // 模板存储
    std::unordered_map<std::string, std::unique_ptr<TemplateDefinition>> style_templates_;
    std::unordered_map<std::string, std::unique_ptr<TemplateDefinition>> element_templates_;
    std::unordered_map<std::string, std::unique_ptr<TemplateDefinition>> var_templates_;
    
    // 自定义存储
    std::unordered_map<std::string, std::unique_ptr<CustomDefinition>> custom_styles_;
    std::unordered_map<std::string, std::unique_ptr<CustomDefinition>> custom_elements_;
    std::unordered_map<std::string, std::unique_ptr<CustomDefinition>> custom_vars_;
    
    // 原始嵌入存储
    std::unordered_map<std::string, std::unique_ptr<OriginDefinition>> origins_;
    
    // 命名空间管理
    std::vector<std::string> namespace_stack_;
    std::unordered_map<std::string, NamespaceDefinition> namespaces_;
    
    // 配置
    ConfigurationOptions config_;
};

// 编译状态 - 管理编译过程中的状态
class CompilerState {
public:
    CompilerState(GlobalMap& global_map);
    
    // 错误和警告管理
    void addError(const std::string& msg, size_t line = 0, size_t column = 0);
    void addWarning(const std::string& msg, size_t line = 0, size_t column = 0);
    bool hasErrors() const { return !errors_.empty(); }
    const std::vector<CompilerError>& getErrors() const { return errors_; }
    const std::vector<CompilerError>& getWarnings() const { return warnings_; }
    
    // 局部样式管理
    void pushLocalStyleContext();
    void popLocalStyleContext();
    void addLocalStyle(const std::string& selector, const std::string& style);
    std::string generateClassName();
    
    // 全局样式收集
    void addGlobalStyle(const std::string& style);
    const std::vector<std::string>& getGlobalStyles() const { return global_styles_; }
    
    // 全局脚本收集
    void addGlobalScript(const std::string& script);
    const std::vector<std::string>& getGlobalScripts() const { return global_scripts_; }
    
    // 元素上下文管理
    void pushElementContext(const std::string& tag_name);
    void popElementContext();
    std::string getCurrentElement() const;
    
    // 约束检查
    bool isAllowed(const std::string& item) const;
    void addExceptRule(const std::string& rule);
    void clearExceptRules();
    
    // 访问全局映射
    GlobalMap& getGlobalMap() { return global_map_; }
    const GlobalMap& getGlobalMap() const { return global_map_; }
    
private:
    GlobalMap& global_map_;
    
    // 错误和警告
    std::vector<CompilerError> errors_;
    std::vector<CompilerError> warnings_;
    
    // 局部样式上下文
    struct LocalStyleContext {
        std::string element_class;
        std::vector<std::pair<std::string, std::string>> styles;
    };
    std::stack<LocalStyleContext> local_style_stack_;
    
    // 全局样式和脚本
    std::vector<std::string> global_styles_;
    std::vector<std::string> global_scripts_;
    
    // 元素上下文栈
    std::vector<std::string> element_stack_;
    
    // 约束规则
    std::vector<std::string> except_rules_;
    
    // 类名生成计数器
    size_t class_counter_ = 0;
};

} // namespace chtl

#endif // CHTL_STATE_H