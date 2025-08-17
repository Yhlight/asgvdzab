#pragma once

#include "ASTNodes.hpp"

namespace CHTL {

/**
 * AST访问者接口
 */
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    // 基础节点访问
    virtual void visit(Program& node) = 0;
    virtual void visit(Identifier& node) = 0;
    virtual void visit(Literal& node) = 0;
    
    // HTML结构访问
    virtual void visit(Element& node) = 0;
    virtual void visit(Attribute& node) = 0;
    virtual void visit(TextNode& node) = 0;
    
    // 样式系统访问
    virtual void visit(StyleBlock& node) = 0;
    virtual void visit(StyleProperty& node) = 0;
    virtual void visit(Selector& node) = 0;
    virtual void visit(StyleRule& node) = 0;
    
    // 模板系统访问
    virtual void visit(TemplateStyle& node) = 0;
    virtual void visit(TemplateElement& node) = 0;
    virtual void visit(TemplateVariable& node) = 0;
    virtual void visit(TemplateUsage& node) = 0;
    
    // 命名空间和导入访问
    virtual void visit(NamespaceDefinition& node) = 0;
    virtual void visit(ImportStatement& node) = 0;
    
    // 注释访问
    virtual void visit(Comment& node) = 0;
};

/**
 * 基础AST访问者
 * 提供默认的遍历行为
 */
class BaseASTVisitor : public ASTVisitor {
public:
    // 基础节点访问的默认实现
    void visit(Program& node) override;
    void visit(Identifier& node) override;
    void visit(Literal& node) override;
    
    // HTML结构访问的默认实现
    void visit(Element& node) override;
    void visit(Attribute& node) override;
    void visit(TextNode& node) override;
    
    // 样式系统访问的默认实现
    void visit(StyleBlock& node) override;
    void visit(StyleProperty& node) override;
    void visit(Selector& node) override;
    void visit(StyleRule& node) override;
    
    // 模板系统访问的默认实现
    void visit(TemplateStyle& node) override;
    void visit(TemplateElement& node) override;
    void visit(TemplateVariable& node) override;
    void visit(TemplateUsage& node) override;
    
    // 命名空间和导入访问的默认实现
    void visit(NamespaceDefinition& node) override;
    void visit(ImportStatement& node) override;
    
    // 注释访问的默认实现
    void visit(Comment& node) override;

protected:
    /**
     * 访问子节点集合
     */
    void visitChildren(const std::vector<ASTNodePtr>& children);
    
    /**
     * 访问单个子节点
     */
    void visitChild(ASTNodePtr child);
};

} // namespace CHTL