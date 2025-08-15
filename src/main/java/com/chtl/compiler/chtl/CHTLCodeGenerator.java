package com.chtl.compiler.chtl;

import com.chtl.model.AttributeNode;
import com.chtl.model.CHTLNode;
import com.chtl.model.CHTLNodeVisitor;
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
import java.util.UUID;

/**
 * CHTL代码生成器
 * 负责将AST转换为HTML代码
 */
public class CHTLCodeGenerator implements CHTLNodeVisitor {
    private static final Logger logger = LoggerFactory.getLogger(CHTLCodeGenerator.class);
    
    private StringBuilder htmlBuilder;
    private StringBuilder cssBuilder;
    private StringBuilder jsBuilder;
    private int indentLevel;
    
    public CHTLCodeGenerator() {
        this.htmlBuilder = new StringBuilder();
        this.cssBuilder = new StringBuilder();
        this.jsBuilder = new StringBuilder();
        this.indentLevel = 0;
    }
    
    /**
     * 生成HTML代码
     */
    public String generate(CHTLNode ast) {
        logger.debug("开始生成HTML代码");
        
        // 重置构建器
        htmlBuilder.setLength(0);
        cssBuilder.setLength(0);
        jsBuilder.setLength(0);
        indentLevel = 0;
        
        // 遍历AST生成代码
        ast.accept(this);
        
        return htmlBuilder.toString();
    }
    
    /**
     * 获取生成的CSS代码
     */
    public String getGeneratedCSS() {
        return cssBuilder.toString();
    }
    
    /**
     * 获取生成的JS代码
     */
    public String getGeneratedJS() {
        return jsBuilder.toString();
    }
    
    @Override
    public void visit(ElementNode node) {
        // 跳过根节点
        if ("root".equals(node.getName())) {
            for (CHTLNode child : node.getChildren()) {
                child.accept(this);
            }
            return;
        }
        
        // 生成开始标签
        appendIndent();
        htmlBuilder.append("<").append(node.getName());
        
        // 生成属性
        for (Map.Entry<String, String> attr : node.getAttributes().entrySet()) {
            htmlBuilder.append(" ")
                      .append(attr.getKey())
                      .append("=\"")
                      .append(escapeHtml(attr.getValue()))
                      .append("\"");
        }
        
        // 检查是否是自闭合标签
        if (isSelfClosingTag(node.getName()) && node.getChildren().isEmpty()) {
            htmlBuilder.append(" />\n");
        } else {
            htmlBuilder.append(">");
            
            // 如果有子节点，换行并增加缩进
            if (!node.getChildren().isEmpty()) {
                htmlBuilder.append("\n");
                indentLevel++;
                
                for (CHTLNode child : node.getChildren()) {
                    child.accept(this);
                }
                
                indentLevel--;
                appendIndent();
            }
            
            // 生成结束标签
            htmlBuilder.append("</").append(node.getName()).append(">\n");
        }
    }
    
    @Override
    public void visit(TextNode node) {
        String content = node.getContent().trim();
        if (!content.isEmpty()) {
            appendIndent();
            htmlBuilder.append(escapeHtml(content)).append("\n");
        }
    }
    
    @Override
    public void visit(AttributeNode node) {
        // 属性已在ElementNode中处理
    }
    
    @Override
    public void visit(StyleNode node) {
        if (node.isLocal()) {
            // 局部样式需要特殊处理
            processLocalStyle(node);
        } else {
            // 全局样式直接添加到CSS构建器
            for (StyleNode.StyleRule rule : node.getRules()) {
                cssBuilder.append(rule.getSelector()).append(" {\n");
                for (StyleNode.StyleProperty prop : rule.getProperties()) {
                    cssBuilder.append("  ")
                             .append(prop.getName())
                             .append(": ")
                             .append(prop.getValue())
                             .append(";\n");
                }
                cssBuilder.append("}\n\n");
            }
        }
    }
    
    @Override
    public void visit(ScriptNode node) {
        if (node.isLocal()) {
            // 局部脚本添加到JS构建器
            jsBuilder.append("// Local script\n");
            jsBuilder.append("(function() {\n");
            jsBuilder.append(node.getContent());
            jsBuilder.append("\n})();\n\n");
        } else {
            // 全局脚本
            appendIndent();
            htmlBuilder.append("<script>\n");
            indentLevel++;
            appendIndent();
            htmlBuilder.append(node.getContent());
            htmlBuilder.append("\n");
            indentLevel--;
            appendIndent();
            htmlBuilder.append("</script>\n");
        }
    }
    
