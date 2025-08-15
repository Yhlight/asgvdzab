package com.chtl.compiler.style;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;

import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.CustomDefinitionNode;
import com.chtl.ast.node.CustomOperationNode;
import com.chtl.ast.node.CustomUsageNode;
import com.chtl.ast.node.NumberLiteralNode;
import com.chtl.ast.node.StringLiteralNode;
import com.chtl.ast.node.StyleBlockNode;
import com.chtl.ast.node.StylePropertyNode;
import com.chtl.ast.node.StyleRuleNode;
import com.chtl.ast.node.StyleSelectorNode;
import com.chtl.ast.node.TemplateDefinitionNode;
import com.chtl.ast.node.TemplateUsageNode;
import com.chtl.ast.node.UnquotedLiteralNode;
import com.chtl.compiler.style.StyleSystem.ElementContext;
import com.chtl.context.CompilationError;
import com.chtl.model.ElementNode;


import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.context.CompilationContext;
import com.chtl.scanner.ScannerStateMachine;

import java.util.*;

/**
 * CHTL样式系统
 * 负责处理局部样式和全局样式
 * 严格遵循CHTL语法文档
 */
public class StyleSystem {
    private final CompilationContext context;
    private final ScannerStateMachine stateMachine;
    
    // 全局样式收集器
    private final StringBuilder globalStyles;
    
    // 局部样式处理器
    private final LocalStyleProcessor localStyleProcessor;
    
    // 样式组管理器
    private final StyleGroupManager styleGroupManager;
    
    // 样式继承处理器
    private final StyleInheritanceProcessor inheritanceProcessor;
    
    // 自动生成的类名/ID计数器
    private int autoClassCounter = 0;
    private int autoIdCounter = 0;
    
    // 当前处理的元素栈
    private final Stack<ElementContext> elementStack;
    
    /**
     * 元素上下文
     */
    private static class ElementContext {
        final ElementNode element;
        final Set<String> classNames = new HashSet<>();
        final String id;
        final Map<String, String> inlineStyles = new LinkedHashMap<>();
        
        ElementContext(ElementNode element, String id) {
            this.element = element;
            this.id = id;
        }
    }
    
    public StyleSystem(CompilationContext context) {
        this.context = context;
        this.stateMachine = new ScannerStateMachine(context);
        this.globalStyles = new StringBuilder();
        this.localStyleProcessor = new LocalStyleProcessor(this);
        this.styleGroupManager = new StyleGroupManager(context);
        this.inheritanceProcessor = new StyleInheritanceProcessor(context);
        this.elementStack = new Stack<>();
    }
    
    /**
     * 处理样式块节点
     */
    public void processStyleBlock(StyleBlockNode styleBlock, ElementNode parentElement) {
        try (var stateGuard = context.enterState(CompilationContext.State.ELEMENT)) {
            // 创建元素上下文
            ElementContext elementContext = new ElementContext(
                parentElement, 
                generateAutoId(parentElement)
            );
            elementStack.push(elementContext);
            
            // 处理样式块内容
            for (CHTLASTNode child : styleBlock.getChildren()) {
                processStyleChild(child, elementContext);
            }
            
            // 应用收集到的样式
            applyStylesToElement(elementContext);
            
            elementStack.pop();
        }
    }
    
    /**
     * 处理样式块中的子节点
     */
    private void processStyleChild(CHTLASTNode node, ElementContext elementContext) {
        if (node instanceof StylePropertyNode) {
            // 内联样式
            processInlineStyle((StylePropertyNode) node, elementContext);
        } else if (node instanceof StyleRuleNode) {
            // 类选择器、ID选择器、伪类等
            processStyleRule((StyleRuleNode) node, elementContext);
        } else if (node instanceof CustomUsageNode) {
            // 样式组使用
            processStyleGroupUsage((CustomUsageNode) node, elementContext);
        } else if (node instanceof TemplateUsageNode) {
            // 样式组模板使用
            processStyleTemplateUsage((TemplateUsageNode) node, elementContext);
        }
    }
    
