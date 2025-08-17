#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <variant>
#include <optional>

namespace chtl {
namespace ast {

// 前向声明
class ASTNode;
class Expression;
class Statement;

// AST节点的智能指针类型
using ASTNodePtr = std::shared_ptr<ASTNode>;
using ExpressionPtr = std::shared_ptr<Expression>;
using StatementPtr = std::shared_ptr<Statement>;

/**
 * 源码位置信息
 */
struct SourceLocation {
    std::string filename;
    int line = 1;
    int column = 1;
    int offset = 0;
    
    SourceLocation() = default;
    SourceLocation(const std::string& file, int ln, int col, int off = 0)
        : filename(file), line(ln), column(col), offset(off) {}
};

/**
 * AST节点类型枚举
 */
enum class NodeType {
    // 基础节点
    PROGRAM,                // 程序根节点
    IDENTIFIER,             // 标识符
    LITERAL,                // 字面量
    
    // HTML元素相关
    ELEMENT,                // HTML元素
    ATTRIBUTE,              // 属性
    TEXT_NODE,              // 文本节点
    
    // CHTL特殊结构
    TEMPLATE_STYLE,         // [Template] @Style
    TEMPLATE_ELEMENT,       // [Template] @Element  
    TEMPLATE_VAR,           // [Template] @Var
    CUSTOM_STYLE,           // [Custom] @Style
    CUSTOM_ELEMENT,         // [Custom] @Element
    CUSTOM_VAR,             // [Custom] @Var
    
    // 命名空间和导入
    NAMESPACE,              // [Namespace]
    IMPORT,                 // [Import]
    
    // 样式相关
    STYLE_BLOCK,            // style {}块
    STYLE_PROPERTY,         // CSS属性
    SELECTOR,               // CSS选择器
    
    // 配置和约束
    CONFIGURATION,          // [Configuration]
    CONSTRAINT,             // except约束
    
    // 原始嵌入
    ORIGIN_HTML,            // [Origin] @Html
    ORIGIN_STYLE,           // [Origin] @Style
    ORIGIN_JAVASCRIPT,      // [Origin] @JavaScript
    
    // 表达式
    VARIABLE_ACCESS,        // 变量访问
    FUNCTION_CALL,          // 函数调用
    TEMPLATE_USAGE,         // 模板使用
    INHERITANCE,            // 继承表达式
    
    // 注释
    COMMENT,                // 注释
    GENERATOR_COMMENT,      // 生成器注释 --
    
    // 特殊操作
    SPECIALIZATION,         // 特例化操作
    DELETE_OPERATION,       // delete操作
    ADD_OPERATION,          // add操作
    INDEX_ACCESS,           // 索引访问
    
    // CHTL JS相关
    CHTL_JS_BLOCK,          // CHTL JS代码块
    ENHANCED_SELECTOR,      // {{选择器}}
    ARROW_OPERATION,        // ->操作符
    
    // 辅助节点
    BLOCK,                  // 代码块
    EXPRESSION_STATEMENT,   // 表达式语句
    DECLARATION             // 声明
};

/**
 * 字面量类型
 */
enum class LiteralType {
    STRING,         // 字符串字面量
    NUMBER,         // 数字字面量
    BOOLEAN,        // 布尔字面量
    UNQUOTED        // 无引号字面量
};

/**
 * 访问修饰符
 */
enum class AccessModifier {
    PUBLIC,
    PRIVATE,
    PROTECTED
};

/**
 * AST节点基类
 */
class ASTNode {
public:
    explicit ASTNode(NodeType type, const SourceLocation& loc = {})
        : nodeType_(type), location_(loc) {}
    
    virtual ~ASTNode() = default;
    
    NodeType getType() const { return nodeType_; }
    const SourceLocation& getLocation() const { return location_; }
    void setLocation(const SourceLocation& loc) { location_ = loc; }
    
    // 访问者模式支持
    virtual void accept(class ASTVisitor& visitor) = 0;
    
    // 克隆支持
    virtual std::unique_ptr<ASTNode> clone() const = 0;
    
    // 调试输出
    virtual std::string toString() const = 0;

protected:
    NodeType nodeType_;
    SourceLocation location_;
};

/**
 * 程序根节点
 */
class Program : public ASTNode {
public:
    Program() : ASTNode(NodeType::PROGRAM) {}
    
    void addChild(ASTNodePtr child) {
        children_.push_back(std::move(child));
    }
    
