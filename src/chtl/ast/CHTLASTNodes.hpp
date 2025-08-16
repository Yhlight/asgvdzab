#pragma once

#include "ASTNode.hpp"
#include <unordered_map>

namespace chtl {
namespace ast {

/**
 * 程序根节点
 */
class ProgramNode : public ASTNode {
public:
    ProgramNode() : ASTNode(ASTNodeType::PROGRAM) {}
    void accept(ASTVisitor& visitor) override;
};

/**
 * 元素节点 (div, span, html等)
 */
class ElementNode : public ASTNode {
public:
    ElementNode(const std::string& tagName, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::ELEMENT, line, column), tagName_(tagName) {}
    
    void accept(ASTVisitor& visitor) override;
    
    const std::string& getTagName() const { return tagName_; }
    
    // 属性管理
    void addAttribute(const std::string& name, const std::string& value) {
        attributes_[name] = value;
    }
    
    const std::unordered_map<std::string, std::string>& getAttributes() const {
        return attributes_;
    }
    
private:
    std::string tagName_;
    std::unordered_map<std::string, std::string> attributes_;
};

/**
 * 文本节点
 */
class TextNode : public ASTNode {
public:
    TextNode(size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::TEXT, line, column) {}
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * 属性节点
 */
class AttributeNode : public ASTNode {
public:
    AttributeNode(const std::string& name, ASTNodePtr value, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::ATTRIBUTE, line, column), name_(name), value_(value) {}
    
    void accept(ASTVisitor& visitor) override;
    
    const std::string& getName() const { return name_; }
    ASTNodePtr getValue() const { return value_; }
    
private:
    std::string name_;
    ASTNodePtr value_;
};

/**
 * 注释节点
 */
class CommentNode : public ASTNode {
public:
    enum CommentType {
        LINE_COMMENT,    // //
        BLOCK_COMMENT,   // /* */
        HTML_COMMENT     // --
    };
    
    CommentNode(CommentType commentType, const std::string& content, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::COMMENT, line, column), commentType_(commentType), content_(content) {}
    
    void accept(ASTVisitor& visitor) override;
    
    CommentType getCommentType() const { return commentType_; }
    const std::string& getContent() const { return content_; }
    
private:
    CommentType commentType_;
    std::string content_;
};

/**
 * 局部样式块节点
 */
class StyleBlockNode : public ASTNode {
public:
    StyleBlockNode(size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::STYLE_BLOCK, line, column) {}
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * 样式规则节点 (.class { ... })
 */
class StyleRuleNode : public ASTNode {
public:
    StyleRuleNode(ASTNodePtr selector, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::STYLE_RULE, line, column), selector_(selector) {}
    
    void accept(ASTVisitor& visitor) override;
    
    ASTNodePtr getSelector() const { return selector_; }
    
private:
    ASTNodePtr selector_;
};

/**
 * 样式选择器节点
 */
class StyleSelectorNode : public ASTNode {
public:
    enum SelectorType {
        CLASS_SELECTOR,      // .class
        ID_SELECTOR,         // #id
        ELEMENT_SELECTOR,    // element
        AMPERSAND_SELECTOR,  // &
        PSEUDO_CLASS,        // :hover
        PSEUDO_ELEMENT,      // ::before
        COMPLEX_SELECTOR     // 复合选择器
    };
    
    StyleSelectorNode(SelectorType selectorType, const std::string& value, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::STYLE_SELECTOR, line, column), selectorType_(selectorType), value_(value) {}
    
    void accept(ASTVisitor& visitor) override;
    
    SelectorType getSelectorType() const { return selectorType_; }
    const std::string& getValue() const { return value_; }
    
private:
    SelectorType selectorType_;
    std::string value_;
};

/**
 * 样式属性节点
 */
class StylePropertyNode : public ASTNode {
public:
    StylePropertyNode(const std::string& property, ASTNodePtr value, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::STYLE_PROPERTY, line, column), property_(property), value_(value) {}
    
    void accept(ASTVisitor& visitor) override;
    
