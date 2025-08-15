package com.chtl.ast.node;

import com.chtl.ast.*;
import com.chtl.compiler.chtljs.selector.SelectorType;

/**
 * CHTL JS选择器节点
 * 表示{{selector}}语法
 */
public class CHTLJSSelectorNode extends CHTLJSExpressionNode {
    private SelectorType selectorType;
    private String selector;
    private Integer index; // 用于索引访问，如{{button[0]}}
    
    public CHTLJSSelectorNode(SelectorType selectorType, String selector) {
        super(NodeType.CHTLJS_SELECTOR);
        this.selectorType = selectorType;
        this.selector = selector;
    }
    
    @Override
    public boolean isSelector() {
        return true;
    }
    
    public SelectorType getSelectorType() {
        return selectorType;
    }
    
    public void setSelectorType(SelectorType selectorType) {
        this.selectorType = selectorType;
    }
    
    public String getSelector() {
        return selector;
    }
    
    public void setSelector(String selector) {
        this.selector = selector;
    }
    
    public Integer getIndex() {
        return index;
    }
    
    public void setIndex(Integer index) {
        this.index = index;
    }
    
    public boolean hasIndex() {
        return index != null;
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSSelector(this);
        }
    }
    
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("{{");
        switch (selectorType) {
            case CLASS:
                sb.append(".").append(selector);
                break;
            case ID:
                sb.append("#").append(selector);
                break;
            default:
                sb.append(selector);
        }
        if (hasIndex()) {
            sb.append("[").append(index).append("]");
        }
        sb.append("}}");
        return sb.toString();
    }
}