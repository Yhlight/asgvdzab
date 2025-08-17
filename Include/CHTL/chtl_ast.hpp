#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <variant>
#include "common/types.hpp"

namespace chtl {

// 前向声明
class CHTLASTNode;
using CHTLASTNodePtr = std::shared_ptr<CHTLASTNode>;

/**
 * CHTL AST节点类型枚举
 * 严格按照CHTL语法文档.md定义
 */
enum class CHTLASTNodeType {
    // 根节点
    PROGRAM,                    // 整个CHTL程序

    // 注释节点
    COMMENT_LINE,               // // 注释
    COMMENT_MULTILINE,          // /* */ 注释
    COMMENT_GENERATOR,          // -- 生成器识别的注释

    // 基础节点
    TEXT_NODE,                  // text { } 文本节点
    ELEMENT_NODE,               // HTML元素节点 (div, span, etc.)
    ATTRIBUTE_NODE,             // 属性节点 (id: value;)

    // 样式相关节点
    STYLE_BLOCK,                // style { } 局部样式块
    INLINE_STYLE,               // 内联样式属性
    CLASS_SELECTOR,             // .className 类选择器 
    ID_SELECTOR,                // #idName ID选择器
    PSEUDO_CLASS,               // &:hover 伪类选择器
    PSEUDO_ELEMENT,             // &::before 伪元素选择器
    CONTEXT_REFERENCE,          // & 上下文引用

    // 模板节点
    TEMPLATE_DECLARATION,       // [Template] 声明
    TEMPLATE_STYLE,             // [Template] @Style
    TEMPLATE_ELEMENT,           // [Template] @Element  
    TEMPLATE_VAR,               // [Template] @Var
    TEMPLATE_USAGE,             // 模板使用 @Style Name;

    // 自定义节点
    CUSTOM_DECLARATION,         // [Custom] 声明
    CUSTOM_STYLE,               // [Custom] @Style
    CUSTOM_ELEMENT,             // [Custom] @Element
    CUSTOM_VAR,                 // [Custom] @Var
    CUSTOM_USAGE,               // 自定义使用

    // 自定义特例化操作
    CUSTOM_SPECIALIZATION,      // 特例化块
    DELETE_OPERATION,           // delete 操作
    INSERT_OPERATION,           // insert 操作
    REPLACE_OPERATION,          // replace 操作
    INDEX_ACCESS,               // [index] 索引访问

    // 继承相关
    INHERIT_STATEMENT,          // inherit 显式继承
    COMPOSITION_INHERIT,        // 组合式继承

    // 变量组相关
    VAR_GROUP,                  // 变量组
    VAR_PROPERTY,               // 变量属性
    VAR_USAGE,                  // 变量使用 VarName(prop)
    VAR_SPECIALIZATION,         // 变量特例化 VarName(prop = value)

    // 原始嵌入
    ORIGIN_DECLARATION,         // [Origin] 声明
    ORIGIN_HTML,                // [Origin] @Html
    ORIGIN_STYLE,               // [Origin] @Style  
    ORIGIN_JAVASCRIPT,          // [Origin] @JavaScript

    // 导入相关
    IMPORT_DECLARATION,         // [Import] 声明
    IMPORT_HTML,                // [Import] @Html
    IMPORT_STYLE,               // [Import] @Style
    IMPORT_JAVASCRIPT,          // [Import] @JavaScript
    IMPORT_CHTL,                // [Import] @Chtl
    IMPORT_CUSTOM,              // [Import] [Custom] @Element
    IMPORT_TEMPLATE,            // [Import] [Template] @Style
    IMPORT_CJMOD,               // [Import] @CJmod

    // 命名空间
    NAMESPACE_DECLARATION,      // [Namespace] 声明
    NAMESPACE_USAGE,            // from namespace 使用

    // 配置
    CONFIGURATION_DECLARATION,  // [Configuration] 声明
    CONFIG_PROPERTY,            // 配置属性

    // CMOD相关
    INFO_DECLARATION,           // [Info] 声明 (CMOD)
    EXPORT_DECLARATION,         // [Export] 声明 (CMOD)

    // 约束相关
    EXCEPT_STATEMENT,           // except 约束语句

