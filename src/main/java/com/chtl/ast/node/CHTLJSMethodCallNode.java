package com.chtl.ast.node;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.CHTLJSVisitor;
import java.util.List;
import java.util.ArrayList;

public class CHTLJSMethodCallNode extends AbstractCHTLASTNode {
    private String methodName;
    private List<CHTLASTNode> arguments;
    
    public CHTLJSMethodCallNode(String methodName) {
        super(NodeType.CHTL_JS_EXPRESSION);
        this.methodName = methodName;
        this.arguments = new ArrayList<>();
    }
    
    public String getMethodName() {
        return methodName;
    }
    
    public List<CHTLASTNode> getArguments() {
        return arguments;
    }
    
    public void addArgument(CHTLASTNode arg) {
        arguments.add(arg);
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSMethodCall(this);
        }
    }
}