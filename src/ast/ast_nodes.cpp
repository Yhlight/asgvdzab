#include "ast/ast_nodes.hpp"
#include "ast/ast_visitor.hpp"
#include <sstream>
#include <algorithm>

namespace chtl {
namespace ast {

// ========== Program 实现 ==========

void Program::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> Program::clone() const {
    auto clone = std::make_unique<Program>();
    clone->setLocation(getLocation());
    
    for (const auto& child : children_) {
        clone->addChild(std::shared_ptr<ASTNode>(child->clone().release()));
    }
    
    return std::move(clone);
}

std::string Program::toString() const {
    std::stringstream ss;
    ss << "Program(children=" << children_.size() << ")";
    return ss.str();
}

// ========== Identifier 实现 ==========

void Identifier::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> Identifier::clone() const {
    auto clone = std::make_unique<Identifier>(name_, getLocation());
    return std::move(clone);
}

std::string Identifier::toString() const {
    return "Identifier(\"" + name_ + "\")";
}

// ========== Literal 实现 ==========

void Literal::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> Literal::clone() const {
    auto clone = std::make_unique<Literal>(value_, literalType_, getLocation());
    return std::move(clone);
}

std::string Literal::toString() const {
    std::string typeStr;
    switch (literalType_) {
        case LiteralType::STRING: typeStr = "string"; break;
        case LiteralType::NUMBER: typeStr = "number"; break;
        case LiteralType::BOOLEAN: typeStr = "boolean"; break;
        case LiteralType::UNQUOTED: typeStr = "unquoted"; break;
    }
    return "Literal(" + typeStr + ", \"" + value_ + "\")";
}

// ========== Element 实现 ==========

void Element::addAttribute(std::shared_ptr<Attribute> attr) {
    if (attr) {
        attributes_.push_back(attr);
    }
}

void Element::setStyleBlock(std::shared_ptr<StyleBlock> style) {
    styleBlock_ = style;
}

void Element::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> Element::clone() const {
    auto clone = std::make_unique<Element>(tagName_, getLocation());
    
    // 克隆属性
    for (const auto& attr : attributes_) {
        auto attrClone = std::dynamic_pointer_cast<Attribute>(
            std::shared_ptr<ASTNode>(attr->clone().release())
        );
        clone->addAttribute(attrClone);
    }
    
    // 克隆子节点
    for (const auto& child : children_) {
        clone->addChild(std::shared_ptr<ASTNode>(child->clone().release()));
    }
    
    // 克隆样式块
    if (styleBlock_) {
        auto styleClone = std::dynamic_pointer_cast<StyleBlock>(
            std::shared_ptr<ASTNode>(styleBlock_->clone().release())
        );
        clone->setStyleBlock(styleClone);
    }
    
    return std::move(clone);
}

std::string Element::toString() const {
    std::stringstream ss;
    ss << "Element(\"" << tagName_ << "\", attrs=" << attributes_.size()
       << ", children=" << children_.size();
    if (styleBlock_) {
        ss << ", style=true";
    }
    ss << ")";
    return ss.str();
}

// ========== Attribute 实现 ==========

void Attribute::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> Attribute::clone() const {
    auto clone = std::make_unique<Attribute>(name_, value_, getLocation());
    return std::move(clone);
}

std::string Attribute::toString() const {
    return "Attribute(\"" + name_ + "\", \"" + value_ + "\")";
}

// ========== TextNode 实现 ==========

void TextNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> TextNode::clone() const {
    auto clone = std::make_unique<TextNode>(content_, getLocation());
    return std::move(clone);
}

std::string TextNode::toString() const {
    std::string truncated = content_;
    if (truncated.length() > 50) {
        truncated = truncated.substr(0, 47) + "...";
    }
    return "TextNode(\"" + truncated + "\")";
}

// ========== StyleBlock 实现 ==========

void StyleBlock::addProperty(std::shared_ptr<StyleProperty> prop) {
    if (prop) {
        properties_.push_back(prop);
    }
}

void StyleBlock::addSelector(std::shared_ptr<Selector> selector) {
    if (selector) {
        selectors_.push_back(selector);
    }
}

void StyleBlock::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> StyleBlock::clone() const {
    auto clone = std::make_unique<StyleBlock>(getLocation());
    
    // 克隆属性
    for (const auto& prop : properties_) {
        auto propClone = std::dynamic_pointer_cast<StyleProperty>(
            std::shared_ptr<ASTNode>(prop->clone().release())
        );
        clone->addProperty(propClone);
    }
    
    // 克隆选择器
    for (const auto& selector : selectors_) {
        auto selectorClone = std::dynamic_pointer_cast<Selector>(
            std::shared_ptr<ASTNode>(selector->clone().release())
        );
        clone->addSelector(selectorClone);
    }
    
    return std::move(clone);
}

std::string StyleBlock::toString() const {
    std::stringstream ss;
    ss << "StyleBlock(properties=" << properties_.size()
       << ", selectors=" << selectors_.size() << ")";
    return ss.str();
}

// ========== StyleProperty 实现 ==========

void StyleProperty::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> StyleProperty::clone() const {
    auto clone = std::make_unique<StyleProperty>(property_, value_, getLocation());
    return std::move(clone);
}

std::string StyleProperty::toString() const {
    return "StyleProperty(\"" + property_ + "\", \"" + value_ + "\")";
}

// ========== Selector 实现 ==========

void Selector::addProperty(std::shared_ptr<StyleProperty> prop) {
    if (prop) {
        properties_.push_back(prop);
    }
}

void Selector::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> Selector::clone() const {
    auto clone = std::make_unique<Selector>(selector_, getLocation());
    
    for (const auto& prop : properties_) {
        auto propClone = std::dynamic_pointer_cast<StyleProperty>(
            std::shared_ptr<ASTNode>(prop->clone().release())
        );
        clone->addProperty(propClone);
    }
    
    return std::move(clone);
}

std::string Selector::toString() const {
    std::stringstream ss;
    ss << "Selector(\"" << selector_ << "\", properties=" << properties_.size() << ")";
    return ss.str();
}

// ========== TemplateStyle 实现 ==========

void TemplateStyle::addProperty(std::shared_ptr<StyleProperty> prop) {
    if (prop) {
        properties_.push_back(prop);
    }
}

void TemplateStyle::addInheritance(std::shared_ptr<TemplateUsage> inheritance) {
    if (inheritance) {
        inheritances_.push_back(inheritance);
    }
}

void TemplateStyle::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> TemplateStyle::clone() const {
    auto clone = std::make_unique<TemplateStyle>(name_, getLocation());
    
    // 克隆属性
    for (const auto& prop : properties_) {
        auto propClone = std::dynamic_pointer_cast<StyleProperty>(
            std::shared_ptr<ASTNode>(prop->clone().release())
        );
        clone->addProperty(propClone);
    }
    
    // 克隆继承
    for (const auto& inheritance : inheritances_) {
        auto inheritClone = std::dynamic_pointer_cast<TemplateUsage>(
            std::shared_ptr<ASTNode>(inheritance->clone().release())
        );
        clone->addInheritance(inheritClone);
    }
    
    return std::move(clone);
}

std::string TemplateStyle::toString() const {
    std::stringstream ss;
    ss << "TemplateStyle(\"" << name_ << "\", properties=" << properties_.size()
       << ", inheritances=" << inheritances_.size() << ")";
    return ss.str();
}

// ========== TemplateElement 实现 ==========

void TemplateElement::addInheritance(std::shared_ptr<TemplateUsage> inheritance) {
    if (inheritance) {
        inheritances_.push_back(inheritance);
    }
}

void TemplateElement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> TemplateElement::clone() const {
    auto clone = std::make_unique<TemplateElement>(name_, getLocation());
    
    // 克隆子节点
    for (const auto& child : children_) {
        clone->addChild(std::shared_ptr<ASTNode>(child->clone().release()));
    }
    
    // 克隆继承
    for (const auto& inheritance : inheritances_) {
        auto inheritClone = std::dynamic_pointer_cast<TemplateUsage>(
            std::shared_ptr<ASTNode>(inheritance->clone().release())
        );
        clone->addInheritance(inheritClone);
    }
    
    return std::move(clone);
}

std::string TemplateElement::toString() const {
    std::stringstream ss;
    ss << "TemplateElement(\"" << name_ << "\", children=" << children_.size()
       << ", inheritances=" << inheritances_.size() << ")";
    return ss.str();
}

// ========== TemplateVar 实现 ==========

void TemplateVar::addVariable(const std::string& name, const std::string& value) {
    variables_[name] = value;
}

void TemplateVar::addInheritance(std::shared_ptr<TemplateUsage> inheritance) {
    if (inheritance) {
        inheritances_.push_back(inheritance);
    }
}

void TemplateVar::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> TemplateVar::clone() const {
    auto clone = std::make_unique<TemplateVar>(name_, getLocation());
    
    // 克隆变量
    clone->variables_ = variables_;
    
    // 克隆继承
    for (const auto& inheritance : inheritances_) {
        auto inheritClone = std::dynamic_pointer_cast<TemplateUsage>(
            std::shared_ptr<ASTNode>(inheritance->clone().release())
        );
        clone->addInheritance(inheritClone);
    }
    
    return std::move(clone);
}

std::string TemplateVar::toString() const {
    std::stringstream ss;
    ss << "TemplateVar(\"" << name_ << "\", variables=" << variables_.size()
       << ", inheritances=" << inheritances_.size() << ")";
    return ss.str();
}

// ========== CustomStyle 实现 ==========

void CustomStyle::addProperty(std::shared_ptr<StyleProperty> prop) {
    if (prop) {
        properties_.push_back(prop);
    }
}

void CustomStyle::addParameter(const std::string& name, const std::string& defaultValue) {
    parameters_[name] = defaultValue;
}

void CustomStyle::addSpecialization(std::shared_ptr<Specialization> spec) {
    if (spec) {
        specializations_.push_back(spec);
    }
}

void CustomStyle::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> CustomStyle::clone() const {
    auto clone = std::make_unique<CustomStyle>(name_, getLocation());
    
    // 克隆属性
    for (const auto& prop : properties_) {
        auto propClone = std::dynamic_pointer_cast<StyleProperty>(
            std::shared_ptr<ASTNode>(prop->clone().release())
        );
        clone->addProperty(propClone);
    }
    
    // 克隆参数
    clone->parameters_ = parameters_;
    
    // 克隆特例化
    for (const auto& spec : specializations_) {
        auto specClone = std::dynamic_pointer_cast<Specialization>(
            std::shared_ptr<ASTNode>(spec->clone().release())
        );
        clone->addSpecialization(specClone);
    }
    
    return std::move(clone);
}

std::string CustomStyle::toString() const {
    std::stringstream ss;
    ss << "CustomStyle(\"" << name_ << "\", properties=" << properties_.size()
       << ", parameters=" << parameters_.size()
       << ", specializations=" << specializations_.size() << ")";
    return ss.str();
}

// ========== Namespace 实现 ==========

void Namespace::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> Namespace::clone() const {
    auto clone = std::make_unique<Namespace>(name_, getLocation());
    
    // 克隆子节点
    for (const auto& child : children_) {
        clone->addChild(std::shared_ptr<ASTNode>(child->clone().release()));
    }
    
    // 注意：不克隆parent_以避免循环引用
    
    return std::move(clone);
}

std::string Namespace::toString() const {
    std::stringstream ss;
    ss << "Namespace(\"" << name_ << "\", children=" << children_.size() << ")";
    return ss.str();
}

// ========== Import 实现 ==========

void Import::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> Import::clone() const {
    auto clone = std::make_unique<Import>(importType_, path_, getLocation());
    clone->setAlias(alias_);
    clone->setFrom(from_);
    return std::move(clone);
}

std::string Import::toString() const {
    std::stringstream ss;
    ss << "Import(type=" << static_cast<int>(importType_) << ", path=\"" << path_ << "\"";
    if (!alias_.empty()) {
        ss << ", alias=\"" << alias_ << "\"";
    }
    if (!from_.empty()) {
        ss << ", from=\"" << from_ << "\"";
    }
    ss << ")";
    return ss.str();
}

// ========== TemplateUsage 实现 ==========

void TemplateUsage::addArgument(const std::string& name, const std::string& value) {
    arguments_[name] = value;
}

void TemplateUsage::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> TemplateUsage::clone() const {
    auto clone = std::make_unique<TemplateUsage>(usageType_, name_, getLocation());
    clone->setFrom(from_);
    clone->arguments_ = arguments_;
    return std::move(clone);
}

std::string TemplateUsage::toString() const {
    std::stringstream ss;
    ss << "TemplateUsage(type=" << static_cast<int>(usageType_) << ", name=\"" << name_ << "\"";
    if (!from_.empty()) {
        ss << ", from=\"" << from_ << "\"";
    }
    if (!arguments_.empty()) {
        ss << ", args=" << arguments_.size();
    }
    ss << ")";
    return ss.str();
}

// ========== Specialization 实现 ==========

void Specialization::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> Specialization::clone() const {
    auto clone = std::make_unique<Specialization>(specType_, target_, getLocation());
    clone->setValue(value_);
    clone->setIndex(index_);
    return std::move(clone);
}

std::string Specialization::toString() const {
    std::stringstream ss;
    ss << "Specialization(type=" << static_cast<int>(specType_) << ", target=\"" << target_ << "\"";
    if (!value_.empty()) {
        ss << ", value=\"" << value_ << "\"";
    }
    if (index_ != -1) {
        ss << ", index=" << index_;
    }
    ss << ")";
    return ss.str();
}

// ========== Comment 实现 ==========

void Comment::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> Comment::clone() const {
    auto clone = std::make_unique<Comment>(commentType_, content_, getLocation());
    return std::move(clone);
}

std::string Comment::toString() const {
    std::string typeStr;
    switch (commentType_) {
        case CommentType::SINGLE_LINE: typeStr = "single"; break;
        case CommentType::MULTI_LINE: typeStr = "multi"; break;
        case CommentType::GENERATOR: typeStr = "generator"; break;
    }
    
    std::string truncated = content_;
    if (truncated.length() > 30) {
        truncated = truncated.substr(0, 27) + "...";
    }
    
    return "Comment(" + typeStr + ", \"" + truncated + "\")";
}

// ========== Configuration 实现 ==========

void Configuration::addSetting(const std::string& key, const std::string& value) {
    settings_[key] = value;
}

void Configuration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> Configuration::clone() const {
    auto clone = std::make_unique<Configuration>(getLocation());
    clone->settings_ = settings_;
    return std::move(clone);
}

std::string Configuration::toString() const {
    std::stringstream ss;
    ss << "Configuration(settings=" << settings_.size() << ")";
    return ss.str();
}

// ========== Constraint 实现 ==========

void Constraint::addException(const std::string& exception) {
    exceptions_.push_back(exception);
}

void Constraint::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> Constraint::clone() const {
    auto clone = std::make_unique<Constraint>(constraintType_, target_, getLocation());
    clone->exceptions_ = exceptions_;
    return std::move(clone);
}

std::string Constraint::toString() const {
    std::stringstream ss;
    ss << "Constraint(type=" << static_cast<int>(constraintType_) 
       << ", target=\"" << target_ << "\", exceptions=" << exceptions_.size() << ")";
    return ss.str();
}

// ========== VariableAccess 实现 ==========

void VariableAccess::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::unique_ptr<ASTNode> VariableAccess::clone() const {
    auto clone = std::make_unique<VariableAccess>(variableName_, getLocation());
    clone->setProperty(property_);
    return std::move(clone);
}

std::string VariableAccess::toString() const {
    std::string result = "VariableAccess(\"" + variableName_ + "\"";
    if (!property_.empty()) {
        result += ", property=\"" + property_ + "\"";
    }
    result += ")";
    return result;
}

} // namespace ast
} // namespace chtl