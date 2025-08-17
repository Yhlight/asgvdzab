#ifndef CHTL_AST_TEMPLATENODES_H
#define CHTL_AST_TEMPLATENODES_H

#include "AST/ASTNode.h"

namespace Chtl {

// 模板定义基类
class TemplateDefinitionNode : public ASTNode {
public:
    enum TemplateType {
        STYLE,      // @Style
        ELEMENT,    // @Element
        VAR         // @Var
    };
    
    TemplateDefinitionNode(const Token& token, TemplateType type);
    
    TemplateType getTemplateType() const { return templateType_; }
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    void accept(class ASTVisitor* visitor) override;

protected:
    TemplateType templateType_;
    std::string name_;
};

// [Template] @Style 样式组模板
class StyleTemplateNode : public TemplateDefinitionNode {
public:
    StyleTemplateNode(const Token& token);
    
    // 获取所有样式属性
    std::vector<ASTNodePtr> getProperties() const;
    
    // 获取继承的样式组
    std::vector<ASTNodePtr> getInherits() const;
    
    void accept(class ASTVisitor* visitor) override;
};

// [Template] @Element 元素模板
class ElementTemplateNode : public TemplateDefinitionNode {
public:
    ElementTemplateNode(const Token& token);
    
    // 获取模板内容（元素列表）
    std::vector<ASTNodePtr> getElements() const;
    
    void accept(class ASTVisitor* visitor) override;
};

// [Template] @Var 变量组模板
class VarTemplateNode : public TemplateDefinitionNode {
public:
    VarTemplateNode(const Token& token);
    
    // 获取所有变量定义
    std::vector<ASTNodePtr> getVariables() const;
    
    void accept(class ASTVisitor* visitor) override;
};

// 自定义定义基类
class CustomDefinitionNode : public ASTNode {
public:
    enum CustomType {
        STYLE,      // @Style
        ELEMENT,    // @Element
        VAR         // @Var
    };
    
    CustomDefinitionNode(const Token& token, CustomType type);
    
    CustomType getCustomType() const { return customType_; }
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    void accept(class ASTVisitor* visitor) override;

protected:
    CustomType customType_;
    std::string name_;
};

// [Custom] @Style 自定义样式组
class CustomStyleNode : public CustomDefinitionNode {
public:
    CustomStyleNode(const Token& token);
    
    // 获取所有属性（包括无值属性）
    std::vector<ASTNodePtr> getProperties() const;
    
    // 获取继承的样式组
    std::vector<ASTNodePtr> getInherits() const;
    
    void accept(class ASTVisitor* visitor) override;
};

// [Custom] @Element 自定义元素
class CustomElementNode : public CustomDefinitionNode {
public:
    CustomElementNode(const Token& token);
    
    // 获取元素内容
    std::vector<ASTNodePtr> getElements() const;
    
    void accept(class ASTVisitor* visitor) override;
};

// [Custom] @Var 自定义变量组
class CustomVarNode : public CustomDefinitionNode {
public:
    CustomVarNode(const Token& token);
    
    // 获取变量定义
    std::vector<ASTNodePtr> getVariables() const;
    
    void accept(class ASTVisitor* visitor) override;
};

// 模板/自定义使用节点 @Style Name; 或 @Element Name;
class TemplateUseNode : public ASTNode {
public:
    enum UseType {
        STYLE,      // @Style
        ELEMENT,    // @Element
        VAR         // @Var (虽然变量组用法不同，但预留)
    };
    
    TemplateUseNode(const Token& token, UseType type);
    
    UseType getUseType() const { return useType_; }
    const std::string& getTemplateName() const { return templateName_; }
    void setTemplateName(const std::string& name) { templateName_ = name; }
    
    // 参数（用于自定义的特例化）
    void addParameter(const std::string& name, ASTNodePtr value);
    const std::unordered_map<std::string, ASTNodePtr>& getParameters() const { return parameters_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    UseType useType_;
    std::string templateName_;
    std::unordered_map<std::string, ASTNodePtr> parameters_;
};

// 变量使用节点 ThemeColor(tableColor)
class VarUseNode : public ASTNode {
public:
    VarUseNode(const Token& token);
    
    const std::string& getVarGroupName() const { return varGroupName_; }
    void setVarGroupName(const std::string& name) { varGroupName_ = name; }
    
    const std::string& getVariableName() const { return variableName_; }
    void setVariableName(const std::string& name) { variableName_ = name; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::string varGroupName_;
    std::string variableName_;
};

// 继承语句 inherit @Style Name;
class InheritStatementNode : public ASTNode {
public:
    enum InheritType {
        STYLE,      // @Style
        ELEMENT,    // @Element
        VAR         // @Var
    };
    
    InheritStatementNode(const Token& token);
    
    InheritType getInheritType() const { return inheritType_; }
    void setInheritType(InheritType type) { inheritType_ = type; }
    
    const std::string& getInheritName() const { return inheritName_; }
    void setInheritName(const std::string& name) { inheritName_ = name; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    InheritType inheritType_;
    std::string inheritName_;
};

// 删除语句 delete property1, property2;
class DeleteStatementNode : public ASTNode {
public:
    DeleteStatementNode(const Token& token);
    
    void addTarget(const std::string& target) { targets_.push_back(target); }
    const std::vector<std::string>& getTargets() const { return targets_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::vector<std::string> targets_;  // 要删除的属性或继承
};

// 插入语句 insert after/before element[index] { }
class InsertStatementNode : public ASTNode {
public:
    enum InsertPosition {
        AFTER,      // after
        BEFORE,     // before
        AT_TOP,     // at top
        AT_BOTTOM,  // at bottom
        REPLACE     // replace
    };
    
    InsertStatementNode(const Token& token);
    
    InsertPosition getPosition() const { return position_; }
    void setPosition(InsertPosition pos) { position_ = pos; }
    
    const std::string& getTargetElement() const { return targetElement_; }
    void setTargetElement(const std::string& element) { targetElement_ = element; }
    
    int getIndex() const { return index_; }
    void setIndex(int index) { index_ = index; }
    
    // 要插入的内容
    void setContent(ASTNodePtr content) { content_ = content; }
    ASTNodePtr getContent() const { return content_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    InsertPosition position_;
    std::string targetElement_;
    int index_ = -1;  // -1表示没有索引
    ASTNodePtr content_;
};

} // namespace Chtl

#endif // CHTL_AST_TEMPLATENODES_H