package com.chtl.context;

import com.chtl.ast.*;
import com.chtl.ast.node.*;
import java.util.*;

/**
 * 约束管理器
 * 管理CHTL的except约束
 * 严格按照CHTL语法文档实现
 */
public class ConstraintManager {
    // 全局约束
    private final List<ConstraintNode> globalConstraints;
    
    // 元素级约束（元素路径 -> 约束列表）
    private final Map<String, List<ConstraintNode>> elementConstraints;
    
    // 当前活动的约束栈
    private final Stack<List<ConstraintNode>> constraintStack;
    
    public ConstraintManager() {
        this.globalConstraints = new ArrayList<>();
        this.elementConstraints = new HashMap<>();
        this.constraintStack = new Stack<>();
    }
    
    /**
     * 添加全局约束
     */
    public void addGlobalConstraint(ConstraintNode constraint) {
        constraint.setGlobal(true);
        globalConstraints.add(constraint);
    }
    
    /**
     * 添加元素级约束
     */
    public void addElementConstraint(String elementPath, ConstraintNode constraint) {
        elementConstraints.computeIfAbsent(elementPath, k -> new ArrayList<>()).add(constraint);
    }
    
    /**
     * 进入新的约束作用域
     */
    public void pushConstraints(List<ConstraintNode> constraints) {
        constraintStack.push(new ArrayList<>(constraints));
    }
    
    /**
     * 离开约束作用域
     */
    public void popConstraints() {
        if (!constraintStack.isEmpty()) {
            constraintStack.pop();
        }
    }
    