    // 脚本块 (不包含CHTL JS扩展语法)
    SCRIPT_BLOCK,               // script { } 脚本块

    // 字面量
    STRING_LITERAL,             // 字符串字面量 "text", 'text', text
    IDENTIFIER,                 // 标识符

    // 位置相关
    INSERT_POSITION,            // after, before, replace, at top, at bottom

    // 其他
    PROPERTY_LIST,              // 属性列表
    BLOCK_STATEMENT,            // 块语句 { }
    UNKNOWN                     // 未知节点
};

/**
 * 插入位置枚举 (严格按照文档)
 */
enum class InsertPosition {
    AFTER,                      // after
    BEFORE,                     // before  
    REPLACE,                    // replace
    AT_TOP,                     // at top
    AT_BOTTOM                   // at bottom
};

/**
 * 字面量类型枚举
 */
enum class LiteralType {
    DOUBLE_QUOTED,              // "text"
    SINGLE_QUOTED,              // 'text'
    UNQUOTED                    // text (无修饰字面量)
};

/**
 * CHTL AST基础节点类
 */
class CHTLASTNode {
public:
    CHTLASTNodeType type;
    Position position;
    std::vector<CHTLASTNodePtr> children;
    CHTLASTNode* parent = nullptr;

    CHTLASTNode(CHTLASTNodeType t, Position pos = Position{})
        : type(t), position(pos) {}

    virtual ~CHTLASTNode() = default;

    // 添加子节点
    void addChild(CHTLASTNodePtr child);
    
    // 获取特定类型的子节点
    std::vector<CHTLASTNodePtr> getChildrenByType(CHTLASTNodeType targetType) const;
    
    // 查找特定类型的第一个子节点
    CHTLASTNodePtr findChild(CHTLASTNodeType targetType) const;

    // 节点类型检查
    bool isTemplateNode() const;
    bool isCustomNode() const;
    bool isStyleNode() const;
    bool isElementNode() const;
    bool isCommentNode() const;

    // 虚函数供子类重写
    virtual std::string toString() const;
    virtual CHTLASTNodePtr clone() const;
};

/**
 * 文本节点
 * 对应: text { "content" } 或 text { content }
 */
class TextNode : public CHTLASTNode {
public:
    std::string content;
    LiteralType literalType;

    TextNode(const std::string& text, LiteralType type = LiteralType::UNQUOTED, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::TEXT_NODE, pos), content(text), literalType(type) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 元素节点
 * 对应: div { }, span { }, etc.
 */
class ElementNode : public CHTLASTNode {
public:
    std::string tagName;
    std::vector<CHTLASTNodePtr> attributes;
    CHTLASTNodePtr styleBlock = nullptr;
    CHTLASTNodePtr scriptBlock = nullptr;

    ElementNode(const std::string& tag, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::ELEMENT_NODE, pos), tagName(tag) {}

    void addAttribute(CHTLASTNodePtr attr);
    void setStyleBlock(CHTLASTNodePtr style);
    void setScriptBlock(CHTLASTNodePtr script);

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 属性节点
 * 对应: id: value; class: "className";
 */
class AttributeNode : public CHTLASTNode {
public:
    std::string name;
    std::string value;
    LiteralType valueType;

    AttributeNode(const std::string& attrName, const std::string& attrValue, 
                  LiteralType type = LiteralType::UNQUOTED, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::ATTRIBUTE_NODE, pos), 
          name(attrName), value(attrValue), valueType(type) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 样式块节点
 * 对应: style { ... }
 */
class StyleBlockNode : public CHTLASTNode {
public:
    StyleBlockNode(Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::STYLE_BLOCK, pos) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 类选择器节点
 * 对应: .className { ... }
 */
class ClassSelectorNode : public CHTLASTNode {
public:
    std::string className;
    std::vector<CHTLASTNodePtr> properties;

    ClassSelectorNode(const std::string& name, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::CLASS_SELECTOR, pos), className(name) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * ID选择器节点
 * 对应: #idName { ... }
 */
class IDSelectorNode : public CHTLASTNode {
public:
    std::string idName;
    std::vector<CHTLASTNodePtr> properties;

