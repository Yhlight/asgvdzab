#ifndef CHTL_AST_ASTVISITOR_H
#define CHTL_AST_ASTVISITOR_H

namespace Chtl {

// 前向声明所有AST节点类
class ASTNode;
class ProgramNode;
class ElementNode;
class TextNode;
class AttributeNode;
class StyleBlockNode;
class StyleRuleNode;
class StyleSelectorNode;
class StylePropertyNode;
class ScriptBlockNode;
class CommentNode;
class StringLiteralNode;
class NumberLiteralNode;
class IdentifierNode;

// 模板和自定义节点
class TemplateDefinitionNode;
class StyleTemplateNode;
class ElementTemplateNode;
class VarTemplateNode;
class CustomDefinitionNode;
class CustomStyleNode;
class CustomElementNode;
class CustomVarNode;
class TemplateUseNode;
class VarUseNode;
class InheritStatementNode;
class DeleteStatementNode;
class InsertStatementNode;

// 配置和导入节点
class OriginBlockNode;
class ConfigurationBlockNode;
class NameBlockNode;
class OriginTypeBlockNode;
class ImportStatementNode;
class ExportStatementNode;
class NamespaceBlockNode;
class InfoBlockNode;

// CHTL JS节点
class SelectorExpressionNode;
class ArrowAccessNode;
class ListenCallNode;
class DelegateCallNode;
class AnimateCallNode;
class ChtlJsExpressionNode;
class VirtualObjectNode;
class VirtualCallNode;

// AST访问者接口
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    // 基本节点访问方法
    virtual void visitProgram(ProgramNode* node) = 0;
    virtual void visitElement(ElementNode* node) = 0;
    virtual void visitText(TextNode* node) = 0;
    virtual void visitAttribute(AttributeNode* node) = 0;
    virtual void visitStyleBlock(StyleBlockNode* node) = 0;
    virtual void visitStyleRule(StyleRuleNode* node) = 0;
    virtual void visitStyleSelector(StyleSelectorNode* node) = 0;
    virtual void visitStyleProperty(StylePropertyNode* node) = 0;
    virtual void visitScriptBlock(ScriptBlockNode* node) = 0;
    virtual void visitComment(CommentNode* node) = 0;
    virtual void visitStringLiteral(StringLiteralNode* node) = 0;
    virtual void visitNumberLiteral(NumberLiteralNode* node) = 0;
    virtual void visitIdentifier(IdentifierNode* node) = 0;
    
    // 模板和自定义节点访问方法
    virtual void visitTemplateDefinition(TemplateDefinitionNode* node) = 0;
    virtual void visitStyleTemplate(StyleTemplateNode* node) = 0;
    virtual void visitElementTemplate(ElementTemplateNode* node) = 0;
    virtual void visitVarTemplate(VarTemplateNode* node) = 0;
    virtual void visitCustomDefinition(CustomDefinitionNode* node) = 0;
    virtual void visitCustomStyle(CustomStyleNode* node) = 0;
    virtual void visitCustomElement(CustomElementNode* node) = 0;
    virtual void visitCustomVar(CustomVarNode* node) = 0;
    virtual void visitTemplateUse(TemplateUseNode* node) = 0;
    virtual void visitVarUse(VarUseNode* node) = 0;
    virtual void visitInheritStatement(InheritStatementNode* node) = 0;
    virtual void visitDeleteStatement(DeleteStatementNode* node) = 0;
    virtual void visitInsertStatement(InsertStatementNode* node) = 0;
    
    // 配置和导入节点访问方法
    virtual void visitOriginBlock(OriginBlockNode* node) = 0;
    virtual void visitConfigurationBlock(ConfigurationBlockNode* node) = 0;
    virtual void visitNameBlock(NameBlockNode* node) = 0;
    virtual void visitOriginTypeBlock(OriginTypeBlockNode* node) = 0;
    virtual void visitImportStatement(ImportStatementNode* node) = 0;
    virtual void visitExportStatement(ExportStatementNode* node) = 0;
    virtual void visitNamespaceBlock(NamespaceBlockNode* node) = 0;
    virtual void visitInfoBlock(InfoBlockNode* node) = 0;
    
    // CHTL JS节点访问方法
    virtual void visitSelectorExpression(SelectorExpressionNode* node) = 0;
    virtual void visitArrowAccess(ArrowAccessNode* node) = 0;
    virtual void visitListenCall(ListenCallNode* node) = 0;
    virtual void visitDelegateCall(DelegateCallNode* node) = 0;
    virtual void visitAnimateCall(AnimateCallNode* node) = 0;
    virtual void visitChtlJsExpression(ChtlJsExpressionNode* node) = 0;
    virtual void visitVirtualObject(VirtualObjectNode* node) = 0;
    virtual void visitVirtualCall(VirtualCallNode* node) = 0;
};

// 默认访问者实现（可选的基类）
class DefaultASTVisitor : public ASTVisitor {
public:
    // 提供默认的空实现，子类可以只覆盖需要的方法
    void visitProgram(ProgramNode* node) override {}
    void visitElement(ElementNode* node) override {}
    void visitText(TextNode* node) override {}
    void visitAttribute(AttributeNode* node) override {}
    void visitStyleBlock(StyleBlockNode* node) override {}
    void visitStyleRule(StyleRuleNode* node) override {}
    void visitStyleSelector(StyleSelectorNode* node) override {}
    void visitStyleProperty(StylePropertyNode* node) override {}
    void visitScriptBlock(ScriptBlockNode* node) override {}
    void visitComment(CommentNode* node) override {}
    void visitStringLiteral(StringLiteralNode* node) override {}
    void visitNumberLiteral(NumberLiteralNode* node) override {}
    void visitIdentifier(IdentifierNode* node) override {}
    
    void visitTemplateDefinition(TemplateDefinitionNode* node) override {}
    void visitStyleTemplate(StyleTemplateNode* node) override {}
    void visitElementTemplate(ElementTemplateNode* node) override {}
    void visitVarTemplate(VarTemplateNode* node) override {}
    void visitCustomDefinition(CustomDefinitionNode* node) override {}
    void visitCustomStyle(CustomStyleNode* node) override {}
    void visitCustomElement(CustomElementNode* node) override {}
    void visitCustomVar(CustomVarNode* node) override {}
    void visitTemplateUse(TemplateUseNode* node) override {}
    void visitVarUse(VarUseNode* node) override {}
    void visitInheritStatement(InheritStatementNode* node) override {}
    void visitDeleteStatement(DeleteStatementNode* node) override {}
    void visitInsertStatement(InsertStatementNode* node) override {}
    
    void visitOriginBlock(OriginBlockNode* node) override {}
    void visitConfigurationBlock(ConfigurationBlockNode* node) override {}
    void visitNameBlock(NameBlockNode* node) override {}
    void visitOriginTypeBlock(OriginTypeBlockNode* node) override {}
    void visitImportStatement(ImportStatementNode* node) override {}
    void visitExportStatement(ExportStatementNode* node) override {}
    void visitNamespaceBlock(NamespaceBlockNode* node) override {}
    void visitInfoBlock(InfoBlockNode* node) override {}
    
    void visitSelectorExpression(SelectorExpressionNode* node) override {}
    void visitArrowAccess(ArrowAccessNode* node) override {}
    void visitListenCall(ListenCallNode* node) override {}
    void visitDelegateCall(DelegateCallNode* node) override {}
    void visitAnimateCall(AnimateCallNode* node) override {}
    void visitChtlJsExpression(ChtlJsExpressionNode* node) override {}
};

} // namespace Chtl

#endif // CHTL_AST_ASTVISITOR_H