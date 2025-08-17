#pragma once

#include "Common.hpp"
#include "Lexer.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <variant>

namespace CHTL {

// 前向声明
class ASTNode;
class ASTVisitor;

// 智能指针类型定义
using ASTNodePtr = std::shared_ptr<ASTNode>;
using ASTNodeUniquePtr = std::unique_ptr<ASTNode>;

/**
 * AST节点类型枚举
 */
enum class ASTNodeType {
    // 程序结构
    PROGRAM,                // 程序根节点
    BLOCK,                  // 代码块
    
    // 基础节点
    IDENTIFIER,             // 标识符
    LITERAL,                // 字面量
    
    // HTML结构
    ELEMENT,                // HTML元素
    ATTRIBUTE,              // 属性
    TEXT_NODE,              // 文本节点
    
    // CHTL特殊结构
    TEMPLATE_DEFINITION,    // 模板定义
    TEMPLATE_STYLE,         // [Template] @Style
    TEMPLATE_ELEMENT,       // [Template] @Element
    TEMPLATE_VARIABLE,      // [Template] @Var
    
    CUSTOM_DEFINITION,      // 自定义定义
    CUSTOM_STYLE,           // [Custom] @Style
    CUSTOM_ELEMENT,         // [Custom] @Element
    CUSTOM_VARIABLE,        // [Custom] @Var
    
    // 命名空间和导入
    NAMESPACE_DEFINITION,   // [Namespace]
    IMPORT_STATEMENT,       // [Import]
    
    // 样式系统
    STYLE_BLOCK,            // style {}
    STYLE_PROPERTY,         // CSS属性
    STYLE_RULE,             // CSS规则
    SELECTOR,               // CSS选择器
    
    // 配置和约束
    CONFIGURATION,          // [Configuration]
    CONSTRAINT,             // 约束
    
    // 表达式
    TEMPLATE_USAGE,         // 模板使用
    VARIABLE_ACCESS,        // 变量访问
    FUNCTION_CALL,          // 函数调用
    BINARY_EXPRESSION,      // 二元表达式
    UNARY_EXPRESSION,       // 一元表达式
    
    // 特殊操作
    INHERITANCE,            // 继承
    SPECIALIZATION,         // 特例化
    
    // 注释
    COMMENT,                // 注释
    
    // 原始嵌入
    ORIGIN_BLOCK            // [Origin]
};

/**
 * AST节点基类
 */
class ASTNode {
public:
    explicit ASTNode(ASTNodeType type, const SourceLocation& location = {})
        : nodeType_(type), location_(location) {}
    
    virtual ~ASTNode() = default;
    
    // 基础访问方法
    ASTNodeType getNodeType() const { return nodeType_; }
    const SourceLocation& getLocation() const { return location_; }
    void setLocation(const SourceLocation& location) { location_ = location; }
    
    // 访问者模式
    virtual void accept(ASTVisitor& visitor) = 0;
    
    // 克隆方法
    virtual ASTNodeUniquePtr clone() const = 0;
    
    // 调试方法
    virtual std::string toString() const = 0;
    virtual std::string toDetailedString() const;
    
    // 类型检查方法
    template<typename T>
    bool is() const {
        return dynamic_cast<const T*>(this) != nullptr;
    }
    
    template<typename T>
    T* as() {
        return dynamic_cast<T*>(this);
    }
    
    template<typename T>
    const T* as() const {
        return dynamic_cast<const T*>(this);
    }

protected:
    ASTNodeType nodeType_;
    SourceLocation location_;
};

/**
 * 程序根节点
 */
class Program : public ASTNode {
public:
    Program() : ASTNode(ASTNodeType::PROGRAM) {}
    
    void addChild(ASTNodePtr child) { children_.push_back(std::move(child)); }
    const std::vector<ASTNodePtr>& getChildren() const { return children_; }
    
