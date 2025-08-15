package com.chtl.ast.chtljs;

import com.chtl.compiler.chtljs.selector.SelectorType;


/**
 * 增强选择器节点
 * 对应CHTL JS中的{{...}}
 */
public class EnhancedSelectorNode extends AbstractCHTLJSASTNode {
    
    public enum SelectorType {
        TAG,        // {{box}} - 标签或标识符
        CLASS,      // {{.box}} - 类选择器
        ID,         // {{#box}} - ID选择器
        INDEX,      // {{button[0]}} - 索引访问
        DESCENDANT  // {{.box button}} - 后代选择器
    }
    
    private SelectorType selectorType;
    private String selector; // 选择器内容（不含{{}}）
    private String tagName;  // 标签名（对于INDEX类型）
    private int index;       // 索引值（对于INDEX类型）
    private String parentSelector; // 父选择器（对于DESCENDANT类型）
    private String childSelector;  // 子选择器（对于DESCENDANT类型）
    
    public EnhancedSelectorNode(String selector) {
        super(NodeType.ENHANCED_SELECTOR);
        this.selector = selector;
        parseSelector();
    }
    
    private void parseSelector() {
        if (selector.startsWith(".")) {
            selectorType = SelectorType.CLASS;
        } else if (selector.startsWith("#")) {
            selectorType = SelectorType.ID;
        } else if (selector.contains("[") && selector.contains("]")) {
            selectorType = SelectorType.INDEX;
            int bracketIndex = selector.indexOf('[');
            tagName = selector.substring(0, bracketIndex);
            String indexStr = selector.substring(bracketIndex + 1, selector.indexOf(']'));
            try {
                index = Integer.parseInt(indexStr);
            } catch (NumberFormatException e) {
                index = 0;
            }
        } else if (selector.contains(" ")) {
            selectorType = SelectorType.DESCENDANT;
            int spaceIndex = selector.indexOf(' ');
            parentSelector = selector.substring(0, spaceIndex);
            childSelector = selector.substring(spaceIndex + 1).trim();
        } else {
            selectorType = SelectorType.TAG;
        }
    }
    
    public SelectorType getSelectorType() {
        return selectorType;
    }
    
    public String getSelector() {
        return selector;
    }
    
    public void setSelector(String selector) {
        this.selector = selector;
        parseSelector();
    }
    
    public String getTagName() {
        return tagName;
    }
    
    public int getIndex() {
        return index;
    }
    
    public String getParentSelector() {
        return parentSelector;
    }
    
    public String getChildSelector() {
        return childSelector;
    }
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitEnhancedSelector(this);
    }
}