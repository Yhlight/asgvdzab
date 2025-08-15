package com.chtl.ast.node;import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.*;
import java.util.ArrayList;
import java.util.List;

/**
 * 样式规则节点
 * 对应CSS规则，如 .box { color: red; }
 */
public class StyleRuleNode extends AbstractCHTLASTNode {
    private StyleSelectorNode selector;
    private List<StylePropertyNode> properties;
    
    public StyleRuleNode(StyleSelectorNode selector) {
        super(NodeType.STYLE_RULE);
        this.selector = selector;
        this.properties = new ArrayList<>();
        if (selector != null) {
            selector.setParent(this);
        }
    }
    
    public StyleSelectorNode getSelector() {
        return selector;
    }
    
    public void setSelector(StyleSelectorNode selector) {
        this.selector = selector;
        if (selector != null) {
            selector.setParent(this);
        }
    }
    
    public List<StylePropertyNode> getProperties() {
        return properties;
    }
    
    public void addProperty(StylePropertyNode property) {
        if (property != null) {
            properties.add(property);
            property.setParent(this);
        }
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitStyleRule(this);
    }
}