    const std::vector<ASTNodePtr>& getChildren() const { return children_; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::vector<ASTNodePtr> children_;
};

/**
 * 标识符节点
 */
class Identifier : public ASTNode {
public:
    explicit Identifier(const std::string& name, const SourceLocation& loc = {})
        : ASTNode(NodeType::IDENTIFIER, loc), name_(name) {}
    
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string name_;
};

/**
 * 字面量节点
 */
class Literal : public ASTNode {
public:
    Literal(const std::string& value, LiteralType type, const SourceLocation& loc = {})
        : ASTNode(NodeType::LITERAL, loc), value_(value), literalType_(type) {}
    
    const std::string& getValue() const { return value_; }
    LiteralType getLiteralType() const { return literalType_; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string value_;
    LiteralType literalType_;
};

/**
 * HTML元素节点
 */
class Element : public ASTNode {
public:
    explicit Element(const std::string& tagName, const SourceLocation& loc = {})
        : ASTNode(NodeType::ELEMENT, loc), tagName_(tagName) {}
    
    const std::string& getTagName() const { return tagName_; }
    void setTagName(const std::string& tagName) { tagName_ = tagName; }
    
    void addAttribute(std::shared_ptr<class Attribute> attr);
    void addChild(ASTNodePtr child) { children_.push_back(std::move(child)); }
    void setStyleBlock(std::shared_ptr<class StyleBlock> style);
    
    const std::vector<std::shared_ptr<class Attribute>>& getAttributes() const { return attributes_; }
    const std::vector<ASTNodePtr>& getChildren() const { return children_; }
    std::shared_ptr<class StyleBlock> getStyleBlock() const { return styleBlock_; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string tagName_;
    std::vector<std::shared_ptr<class Attribute>> attributes_;
    std::vector<ASTNodePtr> children_;
    std::shared_ptr<class StyleBlock> styleBlock_;
};

/**
 * 属性节点
 */
class Attribute : public ASTNode {
public:
    Attribute(const std::string& name, const std::string& value, const SourceLocation& loc = {})
        : ASTNode(NodeType::ATTRIBUTE, loc), name_(name), value_(value) {}
    
    const std::string& getName() const { return name_; }
    const std::string& getValue() const { return value_; }
    void setValue(const std::string& value) { value_ = value; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string name_;
    std::string value_;
};

/**
 * 文本节点
 */
class TextNode : public ASTNode {
public:
    explicit TextNode(const std::string& content, const SourceLocation& loc = {})
        : ASTNode(NodeType::TEXT_NODE, loc), content_(content) {}
    
    const std::string& getContent() const { return content_; }
    void setContent(const std::string& content) { content_ = content; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string content_;
};

/**
 * 样式块节点
 */
class StyleBlock : public ASTNode {
public:
    explicit StyleBlock(const SourceLocation& loc = {})
        : ASTNode(NodeType::STYLE_BLOCK, loc) {}
    
    void addProperty(std::shared_ptr<class StyleProperty> prop);
    void addSelector(std::shared_ptr<class Selector> selector);
    
    const std::vector<std::shared_ptr<class StyleProperty>>& getProperties() const { return properties_; }
    const std::vector<std::shared_ptr<class Selector>>& getSelectors() const { return selectors_; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::vector<std::shared_ptr<class StyleProperty>> properties_;  // 内联样式
    std::vector<std::shared_ptr<class Selector>> selectors_;        // 选择器样式
};

/**
 * 样式属性节点
 */
class StyleProperty : public ASTNode {
public:
    StyleProperty(const std::string& property, const std::string& value, const SourceLocation& loc = {})
        : ASTNode(NodeType::STYLE_PROPERTY, loc), property_(property), value_(value) {}
    
    const std::string& getProperty() const { return property_; }
    const std::string& getValue() const { return value_; }
    void setValue(const std::string& value) { value_ = value; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string property_;
    std::string value_;
};

/**
 * CSS选择器节点
 */
class Selector : public ASTNode {
public:
    explicit Selector(const std::string& selector, const SourceLocation& loc = {})
        : ASTNode(NodeType::SELECTOR, loc), selector_(selector) {}
    
    const std::string& getSelector() const { return selector_; }
    void addProperty(std::shared_ptr<StyleProperty> prop);
    
    const std::vector<std::shared_ptr<StyleProperty>>& getProperties() const { return properties_; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string selector_;
    std::vector<std::shared_ptr<StyleProperty>> properties_;
};

/**
 * 模板样式节点
 */
class TemplateStyle : public ASTNode {
public:
    explicit TemplateStyle(const std::string& name, const SourceLocation& loc = {})
        : ASTNode(NodeType::TEMPLATE_STYLE, loc), name_(name) {}
    
