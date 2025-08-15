package com.chtl.cjmod;
import java.util.ArrayList;
import java.util.List;


import java.util.*;

/**
 * 默认DOM构建器实现
 * 生成JavaScript代码来操作DOM
 */
public class DefaultDOMBuilder implements CJmodInterface.DOMBuilder {
    
    private StringBuilder jsCode;
    private String currentSelector;
    private List<DefaultDOMBuilder> children;
    
    public DefaultDOMBuilder() {
        this.jsCode = new StringBuilder();
        this.children = new ArrayList<>();
    }
    
    @Override
    public CJmodInterface.DOMBuilder querySelector(String selector) {
        currentSelector = selector;
        jsCode.append("document.querySelector('").append(escapeJs(selector)).append("')");
        return this;
    }
    
    @Override
    public CJmodInterface.DOMBuilder getElementById(String id) {
        currentSelector = "#" + id;
        jsCode.append("document.getElementById('").append(escapeJs(id)).append("')");
        return this;
    }
    
    @Override
    public CJmodInterface.DOMBuilder getElementsByClassName(String className) {
        currentSelector = "." + className;
        jsCode.append("document.getElementsByClassName('").append(escapeJs(className)).append("')[0]");
        return this;
    }
    
    @Override
    public CJmodInterface.DOMBuilder getElementsByTagName(String tagName) {
        currentSelector = tagName;
        jsCode.append("document.getElementsByTagName('").append(escapeJs(tagName)).append("')[0]");
        return this;
    }
    
    @Override
    public CJmodInterface.DOMBuilder addEventListener(String event, String handler) {
        jsCode.append(".addEventListener('").append(escapeJs(event)).append("', ")
              .append(handler).append(")");
        return this;
    }
    
    @Override
    public CJmodInterface.DOMBuilder setAttribute(String name, String value) {
        jsCode.append(".setAttribute('").append(escapeJs(name)).append("', '")
              .append(escapeJs(value)).append("')");
        return this;
    }
    
    @Override
    public CJmodInterface.DOMBuilder setStyle(String property, String value) {
        jsCode.append(".style.").append(toCamelCase(property))
              .append(" = '").append(escapeJs(value)).append("'");
        return this;
    }
    
    @Override
    public CJmodInterface.DOMBuilder addClass(String className) {
        jsCode.append(".classList.add('").append(escapeJs(className)).append("')");
        return this;
    }
    
    @Override
    public CJmodInterface.DOMBuilder removeClass(String className) {
        jsCode.append(".classList.remove('").append(escapeJs(className)).append("')");
        return this;
    }
    
    @Override
    public CJmodInterface.DOMBuilder appendChild(CJmodInterface.DOMBuilder child) {
        if (child instanceof DefaultDOMBuilder) {
            DefaultDOMBuilder childBuilder = (DefaultDOMBuilder) child;
            children.add(childBuilder);
            jsCode.append(".appendChild(").append(childBuilder.build()).append(")");
        }
        return this;
    }
    
    @Override
    public CJmodInterface.DOMBuilder insertBefore(CJmodInterface.DOMBuilder child, 
                                                 CJmodInterface.DOMBuilder reference) {
        if (child instanceof DefaultDOMBuilder && reference instanceof DefaultDOMBuilder) {
            DefaultDOMBuilder childBuilder = (DefaultDOMBuilder) child;
            DefaultDOMBuilder refBuilder = (DefaultDOMBuilder) reference;
            jsCode.append(".insertBefore(").append(childBuilder.build())
                  .append(", ").append(refBuilder.build()).append(")");
        }
        return this;
    }
    
    @Override
    public CJmodInterface.DOMBuilder remove() {
        jsCode.append(".remove()");
        return this;
    }
    
    @Override
    public String build() {
        return jsCode.toString();
    }
    
    /**
     * 转义JavaScript字符串
     */
    private String escapeJs(String str) {
        if (str == null) return "";
        return str.replace("\\", "\\\\")
                  .replace("'", "\\'")
                  .replace("\"", "\\\"")
                  .replace("\n", "\\n")
                  .replace("\r", "\\r")
                  .replace("\t", "\\t");
    }
    
    /**
     * 将CSS属性名转换为camelCase
     */
    private String toCamelCase(String property) {
        if (property == null || property.isEmpty()) return property;
        
        StringBuilder result = new StringBuilder();
        boolean nextUpper = false;
        
        for (char c : property.toCharArray()) {
            if (c == '-') {
                nextUpper = true;
            } else {
                if (nextUpper) {
                    result.append(Character.toUpperCase(c));
                    nextUpper = false;
                } else {
                    result.append(c);
                }
            }
        }
        
        return result.toString();
    }
}