    /**
     * 检查节点是否被允许
     */
    public boolean isAllowed(CHTLASTNode node, CompilationContext context) {
        // 获取所有适用的约束
        List<ConstraintNode> applicableConstraints = getApplicableConstraints(context);
        
        // 检查每个约束
        for (ConstraintNode constraint : applicableConstraints) {
            if (violatesConstraint(node, constraint)) {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * 获取当前适用的所有约束
     */
    private List<ConstraintNode> getApplicableConstraints(CompilationContext context) {
        List<ConstraintNode> constraints = new ArrayList<>();
        
        // 添加全局约束
        constraints.addAll(globalConstraints);
        
        // 添加当前路径的元素约束
        String currentPath = buildElementPath(context);
        if (elementConstraints.containsKey(currentPath)) {
            constraints.addAll(elementConstraints.get(currentPath));
        }
        
        // 添加栈中的活动约束
        for (List<ConstraintNode> stackConstraints : constraintStack) {
            constraints.addAll(stackConstraints);
        }
        
        return constraints;
    }
    
    /**
     * 构建当前元素路径
     */
    private String buildElementPath(CompilationContext context) {
        StringBuilder path = new StringBuilder();
        Stack<CHTLASTNode> nodeStack = new Stack<>();
        
        // 构建节点路径
        CHTLASTNode current = context.getCurrentNode();
        while (current != null && current.getParent() != null) {
            nodeStack.push(current);
            current = current.getParent();
        }
        
        // 构建路径字符串
        while (!nodeStack.isEmpty()) {
            CHTLASTNode node = nodeStack.pop();
            if (node instanceof ElementNode) {
                if (path.length() > 0) {
                    path.append("/");
                }
                path.append(((ElementNode) node).getTagName());
            }
        }
        
        return path.toString();
    }
    
    /**
     * 检查节点是否违反约束
     */
    private boolean violatesConstraint(CHTLASTNode node, ConstraintNode constraint) {
        ConstraintNode.ConstraintTarget target = constraint.getTarget();
        
        switch (target) {
            case HTML:
                return node instanceof ElementNode && isHtmlElement((ElementNode) node);
                
            case STYLE:
                return node instanceof StyleBlockNode || 
                       node instanceof TemplateDefinitionNode && 
                       ((TemplateDefinitionNode) node).getTemplateType() == TemplateDefinitionNode.TemplateType.STYLE;
                
            case ELEMENT:
                return node instanceof TemplateDefinitionNode && 
                       ((TemplateDefinitionNode) node).getTemplateType() == TemplateDefinitionNode.TemplateType.ELEMENT;
                
            case VAR:
                return node instanceof VarGroupDefinitionNode || 
                       node instanceof TemplateDefinitionNode && 
                       ((TemplateDefinitionNode) node).getTemplateType() == TemplateDefinitionNode.TemplateType.VAR;
                
            case TEMPLATE:
                return node instanceof TemplateDefinitionNode || node instanceof TemplateUsageNode;
                
            case CUSTOM:
                return node instanceof CustomDefinitionNode || node instanceof CustomUsageNode;
                
            case TEMPLATE_STYLE:
                return node instanceof TemplateDefinitionNode && 
                       ((TemplateDefinitionNode) node).getTemplateType() == TemplateDefinitionNode.TemplateType.STYLE ||
                       node instanceof TemplateUsageNode && 
                       ((TemplateUsageNode) node).getTemplateType() == TemplateDefinitionNode.TemplateType.STYLE;
                
            case TEMPLATE_ELEMENT:
                return node instanceof TemplateDefinitionNode && 
                       ((TemplateDefinitionNode) node).getTemplateType() == TemplateDefinitionNode.TemplateType.ELEMENT ||
                       node instanceof TemplateUsageNode && 
                       ((TemplateUsageNode) node).getTemplateType() == TemplateDefinitionNode.TemplateType.ELEMENT;
                
            case TEMPLATE_VAR:
                return node instanceof TemplateDefinitionNode && 
                       ((TemplateDefinitionNode) node).getTemplateType() == TemplateDefinitionNode.TemplateType.VAR ||
                       node instanceof TemplateUsageNode && 
                       ((TemplateUsageNode) node).getTemplateType() == TemplateDefinitionNode.TemplateType.VAR;
                
            case CUSTOM_STYLE:
                return node instanceof CustomDefinitionNode && 
                       ((CustomDefinitionNode) node).getCustomType() == TemplateDefinitionNode.TemplateType.STYLE ||
                       node instanceof CustomUsageNode && 
                       ((CustomUsageNode) node).getCustomType() == TemplateDefinitionNode.TemplateType.STYLE;
                
            case CUSTOM_ELEMENT:
                return node instanceof CustomDefinitionNode && 
                       ((CustomDefinitionNode) node).getCustomType() == TemplateDefinitionNode.TemplateType.ELEMENT ||
                       node instanceof CustomUsageNode && 
                       ((CustomUsageNode) node).getCustomType() == TemplateDefinitionNode.TemplateType.ELEMENT;
                
            case CUSTOM_VAR:
                return node instanceof CustomDefinitionNode && 
                       ((CustomDefinitionNode) node).getCustomType() == TemplateDefinitionNode.TemplateType.VAR ||
                       node instanceof CustomUsageNode && 
                       ((CustomUsageNode) node).getCustomType() == TemplateDefinitionNode.TemplateType.VAR;
                
            case SPECIFIC_NAME:
                String specificName = constraint.getSpecificName();
                return matchesSpecificName(node, specificName);
                
            default:
                return false;
        }
    }
    
    /**
     * 检查是否是HTML元素
     */
    private boolean isHtmlElement(ElementNode node) {
        // 在CHTL中，所有元素都被认为是HTML元素
        // 除非它们是特殊的CHTL元素
        String tagName = node.getTagName();
        return !isSpecialCHTLElement(tagName);
    }
    
    /**
     * 检查是否是特殊的CHTL元素
     */
    private boolean isSpecialCHTLElement(String tagName) {
        // text, style, script是CHTL特殊元素
        return "text".equals(tagName) || "style".equals(tagName) || "script".equals(tagName);
    }
    
    /**
     * 检查节点是否匹配特定名称
     */
    private boolean matchesSpecificName(CHTLASTNode node, String name) {
        if (node instanceof ElementNode) {
            return ((ElementNode) node).getTagName().equals(name);
        } else if (node instanceof TemplateDefinitionNode) {
            return ((TemplateDefinitionNode) node).getName().equals(name);
        } else if (node instanceof TemplateUsageNode) {
            return ((TemplateUsageNode) node).getName().equals(name);
        } else if (node instanceof CustomDefinitionNode) {
            return ((CustomDefinitionNode) node).getName().equals(name);
        } else if (node instanceof CustomUsageNode) {
            return ((CustomUsageNode) node).getName().equals(name);
        } else if (node instanceof VarGroupDefinitionNode) {
            return ((VarGroupDefinitionNode) node).getName().equals(name);
        }
        return false;
    }
    
    /**
     * RAII约束作用域管理器
     */
    public class ConstraintScope implements AutoCloseable {
        private final List<ConstraintNode> constraints;
        
        public ConstraintScope(List<ConstraintNode> constraints) {
            this.constraints = constraints;
            pushConstraints(constraints);
        }
        
        @Override
        public void close() {
            popConstraints();
        }
    }
    
    /**
     * 创建约束作用域
     */
    public ConstraintScope createScope(List<ConstraintNode> constraints) {
        return new ConstraintScope(constraints);
    }
    
    /**
     * 清理所有约束
     */
    public void clear() {
        globalConstraints.clear();
        elementConstraints.clear();
        constraintStack.clear();
    }
}