    const std::string& getName() const { return name_; }
    void addProperty(std::shared_ptr<StyleProperty> prop);
    void addInheritance(std::shared_ptr<class TemplateUsage> inheritance);
    
    const std::vector<std::shared_ptr<StyleProperty>>& getProperties() const { return properties_; }
    const std::vector<std::shared_ptr<class TemplateUsage>>& getInheritances() const { return inheritances_; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string name_;
    std::vector<std::shared_ptr<StyleProperty>> properties_;
    std::vector<std::shared_ptr<class TemplateUsage>> inheritances_;
};

/**
 * 模板元素节点
 */
class TemplateElement : public ASTNode {
public:
    explicit TemplateElement(const std::string& name, const SourceLocation& loc = {})
        : ASTNode(NodeType::TEMPLATE_ELEMENT, loc), name_(name) {}
    
    const std::string& getName() const { return name_; }
    void addChild(ASTNodePtr child) { children_.push_back(std::move(child)); }
    void addInheritance(std::shared_ptr<class TemplateUsage> inheritance);
    
    const std::vector<ASTNodePtr>& getChildren() const { return children_; }
    const std::vector<std::shared_ptr<class TemplateUsage>>& getInheritances() const { return inheritances_; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string name_;
    std::vector<ASTNodePtr> children_;
    std::vector<std::shared_ptr<class TemplateUsage>> inheritances_;
};

/**
 * 模板变量节点
 */
class TemplateVar : public ASTNode {
public:
    explicit TemplateVar(const std::string& name, const SourceLocation& loc = {})
        : ASTNode(NodeType::TEMPLATE_VAR, loc), name_(name) {}
    
    const std::string& getName() const { return name_; }
    void addVariable(const std::string& name, const std::string& value);
    void addInheritance(std::shared_ptr<class TemplateUsage> inheritance);
    
    const std::unordered_map<std::string, std::string>& getVariables() const { return variables_; }
    const std::vector<std::shared_ptr<class TemplateUsage>>& getInheritances() const { return inheritances_; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string name_;
    std::unordered_map<std::string, std::string> variables_;
    std::vector<std::shared_ptr<class TemplateUsage>> inheritances_;
};

/**
 * 自定义样式节点
 */
class CustomStyle : public ASTNode {
public:
    explicit CustomStyle(const std::string& name, const SourceLocation& loc = {})
        : ASTNode(NodeType::CUSTOM_STYLE, loc), name_(name) {}
    
    const std::string& getName() const { return name_; }
    void addProperty(std::shared_ptr<StyleProperty> prop);
    void addParameter(const std::string& name, const std::string& defaultValue = "");
    void addSpecialization(std::shared_ptr<class Specialization> spec);
    
    const std::vector<std::shared_ptr<StyleProperty>>& getProperties() const { return properties_; }
    const std::unordered_map<std::string, std::string>& getParameters() const { return parameters_; }
    const std::vector<std::shared_ptr<class Specialization>>& getSpecializations() const { return specializations_; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string name_;
    std::vector<std::shared_ptr<StyleProperty>> properties_;
    std::unordered_map<std::string, std::string> parameters_;
    std::vector<std::shared_ptr<class Specialization>> specializations_;
};

/**
 * 命名空间节点
 */
class Namespace : public ASTNode {
public:
    explicit Namespace(const std::string& name, const SourceLocation& loc = {})
        : ASTNode(NodeType::NAMESPACE, loc), name_(name) {}
    
    const std::string& getName() const { return name_; }
    void addChild(ASTNodePtr child) { children_.push_back(std::move(child)); }
    void setParent(std::shared_ptr<Namespace> parent) { parent_ = parent; }
    
    const std::vector<ASTNodePtr>& getChildren() const { return children_; }
    std::shared_ptr<Namespace> getParent() const { return parent_; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string name_;
    std::vector<ASTNodePtr> children_;
    std::shared_ptr<Namespace> parent_;
};

/**
 * 导入节点
 */
class Import : public ASTNode {
public:
    enum class ImportType {
        HTML, STYLE, JAVASCRIPT, CHTL, CJMOD,
        CUSTOM_ELEMENT, CUSTOM_STYLE, CUSTOM_VAR,
        TEMPLATE_ELEMENT, TEMPLATE_STYLE, TEMPLATE_VAR
    };
    
    Import(ImportType type, const std::string& path, const SourceLocation& loc = {})
        : ASTNode(NodeType::IMPORT, loc), importType_(type), path_(path) {}
    