    IDSelectorNode(const std::string& name, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::ID_SELECTOR, pos), idName(name) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 内联样式节点
 * 对应: style { width: 100px; height: 200px; }
 */
class InlineStyleNode : public CHTLASTNode {
public:
    std::string property;
    std::string value;

    InlineStyleNode(const std::string& prop, const std::string& val, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::INLINE_STYLE, pos), property(prop), value(val) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 上下文引用节点
 * 对应: &:hover, &::before
 */
class ContextReferenceNode : public CHTLASTNode {
public:
    std::string selector;       // :hover, ::before, etc.
    std::vector<CHTLASTNodePtr> properties;

    ContextReferenceNode(const std::string& sel, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::CONTEXT_REFERENCE, pos), selector(sel) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 模板声明节点
 * 对应: [Template] @Style/@Element/@Var Name { ... }
 */
class TemplateDeclarationNode : public CHTLASTNode {
public:
    std::string templateType;   // @Style, @Element, @Var
    std::string name;

    TemplateDeclarationNode(const std::string& type, const std::string& templateName, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::TEMPLATE_DECLARATION, pos), 
          templateType(type), name(templateName) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 模板使用节点
 * 对应: @Style TemplateName;
 */
class TemplateUsageNode : public CHTLASTNode {
public:
    std::string templateType;   // @Style, @Element, @Var
    std::string name;

    TemplateUsageNode(const std::string& type, const std::string& templateName, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::TEMPLATE_USAGE, pos), 
          templateType(type), name(templateName) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 自定义声明节点
 * 对应: [Custom] @Style/@Element/@Var Name { ... }
 */
class CustomDeclarationNode : public CHTLASTNode {
public:
    std::string customType;     // @Style, @Element, @Var
    std::string name;

    CustomDeclarationNode(const std::string& type, const std::string& customName, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::CUSTOM_DECLARATION, pos), 
          customType(type), name(customName) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 自定义特例化节点
 * 对应自定义元素的特例化操作
 */
class CustomSpecializationNode : public CHTLASTNode {
public:
    std::string customName;

    CustomSpecializationNode(const std::string& name, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::CUSTOM_SPECIALIZATION, pos), customName(name) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 删除操作节点
 * 对应: delete selector; delete element[index];
 */
class DeleteOperationNode : public CHTLASTNode {
public:
    std::string selector;
    int index = -1;             // -1表示无索引

    DeleteOperationNode(const std::string& sel, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::DELETE_OPERATION, pos), selector(sel) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 插入操作节点
 * 对应: insert after/before/replace/at top/at bottom selector { ... }
 */
class InsertOperationNode : public CHTLASTNode {
public:
    InsertPosition position;
    std::string selector;
    int index = -1;             // -1表示无索引

    InsertOperationNode(InsertPosition pos, const std::string& sel, Position astPos = Position{})
        : CHTLASTNode(CHTLASTNodeType::INSERT_OPERATION, astPos), position(pos), selector(sel) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 索引访问节点
 * 对应: element[index]
 */
class IndexAccessNode : public CHTLASTNode {
public:
    std::string element;
    int index;

    IndexAccessNode(const std::string& elem, int idx, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::INDEX_ACCESS, pos), element(elem), index(idx) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 继承语句节点
 * 对应: inherit @Style TemplateName;
 */
class InheritStatementNode : public CHTLASTNode {
public:
    std::string inheritType;    // @Style, @Element, @Var
    std::string name;

    InheritStatementNode(const std::string& type, const std::string& inheritName, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::INHERIT_STATEMENT, pos), 
          inheritType(type), name(inheritName) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 变量组节点
 * 对应: [Template/@Custom] @Var Name { prop: value; }
 */
class VarGroupNode : public CHTLASTNode {
public:
    std::string name;
    std::vector<CHTLASTNodePtr> properties;

    VarGroupNode(const std::string& varName, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::VAR_GROUP, pos), name(varName) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 变量使用节点
 * 对应: VarName(property) 或 VarName(prop = value)
 */
class VarUsageNode : public CHTLASTNode {
public:
    std::string varName;
    std::string property;
    std::string value;          // 空字符串表示无特例化值
    bool hasSpecialization;

