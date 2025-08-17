#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace CHTL {

/**
 * CHTL AST节点基类
 */
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual std::string toString() const = 0;
    virtual std::string toHTML() const = 0;
};

using ASTNodePtr = std::shared_ptr<ASTNode>;
using ASTNodeList = std::vector<ASTNodePtr>;

/**
 * 文本节点
 */
class TextNode : public ASTNode {
public:
    std::string content;
    
    explicit TextNode(const std::string& text) : content(text) {}
    
    std::string toString() const override {
        return "TextNode(\"" + content + "\")";
    }
    
    std::string toHTML() const override {
        return content;
    }
};

/**
 * 属性节点
 */
class AttributeNode : public ASTNode {
public:
    std::string name;
    std::string value;
    
    AttributeNode(const std::string& n, const std::string& v) : name(n), value(v) {}
    
    std::string toString() const override {
        return "Attribute(" + name + "=\"" + value + "\")";
    }
    
    std::string toHTML() const override {
        return name + "=\"" + value + "\"";
    }
};

/**
 * 样式属性节点
 */
class StylePropertyNode : public ASTNode {
public:
    std::string property;
    std::string value;
    
    StylePropertyNode(const std::string& prop, const std::string& val) 
        : property(prop), value(val) {}
    
    std::string toString() const override {
        return "StyleProperty(" + property + ": " + value + ")";
    }
    
    std::string toHTML() const override {
        return property + ": " + value + ";";
    }
};

/**
 * 样式选择器节点
 */
class StyleSelectorNode : public ASTNode {
public:
    std::string selector;
    std::vector<std::shared_ptr<StylePropertyNode>> properties;
    
    explicit StyleSelectorNode(const std::string& sel) : selector(sel) {}
    
    void addProperty(std::shared_ptr<StylePropertyNode> prop) {
        properties.push_back(prop);
    }
    
    std::string toString() const override {
        return "StyleSelector(" + selector + ")";
    }
    
    std::string toHTML() const override {
        std::string result = selector + " {\n";
        for (const auto& prop : properties) {
            result += "  " + prop->toHTML() + "\n";
        }
        result += "}";
        return result;
    }
};

/**
 * 样式块节点
 */
class StyleBlockNode : public ASTNode {
public:
    std::vector<std::shared_ptr<StylePropertyNode>> inlineProperties;  // 内联样式
    std::vector<std::shared_ptr<StyleSelectorNode>> selectors;         // 选择器样式
    
    void addInlineProperty(std::shared_ptr<StylePropertyNode> prop) {
        inlineProperties.push_back(prop);
    }
    
    void addSelector(std::shared_ptr<StyleSelectorNode> sel) {
        selectors.push_back(sel);
    }
    
    std::string toString() const override {
        return "StyleBlock";
    }
    
    std::string toHTML() const override {
        std::string result;
        
        // 内联样式
        if (!inlineProperties.empty()) {
            for (const auto& prop : inlineProperties) {
                if (!result.empty()) result += " ";
                result += prop->toHTML();
            }
        }
        
        return result;
    }
    
    // 获取全局样式（选择器样式）
    std::string getGlobalCSS() const {
        std::string result;
        for (const auto& selector : selectors) {
            if (!result.empty()) result += "\n";
            result += selector->toHTML();
        }
        return result;
    }
};

/**
 * HTML元素节点
 */
class ElementNode : public ASTNode {
public:
    std::string tagName;
    std::vector<std::shared_ptr<AttributeNode>> attributes;
    std::shared_ptr<StyleBlockNode> styleBlock;
    ASTNodeList children;
    bool isSelfClosing;
    
    explicit ElementNode(const std::string& tag) 
        : tagName(tag), isSelfClosing(false) {}
    
    void addAttribute(std::shared_ptr<AttributeNode> attr) {
        attributes.push_back(attr);
    }
    
    void setStyleBlock(std::shared_ptr<StyleBlockNode> style) {
        styleBlock = style;
    }
    
