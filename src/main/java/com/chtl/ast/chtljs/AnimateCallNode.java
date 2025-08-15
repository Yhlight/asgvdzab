package com.chtl.ast.chtljs;
import com.chtl.ast.NodeType;

/**
 * animate函数调用节点
 * 对应CHTL JS中的animate({...})
 */
public class AnimateCallNode extends AbstractCHTLJSASTNode {
    private AnimationConfigNode config;
    
    public AnimateCallNode() {
        super(NodeType.ANIMATE_CALL);
    }
    
    public AnimationConfigNode getConfig() {
        return config;
    }
    
    public void setConfig(AnimationConfigNode config) {
        this.config = config;
    }
    
    @Override
    public void accept(CHTLJSASTVisitor visitor) {
        visitor.visitAnimateCall(this);
    }
}