    // 特定类型的子节点访问
    std::vector<std::shared_ptr<class NamespaceDefinition>> getNamespaces() const;
    std::vector<std::shared_ptr<class ImportStatement>> getImports() const;
    std::vector<std::shared_ptr<class TemplateDefinition>> getTemplates() const;
    std::vector<std::shared_ptr<class Element>> getElements() const;
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    std::vector<ASTNodePtr> children_;
};

/**
 * 标识符节点
 */
class Identifier : public ASTNode {
public:
    explicit Identifier(const std::string& name, const SourceLocation& location = {})
        : ASTNode(ASTNodeType::IDENTIFIER, location), name_(name) {}
    
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    // 命名空间支持
    bool isQualified() const { return name_.find('.') != std::string::npos; }
    std::vector<std::string> getNameParts() const;
    std::string getNamespace() const;
    std::string getLocalName() const;
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    std::string name_;
};

/**
 * 字面量节点
 */
class Literal : public ASTNode {
public:
    Literal(const std::string& value, LiteralType type, const SourceLocation& location = {})
        : ASTNode(ASTNodeType::LITERAL, location), value_(value), literalType_(type) {}
    
    const std::string& getValue() const { return value_; }
    LiteralType getLiteralType() const { return literalType_; }
    
    // 类型转换方法
    std::string asString() const;
    double asNumber() const;
    bool asBoolean() const;
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
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
    explicit Element(const std::string& tagName, const SourceLocation& location = {})
        : ASTNode(ASTNodeType::ELEMENT, location), tagName_(tagName) {}
    
    // 基础访问
    const std::string& getTagName() const { return tagName_; }
    void setTagName(const std::string& tagName) { tagName_ = tagName; }
    
    // 属性管理
    void addAttribute(std::shared_ptr<class Attribute> attribute);
    void removeAttribute(const std::string& name);
    std::shared_ptr<class Attribute> getAttribute(const std::string& name) const;
    const std::vector<std::shared_ptr<class Attribute>>& getAttributes() const { return attributes_; }
    bool hasAttribute(const std::string& name) const;
    
    // 子节点管理
    void addChild(ASTNodePtr child) { children_.push_back(std::move(child)); }
    void insertChild(size_t index, ASTNodePtr child);
    void removeChild(size_t index);
    const std::vector<ASTNodePtr>& getChildren() const { return children_; }
    
    // 样式块
    void setStyleBlock(std::shared_ptr<class StyleBlock> styleBlock) { styleBlock_ = styleBlock; }
    std::shared_ptr<class StyleBlock> getStyleBlock() const { return styleBlock_; }
    bool hasStyleBlock() const { return styleBlock_ != nullptr; }
    
    // HTML特性检查
    bool isSelfClosing() const;
    bool isBlockElement() const;
    bool isInlineElement() const;
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
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
    Attribute(const std::string& name, const std::string& value, const SourceLocation& location = {})
        : ASTNode(ASTNodeType::ATTRIBUTE, location), name_(name), value_(value) {}
    
    const std::string& getName() const { return name_; }
    const std::string& getValue() const { return value_; }
    void setValue(const std::string& value) { value_ = value; }
    
    // 属性类型检查
    bool isBoolean() const;
    bool isDataAttribute() const;
    bool isAriaAttribute() const;
    bool isEventAttribute() const;
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
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
    explicit TextNode(const std::string& content, const SourceLocation& location = {})
        : ASTNode(ASTNodeType::TEXT_NODE, location), content_(content) {}
    
    const std::string& getContent() const { return content_; }
    void setContent(const std::string& content) { content_ = content; }
    
    // 文本处理
    std::string getProcessedContent() const;  // 处理转义序列等
    bool isEmpty() const { return content_.empty(); }
    bool isWhitespaceOnly() const;
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    std::string content_;
};

/**
 * 样式块节点
 */
class StyleBlock : public ASTNode {
public:
    explicit StyleBlock(const SourceLocation& location = {})
        : ASTNode(ASTNodeType::STYLE_BLOCK, location) {}
    
    // 内联样式属性
    void addProperty(std::shared_ptr<class StyleProperty> property);
    void removeProperty(const std::string& name);
    std::shared_ptr<class StyleProperty> getProperty(const std::string& name) const;
    const std::vector<std::shared_ptr<class StyleProperty>>& getProperties() const { return properties_; }
    
    // CSS规则
    void addRule(std::shared_ptr<class StyleRule> rule);
    const std::vector<std::shared_ptr<class StyleRule>>& getRules() const { return rules_; }
    
