package com.chtl.compiler.constraint;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.ConstraintNode;
import com.chtl.ast.node.CustomDefinitionNode;
import com.chtl.ast.node.CustomUsageNode;
import com.chtl.ast.node.NamespaceNode;
import com.chtl.ast.node.TemplateDefinitionNode;
import com.chtl.ast.node.TemplateUsageNode;
import com.chtl.ast.node.UnquotedLiteralNode;
import com.chtl.compiler.constraint.ConstraintManager.ConstraintRule;
import com.chtl.compiler.constraint.ConstraintManager.ConstraintScope;
import com.chtl.compiler.constraint.ConstraintManager.ConstraintTarget;
import com.chtl.model.ElementNode;


import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.context.CompilationContext;
import com.chtl.context.CompilationError;
import com.chtl.scanner.ScannerStateMachine;

import java.util.*;

/**
 * 约束管理器
 * 负责处理except关键字定义的约束规则
 * 严格遵循CHTL语法文档
 */
public class ConstraintManager {
    private final CompilationContext context;
    private final ScannerStateMachine stateMachine;
    
    // 约束规则存储
    // key: 作用域节点（元素、命名空间等）
    // value: 该作用域的约束规则
    private final Map<CHTLASTNode, ConstraintScope> constraintScopes;
    
    // 当前作用域栈
    private final Stack<ConstraintScope> scopeStack;
    
    /**
     * 约束作用域
     */
    private static class ConstraintScope {
        final CHTLASTNode scopeNode;
        final List<ConstraintRule> rules;
        final ConstraintScope parent;
        
        ConstraintScope(CHTLASTNode scopeNode, ConstraintScope parent) {
            this.scopeNode = scopeNode;
            this.rules = new ArrayList<>();
            this.parent = parent;
        }
        
        void addRule(ConstraintRule rule) {
            rules.add(rule);
        }
        
        boolean hasConstraints() {
            return !rules.isEmpty();
        }
    }
    
    /**
     * 约束规则
     */
    private static class ConstraintRule {
        final ConstraintType type;
        final List<ConstraintTarget> targets;
        
        ConstraintRule(ConstraintType type) {
            this.type = type;
            this.targets = new ArrayList<>();
        }
        
        void addTarget(ConstraintTarget target) {
            targets.add(target);
        }
    }
    
    /**
     * 约束目标
     */
    private static class ConstraintTarget {
        final TargetType type;
        final String name;
        final String templateType; // 用于@Var, @Element等
        
        ConstraintTarget(TargetType type, String name, String templateType) {
            this.type = type;
            this.name = name;
            this.templateType = templateType;
        }
    }
    
    /**
     * 约束类型
     */
    public enum ConstraintType {
        PRECISE,    // 精确约束
        TYPE,       // 类型约束
        GLOBAL      // 全局约束
    }
    
    /**
     * 目标类型
     */
    public enum TargetType {
        HTML_ELEMENT,       // HTML元素（如span）
        CUSTOM_OBJECT,      // 自定义对象（[Custom] @Element Box）
        TEMPLATE_OBJECT,    // 模板对象（[Template] @Style Name）
        HTML_TYPE,          // @Html类型
        CUSTOM_TYPE,        // [Custom]类型
        TEMPLATE_TYPE,      // [Template]类型
        TEMPLATE_VAR        // [Template] @Var类型
    }
    
    public ConstraintManager(CompilationContext context) {
        this.context = context;
        this.stateMachine = new ScannerStateMachine(context);
        this.constraintScopes = new HashMap<>();
        this.scopeStack = new Stack<>();
    }
    
    /**
     * 进入作用域
     */
    public void enterScope(CHTLASTNode scopeNode) {
        ConstraintScope parent = scopeStack.isEmpty() ? null : scopeStack.peek();
        ConstraintScope newScope = new ConstraintScope(scopeNode, parent);
        
        scopeStack.push(newScope);
        constraintScopes.put(scopeNode, newScope);
    }
    
    /**
     * 退出作用域
     */
    public void exitScope() {
        if (!scopeStack.isEmpty()) {
            scopeStack.pop();
        }
    }
    
    /**
     * 处理约束节点
     */
    public void processConstraint(ConstraintNode constraint) {
        if (scopeStack.isEmpty()) {
            context.addError(new CompilationError(
                "约束必须在作用域内定义",
                CompilationError.ErrorType.SYNTAX_ERROR
            ));
            return;
        }
        
        ConstraintScope currentScope = scopeStack.peek();
        ConstraintType constraintType = determineConstraintType(constraint, currentScope);
        ConstraintRule rule = new ConstraintRule(constraintType);
        
        // 解析约束目标
        // 现有的ConstraintNode API只支持单个目标
        ConstraintTarget constraintTarget = parseConstraintFromNode(constraint);
        if (constraintTarget != null) {
            rule.addTarget(constraintTarget);
        }
        
        currentScope.addRule(rule);
    }
    
