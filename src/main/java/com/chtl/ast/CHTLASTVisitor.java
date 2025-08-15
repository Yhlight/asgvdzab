package com.chtl.ast;

import com.chtl.ast.node.CommentNode;
import com.chtl.ast.node.ConfigurationNode;
import com.chtl.ast.node.ConstraintNode;
import com.chtl.ast.node.CustomDefinitionNode;
import com.chtl.ast.node.CustomOperationNode;
import com.chtl.ast.node.CustomUsageNode;
import com.chtl.ast.node.ImportStatementNode;
import com.chtl.ast.node.InheritStatementNode;
import com.chtl.ast.node.NamespaceNode;
import com.chtl.ast.node.NumberLiteralNode;
import com.chtl.ast.node.OriginBlockNode;
import com.chtl.ast.node.RootNode;
import com.chtl.ast.node.ScriptBlockNode;
import com.chtl.ast.node.SpecializationNode;
import com.chtl.ast.node.StringLiteralNode;
import com.chtl.ast.node.StyleBlockNode;
import com.chtl.ast.node.StylePropertyNode;
import com.chtl.ast.node.StyleRuleNode;
import com.chtl.ast.node.StyleSelectorNode;
import com.chtl.ast.node.TemplateDefinitionNode;
import com.chtl.ast.node.TemplateUsageNode;
import com.chtl.ast.node.UnquotedLiteralNode;
import com.chtl.ast.node.VarGroupDefinitionNode;
import com.chtl.ast.node.VarGroupUsageNode;
import com.chtl.model.AttributeNode;
import com.chtl.model.ElementNode;
import com.chtl.model.TextNode;


import com.chtl.ast.node.*;

/**
 * CHTL AST访问者接口
 * 用于遍历AST树
 */
public interface CHTLASTVisitor {
    // 基础节点
    void visitRoot(RootNode node);
    void visitElement(ElementNode node);
    void visitText(TextNode node);
    void visitAttribute(AttributeNode node);
    void visitComment(CommentNode node);
    
    // 样式相关
    void visitStyleBlock(StyleBlockNode node);
    void visitStyleRule(StyleRuleNode node);
    void visitStyleProperty(StylePropertyNode node);
    void visitStyleSelector(StyleSelectorNode node);
    
    // 脚本相关
    void visitScriptBlock(ScriptBlockNode node);
    
    // 模板相关
    void visitTemplateDefinition(TemplateDefinitionNode node);
    void visitTemplateUsage(TemplateUsageNode node);
    
    // 自定义相关
    void visitCustomDefinition(CustomDefinitionNode node);
    void visitCustomUsage(CustomUsageNode node);
    void visitCustomOperation(CustomOperationNode node);
    void visitSpecialization(SpecializationNode node);
    
    // 原始嵌入
    void visitOriginBlock(OriginBlockNode node);
    
    // 导入
    void visitImportStatement(ImportStatementNode node);
    
    // 命名空间
    void visitNamespace(NamespaceNode node);
    
    // 配置
    void visitConfiguration(ConfigurationNode node);
    
    // 约束
    void visitConstraint(ConstraintNode node);
    
    // 变量组
    void visitVarGroupDefinition(VarGroupDefinitionNode node);
    void visitVarGroupUsage(VarGroupUsageNode node);
    
    // 继承
    void visitInheritStatement(InheritStatementNode node);
    
    // 字面量
    void visitStringLiteral(StringLiteralNode node);
    void visitUnquotedLiteral(UnquotedLiteralNode node);
    void visitNumberLiteral(NumberLiteralNode node);
}