    const std::string& getProperty() const { return property_; }
    ASTNodePtr getValue() const { return value_; }
    
private:
    std::string property_;
    ASTNodePtr value_;
};

/**
 * 局部脚本块节点
 */
class ScriptBlockNode : public ASTNode {
public:
    ScriptBlockNode(size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::SCRIPT_BLOCK, line, column) {}
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * 模板定义节点 [Template]
 */
class TemplateDefNode : public ASTNode {
public:
    enum TemplateType {
        STYLE_TEMPLATE,
        ELEMENT_TEMPLATE,
        VAR_TEMPLATE
    };
    
    TemplateDefNode(TemplateType templateType, const std::string& name, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::TEMPLATE_DEF, line, column), templateType_(templateType), name_(name) {}
    
    void accept(ASTVisitor& visitor) override;
    
    TemplateType getTemplateType() const { return templateType_; }
    const std::string& getName() const { return name_; }
    
private:
    TemplateType templateType_;
    std::string name_;
};

/**
 * 自定义定义节点 [Custom]
 */
class CustomDefNode : public ASTNode {
public:
    enum CustomType {
        STYLE_CUSTOM,
        ELEMENT_CUSTOM,
        VAR_CUSTOM
    };
    
    CustomDefNode(CustomType customType, const std::string& name, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::CUSTOM_DEF, line, column), customType_(customType), name_(name) {}
    
    void accept(ASTVisitor& visitor) override;
    
    CustomType getCustomType() const { return customType_; }
    const std::string& getName() const { return name_; }
    
private:
    CustomType customType_;
    std::string name_;
};

/**
 * 模板使用节点
 */
class TemplateUseNode : public ASTNode {
public:
    TemplateUseNode(const std::string& templateName, const std::string& type, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::TEMPLATE_USE, line, column), templateName_(templateName), type_(type) {}
    
    void accept(ASTVisitor& visitor) override;
    
    const std::string& getTemplateName() const { return templateName_; }
    const std::string& getType() const { return type_; }
    
private:
    std::string templateName_;
    std::string type_;  // @Style, @Element, @Var
};

/**
 * 自定义使用节点
 */
class CustomUseNode : public ASTNode {
public:
    CustomUseNode(const std::string& customName, const std::string& type, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::CUSTOM_USE, line, column), customName_(customName), type_(type) {}
    
    void accept(ASTVisitor& visitor) override;
    
    const std::string& getCustomName() const { return customName_; }
    const std::string& getType() const { return type_; }
    
private:
    std::string customName_;
    std::string type_;  // @Style, @Element, @Var
};

/**
 * delete操作节点
 */
class DeleteOpNode : public ASTNode {
public:
    DeleteOpNode(size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::DELETE_OP, line, column) {}
    
    void accept(ASTVisitor& visitor) override;
    
    void addTarget(const std::string& target) {
        targets_.push_back(target);
    }
    
    const std::vector<std::string>& getTargets() const { return targets_; }
    
private:
    std::vector<std::string> targets_;
};

/**
 * insert操作节点
 */
class InsertOpNode : public ASTNode {
public:
    enum InsertPosition {
        AFTER,
        BEFORE,
        REPLACE,
        AT_TOP,
        AT_BOTTOM
    };
    
    InsertOpNode(InsertPosition position, const std::string& selector, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::INSERT_OP, line, column), position_(position), selector_(selector) {}
    
    void accept(ASTVisitor& visitor) override;
    
    InsertPosition getPosition() const { return position_; }
    const std::string& getSelector() const { return selector_; }
    
private:
    InsertPosition position_;
    std::string selector_;
};

/**
 * inherit操作节点
 */
class InheritOpNode : public ASTNode {
public:
    InheritOpNode(const std::string& target, const std::string& type, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::INHERIT_OP, line, column), target_(target), type_(type) {}
    
    void accept(ASTVisitor& visitor) override;
    
    const std::string& getTarget() const { return target_; }
    const std::string& getType() const { return type_; }
    
private:
    std::string target_;
    std::string type_;  // @Style, @Element, @Var
};

/**
 * 原始嵌入节点 [Origin]
 */
class OriginBlockNode : public ASTNode {
public:
    enum OriginType {
        HTML_ORIGIN,
        STYLE_ORIGIN,
        JAVASCRIPT_ORIGIN
    };
    
    OriginBlockNode(OriginType originType, const std::string& name = "", size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::ORIGIN_BLOCK, line, column), originType_(originType), name_(name) {}
    
    void accept(ASTVisitor& visitor) override;
    
