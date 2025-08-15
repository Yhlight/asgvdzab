package com.chtl.ast.chtljs;

/**
 * 表达式节点
 * 对应CHTL JS中的一般JavaScript表达式
 */
public class ExpressionNode extends AbstractCHTLJSASTNode {
    private String expression; // 表达式内容
    
    public ExpressionNode(String expression) {
        super(NodeType.EXPRESSION);
        this.expression = expression;
    }
    
    public String getExpression() {
        return expression;
    }
    
    public void setExpression(String expression) {
        this.expression = expression;
    }
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitExpression(this);
    }
}