    void addChild(ASTNodePtr child) {
        children.push_back(child);
    }
    
    std::string toString() const override {
        return "Element(" + tagName + ")";
    }
    
    std::string toHTML() const override {
        std::string result = "<" + tagName;
        
        // 添加属性
        for (const auto& attr : attributes) {
            result += " " + attr->toHTML();
        }
        
        // 添加内联样式
        if (styleBlock && !styleBlock->inlineProperties.empty()) {
            result += " style=\"" + styleBlock->toHTML() + "\"";
        }
        
        if (isSelfClosing) {
            result += " />";
            return result;
        }
        
        result += ">";
        
        // 添加子元素
        for (const auto& child : children) {
            result += child->toHTML();
        }
        
        result += "</" + tagName + ">";
        return result;
    }
};

/**
 * 模板节点
 */
class TemplateNode : public ASTNode {
public:
    enum class Type { STYLE, ELEMENT, VAR };
    
    Type type;
    std::string name;
    ASTNodeList content;
    
    TemplateNode(Type t, const std::string& n) : type(t), name(n) {}
    
    void addContent(ASTNodePtr node) {
        content.push_back(node);
    }
    
    std::string toString() const override {
        std::string typeStr = (type == Type::STYLE) ? "Style" : 
                             (type == Type::ELEMENT) ? "Element" : "Var";
        return "Template(" + typeStr + ", " + name + ")";
    }
    
    std::string toHTML() const override {
        // 模板在编译时展开，不直接输出HTML
        return "";
    }
};

/**
 * 自定义节点
 */
class CustomNode : public ASTNode {
public:
    enum class Type { STYLE, ELEMENT, VAR };
    
    Type type;
    std::string name;
    ASTNodeList content;
    std::unordered_map<std::string, std::string> specializations;
    
    CustomNode(Type t, const std::string& n) : type(t), name(n) {}
    
    void addContent(ASTNodePtr node) {
        content.push_back(node);
    }
    
    void addSpecialization(const std::string& key, const std::string& value) {
        specializations[key] = value;
    }
    
    std::string toString() const override {
        std::string typeStr = (type == Type::STYLE) ? "Style" : 
                             (type == Type::ELEMENT) ? "Element" : "Var";
        return "Custom(" + typeStr + ", " + name + ")";
    }
    
    std::string toHTML() const override {
        // 自定义节点在编译时处理，不直接输出HTML
        return "";
    }
};

/**
 * 原始嵌入节点
 */
class OriginNode : public ASTNode {
public:
    std::string type;  // @Html, @Style, @JavaScript等
    std::string name;  // 可选的名称
    std::string content;
    
    OriginNode(const std::string& t, const std::string& c) : type(t), content(c) {}
    OriginNode(const std::string& t, const std::string& n, const std::string& c) 
        : type(t), name(n), content(c) {}
    
    std::string toString() const override {
        return "Origin(" + type + (name.empty() ? "" : ", " + name) + ")";
    }
    
    std::string toHTML() const override {
        return content;  // 原始内容直接输出
    }
};

/**
 * 根节点
 */
class DocumentNode : public ASTNode {
public:
    ASTNodeList children;
    std::vector<std::shared_ptr<TemplateNode>> templates;
    std::vector<std::shared_ptr<CustomNode>> customs;
    std::string globalCSS;  // 收集的全局CSS
    
    void addChild(ASTNodePtr child) {
        children.push_back(child);
    }
    
    void addTemplate(std::shared_ptr<TemplateNode> tmpl) {
        templates.push_back(tmpl);
    }
    
    void addCustom(std::shared_ptr<CustomNode> custom) {
        customs.push_back(custom);
    }
    
    void addGlobalCSS(const std::string& css) {
        if (!globalCSS.empty()) globalCSS += "\n";
        globalCSS += css;
    }
    
    std::string toString() const override {
        return "Document";
    }
    
    std::string toHTML() const override {
        std::string result;
        for (const auto& child : children) {
            result += child->toHTML();
        }
        return result;
    }
};

} // namespace CHTL