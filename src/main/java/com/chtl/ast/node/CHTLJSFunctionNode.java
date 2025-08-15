package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.CHTLJSVisitor;
import java.util.List;
import java.util.ArrayList;

public class CHTLJSFunctionNode extends AbstractCHTLASTNode {
    private String name;
    private List<String> parameters;
    private CHTLASTNode body;

    public CHTLJSFunctionNode(String name) {
        super(NodeType.CHTL_JS_EXPRESSION);
        this.name = name;
        this.parameters = new ArrayList<>();
    }

    public String getName() { return name; }
    public List<String> getParameters() { return parameters; }
    public CHTLASTNode getBody() { return body; }

    public void addParameter(String param) {
        parameters.add(param);
    }

    public void setBody(CHTLASTNode body) {
        this.body = body;
    }

    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSFunction(this);
        }
    }
}
