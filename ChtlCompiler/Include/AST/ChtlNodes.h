#ifndef CHTL_AST_CHTLNODES_H
#define CHTL_AST_CHTLNODES_H

#include "AST/ASTNode.h"
#include <unordered_map>

namespace Chtl {

// 程序根节点
class ProgramNode : public ASTNode {
public:
    ProgramNode();
    void accept(class ASTVisitor* visitor) override;
};

// 元素节点 (div, span, html等)
class ElementNode : public ASTNode {
public:
    ElementNode(const Token& tagName);
    
    const std::string& getTagName() const { return tagName_; }
    
    // 获取特定类型的子节点
    std::vector<ASTNodePtr> getAttributes() const;
    std::vector<ASTNodePtr> getStyleBlocks() const;
    std::vector<ASTNodePtr> getScriptBlocks() const;
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::string tagName_;
};

// 文本节点 text { "内容" }
class TextNode : public ASTNode {
public:
    TextNode(const Token& token);
    
    void setContent(const std::string& content) { content_ = content; }
    const std::string& getContent() const { return content_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::string content_;
};

// 属性节点 id: value; 或 class = value;
class AttributeNode : public ASTNode {
public:
    AttributeNode(const Token& name);
    
    void setName(const std::string& name) { name_ = name; }
    void setValue(const std::string& value) { value_ = value; }
    
    const std::string& getName() const { return name_; }
    const std::string& getValue() const { return value_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::string name_;
    std::string value_;
};

// 局部样式块 style { }
class StyleBlockNode : public ASTNode {
public:
    StyleBlockNode(const Token& token);
    
    // 区分内联样式和CSS规则
    std::vector<ASTNodePtr> getInlineStyles() const;
    std::vector<ASTNodePtr> getCSSRules() const;
    
    void accept(class ASTVisitor* visitor) override;
};

// CSS规则节点 .selector { properties }
class StyleRuleNode : public ASTNode {
public:
    StyleRuleNode(const Token& token);
    
    void setSelector(ASTNodePtr selector) { selector_ = selector; }
    ASTNodePtr getSelector() const { return selector_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    ASTNodePtr selector_;  // 选择器节点
};

// CSS选择器节点
class StyleSelectorNode : public ASTNode {
public:
    enum SelectorType {
        CLASS,          // .className
        ID,             // #id
        TAG,            // tagName
        CONTEXT,        // & (上下文推导)
        PSEUDO_CLASS,   // &:hover
        PSEUDO_ELEMENT, // &::before
        COMPLEX         // 复合选择器
    };
    
    StyleSelectorNode(const Token& token, SelectorType type);
    
    SelectorType getSelectorType() const { return selectorType_; }
    const std::string& getSelector() const { return selector_; }
    void setSelector(const std::string& selector) { selector_ = selector; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    SelectorType selectorType_;
    std::string selector_;
};

// CSS属性节点 property: value;
class StylePropertyNode : public ASTNode {
public:
    StylePropertyNode(const Token& token);
    
    void setProperty(const std::string& property) { property_ = property; }
    void setValue(const std::string& value) { value_ = value; }
    
    const std::string& getProperty() const { return property_; }
    const std::string& getValue() const { return value_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::string property_;
    std::string value_;
};

// 局部脚本块 script { }
class ScriptBlockNode : public ASTNode {
public:
    ScriptBlockNode(const Token& token);
    
    void setContent(const std::string& content) { content_ = content; }
    const std::string& getContent() const { return content_; }
    
    // 是否包含CHTL JS语法
    bool hasChtlJsSyntax() const { return hasChtlJsSyntax_; }
    void setHasChtlJsSyntax(bool value) { hasChtlJsSyntax_ = value; }
    
    void accept(class ASTVisitor* visitor) override;

    // 转换后的内容（如果包含CHTL JS）
    const std::string& getTransformedContent() const { return transformedContent_; }
    void setTransformedContent(const std::string& content) { transformedContent_ = content; }

private:
    std::string content_;
    std::string transformedContent_;
    bool hasChtlJsSyntax_ = false;
};

// 注释节点
class CommentNode : public ASTNode {
public:
    enum CommentType {
        SINGLE,     // //
        MULTI,      // /* */
        GENERATOR   // --
    };
    
    CommentNode(const Token& token, CommentType type);
    
    CommentType getCommentType() const { return commentType_; }
    const std::string& getContent() const { return content_; }
    void setContent(const std::string& content) { content_ = content; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    CommentType commentType_;
    std::string content_;
};

// 字面量节点基类
class LiteralNode : public ASTNode {
public:
    LiteralNode(ASTNodeType type, const Token& token);
    
    const std::string& getValue() const { return value_; }
    void setValue(const std::string& value) { value_ = value; }

protected:
    std::string value_;
};

// 字符串字面量
class StringLiteralNode : public LiteralNode {
public:
    enum QuoteType {
        DOUBLE_QUOTE,   // "string"
        SINGLE_QUOTE,   // 'string'
        UNQUOTED        // 无引号字面量
    };
    
    StringLiteralNode(const Token& token, QuoteType quoteType);
    
    QuoteType getQuoteType() const { return quoteType_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    QuoteType quoteType_;
};

// 数字字面量
class NumberLiteralNode : public LiteralNode {
public:
    NumberLiteralNode(const Token& token);
    
    double getNumericValue() const;
    
    void accept(class ASTVisitor* visitor) override;
};

// 标识符节点
class IdentifierNode : public ASTNode {
public:
    IdentifierNode(const Token& token);
    
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::string name_;
};

} // namespace Chtl

#endif // CHTL_AST_CHTLNODES_H