    ImportType getImportType() const { return importType_; }
    const std::string& getPath() const { return path_; }
    const std::string& getAlias() const { return alias_; }
    const std::string& getFrom() const { return from_; }
    
    void setAlias(const std::string& alias) { alias_ = alias; }
    void setFrom(const std::string& from) { from_ = from; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    ImportType importType_;
    std::string path_;
    std::string alias_;
    std::string from_;
};

/**
 * 模板使用节点
 */
class TemplateUsage : public ASTNode {
public:
    enum class UsageType {
        STYLE, ELEMENT, VAR
    };
    
    TemplateUsage(UsageType type, const std::string& name, const SourceLocation& loc = {})
        : ASTNode(NodeType::TEMPLATE_USAGE, loc), usageType_(type), name_(name) {}
    
    UsageType getUsageType() const { return usageType_; }
    const std::string& getName() const { return name_; }
    const std::string& getFrom() const { return from_; }
    const std::unordered_map<std::string, std::string>& getArguments() const { return arguments_; }
    
    void setFrom(const std::string& from) { from_ = from; }
    void addArgument(const std::string& name, const std::string& value);
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    UsageType usageType_;
    std::string name_;
    std::string from_;
    std::unordered_map<std::string, std::string> arguments_;
};

/**
 * 特例化节点
 */
class Specialization : public ASTNode {
public:
    enum class SpecType {
        DELETE_PROPERTY,    // delete属性
        DELETE_INHERITANCE, // delete继承
        ADD_STYLE,         // add样式
        INDEX_ACCESS       // index访问
    };
    
    Specialization(SpecType type, const std::string& target, const SourceLocation& loc = {})
        : ASTNode(NodeType::SPECIALIZATION, loc), specType_(type), target_(target) {}
    
    SpecType getSpecType() const { return specType_; }
    const std::string& getTarget() const { return target_; }
    const std::string& getValue() const { return value_; }
    int getIndex() const { return index_; }
    
    void setValue(const std::string& value) { value_ = value; }
    void setIndex(int index) { index_ = index; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    SpecType specType_;
    std::string target_;
    std::string value_;
    int index_ = -1;
};

/**
 * 注释节点
 */
class Comment : public ASTNode {
public:
    enum class CommentType {
        SINGLE_LINE,        // //
        MULTI_LINE,         // /* */
        GENERATOR           // --
    };
    
    Comment(CommentType type, const std::string& content, const SourceLocation& loc = {})
        : ASTNode(NodeType::COMMENT, loc), commentType_(type), content_(content) {}
    
    CommentType getCommentType() const { return commentType_; }
    const std::string& getContent() const { return content_; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    CommentType commentType_;
    std::string content_;
};

/**
 * 配置节点
 */
class Configuration : public ASTNode {
public:
    explicit Configuration(const SourceLocation& loc = {})
        : ASTNode(NodeType::CONFIGURATION, loc) {}
    
    void addSetting(const std::string& key, const std::string& value);
    const std::unordered_map<std::string, std::string>& getSettings() const { return settings_; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::unordered_map<std::string, std::string> settings_;
};

/**
 * 约束节点
 */
class Constraint : public ASTNode {
public:
    enum class ConstraintType {
        ELEMENT_EXCEPT,     // 元素约束
        TYPE_EXCEPT,        // 类型约束
        GLOBAL_EXCEPT       // 全局约束
    };
    
    Constraint(ConstraintType type, const std::string& target, const SourceLocation& loc = {})
        : ASTNode(NodeType::CONSTRAINT, loc), constraintType_(type), target_(target) {}
    
    ConstraintType getConstraintType() const { return constraintType_; }
    const std::string& getTarget() const { return target_; }
    const std::vector<std::string>& getExceptions() const { return exceptions_; }
    
    void addException(const std::string& exception);
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    ConstraintType constraintType_;
    std::string target_;
    std::vector<std::string> exceptions_;
};

/**
 * 变量访问节点
 */
class VariableAccess : public ASTNode {
public:
    explicit VariableAccess(const std::string& varName, const SourceLocation& loc = {})
        : ASTNode(NodeType::VARIABLE_ACCESS, loc), variableName_(varName) {}
    
    const std::string& getVariableName() const { return variableName_; }
    const std::string& getProperty() const { return property_; }
    
    void setProperty(const std::string& property) { property_ = property; }
    
    void accept(ASTVisitor& visitor) override;
    std::unique_ptr<ASTNode> clone() const override;
    std::string toString() const override;

private:
    std::string variableName_;
    std::string property_;
};

} // namespace ast
} // namespace chtl