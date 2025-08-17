#ifndef CHTL_AST_H
#define CHTL_AST_H

#include <string>
#include <vector>
#include <memory>
#include <variant>
#include "compiler/chtl/chtl_tokens.h"

namespace chtl {

// 模板类型
enum class TemplateType {
    STYLE,
    ELEMENT,
    VAR
};

// 自定义类型
enum class CustomType {
    STYLE,
    ELEMENT,
    VAR
};

// AST节点类型枚举
enum class ASTNodeType {
    // 基础节点
    ROOT,                    // 根节点
    TEXT,                    // 文本节点
    ELEMENT,                 // 元素节点
    ATTRIBUTE,               // 属性节点
    COMMENT,                 // 注释节点
    
    // 样式相关
    STYLE_BLOCK,             // 局部样式块
    STYLE_RULE,              // 样式规则
    STYLE_PROPERTY,          // 样式属性
    STYLE_SELECTOR,          // 样式选择器
    
    // 脚本相关
    SCRIPT_BLOCK,            // 局部脚本块
    
    // 模板相关
    TEMPLATE_DEFINITION,     // 模板定义
    TEMPLATE_USAGE,          // 模板使用
    
    // 自定义相关
    CUSTOM_DEFINITION,       // 自定义定义
    CUSTOM_USAGE,           // 自定义使用
    CUSTOM_SPECIALIZATION,   // 自定义特例化
    
    // 原始嵌入
    ORIGIN_BLOCK,           // 原始嵌入块
    
    // 导入
    IMPORT_STATEMENT,       // 导入语句
    
    // 命名空间
    NAMESPACE_BLOCK,        // 命名空间块
    
    // 配置
    CONFIGURATION_BLOCK,    // 配置块
    NAME_BLOCK,            // Name配置块
    ORIGIN_TYPE_BLOCK,     // OriginType配置块
    
    // 表达式
    IDENTIFIER,            // 标识符
    STRING_LITERAL,        // 字符串字面量
    NUMBER_LITERAL,        // 数字字面量
    VARIABLE_ACCESS,       // 变量访问（如ThemeColor(tableColor)）
    
    // 特殊操作
    DELETE_OPERATION,      // delete操作
    INSERT_OPERATION,      // insert操作
    INHERIT_OPERATION,     // inherit操作
    EXCEPT_CLAUSE,         // except约束
    
    // 索引访问
    INDEX_ACCESS,          // 索引访问（如div[1]）
    
    // 变量组
    VAR_PROPERTY,          // 变量属性
    
    // 全缀名
    QUALIFIED_NAME,        // 全缀名（如[Custom] @Element Box）
    
    // CMOD相关
    INFO_BLOCK,            // [Info]块
    EXPORT_BLOCK           // [Export]块
};

// 基础AST节点
class ASTNode {
public:
    ASTNode(ASTNodeType type, size_t line = 0, size_t column = 0)
        : type_(type), line_(line), column_(column) {}
    
    virtual ~ASTNode() = default;
    
    ASTNodeType getType() const { return type_; }
    size_t getLine() const { return line_; }
    size_t getColumn() const { return column_; }
    
    // 子节点管理
    void addChild(std::unique_ptr<ASTNode> child) {
        children_.push_back(std::move(child));
    }
    
    const std::vector<std::unique_ptr<ASTNode>>& getChildren() const {
        return children_;
    }
    
    std::vector<std::unique_ptr<ASTNode>>& getChildren() {
        return children_;
    }
    
protected:
    ASTNodeType type_;
    size_t line_;
    size_t column_;
    std::vector<std::unique_ptr<ASTNode>> children_;
};

// 根节点
class RootNode : public ASTNode {
public:
    RootNode() : ASTNode(ASTNodeType::ROOT) {}
};

// 文本节点
class TextNode : public ASTNode {
public:
    TextNode(const std::string& content, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::TEXT, line, column), content_(content) {}
    
    const std::string& getContent() const { return content_; }
    
private:
    std::string content_;
};

// 元素节点
class ElementNode : public ASTNode {
public:
    ElementNode(const std::string& tag_name, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::ELEMENT, line, column), tag_name_(tag_name),
          has_local_style(false), has_local_script(false) {}
    
    const std::string& getTagName() const { return tag_name_; }
    
    // 属性管理
    void addAttribute(const std::string& name, const std::string& value) {
        attributes_.push_back({name, value});
    }
    
    const std::vector<std::pair<std::string, std::string>>& getAttributes() const {
        return attributes_;
    }
    
