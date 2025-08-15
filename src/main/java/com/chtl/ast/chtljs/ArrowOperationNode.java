package com.chtl.ast.chtljs;
import com.chtl.ast.NodeType;

/**
 * 箭头操作节点
 * 对应CHTL JS中的->操作符
 */
public class ArrowOperationNode extends AbstractCHTLJSASTNode {
    private CHTLJSASTNode left;  // 左侧操作数
    private CHTLJSASTNode right; // 右侧操作数
    
    public ArrowOperationNode() {
        super(NodeType.ARROW_OPERATION);
    }
    
    public CHTLJSASTNode getLeft() {
        return left;
    }
    
    public void setLeft(CHTLJSASTNode left) {
        this.left = left;
    }
    
    public CHTLJSASTNode getRight() {
        return right;
    }
    
    public void setRight(CHTLJSASTNode right) {
        this.right = right;
    }
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitArrowOperation(this);
    }
}