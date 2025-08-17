#include "../../include/Core/ASTNode.h"
#include <algorithm>
#include <sstream>

namespace CHTL {
namespace Core {

// 前向声明访问者
class ASTVisitor;

// ASTNode基类实现

void ASTNode::addChild(ASTNodePtr child) {
    if (child) {
        child->setParent(shared_from_this());
        children_.push_back(child);
    }
}

void ASTNode::insertChild(size_t index, ASTNodePtr child) {
    if (child && index <= children_.size()) {
        child->setParent(shared_from_this());
        children_.insert(children_.begin() + index, child);
    }
}

void ASTNode::removeChild(size_t index) {
    if (index < children_.size()) {
        children_[index]->setParent(nullptr);
        children_.erase(children_.begin() + index);
    }
}

void ASTNode::removeChild(ASTNodePtr child) {
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end()) {
        (*it)->setParent(nullptr);
        children_.erase(it);
    }
}

ASTNodePtr ASTNode::getChild(size_t index) const {
    return index < children_.size() ? children_[index] : nullptr;
}

ASTNodePtr ASTNode::findChild(ASTNodeType type) const {
    for (const auto& child : children_) {
        if (child->getType() == type) {
            return child;
        }
    }
    return nullptr;
}

std::vector<ASTNodePtr> ASTNode::findChildren(ASTNodeType type) const {
    std::vector<ASTNodePtr> result;
    for (const auto& child : children_) {
        if (child->getType() == type) {
            result.push_back(child);
        }
    }
    return result;
}

void ASTNode::setAttribute(const String& key, const String& value) {
    attributes_[key] = value;
}

String ASTNode::getAttribute(const String& key) const {
    auto it = attributes_.find(key);
    return it != attributes_.end() ? it->second : "";
}

bool ASTNode::hasAttribute(const String& key) const {
    return attributes_.find(key) != attributes_.end();
}

void ASTNode::removeAttribute(const String& key) {
    attributes_.erase(key);
}

String ASTNode::toString() const {
    std::stringstream ss;
    ss << "ASTNode{type=" << static_cast<int>(type_) 
       << ", children=" << children_.size() << "}";
    return ss.str();
}

bool ASTNode::isTemplate() const {
    return type_ == ASTNodeType::TEMPLATE_STYLE ||
           type_ == ASTNodeType::TEMPLATE_ELEMENT ||
           type_ == ASTNodeType::TEMPLATE_VAR;
}

bool ASTNode::isCustom() const {
    return type_ == ASTNodeType::CUSTOM_STYLE ||
           type_ == ASTNodeType::CUSTOM_ELEMENT ||
           type_ == ASTNodeType::CUSTOM_VAR;
}

bool ASTNode::isOrigin() const {
    return type_ == ASTNodeType::ORIGIN_HTML ||
           type_ == ASTNodeType::ORIGIN_CSS ||
           type_ == ASTNodeType::ORIGIN_JS;
}

// 具体节点类型实现

// DocumentNode
void DocumentNode::accept(ASTVisitor& visitor) {
    // 访问者模式实现将在后续添加
}

ASTNodePtr DocumentNode::clone() const {
    auto cloned = std::make_shared<DocumentNode>();
    cloned->sourceRange_ = sourceRange_;
    cloned->attributes_ = attributes_;
    
    for (const auto& child : children_) {
        cloned->addChild(child->clone());
    }
    
    return cloned;
}

// ElementNode
void ElementNode::accept(ASTVisitor& visitor) {
    // 访问者模式实现将在后续添加
}

ASTNodePtr ElementNode::clone() const {
    auto cloned = std::make_shared<ElementNode>(tagName_);
    cloned->sourceRange_ = sourceRange_;
    cloned->attributes_ = attributes_;
    
    for (const auto& child : children_) {
        cloned->addChild(child->clone());
    }
    
    return cloned;
}

String ElementNode::toString() const {
    std::stringstream ss;
    ss << "ElementNode{tag=" << tagName_ 
       << ", children=" << children_.size() << "}";
    return ss.str();
}

// TextNode
void TextNode::accept(ASTVisitor& visitor) {
    // 访问者模式实现将在后续添加
}

ASTNodePtr TextNode::clone() const {
    auto cloned = std::make_shared<TextNode>(content_);
    cloned->sourceRange_ = sourceRange_;
    cloned->attributes_ = attributes_;
    return cloned;
}

String TextNode::toString() const {
    std::stringstream ss;
    ss << "TextNode{content=\"" << content_ << "\"}";
    return ss.str();
}

// AttributeNode
void AttributeNode::accept(ASTVisitor& visitor) {
    // 访问者模式实现将在后续添加
}

ASTNodePtr AttributeNode::clone() const {
    auto cloned = std::make_shared<AttributeNode>(name_, value_);
    cloned->sourceRange_ = sourceRange_;
    cloned->attributes_ = attributes_;
    return cloned;
}

String AttributeNode::toString() const {
    std::stringstream ss;
    ss << "AttributeNode{name=\"" << name_ << "\", value=\"" << value_ << "\"}";
    return ss.str();
}

// StyleBlockNode
void StyleBlockNode::accept(ASTVisitor& visitor) {
    // 访问者模式实现将在后续添加
}

ASTNodePtr StyleBlockNode::clone() const {
    auto cloned = std::make_shared<StyleBlockNode>();
    cloned->sourceRange_ = sourceRange_;
    cloned->attributes_ = attributes_;
    
    for (const auto& child : children_) {
        cloned->addChild(child->clone());
    }
    
    return cloned;
}

// ScriptBlockNode
void ScriptBlockNode::accept(ASTVisitor& visitor) {
    // 访问者模式实现将在后续添加
}

ASTNodePtr ScriptBlockNode::clone() const {
    auto cloned = std::make_shared<ScriptBlockNode>();
    cloned->sourceRange_ = sourceRange_;
    cloned->attributes_ = attributes_;
    
    for (const auto& child : children_) {
        cloned->addChild(child->clone());
    }
    
    return cloned;
}

// TemplateStyleNode
void TemplateStyleNode::accept(ASTVisitor& visitor) {
    // 访问者模式实现将在后续添加
}

ASTNodePtr TemplateStyleNode::clone() const {
    auto cloned = std::make_shared<TemplateStyleNode>(name_);
    cloned->sourceRange_ = sourceRange_;
    cloned->attributes_ = attributes_;
    
    for (const auto& child : children_) {
        cloned->addChild(child->clone());
    }
    
    return cloned;
}

// TemplateElementNode
void TemplateElementNode::accept(ASTVisitor& visitor) {
    // 访问者模式实现将在后续添加
}

ASTNodePtr TemplateElementNode::clone() const {
    auto cloned = std::make_shared<TemplateElementNode>(name_);
    cloned->sourceRange_ = sourceRange_;
    cloned->attributes_ = attributes_;
    
    for (const auto& child : children_) {
        cloned->addChild(child->clone());
    }
    
    return cloned;
}

// TemplateVarNode
void TemplateVarNode::accept(ASTVisitor& visitor) {
    // 访问者模式实现将在后续添加
}

ASTNodePtr TemplateVarNode::clone() const {
    auto cloned = std::make_shared<TemplateVarNode>(name_);
    cloned->sourceRange_ = sourceRange_;
    cloned->attributes_ = attributes_;
    
    for (const auto& child : children_) {
        cloned->addChild(child->clone());
    }
    
    return cloned;
}

// 便利函数实现

ASTNodePtr makeDocumentNode() {
    return std::make_shared<DocumentNode>();
}

ASTNodePtr makeElementNode(const String& tagName) {
    return std::make_shared<ElementNode>(tagName);
}

ASTNodePtr makeTextNode(const String& content) {
    return std::make_shared<TextNode>(content);
}

ASTNodePtr makeAttributeNode(const String& name, const String& value) {
    return std::make_shared<AttributeNode>(name, value);
}

ASTNodePtr makeStyleBlockNode() {
    return std::make_shared<StyleBlockNode>();
}

ASTNodePtr makeScriptBlockNode() {
    return std::make_shared<ScriptBlockNode>();
}

ASTNodePtr makeTemplateStyleNode(const String& name) {
    return std::make_shared<TemplateStyleNode>(name);
}

ASTNodePtr makeTemplateElementNode(const String& name) {
    return std::make_shared<TemplateElementNode>(name);
}

ASTNodePtr makeTemplateVarNode(const String& name) {
    return std::make_shared<TemplateVarNode>(name);
}

} // namespace Core
} // namespace CHTL