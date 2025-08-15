package com.chtl.compiler.constraint;

import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.context.CompilationContext;

/**
 * 约束验证器
 * 在AST遍历过程中验证约束规则
 * 严格遵循CHTL语法文档
 */
public class ConstraintValidator extends AbstractCHTLASTVisitor {
    private final CompilationContext context;
    private final ConstraintManager constraintManager;
    
    // 当前作用域节点
    private CHTLASTNode currentScope;
    
    public ConstraintValidator(CompilationContext context, ConstraintManager constraintManager) {
        this.context = context;
        this.constraintManager = constraintManager;
    }
    
    /**
     * 验证AST
     */
    public void validate(CHTLASTNode root) {
        root.accept(this);
    }
    
    @Override
    public void visitElement(ElementNode node) {
        // 进入元素作用域
        CHTLASTNode previousScope = currentScope;
        currentScope = node;
        constraintManager.enterScope(node);
        
        // 验证元素本身是否违反父作用域的约束
        if (previousScope != null) {
            constraintManager.validateNode(node, previousScope);
        }
        
        // 访问子节点
        super.visitElement(node);
        
        // 退出作用域
        constraintManager.exitScope();
        currentScope = previousScope;
    }
    
    @Override
    public void visitNamespace(NamespaceNode node) {
        // 进入命名空间作用域
        CHTLASTNode previousScope = currentScope;
        currentScope = node;
        constraintManager.enterScope(node);
        
        // 访问子节点
        super.visitNamespace(node);
        
        // 退出作用域
        constraintManager.exitScope();
        currentScope = previousScope;
    }
    
    @Override
    public void visitConstraint(ConstraintNode node) {
        // 处理约束定义
        constraintManager.processConstraint(node);
        
        // 不需要访问子节点
    }
    
    @Override
    public void visitTemplateDefinition(TemplateDefinitionNode node) {
        // 验证模板定义是否违反约束
        if (currentScope != null) {
            constraintManager.validateNode(node, currentScope);
        }
        
        // 进入模板定义作用域
        CHTLASTNode previousScope = currentScope;
        currentScope = node;
        constraintManager.enterScope(node);
        
        // 访问子节点
        super.visitTemplateDefinition(node);
        
        // 退出作用域
        constraintManager.exitScope();
        currentScope = previousScope;
    }
    
    @Override
    public void visitTemplateUsage(TemplateUsageNode node) {
        // 验证模板使用是否违反约束
        if (currentScope != null) {
            constraintManager.validateNode(node, currentScope);
        }
        
        // 访问子节点
        super.visitTemplateUsage(node);
    }
    
    @Override
    public void visitCustomDefinition(CustomDefinitionNode node) {
        // 验证自定义定义是否违反约束
        if (currentScope != null) {
            constraintManager.validateNode(node, currentScope);
        }
        
        // 进入自定义定义作用域
        CHTLASTNode previousScope = currentScope;
        currentScope = node;
        constraintManager.enterScope(node);
        
        // 访问子节点
        super.visitCustomDefinition(node);
        
        // 退出作用域
        constraintManager.exitScope();
        currentScope = previousScope;
    }
    
    @Override
    public void visitCustomUsage(CustomUsageNode node) {
        // 验证自定义使用是否违反约束
        if (currentScope != null) {
            constraintManager.validateNode(node, currentScope);
        }
        
        // 访问子节点
        super.visitCustomUsage(node);
    }
    
    @Override
    public void visitVarGroupDefinition(VarGroupDefinitionNode node) {
        // 验证变量组定义是否违反约束
        if (currentScope != null) {
            constraintManager.validateNode(node, currentScope);
        }
        
        // 进入变量组作用域
        CHTLASTNode previousScope = currentScope;
        currentScope = node;
        constraintManager.enterScope(node);
        
        // 访问子节点
        super.visitVarGroupDefinition(node);
        
        // 退出作用域
        constraintManager.exitScope();
        currentScope = previousScope;
    }
}