    // 检查方法
    bool hasInlineStyles() const { return !properties_.empty(); }
    bool hasRules() const { return !rules_.empty(); }
    bool isEmpty() const { return properties_.empty() && rules_.empty(); }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    std::vector<std::shared_ptr<class StyleProperty>> properties_;
    std::vector<std::shared_ptr<class StyleRule>> rules_;
};

/**
 * 样式属性节点
 */
class StyleProperty : public ASTNode {
public:
    StyleProperty(const std::string& property, const std::string& value, 
                  const SourceLocation& location = {})
        : ASTNode(ASTNodeType::STYLE_PROPERTY, location), property_(property), value_(value) {}
    
    const std::string& getProperty() const { return property_; }
    const std::string& getValue() const { return value_; }
    void setValue(const std::string& value) { value_ = value; }
    
    // CSS属性验证
    bool isValidProperty() const;
    bool isValidValue() const;
    bool isShorthandProperty() const;
    std::vector<std::string> expandShorthand() const;
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
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
    explicit Selector(const std::string& selector, SelectorType type, 
                      const SourceLocation& location = {})
        : ASTNode(ASTNodeType::SELECTOR, location), selector_(selector), selectorType_(type) {}
    
    const std::string& getSelector() const { return selector_; }
    SelectorType getSelectorType() const { return selectorType_; }
    
    // 选择器分析
    std::string getNormalizedSelector() const;
    int getSpecificity() const;
    bool isContextual() const { return selectorType_ == SelectorType::CONTEXT; }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    std::string selector_;
    SelectorType selectorType_;
};

/**
 * CSS规则节点
 */
class StyleRule : public ASTNode {
public:
    explicit StyleRule(std::shared_ptr<Selector> selector, const SourceLocation& location = {})
        : ASTNode(ASTNodeType::STYLE_RULE, location), selector_(selector) {}
    
    std::shared_ptr<Selector> getSelector() const { return selector_; }
    void setSelector(std::shared_ptr<Selector> selector) { selector_ = selector; }
    
    void addProperty(std::shared_ptr<StyleProperty> property);
    const std::vector<std::shared_ptr<StyleProperty>>& getProperties() const { return properties_; }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    std::shared_ptr<Selector> selector_;
    std::vector<std::shared_ptr<StyleProperty>> properties_;
};

/**
 * 模板定义基类
 */
class TemplateDefinition : public ASTNode {
public:
    TemplateDefinition(ASTNodeType type, const std::string& name, TemplateType templateType,
                       const SourceLocation& location = {})
        : ASTNode(type, location), name_(name), templateType_(templateType) {}
    
    const std::string& getName() const { return name_; }
    TemplateType getTemplateType() const { return templateType_; }
    
    // 继承支持
    void addInheritance(std::shared_ptr<class TemplateUsage> inheritance);
    const std::vector<std::shared_ptr<class TemplateUsage>>& getInheritances() const { return inheritances_; }
    
    virtual std::string getFullyQualifiedName() const;

protected:
    std::string name_;
    TemplateType templateType_;
    std::vector<std::shared_ptr<class TemplateUsage>> inheritances_;
};

/**
 * 模板样式定义
 */
class TemplateStyle : public TemplateDefinition {
public:
    explicit TemplateStyle(const std::string& name, const SourceLocation& location = {})
        : TemplateDefinition(ASTNodeType::TEMPLATE_STYLE, name, TemplateType::STYLE, location) {}
    
    void addProperty(std::shared_ptr<StyleProperty> property);
    const std::vector<std::shared_ptr<StyleProperty>>& getProperties() const { return properties_; }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    std::vector<std::shared_ptr<StyleProperty>> properties_;
};

/**
 * 模板元素定义
 */
class TemplateElement : public TemplateDefinition {
public:
    explicit TemplateElement(const std::string& name, const SourceLocation& location = {})
        : TemplateDefinition(ASTNodeType::TEMPLATE_ELEMENT, name, TemplateType::ELEMENT, location) {}
    
