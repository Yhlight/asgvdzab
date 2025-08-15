package com.chtl.ast.node;

import com.chtl.ast.*;
import java.util.ArrayList;
import java.util.List;

/**
 * 样式块节点
 * 对应CHTL中的style { }块
 */
public class StyleBlockNode extends AbstractCHTLASTNode {
    private boolean isLocal; // true表示局部样式块，false表示全局样式块
    private List<StyleRuleNode> rules;
    private List<StylePropertyNode> inlineStyles; // 内联样式属性
    
    public StyleBlockNode(boolean isLocal) {
        super(NodeType.STYLE_BLOCK);
        this.isLocal = isLocal;
        this.rules = new ArrayList<>();
        this.inlineStyles = new ArrayList<>();
    }
    
    public boolean isLocal() {
        return isLocal;
    }
    
    public void setLocal(boolean local) {
        isLocal = local;
    }
    
    public List<StyleRuleNode> getRules() {
        return rules;
    }
    
    public void addRule(StyleRuleNode rule) {
        if (rule != null) {
            rules.add(rule);
            rule.setParent(this);
        }
    }
    
    public List<StylePropertyNode> getInlineStyles() {
        return inlineStyles;
    }
    
    public void addInlineStyle(StylePropertyNode property) {
        if (property != null) {
            inlineStyles.add(property);
            property.setParent(this);
        }
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitStyleBlock(this);
    }
}