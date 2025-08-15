package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;
import com.chtl.compiler.chtljs.selector.SelectorType;


import com.chtl.ast.*;

/**
 * 样式选择器节点
 * 对应CSS选择器，如 .box、#id、&:hover等
 */
public class StyleSelectorNode extends AbstractCHTLASTNode {
    
    public enum SelectorType {
        CLASS,          // .className
        ID,             // #idName
        TAG,            // tagName
        CONTEXT,        // & 上下文选择器
        PSEUDO_CLASS,   // :hover、:active等
        PSEUDO_ELEMENT, // ::before、::after等
        COMPOUND        // 复合选择器
    }
    
    private SelectorType selectorType;
    private String value;
    private String pseudoValue; // 用于存储伪类或伪元素的值
    
    public StyleSelectorNode(SelectorType selectorType, String value) {
        super(NodeType.STYLE_SELECTOR);
        this.selectorType = selectorType;
        this.value = value;
    }
    
    public SelectorType getSelectorType() {
        return selectorType;
    }
    
    public void setSelectorType(SelectorType selectorType) {
        this.selectorType = selectorType;
    }
    
    public String getValue() {
        return value;
    }
    
    public void setValue(String value) {
        this.value = value;
    }
    
    public String getPseudoValue() {
        return pseudoValue;
    }
    
    public void setPseudoValue(String pseudoValue) {
        this.pseudoValue = pseudoValue;
    }
    
    /**
     * 获取完整的选择器字符串
     */
    public String getFullSelector() {
        StringBuilder sb = new StringBuilder();
        
        switch (selectorType) {
            case CLASS:
                sb.append(".").append(value);
                break;
            case ID:
                sb.append("#").append(value);
                break;
            case TAG:
                sb.append(value);
                break;
            case CONTEXT:
                sb.append("&");
                if (pseudoValue != null) {
                    sb.append(pseudoValue);
                }
                break;
            case PSEUDO_CLASS:
                sb.append(":").append(value);
                break;
            case PSEUDO_ELEMENT:
                sb.append("::").append(value);
                break;
            case COMPOUND:
                sb.append(value);
                break;
        }
        
        return sb.toString();
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitStyleSelector(this);
    }
}