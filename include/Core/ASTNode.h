#pragma once

#include "Types.h"
#include "Token.h"
#include <vector>
#include <memory>

namespace CHTL {
namespace Core {

class ASTNode : public std::enable_shared_from_this<ASTNode> {
public:
    ASTNode(ASTNodeType type) : type_(type) {}
    virtual ~ASTNode() = default;

    // 获取节点类型
    ASTNodeType getType() const { return type_; }
    
    // 获取/设置位置信息
    const SourceRange& getSourceRange() const { return sourceRange_; }
    void setSourceRange(const SourceRange& range) { sourceRange_ = range; }
    
    // 子节点管理
    void addChild(ASTNodePtr child);
    void insertChild(size_t index, ASTNodePtr child);
    void removeChild(size_t index);
    void removeChild(ASTNodePtr child);
    void clearChildren() { children_.clear(); }
    
    // 获取子节点
    const std::vector<ASTNodePtr>& getChildren() const { return children_; }
    ASTNodePtr getChild(size_t index) const;
    size_t getChildCount() const { return children_.size(); }
    
    // 查找子节点
    ASTNodePtr findChild(ASTNodeType type) const;
    std::vector<ASTNodePtr> findChildren(ASTNodeType type) const;
    
    // 父节点管理
    ASTNodePtr getParent() const { return parent_.lock(); }
    void setParent(ASTNodePtr parent) { parent_ = parent; }
    
    // 属性管理
    void setAttribute(const String& key, const String& value);
    String getAttribute(const String& key) const;
    bool hasAttribute(const String& key) const;
    void removeAttribute(const String& key);
    const StringMap& getAttributes() const { return attributes_; }
    
    // 访问者模式支持
    virtual void accept(class ASTVisitor& visitor) = 0;
    
    // 克隆节点
    virtual ASTNodePtr clone() const = 0;
    
    // 转换为字符串表示
    virtual String toString() const;
    
    // 类型检查辅助函数
    bool isElement() const { return type_ == ASTNodeType::ELEMENT; }
    bool isTextNode() const { return type_ == ASTNodeType::TEXT_NODE; }
    bool isAttribute() const { return type_ == ASTNodeType::ATTRIBUTE; }
    bool isStyleBlock() const { return type_ == ASTNodeType::STYLE_BLOCK; }
    bool isScriptBlock() const { return type_ == ASTNodeType::SCRIPT_BLOCK; }
    bool isTemplate() const;
    bool isCustom() const;
    bool isOrigin() const;
    
protected:
    ASTNodeType type_;
    SourceRange sourceRange_;
    std::vector<ASTNodePtr> children_;
    std::weak_ptr<ASTNode> parent_;
    StringMap attributes_;
};

// 具体的AST节点类型

// 文档根节点
class DocumentNode : public ASTNode {
public:
    DocumentNode() : ASTNode(ASTNodeType::DOCUMENT) {}
    void accept(ASTVisitor& visitor) override;
    ASTNodePtr clone() const override;
};

// HTML元素节点
class ElementNode : public ASTNode {
public:
    ElementNode(const String& tagName) 
        : ASTNode(ASTNodeType::ELEMENT), tagName_(tagName) {}
    
    const String& getTagName() const { return tagName_; }
    void setTagName(const String& tagName) { tagName_ = tagName; }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodePtr clone() const override;
    String toString() const override;

private:
    String tagName_;
};

// 文本节点
class TextNode : public ASTNode {
public:
    TextNode(const String& content = "") 
        : ASTNode(ASTNodeType::TEXT_NODE), content_(content) {}
    
    const String& getContent() const { return content_; }
    void setContent(const String& content) { content_ = content; }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodePtr clone() const override;
    String toString() const override;

private:
    String content_;
};

// 属性节点
class AttributeNode : public ASTNode {
public:
    AttributeNode(const String& name, const String& value = "")
        : ASTNode(ASTNodeType::ATTRIBUTE), name_(name), value_(value) {}
    
    const String& getName() const { return name_; }
    void setName(const String& name) { name_ = name; }
    
    const String& getValue() const { return value_; }
    void setValue(const String& value) { value_ = value; }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodePtr clone() const override;
    String toString() const override;

private:
    String name_;
    String value_;
};

// 样式块节点
class StyleBlockNode : public ASTNode {
public:
    StyleBlockNode() : ASTNode(ASTNodeType::STYLE_BLOCK) {}
    
    void accept(ASTVisitor& visitor) override;
    ASTNodePtr clone() const override;
};

// 脚本块节点
class ScriptBlockNode : public ASTNode {
public:
    ScriptBlockNode() : ASTNode(ASTNodeType::SCRIPT_BLOCK) {}
    
    void accept(ASTVisitor& visitor) override;
    ASTNodePtr clone() const override;
};

// 模板样式节点
class TemplateStyleNode : public ASTNode {
public:
    TemplateStyleNode(const String& name)
        : ASTNode(ASTNodeType::TEMPLATE_STYLE), name_(name) {}
    
    const String& getName() const { return name_; }
    void setName(const String& name) { name_ = name; }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodePtr clone() const override;

private:
    String name_;
};

// 模板元素节点
class TemplateElementNode : public ASTNode {
public:
    TemplateElementNode(const String& name)
        : ASTNode(ASTNodeType::TEMPLATE_ELEMENT), name_(name) {}
    
    const String& getName() const { return name_; }
    void setName(const String& name) { name_ = name; }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodePtr clone() const override;

private:
    String name_;
};

// 模板变量节点
class TemplateVarNode : public ASTNode {
public:
    TemplateVarNode(const String& name)
        : ASTNode(ASTNodeType::TEMPLATE_VAR), name_(name) {}
    
    const String& getName() const { return name_; }
    void setName(const String& name) { name_ = name; }
    
    void accept(ASTVisitor& visitor) override;
    ASTNodePtr clone() const override;

private:
    String name_;
};

// 创建AST节点的便利函数
ASTNodePtr makeDocumentNode();
ASTNodePtr makeElementNode(const String& tagName);
ASTNodePtr makeTextNode(const String& content = "");
ASTNodePtr makeAttributeNode(const String& name, const String& value = "");
ASTNodePtr makeStyleBlockNode();
ASTNodePtr makeScriptBlockNode();
ASTNodePtr makeTemplateStyleNode(const String& name);
ASTNodePtr makeTemplateElementNode(const String& name);
ASTNodePtr makeTemplateVarNode(const String& name);

} // namespace Core
} // namespace CHTL