    // 公开成员（为了兼容）
    bool has_local_style;
    bool has_local_script;
    std::string generated_class_name;
    
private:
    std::string tag_name_;
    std::vector<std::pair<std::string, std::string>> attributes_;
};

// 属性节点
class AttributeNode : public ASTNode {
public:
    AttributeNode(const std::string& name, const std::string& value, 
                  size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::ATTRIBUTE, line, column), 
          name_(name), value_(value) {}
    
    const std::string& getName() const { return name_; }
    const std::string& getValue() const { return value_; }
    
private:
    std::string name_;
    std::string value_;
};

// 样式块节点
class StyleBlockNode : public ASTNode {
public:
    StyleBlockNode(size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::STYLE_BLOCK, line, column) {}
};

// 样式规则节点
class StyleRuleNode : public ASTNode {
public:
    StyleRuleNode(const std::string& selector, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::STYLE_RULE, line, column), selector_(selector) {}
    
    const std::string& getSelector() const { return selector_; }
    
private:
    std::string selector_;
};

// 样式属性节点
class StylePropertyNode : public ASTNode {
public:
    StylePropertyNode(const std::string& property, const std::string& value,
                     size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::STYLE_PROPERTY, line, column),
          property_(property), value_(value) {}
    
    const std::string& getProperty() const { return property_; }
    const std::string& getValue() const { return value_; }
    
private:
    std::string property_;
    std::string value_;
};

// 脚本块节点
class ScriptBlockNode : public ASTNode {
public:
    ScriptBlockNode(const std::string& content, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::SCRIPT_BLOCK, line, column), content_(content) {}
    
    const std::string& getContent() const { return content_; }
    
private:
    std::string content_;
};

// 模板定义节点
class TemplateDefinitionNode : public ASTNode {
public:
    TemplateDefinitionNode(TemplateType type, const std::string& name,
                          size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::TEMPLATE_DEFINITION, line, column),
          template_type_(type), name_(name) {}
    
    TemplateType getTemplateType() const { return template_type_; }
    const std::string& getName() const { return name_; }
    
private:
    TemplateType template_type_;
    std::string name_;
};

// 模板使用节点
class TemplateUsageNode : public ASTNode {
public:
    TemplateUsageNode(TemplateType type, const std::string& name,
                     size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::TEMPLATE_USAGE, line, column),
          template_type_(type), name_(name) {}
    
    TemplateType getTemplateType() const { return template_type_; }
    const std::string& getName() const { return name_; }
    
    // 用于变量组模板的参数
    void addParameter(const std::string& param_name, const std::string& value) {
        parameters_.push_back({param_name, value});
    }
    
    const std::vector<std::pair<std::string, std::string>>& getParameters() const {
        return parameters_;
    }
    
private:
    TemplateType template_type_;
    std::string name_;
    std::vector<std::pair<std::string, std::string>> parameters_;
};

// 自定义定义节点
class CustomDefinitionNode : public ASTNode {
public:
    CustomDefinitionNode(CustomType type, const std::string& name,
                        size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::CUSTOM_DEFINITION, line, column),
          custom_type_(type), name_(name) {}
    
    CustomType getCustomType() const { return custom_type_; }
    const std::string& getName() const { return name_; }
    
private:
    CustomType custom_type_;
    std::string name_;
};

// 自定义使用节点
class CustomUsageNode : public ASTNode {
public:
    CustomUsageNode(CustomType type, const std::string& name,
                   size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::CUSTOM_USAGE, line, column),
          custom_type_(type), name_(name) {}
    
    CustomType getCustomType() const { return custom_type_; }
    const std::string& getName() const { return name_; }
    
private:
    CustomType custom_type_;
    std::string name_;
};

// 原始嵌入节点
class OriginBlockNode : public ASTNode {
public:
    OriginBlockNode(const std::string& type, const std::string& name,
                   const std::string& content, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::ORIGIN_BLOCK, line, column),
          type_(type), name_(name), content_(content) {}
    
    const std::string& getOriginType() const { return type_; }
    const std::string& getName() const { return name_; }
    const std::string& getContent() const { return content_; }
    
private:
    std::string type_;    // @Html, @Style, @JavaScript或自定义
    std::string name_;    // 可选的名称
    std::string content_;
};

// 导入语句节点
class ImportStatementNode : public ASTNode {
public:
    ImportStatementNode(size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::IMPORT_STATEMENT, line, column) {}
    
    // 设置导入类型
    void setImportType(const std::string& type) { import_type_ = type; }
    
    // 设置导入路径
    void setPath(const std::string& path) { path_ = path; }
    
    // 设置别名
    void setAlias(const std::string& alias) { alias_ = alias; }
    
    // 设置具体项（用于从CHTL文件导入特定内容）
    void setItemName(const std::string& name) { item_name_ = name; }
    