    @Override
    public void visit(TemplateNode node) {
        // 模板在预处理阶段已经展开，这里不应该再遇到
        logger.warn("遇到未展开的模板节点: {}", node.getTemplateName());
    }
    
    @Override
    public void visit(CustomNode node) {
        // 自定义在预处理阶段已经展开，这里不应该再遇到
        logger.warn("遇到未展开的自定义节点: {}", node.getCustomName());
    }
    
    /**
     * 处理局部样式
     */
    private void processLocalStyle(StyleNode node) {
        // 为包含局部样式的元素生成唯一类名
        String uniqueClass = "chtl-" + UUID.randomUUID().toString().substring(0, 8);
        
        // 获取父元素并添加类名
        CHTLNode parent = node.getParent();
        if (parent instanceof ElementNode) {
            ElementNode element = (ElementNode) parent;
            String existingClass = element.getAttribute("class");
            if (existingClass != null && !existingClass.isEmpty()) {
                element.setAttribute("class", existingClass + " " + uniqueClass);
            } else {
                element.setAttribute("class", uniqueClass);
            }
        }
        
        // 处理样式规则
        for (StyleNode.StyleRule rule : node.getRules()) {
            if ("inline".equals(rule.getSelector())) {
                // 内联样式
                if (parent instanceof ElementNode) {
                    ElementNode element = (ElementNode) parent;
                    StringBuilder styleAttr = new StringBuilder();
                    
                    for (StyleNode.StyleProperty prop : rule.getProperties()) {
                        if (!"_raw".equals(prop.getName())) {
                            styleAttr.append(prop.getName())
                                    .append(": ")
                                    .append(prop.getValue())
                                    .append("; ");
                        }
                    }
                    
                    String existingStyle = element.getAttribute("style");
                    if (existingStyle != null && !existingStyle.isEmpty()) {
                        element.setAttribute("style", existingStyle + " " + styleAttr.toString());
                    } else {
                        element.setAttribute("style", styleAttr.toString());
                    }
                }
            } else {
                // 其他选择器添加到全局CSS
                String selector = rule.getSelector();
                if (selector.startsWith(".") || selector.startsWith("#")) {
                    // 类选择器或ID选择器
                    cssBuilder.append(selector).append(" {\n");
                } else if (selector.startsWith("&")) {
                    // 上下文推导选择器
                    selector = selector.replace("&", "." + uniqueClass);
                    cssBuilder.append(selector).append(" {\n");
                } else {
                    // 其他选择器，作用域限定
                    cssBuilder.append(".").append(uniqueClass).append(" ").append(selector).append(" {\n");
                }
                
                for (StyleNode.StyleProperty prop : rule.getProperties()) {
                    cssBuilder.append("  ")
                             .append(prop.getName())
                             .append(": ")
                             .append(prop.getValue())
                             .append(";\n");
                }
                cssBuilder.append("}\n\n");
            }
        }
    }
    
    /**
     * 添加缩进
     */
    private void appendIndent() {
        for (int i = 0; i < indentLevel; i++) {
            htmlBuilder.append("  ");
        }
    }
    
    /**
     * 转义HTML特殊字符
     */
    private String escapeHtml(String text) {
        return text.replace("&", "&amp;")
                   .replace("<", "&lt;")
                   .replace(">", "&gt;")
                   .replace("\"", "&quot;")
                   .replace("'", "&#39;");
    }
    
    /**
     * 检查是否是自闭合标签
     */
    private boolean isSelfClosingTag(String tagName) {
        return "area".equals(tagName) || "base".equals(tagName) || 
               "br".equals(tagName) || "col".equals(tagName) || 
               "embed".equals(tagName) || "hr".equals(tagName) || 
               "img".equals(tagName) || "input".equals(tagName) || 
               "link".equals(tagName) || "meta".equals(tagName) || 
               "param".equals(tagName) || "source".equals(tagName) || 
               "track".equals(tagName) || "wbr".equals(tagName);
    }
}