    void addChild(ASTNodePtr child) { children_.push_back(std::move(child)); }
    const std::vector<ASTNodePtr>& getChildren() const { return children_; }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    std::vector<ASTNodePtr> children_;
};

/**
 * 模板变量定义
 */
class TemplateVariable : public TemplateDefinition {
public:
    explicit TemplateVariable(const std::string& name, const SourceLocation& location = {})
        : TemplateDefinition(ASTNodeType::TEMPLATE_VARIABLE, name, TemplateType::VARIABLE, location) {}
    
    void addVariable(const std::string& name, const std::string& value);
    void removeVariable(const std::string& name);
    std::string getVariable(const std::string& name) const;
    const std::unordered_map<std::string, std::string>& getVariables() const { return variables_; }
    bool hasVariable(const std::string& name) const;
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    std::unordered_map<std::string, std::string> variables_;
};

/**
 * 模板使用节点
 */
class TemplateUsage : public ASTNode {
public:
    TemplateUsage(TemplateType type, const std::string& name, const SourceLocation& location = {})
        : ASTNode(ASTNodeType::TEMPLATE_USAGE, location), templateType_(type), name_(name) {}
    
    TemplateType getTemplateType() const { return templateType_; }
    const std::string& getName() const { return name_; }
    const std::string& getFromNamespace() const { return fromNamespace_; }
    
    void setFromNamespace(const std::string& fromNamespace) { fromNamespace_ = fromNamespace; }
    
    // 参数支持
    void addArgument(const std::string& name, const std::string& value);
    const std::unordered_map<std::string, std::string>& getArguments() const { return arguments_; }
    bool hasArguments() const { return !arguments_.empty(); }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    TemplateType templateType_;
    std::string name_;
    std::string fromNamespace_;
    std::unordered_map<std::string, std::string> arguments_;
};

/**
 * 命名空间定义
 */
class NamespaceDefinition : public ASTNode {
public:
    explicit NamespaceDefinition(const std::string& name, const SourceLocation& location = {})
        : ASTNode(ASTNodeType::NAMESPACE_DEFINITION, location), name_(name) {}
    
    const std::string& getName() const { return name_; }
    bool isNested() const { return name_.find('.') != std::string::npos; }
    
    void addChild(ASTNodePtr child) { children_.push_back(std::move(child)); }
    const std::vector<ASTNodePtr>& getChildren() const { return children_; }
    
    // 特定类型的子节点访问
    std::vector<std::shared_ptr<TemplateDefinition>> getTemplates() const;
    std::vector<std::shared_ptr<ImportStatement>> getImports() const;
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    std::string name_;
    std::vector<ASTNodePtr> children_;
};

/**
 * 导入语句
 */
class ImportStatement : public ASTNode {
public:
    ImportStatement(ImportType type, const std::string& path, const SourceLocation& location = {})
        : ASTNode(ASTNodeType::IMPORT_STATEMENT, location), importType_(type), path_(path) {}
    
    ImportType getImportType() const { return importType_; }
    const std::string& getPath() const { return path_; }
    const std::string& getAlias() const { return alias_; }
    const std::string& getFromClause() const { return fromClause_; }
    
    void setAlias(const std::string& alias) { alias_ = alias; }
    void setFromClause(const std::string& fromClause) { fromClause_ = fromClause; }
    
    // 导入路径分析
    bool isRelativeImport() const;
    bool isAbsoluteImport() const;
    std::string getResolvedPath() const;
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    ImportType importType_;
    std::string path_;
    std::string alias_;
    std::string fromClause_;
};

/**
 * 注释节点
 */
class Comment : public ASTNode {
public:
    enum class CommentType {
        SINGLE_LINE,    // //
        MULTI_LINE,     // /* */
        GENERATOR       // --
    };
    
    Comment(CommentType type, const std::string& content, const SourceLocation& location = {})
        : ASTNode(ASTNodeType::COMMENT, location), commentType_(type), content_(content) {}
    
    CommentType getCommentType() const { return commentType_; }
    const std::string& getContent() const { return content_; }
    bool isGeneratorComment() const { return commentType_ == CommentType::GENERATOR; }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodeUniquePtr clone() const override;
    std::string toString() const override;

private:
    CommentType commentType_;
    std::string content_;
};

} // namespace CHTL