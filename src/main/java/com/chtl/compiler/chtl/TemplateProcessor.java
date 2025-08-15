package com.chtl.compiler.chtl;

import com.chtl.model.CHTLNode;
import com.chtl.model.CustomNode;
import com.chtl.model.ElementNode;
import com.chtl.model.ScriptNode;
import com.chtl.model.StyleNode;
import com.chtl.model.TemplateNode;
import com.chtl.model.TextNode;


import com.chtl.model.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Map;

/**
 * 模板处理器
 * 负责处理模板展开和继承
 */
public class TemplateProcessor {
    private static final Logger logger = LoggerFactory.getLogger(TemplateProcessor.class);
    
    private final Map<String, TemplateNode> templates;
    private final Map<String, CustomNode> customs;
    
    public TemplateProcessor(Map<String, TemplateNode> templates, Map<String, CustomNode> customs) {
        this.templates = templates;
        this.customs = customs;
    }
    
    /**
     * 处理AST中的模板展开
     */
    public void process(CHTLNode root) {
        logger.debug("开始处理模板展开");
        processNode(root);
    }
    
    private void processNode(CHTLNode node) {
        if (node instanceof ElementNode) {
            ElementNode element = (ElementNode) node;
            
            // 检查是否是模板引用
            if (isTemplateReference(element)) {
                expandTemplate(element);
            } else {
                // 递归处理子节点
                for (int i = 0; i < element.getChildren().size(); i++) {
                    CHTLNode child = element.getChildren().get(i);
                    processNode(child);
                }
            }
        }
    }
    
    /**
     * 检查元素是否是模板引用
     */
    private boolean isTemplateReference(ElementNode element) {
        String name = element.getName();
        return name.startsWith("@Style") || name.startsWith("@Element") || name.startsWith("@Var");
    }
    
    /**
     * 展开模板
     */
    private void expandTemplate(ElementNode element) {
        String refName = extractTemplateName(element.getName());
        
        // 查找对应的模板
        TemplateNode template = templates.get(refName);
        if (template == null) {
            // 尝试在自定义中查找
            CustomNode custom = customs.get(refName);
            if (custom != null) {
                expandCustom(element, custom);
                return;
            }
            
            logger.warn("未找到模板或自定义: {}", refName);
            return;
        }
        
        // 展开模板内容
        CHTLNode parent = element.getParent();
        if (parent instanceof ElementNode) {
            ElementNode parentElement = (ElementNode) parent;
            int index = parentElement.getChildren().indexOf(element);
            
            // 移除模板引用
            parentElement.getChildren().remove(index);
            
            // 插入模板内容
            for (CHTLNode child : template.getChildren()) {
                CHTLNode clonedChild = cloneNode(child);
                parentElement.getChildren().add(index++, clonedChild);
                clonedChild.setParent(parentElement);
            }
        }
    }
    
    /**
     * 展开自定义
     */
    private void expandCustom(ElementNode element, CustomNode custom) {
        // 类似模板展开，但需要处理特例化
        CHTLNode parent = element.getParent();
        if (parent instanceof ElementNode) {
            ElementNode parentElement = (ElementNode) parent;
            int index = parentElement.getChildren().indexOf(element);
            
            // 移除自定义引用
            parentElement.getChildren().remove(index);
            
            // 插入自定义内容
            for (CHTLNode child : custom.getChildren()) {
                CHTLNode clonedChild = cloneNode(child);
                
                // 处理特例化（如果element有子节点，可能包含特例化信息）
                if (!element.getChildren().isEmpty()) {
                    applySpecialization(clonedChild, element);
                }
                
                parentElement.getChildren().add(index++, clonedChild);
                clonedChild.setParent(parentElement);
            }
        }
    }
    
    /**
     * 提取模板名称
     */
    private String extractTemplateName(String fullName) {
        // 移除@Style、@Element、@Var前缀
        if (fullName.startsWith("@")) {
            int spaceIndex = fullName.indexOf(' ');
            if (spaceIndex > 0) {
                return fullName.substring(spaceIndex + 1);
            }
        }
        return fullName;
    }
    
    /**
     * 克隆节点
     */
    private CHTLNode cloneNode(CHTLNode node) {
        if (node instanceof ElementNode) {
            ElementNode original = (ElementNode) node;
            ElementNode clone = new ElementNode(original.getName());
            
            // 复制属性
            for (Map.Entry<String, String> attr : original.getAttributes().entrySet()) {
                clone.setAttribute(attr.getKey(), attr.getValue());
            }
            
            // 递归克隆子节点
            for (CHTLNode child : original.getChildren()) {
                clone.addChild(cloneNode(child));
            }
            
            return clone;
        } else if (node instanceof TextNode) {
            TextNode original = (TextNode) node;
            return new TextNode(original.getContent());
        } else if (node instanceof StyleNode) {
            StyleNode original = (StyleNode) node;
            StyleNode clone = new StyleNode(original.isLocal());
            
            // 复制样式规则
            for (StyleNode.StyleRule rule : original.getRules()) {
                StyleNode.StyleRule clonedRule = new StyleNode.StyleRule(rule.getSelector());
                for (StyleNode.StyleProperty prop : rule.getProperties()) {
                    clonedRule.addProperty(new StyleNode.StyleProperty(prop.getName(), prop.getValue()));
                }
                clone.addRule(clonedRule);
            }
            
            return clone;
        } else if (node instanceof ScriptNode) {
            ScriptNode original = (ScriptNode) node;
            return new ScriptNode(original.getContent(), original.isLocal());
        }
        
        // 其他类型暂不支持克隆
        return node;
    }
    
    /**
     * 应用特例化
     */
    private void applySpecialization(CHTLNode target, ElementNode specialization) {
        // 这里简化处理，实际应该根据特例化语法进行处理
        // 例如：添加样式、删除元素等
        logger.debug("应用特例化到节点");
    }
}