    OriginType getOriginType() const { return originType_; }
    const std::string& getName() const { return name_; }
    void setContent(const std::string& content) { content_ = content; }
    const std::string& getContent() const { return content_; }
    
private:
    OriginType originType_;
    std::string name_;
    std::string content_;
};

/**
 * 导入语句节点 [Import]
 */
class ImportStmtNode : public ASTNode {
public:
    enum ImportType {
        HTML_IMPORT,
        STYLE_IMPORT,
        JAVASCRIPT_IMPORT,
        CHTL_IMPORT,
        CJMOD_IMPORT,
        TEMPLATE_IMPORT,
        CUSTOM_IMPORT
    };
    
    ImportStmtNode(ImportType importType, const std::string& path, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::IMPORT_STMT, line, column), importType_(importType), path_(path) {}
    
    void accept(ASTVisitor& visitor) override;
    
    ImportType getImportType() const { return importType_; }
    const std::string& getPath() const { return path_; }
    void setAlias(const std::string& alias) { alias_ = alias; }
    const std::string& getAlias() const { return alias_; }
    void setTarget(const std::string& target) { target_ = target; }
    const std::string& getTarget() const { return target_; }
    
private:
    ImportType importType_;
    std::string path_;
    std::string alias_;
    std::string target_;  // 对于模板/自定义导入，指定要导入的名称
};

/**
 * 命名空间定义节点 [Namespace]
 */
class NamespaceDefNode : public ASTNode {
public:
    NamespaceDefNode(const std::string& name, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::NAMESPACE_DEF, line, column), name_(name) {}
    
    void accept(ASTVisitor& visitor) override;
    
    const std::string& getName() const { return name_; }
    
private:
    std::string name_;
};

/**
 * 配置块节点 [Configuration]
 */
class ConfigBlockNode : public ASTNode {
public:
    ConfigBlockNode(size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::CONFIG_BLOCK, line, column) {}
    
    void accept(ASTVisitor& visitor) override;
    
    void addConfig(const std::string& key, const std::string& value) {
        configs_[key] = value;
    }
    
    const std::unordered_map<std::string, std::string>& getConfigs() const {
        return configs_;
    }
    
private:
    std::unordered_map<std::string, std::string> configs_;
};

/**
 * except约束节点
 */
class ExceptConstraintNode : public ASTNode {
public:
    ExceptConstraintNode(size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::EXCEPT_CONSTRAINT, line, column) {}
    
    void accept(ASTVisitor& visitor) override;
    
    void addTarget(const std::string& target) {
        targets_.push_back(target);
    }
    
    const std::vector<std::string>& getTargets() const { return targets_; }
    
private:
    std::vector<std::string> targets_;
};

/**
 * 字符串字面量节点
 */
class StringLiteralNode : public ASTNode {
public:
    StringLiteralNode(const std::string& value, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::STRING_LITERAL, line, column), value_(value) {}
    
    void accept(ASTVisitor& visitor) override;
    
    const std::string& getValue() const { return value_; }
    
private:
    std::string value_;
};

/**
 * 数字字面量节点
 */
class NumberLiteralNode : public ASTNode {
public:
    NumberLiteralNode(const std::string& value, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::NUMBER_LITERAL, line, column), value_(value) {}
    
    void accept(ASTVisitor& visitor) override;
    
    const std::string& getValue() const { return value_; }
    double getNumericValue() const { return std::stod(value_); }
    
private:
    std::string value_;
};

/**
 * 无引号字面量节点
 */
class UnquotedLiteralNode : public ASTNode {
public:
    UnquotedLiteralNode(const std::string& value, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::UNQUOTED_LITERAL, line, column), value_(value) {}
    
    void accept(ASTVisitor& visitor) override;
    
    const std::string& getValue() const { return value_; }
    
private:
    std::string value_;
};

/**
 * 标识符节点
 */
class IdentifierNode : public ASTNode {
public:
    IdentifierNode(const std::string& name, size_t line = 0, size_t column = 0)
        : ASTNode(ASTNodeType::IDENTIFIER, line, column), name_(name) {}
    
    void accept(ASTVisitor& visitor) override;
    
    const std::string& getName() const { return name_; }
    
private:
    std::string name_;
};

} // namespace ast
} // namespace chtl