    /**
     * 确定约束类型
     */
    private ConstraintType determineConstraintType(ConstraintNode constraint, ConstraintScope scope) {
        // 在命名空间中的约束是全局约束
        if (scope.scopeNode instanceof NamespaceNode) {
            return ConstraintType.GLOBAL;
        }
        
        // 检查约束目标来确定是精确约束还是类型约束
        if (isTypeConstraintNode(constraint)) {
            return ConstraintType.TYPE;
        }
        
        return ConstraintType.PRECISE;
    }
    
    /**
     * 检查是否是类型约束
     */
    private boolean isTypeConstraint(CHTLASTNode target) {
        if (target instanceof UnquotedLiteralNode) {
            String value = ((UnquotedLiteralNode) target).getValue();
            return value.startsWith("@") || value.equals("[Custom]") || value.equals("[Template]");
        }
        return false;
    }
    
    /**
     * 解析约束目标
     */
    private ConstraintTarget parseConstraintTarget(CHTLASTNode target) {
        if (target instanceof UnquotedLiteralNode) {
            String value = ((UnquotedLiteralNode) target).getValue();
            
            // @Html类型约束
            if (value.equals("@Html")) {
                return new ConstraintTarget(TargetType.HTML_TYPE, null, null);
            }
            
            // [Custom]类型约束
            if (value.equals("[Custom]")) {
                return new ConstraintTarget(TargetType.CUSTOM_TYPE, null, null);
            }
            
            // [Template]类型约束
            if (value.equals("[Template]")) {
                return new ConstraintTarget(TargetType.TEMPLATE_TYPE, null, null);
            }
            
            // HTML元素精确约束
            if (!value.startsWith("@") && !value.startsWith("[")) {
                return new ConstraintTarget(TargetType.HTML_ELEMENT, value, null);
            }
        }
        
        // [Template] @Var类型约束
        if (target instanceof TemplateUsageNode) {
            TemplateUsageNode usage = (TemplateUsageNode) target;
            if (usage.getTemplateType() == TemplateDefinitionNode.TemplateType.VAR) {
                return new ConstraintTarget(TargetType.TEMPLATE_VAR, null, "Var");
            }
        }
        
        // [Custom] @Element Box精确约束
        if (target instanceof CustomUsageNode) {
            CustomUsageNode usage = (CustomUsageNode) target;
            return new ConstraintTarget(TargetType.CUSTOM_OBJECT, usage.getName(), 
                usage.getCustomType() != null ? usage.getCustomType().toString() : null);
        }
        
        return null;
    }
    
    /**
     * 验证节点是否违反约束
     */
    public boolean validateNode(CHTLASTNode node, CHTLASTNode parentScope) {
        ConstraintScope scope = findScope(parentScope);
        if (scope == null || !scope.hasConstraints()) {
            return true; // 没有约束，验证通过
        }
        
        // 检查所有适用的约束规则
        while (scope != null) {
            for (ConstraintRule rule : scope.rules) {
                if (!validateAgainstRule(node, rule)) {
                    return false;
                }
            }
            
            // 检查父作用域的约束（约束继承）
            scope = scope.parent;
        }
        
        return true;
    }
    
    /**
     * 根据规则验证节点
     */
    private boolean validateAgainstRule(CHTLASTNode node, ConstraintRule rule) {
        for (ConstraintTarget target : rule.targets) {
            if (matchesTarget(node, target)) {
                // 节点匹配被禁止的目标
                reportConstraintViolation(node, target, rule);
                return false;
            }
        }
        return true;
    }
    
