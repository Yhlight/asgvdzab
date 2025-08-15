package com.chtl.ast;

import com.chtl.ast.node.*;

public abstract class AbstractCHTLASTVisitor implements CHTLASTVisitor {
	@Override public void visitRoot(RootNode node) { visitChildren(node); }
	@Override public void visitElement(com.chtl.model.ElementNode node) {}
	@Override public void visitText(com.chtl.model.TextNode node) {}
	@Override public void visitAttribute(com.chtl.model.AttributeNode node) {}
	@Override public void visitComment(CommentNode node) { visitChildren(node); }
	@Override public void visitStyleBlock(StyleBlockNode node) { visitChildren(node); }
	@Override public void visitStyleRule(StyleRuleNode node) { visitChildren(node); }
	@Override public void visitStyleProperty(StylePropertyNode node) { }
	@Override public void visitStyleSelector(StyleSelectorNode node) { }
	@Override public void visitScriptBlock(ScriptBlockNode node) { visitChildren(node); }
	@Override public void visitTemplateDefinition(TemplateDefinitionNode node) { visitChildren(node); }
	@Override public void visitTemplateUsage(TemplateUsageNode node) { }
	@Override public void visitCustomDefinition(CustomDefinitionNode node) { visitChildren(node); }
	@Override public void visitCustomUsage(CustomUsageNode node) { }
	@Override public void visitCustomOperation(CustomOperationNode node) { visitChildren(node); }
	@Override public void visitSpecialization(SpecializationNode node) { visitChildren(node); }
	@Override public void visitOriginBlock(OriginBlockNode node) { }
	@Override public void visitImportStatement(ImportStatementNode node) { }
	@Override public void visitNamespace(NamespaceNode node) { visitChildren(node); }
	@Override public void visitConfiguration(ConfigurationNode node) { visitChildren(node); }
	@Override public void visitConstraint(ConstraintNode node) { }
	@Override public void visitVarGroupDefinition(VarGroupDefinitionNode node) { visitChildren(node); }
	@Override public void visitVarGroupUsage(VarGroupUsageNode node) { }
	@Override public void visitInheritStatement(InheritStatementNode node) { }
	@Override public void visitStringLiteral(StringLiteralNode node) { }
	@Override public void visitUnquotedLiteral(UnquotedLiteralNode node) { }
	@Override public void visitNumberLiteral(NumberLiteralNode node) { }

	protected void visitChildren(CHTLASTNode node) {
		if (node instanceof com.chtl.ast.AbstractCHTLASTNode) {
			for (CHTLASTNode child : ((com.chtl.ast.AbstractCHTLASTNode) node).getChildren()) {
				child.accept(this);
			}
		}
	}
}