    /**
     * 处理内联样式
     */
    private void processInlineStyle(StylePropertyNode property, ElementContext elementContext) {
        String name = property.getProperty();
        String value = getStringValue(property.getValue());
        
        // 检查是否有变量引用
        if (value.contains("(") && value.contains(")")) {
            value = resolveVariableReference(value);
        }
        
        elementContext.inlineStyles.put(name, value);
    }
    
    /**
     * 处理样式规则（类选择器、ID选择器等）
     */
    private void processStyleRule(StyleRuleNode rule, ElementContext elementContext) {
        StyleSelectorNode selectorNode = rule.getSelector();
        String selector = selectorNode != null ? selectorNode.getValue() : "";
        
        if (selector.startsWith(".")) {
            // 类选择器
            String className = selector.substring(1);
            elementContext.classNames.add(className);
            
            // 生成全局样式
            generateGlobalStyleRule(selector, rule);
        } else if (selector.startsWith("#")) {
            // ID选择器
            // 注意：ID已经在创建上下文时生成
            generateGlobalStyleRule(selector, rule);
        } else if (selector.startsWith("&")) {
            // 上下文推导
            String resolvedSelector = resolveContextSelector(selector, elementContext);
            generateGlobalStyleRule(resolvedSelector, rule);
        }
    }
    
    /**
     * 处理样式组使用
     */
    private void processStyleGroupUsage(CustomUsageNode usage, ElementContext elementContext) {
        String groupName = usage.getName();
        CustomDefinitionNode styleGroup = styleGroupManager.getCustomStyleGroup(groupName);
        
        if (styleGroup == null) {
            context.addError(new com.chtl.context.CompilationError(
                "未定义的样式组: " + groupName,
                com.chtl.context.CompilationError.ErrorType.UNDEFINED_REFERENCE
            ));
            return;
        }
        
        // 处理样式组特例化
        if (!usage.getChildren().isEmpty()) {
            processStyleGroupSpecialization(styleGroup, usage, elementContext);
        } else {
            // 直接应用样式组
            applyStyleGroup(styleGroup, elementContext);
        }
    }
    
    /**
     * 处理样式组模板使用
     */
    private void processStyleTemplateUsage(TemplateUsageNode usage, ElementContext elementContext) {
        String templateName = usage.getName();
        TemplateDefinitionNode template = styleGroupManager.getStyleTemplate(templateName);
        
        if (template == null) {
            context.addError(new com.chtl.context.CompilationError(
                "未定义的样式模板: " + templateName,
                com.chtl.context.CompilationError.ErrorType.UNDEFINED_REFERENCE
            ));
            return;
        }
        
        // 应用模板样式
        applyStyleTemplate(template, elementContext);
    }
    
    /**
     * 处理样式组特例化
     */
    private void processStyleGroupSpecialization(
            CustomDefinitionNode styleGroup, 
            CustomUsageNode usage, 
            ElementContext elementContext) {
        
        // 先应用基础样式组
        applyStyleGroup(styleGroup, elementContext);
        
        // 处理特例化操作
        for (CHTLASTNode child : usage.getChildren()) {
            if (child instanceof CustomOperationNode) {
                CustomOperationNode operation = (CustomOperationNode) child;
                
                if (operation.getOperationType() == CustomOperationNode.OperationType.DELETE) {
                    // 删除操作
                    processDeleteOperation(operation, elementContext);
                }
            } else if (child instanceof StylePropertyNode) {
                // 覆盖属性
                processInlineStyle((StylePropertyNode) child, elementContext);
            }
        }
    }
    
    /**
     * 处理删除操作
     */
    private void processDeleteOperation(CustomOperationNode operation, ElementContext elementContext) {
        for (String target : operation.getDeleteTargets()) {
            if (target.startsWith("@Style")) {
                // 删除样式组继承
                // TODO: 实现样式组继承的删除
            } else {
                // 删除属性
                elementContext.inlineStyles.remove(target);
            }
        }
    }
    