    /**
     * 检查节点是否匹配约束目标
     */
    private boolean matchesTarget(CHTLASTNode node, ConstraintTarget target) {
        switch (target.type) {
            case HTML_ELEMENT:
                if (node instanceof ElementNode) {
                    return ((ElementNode) node).getTagName().equals(target.name);
                }
                break;
                
            case CUSTOM_OBJECT:
                if (node instanceof CustomUsageNode) {
                    CustomUsageNode usage = (CustomUsageNode) node;
                    return usage.getName().equals(target.name);
                }
                break;
                
            case TEMPLATE_OBJECT:
                if (node instanceof TemplateUsageNode) {
                    TemplateUsageNode usage = (TemplateUsageNode) node;
                    return usage.getName().equals(target.name);
                }
                break;
                
            case HTML_TYPE:
                return node instanceof ElementNode;
                
            case CUSTOM_TYPE:
                return node instanceof CustomDefinitionNode || node instanceof CustomUsageNode;
                
            case TEMPLATE_TYPE:
                return node instanceof TemplateDefinitionNode || node instanceof TemplateUsageNode;
                
            case TEMPLATE_VAR:
                if (node instanceof TemplateUsageNode) {
                    TemplateUsageNode usage = (TemplateUsageNode) node;
                    return usage.getTemplateType() == TemplateDefinitionNode.TemplateType.VAR;
                }
                break;
        }
        
        return false;
    }
    
    /**
     * 报告约束违反
     */
    private void reportConstraintViolation(CHTLASTNode node, ConstraintTarget target, ConstraintRule rule) {
        StringBuilder message = new StringBuilder("违反约束规则: ");
        
        switch (target.type) {
            case HTML_ELEMENT:
                message.append("禁止使用HTML元素 '").append(target.name).append("'");
                break;
            case CUSTOM_OBJECT:
                message.append("禁止使用自定义对象 '").append(target.name).append("'");
                break;
            case TEMPLATE_OBJECT:
                message.append("禁止使用模板对象 '").append(target.name).append("'");
                break;
            case HTML_TYPE:
                message.append("禁止使用HTML元素");
                break;
            case CUSTOM_TYPE:
                message.append("禁止使用[Custom]类型");
                break;
            case TEMPLATE_TYPE:
                message.append("禁止使用[Template]类型");
                break;
            case TEMPLATE_VAR:
                message.append("禁止使用变量组模板");
                break;
        }
        
        context.addError(new CompilationError(
            message.toString(),
            CompilationError.ErrorType.CONSTRAINT_VIOLATION
        ));
    }
    
    /**
     * 从ConstraintNode解析约束目标
     */
    private ConstraintTarget parseConstraintFromNode(ConstraintNode constraint) {
        ConstraintNode.ConstraintTarget nodeTarget = constraint.getTarget();
        String specificName = constraint.getSpecificName();
        
        switch (nodeTarget) {
            case HTML:
                return new ConstraintTarget(TargetType.HTML_TYPE, null, null);
            case CUSTOM:
                return new ConstraintTarget(TargetType.CUSTOM_TYPE, null, null);
            case TEMPLATE:
                return new ConstraintTarget(TargetType.TEMPLATE_TYPE, null, null);
            case TEMPLATE_VAR:
                return new ConstraintTarget(TargetType.TEMPLATE_VAR, null, "Var");
            case SPECIFIC_NAME:
                // 需要判断是HTML元素还是自定义对象
                if (specificName != null && !specificName.startsWith("[") && !specificName.startsWith("@")) {
                    return new ConstraintTarget(TargetType.HTML_ELEMENT, specificName, null);
                }
                break;
        }
        
        return null;
    }
    
    /**
     * 检查ConstraintNode是否是类型约束
     */
    private boolean isTypeConstraintNode(ConstraintNode constraint) {
        ConstraintNode.ConstraintTarget target = constraint.getTarget();
        return target == ConstraintNode.ConstraintTarget.HTML ||
               target == ConstraintNode.ConstraintTarget.CUSTOM ||
               target == ConstraintNode.ConstraintTarget.TEMPLATE ||
               target == ConstraintNode.ConstraintTarget.TEMPLATE_VAR;
    }
    
    /**
     * 查找作用域
     */
    private ConstraintScope findScope(CHTLASTNode node) {
        // 首先检查节点本身是否是作用域
        ConstraintScope scope = constraintScopes.get(node);
        if (scope != null) {
            return scope;
        }
        
        // 向上查找父节点的作用域
        CHTLASTNode parent = node.getParent();
        while (parent != null) {
            scope = constraintScopes.get(parent);
            if (scope != null) {
                return scope;
            }
            parent = parent.getParent();
        }
        
        return null;
    }
    
    /**
     * 获取当前作用域的所有约束
     */
    public List<ConstraintRule> getCurrentConstraints() {
        if (scopeStack.isEmpty()) {
            return Collections.emptyList();
        }
        
        List<ConstraintRule> allRules = new ArrayList<>();
        ConstraintScope scope = scopeStack.peek();
        
        while (scope != null) {
            allRules.addAll(scope.rules);
            scope = scope.parent;
        }
        
        return allRules;
    }
}