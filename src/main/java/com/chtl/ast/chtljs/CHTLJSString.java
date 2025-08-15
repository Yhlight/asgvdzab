package com.chtl.ast.chtljs;

import com.chtl.chtljs.ast.*;

public class CHTLJSString extends AbstractCHTLJSASTNode {
    private String value;

    public CHTLJSString(String value) {
        super(CHTLJSASTNode.NodeType.STRING);
        this.value = value;
    }

    public String getValue() {
        return value;
    }
}
