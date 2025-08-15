package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.CHTLJSVisitor;
import java.util.Map;
import java.util.HashMap;

public class CHTLJSAnimateNode extends AbstractCHTLASTNode {
    private CHTLASTNode target;
    private Map<String, Object> properties;

    public CHTLJSAnimateNode() {
        super(NodeType.ANIMATION_CONTROLLER);
        this.properties = new HashMap<>();
    }

    public CHTLASTNode getTarget() { return target; }
    public void setTarget(CHTLASTNode target) { this.target = target; }

    public Map<String, Object> getProperties() { return properties; }
    public void setProperty(String key, Object value) {
        properties.put(key, value);
    }

    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSAnimate(this);
        }
    }
}
