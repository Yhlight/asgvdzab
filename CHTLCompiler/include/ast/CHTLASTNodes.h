#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

// 注释节点
class CommentNode : public ASTNode {
public:
    std::string comment_type;       // 注释类型：//, /* */, --
    std::string content;            // 注释内容
    bool is_generator_comment;      // 是否为生成器注释
    
    CommentNode(const std::string& type, const std::string& cont, bool is_gen = false,
                size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::COMMENT, l, c, start, end), 
          comment_type(type), content(cont), is_generator_comment(is_gen) {}
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 文本节点
class TextNode : public ASTNode {
public:
    std::string text_content;       // 文本内容
    bool is_quoted;                 // 是否为引号包围的文本
    
    TextNode(const std::string& content, bool quoted = false,
             size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::TEXT_NODE, l, c, start, end), 
          text_content(content), is_quoted(quoted) {}
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 字面量节点
class LiteralNode : public ASTNode {
public:
    std::string value;              // 字面量值
    std::string quote_type;         // 引号类型：", ', 或无引号
    
    LiteralNode(const std::string& val, const std::string& quote = "",
                size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::LITERAL, l, c, start, end), 
          value(val), quote_type(quote) {}
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// HTML元素节点
class HTMLElementNode : public ASTNode {
public:
    std::string tag_name;           // 标签名
    std::vector<std::unique_ptr<ASTNode>> attributes; // 属性列表
    std::vector<std::unique_ptr<ASTNode>> children;   // 子元素列表
    bool is_self_closing;           // 是否为自闭合标签
    
    HTMLElementNode(const std::string& tag, bool self_closing = false,
                    size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::HTML_ELEMENT, l, c, start, end), 
          tag_name(tag), is_self_closing(self_closing) {}
    
    void addAttribute(std::unique_ptr<ASTNode> attr);
    void addChild(std::unique_ptr<ASTNode> child);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 属性节点
class AttributeNode : public ASTNode {
public:
    std::string name;               // 属性名
    std::unique_ptr<ASTNode> value; // 属性值
    std::string separator;          // 分隔符：: 或 =
    
    AttributeNode(const std::string& attr_name, std::unique_ptr<ASTNode> attr_value,
                  const std::string& sep = ":", size_t l = 0, size_t c = 0, 
                  size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::ATTRIBUTE, l, c, start, end), 
          name(attr_name), value(std::move(attr_value)), separator(sep) {}
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 样式块节点
class StyleBlockNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> css_rules; // CSS规则列表
    std::vector<std::unique_ptr<ASTNode>> inline_styles; // 内联样式列表
    
    StyleBlockNode(size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::STYLE_BLOCK, l, c, start, end) {}
    
    void addCSSRule(std::unique_ptr<ASTNode> rule);
    void addInlineStyle(std::unique_ptr<ASTNode> style);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// CSS规则节点
class CSSRuleNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> selector;      // CSS选择器
    std::vector<std::unique_ptr<ASTNode>> properties; // CSS属性列表
    
    CSSRuleNode(std::unique_ptr<ASTNode> sel, size_t l = 0, size_t c = 0, 
                size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::CSS_RULE, l, c, start, end), selector(std::move(sel)) {}
    
    void addProperty(std::unique_ptr<ASTNode> prop);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// CSS选择器节点
class CSSSelectorNode : public ASTNode {
public:
    std::string selector_type;      // 选择器类型：., #, 标签名, 或 &
    std::string selector_value;     // 选择器值
    bool is_context_reference;      // 是否为上下文引用(&)
    
    CSSSelectorNode(const std::string& type, const std::string& value, bool is_context = false,
                    size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::CSS_SELECTOR, l, c, start, end), 
          selector_type(type), selector_value(value), is_context_reference(is_context) {}
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// CSS属性节点
class CSSPropertyNode : public ASTNode {
public:
    std::string property_name;      // 属性名
    std::unique_ptr<ASTNode> property_value; // 属性值
    
    CSSPropertyNode(const std::string& name, std::unique_ptr<ASTNode> value,
                    size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::CSS_PROPERTY, l, c, start, end), 
          property_name(name), property_value(std::move(value)) {}
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 模板声明节点
class TemplateDeclarationNode : public ASTNode {
public:
    std::string template_type;      // 模板类型：@Style, @Element, @Var
    std::string template_name;      // 模板名称
    std::vector<std::unique_ptr<ASTNode>> template_body; // 模板体
    