    VarUsageNode(const std::string& name, const std::string& prop, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::VAR_USAGE, pos), 
          varName(name), property(prop), hasSpecialization(false) {}

    void setSpecialization(const std::string& val) {
        value = val;
        hasSpecialization = true;
    }

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 导入声明节点
 * 对应: [Import] @Html/@Style/@JavaScript/@Chtl/@CJmod from path as name
 */
class ImportDeclarationNode : public CHTLASTNode {
public:
    std::string importType;     // @Html, @Style, @JavaScript, @Chtl, @CJmod
    std::string targetName;     // 导入的目标名称
    std::string path;
    std::string alias;          // as后的别名，可为空

    ImportDeclarationNode(const std::string& type, const std::string& target, 
                         const std::string& importPath, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::IMPORT_DECLARATION, pos), 
          importType(type), targetName(target), path(importPath) {}

    void setAlias(const std::string& aliasName) { alias = aliasName; }

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 命名空间声明节点
 * 对应: [Namespace] name { ... }
 */
class NamespaceDeclarationNode : public CHTLASTNode {
public:
    std::string name;

    NamespaceDeclarationNode(const std::string& namespaceName, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::NAMESPACE_DECLARATION, pos), name(namespaceName) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 配置声明节点
 * 对应: [Configuration] { ... }
 */
class ConfigurationDeclarationNode : public CHTLASTNode {
public:
    std::vector<CHTLASTNodePtr> properties;

    ConfigurationDeclarationNode(Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::CONFIGURATION_DECLARATION, pos) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 原始嵌入声明节点
 * 对应: [Origin] @Html/@Style/@JavaScript { ... }
 */
class OriginDeclarationNode : public CHTLASTNode {
public:
    std::string originType;     // @Html, @Style, @JavaScript
    std::string content;

    OriginDeclarationNode(const std::string& type, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::ORIGIN_DECLARATION, pos), originType(type) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 原始嵌入节点
 * 对应原始嵌入的通用节点
 */
class OriginEmbedNode : public CHTLASTNode {
public:
    std::string originType;     // @Html, @Style, @JavaScript
    std::string content;        // 原始代码内容
    std::string name;           // 嵌入名称 (用于增强模式)

    OriginEmbedNode(const std::string& type, const std::string& embedContent, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::ORIGIN_DECLARATION, pos), originType(type), content(embedContent) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 约束语句节点
 * 对应: except @Html; except [Template] @Var;
 */
class ExceptStatementNode : public CHTLASTNode {
public:
    std::string constraint;     // 约束内容

    ExceptStatementNode(const std::string& constraintText, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::EXCEPT_STATEMENT, pos), constraint(constraintText) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 脚本块节点
 * 对应: script { ... } (不包含CHTL JS扩展语法)
 */
class ScriptBlockNode : public CHTLASTNode {
public:
    std::string content;

    ScriptBlockNode(Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::SCRIPT_BLOCK, pos) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 字符串字面量节点
 */
class StringLiteralNode : public CHTLASTNode {
public:
    std::string value;
    LiteralType literalType;

    StringLiteralNode(const std::string& val, LiteralType type, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::STRING_LITERAL, pos), value(val), literalType(type) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 标识符节点
 */
class IdentifierNode : public CHTLASTNode {
public:
    std::string name;

    IdentifierNode(const std::string& identifier, Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::IDENTIFIER, pos), name(identifier) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 程序根节点
 */
class ProgramNode : public CHTLASTNode {
public:
    ProgramNode(Position pos = Position{})
        : CHTLASTNode(CHTLASTNodeType::PROGRAM, pos) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

/**
 * 注释节点
 */
class CommentNode : public CHTLASTNode {
public:
    std::string content;
    CHTLASTNodeType commentType; // COMMENT_LINE, COMMENT_MULTILINE, COMMENT_GENERATOR

    CommentNode(const std::string& text, CHTLASTNodeType type, Position pos = Position{})
        : CHTLASTNode(type, pos), content(text), commentType(type) {}

    std::string toString() const override;
    CHTLASTNodePtr clone() const override;
};

} // namespace chtl