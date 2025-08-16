package com.chtl.compiler.constraint;import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.ConstraintNode;
import com.chtl.ast.*;
import com.chtl.context.CompilationContext;
import com.chtl.scanner.ScannerStateMachine;

/**
 * 约束系统
 * 集成约束管理和验证功能
 * 严格遵循CHTL语法文档
 */
public class ConstraintSystem {
    private final CompilationContext context;
    private final ConstraintManager manager;
    private final ConstraintValidator validator;
    private final ScannerStateMachine stateMachine;
    
    public ConstraintSystem(CompilationContext context) {
        this.context = context;
        this.stateMachine = new ScannerStateMachine(context);
        this.manager = new ConstraintManager(context);
        this.validator = new ConstraintValidator(context, manager);
    }
    
    /**
     * 验证整个AST的约束
     */
    public void validateAST(CHTLASTNode root) {
        try (var stateGuard = context.enterState(CompilationContext.State.CONSTRAINT_CHECKING)) {
            // 使用验证器遍历AST
            validator.validate(root);
            
            // 报告验证结果
            reportValidationResults();
        }
    }
    
    /**
     * 处理单个约束节点
     */
    public void processConstraint(ConstraintNode constraint) {
        manager.processConstraint(constraint);
    }
    
    /**
     * 进入作用域
     */
    public void enterScope(CHTLASTNode scopeNode) {
        manager.enterScope(scopeNode);
    }
    
    /**
     * 退出作用域
     */
    public void exitScope() {
        manager.exitScope();
    }
    
    /**
     * 验证单个节点
     */
    public boolean validateNode(CHTLASTNode node, CHTLASTNode parentScope) {
        return manager.validateNode(node, parentScope);
    }
    
    /**
     * 报告验证结果
     */
    private void reportValidationResults() {
        int errorCount = context.getErrors().size();
        if (errorCount > 0) {
            System.out.println("约束验证发现 " + errorCount + " 个错误");
        } else {
            System.out.println("约束验证通过");
        }
    }
    
    /**
     * 获取约束管理器
     */
    public ConstraintManager getManager() {
        return manager;
    }
    
    /**
     * 获取约束验证器
     */
    public ConstraintValidator getValidator() {
        return validator;
    }
}
