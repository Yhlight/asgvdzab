package com.chtl.ast.node;

import com.chtl.ast.*;
import java.util.*;

/**
 * CHTL JS对象节点
 * 表示对象字面量 { key: value }
 */
public class CHTLJSObjectNode extends CHTLJSExpressionNode {
    private Map<String, CHTLASTNode> properties;
    
    public CHTLJSObjectNode() {
        super(NodeType.CHTLJS_OBJECT);
        this.properties = new LinkedHashMap<>();
    }
    
    public Map<String, CHTLASTNode> getProperties() {
        return properties;
    }
    
    public void addProperty(String key, CHTLASTNode value) {
        if (value != null) {
            properties.put(key, value);
            value.setParent(this);
        }
    }
    
    public CHTLASTNode getProperty(String key) {
        return properties.get(key);
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSObject(this);
        }
    }
}