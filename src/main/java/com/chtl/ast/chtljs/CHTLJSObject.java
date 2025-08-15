package com.chtl.ast.chtljs;
import com.chtl.ast.NodeType;
import java.util.HashMap;
import java.util.Map;


import com.chtl.chtljs.ast.*;
import java.util.*;

public class CHTLJSObject extends AbstractCHTLJSASTNode {
    private Map<String, CHTLJSASTNode> properties = new HashMap<>();

    public CHTLJSObject() {
        super(CHTLJSASTNode.NodeType.OBJECT);
    }

    public void addProperty(String key, CHTLJSASTNode value) {
        properties.put(key, value);
    }

    public Map<String, CHTLJSASTNode> getProperties() {
        return properties;
    }
}
