package com.chtl.ast.node;

import com.chtl.ast.*;

/**
 * CHTL JS表达式节点基类
 * 所有CHTL JS特有的表达式都继承自此类
 */
public abstract class CHTLJSExpressionNode extends AbstractCHTLASTNode {
    
    public CHTLJSExpressionNode(NodeType nodeType) {
        super(nodeType);
    }
    
    /**
     * 是否是选择器表达式
     */
    public boolean isSelector() {
        return false;
    }
    
    /**
     * 是否是方法调用
     */
    public boolean isMethodCall() {
        return false;
    }
    
    /**
     * 是否是动画表达式
     */
    public boolean isAnimation() {
        return false;
    }
}