    TemplateDeclarationNode(const std::string& type, const std::string& name,
                           size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::TEMPLATE_DECLARATION, l, c, start, end), 
          template_type(type), template_name(name) {}
    
    void addTemplateBody(std::unique_ptr<ASTNode> body);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 模板使用节点
class TemplateUsageNode : public ASTNode {
public:
    std::string template_name;      // 使用的模板名称
    std::string template_type;      // 模板类型
    std::vector<std::unique_ptr<ASTNode>> customizations; // 自定义内容
    
    TemplateUsageNode(const std::string& name, const std::string& type,
                      size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::TEMPLATE_USAGE, l, c, start, end), 
          template_name(name), template_type(type) {}
    
    void addCustomization(std::unique_ptr<ASTNode> custom);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 自定义声明节点
class CustomDeclarationNode : public ASTNode {
public:
    std::string custom_type;        // 自定义类型：@Style, @Element, @Var
    std::string custom_name;        // 自定义名称
    std::vector<std::unique_ptr<ASTNode>> custom_body; // 自定义体
    std::vector<std::unique_ptr<ASTNode>> specializations; // 特例化操作
    
    CustomDeclarationNode(const std::string& type, const std::string& name,
                         size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::CUSTOM_DECLARATION, l, c, start, end), 
          custom_type(type), custom_name(name) {}
    
    void addCustomBody(std::unique_ptr<ASTNode> body);
    void addSpecialization(std::unique_ptr<ASTNode> spec);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 删除操作节点
class DeleteOperationNode : public ASTNode {
public:
    std::vector<std::string> delete_targets; // 要删除的目标列表
    
    DeleteOperationNode(const std::vector<std::string>& targets,
                       size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::DELETE_OPERATION, l, c, start, end), 
          delete_targets(targets) {}
    
    void addDeleteTarget(const std::string& target);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 原始嵌入节点
class OriginDeclarationNode : public ASTNode {
public:
    std::string origin_type;        // 原始类型：@Html, @Style, @JavaScript
    std::string origin_content;     // 原始内容
    
    OriginDeclarationNode(const std::string& type, const std::string& content,
                         size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::ORIGIN_DECLARATION, l, c, start, end), 
          origin_type(type), origin_content(content) {}
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 导入声明节点
class ImportDeclarationNode : public ASTNode {
public:
    std::string import_type;        // 导入类型：@Html, @Style, @JavaScript, @Chtl, @CJmod
    std::string import_path;        // 导入路径
    std::string import_name;        // 导入名称（如果有）
    
    ImportDeclarationNode(const std::string& type, const std::string& path, 
                         const std::string& name = "", size_t l = 0, size_t c = 0, 
                         size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::IMPORT_DECLARATION, l, c, start, end), 
          import_type(type), import_path(path), import_name(name) {}
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 命名空间声明节点
class NamespaceDeclarationNode : public ASTNode {
public:
    std::string namespace_name;     // 命名空间名称
    std::vector<std::unique_ptr<ASTNode>> namespace_body; // 命名空间体
    
    NamespaceDeclarationNode(const std::string& name, size_t l = 0, size_t c = 0, 
                            size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::NAMESPACE_DECLARATION, l, c, start, end), 
          namespace_name(name) {}
    
    void addNamespaceBody(std::unique_ptr<ASTNode> body);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 约束声明节点
class ConstraintDeclarationNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> except_clauses; // except子句列表
    
    ConstraintDeclarationNode(size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::CONSTRAINT_DECLARATION, l, c, start, end) {}
    
    void addExceptClause(std::unique_ptr<ASTNode> clause);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 模块信息节点
class InfoDeclarationNode : public ASTNode {
public:
    std::unordered_map<std::string, std::string> info_properties; // 信息属性映射
    
    InfoDeclarationNode(size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::INFO_DECLARATION, l, c, start, end) {}
    
    void addInfoProperty(const std::string& key, const std::string& value);
    std::string getInfoProperty(const std::string& key) const;
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 导出声明节点
class ExportDeclarationNode : public ASTNode {
public:
    std::vector<std::string> exported_styles;      // 导出的样式组
    std::vector<std::string> exported_elements;    // 导出的元素
    std::vector<std::string> exported_vars;        // 导出的变量组
    
    ExportDeclarationNode(size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::EXPORT_DECLARATION, l, c, start, end) {}
    
    void addExportedStyle(const std::string& style);
    void addExportedElement(const std::string& element);
    void addExportedVar(const std::string& var);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

} // namespace chtl