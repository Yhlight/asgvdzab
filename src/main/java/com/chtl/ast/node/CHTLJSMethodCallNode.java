package com.chtl.ast.node;

import com.chtl.ast.*;
import java.util.*;

/**
 * CHTL JS方法调用节点
 * 表示{{selector}}->method()语法
 */
public class CHTLJSMethodCallNode extends CHTLJSExpressionNode {
    private CHTLJSSelectorNode target;
    private String methodName;
    private List<CHTLASTNode> arguments;
    
    public CHTLJSMethodCallNode(CHTLJSSelectorNode target, String methodName) {
        super(NodeType.CHTLJS_METHOD_CALL);
        this.target = target;
        this.methodName = methodName;
        this.arguments = new ArrayList<>();
        
        if (target != null) {
            target.setParent(this);
        }
    }
    
    @Override
    public boolean isMethodCall() {
        return true;
    }
    
    public CHTLJSSelectorNode getTarget() {
        return target;
    }
    
    public void setTarget(CHTLJSSelectorNode target) {
        this.target = target;
        if (target != null) {
            target.setParent(this);
        }
    }
    
    public String getMethodName() {
        return methodName;
    }
    
    public void setMethodName(String methodName) {
        this.methodName = methodName;
    }
    
    public List<CHTLASTNode> getArguments() {
        return arguments;
    }
    
    public void addArgument(CHTLASTNode argument) {
        if (argument != null) {
            arguments.add(argument);
            argument.setParent(this);
        }
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        if (visitor instanceof CHTLJSVisitor) {
            ((CHTLJSVisitor) visitor).visitCHTLJSMethodCall(this);
        }
    }
}