package com.chtl.ast.node;
import java.util.HashMap;import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.CHTLJSVisitor;
import java.util.Map;
import java.util.LinkedHashMap;

public class CHTLJSObjectNode extends AbstractCHTLASTNode {
    private Map<String, CHTLASTNode> properties;

    public CHTLJSObjectNode() {
        this.properties = new LinkedHashMap<>();
    }

    public Map<String, CHTLASTNode> getProperties() { return properties; }

    public void setProperty(String key, CHTLASTNode value) {
        properties.put(key, value);
    }

    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSObject(this);
        }
    }
}