    const std::string& getImportType() const { return import_type_; }
    const std::string& getPath() const { return path_; }
    const std::string& getAlias() const { return alias_; }
    const std::string& getItemName() const { return item_name_; }
    
private:
    std::string import_type_;  // @Html, @Style, @JavaScript, @Chtl, @CJmod等
    std::string path_;         // 文件路径
    std::string alias_;        // 可选的别名
    std::string item_name_;    // 导入的具体项名称
};

// 命名空间节点
class NamespaceBlockNode : public ASTNode {
public:
    NamespaceBlockNode(const std::string& name, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::NAMESPACE_BLOCK, line, column), name_(name) {}
    
    const std::string& getName() const { return name_; }
    
private:
    std::string name_;
};

// 配置块节点
class ConfigurationBlockNode : public ASTNode {
public:
    ConfigurationBlockNode(size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::CONFIGURATION_BLOCK, line, column) {}
    
    // 添加配置项
    void addConfig(const std::string& key, const std::string& value) {
        configs_[key] = value;
    }
    
    const std::unordered_map<std::string, std::string>& getConfigs() const {
        return configs_;
    }
    
private:
    std::unordered_map<std::string, std::string> configs_;
};

// 删除操作节点
class DeleteOperationNode : public ASTNode {
public:
    DeleteOperationNode(size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::DELETE_OPERATION, line, column) {}
    
    // 添加要删除的项
    void addTarget(const std::string& target) {
        targets_.push_back(target);
    }
    
    const std::vector<std::string>& getTargets() const { return targets_; }
    
private:
    std::vector<std::string> targets_;
};

// 插入操作节点
class InsertOperationNode : public ASTNode {
public:
    enum class Position {
        AFTER,
        BEFORE,
        REPLACE,
        AT_TOP,
        AT_BOTTOM
    };
    
    InsertOperationNode(Position pos, const std::string& target,
                       size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::INSERT_OPERATION, line, column),
          position_(pos), target_(target) {}
    
    Position getPosition() const { return position_; }
    const std::string& getTarget() const { return target_; }
    
private:
    Position position_;
    std::string target_;
};

// 继承操作节点
class InheritOperationNode : public ASTNode {
public:
    InheritOperationNode(const std::string& type, const std::string& name,
                        size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::INHERIT_OPERATION, line, column),
          inherit_type_(type), name_(name) {}
    
    const std::string& getInheritType() const { return inherit_type_; }
    const std::string& getName() const { return name_; }
    
private:
    std::string inherit_type_;  // @Style, @Element, @Var
    std::string name_;
};

// 约束节点
class ExceptClauseNode : public ASTNode {
public:
    ExceptClauseNode(size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::EXCEPT_CLAUSE, line, column) {}
    
    void addRule(const std::string& rule) {
        rules_.push_back(rule);
    }
    
    const std::vector<std::string>& getRules() const { return rules_; }
    
private:
    std::vector<std::string> rules_;
};

// 索引访问节点
class IndexAccessNode : public ASTNode {
public:
    IndexAccessNode(const std::string& base, int index,
                   size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::INDEX_ACCESS, line, column),
          base_(base), index_(index) {}
    
    const std::string& getBase() const { return base_; }
    int getIndex() const { return index_; }
    
private:
    std::string base_;
    int index_;
};

// 变量访问节点
class VariableAccessNode : public ASTNode {
public:
    VariableAccessNode(const std::string& var_group, const std::string& var_name,
                      size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::VARIABLE_ACCESS, line, column),
          var_group_(var_group), var_name_(var_name) {}
    
    const std::string& getVarGroup() const { return var_group_; }
    const std::string& getVarName() const { return var_name_; }
    
    // 用于变量特例化
    void setValue(const std::string& value) { override_value_ = value; }
    bool hasOverrideValue() const { return !override_value_.empty(); }
    const std::string& getOverrideValue() const { return override_value_; }
    
private:
    std::string var_group_;
    std::string var_name_;
    std::string override_value_;
};

// 标识符节点
class IdentifierNode : public ASTNode {
public:
    IdentifierNode(const std::string& name, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::IDENTIFIER, line, column), name_(name) {}
    
    const std::string& getName() const { return name_; }
    
private:
    std::string name_;
};

// 字符串字面量节点
class StringLiteralNode : public ASTNode {
public:
    StringLiteralNode(const std::string& value, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::STRING_LITERAL, line, column), value_(value) {}
    
    const std::string& getValue() const { return value_; }
    
private:
    std::string value_;
};

// 数字字面量节点
class NumberLiteralNode : public ASTNode {
public:
    NumberLiteralNode(double value, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::NUMBER_LITERAL, line, column), value_(value) {}
    
    double getValue() const { return value_; }
    
private:
    double value_;
};

} // namespace chtl

#endif // CHTL_AST_H