    /**
     * 应用样式组
     */
    private void applyStyleGroup(CustomDefinitionNode styleGroup, ElementContext elementContext) {
        for (CHTLASTNode child : styleGroup.getChildren()) {
            if (child instanceof StylePropertyNode) {
                // 无值属性需要从上下文获取值
                StylePropertyNode property = (StylePropertyNode) child;
                if (property.getValue() == null) {
                    // 从特例化上下文中查找值
                    // TODO: 实现无值属性的处理
                } else {
                    processInlineStyle(property, elementContext);
                }
            } else if (child instanceof CustomUsageNode) {
                // 样式组继承
                processStyleGroupUsage((CustomUsageNode) child, elementContext);
            }
        }
    }
    
    /**
     * 应用样式模板
     */
    private void applyStyleTemplate(TemplateDefinitionNode template, ElementContext elementContext) {
        for (CHTLASTNode child : template.getChildren()) {
            if (child instanceof StylePropertyNode) {
                processInlineStyle((StylePropertyNode) child, elementContext);
            } else if (child instanceof TemplateUsageNode) {
                // 模板继承
                processStyleTemplateUsage((TemplateUsageNode) child, elementContext);
            }
        }
    }
    
    /**
     * 解析变量引用
     */
    private String resolveVariableReference(String value) {
        // 处理 VarName(key = value) 格式
        // TODO: 实现变量引用解析
        return value;
    }
    
    /**
     * 解析上下文选择器
     */
    private String resolveContextSelector(String selector, ElementContext elementContext) {
        // 优先使用类名，其次使用ID
        String base = !elementContext.classNames.isEmpty() ? 
            "." + elementContext.classNames.iterator().next() : 
            "#" + elementContext.id;
            
        return selector.replace("&", base);
    }
    
    /**
     * 生成全局样式规则
     */
    private void generateGlobalStyleRule(String selector, StyleRuleNode rule) {
        globalStyles.append(selector).append(" {\n");
        
        for (CHTLASTNode child : rule.getChildren()) {
            if (child instanceof StylePropertyNode) {
                StylePropertyNode property = (StylePropertyNode) child;
                globalStyles.append("    ")
                    .append(property.getProperty())
                    .append(": ")
                    .append(getStringValue(property.getValue()))
                    .append(";\n");
            }
        }
        
        globalStyles.append("}\n\n");
    }
    
    /**
     * 应用样式到元素
     */
    private void applyStylesToElement(ElementContext elementContext) {
        ElementNode element = elementContext.element;
        
        // 设置类名
        if (!elementContext.classNames.isEmpty()) {
            element.setAttribute("class", String.join(" ", elementContext.classNames));
        }
        
        // 设置ID
        if (elementContext.id != null && !elementContext.id.isEmpty()) {
            element.setAttribute("id", elementContext.id);
        }
        
        // 设置内联样式
        if (!elementContext.inlineStyles.isEmpty()) {
            StringBuilder style = new StringBuilder();
            for (Map.Entry<String, String> entry : elementContext.inlineStyles.entrySet()) {
                if (style.length() > 0) {
                    style.append("; ");
                }
                style.append(entry.getKey()).append(": ").append(entry.getValue());
            }
            element.setAttribute("style", style.toString());
        }
    }
    
    /**
     * 生成自动ID
     */
    private String generateAutoId(ElementNode element) {
        // 根据元素标签名生成ID
        return element.getTagName() + "_" + (++autoIdCounter);
    }
    
    /**
     * 获取全局样式
     */
    public String getGlobalStyles() {
        return globalStyles.toString();
    }
    
    /**
     * 清空全局样式
     */
    public void clearGlobalStyles() {
        globalStyles.setLength(0);
    }
    
    /**
     * 获取样式组管理器
     */
    public StyleGroupManager getStyleGroupManager() {
        return styleGroupManager;
    }
    
    /**
     * 从AST节点获取字符串值
     */
    private String getStringValue(CHTLASTNode node) {
        if (node == null) return null;
        
        if (node instanceof StringLiteralNode) {
            return ((StringLiteralNode) node).getValue();
        } else if (node instanceof UnquotedLiteralNode) {
            return ((UnquotedLiteralNode) node).getValue();
        } else if (node instanceof NumberLiteralNode) {
            return ((NumberLiteralNode) node).getValue();
        }
        
        return node.toString();
    }
}